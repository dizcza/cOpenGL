/*
 * camera.h
 *
 *  Created on: Nov 12, 2016
 *      Author: dizcza
 */

#ifndef CAMERA_H_
#define CAMERA_H_

#include "opengl.h"
#include "linmath.h"

typedef struct Camera {
	mat4x4 view;
	mat4x4 proj;
} Camera;

void Camera_Init(Camera* camera);
void Camera_GetViewProj(const Camera* camera, mat4x4 vp);

#endif /* CAMERA_H_ */
