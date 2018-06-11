#include <stdio.h>
#include <stdlib.h>
#include "main.h"

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

#define START_TASK_PRIO	1
#define START_TSK_SIZE	128
TaskHandle_t StartTask_Handler;
void start_task(void *pvParameters);

#define LED0_TASK_PRIO 2
#define LED0_TSK_SIZE 50
TaskHandle_t LED0Task_Handler;
void led0_task(void *p_arg);

#define LED1_TASK_PRIO 3
#define LED1_TSK_SIZE 50
TaskHandle_t LED1Task_Handler;
void led1_task(void *p_arg);

#define FLOAT_TASK_PRIO 4
#define FLOAT_TSK_SIZE 128
TaskHandle_t FLOATTask_Handler;
void float_task(void *p_arg);

int main(void)
{
	xTaskCreate((TaskFunction_t)start_task, 
			(const char *)"start_task",
			(uint16_t )START_TSK_SIZE,
			(void *)NULL,
			(UBaseType_t)START_TASK_PRIO,
			(TaskHandle_t *)*&StartTask_Handler);
	vTaskStartScheduler();

	return 0;
}

void start_task(void *pvParameters)
{
	taskENTER_CRITICAL();

	xTaskCreate((TaskFunction_t)led0_task,
			(const char *)"led0_task",
			(uint16_t)LED0_TSK_SIZE,
			(void *)NULL,
			(UBaseType_t)LED0_TASK_PRIO,
			(TaskHandle_t *)&LED0Task_Handler);

	xTaskCreate((TaskFunction_t)led1_task,
			(const char *)"led1_task",
			(uint16_t)LED1_TSK_SIZE,
			(void *)NULL,
			(UBaseType_t)LED0_TASK_PRIO,
			(TaskHandle_t *)&LED1Task_Handler);

	xTaskCreate((TaskFunction_t)float_task,
			(const char *)"float_task",
			(uint16_t)FLOAT_TSK_SIZE,
			(void *)NULL,
			(UBaseType_t)FLOAT_TASK_PRIO,
			(TaskHandle_t *)&FLOATTask_Handler);
	vTaskDelete(StartTask_Handler);
	taskEXIT_CRITICAL();
}

void led0_task(void *pvParameters)
{
	static uint32_t StaticTest;
	while (1)
	{
		printf("LED0\n");
	/*	vTaskDelay(500);*/
		for (uint32_t i = 0; i < 0xffffffff; i++)
			;
		StaticTest++;
		if (StaticTest > 4)
			vTaskSuspend(0);
	}
}

void led1_task(void *pvParameters)
{
	while (1)
	{
		printf("LED1\n");
		vTaskDelay(80);
	}
}

void float_task(void *p_arg)
{
	static float float_num = 0.00;
	while (1)
	{
		float_num += 0.01f;
		printf("the value of float_num is : % .4f\n", float_num);
		vTaskDelay(10);
	}
}

void vMainQueueSendPassed(void)
{
	;
}

void vApplicationIdleHook(void)
{
	;
}
