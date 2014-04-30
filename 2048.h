#define SIZE 4

#include "stdlib.h"
#include <stdbool.h>
#include <stdint.h>
#include <time.h>

unsigned int getScore();

int8_t findTarget(uint16_t array[SIZE],int8_t x,int8_t stop);

bool slideArray(uint16_t array[SIZE]);

void rotateBoard(uint16_t board[SIZE][SIZE]);

bool moveUp(uint16_t board[SIZE][SIZE]);

bool moveLeft(uint16_t board[SIZE][SIZE]);

bool moveDown(uint16_t board[SIZE][SIZE]);

bool moveRight(uint16_t board[SIZE][SIZE]);

bool findPairDown(uint16_t board[SIZE][SIZE]);

int16_t countEmpty(uint16_t board[SIZE][SIZE]);

bool gameEnded(uint16_t board[SIZE][SIZE]);

void addRandom(uint16_t board[SIZE][SIZE]);

void boardToColour(uint16_t board[SIZE][SIZE], unsigned char colours[SIZE*SIZE]);

void boardToNum(uint16_t board[SIZE][SIZE], unsigned char num_out[64]);
