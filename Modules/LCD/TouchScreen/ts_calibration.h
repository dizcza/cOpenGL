/*
 * ts_calibration.h
 *
 *  Created on: Apr 4, 2017
 *      Author: dizcza
 */

#ifndef TS_CALIBRATION_H_
#define TS_CALIBRATION_H_

/**
  * @brief  Performs the TS calibration
  * @param  None
  * @retval None
  */
void Touchscreen_Calibration(void);

/**
  * @brief  Calibrate X position
  * @param  x : X position
  * @retval Calibrated x
  */
uint16_t Calibration_GetX(uint16_t x);
/**
  * @brief  Calibrate Y position
  * @param  y : Y position
  * @retval Calibrated y
  */
uint16_t Calibration_GetY(uint16_t y);

/**
  * @brief  check if the TS is calibrated
  * @param  None
  * @retval Calibration state (1 : calibrated / 0: no)
  */
uint8_t IsCalibrationDone(void);

#endif /* TS_CALIBRATION_H_ */
