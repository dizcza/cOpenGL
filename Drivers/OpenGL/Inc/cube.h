/*
 * cube.h
 *
 *  Created on: Nov 11, 2016
 *      Author: dizczaw7
 */

#ifndef CUBE_H_
#define CUBE_H_

#include "stdint.h"
#include "linmath.h"
#include "triangle.h"

#define CUBE_VERTEX_COUNT 8
#define CUBE_INDEX_COUNT 36
#define CUBE_TRIANGLE_COUNT (CUBE_INDEX_COUNT / 3)

typedef struct Cube {
	mat4x4 model;
	vec4 vertices[CUBE_VERTEX_COUNT];
	uint8_t indices[CUBE_INDEX_COUNT];
} Cube;

void Cube_Init(Cube* cube, float size);

void Cube_Scale(Cube* cube, float scale);
void Cube_Translate(Cube* cube, float x, float y, float z);
void Cube_Rotate(Cube* cube, quat q);

void Cube_GetTrianlge(Cube* cube, Triangle trian, uint8_t tr_id);

#endif /* CUBE_H_ */
