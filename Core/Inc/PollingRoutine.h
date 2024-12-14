/*
 * PollingRoutine.h
 *
 *  Created on: Oct 24, 2023
 *      Author: karl.yamashita
 *
 *
 *      Template
 */

#ifndef INC_POLLINGROUTINE_H_
#define INC_POLLINGROUTINE_H_


/*

#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <errno.h>
#include <ctype.h>
#define Nop() asm(" NOP ")

#include "PollingRoutine.h"

*/
#ifndef __weak
#define __weak __attribute__((weak))
#endif

void PollingInit(void);
void PollingRoutine(void);

void UART_Parse(UART_DMA_Struct_t *msg);
int ParseGet(char *msg, char *retStr);
int ParseSet(char *msg);

void MCU_Prompt(void);

void PrintError(UART_DMA_Struct_t *msg, char *msg_copy, uint32_t error);
void PrintReply(UART_DMA_Struct_t *msg, char *msg_copy, char *msg2);

void GPIO_LED_Green_Toggle(void);



#endif /* INC_POLLINGROUTINE_H_ */
