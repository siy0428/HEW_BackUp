#ifndef _SPRITE_H_
#define _SPRITE_H_

#include <d3dx9.h>
#include <d3d9.h>

void Sprite_Draw(int texId, float dx, float dy);
void Sprite_Draw(int texId, float dx, float dy, int alpha);
void Sprite_Draw(int texId, float dx, float dy, int cx, int cy, int cw, int ch);
void Sprite_Draw(int texId, float dx, float dy, int cx, int cy, int cw, int ch, int alpha);
void Sprite_Draw(int texId, float dx, float dy, int cx, int cy, int cw, int ch, float angle, float center_x, float center_y, float zoom);
void Sprite_Draw(int texId, float dx, float dy, int cx, int cy, int cw, int ch, float angle, float center_x, float center_y, float zoom, D3DXCOLOR color);
void Sprite_Reverse_Draw(int texId, float dx, float dy, int cx, int cy, int cw, int ch);
void Sprite_Upside_Draw(int texId, float dx, float dy, int cx, int cy, int cw, int ch);
void Sprite_SetColor(D3DXCOLOR color);

#endif