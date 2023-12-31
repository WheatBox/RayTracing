#pragma once

#include "Color.h"

typedef struct Emission {
	Color color;
	float power;
} Emission;

typedef struct Material {
	Color color;
	float specular;

	Emission emission;
} Material;
