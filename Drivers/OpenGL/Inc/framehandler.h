/*
 * framehandler.h
 *
 *  Created on: Apr 3, 2017
 *      Author: dizcza
 */

#ifndef OPENGL_INC_FRAMEHANDLER_H_
#define OPENGL_INC_FRAMEHANDLER_H_

#include "framebuffer.h"

void FrameHandler_Init(uint16_t width, uint16_t height);
FrameBuffer* FrameHandler_getActiveFrame();
FrameBuffer* FrameHandler_getBackgrFrame();
void FrameHandler_onReadyToDraw(FrameBuffer* frame);

#endif /* OPENGL_INC_FRAMEHANDLER_H_ */
