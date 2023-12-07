/*****************************************************************
 * @file    uart_hal.h
 * @author  Sanchitha Dias
 * @date    01/12/2024
 * @brief   Contains all the interfaces of UART communication of
 *          the Minesweeper Game.
******************************************************************/
#ifndef UART_HAL_H
#define UART_HAL_H
/******************************************************************
*********                       Includes                  *********
******************************************************************/
#include "board.h"
#include "stdint.h"
/******************************************************************
*********                      Enumerations               *********
******************************************************************/
typedef enum
{
    UART_SUCCESS = 0,
    UART_DATA_READY = 0,

    UART_FAILED,
    UART_NOT_READY,

    UART_RETURN_TYPE_COUNT
}UartRetrunType_t;

/******************************************************************
*********                Function Definitions             *********
******************************************************************/
UartRetrunType_t UART1_Init(const uint32_t baudRate);

UartRetrunType_t UART1_Data_Ready(void);

UartRetrunType_t UART1_Write_Text(const char *writeString);

char UART1_Read(void);

#endif
/******************************************************************
*********                       EOF                       *********
*******************************************************************
********* Dev. by Sanchitha Dias (sanchithadias@gmail.com)*********
******************************************************************/