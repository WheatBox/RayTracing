#pragma once

// A * x * x + B * x + C = 0
// Return the count of results
// If 0, outX1 = outX2 = 0
// If 1, outX1 = outX2 = x
// If 2, outX1 = x1, outX2 = x2
int SolveQuadraticEquation(float * outX1, float * outX2, float A, float B, float C);
