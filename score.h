#ifndef _SCORE_H_
#define _SCORE_H_

void Score_Init(void);
void Score_Draw(int score, float x, float y, int digit, bool left = false);
void Score_Update(void);
void Score_Add(int score);

#endif