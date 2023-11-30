/*****************************************************************
 * @file    gpio_hal.h
 * @author  Sanchitha Dias
 * @date    12/11/2024
 * @brief   Contains all the interfaces of GPIO communication of
 *          the Magnetometer Test Application.
******************************************************************/
#ifndef GPIO_HAL_H
#define GPIO_HAL_H

#include "board.h"
#include "stdint.h"

void pinMode(char port, char pin, uint8_t mode);

#endif
/******************************************************************
*********                       EOF                       *********
*******************************************************************
********* Dev. by Sanchitha Dias (sanchithadias@gmail.com)*********
******************************************************************/