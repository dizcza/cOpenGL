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

#define TFTWIDTH 269 / 2
#define TFTHEIGHT 323 / 2


typedef struct FrameBuffer {
	float depth[TFTHEIGHT][TFTWIDTH];
	uint32_t color[TFTHEIGHT][TFTWIDTH];
	float aspect;
} FrameBuffer;

void FrameBuffer_Init(FrameBuffer* frame);
void FrameBuffer_Flush(FrameBuffer* frame);
void FrameBuffer_Clear(FrameBuffer* frame, uint32_t color);
void FrameBuffer_DrawCube(FrameBuffer* frame, const Camera* camera, const Cube* cube);

#endif /* FRAMEBUFFER_H_ */
