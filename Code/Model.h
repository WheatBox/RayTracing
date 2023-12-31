#pragma once

#include "BasicTypes.h"

#include "Vec3.h"
#include "Material.h"

typedef enum EModelType {
	eMFT_Sphere,
	eMFT_Triangle,
} EModelType;

typedef struct Sphere {
	Vec3 center;
	float radius;
	Material material;
} Sphere;

typedef struct Triangle {
	// Vertices
	Vec3 v0, v1, v2;
	Material material;
} Triangle;

typedef struct Model {
	EModelType type;
	union {
		Sphere sphere;
		Triangle triangle;
	} data;
} Model;

static inline const Material * Model_GetMaterial(const Model * pModel) {
	switch(pModel->type) {
	case eMFT_Sphere:
		return & pModel->data.sphere.material;
	case eMFT_Triangle:
		return & pModel->data.triangle.material;
	}
	return NULL;
}
