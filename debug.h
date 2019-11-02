#ifndef _DEBUG_H_
#define _DEBUG_H_

#include <d3dx9.h>

typedef struct
{
	D3DXVECTOR4 position;
	D3DCOLOR color;
	int CircleCount = 0;
}DEBUGCIRCLE;

#define FVF_DEBUG_CIRCLE_VERTEX (D3DFVF_XYZRHW | D3DFVF_DIFFUSE)

void Debug_Init(void);
void Debug_Uninit(void);
void Debug_Begin(void);
void Debug_End(void);
void Debug_DrawCircle(float x, float y, float radius);
void Debug_SetColor(D3DCOLOR color);
void Debug_CountUp(void);
int Debug_GetCount(void);
void Debug_index(int index);
int Debug_GetIndex(void);

#endif