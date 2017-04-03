/*
 * framebuffer.h
 *
 *  Created on: Nov 12, 2016
 *      Author: dizcza
 */

#ifndef FRAMEBUFFER_H_
#define FRAMEBUFFER_H_

#include "cube.h"
#include "camera.h"

typedef struct FrameBuffer {
	uint32_t frame_id;
	uint16_t width, height;
	float aspect;

	void (*DrawPixel)(uint16_t x, uint16_t y, uint32_t color);
	uint32_t (*ReadPixel)(uint16_t x, uint16_t y);
	void (*WriteDepth)(uint16_t x, uint16_t y, float depth);
	float (*ReadDepth)(uint16_t x, uint16_t y);
} FrameBuffer;

void FrameBuffer_Init(FrameBuffer* frame, uint32_t frm_id, uint16_t width, uint16_t height);
void FrameBuffer_Clear(FrameBuffer* frame, uint32_t color);
void FrameBuffer_DrawCube(FrameBuffer* frame, const Camera* camera, const Cube* cube);

#endif /* FRAMEBUFFER_H_ */
