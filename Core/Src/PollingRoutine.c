/*
 * PollingRoutine.c
 *
 *  Created on: Oct 24, 2023
 *      Author: karl.yamashita
 *
 *
 *      Template for projects.
 *
 *      The object of this PollingRoutine.c/h files is to not have to write code in main.c which already has a lot of generated code.
 *      It is cumbersome having to scroll through all the generated code for your own code and having to find a USER CODE section so your code is not erased when CubeMX re-generates code.
 *      
 *      Direction: Call PollingInit before the main while loop. Call PollingRoutine from within the main while loop
 * 
 *      Example;
        // USER CODE BEGIN WHILE
        PollingInit();
        while (1)
        {
            PollingRoutine();
            // USER CODE END WHILE

            // USER CODE BEGIN 3
        }
        // USER CODE END 3



	Description: The STM32 communicates with the LTM4686 by I2C. The STM32 receives UART2 commands to change the VOUT Command and other registers.
				Timer interrupt initiates the ADC sequencer to get the ADC values for LTM4686_Sns and OP_Sns. The ADC values are
				calculated and added to the telemetry packet. The I_OCD pins are monitored and added to the telemetry packet as fault bit.
				The Telemetry packet gets sent every 10ms over UART1.

	Notes on Bootloader FW:
				The Bootloader takes ~32K of memory if not optimized.
				We use optimization to reduce bootloader code, so we can use address 0x08007000 - 0x8008000 which is freed up memory.
				We can use this freed up memory for saving data like serial number, part number, etc., read/written by application FW.
				This memory section is not erased during FW update so stays persistent.

	Notes on Application FW:
				The application starts at address 0x8008000.

				User will need to modify system_stm32g0xx.c file. This is an IMPORTANT step else application won't run.
				1. Un-comment #define USER_VECT_TAB_ADDRESS (line 117)
				2. Modify VECT_TAB_OFFSET to 0x08008000 (line 131)

				User will need to modify STM32G071RBTX_FLASH.ld file. This keeps the compiler from using the bootloader memory space
				for the application.
				1. In MEMORY definition, modify FLASH Origin to 0x08008000 and LENGTH to 96K


	Nucleo-G071RB: I2C2
				PA12 (SCL) - CN10.12
				PA11 (SDA) - CN10.14
				GND - CN10.20

	Versions:
		1.0.1 (9/21/2024) estimated start project -


 */


#include "main.h"


const char version[] = "v1.0.1";



extern UART_HandleTypeDef huart2;


// UART2 for Command
UART_DMA_QueueStruct uart2_msg =
{
	.huart = &huart2,
	.rx.queueSize = UART_DMA_QUEUE_SIZE,
	.tx.queueSize = UART_DMA_QUEUE_SIZE,
	.rx.dma_ptr.SkipOverFlow = true
};


/*
 * Description: Initialize other routines before main while loop.
 *
 */
void PollingInit(void)
{
	// Enable UART2 interrupt
	UART_DMA_EnableRxInterruptIdle(&uart2_msg);

	// start blinking SOH LED
	TimerCallbackRegisterOnly(&timerCallback, GPIO_LED_Green_Toggle);
	TimerCallbackTimerStart(&timerCallback, GPIO_LED_Green_Toggle, 500, true);

	// let the user know we're up and running
	MCU_Prompt();
}

/*
 * Description: Called from main while loop.
 */
void PollingRoutine(void)
{
	TimerCallbackCheck(&timerCallback);

	UART_DMA_ParseCircularBuffer(&uart2_msg);

	UART_Parse(&uart2_msg);
}

/*
 * Description: Parse Command messages.
 */
void UART_Parse(UART_DMA_QueueStruct *msg)
{
	int status = 0;
	char *ptr;
	char retStr[128] = "OK";
	char msg_copy[128] = {0};

	if(UART_DMA_MsgRdy(msg))
	{
		Nop();

		memcpy(&msg_copy, msg->rx.msgToParse->data, strlen((char*)msg->rx.msgToParse->data) - 2); // remove CR/LF

		ptr = (char*)msg->rx.msgToParse->data;

		// remove spaces and lower case
		RemoveSpaces(ptr);
		ToLower(ptr);

		if(strncmp((char*)ptr, "set", strlen("set")) == 0)
		{
			ptr += strlen("set");
			status = ParseSet(ptr);
		}
		else if(strncmp((char*)ptr, "get", strlen("get")) == 0)
		{
			ptr += strlen("get");
			status = ParseGet(ptr, retStr);
		}
		else
		{
			status = COMMAND_UNKNOWN;
		}

		// check return status
		if(status == NO_ACK)
		{
			return;
		}
		else if(status != 0) // other return status other than NO_ACK or NO_ERROR
		{
			PrintError(msg, msg_copy, status);
		}
		else // NO_ERROR
		{
			PrintReply(msg, msg_copy, retStr);
		}

		memset(&msg->rx.msgToParse->data, 0, UART_DMA_DATA_SIZE); // clear current buffer index
	}
}

/*
 * Description: Parse Get commands
 */
int ParseGet(char *msg, char *retStr)
{
	int status = NO_ERROR;

	if(strncmp(msg, "fwversion", strlen("fwversion"))== 0)
	{
		sprintf(retStr, "%s", version);
	}
	else
	{
		status = COMMAND_UNKNOWN;
	}

	return status;
}

/*
 * Description: Parse Set commands
 */
int ParseSet(char *msg)
{
	int status = NO_ERROR;

	if(strncmp(msg, "ledtoggle:", strlen("ledtoggle:")) == 0)
	{
		msg += strlen("ledtoggle:");
		if(atoi(msg))
		{
			TimerCallbackTimerStart(&timerCallback, GPIO_LED_Green_Toggle, 500, TIMER_REPEAT);
		}
		else
		{
			TimerCallbackDisable(&timerCallback, GPIO_LED_Green_Toggle);
		}
	}
	else
	{
		status = COMMAND_UNKNOWN;
	}

	return status;
}

void MCU_Prompt(void)
{
	char str[64] = "STM32 Ready";
	strcat(str, " ");
	strcat(str, version);

	UART_DMA_NotifyUser(&uart2_msg, str, strlen(str), true);
}

/*
 * Description: Returns original message + error message.
 * Input: DMA message data structure, the original string message, the error status
 * Output: none
 * Return: none
 */
void PrintError(UART_DMA_QueueStruct *msg, char *msg_copy, uint32_t error)
{
	char str[64] = {0};

	GetErrorString(error, str);

	strcat(msg_copy, "=");
	strcat(msg_copy, str);

	UART_DMA_NotifyUser(&uart2_msg, msg_copy, strlen(msg_copy), true);
}

/*
 * Description: Returns the original message + string arguments
 * Input: DMA message data structure, the original string message, the string to add to the original message
 * Output: none
 * Return: none
 */
void PrintReply(UART_DMA_QueueStruct *msg, char *msg_copy, char *msg2)
{
	strcat(msg_copy, "=");
	strcat(msg_copy, msg2);
	UART_DMA_NotifyUser(&uart2_msg, msg_copy, strlen(msg_copy), true);
}


void GPIO_LED_Green_Toggle(void)
{
	HAL_GPIO_TogglePin(LED_GREEN_GPIO_Port, LED_GREEN_Pin);
}



