/*
 * framehandler.c
 *
 *  Created on: Apr 3, 2017
 *      Author: dizcza
 */

#define FRAME_HANDLER_MAX_LAYERS 2

#include "stm32f429i_discovery_lcd.h"
#include "framehandler.h"
#include "framebuffer.h"

static FrameBuffer m_frames[FRAME_HANDLER_MAX_LAYERS];
static uint32_t m_drawing_frame_id = 1;
static uint32_t m_first_flush_timestamp = 0;
static uint32_t m_flushes = 0;
static char m_fps_info_str[10];

static uint32_t inline FrameHandler_GetOtherFrameId() {
	return (m_drawing_frame_id + 1) % FRAME_HANDLER_MAX_LAYERS;
}

void FrameHandler_Init(uint16_t width, uint16_t height) {
	uint32_t frame_id;
	for (frame_id = 0; frame_id < FRAME_HANDLER_MAX_LAYERS; ++frame_id) {
		FrameBuffer_Init(&m_frames[frame_id], frame_id, width, height);
	}
}

void FrameHandler_DrawCube(const Camera* camera, const Cube* cube) {
	FrameBuffer_DrawCube(&m_frames[m_drawing_frame_id], camera, cube);
}

void FrameHandler_glFlush() {
	BSP_LCD_SetLayerVisible_NoReload(FrameHandler_GetOtherFrameId(), DISABLE);
	BSP_LCD_SetLayerVisible(m_drawing_frame_id, ENABLE);
	m_drawing_frame_id = FrameHandler_GetOtherFrameId();
	BSP_LCD_SelectLayer(m_drawing_frame_id);
	FrameBuffer_Clear(&m_frames[m_drawing_frame_id]);
	if (m_flushes == 0) {
		m_first_flush_timestamp = HAL_GetTick();
	} else {
		float fps = 1000.f * m_flushes / (HAL_GetTick() - m_first_flush_timestamp);
		sprintf(m_fps_info_str, "FPS %.2f", fps);
		BSP_LCD_DisplayStringAtLine(0, (uint8_t*)m_fps_info_str);
	}
	m_flushes++;
}
