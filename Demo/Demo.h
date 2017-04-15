/*
 * CubeRotationAnim.h
 *
 *  Created on: Apr 8, 2017
 *      Author: dizcza
 */

#ifndef INC_DEMO_H_
#define INC_DEMO_H_

#include "camera.h"

typedef struct {
  void  (*Resume)(void);
  void  (*Pause)(void);
} cOpenGL_DemoTypedef;

void CubeRotationAnim_Init(const Camera* camera);
void CubeRotationAnim_Resume();
void CubeRotationAnim_Pause();

void CubeTouchMe_Init(const Camera* camera);
void CubeTouchMe_Resume();
void CubeTouchMe_Pause();

#endif /* INC_DEMO_H_ */
