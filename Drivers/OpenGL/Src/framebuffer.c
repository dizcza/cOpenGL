/*
 * framebuffer.c
 *
 *  Created on: Nov 12, 2016
 *      Author: dizcza
 */

#include "framebuffer.h"
#include "linmath.h"
#include "colors.h"
#include "debug_printf.h"
#include "stm32f429i_discovery_lcd.h"
#include <stdlib.h> // todo: use hardware random generator

static uint32_t m_active_layer_id;

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

void FrameBuffer_Clear(FrameBuffer* frame, uint32_t color) {
	int16_t x, y;
	for (x = 0; x < TFTWIDTH; ++x) {
		for (y = 0; y < TFTHEIGHT; ++y) {
			frame->depth[y][x] = 1.0;
			frame->color[y][x] = color;
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


void FrameBuffer_FillTrian(FrameBuffer* frame, trian3 const trian_xyz, mat4x4 const mvp, vec3uint32 const colors) {
	mat3x3 m_rgb;
	Colors_fRgbFromHexMat(m_rgb, colors);
	vec3 fv_rgb;

	uint16_t y, x;
	trian2 trian_xy;
	vec3 vdepths;
	trian3_getVerticesDepth(vdepths, trian_xyz);
	trian2_fromTrian3(trian_xy, trian_xyz);
	vec2int16 bmin;
	vec2int16 bmax;
	vec3 baryc;
	trian2_bbox_int16(bmin, bmax, trian_xy);
	for (y = max(bmin[1], 0); y < min(bmax[1], TFTHEIGHT); ++y) {
		for (x = max(bmin[0], 0); x < min(bmax[0], TFTWIDTH); ++x) {
			vec2 p = {x, y};
			trian2_barycentric(baryc, trian_xy, p);
			if (vec3_all_pos(baryc)) {
				float p_depth = vec3_mul_inner(baryc, vdepths);
				if (p_depth >= -1.0 && p_depth < frame->depth[y][x]) {
					frame->depth[y][x] = p_depth;
					mat3x3_mul_vec3(fv_rgb, m_rgb, baryc);
					uint32_t p_color = Colors_HexFromRgbf(fv_rgb);
					frame->color[y][x] = p_color;
					//db_printf("color 0x%08x \n", p_color);
					//BSP_LCD_DrawPixel(x, y, frame->color[y][x]);
				}
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
	vec3uint32 colors;
	for (i = 0; i < 3; ++i) {
		colors[i] = abs(rand());
	}
	colors[0] = LCD_COLOR_RED;
	colors[1] = LCD_COLOR_GREEN;
	colors[2] = LCD_COLOR_BLUE;
	FrameBuffer_FillTrian(frame, trian_xyz, mvp, colors);
}


void FrameBuffer_DrawCube(FrameBuffer* frame, const Camera* camera, const Cube* cube) {
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



