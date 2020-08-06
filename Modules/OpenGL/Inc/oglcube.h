/*
 * cube.h
 *
 *  Created on: Nov 11, 2016
 *      Author: dizcza
 */

#ifndef OPENGL_CUBE_H_
#define OPENGL_CUBE_H_

#include <stdint.h>

#include "triangle.h"
#include "linmath.h"

#define CUBE_VERTEX_COUNT 8
#define CUBE_INDEX_COUNT 36
#define CUBE_TRIANGLE_COUNT (CUBE_INDEX_COUNT / 3)

typedef struct Cube {
	mat4x4 model;
	vec4 vertices[CUBE_VERTEX_COUNT];
	uint32_t colors[CUBE_VERTEX_COUNT];
	uint8_t indices[CUBE_INDEX_COUNT];
} Cube;

void OpenGL_Cube_Init(Cube* cube, float size);

void OpenGL_Cube_Scale(Cube* cube, float scale);
void OpenGL_Cube_Translate(Cube* cube, float x, float y, float z);
void OpenGL_Cube_TranslateVec3(Cube* cube, vec3 const v);
void OpenGL_Cube_RotateWorld(Cube* cube, quat const q);
void OpenGL_Cube_RotateLocal(Cube* cube, quat const q);

void OpenGL_Cube_GetTriangle(const Cube* cube, trian4 trian, vec3uint32 colors, uint8_t tr_id);

#endif /* OPENGL_CUBE_H_ */
