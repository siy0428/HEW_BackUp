#ifndef _NUMBER_H_
#define _NUMBER_H_

#define NUM_WIDTH  ()	//�����̕�
#define NUM_HEIGHT ()	//�����̍���

void Number_Init(void);
void Number_Draw(int number, float x, float y);
void Number_Draw(int number, float x, float y, float cx, float cy, float cw, float ch);

#endif