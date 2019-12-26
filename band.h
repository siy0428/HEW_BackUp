#ifndef _BAND_H_
#define _BAND_H_

#include <d3dx9.h>

void Band_Init(void);
void Band_Uninit(void);
void Band_Update(void);
void Band_Draw(D3DXVECTOR3 pos);

#endif