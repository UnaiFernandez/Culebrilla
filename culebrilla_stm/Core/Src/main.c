/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2023 STMicroelectronics.
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
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "../Inc/culebrilla.h"
#include <stdlib.h>
#include <stdio.h>
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
ADC_HandleTypeDef hadc1;

UART_HandleTypeDef huart2;

/* Definitions for pulsador */
osThreadId_t pulsadorHandle;
const osThreadAttr_t pulsador_attributes = {
  .name = "pulsador",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityHigh,
};
/* Definitions for juego */
osThreadId_t juegoHandle;
const osThreadAttr_t juego_attributes = {
  .name = "juego",
  .stack_size = 512 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for uart */
osThreadId_t uartHandle;
const osThreadAttr_t uart_attributes = {
  .name = "uart",
  .stack_size = 256 * 4,
  .priority = (osPriority_t) osPriorityHigh2,
};
/* Definitions for uart_rec */
osThreadId_t uart_recHandle;
const osThreadAttr_t uart_rec_attributes = {
  .name = "uart_rec",
  .stack_size = 256 * 4,
  .priority = (osPriority_t) osPriorityHigh6,
};
/* Definitions for display */
osThreadId_t displayHandle;
const osThreadAttr_t display_attributes = {
  .name = "display",
  .stack_size = 256 * 4,
  .priority = (osPriority_t) osPriorityAboveNormal7,
};
/* Definitions for board */
osThreadId_t boardHandle;
const osThreadAttr_t board_attributes = {
  .name = "board",
  .stack_size = 256 * 4,
  .priority = (osPriority_t) osPriorityAboveNormal,
};
/* Definitions for cola_juego */
osMessageQueueId_t cola_juegoHandle;
const osMessageQueueAttr_t cola_juego_attributes = {
  .name = "cola_juego"
};
/* Definitions for dir */
osMutexId_t dirHandle;
const osMutexAttr_t dir_attributes = {
  .name = "dir"
};
/* USER CODE BEGIN PV */
uint8_t state = 0; //El estado en el que se encuentra el  siendo 0= Inicio,1= En marcha,2= Pause,3= Finalizado,
uint8_t mode = 0;
typedef struct valores {
	uint32_t eje[2]; //Eje X=0 Y=1 Podriamos cambiarle el tamaño
	uint32_t joy[2];
	uint32_t puntos;
} valores_t;
//char buff[2];
uint8_t buff[2];
int dir;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART2_UART_Init(void);
static void MX_ADC1_Init(void);
void start_pulsador(void *argument);
void start_juego(void *argument);
void start_uart(void *argument);
void start_uart_rec(void *argument);
void start_display(void *argument);
void start_board(void *argument);

/* USER CODE BEGIN PFP */

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
  MX_USART2_UART_Init();
  MX_ADC1_Init();
  /* USER CODE BEGIN 2 */

  /* USER CODE END 2 */

  /* Init scheduler */
  osKernelInitialize();
  /* Create the mutex(es) */
  /* creation of dir */
  dirHandle = osMutexNew(&dir_attributes);

  /* USER CODE BEGIN RTOS_MUTEX */
	/* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
	/* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
	/* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* Create the queue(s) */
  /* creation of cola_juego */
  cola_juegoHandle = osMessageQueueNew (1, sizeof(valores_t), &cola_juego_attributes);

  /* USER CODE BEGIN RTOS_QUEUES */
	/* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* creation of pulsador */
  pulsadorHandle = osThreadNew(start_pulsador, NULL, &pulsador_attributes);

  /* creation of juego */
  juegoHandle = osThreadNew(start_juego, NULL, &juego_attributes);

  /* creation of uart */
  uartHandle = osThreadNew(start_uart, NULL, &uart_attributes);

  /* creation of uart_rec */
  uart_recHandle = osThreadNew(start_uart_rec, NULL, &uart_rec_attributes);

  /* creation of display */
  displayHandle = osThreadNew(start_display, NULL, &display_attributes);

  /* creation of board */
  boardHandle = osThreadNew(start_board, NULL, &board_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
	/* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_EVENTS */
	/* add events, ... */
  /* USER CODE END RTOS_EVENTS */

  /* Start scheduler */
  osKernelStart();

  /* We should never get here as control is now taken by the scheduler */
  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
	while (1) {
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

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief ADC1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_ADC1_Init(void)
{

  /* USER CODE BEGIN ADC1_Init 0 */

  /* USER CODE END ADC1_Init 0 */

  ADC_ChannelConfTypeDef sConfig = {0};

  /* USER CODE BEGIN ADC1_Init 1 */

  /* USER CODE END ADC1_Init 1 */

  /** Configure the global features of the ADC (Clock, Resolution, Data Alignment and number of conversion)
  */
  hadc1.Instance = ADC1;
  hadc1.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV2;
  hadc1.Init.Resolution = ADC_RESOLUTION_12B;
  hadc1.Init.ScanConvMode = ENABLE;
  hadc1.Init.ContinuousConvMode = ENABLE;
  hadc1.Init.DiscontinuousConvMode = DISABLE;
  hadc1.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
  hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;
  hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc1.Init.NbrOfConversion = 2;
  hadc1.Init.DMAContinuousRequests = DISABLE;
  hadc1.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
  if (HAL_ADC_Init(&hadc1) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time.
  */
  sConfig.Channel = ADC_CHANNEL_12;
  sConfig.Rank = 1;
  sConfig.SamplingTime = ADC_SAMPLETIME_480CYCLES;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time.
  */
  sConfig.Channel = ADC_CHANNEL_13;
  sConfig.Rank = 2;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN ADC1_Init 2 */

  /* USER CODE END ADC1_Init 2 */

}

/**
  * @brief USART2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART2_UART_Init(void)
{

  /* USER CODE BEGIN USART2_Init 0 */

  /* USER CODE END USART2_Init 0 */

  /* USER CODE BEGIN USART2_Init 1 */

  /* USER CODE END USART2_Init 1 */
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 115200;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART2_Init 2 */

  /* USER CODE END USART2_Init 2 */

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
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_14|GPIO_PIN_15|GPIO_PIN_0|GPIO_PIN_1
                          |GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7|GPIO_PIN_8
                          |GPIO_PIN_9|GPIO_PIN_12, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOH, GPIO_PIN_1, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7|GPIO_PIN_8
                          |GPIO_PIN_11|GPIO_PIN_12, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_12
                          |GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15|GPIO_PIN_3
                          |GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7|GPIO_PIN_8
                          |GPIO_PIN_9, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOD, GPIO_PIN_2, GPIO_PIN_RESET);

  /*Configure GPIO pin : PC13 */
  GPIO_InitStruct.Pin = GPIO_PIN_13;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pins : PC14 PC15 PC0 PC1
                           PC5 PC6 PC7 PC8
                           PC9 PC12 */
  GPIO_InitStruct.Pin = GPIO_PIN_14|GPIO_PIN_15|GPIO_PIN_0|GPIO_PIN_1
                          |GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7|GPIO_PIN_8
                          |GPIO_PIN_9|GPIO_PIN_12;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pin : PH1 */
  GPIO_InitStruct.Pin = GPIO_PIN_1;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOH, &GPIO_InitStruct);

  /*Configure GPIO pins : PA5 PA6 PA7 PA8
                           PA11 PA12 */
  GPIO_InitStruct.Pin = GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7|GPIO_PIN_8
                          |GPIO_PIN_11|GPIO_PIN_12;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : PB0 PB1 PB2 PB12
                           PB13 PB14 PB15 PB3
                           PB5 PB6 PB7 PB8
                           PB9 */
  GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_12
                          |GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15|GPIO_PIN_3
                          |GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7|GPIO_PIN_8
                          |GPIO_PIN_9;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pin : PD2 */
  GPIO_InitStruct.Pin = GPIO_PIN_2;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

  /*Configure GPIO pin : PB4 */
  GPIO_InitStruct.Pin = GPIO_PIN_4;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
	HAL_UART_Receive_IT(huart, buff, sizeof(buff));
	if (mode == TECLAS) {
		switch (buff[0]) {
		case 'r':
			osMutexAcquire(dirHandle, 10000);
			change_direction(&dir, R);
			osMutexRelease(dirHandle);

			break;
		case 'd':
			osMutexAcquire(dirHandle, 10000);
			change_direction(&dir, D);
			osMutexRelease(dirHandle);
			break;
		case 'l':
			osMutexAcquire(dirHandle, 10000);
			change_direction(&dir, L);
			osMutexRelease(dirHandle);
			break;
		case 'u':
			osMutexAcquire(dirHandle, 10000);
			change_direction(&dir, U);
			osMutexRelease(dirHandle);
			break;
		}
	}
}

uint8_t leer_pulsador(int pul) {

		return HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_13);

}
/* USER CODE END 4 */

/* USER CODE BEGIN Header_start_pulsador */
/**
 * @brief  Function implementing the pulsador thread.
 * @param  argument: Not used
 * @retval None
 */
/* USER CODE END Header_start_pulsador */
void start_pulsador(void *argument)
{
  /* USER CODE BEGIN 5 */
	/* Infinite loop */
	for (;;) {

		if (leer_pulsador(0) == 0) {
			switch (state) {
			case START:
				state = PLAY;
				break;
			case PLAY:
				state = PAUSA;
				break;
			case PAUSA:
				state = PLAY;
				break;
			}
		}

		osDelay(10);
	}
  /* USER CODE END 5 */
}

/* USER CODE BEGIN Header_start_juego */
/**
 * @brief Function implementing the juego thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_start_juego */
void start_juego(void *argument)
{
  /* USER CODE BEGIN start_juego */
	/* Infinite loop */

	valores_t datos_juego;
	//valores_t datos_joy;
	//osStatus_t status;

	coord_t s_pos;

	snake_t snake;
	food_t food;

	for (;;) {

		// para empezar en una direccion
		osMutexAcquire(dirHandle, 10000);
		dir = R;
		osMutexRelease(dirHandle);

		srand(0);

		startup_board();						//Pone la matriz apagada
		snake = spawn_snake();			//aparece el cuerpo de la serpiente

		food.pos = snake.body[0]; // para evitar un bug de spawn_food

		spawn_food(&food, &snake); //Aparecer comida en la matriz aleatoriamente

		while (state != PLAY) {
			osDelay(10);
		}

		while (snake.die == 0) { //Hasta que la serpiente muera estaremos en el while

			//TODO: pensar mas elegante
			s_pos = snake.body[snake.length - 1]; //Se coje la posicion inicial de la serpiente
			leds_off(s_pos.x, s_pos.y); //Se apaga la posicion de la posicion en la que estaba el cuerpo de la serpiente
			move_snake(&snake, dir); // Se mueve el cuerpo eliminado anteriormente a la siguiente posicion en la direccion establecida

			eat(&snake, &food); //Mas o menos entendido, sirve para vigilar si ha comido la fruta

			if (food.eaten == 1) {
				spawn_food(&food, &snake);
			}

			update_board(&snake, &food); //Para mostrar el cuerpo y comida dela serpiente en la matiz
			//sleep(1);
			datos_juego.eje[0] = snake.body[0].x;
			datos_juego.eje[1] = snake.body[0].y;
			datos_juego.joy[0] = 0;
			datos_juego.joy[1] = 0;
			datos_juego.puntos = snake.length;
			osMessageQueuePut(cola_juegoHandle, &datos_juego, 0U, 0U);

			while (state == PAUSA) {
				osDelay(10);
			}

			osDelay(SPEED);
		}
		state = PAUSA;
		osDelay(100);
	}

	//}
  /* USER CODE END start_juego */
}

/* USER CODE BEGIN Header_start_uart */
/**
 * @brief Function implementing the uart thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_start_uart */
void start_uart(void *argument)
{
  /* USER CODE BEGIN start_uart */
	/* Infinite loop */
	valores_t datos;
	osStatus_t status;
	uint8_t msg[] = "400,8,5,1";
	uint32_t x, y, puntos;
	int m;
	for (;;) {
		status = osMessageQueueGet(cola_juegoHandle, &datos, 0U, 0U);
		if (status == osOK) {
			x = datos.eje[0];
			y = datos.eje[1];
			puntos = datos.puntos;
			m = mode;
		}
			sprintf(msg, "%lu,%lu,%lu,%d", puntos - 1, x,
					y, m);
			HAL_UART_Transmit_IT(&huart2, msg, sizeof(msg));
		osDelay(20);
	}
  /* USER CODE END start_uart */
}

/* USER CODE BEGIN Header_start_uart_rec */
/**
 * @brief Function implementing the uart_rec thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_start_uart_rec */
void start_uart_rec(void *argument)
{
  /* USER CODE BEGIN start_uart_rec */
	/* Infinite loop */
	for (;;) {
		HAL_UART_Receive_IT(&huart2, buff, sizeof(buff));
		osDelay(1);
	}
  /* USER CODE END start_uart_rec */
}

/* USER CODE BEGIN Header_start_display */
/**
 * @brief Function implementing the display thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_start_display */
void start_display(void *argument)
{
  /* USER CODE BEGIN start_display */
	/* Infinite loop */
	int score;
	osStatus_t status;
	valores_t datos;
	for (;;) {
		status = osMessageQueueGet(cola_juegoHandle, &datos, 0U, 0U);
		if (status == osOK)
			score = datos.puntos - 1;
		print_score(state, score);
		osDelay(1);
	}
  /* USER CODE END start_display */
}

/* USER CODE BEGIN Header_start_board */
/**
* @brief Function implementing the board thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_start_board */
void start_board(void *argument)
{
  /* USER CODE BEGIN start_board */
  /* Infinite loop */
  for(;;)
  {
		print_board();
    osDelay(1);
  }
  /* USER CODE END start_board */
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
	/* User can add his own implementation to report the HAL error return state */
	__disable_irq();
	while (1) {
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
