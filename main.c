#include "msp430g2553.h"
#include "uart.h"
#include "gfx.h"
#include "buttons.h"
#include <stdbool.h>
#include "2048.h"
//////////////////////
#include <string.h>
#include <stdio.h>
#include <stdint.h>
#include <time.h>
///////////////////

#define SIZE 4

//COLORS for Gfx
#define GRAY 0xAD		//GOOD
#define RED 0x07		//GOOD
#define ORANGE 0x1F		//GOOD
#define YELLOW 0x3F		//GOOD
#define GREEN 0x20		//GOOD
#define BLUE 0x88		//GOOD
#define PURPLE 0x84		//GOOD
#define PINK 0xCF		//GOOD
#define LIME 0x00
#define CYAN 0xF0		//GOOD
#define BROWN 0x14		//GOOD
#define WHITE 0xFF		//GOOD

unsigned char colours[16] = {GRAY, RED, ORANGE, YELLOW, GREEN, BLUE, PURPLE, PINK, LIME, CYAN, BROWN, CYAN, LIME, PINK, PURPLE, BLUE};
int dpad[5] = {0,0,0,0,0};
//unsigned char digits[64];	//unused due to MSP430 memory limitations
//int erase_count = 0;		//unused due to MSP430 memory limitations

int main(void)
{
    WDTCTL = WDTPW + WDTHOLD; 			//Stop WDT
    BCSCTL1 = CALBC1_8MHZ; 				//Set DCO to 8Mhz
    DCOCTL = CALDCO_8MHZ; 				//Set DCO to 8Mhz

    init_button(); 						// initialize the buttons for input
    init_wdt();							//Initializes the Watch Dog Timer
    uart_init();						//Initialize the UART connection
    __enable_interrupt();				//Interrupts Enabled (GIE)



    //Send Baud Rate to VGA Module
    unsigned const char baud_rate = 'U';
    uart_putc(baud_rate);
    uart_getc();


    //2048 Game Variables
    uint16_t board[SIZE][SIZE];
    bool success;
    int colour_index;

    //Sets up board by writing all 0s - then adding 2 randoms
    memset(board,0,sizeof(board));
    addRandom(board);
    addRandom(board);

    //Erase Screen then draw our Game Board
    Erase_Screen();
    init_Grid();
    boardToColour(board,colours);
    draw_Grid(colours);

    //Game will forever be in loop
    while(true)
    {
    	uplrToDpad(dpad);

    	//Input = UP
    	if(dpad[0])
    	{
    		success = moveLeft(board); //makes it go up
    		dpad[0] = 0;
    	}

    	//Input = LEFT
    	else if(dpad[1])
    	{
    		success = moveUp(board); //makes it go left
    		dpad[1] = 0;
    	}

    	//Input = RIGHT
    	else if(dpad[2])
    	{
    		success = moveDown(board); //makes it go right
    		dpad[2] = 0;
    	}

    	//Input = DOWN
    	else if(dpad[3])
    	{
    		success = moveRight(board);
    		dpad[3] = 0;
    	}

    	//Input = RESET
    	else if(dpad[4])
    	{
    	    Erase_Screen();
    	    init_Grid();

    	    memset(board,0,sizeof(board));
    	    addRandom(board);
    	    addRandom(board);

    	    boardToColour(board,colours);
    	    draw_Grid(colours);
    		dpad[4] = 0;
    	}

    	//ERROR
    	else
    	{
    		success = false;
    	}

    	if(success)
    	{

    	    //Add Random Tile
    		addRandom(board);

    		/*
    		//Clear Screen Every 10 Moves to free up Memory
    		//This is because redrawing the board just overlaps
    		//over the previous boxes. We don't erase the screen
    		//every time because it makes the screen flicker every time
    		if(erase_count == 10)
    		{
    			Erase_Screen();
    			init_Grid();
    			erase_count = 0;
    		}
    		else erase_count++;
    		*/

    		//Redraw Board
    	    boardToColour(board,colours);
    	    draw_Grid(colours);

    	    //Print Game Over to GFX
    		if(gameEnded(board))
    		{
    			gameOver();
    			return 0;
    		}

    		//Print You Win! to GFX if detects 2048 [WHITE] Tile
    		for(colour_index = 0; colour_index < sizeof(colours); colour_index++)
    		{
    			if(colours[colour_index] == WHITE) youWin();
    		}
    	}
    }
    return 0;
}


