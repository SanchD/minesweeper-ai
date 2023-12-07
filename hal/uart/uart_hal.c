/*****************************************************************
 * @file    uart_hal.c
 * @author  Sanchitha Dias
 * @date    01/12/2024
 * @brief   Contains all the interfaces of UART communication of
 *          the Minesweeper Game.
******************************************************************/
#include "uart_hal.h"

UartRetrunType_t UART1_Init(const uint32_t baudRate)
{
    (void) baudRate;

    return UART_SUCCESS;
}

UartRetrunType_t UART1_Data_Ready(void)
{
    return UART_DATA_READY;
}

UartRetrunType_t UART1_Write_Text(const char *writeString)
{
    (void) writeString;

    return UART_SUCCESS;
}

char UART1_Read(void)
{
    return 'S';
}

/******************************************************************
*********                       EOF                       *********
*******************************************************************
********* Dev. by Sanchitha Dias (sanchithadias@gmail.com)*********
******************************************************************/