/*****************************************************************/
/*                          Team Infinity                        */
/*****************************************************************/
/*****************************************************************/
/*                       Tutor: Samiru Gayan                     */
/*****************************************************************/
/*****************************************************************/
/*                      Date - 14 December 2014                  */
/*                      Minesweeper Game Player!                 */
/*****************************************************************/
/*****************************************************************
 * @file    main.c
 * @author  Team Infinity
 * @date    14/12/2014
 * @brief   Main application of the Minesweeper Player
 ******************************************************************/
/******************************************************************
*********                       Includes                  *********
******************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "board.h"
#include "defines.h"

#include "gpio_hal.h"
#include "uart_hal.h"
/******************************************************************
*********                  Local Defines                  *********
******************************************************************/

/******************************************************************
*********                Local Enumerations               *********
******************************************************************/

/******************************************************************
*********                  Local Objects                  *********
******************************************************************/

/******************************************************************
*********                  Local Variables                *********
******************************************************************/
uint8_t counT;                     // just a counter variable which can counT upto 255. Used to update Click array
char onBoard[BOARD_HEIGHT][BOARD_WIDTH]; // 20x20 array is used to store the data recieved from the MS game
char ImDone[BOARD_HEIGHT][BOARD_WIDTH];  // 20x20 array is used to stop processing each cell after it has already processed(Clicked or Flaged)
char Click[] = "0000C";                  // Storing string of characters where to be clicked. This array can be directly trasnferred
char Flag[] = "0000M";                   // Storing the loctiona to be flagged as a string of charcters(1st 2 chars = Y location/2nd 2 chars = X location)
char HrdCD[] = "0000M";                  // Storing the intial clicking location in order to compare in function hardCode
/******************************************************************
*********          Local Function Definitions             *********
******************************************************************/
void attmeptClickFree();                                       // use to check & Click a freesquare
void attemptFlagMine();                                        // use to check & Flag a mine
void clickFree(const uint8_t x, const uint8_t y);              // Transmitting data of the location to be clicked
char countFlagsAround(const uint8_t x, const uint8_t y);       // Counts the no. of Flags/Mines around an each no.
char countFreeSquaresAround(const uint8_t x, const uint8_t y); // Counts the no. of freesquares around an each no.
void flagMine(const uint8_t x, const uint8_t y);               // Transmitting data of the location to be flaged
void hardCode();                                               // Clicking an un deducable square
void initImDone();                                             // Initializing(clearing) the array ImDone
void updateonBoard();                                          // updates the onBoard array by reciving data from the game
/******************************************************************
*********              Application Firmware               *********
******************************************************************/
int main()
{
    counT = 0;
    initImDone();      // calling the funtion to Initialize(clear) the array ImDone
    UART1_Init(19200); // Setup the baudrate to 19200(bits per second)
    while (counT != 4)
    { // waits till the playgame button click & recive the intial clicking location
        if (UART1_Data_Ready())
        {                                // if all the bits of the character location is recieved
            Click[counT] = UART1_Read(); // store it at the counT location of the Click string array
            counT++;                     // Increment counT to access the next loaction of the Click array
        }
    }
    strncpy(HrdCD, Click, sizeof(HrdCD) - 1);    // Copies the data in string Click to HrdCD
    UART1_Write_Text(Click); // Transmit the string of the location to be clicked initially
    updateonBoard();         // Recieve the revealed squares after clicking & save it in the array onBoard
    while (1)
    {                       // The whole program runs in this loop
        attemptFlagMine();  // calling the function which process & locate mines
        attmeptClickFree(); // calling the function which process & locate free squares
    }

    return 0;
}

/* This function searches for a square with a no. from 1 to 8 & wether it is already processed. If it is still haven't processed, then
   it checks wether the no. in tha square minus no. of flags/mines around that square(i,j),is equal to the no. of freesquares around. If that
   it is true then program flags all the free squares */
void attemptFlagMine()
{
    uint8_t i, j, ii, jj; // defining counter variables
    for (i = 0; i < BOARD_WIDTH; i++)
    { // counts upto 19(BOARD_WIDTH-1)
        for (j = 0; j < BOARD_HEIGHT; j++)
        { // counts upto 19(BOARD_HEIGHT-1)
            if ((CHAR_TO_INT(onBoard[j][i]) < 9) && ImDone[j][i] == 0x00)
            { // converts character int in onBoard to int & chks weather it is
              // between 1-8 & also that sqaure(i,j) is not processed yet
                if (CHAR_TO_INT(onBoard[j][i]) - countFlagsAround(i, j) == countFreeSquaresAround(i, j))
                { // converts character int in onBoard to
                    // int & subtract it by the no. of mines/Flags around that
                    // square(i,j) is eqaul to no. of free sqaures around it(i,j)
                    ImDone[j][i] = 0x01; // set this location(i,j) to 0x01. which stands for processed
                    for (ii = 0; ii < BOARD_WIDTH; ii++)
                    {
                        for (jj = 0; jj < BOARD_HEIGHT; jj++)
                        {
                            if ((abs(ii - i) <= 1) && (abs(jj - j) <= 1))
                            { // go throught each sqaure wich has a distance of 1 or 0 from(i,j)
                                if (onBoard[jj][ii] == 'X')
                                {                       // check for an unopened square('X') around the location (i,j)
                                    flagMine(ii, jj); // Flag on unopened square
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}

void attmeptClickFree()
{
    uint8_t i, j, ii, jj; // defining counter variables
    char Success = 0x00;        // status of the game whether can solve or not
    for (i = 0; i < BOARD_WIDTH; i++)
    { // counts upto 19(BOARD_WIDTH-1)
        for (j = 0; j < BOARD_HEIGHT; j++)
        { // counts upto 19(BOARD_HEIGHT-1)
            if ((CHAR_TO_INT(onBoard[j][i]) < 9) && ImDone[j][i] == 0x00)
            { // converts character int in onBoard to int & chks weather it
              // is between 1-8 & also that sqaure(i,j) is not processed yet
                if (CHAR_TO_INT(onBoard[j][i]) == countFlagsAround(i, j))
                {                        // converts character int in onBoard to int & checks wether that
                                         // no. at (i,j) is equal to the no. of flags/mine around it
                    ImDone[j][i] = 0x01; // set this location(i,j) to 0x01. which stands for processed
                    Success = 0x01;
                    for (ii = 0; ii < BOARD_WIDTH; ii++)
                    {
                        for (jj = 0; jj < BOARD_HEIGHT; jj++)
                        {
                            if ((abs(ii - i) <= 1) && (abs(jj - j) <= 1))
                            { // go throught each sqaure wich has a distance of 1 or 0 from(i,j)
                                if (onBoard[jj][ii] == 'X')
                                {                        // check for an unopened square('X') around the location (i,j)
                                    clickFree(ii, jj); // click on excess unopened square
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    if (!Success)   // IF cannot process
    {
        hardCode(); // Click the hard coded location
    }
}

/* Initialize all the cells of the ImDone array to 0x00*/
void initImDone()
{
    memset(ImDone, 0, sizeof(ImDone));
}

/* Flags the location pointed by the variables x & y, where x is the x location & y is the y location*/
void flagMine(const uint8_t x, const uint8_t y)
{
    sprintf(Flag, "%2d%2dM", y % BOARD_HEIGHT, x % BOARD_WIDTH);
    UART1_Write_Text(Flag); // transmit the location to be flaged pointed by Flag string
    updateonBoard();        // recieve & store the data of the game in onBoard array
}

/* Clicks the location pointed by the variables x & y, where x is the x location & y is the y location*/
void clickFree(const uint8_t x, const uint8_t y)
{
    sprintf(Click, "%2d%2dC", y % BOARD_HEIGHT, x % BOARD_WIDTH);
    UART1_Write_Text(Click); // transmit the location to be Clicked pointed by Click string
    updateonBoard();         // recieve & store the data of the game in onBoard array
}

/* This function clicks the locations where the program can't deduce whether its a mine*/
void hardCode()
{
    if ((strncmp(HrdCD, "0405M", sizeof(HrdCD) - 1)) <= 0)
    {                              // cheks the first clicked location in order to deduce the board
        UART1_Write_Text("1702C"); // send the custom location to Click where there's no mine
        updateonBoard();           // recieve & store the data of the game in onBoard array
    }
    else if ((strncmp(HrdCD, "1113M", sizeof(HrdCD) - 1) - 1) <= 0)
    {                              // cheks the first clicked location in order to deduce the board
        UART1_Write_Text("1418C"); // send the custom location to Click where there's no mine
        updateonBoard();           // recieve & store the data of the game in onBoard array
    }
}

/* This function counts the no. of flags/mines around the numbered cell pointed by the location x & y
 ****************************************************
 *                 *               *                *
 *  (x-1, y-1)    *  (x, y-1)    *  (x+1, y-1)   *
 *                 *               *                *
 ****************************************************
 *                 *               *                *
 *  (x-1, y)      *    (x, y)    *  (x+1, y)     *
 *                 *               *                *
 ****************************************************
 *                 *               *                *
 *   (x-1, y+1)   *  (x, y+1)    *  (x+1, y+1)   *
 *                 *               *                *
 ****************************************************
 */
char countFlagsAround(const uint8_t x, const uint8_t y)
{
    uint8_t mines = 0, // set the mine counter to 0
            Up = 0,    // used to store the detected upper side/border
            Dwn = 0,   // used to store the detected bottom side/border
            Rit = 0,   // used to store the detected right side/border
            Lft = 0;   // used to store the detected left side/border
    if (x == 0)
    {
        Lft = 1; // if the square currently in is on the left side border set variable Lft
    }
    if (y == 0)
    {
        Up = 1; // if the square currently in is on the upper side border set variable Up
    }
    if (x == (BOARD_WIDTH - 1))
    {
        Rit = 1; // if the square currently in is on the right side border set variable Rit
    }
    if (y == (BOARD_HEIGHT - 1))
    {
        Dwn = 1; // if the square currently in is on the bottom side border set variable Dwn
    }
    if (!Up && !Lft && (onBoard[y - 1][x - 1] == 'M'))
    {
        mines++; // check left upper square is a Flag/mine & cell (x, y) should not be on the left upper edge. if true increment mines
    }
    if (!Up && (onBoard[y - 1][x] == 'M'))
    {
        mines++; // check upper square is a Flag/mine & cell (x, y) should not be on the upper side. if true increment mines
    }
    if (!Up && !Rit && (onBoard[y - 1][x + 1] == 'M'))
    {
        mines++; // check right upper square is a Flag/mine & cell (x, y) should not be on the right upper edge. if true increment mines
    }
    if (!Lft && (onBoard[y][x - 1] == 'M'))
    {
        mines++; // check left square is a Flag/mine & cell (x, y) should not be on the left side. if true increment mines
    }
    if (!Rit && (onBoard[y][x + 1] == 'M'))
    {
        mines++; // check right square is a Flag/mine & cell (x, y) should not be on the right side. if true increment mines
    }
    if (!Dwn && !Lft && (onBoard[y + 1][x - 1] == 'M'))
    {
        mines++; // check left bottom square is a Flag/mine & cell (x,x) should not be on the left bottom edge. if true increment mines
    }
    if (!Dwn && (onBoard[y + 1][x] == 'M'))
    {
        mines++; // check bottom square is a Flag/mine & cell (x, y) should not be on the bottom side. if true increment mines
    }
    if (!Dwn && !Rit && (onBoard[y + 1][x + 1] == 'M'))
    {
        mines++; // check right bottom square is a Flag/mine & cell (x, y) should not be on the right bottom edge. if true increment mines
    }
    return mines; // return no. of mines/flags counted
}

/* This function counts the no. of free squares around the numbered cell pointed by the location x & x
 ****************************************************
 *                 *               *                *
 *  (x-1, y-1)    *  (x, y-1)    *  (x+1, y-1)   *
 *                 *               *                *
 ****************************************************
 *                 *               *                *
 *  (x-1, y)      *    (x, y)    *  (x+1, y)     *
 *                 *               *                *
 ****************************************************
 *                 *               *                *
 *   (x-1, y+1)   *  (x, y+1)    *  (x+1, y+1)   *
 *                 *               *                *
 ****************************************************
 */
char countFreeSquaresAround(const uint8_t x, const uint8_t y)
{
    uint8_t freeSquares = 0, // set the free squares counter to 0
            Up = 0,          // used to store the detected upper side/border
            Dwn = 0,         // used to store the detected bottom side/border
            Rit = 0,         // used to store the detected right side/border
            Lft = 0;         // used to store the detected left side/border
    if (x == 0)
    {
        Lft = 1; // if the square currently in is on the left side border set variable Lft
    }
    if (y == 0)
        Up = 1; // if the square currently in is on the upper side border set variable Up

    if (x == (BOARD_WIDTH - 1))
    {
        Rit = 1; // if the square currently in is on the right side border set variable Rit
    }
    if (y == (BOARD_HEIGHT - 1))
    {
        Dwn = 1; // if the square currently in is on the bottom side border set variable Dwn
    }
    if (!Up && !Lft && (onBoard[y - 1][x - 1] == 'X'))
    {
        freeSquares++; // check left upper square is a Flag/mine & cell (x, y) should not be on the left upper edge. if true increment freesquares
    }
    if (!Up && (onBoard[y - 1][x] == 'X'))
    {
        freeSquares++; // check upper square is a Flag/mine & cell (x, y) should not be on the upper side. if true increment freesquares
    }
    if (!Up && !Rit && (onBoard[y - 1][x + 1] == 'X'))
    {
        freeSquares++; // check right upper square is a Flag/mine & cell (x, y) should not be on the right upper edge. if true increment freesquares
    }
    if (!Lft && (onBoard[y][x - 1] == 'X'))
    {
        freeSquares++; // check left square is a Flag/mine & cell (x, y) should not be on the left side. if true increment freesquares
    }
    if (!Rit && (onBoard[y][x + 1] == 'X'))
    {
        freeSquares++; // check right square is a Flag/mine & cell (x, y) should not be on the right side. if true increment freesquares
    }
    if (!Dwn && !Lft && (onBoard[y + 1][x - 1] == 'X'))
    {
        freeSquares++; // check left bottom square is a Flag/mine & cell (x, y) should not be on the left bottom edge. if true increment freesquares
    }
    if (!Dwn && (onBoard[y + 1][x] == 'X'))
    {
        freeSquares++; // check bottom square is a Flag/mine & cell (x, y) should not be on the bottom side. if true increment freesquares
    }
    if (!Dwn && !Rit && (onBoard[y + 1][x + 1] == 'X'))
    {
        freeSquares++; // check right bottom square is a Flag/mine & cell (x, y) should not be on the right bottom edge. if true increment freesquares
    }
    return freeSquares; // return freesquares counted
}

/* This function is called each an every time we make a Click or Flag. Beacause the program starts to send data. To store the data sent onBoard is covered by this*/
void updateonBoard()
{
    uint32_t x, y; // declaration of counter variables
    for (x = 0; x < BOARD_WIDTH; x++)
    { // counts upto 19(BOARD_WIDTH-1)
        for (y = 0; y < BOARD_HEIGHT; y++)
        { // counts upto 19(BOARD_HEIGHT-1)
            while (1)
            { // This while loop holds the program till the game finishes sending 8-bit data of each cell
                if (UART1_Data_Ready())
                {                                 // Check whether the full char of 8-bit is recieved
                    onBoard[x][y] = UART1_Read(); // save the data received in the location (x, y)
                    break;
                }
            }
        }
    }
}
/******************************************************************
*********                       EOF                       *********
*******************************************************************
********* Dev. by Sanchitha Dias (sanchithadias@gmail.com)*********
******************************************************************/
