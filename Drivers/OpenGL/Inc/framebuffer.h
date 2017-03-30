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
#include "ili9341.h"

#define PX_CNT 100

//#define TFTWIDTH ILI9341_LCD_PIXEL_WIDTH / 2
//#define TFTHEIGHT ILI9341_LCD_PIXEL_HEIGHT / 2

#define TFTWIDTH 269 / 2
#define TFTHEIGHT 323 / 2


typedef struct FrameBuffer {
	float depth[TFTHEIGHT][TFTWIDTH];
	uint32_t color[TFTHEIGHT][TFTWIDTH];
	float aspect;
} FrameBuffer;

void FrameBuffer_Init(FrameBuffer* frame);
void FrameBuffer_Flush(FrameBuffer* frame);

void FrameBuffer_DrawCube(FrameBuffer* frame, Camera* const camera, Cube* const cube);

#endif /* FRAMEBUFFER_H_ */
