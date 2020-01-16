#ifndef _NUMBER_H_
#define _NUMBER_H_

#define NUM_WIDTH  ()	//数字の幅
#define NUM_HEIGHT ()	//数字の高さ

void Number_Init(void);
void Number_Draw(int number, float x, float y);
void Number_Draw(int number, float x, float y, float size);
void Number_Draw(int x, int y);

#endif