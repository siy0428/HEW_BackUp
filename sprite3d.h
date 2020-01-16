#ifndef _SPRITE_3D_H_
#define _SPRITE_3D_H_

#include <d3dx9.h>
#include <d3d9.h>

void Sprite3d_Draw(int texId, D3DXMATRIX mtxWorld);
void Sprite3d_Draw(int texId, D3DXMATRIX mtxWorld, int cx, int cy, int cw, int ch);

#endif