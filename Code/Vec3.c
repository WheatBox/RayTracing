#include "Vec3.h"

#include "Math.h"

Vec3 Vec3_Add(Vec3 v1, Vec3 v2) {
	Vec3 v = {
		v1.x + v2.x,
		v1.y + v2.y,
		v1.z + v2.z
	};
	return v;
}

Vec3 Vec3_Minus(Vec3 v1, Vec3 v2) {
	Vec3 v = {
		v1.x - v2.x,
		v1.y - v2.y,
		v1.z - v2.z
	};
	return v;
}

Vec3 Vec3_Multi(Vec3 v1, Vec3 v2) {
	Vec3 v = {
		v1.x * v2.x,
		v1.y * v2.y,
		v1.z * v2.z
	};
	return v;
}

Vec3 Vec3_MultiVal(Vec3 v1, float val) {
	Vec3 v = {
		v1.x * val,
		v1.y * val,
		v1.z * val
	};
	return v;
}

float Vec3_Dot(Vec3 v1, Vec3 v2) {
	return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

Vec3 Vec3_Cross(Vec3 v1, Vec3 v2) {
	Vec3 v = {
		v1.y * v2.z - v1.z * v2.y,
		v1.z * v2.x - v1.x * v2.z,
		v1.x * v2.y - v1.y * v2.x,
	};
	return v;
}

float Vec3_Length(Vec3 v) {
	return sqrtf(v.x * v.x + v.y * v.y + v.z * v.z);
}

float Vec3_Distance(Vec3 v1, Vec3 v2) {
	return Vec3_Length(Vec3_Minus(v1, v2));
}

Vec3 Vec3_Normalize(Vec3 v) {
	return Vec3_MultiVal(v, 1.f / Vec3_Length(v));
}

Vec3 Vec3_Reflect(Vec3 vIn, Vec3 vNormal) {
	vNormal = Vec3_Normalize(vNormal);
	Vec3 vOut = Vec3_Minus(vIn, Vec3_MultiVal(vNormal, Vec3_Dot(vIn, vNormal) * 2.f));
	return vOut;
}

#include <stdlib.h> // For rand() and RAND_MAX
static inline float GaussRand() {
	return sqrtf(-2.f * logf((float)rand() / (float)RAND_MAX)) * cosf(2.f * 3.1415926f * ((float)rand() / (float)RAND_MAX));
}

Vec3 Vec3_RandomDirection() {
	Vec3 v = { GaussRand(), GaussRand(), GaussRand() };
	return Vec3_Normalize(v);
}

Vec3 Vec3_Lerp(Vec3 v1, Vec3 v2, float rate) {
	Vec3 v = {
		Lerp_f(v1.x, v2.x, rate),
		Lerp_f(v1.y, v2.y, rate),
		Lerp_f(v1.z, v2.z, rate)
	};
	return v;
}
