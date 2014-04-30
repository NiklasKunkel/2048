#ifndef GFX_H_
#define GFX_H_

void Erase_Screen();
void init_Grid();
void draw_Grid(volatile unsigned char array[16]);
void gameOver();
void youWin();

#endif /* GFX_H_ */
