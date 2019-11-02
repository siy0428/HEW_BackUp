#ifndef _COLLISION_H_
#define _COLLISION_H_

#include <d3d9.h>
#include <d3dx9.h>

typedef struct
{
	D3DXVECTOR2 position;	//���W
	float radius;			//���a
}CIRCLE;

bool isCircle(CIRCLE *pA, CIRCLE *pB);

#endif