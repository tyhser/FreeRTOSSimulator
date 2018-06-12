#include <stdio.h>
#include <stdlib.h>
#include "main.h"

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#define mainSENDER_1 1
#define mainSENDER_2 2

xQueueHandle xQueue;
typedef struct
{
	unsigned char ucValue;
	unsigned char ucSource;
} xData;

static const xData xStructsToSend[2] = 
{
	{100, mainSENDER_1},
	{200, mainSENDER_2}
};

static void vSenderTask(void *pvParameters)
{
	portBASE_TYPE xStatus;
	const portTickType xTickToWait = 100 / portTICK_RATE_MS;
	for (; ;)
	{

		xStatus = xQueueSendToBack(xQueue, pvParameters, xTickToWait);
		if (xStatus != pdPASS)
		{
			printf("Could not send to the queue.\n");
		}
		taskYIELD();
	}
}

static void vReceiverTask(void *pvParameters)
{
	xData xReceivedStructure;
	portBASE_TYPE xStatus;

	while (1)
	{
		if (uxQueueMessagesWaiting(xQueue) != 3)
			printf("Queue should have been full!\n");

		xStatus = xQueueReceive(xQueue, &xReceivedStructure, 0);
		if (xStatus == pdPASS)
		{
			if(xReceivedStructure.ucSource == mainSENDER_1)
			{
				printf("From Sender 1 = %d\n", xReceivedStructure.ucValue);
			}
			else
			{
				printf("From Sender 2 = %d\n", xReceivedStructure.ucValue);
			}
		}
		else
			puts("Could not receive from the queue.\n");
	}
}


int main(void)
{
	xQueue = xQueueCreate(3, sizeof(xData));
	if (xQueue != NULL)
	{
		xTaskCreate(vSenderTask, 
				"Sender1",
				1000,
				&(xStructsToSend[0]), 	
				2,
				NULL);
		xTaskCreate(vSenderTask, 
				"Sender2",
				1000,
				&(xStructsToSend[1]), 	
				2,
				NULL);
		xTaskCreate(vReceiverTask, "Receiver", 1000, NULL, 1, NULL);
		vTaskStartScheduler();
	}
	else
	{
		printf("Error: can not create queue!\n");
	}

	for (; ;);
}

void vMainQueueSendPassed(void)
{
	;
}

void vApplicationIdleHook(void)
{
	;
}
