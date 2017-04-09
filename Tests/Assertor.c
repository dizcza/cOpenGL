/*
 * test.c
 *
 *  Created on: Apr 9, 2017
 *      Author: dizcza
 */

#include "math.h"
#include "cube.h"
#include "stm32f4xx_hal.h"

#ifndef assert_param
#define assert_param(expr) ((expr) ? (void)0U : assert_failed((uint8_t *)__FILE__, __LINE__))
void assert_failed(uint8_t* file, uint32_t line);
#endif /* assert_param */

#define ASSERT_EPS ((float) 1e-4)

void Assert_Trian454590(trian4 const trian) {
	uint8_t i, i_next, j;
	mat3x3 mv;
	vec3_sub(mv[0], trian[1], trian[0]);
	vec3_sub(mv[1], trian[2], trian[1]);
	vec3_sub(mv[2], trian[0], trian[2]);
	for (i = 0; i < 3; ++i) {
		vec3_norm_self(mv[i]);
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
	for (i = 0; i < 2; ++i) {
		assert_param(fabsf(products[i] - cosf(3 * M_PI / 4)) < ASSERT_EPS);
	}
	assert_param(fabsf(products[2] - cosf(M_PI / 2)) < ASSERT_EPS);
}

void Assert_Cube(const Cube* cube) {
#ifdef USE_FULL_ASSERT
	vec3uint32 colors;
	trian4 trian;
	uint8_t i;
	for (i = 0; i < CUBE_TRIANGLE_COUNT; ++i) {
		Cube_GetTriangle(cube, trian, colors, i);
		Assert_Trian454590(trian);
	}
#endif /* USE_FULL_ASSERT */
}
