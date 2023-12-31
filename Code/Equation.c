#include "Equation.h"

#include <math.h>

int SolveQuadraticEquation(float * outX1, float * outX2, float A, float B, float C) {
	float delta = B * B - 4 * A * C;
	if(delta < 0) {
		* outX1 = * outX2 = 0.f;
		return 0;
	}

	if(delta == 0) {
		* outX1 = * outX2 = (sqrtf(delta) - B) / (2 * A);
		return 1;
	}

	* outX1 = (sqrtf(delta) - B) / (2 * A);
	* outX2 = (-sqrtf(delta) - B) / (2 * A);
	return 2;
}
