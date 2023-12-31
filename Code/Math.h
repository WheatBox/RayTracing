#pragma once

#define MAX(n1, n2) (((n1) > (n2)) ? (n1) : (n2))
#define MIN(n1, n2) (((n1) < (n2)) ? (n1) : (n2))
#define LERP(n1, n2, rate) (((n2) - (n1)) * (rate) + (n1))
#define CLAMP(val, n1, n2) \
	((n1) < (n2)) \
	? (((val) < (n1)) ? (n1) : (((val) > (n2)) ? (n2) : (val))) \
	: (((val) > (n1)) ? (n1) : (((val) < (n2)) ? (n2) : (val)))
#define ABS(val) ((val) > 0 ? (val) : -(val))

static inline int Max(int n1, int n2) {
	return MAX(n1, n2);
}
static inline float Max_f(float n1, float n2) {
	return MAX(n1, n2);
}

static inline int Min(int n1, int n2) {
	return MIN(n1, n2);
}
static inline float Min_f(float n1, float n2) {
	return MIN(n1, n2);
}

static inline int Lerp(int n1, int n2, float rate) {
	return (int)LERP(n1, n2, rate);
}
static inline float Lerp_f(float n1, float n2, float rate) {
	return LERP(n1, n2, rate);
}

static inline int Clamp(int val, int n1, int n2) {
	return CLAMP(val, n1, n2);
}
static inline float Clamp_f(float val, float n1, float n2) {
	return CLAMP(val, n1, n2);
}

static inline int Abs(int val) {
	return ABS(val);
}
static inline float Abs_f(float val) {
	return ABS(val);
}

#include <math.h>