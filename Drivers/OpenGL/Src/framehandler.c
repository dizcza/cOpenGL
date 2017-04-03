/*
 * framehandler.c
 *
 *  Created on: Apr 3, 2017
 *      Author: dizcza
 */

#define FRAME_HANDLER_MAX_LAYERS 2
#define CLEAR_COLOR LCD_COLOR_WHITE

#include "framehandler.h"
#include "stm32f429i_discovery_lcd.h"

static FrameBuffer m_frames[FRAME_HANDLER_MAX_LAYERS];
static uint32_t m_active_frame_id = 0;


void FrameHandler_Init(uint16_t width, uint16_t height) {
	uint32_t frame_id;
	for (frame_id = 0; frame_id < FRAME_HANDLER_MAX_LAYERS; ++frame_id) {
		FrameBuffer_Init(&m_frames[frame_id], frame_id, width, height);
	}
}

void FrameHandler_onReadyToDraw(FrameBuffer* frame) {
	BSP_LCD_SetLayerVisible_NoReload(m_active_frame_id, DISABLE);
	BSP_LCD_SetLayerVisible(frame->frame_id, ENABLE);
	FrameBuffer_Clear(m_active_frame_id, CLEAR_COLOR);
	m_active_frame_id = frame->frame_id;
}

FrameBuffer* FrameHandler_getActiveFrame() {
	return &m_frames[m_active_frame_id];
}

FrameBuffer* FrameHandler_getBackgrFrame() {
	uint32_t backrg_id = (m_active_frame_id + 1) % FRAME_HANDLER_MAX_LAYERS;
	return &m_frames[backrg_id];
}
