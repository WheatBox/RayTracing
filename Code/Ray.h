#pragma once

#include "BasicTypes.h"

#include "Model.h"

bool Ray_IntersectSphere(Vec3 * outContactPos, Vec3 * outVNormal, const Sphere * pSphere, Vec3 origin, Vec3 dir);
bool Ray_IntersectTriangle(Vec3 * outContactPos, Vec3 * outVNormal, const Triangle * pTriangle, Vec3 rayPos, Vec3 rayDir);

static inline bool Ray_IntersectModel(Vec3 * outContactPos, Vec3 * outVNormal, const Model * pModel, Vec3 rayPos, Vec3 rayDir) {
	switch(pModel->type) {
	case eMFT_Sphere:
		return Ray_IntersectSphere(outContactPos, outVNormal, & pModel->data.sphere, rayPos, rayDir);
	case eMFT_Triangle:
		return Ray_IntersectTriangle(outContactPos, outVNormal, & pModel->data.triangle, rayPos, rayDir);
	}
	return false;
}

Model * Ray_IntersectGetModel(Vec3 * outContactPos, Vec3 * outVNormal, Vec3 rayPos, Vec3 rayDir, Model * models, size_t modelCount, const Model * pRayFromModel);

Color Ray_Trace(float * outEmissionPower, int times, Vec3 rayPos, Vec3 rayDir, Model * models, size_t modelCount, const Model * pRayFromModel);