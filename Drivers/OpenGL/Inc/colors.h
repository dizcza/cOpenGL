/*
 * colors.h
 *
 *  Created on: Apr 2, 2017
 *      Author: dizcza
 */

#ifndef OPENGL_INC_COLORS_H_
#define OPENGL_INC_COLORS_H_

#include "linmath.h"

void Colors_RgbFromHex(vec3uint32 v_rgb, uint32_t const hex);

void Colors_fRgbFromHex(vec3 fv_rgb, uint32_t const hex);

uint32_t Colors_HexFromRgb(vec3uint32 const v_rgb);

uint32_t Colors_HexFromRgbf(vec3 const fv_rgb);

void Colors_fRgbFromHexMat(mat3x3 m_rgb, vec3uint32 const v_hex);

#endif /* OPENGL_INC_COLORS_H_ */
