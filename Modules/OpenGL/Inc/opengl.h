/*
 * opengl.h
 *
 *  Created on: Aug 6, 2020
 *      Author: dizcza
 */

#ifndef OPENGL_INC_OPENGL_H_
#define OPENGL_INC_OPENGL_H_

#include "stm32f4xx_hal.h"


#define OPENGL_ASSERT

/* Exported macro ------------------------------------------------------------*/
#ifdef  OPENGL_ASSERT
/**
  * @brief  The opengl_assert macro is used for function's parameters check.
  * @param  expr If expr is false, it calls opengl_assert_failed function
  *         which reports the name of the source file and the source
  *         line number of the call that failed.
  *         If expr is true, it returns no value.
  * @retval None
  */
  #define opengl_assert(expr) ((expr) ? (void)0U : opengl_assert_failed((uint8_t *)__FILE__, __LINE__))
/* Exported functions ------------------------------------------------------- */
  void opengl_assert_failed(uint8_t* file, uint32_t line);
#else
  #define opengl_assert(expr) ((void)0U)
#endif /* OPENGL_ASSERT */


#endif /* OPENGL_INC_OPENGL_H_ */
