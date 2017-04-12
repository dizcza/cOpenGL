/**
  ******************************************************************************
  * File Name          : main.c
  * Description        : Main program body
  ******************************************************************************
  *
  * COPYRIGHT(c) 2017 STMicroelectronics
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32f4xx_hal.h"
#include "ltdc.h"
#include "gpio.h"
#include "fmc.h"

/* USER CODE BEGIN Includes */
#include "stm32f429i_discovery.h"
#include "stm32f429i_discovery_ts.h"
#include "stm32f429i_discovery_io.h"
#include "stm32f429i_discovery_lcd.h"

#include "ts_calibration.h"
#include "cube.h"
#include "camera.h"
#include "framehandler.h"
#include "depth_sdram.h"
#include "debug_printf.h"
#include "linmath_test.h"
#include "Demo.h"

/* USER CODE END Includes */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

#define DEPTH_SDRAM_START_ADRRES (LCD_FRAME_BUFFER + 2 * BUFFER_OFFSET)

/* Private variables ---------------------------------------------------------*/
extern SDRAM_HandleTypeDef hsdram2;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
void Error_Handler(void);

/* USER CODE BEGIN PFP */
/* Private function prototypes -----------------------------------------------*/

/* USER CODE END PFP */

/* USER CODE BEGIN 0 */

void BSP_InitStuff() {
	/* Configure USER Button */
	BSP_PB_Init(BUTTON_KEY, BUTTON_MODE_EXTI);

	/* Initialize the LCD */
	BSP_LCD_Init();

	/* Initialize the LCD Layers */
	BSP_LCD_LayerDefaultInit(1, LCD_FRAME_BUFFER);
	BSP_LCD_LayerDefaultInit(0, LCD_FRAME_BUFFER + BUFFER_OFFSET);

	/* Initialize LEDs */
	BSP_LED_Init(LED3);
	BSP_LED_Init(LED4);

	BSP_LCD_SelectLayer(1);
	BSP_LCD_SetFont(&LCD_DEFAULT_FONT);
	BSP_LCD_SetBackColor(0xFF00FF00);
	BSP_LCD_Clear(LCD_COLOR_WHITE);

	assert_param(BSP_TS_Init(BSP_LCD_GetXSize(), BSP_LCD_GetYSize()) == TS_OK);

	if (IsCalibrationDone() == 0) {
		Touchscreen_Calibration();
	}
}

void Cube_TouchMe() {
	uint8_t status;
	BSP_LCD_DrawRect(10, 10, 10, 10);
	while (1) {
		status = BSP_TS_ITGetStatus();
		db_printf("status %d\n", status);
		BSP_TS_ITClear();
		if (status) {
			BSP_LCD_DrawRect(100, 100, 40, 40);
			db_printf("GOT!\n");
		}
	}
	while (1) {
		TS_StateTypeDef ts_state;
		BSP_TS_GetState(&ts_state);
		BSP_LCD_Clear(LCD_COLOR_WHITE);
		if (ts_state.TouchDetected) {
			uint16_t x = Calibration_GetX(ts_state.X);
			uint16_t y = Calibration_GetY(ts_state.Y);
			db_printf("x=%d y=%d ; ", x, y);db_printf("xts=%d yts=%d\n", ts_state.X, ts_state.Y);
			char str[20];
			sprintf(str, "X=%d,Y=%d\n", ts_state.X, ts_state.Y);
			BSP_LCD_DisplayStringAtLine(0, (uint8_t*) str);
			sprintf(str, "X=%d,Y=%d\n", x, y);
			BSP_LCD_DisplayStringAtLine(1, (uint8_t*) str);
			//HAL_Delay(300);
		}
	}

}

/* USER CODE END 0 */

int main(void)
{

  /* USER CODE BEGIN 1 */

#ifdef DEBUG
	initialise_monitor_handles();
#endif

  /* USER CODE END 1 */

  /* MCU Configuration----------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* Configure the system clock */
  SystemClock_Config();

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_LTDC_Init();
  MX_FMC_Init();

  /* USER CODE BEGIN 2 */
	BSP_InitStuff();

#ifdef USE_FULL_ASSERT
	Linmath_RunTests();
#endif /* USE_FULL_ASSERT */

	Depth_SDRAM_Init(&hsdram2, DEPTH_SDRAM_START_ADRRES, BSP_LCD_GetXSize(),
			BSP_LCD_GetYSize());

	CubeRotationAnim_DemoRun();

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
	while (1) {
  /* USER CODE END WHILE */

  /* USER CODE BEGIN 3 */

	}
  /* USER CODE END 3 */

}

/** System Clock Configuration
*/
void SystemClock_Config(void)
{

  RCC_OscInitTypeDef RCC_OscInitStruct;
  RCC_ClkInitTypeDef RCC_ClkInitStruct;
  RCC_PeriphCLKInitTypeDef PeriphClkInitStruct;

    /**Configure the main internal regulator output voltage 
    */
  __HAL_RCC_PWR_CLK_ENABLE();

  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

    /**Initializes the CPU, AHB and APB busses clocks 
    */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 4;
  RCC_OscInitStruct.PLL.PLLN = 180;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 7;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

    /**Activate the Over-Drive mode 
    */
  if (HAL_PWREx_EnableOverDrive() != HAL_OK)
  {
    Error_Handler();
  }

    /**Initializes the CPU, AHB and APB busses clocks 
    */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    Error_Handler();
  }

  PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_LTDC;
  PeriphClkInitStruct.PLLSAI.PLLSAIN = 50;
  PeriphClkInitStruct.PLLSAI.PLLSAIR = 2;
  PeriphClkInitStruct.PLLSAIDivR = RCC_PLLSAIDIVR_2;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

    /**Configure the Systick interrupt time 
    */
  HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);

    /**Configure the Systick 
    */
  HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

  /* SysTick_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @param  None
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler */
	/* User can add his own implementation to report the HAL error return state */
	while (1) {
		BSP_LED_Toggle(LED4);
		HAL_Delay(200);
	}
  /* USER CODE END Error_Handler */ 
}

#ifdef USE_FULL_ASSERT

/**
   * @brief Reports the name of the source file and the source line number
   * where the assert_param error has occurred.
   * @param file: pointer to the source file name
   * @param line: assert_param error line source number
   * @retval None
   */
void assert_failed(uint8_t* file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
	/* User can add his own implementation to report the file name and line number,
	 ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
	db_printf("Wrong parameters value: file %s on line %d\r\n", file, line);
	char msg[LCD_MAX_CHARS_LINE + 1];
	BSP_LCD_SetBackColor(LCD_COLOR_WHITE);
	BSP_LCD_SetTextColor(LCD_COLOR_RED);
	BSP_LCD_SetFont(&Font16);
	BSP_LCD_DisplayStringAtLine(0, (uint8_t*) "assert failed");
	sprintf(msg, "on line %lu", line);
	BSP_LCD_DisplayStringAtLine(1, (uint8_t*) msg);
	BSP_LCD_SetFont(&Font12);

	uint16_t lineid = 3;
	int32_t msglen = 0;
	while (file[msglen] != '\0') {
		msglen++;
	}
	int32_t msg_start = 0, i;
	while (msglen > msg_start) {
		int32_t part_size = min(LCD_MAX_CHARS_LINE, msglen - msg_start);
		for (i = 0; i < part_size; ++i) {
			msg[i] = file[msg_start + i];
		}
		msg[part_size] = '\0';
		BSP_LCD_DisplayStringAtLine(lineid, (uint8_t*) msg);
		msg_start += part_size;
		lineid++;
	}
	Error_Handler();
  /* USER CODE END 6 */

}

#endif

/**
  * @}
  */ 

/**
  * @}
*/ 

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
