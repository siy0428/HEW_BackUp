#ifndef _PENGUIN_H_
#define _PENGUIN_H_

#include <d3dx9.h>

void Penguin_Init(void);
void Penguin_Uninit(void);
void Penguin_Update(void);
void Penguin_Draw(void);
void Penguin_OnStoneNum(int index, int stone_index);
bool Penguin_OnStone(int index);
void Penguin_Set(D3DXMATRIX mtxWorld);
void Penguin_Set(D3DXMATRIX mtxWorld, int index);

#endif