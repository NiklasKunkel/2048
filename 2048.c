#include "2048.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <time.h>

#define SIZE 4
#define _XOPEN_SOURCE 500
#define GRAY 0xAD		//GOOD
#define RED 0x07		//GOOD
#define ORANGE 0x1F
#define YELLOW 0x3F		//GOOD
#define GREEN 0x20		//GOOD
#define BLUE 0x88		//GOOD
#define PURPLE 0x84
#define PINK 0xCF
#define LIME 0x00
#define CYAN 0xF0		//GOOD
#define BROWN 0x14
#define WHITE 0xFF		//GOOD

unsigned int score=0;

unsigned int getScore()
{
	return score;
}

int8_t findTarget(uint16_t array[SIZE],int8_t x,int8_t stop)
{
	int8_t t;
	// if the position is already on the first, don't evaluate
	if (x==0) {
		return x;
	}
	for(t=x-1;t>=0;t--) {
		if (array[t]!=0) {
			if (array[t]!=array[x]) {
				// merge is not possible, take next position
				return t+1;
			}
			return t;
		} else {
			// we should not slide further, return this one
			if (t==stop) {
				return t;
			}
		}
	}
	// we did not find a
	return x;
}

bool slideArray(uint16_t array[SIZE])
{
	bool success = false;
	int8_t x,t,stop=0;

	for (x=0;x<SIZE;x++) {
		if (array[x]!=0) {
			t = findTarget(array,x,stop);
			// if target is not original position, then move or merge
			if (t!=x) {
				// if target is not zero, set stop to avoid double merge
				if (array[t]!=0) {
					score+=array[t]+array[x];
					stop = t+1;
				}
				array[t]+=array[x];
				array[x]=0;
				success = true;
			}
		}
	}
	return success;
}

void rotateBoard(uint16_t board[SIZE][SIZE])
{
	int8_t i,j,n=SIZE;
	uint16_t tmp;
	for (i=0; i<n/2; i++){
		for (j=i; j<n-i-1; j++){
			tmp = board[i][j];
			board[i][j] = board[j][n-i-1];
			board[j][n-i-1] = board[n-i-1][n-j-1];
			board[n-i-1][n-j-1] = board[n-j-1][i];
			board[n-j-1][i] = tmp;
		}
	}
}

bool moveUp(uint16_t board[SIZE][SIZE])
{
	bool success = false;
	int8_t x;
	for (x=0;x<SIZE;x++) {
		success |= slideArray(board[x]);
	}
	return success;
}

bool moveLeft(uint16_t board[SIZE][SIZE])
{
	bool success;
	rotateBoard(board);
	success = moveUp(board);
	rotateBoard(board);
	rotateBoard(board);
	rotateBoard(board);
	return success;
}

bool moveDown(uint16_t board[SIZE][SIZE])
{
	bool success;
	rotateBoard(board);
	rotateBoard(board);
	success = moveUp(board);
	rotateBoard(board);
	rotateBoard(board);
	return success;
}

bool moveRight(uint16_t board[SIZE][SIZE])
{
	bool success;
	rotateBoard(board);
	rotateBoard(board);
	rotateBoard(board);
	success = moveUp(board);
	rotateBoard(board);
	return success;
}

bool findPairDown(uint16_t board[SIZE][SIZE])
{
	bool success = false;
	int8_t x,y;
	for (x=0;x<SIZE;x++) {
		for (y=0;y<SIZE-1;y++) {
			if (board[x][y]==board[x][y+1]) return true;
		}
	}
	return success;
}

int16_t countEmpty(uint16_t board[SIZE][SIZE])
{
	int8_t x,y;
	int16_t count=0;
	for (x=0;x<SIZE;x++) {
		for (y=0;y<SIZE;y++) {
			if (board[x][y]==0) {
				count++;
			}
		}
	}
	return count;
}

bool gameEnded(uint16_t board[SIZE][SIZE])
{
	bool ended = true;
	if (countEmpty(board)>0) return false;
	if (findPairDown(board)) return false;
	rotateBoard(board);
	if (findPairDown(board)) ended = false;
	rotateBoard(board);
	rotateBoard(board);
	rotateBoard(board);
	return ended;
}

void addRandom(uint16_t board[SIZE][SIZE])
{
	static bool initialized = false;
	int8_t x,y;
	int16_t r,len=0;
	uint16_t n,list[SIZE*SIZE][2];

	if (!initialized) {
		srand(time(NULL));
		initialized = true;
	}

	for (x=0;x<SIZE;x++) {
		for (y=0;y<SIZE;y++) {
			if (board[x][y]==0) {
				list[len][0]=x;
				list[len][1]=y;
				len++;
			}
		}
	}

	if (len>0) {
		r = rand()%len;
		x = list[r][0];
		y = list[r][1];
		n = ((rand()%10)/9+1)*2;
		board[x][y]=n;
	}
}

//Translates Numbers of each Box to Correspond to Colors
void boardToColour(uint16_t board[SIZE][SIZE], unsigned char colours[SIZE*SIZE])
{
	//Index in colour array
	int index = 0;

	//ROWS in Board
	int i,j;
	for(i = 0; i < SIZE; i++)
	{
		//COLUMNS in Board
		for(j = 0; j < SIZE; j++)
		{
			switch(board[i][j])
			{
			case 0:
				colours[index++] = GRAY;
				break;

			case 2:
				colours[index++] = RED;
				break;

			case 4:
				colours[index++] = ORANGE;
				break;

			case 8:
				colours[index++] = YELLOW;
				break;

			case 16:
				colours[index++] = GREEN;
				break;

			case 32:
				colours[index++] = BLUE;
				break;

			case 64:
				colours[index++] = PURPLE;
				break;

			case 128:
				colours[index++] = PINK;
				break;

			case 256:
				colours[index++] = LIME;
				break;

			case 512:
				colours[index++] = CYAN;
				break;

			case 1024:
				colours[index++] = BROWN;
				break;

			case 2048:
				colours[index++] = WHITE;
				break;

			default:
				colours[index++] = GRAY;
			}
		}
	}
}

/* This was going to be used to break up all the numbers in board into single digits
 * The single digit format was needed in order to print them into separate 1 byte Hex Values
 * However we ran out of memory on the MSP430, thus we can't actually print these digits on the squares.
void boardToNum(uint16_t board[SIZE][SIZE], unsigned char num_out[64])
{
	unsigned int index = 0;
	int temp;
	int i,j,k;
	for(i = 0; i < SIZE; i++)
	{
		for(j = 0; j < SIZE; j++)
		{
			temp = board[i][j];
			for(k = 0; k < SIZE; k++)
			{
				if(temp == 0) num_out[index + 3 - k] = 0x20;
				else
				{
				//sprintf(hex,"%x",temp % 10);
				num_out[index + 3 - k] = (unsigned char)(temp % 10);
				temp /= 10;
				}
			}
			index += 4;
		}
	}
}
*/
