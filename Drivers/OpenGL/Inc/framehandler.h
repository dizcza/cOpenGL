/*
 * framehandler.h
 *
 *  Created on: Apr 3, 2017
 *      Author: dizcza
 */

#ifndef OPENGL_INC_FRAMEHANDLER_H_
#define OPENGL_INC_FRAMEHANDLER_H_

#include "cube.h"
#include "camera.h"

void FrameHandler_Init(uint16_t width, uint16_t height);
void FrameHandler_Reset();
void FrameHandler_DrawCube(const Camera* camera, const Cube* cube);
void FrameHandler_glFlush();

#endif /* OPENGL_INC_FRAMEHANDLER_H_ */
