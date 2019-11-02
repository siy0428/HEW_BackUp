#ifndef _COLLISION_H_
#define _COLLISION_H_

#include <d3d9.h>
#include <d3dx9.h>

typedef struct
{
	D3DXVECTOR2 position;	//ç¿ïW
	float radius;			//îºåa
}CIRCLE;

bool isCircle(CIRCLE *pA, CIRCLE *pB);

#endif