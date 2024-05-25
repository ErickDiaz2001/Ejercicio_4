/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
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
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdio.h>
#include <string.h>
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
typedef enum commandsEnum {
	COMMAND_LED1_TOGGLE = 'a',
	COMMAND_LED2_TOGGLE = 's',
	COMMAND_LED3_TOGGLE = 'd',
	COMMAND_LED4_TOGGLE = 'f'
} commands_LED_t;

typedef enum BUTTON_STATE {
	TECLA_INACTIVA,
	TECLA_PRESIONADA,
	TECLA_LIBERADA
} button_state_t;

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
//variables de RX
uint8_t dato = 0;

//varibles boton
button_state_t estado_boton [2]= {TECLA_INACTIVA, TECLA_INACTIVA};
uint8_t contador[2] = {0,0};
uint8_t valor_swich_anterior [2]= {0,0};
uint8_t valor_swich [2]= {0,0};
uint8_t TIEMPO_DEBOUNCE = 10;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
int __io_putchar(int ch)//habilita la funcion printf
{
	HAL_UART_Transmit(&huart3, (uint8_t *)&ch, 1, 100);
	return ch;
}

//funcion para el debouncing
//pin_tecla solo para puertos A
// numero de tecla para guardar los datos individuales de cada boton en un array
// el tiempo de debounce es de 10ms
void debounce(uint8_t pin_tecla, uint8_t i)
{
	switch (estado_boton[i])
	{
	case TECLA_INACTIVA:
		valor_swich[i]= HAL_GPIO_ReadPin(GPIOA, pin_tecla);//se lee el estado del boton
		if (valor_swich[i] == 1)
		{
			valor_swich_anterior[i] = valor_swich[i];
			estado_boton[i] = TECLA_PRESIONADA;
		}
		break;
	case TECLA_PRESIONADA:
		valor_swich[i] = HAL_GPIO_ReadPin(GPIOA, pin_tecla);//se lee el estado del boton
		if (valor_swich[i] == valor_swich_anterior[i] && contador[i] > TIEMPO_DEBOUNCE)//comparamos valor anterior con el valor actual
		{
			printf("Tecla apretada: TEC%d\n\r", i);
			valor_swich_anterior[i] = 0;
			contador[i] = 0;
			estado_boton[i] = TECLA_LIBERADA;
		}
		else
		{
			contador[i] = 0;
			estado_boton[i] = TECLA_INACTIVA;
		}
		contador[i] = contador[i] + 1;
		break;
	case TECLA_LIBERADA:
		valor_swich[i] = HAL_GPIO_ReadPin(GPIOA, pin_tecla);//se lee el estado del boton
		if (valor_swich[i] == 0)
		{
			printf("Tecla liberada: TEC%d\n\r", i);
			estado_boton[i] = TECLA_INACTIVA;
		}
		break;
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
  MX_USART3_UART_Init();
  MX_TIM2_Init();
  /* USER CODE BEGIN 2 */
  HAL_UART_Receive_IT(&huart3, &dato, 1);
  HAL_TIM_Base_Start_IT(&htim2);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
	  switch (dato)
	  {
	  case COMMAND_LED1_TOGGLE:
		  HAL_GPIO_TogglePin(GPIOA, LED_1_Pin);
		  break;

	  case COMMAND_LED2_TOGGLE:
		  HAL_GPIO_TogglePin(GPIOA, LED_2_Pin);
		  break;

	  case COMMAND_LED3_TOGGLE:
		  HAL_GPIO_TogglePin(GPIOA, LED_3_Pin);
		  break;

	  case COMMAND_LED4_TOGGLE:
		  HAL_GPIO_TogglePin(GPIOA, LED_4_Pin);
		  break;
	  }
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

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
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

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	if(huart->Instance == USART3)
	{
		HAL_UART_Receive_IT(&huart3, &dato, 1);//recivimos el dato y lo guardamos
	}
}

//interrupcion cada 1ms
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  if(htim->Instance == TIM2)
  {
	  debounce(TECLA_1_Pin, 0);
	  debounce(TECLA_2_Pin, 1);
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
