#include "Color.h"

#include "Math.h"

Color Color_Mix(Color col1, Color col2, float rate) {
	Color col = {
		(uint8)Lerp(col1.r, col2.r, rate),
		(uint8)Lerp(col1.g, col2.g, rate),
		(uint8)Lerp(col1.b, col2.b, rate),
	};
	return col;
}

Color Color_Multi(Color col1, Color col2) {
	Color col = {
		(uint8)Clamp((int)col1.r * col2.r / 255, 0, 255),
		(uint8)Clamp((int)col1.g * col2.g / 255, 0, 255),
		(uint8)Clamp((int)col1.b * col2.b / 255, 0, 255),
	};
	return col;
}

Color Color_MultiVal(Color col1, float val) {
	Color col = {
		(uint8)Clamp_f((float)col1.r * val, 0, 255),
		(uint8)Clamp_f((float)col1.g * val, 0, 255),
		(uint8)Clamp_f((float)col1.b * val, 0, 255),
	};
	return col;
}

Color Color_Add(Color col1, Color col2) {
	Color col = {
		(uint8)Min((int)col1.r + col2.r, 255),
		(uint8)Min((int)col1.g + col2.g, 255),
		(uint8)Min((int)col1.b + col2.b, 255),
	};
	return col;
}

Color Color_Minus(Color col1, Color col2) {
	Color col = {
		(uint8)Max((int)col1.r - col2.r, 0),
		(uint8)Max((int)col1.g - col2.g, 0),
		(uint8)Max((int)col1.b - col2.b, 0),
	};
	return col;
}
