/*
 * linmath_additional.h
 *
 *  Created on: Aug 7, 2020
 *      Author: dizcza
 */

#ifndef LINMATH_H_LINMATH_ADDITIONAL_H_
#define LINMATH_H_LINMATH_ADDITIONAL_H_

#include <stdint.h>
#include "linmath.h"

#define LINMATH_MIN(x,y)                  ((x) < (y) ? (x) : (y))
#define LINMATH_MAX(x,y)                  ((x) > (y) ? (x) : (y))
#define LINMATH_DEGREES_TO_RADS(degrees)  (M_PI * (degrees) / 180.f)


#define LINMATH_ADDITIONAL_MACROS_VEC(n) \
LINMATH_H_FUNC void vec##n##_set(vec##n v, float value) { \
	int i; \
	for (i = 0; i < n; ++i) { \
		v[i] = value; \
	} \
}

LINMATH_ADDITIONAL_MACROS_VEC(2)
LINMATH_ADDITIONAL_MACROS_VEC(3)
LINMATH_ADDITIONAL_MACROS_VEC(4)


LINMATH_H_FUNC int vec3_is_nonnegative(vec3 const v) {
	int i, res = 1;
	for (i = 0; i < 3; ++i) {
		res &= v[i] >= 0;
	}
	return res;
}


LINMATH_H_FUNC void vec4_init(vec4 r, float x, float y, float z, float w) {
	r[0] = x;
	r[1] = y;
	r[2] = z;
	r[3] = w;
}



typedef int16_t vec2int16[2];
typedef uint32_t vec3uint32[3];

LINMATH_H_FUNC void vec2int16_floor(vec2int16 vint, vec2 const vf) {
	int i;
	for (i = 0; i < 2; ++i) {
		vint[i] = (int16_t) floorf(vf[i]);
	}
}

LINMATH_H_FUNC void vec2int16_ceil(vec2int16 vint, vec2 const vf) {
	int i;
	for (i = 0; i < 2; ++i) {
		vint[i] = (int16_t) ceilf(vf[i]);
	}
}

LINMATH_H_FUNC void vec3uint32_from_vec3(vec3uint32 vint, vec3 const vf) {
	int i;
	for (i = 0; i < 3; ++i) {
		vint[i] = (uint32_t) vf[i];
	}
}


/**
 * Matrices are stored in column order.
 */
typedef vec3 mat3x3[3];

LINMATH_H_FUNC void mat3x3_mul_vec3(vec3 r, mat3x3 const M, vec3 const v) {
	int i, j;
	for (j = 0; j < 3; ++j) {
		r[j] = 0.f;
		for (i = 0; i < 3; ++i)
			r[j] += M[i][j] * v[i];
	}
}

LINMATH_H_FUNC void mat3x3_set_col(mat3x3 M, vec3 const v, int col_index) {
	int i;
	for (i = 0; i < 3; ++i) {
		M[col_index][i] = v[i];
	}
}


LINMATH_H_FUNC void mat4x4_translate_gl(mat4x4 M, vec3 const v_shift) {
	int i;
	for (i = 0; i < 3; i++) {
		M[3][i] += v_shift[i];
	}
}



#endif /* LINMATH_H_LINMATH_ADDITIONAL_H_ */
