/*
 * camera.c
 *
 *  Created on: Nov 12, 2016
 *      Author: dizczaw7
 */

#include "camera.h"
#include "linmath.h"

void Camera_Init(Camera* camera) {
	vec3 target = { 0, 0, -1 };
	vec3 eye = { 0, 0, 1 };
	vec3 up = { 0, 1, 0 };
	mat4x4_look_at(camera->view, eye, target, up);

	mat4x4_ortho(camera->proj, -1, 1, -1, 1, 0, 50);
}


void Camera_GetViewProj(Camera* const camera, mat4x4 vp) {
	mat4x4_mul(vp, camera->proj, camera->view);
}
