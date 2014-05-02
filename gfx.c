#include "gfx.h"
#include "uart.h"
#include "time.h"

//Erases the Screen
void Erase_Screen()
{
	uart_putc(0x45);
	uart_getc();
}

//Initalizes the Game Board
void init_Grid()
{
    //Set Background White
    uart_putc(0x42);
    uart_putc(0xB6);
    uart_getc();

    //PRINT 2048 TITLE
    uart_putc(0x53);	//print cmd
    uart_putc(0x00);	//x
    uart_putc(0x72);	//x
    uart_putc(0x00);	//y
    uart_putc(0x03);	//y
    uart_putc(0x01);	//font
    uart_putc(0xA2);	//color
    uart_putc(0x03);	//width
    uart_putc(0x04);	//height
    uart_putc(0x32);	//2
    uart_putc(0x30);	//0
    uart_putc(0x34);	//4
    uart_putc(0x38);	//8
    uart_putc(0x00);
	uart_getc();

	//Print BU
    uart_putc(0x53);	//print cmd
    uart_putc(0x00);	//x
    uart_putc(0xF2);	//x
    uart_putc(0x00);	//y
    uart_putc(0x03);	//y
    uart_putc(0x01);	//font
    uart_putc(0x06);	//color
    uart_putc(0x03);	//width
    uart_putc(0x04);	//height
    uart_putc(0x42);	//B
    uart_putc(0x55);	//U
    uart_putc(0x00);
	uart_getc();

	//Print EC450
    uart_putc(0x53);	//print cmd
    uart_putc(0x00);	//x
    uart_putc(0x22);	//x
    uart_putc(0x00);	//y
    uart_putc(0x06);	//y
    uart_putc(0x00);	//font
    uart_putc(0x00);	//color
    uart_putc(0x02);	//width
    uart_putc(0x03);	//height
    uart_putc(0x45);	//E
    uart_putc(0x43);	//C
    uart_putc(0x34);	//4
    uart_putc(0x35);	//5
    uart_putc(0x30);	//0
    uart_putc(0x00);
	uart_getc();

    //Draw Outside Grid Box TL(32,32) - BR(284,200)
    uart_putc(0x72);
    uart_putc(0x00); //x1
    uart_putc(0x20); //x1
    uart_putc(0x00); //y1
    uart_putc(0x20); //y1
    uart_putc(0x01); //x2
    uart_putc(0x1C); //x2
    uart_putc(0x00); //y2
    uart_putc(0xC8); //y2
    uart_putc(0xA4); //color
    uart_getc();
}

//Draws the the 4 by 4 matrix of boxes
//This is used each turn to redraw the current grid after a shift
//This "draw-over" overlap approach was used rather than clearing the
//screen each time because Erase Screen would cause annoying flickering
void draw_Grid(volatile unsigned char colours[16])
{
		//BOX1 TL(39,39) - BR(94,73)
	    uart_putc(0x72);
	    uart_putc(0x00); //x1
	    uart_putc(0x27); //x1
	    uart_putc(0x00); //y1
	    uart_putc(0x27); //y1
	    uart_putc(0x00); //x2
	    uart_putc(0x5E); //x2
	    uart_putc(0x00); //y2
	    uart_putc(0x49); //y2
	    uart_putc(colours[0]); //color
	    uart_getc();

	    //BOX2 TL(100,30) - BR(155,73)
	    uart_putc(0x72);
	    uart_putc(0x00); //x1
	    uart_putc(0x64); //x1
	    uart_putc(0x00); //y1
	    uart_putc(0x27); //y1
	    uart_putc(0x00); //x2
	    uart_putc(0x9B); //x2
	    uart_putc(0x00); //y2
	    uart_putc(0x49); //y2
	    uart_putc(colours[1]); //color
	    uart_getc();

	    //BOX3 TL(161,39) - BR(216,73)
	    uart_putc(0x72);
	    uart_putc(0x00); //x1
	    uart_putc(0xA1); //x1
	    uart_putc(0x00); //y1
	    uart_putc(0x27); //y1
	    uart_putc(0x00); //x2
	    uart_putc(0xD8); //x2
	    uart_putc(0x00); //y2
	    uart_putc(0x49); //y2
	    uart_putc(colours[2]); //color
	    uart_getc();

	    //BOX4 TL(222,39) - BR(277,73)
	    uart_putc(0x72);
	    uart_putc(0x00); //x1
	    uart_putc(0xDE); //x1
	    uart_putc(0x00); //y1
	    uart_putc(0x27); //y1
	    uart_putc(0x01); //x2
	    uart_putc(0x15); //x2
	    uart_putc(0x00); //y2
	    uart_putc(0x49); //y2
	    uart_putc(colours[3]); //color
	    uart_getc();

	    //BOX5 TL(39,79) - BR(94,113)
	    uart_putc(0x72);
	    uart_putc(0x00); //x1
	    uart_putc(0x27); //x1
	    uart_putc(0x00); //y1
	    uart_putc(0x4F); //y1
	    uart_putc(0x00); //x2
	    uart_putc(0x5E); //x2
	    uart_putc(0x00); //y2
	    uart_putc(0x71); //y2
	    uart_putc(colours[4]); //color
	    uart_getc();

	    //BOX6 TL(100,79) - BR(155,113)
	    uart_putc(0x72);
	    uart_putc(0x00); //x1
	    uart_putc(0x64); //x1
	    uart_putc(0x00); //y1
	    uart_putc(0x4F); //y1
	    uart_putc(0x00); //x2
	    uart_putc(0x9B); //x2
	    uart_putc(0x00); //y2
	    uart_putc(0x71); //y2
	    uart_putc(colours[5]); //color
	    uart_getc();

	    //BOX7 TL(161,79) - BR(216,113)
	    uart_putc(0x72);
	    uart_putc(0x00); //x1
	    uart_putc(0xA1); //x1
	    uart_putc(0x00); //y1
	    uart_putc(0x4F); //y1
	    uart_putc(0x00); //x2
	    uart_putc(0xD8); //x2
	    uart_putc(0x00); //y2
	    uart_putc(0x71); //y2
	    uart_putc(colours[6]); //color
	    uart_getc();

	    //BOX8 TL(222,79) - BR(277,113)
	    uart_putc(0x72);
	    uart_putc(0x00); //x1
	    uart_putc(0xDE); //x1
	    uart_putc(0x00); //y1
	    uart_putc(0x4F); //y1
	    uart_putc(0x01); //x2
	    uart_putc(0x15); //x2
	    uart_putc(0x00); //y2
	    uart_putc(0x71); //y2
	    uart_putc(colours[7]); //color
	    uart_getc();

	    //BOX9 TL(39,119) - BR(94,153)
	    uart_putc(0x72);
	    uart_putc(0x00); //x1
	    uart_putc(0x27); //x1
	    uart_putc(0x00); //y1
	    uart_putc(0x77); //y1
	    uart_putc(0x00); //x2
	    uart_putc(0x5E); //x2
	    uart_putc(0x00); //y2
	    uart_putc(0x99); //y2
	    uart_putc(colours[8]); //color
	    uart_getc();

	    //BOX10 TL(100,119) - BR(155,153)
	    uart_putc(0x72);
	    uart_putc(0x00); //x1
	    uart_putc(0x64); //x1
	    uart_putc(0x00); //y1
	    uart_putc(0x77); //y1
	    uart_putc(0x00); //x2
	    uart_putc(0x9B); //x2
	    uart_putc(0x00); //y2
	    uart_putc(0x99); //y2
	    uart_putc(colours[9]); //color
	    uart_getc();

	    //BOX11 TL(161,119) - BR(216,153)
	    uart_putc(0x72);
	    uart_putc(0x00); //x1
	    uart_putc(0xA1); //x1
	    uart_putc(0x00); //y1
	    uart_putc(0x77); //y1
	    uart_putc(0x00); //x2
	    uart_putc(0xD8); //x2
	    uart_putc(0x00); //y2
	    uart_putc(0x99); //y2
	    uart_putc(colours[10]); //color
	    uart_getc();

	    //BOX12 TL(222,119) - BR(277,153)
	    uart_putc(0x72);
	    uart_putc(0x00); //x1
	    uart_putc(0xDE); //x1
	    uart_putc(0x00); //y1
	    uart_putc(0x77); //y1
	    uart_putc(0x01); //x2
	    uart_putc(0x15); //x2
	    uart_putc(0x00); //y2
	    uart_putc(0x99); //y2
	    uart_putc(colours[11]); //color
	    uart_getc();

	    //BOX13 TL(39,159) - BR(94,193)
	    uart_putc(0x72);
	    uart_putc(0x00); //x1
	    uart_putc(0x27); //x1
	    uart_putc(0x00); //y1
	    uart_putc(0x9F); //y1
	    uart_putc(0x00); //x2
	    uart_putc(0x5E); //x2
	    uart_putc(0x00); //y2
	    uart_putc(0xC1); //y2
	    uart_putc(colours[12]); //color
	    uart_getc();

	    //BOX14 TL(100,159) - BR(155,193)
	    uart_putc(0x72);
	    uart_putc(0x00); //x1
	    uart_putc(0x64); //x1
	    uart_putc(0x00); //y1
	    uart_putc(0x9F); //y1
	    uart_putc(0x00); //x2
	    uart_putc(0x9B); //x2
	    uart_putc(0x00); //y2
	    uart_putc(0xC1); //y2
	    uart_putc(colours[13]); //color
	    uart_getc();

	    //BOX15 TL(161,159) - BR(216,193)
	    uart_putc(0x72);
	    uart_putc(0x00); //x1
	    uart_putc(0xA1); //x1
	    uart_putc(0x00); //y1
	    uart_putc(0x9F); //y1
	    uart_putc(0x00); //x2
	    uart_putc(0xD8); //x2
	    uart_putc(0x00); //y2
	    uart_putc(0xC1); //y2
	    uart_putc(colours[14]); //color
	    uart_getc();

	    //BOX16 TL(222,159) - BR(277,193)
	    uart_putc(0x72);
	    uart_putc(0x00); //x1
	    uart_putc(0xDE); //x1
	    uart_putc(0x00); //y1
	    uart_putc(0x9F); //y1
	    uart_putc(0x01); //x2
	    uart_putc(0x15); //x2
	    uart_putc(0x00); //y2
	    uart_putc(0xC1); //y2
	    uart_putc(colours[15]); //color
	    uart_getc();
}

//Prints Game Over in the Center of the Screen
void gameOver()
{
    //PRINT GAME OVER
    uart_putc(0x53);	//print cmd
    uart_putc(0x00);	//x
    uart_putc(0x32);	//x
    uart_putc(0x00);	//y
    uart_putc(0x60);	//y
    uart_putc(0x01);	//font
    uart_putc(0x06);	//color
    uart_putc(0x03);	//width
    uart_putc(0x04);	//height
    uart_putc(0x47);	//G
    uart_putc(0x41);	//A
    uart_putc(0x4D);	//M
    uart_putc(0x45);	//E
    uart_putc(0x20);	//
    uart_putc(0x4F);	//O
    uart_putc(0x56);	//V
    uart_putc(0x45);	//E
    uart_putc(0x52);	//R
    uart_putc(0x00);
	uart_getc();
}

//Prints You Win in the center of the screen
void youWin()
{
    //PRINT YOU WIN
    uart_putc(0x53);	//print cmd
    uart_putc(0x00);	//x
    uart_putc(0x40);	//x
    uart_putc(0x00);	//y
    uart_putc(0x60);	//y
    uart_putc(0x01);	//font
    uart_putc(0xFA);	//color
    uart_putc(0x03);	//width
    uart_putc(0x04);	//height
    uart_putc(0x59);	//Y
    uart_putc(0x4F);	//O
    uart_putc(0x55);	//U
    uart_putc(0x20);	//
    uart_putc(0x57);	//W
    uart_putc(0x49);	//I
    uart_putc(0x4E);	//N
    uart_putc(0x21);	//!
    uart_putc(0x00);
	uart_getc();
}
