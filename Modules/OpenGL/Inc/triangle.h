/*
 * triangle.h
 *
 *  Created on: Nov 12, 2016
 *      Author: dizcza
 */

#ifndef TRIANGLE_H_
#define TRIANGLE_H_

#include <float.h>

#include "opengl.h"
#include "linmath.h"

#define TRIANGLE_DEFINE_TRIAN(n) \
typedef vec##n trian##n[3]; \
LINMATH_H_FUNC void trian##n##_init(trian##n trian, vec##n const v0, vec##n const v1, vec##n const v2) { \
	vec##n##_dup(trian[0], v0); \
	vec##n##_dup(trian[1], v1); \
	vec##n##_dup(trian[2], v2); \
} \
LINMATH_H_FUNC void trian##n##_bbox(vec##n bmin, vec##n bmax, trian##n const trian) { \
	vec##n##_set(bmin, INFINITY); \
	vec##n##_set(bmax, -INFINITY); \
	uint8_t vi, dim; \
	for (vi = 0; vi < 3; ++vi) { \
		for (dim = 0; dim < n; ++dim) { \
			if (trian[vi][dim] < bmin[dim]) { \
				bmin[dim] = trian[vi][dim]; \
			} \
			if (trian[vi][dim] > bmax[dim]) { \
				bmax[dim] = trian[vi][dim]; \
			} \
		} \
	} \
}

TRIANGLE_DEFINE_TRIAN(2);
TRIANGLE_DEFINE_TRIAN(3);
TRIANGLE_DEFINE_TRIAN(4);

LINMATH_H_FUNC void trian2_bbox_int16(vec2int16 bmin, vec2int16 bmax, trian2 const trian) {
	vec2 bmin_float;
	vec2 bmax_float;
	trian2_bbox(bmin_float, bmax_float, trian);
	vec2int16_floor(bmin, bmin_float);
	vec2int16_ceil(bmax, bmax_float);
}


LINMATH_H_FUNC void trian2_fromTrian3(trian2 res, trian3 const basis) {
	uint8_t i;
	for (i = 0; i < 3; ++i) {
		res[i][0] = basis[i][0];
		res[i][1] = basis[i][1];
	}
}

LINMATH_H_FUNC void trian3_getVerticesDepth(vec3 depth, trian3 const trian) {
	uint8_t i;
	for (i = 0; i < 3; ++i) {
		depth[i] = trian[i][2];
	}
}

LINMATH_H_FUNC float trian2_edgeFunc(vec2 const v_from, vec2 const v_to, vec2 const p) {
	return (p[0] - v_from[0]) * (v_to[1] - v_from[1]) - (p[1] - v_from[1]) * (v_to[0] - v_from[0]);
}

LINMATH_H_FUNC void trian2_barycentric(vec3 coef, trian2 const trian, vec2 const p) {
	const float area = trian2_edgeFunc(trian[0], trian[1], trian[2]);
	coef[0] = trian2_edgeFunc(trian[1], trian[2], p) / area;
	coef[1] = trian2_edgeFunc(trian[2], trian[0], p) / area;
	coef[2] = trian2_edgeFunc(trian[0], trian[1], p) / area;
}

#endif /* TRIANGLE_H_ */
