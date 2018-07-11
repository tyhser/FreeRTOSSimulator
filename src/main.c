#include <stdio.h>
#include <stdlib.h>
#include "main.h"

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

#define TASK1_STK_SIZE 100
#define TASK2_STK_SIZE 100
#define TASK1_PRIO 1
#define TASK2_PRIO 2

QueueHandle_t queue_handler;
void task1(void *pvParameters);

void task2(void *pvParameters);

enum task_index
{
	TASK1,
	TASK2,
	TASK_END
};


typedef struct 
{
	TaskFunction_t task;
	char *task_name;
	uint16_t task_stk_size;
	void *parameter;
	UBaseType_t task_prio;
	TaskHandle_t *task_handler;
}task_table_t;

TaskHandle_t task1_handler;
TaskHandle_t task2_handler;

task_table_t task_table[] = {
	{task1, "task1", TASK1_STK_SIZE, NULL, TASK1_PRIO, &task1_handler},
	{task2, "task2", TASK2_STK_SIZE, NULL, TASK2_PRIO, &task2_handler}
};


int main(void)
{

	for (UBaseType_t index = 0; index < TASK_END; index++)
	{
		xTaskCreate(task_table[index].task,
				task_table[index].task_name,
				task_table[index].task_stk_size,
				task_table[index].parameter,
				task_table[index].task_prio,
				task_table[index].task_handler);
	}

	vTaskStartScheduler();
	return 0;
}

void task1(void *pvParameters)
{
	queue_handler = xQueueCreate(4, sizeof(int));
	int sended = 0;	
	while (1)
	{
		printf("Task1\n");
		xQueueSend(queue_handler, &sended, 0);
		vTaskDelay(100);
		sended++;
	}
}

void task2(void *pvParameters)
{
	int buf = 0;
	while (1)
	{
		printf("Task2\n");
		vTaskDelay(10);
		xQueueReceive(queue_handler, &buf, 0);
		printf("receive: %d", (int)(buf));
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
