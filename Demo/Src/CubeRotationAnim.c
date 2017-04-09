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

extern void Assert_Cube(const Cube* cube);

void CubeRotationAnim_DemoRun() {
	Cube cube;
	Cube_Init(&cube, 0.5f);

//	mat4x4_rotate_Y(cube.model, cube.model, 30.0 * 3.14 / 180);
//	mat4x4_rotate_Z(cube.model, cube.model, 30.0 * 3.14 / 180);
//	mat4x4_rotate_X(cube.model, cube.model, 30.0 * 3.14 / 180);
	Cube_Translate(&cube, 0.f, -1.f, -5.f);

	Assert_Cube(&cube);
	quat q;
	vec3 axis = {1, 1, 0};
	vec3_norm_self(axis);
	float degrees = 150;
	quat_rotate(q, degrees_to_rads(degrees), axis);
	Cube_RotateLocal(&cube, q);
	Assert_Cube(&cube);

	Camera camera;
	Camera_Init(&camera);

	FrameHandler_Init(BSP_LCD_GetXSize(), BSP_LCD_GetYSize());
	FrameHandler_DrawCube(&camera, &cube);
	FrameHandler_glFlush();

	Cube_Translate(&cube, 0.f, 0.4f, 0.f);
	FrameHandler_DrawCube(&camera, &cube);
	FrameHandler_glFlush();

	Cube_Translate(&cube, 0.f, -0.4f, 0.f);
	FrameHandler_DrawCube(&camera, &cube);
	FrameHandler_glFlush();

	while (1) {

	}

}
