/*
 * test.c
 *
 *  Created on: Apr 9, 2017
 *      Author: dizcza
 */

#include <math.h>
#include "opengl.h"
#include "oglcube.h"

#define OPENGL_ASSERT_EPS (1e-4)

void OpenGL_AssertTrian454590(trian4 const trian) {
	uint8_t i, i_next, j;
	mat3x3 mv;
	vec3_sub(mv[0], trian[1], trian[0]);
	vec3_sub(mv[1], trian[2], trian[1]);
	vec3_sub(mv[2], trian[0], trian[2]);
	for (i = 0; i < 3; ++i) {
        vec3_norm(mv[i], mv[i]);
	}
	float products[3];
	for (i = 0; i < 3; ++i) {
		i_next = (i + 1) % 3;
		products[i] = vec3_mul_inner(mv[i], mv[i_next]);
	}
	for (i = 1; i < 3; ++i) {
		j = i;
		while (j > 0 && products[j] < products[j - 1]) {
			float temp = products[j - 1];
			products[j - 1] = products[j];
			products[j] = temp;
			j--;
		}
	}
	float cos_abs_diff;
	for (i = 0; i < 2; ++i) {
		cos_abs_diff = fabsf(products[i] - cosf(3 * M_PI / 4));
		opengl_assert(cos_abs_diff < OPENGL_ASSERT_EPS);
	}
	cos_abs_diff = fabsf(products[2] - cosf(M_PI / 2));
	opengl_assert(cos_abs_diff < OPENGL_ASSERT_EPS);
}

void OpenGL_AssertCube(const Cube* cube) {
	vec3uint32 colors;
	trian4 trian;
	uint8_t i;
	for (i = 0; i < CUBE_TRIANGLE_COUNT; ++i) {
		OpenGL_Cube_GetTriangle(cube, trian, colors, i);
		OpenGL_AssertTrian454590(trian);
	}
}
