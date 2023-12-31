#pragma once

typedef struct Vec3 {
	float x, y, z;
} Vec3;

Vec3 Vec3_Add(Vec3 v1, Vec3 v2);

Vec3 Vec3_Minus(Vec3 v1, Vec3 v2);

Vec3 Vec3_Multi(Vec3 v1, Vec3 v2);

Vec3 Vec3_MultiVal(Vec3 v1, float val);

float Vec3_Dot(Vec3 v1, Vec3 v2);

Vec3 Vec3_Cross(Vec3 v1, Vec3 v2);

float Vec3_Length(Vec3 v);

float Vec3_Distance(Vec3 v1, Vec3 v2);

Vec3 Vec3_Normalize(Vec3 v);

Vec3 Vec3_Reflect(Vec3 vIn, Vec3 vNormal);

Vec3 Vec3_RandomDirection();

Vec3 Vec3_Lerp(Vec3 v1, Vec3 v2, float rate);
