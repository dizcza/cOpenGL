/*
 * colors.h
 *
 *  Created on: Apr 2, 2017
 *      Author: dizcza
 */

#ifndef OPENGL_INC_COLORMAP_H_
#define OPENGL_INC_COLORMAP_H_

#include "opengl.h"
#include "linmath.h"

void Colormap_RgbFromHex(vec3uint32 v_rgb, uint32_t const hex);

void Colormap_fRgbFromHex(vec3 fv_rgb, uint32_t const hex);

uint32_t Colormap_HexFromRgb(vec3uint32 const v_rgb);

uint32_t Colormap_HexFromRgbf(vec3 const fv_rgb);

void Colormap_fRgbFromHexMat(mat3x3 m_rgb, vec3uint32 const v_hex);

#endif /* OPENGL_INC_COLORMAP_H_ */
