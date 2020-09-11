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
#include "can.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define isSet(val, bit_no) (((val) >> (bit_no)) & 1)
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
// CAN Message variables

CAN_TxHeaderTypeDef TxHeader;
CAN_RxHeaderTypeDef RxHeader;

uint8_t RxData[8];
uint32_t TxMailbox;

uint8_t result;

uint8_t heartbeat[1];
uint8_t sourceID[2];
uint8_t autonomous[1];
uint8_t messageType[1];
uint8_t extraID[2];
uint8_t dataLength[1];
uint8_t data[1];
uint8_t TxData[8];

const bool sender = false;

// UART char buffer
char msg[140];

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
//CALL BACK
void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan);
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart);

//USART IRQ handler
void USART2_IRQHandler(void);

//Convert to actual value
double To_Decimal(uint8_t input);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
 * @brief  The application entry point.
 * @retval int
 */
int main(void) {
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
	MX_CAN_Init();
	MX_USART2_UART_Init();
	/* USER CODE BEGIN 2 */

	// Confirm startup to terminal
	snprintf(msg, sizeof(msg) - 1, "Initialization Succeed!!\r\n");
	printf("hi there!");
	HAL_UART_Transmit(&huart2, (uint8_t*) msg, strlen(msg), HAL_MAX_DELAY);

	/* USER CODE END 2 */

	/* Infinite loop */
	/* USER CODE BEGIN WHILE */
	while (1) {

		if(sender){

		snprintf(msg, sizeof(msg) - 1, "Input the external ID here! \r\n");
		HAL_UART_Transmit(&huart2, (uint8_t*) msg, strlen(msg), HAL_MAX_DELAY);
		snprintf(msg, sizeof(msg) - 1, "\r\n");
		HAL_UART_Transmit(&huart2, (uint8_t*) msg, strlen(msg), HAL_MAX_DELAY);

		//input heartbeat
		snprintf(msg, sizeof(msg) - 1, "Heart is turned on? (0/1):");
		HAL_UART_Transmit(&huart2, (uint8_t*) msg, strlen(msg), HAL_MAX_DELAY);
		HAL_UART_Receive(&huart2, (uint8_t*)heartbeat, 1, HAL_MAX_DELAY);
//		heartbeat[0] = To_Decimal(heartbeat[0]);
		snprintf(msg, sizeof(msg) - 1, "\r\n");
		HAL_UART_Transmit(&huart2, (uint8_t*) msg, strlen(msg), HAL_MAX_DELAY);

		//input source ID
		snprintf(msg, sizeof(msg) - 1, "Source ID (in hexadecimal): 0x");
		HAL_UART_Transmit(&huart2, (uint8_t*) msg, strlen(msg), HAL_MAX_DELAY);
		HAL_UART_Receive(&huart2, (uint8_t*)sourceID, 2, HAL_MAX_DELAY);
		long int hexSourceID;
		hexSourceID = 0;
		for (int i = 0; i < 2; i++){
			//sourceID[i] = To_Decimal(sourceID[i]);
			hexSourceID = hexSourceID + sourceID[i] * pow(16,(2 - i));
		}
		snprintf(msg, sizeof(msg) - 1, "\r\n");
		HAL_UART_Transmit(&huart2, (uint8_t*) msg, strlen(msg), HAL_MAX_DELAY);

		//input autonomous
		snprintf(msg, sizeof(msg) - 1, "Autonomous is turned on? (0/1): ");
		HAL_UART_Transmit(&huart2, (uint8_t*) msg, strlen(msg), HAL_MAX_DELAY);
		HAL_UART_Receive(&huart2, (uint8_t*)autonomous, 1, HAL_MAX_DELAY);
//		autonomous[0] = To_Decimal(autonomous[0]);
		snprintf(msg, sizeof(msg) - 1, "\r\n");
		HAL_UART_Transmit(&huart2, (uint8_t*) msg, strlen(msg), HAL_MAX_DELAY);

		//input message types
		snprintf(msg, sizeof(msg) - 1, "Message Type (in hexadecimal): 0x");
		HAL_UART_Transmit(&huart2, (uint8_t*) msg, strlen(msg), HAL_MAX_DELAY);
		HAL_UART_Receive(&huart2, (uint8_t*)messageType, 1, HAL_MAX_DELAY);
//		messageType[0] = To_Decimal(messageType[0]);
		snprintf(msg, sizeof(msg) - 1, "\r\n");
		HAL_UART_Transmit(&huart2, (uint8_t*) msg, strlen(msg), HAL_MAX_DELAY);

		//input extra ID
		snprintf(msg, sizeof(msg) - 1, "Extra ID (in hexadecimal): 0x");
		HAL_UART_Transmit(&huart2, (uint8_t*) msg, strlen(msg), HAL_MAX_DELAY);
		HAL_UART_Receive(&huart2, (uint8_t*)extraID, 2, HAL_MAX_DELAY);
		long int hexExtraID;
		hexExtraID = 0;
		for (int i = 0; i < 2; i++){
//			extraID[i] = To_Decimal(extraID[i]);
			hexExtraID = hexExtraID + extraID[i] * pow(16,(3 - i));
		}
		snprintf(msg, sizeof(msg) - 1, "\r\n");
		HAL_UART_Transmit(&huart2, (uint8_t*) msg, strlen(msg), HAL_MAX_DELAY);

		//input the data length
		snprintf(msg, sizeof(msg) - 1, "Input the data length: ");
		HAL_UART_Transmit(&huart2, (uint8_t*) msg, strlen(msg), HAL_MAX_DELAY);
		HAL_UART_Receive(&huart2, (uint8_t*)dataLength, 1, HAL_MAX_DELAY);
//		dataLength[0] = To_Decimal(dataLength[0]);
		snprintf(msg, sizeof(msg) - 1, "\r\n");
		HAL_UART_Transmit(&huart2, (uint8_t*) msg, strlen(msg), HAL_MAX_DELAY);

		//input data
		snprintf(msg, sizeof(msg) - 1, "Input the data! \r\n ");
		HAL_UART_Transmit(&huart2, (uint8_t*) msg, strlen(msg), HAL_MAX_DELAY);
		for (int i = 0; i < dataLength[0]; i++){
			snprintf(msg, sizeof(msg) - 1, "Data[%d]: ",i);
			HAL_UART_Transmit(&huart2, (uint8_t*) msg, strlen(msg), HAL_MAX_DELAY);
			HAL_UART_Receive(&huart2, (uint8_t*)data, 1, HAL_MAX_DELAY);
			TxData[i] = data[i];
//			for (int j = 0; j < 2; j++){
//				data[j] = To_Decimal(data[j]);
//				TxData[i] = TxData[i] + data[j] * pow(16,(1-j));
//			}
		snprintf(msg, sizeof(msg) - 1, "\r\n ");
		HAL_UART_Transmit(&huart2, (uint8_t*) msg, strlen(msg), HAL_MAX_DELAY);
		}

		//sending the message
		TxHeader.ExtId = (heartbeat[0] << 28) + (hexSourceID << 19) + (autonomous[0] << 18) + (messageType[0] << 15) + hexExtraID;
		TxHeader.RTR = CAN_RTR_DATA;
		TxHeader.IDE = CAN_ID_EXT;
		TxHeader.DLC = dataLength[0];
		TxHeader.TransmitGlobalTime = DISABLE;

			// Request transmisison
			if (HAL_CAN_AddTxMessage(&hcan, &TxHeader, TxData, &TxMailbox)
					!= HAL_OK) {
				Error_Handler();
			}

			snprintf(msg, sizeof(msg) - 1, "byte 0:  %d\r\n", TxData[0]);
			HAL_UART_Transmit(&huart2, (uint8_t*) msg, strlen(msg),
					HAL_MAX_DELAY);
		}
	}
	HAL_Delay(1000);
		/* USER CODE END WHILE */
}
		 //Blocking delay for main loop
	/* USER CODE END 3 */


/**
 * @brief System Clock Configuration
 * @retval None
 */
void SystemClock_Config(void) {
	RCC_OscInitTypeDef RCC_OscInitStruct = { 0 };
	RCC_ClkInitTypeDef RCC_ClkInitStruct = { 0 };

	/** Initializes the CPU, AHB and APB busses clocks
	 */
	RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
	RCC_OscInitStruct.HSIState = RCC_HSI_ON;
	RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
	RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
	if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) {
		Error_Handler();
	}
	/** Initializes the CPU, AHB and APB busses clocks
	 */
	RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK
			| RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
	RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
	RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
	RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
	RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

	if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK) {
		Error_Handler();
	}
}

/* USER CODE BEGIN 4 */

//convert ASCII value to actual value
double To_Decimal(uint8_t input){
	if (input >= 48 && input <= 57){
		result = input - 48;
	}
	else if (input >= 65 && input <= 70){
		result = input - 55;
	}
	else {
		result = 0;
	}
	return result;
}

// Override weak-ass function from stm32l4xx_hal_can.c
/**
 * @brief System Clock Configuration
 * @retval None
 */
void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan) {
	// called when there's a message waiting in the FIFO RX buffer

	snprintf(msg, sizeof(msg) - 1, "CALLBACK \r\n");
	HAL_UART_Transmit(&huart2, (uint8_t*) msg, strlen(msg),
			HAL_MAX_DELAY);

	if (HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO0, &RxHeader, RxData) != HAL_OK) {
		/* Reception Error */
		Error_Handler();
	} else {
		// message read successfully

		// Print ID and first data byte to serial
		snprintf(msg, sizeof(msg) - 1,
				"Extended ID: 0x%lX  Data[0]: 0x%X  Data[1]: 0x%X Data[2]: 0x%X Data[3]: 0x%X Data[4]: 0x%X Data[5]: 0x%X Data[6]: 0x%X Data[7]: 0x%X\r\n",
				 RxHeader.ExtId, RxData[0], RxData[1], RxData[2], RxData[3], RxData[4], RxData[5], RxData[6], RxData[7]);


		if (HAL_UART_Transmit(&huart2, (uint8_t*) msg, strlen(msg),
		HAL_MAX_DELAY) != HAL_OK) {

			Error_Handler();
		}
	}
}

// Handle CAN Rx fifo callback (cannot test in loopback mode)
// Overrides stm32l4xx_hal_can.c
void HAL_CAN_RxFifo0FullCallback(CAN_HandleTypeDef *hcan) {
	char msg[80];

	snprintf(msg, sizeof(msg) - 1, "RX FIFO0 Full callback\r\n");
	HAL_UART_Transmit(&huart2, (uint8_t*) msg, strlen(msg), HAL_MAX_DELAY);
}

const char* getSourceID()
{
	int value = (RxHeader.ExtId << 3) >> (3 + 19);
	if (0 <= value && value <= 3)
	{
		return "External Master";
	}
	if (4 <= value && value <= 5)
	{
		return "Chassis Controller";
	}
	if (6 <= value && value <= 7)
	{
		return "AMS";
	}
	if (8 <= value && value <= 11)
	{
		return "Shutdown";
	}
	if (12 <= value && value <= 13)
	{
		return "Shutdown - BPSD";
	}
	if (14 <= value && value <= 15)
	{
		return "Shutdown - Current";
	}
	if (16 <= value && value <= 17)
	{
		return "PDM";
	}
	if (18 <= value && value <= 19)
	{
		return "Steering Wheel";
	}
	if (20 <= value && value <= 21)
	{
		return "Dashboard";
	}
	if (22 <= value && value <= 23)
	{
		return "BMS";
	}
	if (24 <= value && value <= 25)
	{
		return "IMD";
	}
	if (26 <= value && value <= 27)
	{
		return "Sensors";
	}
	if (32 <= value && value <= 28)
	{
		return "Sensors";
	}
	else
	{
		return "Reserved";
	}
	return "0";
}

/* USER CODE END 4 */

/**
 * @brief  This function is executed in case of error occurrence.
 * @retval None
 */
void Error_Handler(void) {
	/* USER CODE BEGIN Error_Handler_Debug */
	/* User can add his own implementation to report the HAL error return state */

	uint32_t error = HAL_CAN_GetError(&hcan);

	snprintf(msg, sizeof(msg) - 1, "error: %ld\r\n", error);
	HAL_UART_Transmit(&huart2, (uint8_t*) msg, strlen(msg), HAL_MAX_DELAY);

	/* USER CODE END Error_Handler_Debug */
}

void USART2_IRQHandler(void)
{
  HAL_UART_IRQHandler(&huart2);
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
