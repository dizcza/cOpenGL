/*
 * CubeRotationAnim.c
 *
 *  Created on: Apr 8, 2017
 *      Author: dizcza
 */

#include <Demo.h>
#include "stm32f429i_discovery_lcd.h"

#include "cube.h"
#include "camera.h"
#include "framehandler.h"
#include "depth_sdram.h"
#include "debug_printf.h"

#define ROTATION_ANIM_DURATION_MS 3000

extern void Assert_Cube(const Cube* cube);

void CubeRotationAnim_DemoRun() {
	Cube cube;
	Cube_Init(&cube, 0.5f);

	Camera camera;
	Camera_Init(&camera);

	FrameHandler_Init(BSP_LCD_GetXSize(), BSP_LCD_GetYSize());

	vec3 axis = {1, 1, 1};
	quat q;
	uint32_t last = HAL_GetTick();
	while (1) {
		uint32_t curr = HAL_GetTick();
		uint32_t ms_passed = (curr - last) % ROTATION_ANIM_DURATION_MS;
		float time_local = ((float)ms_passed) / ROTATION_ANIM_DURATION_MS;
		float degrees = time_local * 180.f;
		quat_rotate(q, degrees_to_rads(degrees), axis);
		Cube_RotateLocal(&cube, q);
		FrameHandler_DrawCube(&camera, &cube);
		FrameHandler_glFlush();
		last = curr;
	}

}
