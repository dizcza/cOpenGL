/*
 * triangle.h
 *
 *  Created on: Nov 12, 2016
 *      Author: dizcza
 */

#ifndef TRIANGLE_H_
#define TRIANGLE_H_

#include "linmath.h"

#define TRIANGLE_DEFINE_TRIAN(n) \
typedef vec##n trian##n[3]; \
static inline void trian##n##_init(trian##n trian, vec##n const v0, vec##n const v1, vec##n const v2) { \
	vec##n##_copy(trian[0], v0); \
	vec##n##_copy(trian[1], v1); \
	vec##n##_copy(trian[2], v2); \
}

TRIANGLE_DEFINE_TRIAN(2);
TRIANGLE_DEFINE_TRIAN(3);
TRIANGLE_DEFINE_TRIAN(4);

static inline void trian2_fromTrian3(trian2 res, trian3 const basis) {
	uint8_t i;
	for (i = 0; i < 3; ++i) {
		res[i][0] = basis[i][0];
		res[i][1] = basis[i][1];
	}
}

static inline void trian3_getVerticesDepth(vec3 depth, trian3 const trian) {
	uint8_t i;
	for (i = 0; i < 3; ++i) {
		depth[i] = trian[i][2];
	}
}

static inline uint8_t trian2_edgeFunc(vec2 const v_from, vec2 const v_to, vec2 const p) {
	return (p[0] - v_from[0]) * (v_to[1] - v_from[1]) - (p[1] - v_from[1]) * (v_to[0] - v_from[0]) >= 0;
}

static inline uint8_t trian2_isPointInside(trian2 const trian, vec2 const p) {
	uint8_t inside = 1;
	inside &= trian2_edgeFunc(trian[0], trian[1], p);
	inside &= trian2_edgeFunc(trian[1], trian[2], p);
	inside &= trian2_edgeFunc(trian[2], trian[0], p);
	return inside;
}

#endif /* TRIANGLE_H_ */
