/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
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
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "dbger.h"
#include "gt911.h"
#include "usb_device.h"
#include "fatfs_file_handler.h"
#include "atk_ncr.h"
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
/* USER CODE BEGIN Variables */
StoreDisk_t sDisk[_VOLUMES] = {
	// path       FATFS       FIL        opt     mCnt
	{SDPath, 	&SDFatFS, 	&SDFile, 	FM_FAT32, 0},		// SDcard disk
	//{USERPath, 	&USERFatFS, &USERFile, 	FM_FAT,   0}		// SpiFlash disk
};

extern TIM_HandleTypeDef htim2;
extern LTDC_HandleTypeDef hltdc;
/* USER CODE END Variables */
/* Definitions for TouchGFX */
osThreadId_t TouchGFXHandle;
const osThreadAttr_t TouchGFX_attributes = {
  .name = "TouchGFX",
  .stack_size = 2048 * 4,
  .priority = (osPriority_t) osPriorityLow,
};
/* Definitions for cmd_process */
osThreadId_t cmd_processHandle;
const osThreadAttr_t cmd_process_attributes = {
  .name = "cmd_process",
  .stack_size = 1024 * 4,
  .priority = (osPriority_t) osPriorityAboveNormal,
};
/* Definitions for handWrite */
osThreadId_t handWriteHandle;
const osThreadAttr_t handWrite_attributes = {
  .name = "handWrite",
  .stack_size = 1024 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for semGetPoint */
osSemaphoreId_t semGetPointHandle;
const osSemaphoreAttr_t semGetPoint_attributes = {
  .name = "semGetPoint"
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */
void LTDC_draw_line(int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint8_t width);
/* USER CODE END FunctionPrototypes */

void TouchGFX_Task(void *argument);
void cmd_process_Task(void *argument);
void handWriteTask(void *argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* Create the semaphores(s) */
  /* creation of semGetPoint */
  semGetPointHandle = osSemaphoreNew(1, 0, &semGetPoint_attributes);

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* creation of TouchGFX */
  TouchGFXHandle = osThreadNew(TouchGFX_Task, NULL, &TouchGFX_attributes);

  /* creation of cmd_process */
  cmd_processHandle = osThreadNew(cmd_process_Task, NULL, &cmd_process_attributes);

  /* creation of handWrite */
  handWriteHandle = osThreadNew(handWriteTask, NULL, &handWrite_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_EVENTS */
  /* add events, ... */
  /* USER CODE END RTOS_EVENTS */

}

/* USER CODE BEGIN Header_TouchGFX_Task */
/**
  * @brief  Function implementing the TouchGFX thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_TouchGFX_Task */
__weak void TouchGFX_Task(void *argument)
{
  /* USER CODE BEGIN TouchGFX_Task */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END TouchGFX_Task */
}

/* USER CODE BEGIN Header_cmd_process_Task */
/**
* @brief Function implementing the cmd_process thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_cmd_process_Task */
__weak void cmd_process_Task(void *argument)
{
  /* USER CODE BEGIN cmd_process_Task */
	GT911_STA_t gt911_sta = GT911_STA_OK;
	//HAL_NVIC_DisableIRQ(EXTI9_5_IRQn);		// delete it if GT911 is NOT work in interrupt mode;
	gt911_sta = gt911_init();
	if(GT911_STA_OK != gt911_sta) {
		LOG_ERR("GT911 init err[%d]\n", gt911_sta);
	} else {
		LOG_DBG("GT911 init OK\n");
		//HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);		// delete it if GT911 is NOT work in interrupt mode;
	}
	
	FH_mount(&sDisk[0]);
	FH_scan(&sDisk[0], NULL);
	MX_USB_DEVICE_Init();
	
	osDelay(1000);	// wait for screen be stable
	HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_1);
	__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, 100);
	
	alientek_ncr_init();
	
	osSemaphoreRelease(semGetPointHandle);
	
//	for(uint32_t i = 0; i < 90000; i++) {
//		*((uint8_t*)LTDC_L1_ADDR + i) = 0xF0 + (i / 600) % 0x10;
//	}
	
	
  /* Infinite loop */
  for(;;)
  {
	  osDelay(1);
	  
//	  osDelay(1000);
//	  HAL_LTDC_SetAlpha(&hltdc, 0, LTDC_LAYER_2);
//	  osDelay(1000);
//	  HAL_LTDC_SetAlpha(&hltdc, 255, LTDC_LAYER_2);
	  
  }
  /* USER CODE END cmd_process_Task */
}

/* USER CODE BEGIN Header_handWriteTask */
/**
* @brief Function implementing the handWrite thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_handWriteTask */
__weak void handWriteTask(void *argument)
{
  /* USER CODE BEGIN handWriteTask */
  /* Infinite loop */
	for(;;)
	{
		osSemaphoreAcquire(semGetPointHandle, osWaitForever);
		gt911_get_touch(touch_coordinate, &num_touched);
		if(num_touched == 0) {
			osDelay(2);
			osSemaphoreRelease(semGetPointHandle);
		} else {
			//LOG_DBG("point(%d, %d)\n", touch_coordinate[0].x, touch_coordinate[0].y);
			if(touch_coordinate[0].x > LTDC_L1_START_X && touch_coordinate[0].x < LTDC_L1_START_X + LTDC_L1_WIDTH &&
				 touch_coordinate[0].y > LTDC_L1_START_Y && touch_coordinate[0].y < LTDC_L1_START_Y + LTDC_L1_HEIGHT)
			{
				static COORDINATE_t last_point = {.x = 0, .y = 0};
				static uint32_t last_point_tick = 0;
				if(last_point.x == touch_coordinate[0].x && last_point.y == touch_coordinate[0].y) {
					last_point_tick = HAL_GetTick();
				} else {
					#if 0		// coordinate turn 90 dgr test
					int16_t temp = touch_coordinate[0].x;
					touch_coordinate[0].x = (touch_coordinate[0].y - 240) + 350;
					touch_coordinate[0].y = (350 - temp + 240);
					#endif
					
					uint32_t now = HAL_GetTick();
					if(now - last_point_tick > 20) {
						last_point.x = touch_coordinate[0].x;
						last_point.y = touch_coordinate[0].y;
					} else {
						// draw line
						#define LINE_WIDTH	2
						//LOG_DBG("(%d, %d) --> (%d, %d)\n", last_point.x, last_point.y, touch_coordinate[0].x, touch_coordinate[0].y);
						if(last_point.x < LTDC_L1_START_X + LINE_WIDTH) {
							last_point.x = LTDC_L1_START_X + LINE_WIDTH;
						}
						if(last_point.x > LTDC_L1_START_X + LTDC_L1_WIDTH - LINE_WIDTH) {
							last_point.x = LTDC_L1_START_X + LTDC_L1_WIDTH - LINE_WIDTH;
						}
						if(last_point.y < LTDC_L1_START_Y + LINE_WIDTH) {
							last_point.y = LTDC_L1_START_Y + LINE_WIDTH;
						}
						if(last_point.y > LTDC_L1_START_Y + LTDC_L1_HEIGHT - LINE_WIDTH) {
							last_point.y = LTDC_L1_START_Y + LTDC_L1_HEIGHT - LINE_WIDTH;
						}
						if(touch_coordinate[0].x < LTDC_L1_START_X + LINE_WIDTH) {
							touch_coordinate[0].x = LTDC_L1_START_X + LINE_WIDTH;
						}
						if(touch_coordinate[0].x > LTDC_L1_START_X + LTDC_L1_WIDTH - LINE_WIDTH) {
							touch_coordinate[0].x = LTDC_L1_START_X + LTDC_L1_WIDTH - LINE_WIDTH;
						}
						if(touch_coordinate[0].y < LTDC_L1_START_Y + LINE_WIDTH) {
							touch_coordinate[0].y = LTDC_L1_START_Y + LINE_WIDTH;
						}
						if(touch_coordinate[0].y > LTDC_L1_START_Y + LTDC_L1_HEIGHT - LINE_WIDTH) {
							touch_coordinate[0].y = LTDC_L1_START_Y + LTDC_L1_HEIGHT - LINE_WIDTH;
						}
						//LOG_DBG("line:(%d, %d) -> (%d, %d)\n", last_point.x, last_point.y, touch_coordinate[0].x, touch_coordinate[0].y);
						LTDC_draw_line(last_point.x, last_point.y, touch_coordinate[0].x, touch_coordinate[0].y, LINE_WIDTH);
						//HAL_LTDC_SetAlpha(&hltdc, 255, LTDC_LAYER_2);
						last_point.x = touch_coordinate[0].x;
						last_point.y = touch_coordinate[0].y;
						if(point_num > MAX_POINT - 20) {
							LOG_WAR("MAX_POINT[%d] too small\n", MAX_POINT);
						}
					}
					last_point_tick = now;
				}
				osSemaphoreRelease(semGetPointHandle);
			}
		}
	}
  /* USER CODE END handWriteTask */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */

