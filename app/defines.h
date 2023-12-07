/*****************************************************************
 * @file    defines.h
 * @author  Sanchitha Dias
 * @date    12/11/2024
 * @brief   Contains all the runtime configuration definitions of 
 *	        the Minesweeper Player Application.
******************************************************************/
#ifndef	DEFINES_H
#define DEFINES_H

/**
 * @brief Firware debugging through serial
 */
#define	APP_MODE_DEBUG      true

#define LED_BLINK_DELAY_ms  1000

#define BOARD_HEIGHT        20U
#define BOARD_WIDTH         20U
#define TOTAL_MINES         50U
#define NEW_DIVIDE          10U

#define CONCAT(a, b)        a ## b
#define CHAR_TO_INT(c)      (c - '0')

#endif
/******************************************************************
*********                       EOF                       *********
*******************************************************************
********* Dev. by Sanchitha Dias (sanchithadias@gmail.com)*********
******************************************************************/