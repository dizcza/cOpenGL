/*
 * framebuffer.c
 *
 *  Created on: Nov 12, 2016
 *      Author: dizczaw7
 */

#include "framebuffer.h"
#include "linmath.h"

void FrameBuffer_Init(FrameBuffer* frame) {
	int16_t x, y;
	for (x = 0; x < TFTWIDTH; ++x) {
		for (y = 0; y < TFTHEIGHT; ++y) {
			frame->depth[x][y] = 1.0;
			frame->color[x][y] = BLACK;
		}
	}
}

void ndc_to_screen(vec3 screen, vec4 ndc) {
	screen[0] = (ndc[0] + 1) / 2.0 * TFTWIDTH;
	screen[1] = (1 - ndc[1]) / 2.0 * TFTHEIGHT;
	screen[2] = ndc[2];
}

void LCD_DrawPixel(int16_t x, int16_t y, uint16_t color) {
	// Stab!
}

void FrameBuffer_Flush(FrameBuffer* frame) {
	int16_t x, y;
	for (x = 0; x < TFTWIDTH; ++x) {
		for (y = 0; y < TFTHEIGHT; ++y) {
			LCD_DrawPixel(x, y, frame->color[x][y]);
		}
	}
}

void FrameBuffer_ProjectTriangle(FrameBuffer* frame, Triangle trian, mat4x4 const mvp, uint16_t color) {
	uint8_t i;
	vec3 screen[3];
	for (i = 0; i < 3; ++i) {
		vec4 projected_ndc;
		mat4x4_mul_vec4(projected_ndc, mvp, trian[i]);
		vec4_scale(projected_ndc, projected_ndc, 1.0 / projected_ndc[3]);
		ndc_to_screen(screen[i], projected_ndc);
	}
	vec3 buffer[PX_CNT];
	for (i = 0; i < 3; ++i) {
		uint8_t next = (i + 1) % 3;
		vec3_interpolate(buffer, PX_CNT, screen[i], screen[next]);
		int32_t pid = 0;
		for (pid = 0; pid < PX_CNT; ++pid) {
			int16_t x = (int16_t) buffer[pid][0];
			int16_t y = (int16_t) buffer[pid][1];
			float p_depth = buffer[pid][2];
			if (p_depth >= -1.0 && p_depth < frame->depth[x][y]) {
				frame->color[x][y] = color;
				frame->depth[x][y] = p_depth;
			}
		}
	}
}


void FrameBuffer_DrawCube(FrameBuffer* frame, Camera* camera, Cube* cube) {
	mat4x4 vp;
	Camera_GetViewProj(camera, vp);

	mat4x4 mvp;
	mat4x4_mul(mvp, vp, cube->model);

	Triangle trian;
	uint8_t tr_id;
	for (tr_id = 0; tr_id < CUBE_TRIANGLE_COUNT; ++tr_id) {
		Cube_GetTrianlge(cube, trian, tr_id);
		FrameBuffer_ProjectTriangle(frame, trian, mvp, BLUE);
	}
}



