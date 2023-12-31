#pragma once

#include "BasicTypes.h"

typedef struct Color {
	uint8 r, g, b;
} Color;

Color Color_Mix(Color col1, Color col2, float rate);

Color Color_Multi(Color col1, Color col2);

Color Color_MultiVal(Color col1, float val);

Color Color_Add(Color col1, Color col2);

Color Color_Minus(Color col1, Color col2);
