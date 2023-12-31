#include "Ray.h"
#include "Math.h"
#include "Equation.h"

#include <stdlib.h>

bool Ray_IntersectSphere(Vec3 * outContactPos, Vec3 * outVNormal, const Sphere * pSphere, Vec3 origin, Vec3 dir) {

	Vec3 vOC = Vec3_Minus(pSphere->center, origin);
	if(Vec3_Length(vOC) <= pSphere->radius) {
		goto OutputAndOver;
	}

	if(Vec3_Dot(dir, vOC) <= 0.f) {
		return false;
	}

	float distanceToRay = Vec3_Length(Vec3_Cross(dir, vOC)) / Vec3_Length(dir);
	if(distanceToRay <= pSphere->radius) {
		goto OutputAndOver;
	}
	return false;

OutputAndOver: {}

	Vec3 vCO = Vec3_Minus(origin, pSphere->center);

	float t1, t2;
	float A = dir.x * dir.x + dir.y * dir.y + dir.z * dir.z;
	float B = 2 * dir.x * vCO.x + 2 * dir.y * vCO.y + 2 * dir.z * vCO.z;
	float C = vCO.x * vCO.x + vCO.y * vCO.y + vCO.z * vCO.z - pSphere->radius * pSphere->radius;

	int count = SolveQuadraticEquation(& t1, & t2, A, B, C);

	Vec3 contactPos;

	if(count == 0) {
		return false;
	}

	Vec3 vTemp1 = {
		origin.x + dir.x * t1,
		origin.y + dir.y * t1,
		origin.z + dir.z * t1
	};

	if(count == 1) {
		contactPos = vTemp1;
		goto ReturnVNormalAndTrue;
	}

	Vec3 vTemp2 = {
		origin.x + dir.x * t2,
		origin.y + dir.y * t2,
		origin.z + dir.z * t2
	};

	if(Vec3_Distance(vTemp1, origin) < Vec3_Distance(vTemp2, origin)) {
		contactPos = vTemp1;
	} else {
		contactPos = vTemp2;
	}

ReturnVNormalAndTrue:
	if(outContactPos) * outContactPos = contactPos;
	if(outVNormal) * outVNormal = Vec3_Minus(contactPos, pSphere->center);
	return true;
}

bool Ray_IntersectTriangle(Vec3 * outContactPos, Vec3 * outVNormal, const Triangle * pTriangle, Vec3 rayPos, Vec3 rayDir) {
	// Copied and modified from
	// https://github.com/walbourn/directx-sdk-legacy-samples/blob/main/Direct3D/Pick/Pick.cpp

	Vec3 edge1 = Vec3_Minus(pTriangle->v1, pTriangle->v0);
	Vec3 edge2 = Vec3_Minus(pTriangle->v2, pTriangle->v0);

	Vec3 pvec = Vec3_Cross(rayDir, edge2);

	float det = Vec3_Dot(edge1, pvec);

	Vec3 tvec;
	if(det > 0) {
		tvec = Vec3_Minus(rayPos, pTriangle->v0);
	} else {
		tvec = Vec3_Minus(pTriangle->v0, rayPos);
		det = -det;
	}

	if(det < 0.0001f) {
		return false;
	}

	float u = Vec3_Dot(tvec, pvec);
	if(u < 0.f || u > det) {
		return false;
	}

	Vec3 qvec = Vec3_Cross(tvec, edge1);

	float v = Vec3_Dot(rayDir, qvec);
	if(v < 0.f || u + v > det) {
		return false;
	}

	//float t = Vec3_Dot(edge2, qvec);
	float fInvDet = 1.f / det;

	//t *= fInvDet;
	u *= fInvDet;
	v *= fInvDet;

	Vec3 contactPos;
	contactPos = Vec3_MultiVal(pTriangle->v0, 1.f - u - v);
	contactPos = Vec3_Add(contactPos, Vec3_MultiVal(pTriangle->v1, u));
	contactPos = Vec3_Add(contactPos, Vec3_MultiVal(pTriangle->v2, v));

	Vec3 vRC = Vec3_Minus(contactPos, rayPos);
	if(Vec3_Dot(vRC, rayDir) <= 0) {
		return false;
	}

	if(outContactPos) * outContactPos = contactPos;
	if(outVNormal) {
		* outVNormal = Vec3_Cross(
			Vec3_Minus(pTriangle->v1, pTriangle->v0),
			Vec3_Minus(pTriangle->v2, pTriangle->v0)
		);
	}
	
	return true;
}

Model * Ray_IntersectGetModel(Vec3 * outContactPos, Vec3 * outVNormal, Vec3 rayPos, Vec3 rayDir, Model * models, size_t modelCount, const Model * pRayFromModel) {
	rayDir = Vec3_Normalize(rayDir);

	struct _NearData {
		Vec3 contactPos;
		Vec3 vNormal;
		Model * pModel;
	} nearData = { 0 };
	float nearDis = 999999.f;

	for(size_t i = 0; i < modelCount; i++) {
		if(& models[i] == pRayFromModel) {
			continue;
		}

		struct _NearData nearTemp = { 0 };
		if(Ray_IntersectModel(& nearTemp.contactPos, & nearTemp.vNormal, & models[i], rayPos, rayDir)) {
			float disTemp = Vec3_Distance(nearTemp.contactPos, rayPos);
			if(disTemp < nearDis) {
				nearTemp.pModel = & models[i];
				nearData = nearTemp;
				nearDis = disTemp;
			}
		}
	}

	if(nearData.pModel != NULL) {
		if(outContactPos) * outContactPos = nearData.contactPos;
		if(outVNormal) * outVNormal = Vec3_Normalize(nearData.vNormal);
	}

	return nearData.pModel;
}

Color Ray_Trace(float * outEmissionPower, int times, Vec3 rayPos, Vec3 rayDir, Model * models, size_t modelCount, const Model * pRayFromModel) {
	rayDir = Vec3_Normalize(rayDir);

	Color color = { 0 };

	if(times < 0) {
		if(outEmissionPower) * outEmissionPower = 0.f;
		return color;
	}

	Vec3 contactPos = { 0 };
	Vec3 contactVNormal = { 0 };
	Model * contactModel = Ray_IntersectGetModel(& contactPos, & contactVNormal, rayPos, rayDir, models, modelCount, pRayFromModel);

	if(!contactModel) {
		if(outEmissionPower) * outEmissionPower = 0.f;
		return color;
	}
	
	const Material * contactMaterial = Model_GetMaterial(contactModel);

	Vec3 vReflectionDir = Vec3_RandomDirection();
	if(Vec3_Dot(vReflectionDir, contactVNormal) < 0) {
		vReflectionDir = Vec3_MultiVal(vReflectionDir, -1.f);
	}
	vReflectionDir = Vec3_Lerp(vReflectionDir, Vec3_Reflect(rayDir, contactVNormal), Model_GetMaterial(contactModel)->specular);

	float reflectionEmissionPower = 0.f;
	Color reflectionColor = Ray_Trace(& reflectionEmissionPower, times - 1, contactPos, vReflectionDir, models, modelCount, contactModel);

	reflectionEmissionPower *= Vec3_Dot(contactVNormal, vReflectionDir);

	if(outEmissionPower) * outEmissionPower = contactMaterial->emission.power + reflectionEmissionPower;
	color = Color_Multi(reflectionColor, contactMaterial->color);
	color = Color_Add(color, contactMaterial->emission.color);
	return color;
}