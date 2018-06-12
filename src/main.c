#include <stdio.h>
#include <stdlib.h>
#include "main.h"

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

xQueueHandle xQueue;
static void vSenderTask(void *pvParameters)
{
	long lValueToSend;
	portBASE_TYPE xStatus;

	lValueToSend = (long)pvParameters;
	for (; ;)
	{

		xStatus = xQueueSendToBack(xQueue, &lValueToSend, 0);
		if (xStatus != pdPASS)
		{
			printf("Could not send to the queue.\n");
		}
		taskYIELD();
	}
}

static void vReceiverTask(void *pvParameters)
{
	long lReceivedValue;
	portBASE_TYPE xStatus;
	const portTickType xTicksToWait = 100 / portTICK_RATE_MS;

	for (; ;)
	{
		if (uxQueueMessagesWaiting(xQueue) != 0)
			printf("Queue should have been empty!\n");
		xStatus = xQueueReceive(xQueue, &lReceivedValue, xTicksToWait);
		if (xStatus == pdPASS)
			printf("Received = %ld\n", lReceivedValue);
		else
		{
			printf("cound not receive from the queue.\n");
		}
	}
}


int main(void)
{
	xQueue = xQueueCreate(5, sizeof(long));
	if (xQueue != NULL)
	{
		xTaskCreate(vSenderTask, 
				"Sender1",
				1000,
				(void *)100,
				1,
				NULL);
		xTaskCreate(vSenderTask, 
				"Sender2",
				1000,
				(void *)200,
				1,
				NULL);
		xTaskCreate(vReceiverTask, "Receiver", 1000, NULL, 2, NULL);
		vTaskStartScheduler();
	}
	else
	{
		printf("Error: can not create queue!\n");
	}

	for (; ;);

	return 0;
}

void vMainQueueSendPassed(void)
{
	;
}

void vApplicationIdleHook(void)
{
	;
}
