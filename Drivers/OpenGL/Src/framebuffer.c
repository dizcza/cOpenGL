/*
 * framebuffer.c
 *
 *  Created on: Nov 12, 2016
 *      Author: dizcza
 */

#include "stm32f429i_discovery_lcd.h"
#include "framebuffer.h"
#include "linmath.h"
#include "colors.h"
#include "debug_printf.h"
#include "depth_sdram.h"

#define FRAME_CLEAR_COLOR  LCD_COLOR_LIGHTGRAY

static void FrameBuffer_ProjectNdcPointToScreen(const FrameBuffer* frame, vec3 screen, vec4 ndc) {
	screen[0] = (ndc[0] + 1) / 2.0 * frame->width;
	screen[1] = (1 - ndc[1]) / 2.0 * frame->width;
	screen[2] = ndc[2];
}


void FrameBuffer_Init(FrameBuffer* frame, uint32_t frm_id, uint16_t width, uint16_t height) {
	frame->frame_id = frm_id;
	frame->width = width;
	frame->height = height;
	frame->DrawPixel = BSP_LCD_DrawPixel;
	frame->ReadPixel = BSP_LCD_ReadPixel;
	frame->WriteDepth = Depth_SDRAM_WriteDepth;
	frame->ReadDepth = Depth_SDRAM_ReadDepth;
	FrameBuffer_Clear(frame);
}

void FrameBuffer_Clear(FrameBuffer* frame) {
	BSP_LCD_Clear(FRAME_CLEAR_COLOR);
	Depth_SDRAM_ClearDepth();
}


void FrameBuffer_FillTrian(FrameBuffer* frame, trian3 const trian_xyz, vec3uint32 const vcolors) {
	mat3x3 m_rgb;
	Colors_fRgbFromHexMat(m_rgb, vcolors);
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
	for (y = LINMATH_MAX(bmin[1], 0); y < LINMATH_MIN(bmax[1], frame->height); ++y) {
		for (x = LINMATH_MAX(bmin[0], 0); x < LINMATH_MIN(bmax[0], frame->width); ++x) {
			vec2 p = {x, y};
			trian2_barycentric(baryc, trian_xy, p);
			if (vec3_all_pos(baryc)) {
				float p_depth = vec3_mul_inner(baryc, vdepths);
				if (p_depth >= -1.0f && p_depth < frame->ReadDepth(x, y)) {
					frame->WriteDepth(x, y, p_depth);
					mat3x3_mul_vec3(fv_rgb, m_rgb, baryc);
					uint32_t p_color = Colors_HexFromRgbf(fv_rgb);
					frame->DrawPixel(x, y, p_color);
				}
			}
		}
	}
}

void FrameBuffer_ProjectTrian4(FrameBuffer* frame, trian4 const trian, vec3uint32 const vcolors, mat4x4 const mvp) {
	uint8_t i;
	trian3 trian_xyz;
	for (i = 0; i < 3; ++i) {
		vec4 ndc_point;
		mat4x4_mul_vec4(ndc_point, mvp, trian[i]);
		vec4_scale_self(ndc_point, 1.0 / ndc_point[3]);
		FrameBuffer_ProjectNdcPointToScreen(frame, trian_xyz[i], ndc_point);
	}
	FrameBuffer_FillTrian(frame, trian_xyz, vcolors);
}


void FrameBuffer_DrawCube(FrameBuffer* frame, const Camera* camera, const Cube* cube) {
	mat4x4 vp;
	Camera_GetViewProj(camera, vp);

	mat4x4 mvp;
	mat4x4_mul(mvp, vp, cube->model);

	trian4 trian;
	vec3uint32 vcolors;
	uint8_t tr_id;
	for (tr_id = 0; tr_id < CUBE_TRIANGLE_COUNT; ++tr_id) {
		Cube_GetTriangle(cube, trian, vcolors, tr_id);
		FrameBuffer_ProjectTrian4(frame, trian, vcolors, mvp);
	}
}



