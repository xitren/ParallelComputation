/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "cmsis_os.h"
#include "dma.h"
#include "i2c.h"
#include "spi.h"
#include "tim.h"
#include "usart.h"
#include "usb_host.h"
#include "gpio.h"

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
void MX_FREERTOS_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
void multiplyMatrices(int **first, int **second, int **result,
                      int r1, int c1, int r2, int c2) {
	for (int i = 0; i < r1; i++) {
		for (int j = 0; j < c2; j++) {
			result[i][j] = 0;
		}
	}
	for (int i = 0; i < r1; i++) {
		for (int j = 0; j < c2; j++) {
			for (int k = 0; k < c1; k++) {
				result[i][j] += first[i][k] * second[k][j];
			}
		}
	}
}

typedef union _tag_data_simd
{
	int16_t  s_int16[2];
	uint16_t  u_int16[2];
	int32_t  s_int32;
	uint32_t  u_int32;
} data_simd;

void multiplyMatricesSIMD(int16_t **first, int16_t **second, int32_t **result,
                      uint32_t r1, uint32_t c1, uint32_t r2, uint32_t c2) {
	data_simd s1, s2;
	for (uint32_t i = 0; i < r1; i++) {
		for (uint32_t j = 0; j < c2; j++) {
			result[i][j] = 0;
		}
	}
	for (uint32_t i = 0; i < r1; i++) {
		for (uint32_t j = 0; j < c2; j++) {
			for (uint32_t k = 0; k < c1; k+=2) {
				s1.s_int16[0] = first[i][k];
				s2.s_int16[0] = second[i][k];
				s1.s_int16[1] = first[i][k + 1];
				s2.s_int16[1] = second[i][k + 1];
//				p1 = val1[15:0]  * val2[15:0]
//				p2 = val1[31:16] * val2[31:16]
//				res[31:0] = p1 + p2 + val3[31:0]
				result[i][j] = (int32_t)__SMLAD(s1.u_int32, s2.u_int32,
					(uint32_t)result[i][j]);
			}
		}
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
  MX_DMA_Init();
  MX_I2C3_Init();
  MX_SPI4_Init();
  MX_SPI6_Init();
  MX_TIM9_Init();
  MX_TIM10_Init();
  MX_TIM11_Init();
  MX_TIM12_Init();
  MX_TIM14_Init();
  MX_USART1_UART_Init();
  MX_SPI2_Init();
  MX_USART2_UART_Init();
  /* USER CODE BEGIN 2 */

  /* USER CODE END 2 */

  /* Call init function for freertos objects (in freertos.c) */
  MX_FREERTOS_Init(); 

  HAL_GPIO_WritePin(LED_CH1_GPIO_Port, LED_CH1_Pin, GPIO_PIN_SET);
  HAL_GPIO_WritePin(LED_CH2_GPIO_Port, LED_CH2_Pin, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(LED_CH3_GPIO_Port, LED_CH3_Pin, GPIO_PIN_SET);
  HAL_GPIO_WritePin(LED_CH4_GPIO_Port, LED_CH4_Pin, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(LED_CH5_GPIO_Port, LED_CH5_Pin, GPIO_PIN_SET);
  HAL_GPIO_WritePin(LED_CH6_GPIO_Port, LED_CH6_Pin, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(LED_CH7_GPIO_Port, LED_CH7_Pin, GPIO_PIN_SET);
  HAL_GPIO_WritePin(LED_CH8_GPIO_Port, LED_CH8_Pin, GPIO_PIN_RESET);
  /* Start scheduler */
  osKernelStart();
  
  /* We should never get here as control is now taken by the scheduler */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
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

  /** Configure the main internal regulator output voltage 
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 168;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 7;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB busses clocks 
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
void assert_failed(uint8_t *file, uint32_t line)
{ 
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/





