/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
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
#include "spi.h"
#include "gpio.h"
#include "ili9163.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
uint8_t rx_data = 0;
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


  LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_SYSCFG);
  LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_PWR);

  NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_4);

  /* System interrupt init*/

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_SPI1_Init();

  initCD_Pin();
  initCS_Pin();
  initRES_Pin();

  LL_mDelay(1);

  lcdInitialise(LCD_ORIENTATION3);
  //lcdClearDisplay(decodeRgbValue(80, 0, 0));

  lcdFilledRectangle(0, 0, 255, 255, decodeRgbValue(0, 0, 0));

  //lcdPutS("DLROW OLLEH", lcdTextX(2), lcdTextY(1), decodeRgbValue(0, 0, 0), decodeRgbValue(31, 31, 31));
  //lcdPutS("8 einecivC", lcdTextX(2), lcdTextY(4), decodeRgbValue(255, 255, 255), decodeRgbValue(0, 0, 0));
  //lcdPutS("SPI komunikacia aicakinu", lcdTextX(2), lcdTextY(5), decodeRgbValue(255, 255, 255), decodeRgbValue(0, 0, 0));

  //uint8_t state = 0;  //na co bude sluzit state?

  int16_t Pentagon[] = {10, 10, 150, 10, 150, 180, 100, 110, 20, 180};

  // demo cyklus, kde testujem vykreslovanie a animaciu, text este nie je vyrieseny
  while (1)
  {
		//state ? lcdFilledRectangle(10, 75, 34, 99, decodeRgbValue(31, 0, 0)) : lcdFilledRectangle(10, 75, 34, 99, decodeRgbValue(0, 31, 0));
		//state ^= 1;
	  	lcdRectangle(225, 225, 10, 250, decodeRgbValue(255, 255, 255)); //dolny status bar
	  	lcdCircle(115,232,5,decodeRgbValue(255, 255, 255)); //akysi kruh, v povodnej doom je tam hlava hraca
	  	// testovanie vykreslovania animacie
	  	for (int16_t i = 0; i<10; i++){
	  		lcdRectangle(20+i*10, 20, 100+i*10, 100, decodeRgbValue(255, 255, 255));
	  		lcdRectangle(20+i*10, 20, 100+i*10, 100, decodeRgbValue(0, 0, 0));
	  	}
	  	for (int16_t i = 0; i<10; i++){
			lcdDottedRectangle(20+i*10, 20, 100+i*10, 100, decodeRgbValue(255, 255, 255), 3);
			lcdDottedRectangle(20+i*10, 20, 100+i*10, 100, decodeRgbValue(0, 0, 0), 3);
	  	}
	  	for (int16_t i = 0; i<10; i++){
			lcdDottedRectangle(20+i*10, 20, 100+i*10, 100, decodeRgbValue(255, 255, 255), 5);
			lcdDottedRectangle(20+i*10, 20, 100+i*10, 100, decodeRgbValue(0, 0, 0), 5);
		}
	  	//testujem polygon
	  	lcdPolygon( Pentagon, sizeof(Pentagon) / (2 * sizeof(Pentagon[0])), decodeRgbValue(255, 255, 255));
	  	LL_mDelay(200);
	  	lcdPolygon( Pentagon, sizeof(Pentagon) / (2 * sizeof(Pentagon[0])), decodeRgbValue(0, 0, 0));
	  	lcdDottedPolygon( Pentagon, sizeof(Pentagon) / (2 * sizeof(Pentagon[0])), decodeRgbValue(255, 255, 255), 3);
	  	LL_mDelay(200);
	    lcdDottedPolygon( Pentagon, sizeof(Pentagon) / (2 * sizeof(Pentagon[0])), decodeRgbValue(0, 0, 0), 3);
	  	// testujem vykreslenie plneneho stvorca
	  	lcdFilledRectangle(30, 30, 170, 170, decodeRgbValue(255, 255, 255));
	  	LL_mDelay(200);
	  	lcdFilledRectangle(30, 30, 170, 170, decodeRgbValue(0, 0, 0));
	  	//testujem vykreslenie bodkovaneho stvorca
	  	lcdFilledDottedRectangle(30, 30, 170, 170, decodeRgbValue(255, 255, 255), 5);
	  	LL_mDelay(200);
	  	lcdFilledDottedRectangle(30, 30, 170, 170, decodeRgbValue(0, 0, 0), 5);
	    // testujem vykreslenie ciary v oboch smeroch
	  	lcdLine(10, 10, 190, 190, decodeRgbValue(255, 255, 255));
	  	LL_mDelay(200);
	  	lcdLine(10, 10, 190, 190, decodeRgbValue(0, 0, 0));
	  	lcdLine(190, 190, 10, 10, decodeRgbValue(255, 255, 255));
	  	LL_mDelay(200);
	    lcdLine(190, 190, 10, 10, decodeRgbValue(0, 0, 0));
	    // testujem vykreslenie kruhu
	    lcdCircle(100, 100, 80, decodeRgbValue(255, 255, 255));
	    LL_mDelay(200);
	    lcdCircle(100, 100, 80, decodeRgbValue(0, 0, 0));
	    lcdDottedCircle(100, 100, 80, decodeRgbValue(255, 255, 255), 3);
	    LL_mDelay(200);
	    lcdDottedCircle(100, 100, 80, decodeRgbValue(0, 0, 0), 3);
	    // animovany kruh
	    for (int16_t i = 0; i<10; i++){
			lcdDottedCircle(60+i*10, 100, 50, decodeRgbValue(255, 255, 255), 3);
			lcdDottedCircle(60+i*10, 100, 50, decodeRgbValue(0, 0, 0), 3);
		}

		LL_mDelay(300);
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  LL_FLASH_SetLatency(LL_FLASH_LATENCY_0);

  if(LL_FLASH_GetLatency() != LL_FLASH_LATENCY_0)
  {
  Error_Handler();
  }
  LL_RCC_HSI_Enable();

   /* Wait till HSI is ready */
  while(LL_RCC_HSI_IsReady() != 1)
  {

  }
  LL_RCC_HSI_SetCalibTrimming(16);
  LL_RCC_SetAHBPrescaler(LL_RCC_SYSCLK_DIV_1);
  LL_RCC_SetAPB1Prescaler(LL_RCC_APB1_DIV_1);
  LL_RCC_SetAPB2Prescaler(LL_RCC_APB1_DIV_1);
  LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_HSI);

   /* Wait till System clock is ready */
  while(LL_RCC_GetSysClkSource() != LL_RCC_SYS_CLKSOURCE_STATUS_HSI)
  {

  }
  LL_Init1msTick(8000000);
  LL_SYSTICK_SetClkSource(LL_SYSTICK_CLKSOURCE_HCLK);
  LL_SetSystemCoreClock(8000000);
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */

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
void assert_failed(char *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
