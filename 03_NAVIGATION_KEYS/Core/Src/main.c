/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "stdbool.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define DEBOUNCE_DELAY_IN_MS		(uint32_t)200
#define UP							(uint8_t)0x00
#define DOWN						(uint8_t)0x01
#define CENTER						(uint8_t)0x02
#define LEFT						(uint8_t)0x03
#define RIGHT						(uint8_t)0x04
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
TIM_HandleTypeDef htim1;

/* USER CODE BEGIN PV */
typedef struct
{
	bool previous_state;
	bool current_state;
	uint32_t press_cnt;
	bool pressed_flag;
}navigation_struct_t;
typedef struct
{
	bool status;
	bool ovf_flag;
	uint32_t timeout_in_ms;
}user_timer_struct_t;

navigation_struct_t navigation_struct[5];
user_timer_struct_t user_timer_struct[5];

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_TIM1_Init(void);
/* USER CODE BEGIN PFP */
void navigation_keys_init(void);
void navigation_keys_handler(void);
void user_timer_init(void);
void user_timer_start(uint8_t timer_id, uint32_t timeout_in_ms);
void user_timer_stop(uint8_t timer_id);
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim);
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
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
  MX_TIM1_Init();
  /* USER CODE BEGIN 2 */
  HAL_TIM_Base_Start_IT(&htim1);
  user_timer_init();
  navigation_keys_init();
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
	  navigation_keys_handler();
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

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 100;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_3) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief TIM1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM1_Init(void)
{

  /* USER CODE BEGIN TIM1_Init 0 */

  /* USER CODE END TIM1_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM1_Init 1 */

  /* USER CODE END TIM1_Init 1 */
  htim1.Instance = TIM1;
  htim1.Init.Prescaler = 1;
  htim1.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim1.Init.Period = 49999;
  htim1.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim1.Init.RepetitionCounter = 0;
  htim1.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim1) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim1, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim1, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM1_Init 2 */

  /* USER CODE END TIM1_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(ON_BOARD_LED_GPIO_Port, ON_BOARD_LED_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : ON_BOARD_LED_Pin */
  GPIO_InitStruct.Pin = ON_BOARD_LED_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(ON_BOARD_LED_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : DOWN_KEY_Pin RIGHT_KEY_Pin CENTER_KEY_Pin LEFT_KEY_Pin
                           UP_KEY_Pin */
  GPIO_InitStruct.Pin = DOWN_KEY_Pin|RIGHT_KEY_Pin|CENTER_KEY_Pin|LEFT_KEY_Pin
                          |UP_KEY_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(EXTI0_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI0_IRQn);

  HAL_NVIC_SetPriority(EXTI1_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI1_IRQn);

  HAL_NVIC_SetPriority(EXTI2_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI2_IRQn);

  HAL_NVIC_SetPriority(EXTI3_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI3_IRQn);

  HAL_NVIC_SetPriority(EXTI4_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI4_IRQn);

}

/* USER CODE BEGIN 4 */

void navigation_keys_init(void)
{
	for(uint8_t i = 0; i<5; i++)
	{
		navigation_struct[i].current_state = 1;
		navigation_struct[i].previous_state = 1;
		navigation_struct[i].press_cnt = 0;
		navigation_struct[i].pressed_flag = false;
	}
}
void navigation_keys_handler(void)
{
	for(uint8_t button_id = 0; button_id<5; button_id++)
	{
		if(navigation_struct[button_id].pressed_flag == true)
		{
			if(user_timer_struct[button_id].ovf_flag == true)
			{
				user_timer_stop(button_id);
				navigation_struct[button_id].pressed_flag = false;
				switch(button_id)
				{
				case UP:
					navigation_struct[button_id].current_state = HAL_GPIO_ReadPin(UP_KEY_GPIO_Port, UP_KEY_Pin);
					break;
				case DOWN:
					navigation_struct[button_id].current_state = HAL_GPIO_ReadPin(DOWN_KEY_GPIO_Port, DOWN_KEY_Pin);
					break;
				case CENTER:
					navigation_struct[button_id].current_state = HAL_GPIO_ReadPin(CENTER_KEY_GPIO_Port, CENTER_KEY_Pin);
					break;
				case LEFT:
					navigation_struct[button_id].current_state = HAL_GPIO_ReadPin(LEFT_KEY_GPIO_Port, LEFT_KEY_Pin);
					break;
				case RIGHT:
					navigation_struct[button_id].current_state = HAL_GPIO_ReadPin(RIGHT_KEY_GPIO_Port, RIGHT_KEY_Pin);
					break;
				}
				if(navigation_struct[button_id].current_state != navigation_struct[button_id].previous_state)
				{
					navigation_struct[button_id].press_cnt += 1;
				}


			}
		}
	}
}
void user_timer_init(void)
{
	for(uint8_t i = 0; i<5; i++)
	{
		user_timer_struct[i].status = false;
		user_timer_struct[i].ovf_flag = false;
		user_timer_struct[i].timeout_in_ms = 0;
	}
}
void user_timer_start(uint8_t timer_id, uint32_t timeout_in_ms)
{
	user_timer_struct[timer_id].status = true;
	user_timer_struct[timer_id].timeout_in_ms = timeout_in_ms;
}


void user_timer_stop(uint8_t timer_id)
{
	user_timer_struct[timer_id].status = false;
	user_timer_struct[timer_id].ovf_flag = false;
	user_timer_struct[timer_id].timeout_in_ms = 0;
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	if(htim->Instance == htim1.Instance)
	{
		for(volatile uint8_t id = 0; id<5; id++)
		{
			if(user_timer_struct[id].status == true)
			{
				user_timer_struct[id].timeout_in_ms--;
				if(user_timer_struct[id].timeout_in_ms == 0)
				{
					user_timer_struct[id].ovf_flag = true;
				}
			}
		}
	}
}
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	switch(GPIO_Pin)
	{
	case CENTER_KEY_Pin:
		if(HAL_GPIO_ReadPin(CENTER_KEY_GPIO_Port, CENTER_KEY_Pin) == 0)
		{
			navigation_struct[CENTER].pressed_flag = true;
			navigation_struct[CENTER].previous_state = 0;
			user_timer_start(CENTER, DEBOUNCE_DELAY_IN_MS);
		}
		break;
	case UP_KEY_Pin:
		if(HAL_GPIO_ReadPin(UP_KEY_GPIO_Port, UP_KEY_Pin) == 0)
		{
			navigation_struct[UP].pressed_flag = true;
			navigation_struct[UP].previous_state = 0;
			user_timer_start(UP, DEBOUNCE_DELAY_IN_MS);
		}
		break;
	case DOWN_KEY_Pin:
		if(HAL_GPIO_ReadPin(DOWN_KEY_GPIO_Port, DOWN_KEY_Pin) == 0)
		{
			navigation_struct[DOWN].pressed_flag = true;
			navigation_struct[DOWN].previous_state = 0;
			user_timer_start(DOWN, DEBOUNCE_DELAY_IN_MS);
		}
		break;
	case LEFT_KEY_Pin:
		if(HAL_GPIO_ReadPin(LEFT_KEY_GPIO_Port, LEFT_KEY_Pin) == 0)
		{
			navigation_struct[LEFT].pressed_flag = true;
			navigation_struct[LEFT].previous_state = 0;
			user_timer_start(LEFT, DEBOUNCE_DELAY_IN_MS);
		}
		break;
	case RIGHT_KEY_Pin:
		if(HAL_GPIO_ReadPin(RIGHT_KEY_GPIO_Port, RIGHT_KEY_Pin) == 0)
		{
			navigation_struct[RIGHT].pressed_flag = true;
			navigation_struct[RIGHT].previous_state = 0;
			user_timer_start(RIGHT, DEBOUNCE_DELAY_IN_MS);
		}
		break;
	default:
		break;
	}
}
/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
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
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
