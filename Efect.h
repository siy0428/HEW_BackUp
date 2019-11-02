#ifndef EFECT_H_
#define EFECT_H_

#include"collision.h"

#define EFECTMAX 10000

typedef enum
{
	EFFECT_PLAYER,
	EFFECT_BULLET
}TYPE;

void EfectInit(void);
void EfectUpdate(void);
void EfectDraw(void);
void EfectUninit(void);
void Efect_Create(float x, float y, D3DCOLOR color, int lifeFrame, TYPE type);//エフェクトの拡大率を入れるといい

#endif // !EFECT_H_
