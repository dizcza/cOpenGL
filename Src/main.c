/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "dma2d.h"
#include "i2c.h"
#include "ltdc.h"
#include "spi.h"
#include "gpio.h"
#include "fmc.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "stm32f429i_discovery.h"
#include "stm32f429i_discovery_ts.h"
#include "stm32f429i_discovery_io.h"
#include "stm32f429i_discovery_lcd.h"

// opengl.h import must be the first one
#include "opengl.h"

#include "oglcube.h"
#include "TouchScreen/ts_calibration.h"
#include "camera.h"
#include "framehandler.h"
#include "depth_sdram.h"
#include "linmath_test.h"
#include "Demo.h"
#include "Log/lcd_log.h"

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */
#define DEPTH_SDRAM_START_ADRRES (LCD_FRAME_BUFFER + 2 * BUFFER_OFFSET)

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
extern SDRAM_HandleTypeDef hsdram2;
static __IO uint8_t m_DemoId = 0;

cOpenGL_DemoTypedef cOpenGL_Examples[] = {
	{CubeRotationAnim_Resume,  CubeRotationAnim_Pause},
	{CubeTouchMe_Resume,       CubeTouchMe_Pause},
};
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
void BSP_InitAll();

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
void BSP_InitAll() {
   /* Initialize the LCD */
   BSP_LCD_Init();
   LCD_LOG_Init();

   /* Initialize the LCD Layers */
   BSP_LCD_LayerDefaultInit(1, LCD_FRAME_BUFFER);
   BSP_LCD_LayerDefaultInit(0, LCD_FRAME_BUFFER + BUFFER_OFFSET);

   /* Initialize LEDs */
   BSP_LED_Init(LED3);
   BSP_LED_Init(LED4);

   int layer_id;
   for (layer_id = 0; layer_id < 2; layer_id++) {
	   BSP_LCD_SelectLayer(layer_id);
	   BSP_LCD_SetFont(&Font16);
	   BSP_LCD_SetBackColor(0xFF00FF00);
	   BSP_LCD_Clear(LCD_COLOR_WHITE);
   }

   TS_StatusTypeDef ts_ret = BSP_TS_Init(BSP_LCD_GetXSize(), BSP_LCD_GetYSize());
   assert_param(ts_ret == TS_OK);

   if (IsCalibrationDone() == 0) {
	   Touchscreen_Calibration();
   }
}

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
//  MX_FMC_Init();
//  MX_LTDC_Init();
//  MX_DMA2D_Init();
//  MX_SPI5_Init();
//  MX_I2C3_Init();
  /* USER CODE BEGIN 2 */

  // FMC, LTDC, DMA2D, SPI5, and I2C3 will be reinitialized in BSP_LCD_Init()

  BSP_InitAll();

#ifdef OPENGL_ASSERT
  linmath_test_run_all();
#endif /* OPENGL_ASSERT */

  Depth_SDRAM_Init(&hsdram2, DEPTH_SDRAM_START_ADRRES, BSP_LCD_GetXSize(),
			 BSP_LCD_GetYSize());

  Camera camera;
  Camera_Init(&camera);

  FrameHandler_Init(BSP_LCD_GetXSize(), BSP_LCD_GetYSize());

  CubeRotationAnim_Init(&camera);
  CubeTouchMe_Init(&camera);

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
	  cOpenGL_Examples[m_DemoId].Resume();

    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
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
  /** Activate the Over-Drive mode
  */
  if (HAL_PWREx_EnableOverDrive() != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
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
}

/* USER CODE BEGIN 4 */

/**
  * @brief  EXTI line 0 detection callbacks.
  * @param  GPIO_Pin: Specifies the pins connected EXTI line
  * @retval None
  */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {
	if (GPIO_Pin == KEY_BUTTON_PIN) {
		cOpenGL_Examples[m_DemoId].Pause();
		m_DemoId = (m_DemoId + 1) % (sizeof(cOpenGL_Examples) / sizeof(cOpenGL_Examples[0]));
	}
}

#ifdef OPENGL_ASSERT

/**
   * @brief OpenGL application specific reports,
   * where the opengl_assert error has occurred.
   * @param file: pointer to the source file name
   * @param line: opengl_assert error line source number
   * @retval None
   */
void opengl_assert_failed(uint8_t* file, uint32_t line)
{
	BSP_LCD_SetLayerVisible(1, ENABLE);
	BSP_LCD_SelectLayer(1);
	LCD_ErrLog("Assert failed: %s, line %lu\n", file, line);
	Error_Handler();
}

#endif /* OPENGL_ASSERT */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
    HAL_GPIO_WritePin(GPIOG, GPIO_PIN_14, GPIO_PIN_SET);
    while (1);

  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
    opengl_assert_failed(file, line);
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
