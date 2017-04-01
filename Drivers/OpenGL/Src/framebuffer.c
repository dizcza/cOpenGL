/*
 * framebuffer.c
 *
 *  Created on: Nov 12, 2016
 *      Author: dizcza
 */

#include "framebuffer.h"
#include "linmath.h"
#include "stm32f429i_discovery_lcd.h"
#include <stdlib.h> // todo: use hardware random generator

#define ASPECT (((float) TFTWIDTH) / (TFTHEIGHT))

void FrameBuffer_Init(FrameBuffer* frame) {
	frame->aspect = (float) TFTWIDTH / TFTHEIGHT;
	int16_t x, y;
	for (x = 0; x < TFTWIDTH; ++x) {
		for (y = 0; y < TFTHEIGHT; ++y) {
			frame->depth[y][x] = 1.0;
			frame->color[y][x] = LCD_COLOR_BLACK;
		}
	}
}

void ndc_to_screen(vec3 screen, vec4 ndc) {
	screen[0] = (ndc[0] + 1) / 2.0 * TFTWIDTH;
	screen[1] = (1 - ndc[1]) / 2.0 * TFTHEIGHT * ASPECT;
	screen[2] = ndc[2];
}

void FrameBuffer_Flush(FrameBuffer* frame) {
	int16_t x, y;
	for (x = 0; x < TFTWIDTH; ++x) {
		for (y = 0; y < TFTHEIGHT; ++y) {
			BSP_LCD_DrawPixel(x, y, frame->color[y][x]);
			//BSP_LCD_DrawPixel(x, y, LCD_COLOR_MAGENTA);
		}
	}
}


void FrameBuffer_FillTrian(FrameBuffer* frame, trian3 const trian_xyz, mat4x4 const mvp, uint32_t color) {
	uint16_t y, x;
	trian2 trian_xy;
	vec3 vdepths;
	trian3_getVerticesDepth(vdepths, trian_xyz);
	trian2_fromTrian3(trian_xy, trian_xyz);
	int16_t bmin[2];
	int16_t bmax[2];
	trian2_bboxi(bmin, bmax, trian_xy);
	for (y = max(bmin[1], 0); y < min(bmax[1], TFTHEIGHT); ++y) {
		for (x = max(bmin[0], 0); x < min(bmax[0], TFTWIDTH); ++x) {
			//float p_depth = buffer[pid][2]; - calculate based on three vertices; then check with frame buffer depth
			vec2 p = {x, y};
			if (trian2_isPointInside(trian_xy, p)) {
				frame->color[y][x] = color;
			}
		}
	}
}

void FrameBuffer_CountourTrian(FrameBuffer* frame, trian3 const trian_xyz, mat4x4 const mvp, uint32_t color) {
	vec3 buffer[PX_CNT];
	uint8_t i;
	for (i = 0; i < 3; ++i) {
		uint8_t next = (i + 1) % 3;
		vec3_interpolate(buffer, PX_CNT, trian_xyz[i], trian_xyz[next]);
		int32_t pid = 0;
		for (pid = 0; pid < PX_CNT; ++pid) {
			int16_t x = (int16_t) buffer[pid][0];
			int16_t y = (int16_t) buffer[pid][1];
			float p_depth = buffer[pid][2];
			if (p_depth >= -1.0 && p_depth < frame->depth[x][y]) {
				frame->color[y][x] = color;
				frame->depth[y][x] = p_depth;
			}
		}
	}
}

void FrameBuffer_ProjectTrian4(FrameBuffer* frame, trian4 const trian, mat4x4 const mvp) {
	uint8_t i;
	trian3 trian_xyz;
	for (i = 0; i < 3; ++i) {
		vec4 ndc_point;
		mat4x4_mul_vec4(ndc_point, mvp, trian[i]);
		vec4_scale_self(ndc_point, 1.0 / ndc_point[3]);
		ndc_to_screen(trian_xyz[i], ndc_point);
	}
	uint32_t color = abs(rand());
	FrameBuffer_FillTrian(frame, trian_xyz, mvp, color);
}


void FrameBuffer_DrawCube(FrameBuffer* frame, Camera* const camera, Cube* const cube) {
	mat4x4 vp;
	Camera_GetViewProj(camera, vp);

	mat4x4 mvp;
	mat4x4_mul(mvp, vp, cube->model);

	trian4 trian;
	uint8_t tr_id;
	for (tr_id = 0; tr_id < CUBE_TRIANGLE_COUNT; ++tr_id) {
		Cube_GetTrianlge(cube, trian, tr_id);
		FrameBuffer_ProjectTrian4(frame, trian, mvp);
	}
}



