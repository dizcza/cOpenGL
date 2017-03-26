/*
 * framebuffer.h
 *
 *  Created on: Nov 12, 2016
 *      Author: dizczaw7
 */

#ifndef FRAMEBUFFER_H_
#define FRAMEBUFFER_H_

#include "cube.h"
#include "camera.h"
#include "lcd_stab.h"

#define PX_CNT 100

typedef struct FrameBuffer {
	float depth[TFTWIDTH][TFTHEIGHT];
	uint16_t color[TFTWIDTH][TFTHEIGHT];
} FrameBuffer;

void FrameBuffer_Init(FrameBuffer* frame);
void FrameBuffer_Flush(FrameBuffer* frame);

void FrameBuffer_DrawCube(FrameBuffer* frame, Camera* camera, Cube* cube);

#endif /* FRAMEBUFFER_H_ */
