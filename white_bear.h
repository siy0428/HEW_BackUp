#ifndef _WHITE_BEAR_H_
#define _WHITE_BEAR_H_

#include <d3dx9.h>

void WhiteBear_Init(void);
void WhiteBear_Uninit(void);
void WhiteBear_Update(void);
void WhiteBear_Draw(void);
void WhiteBear_Set(D3DXMATRIX mtxWorld);

#endif