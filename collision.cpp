#include "collision.h"

//=====================================================
//�~�̓����蔻��
//=====================================================
bool isCircle(CIRCLE *pA, CIRCLE *pB)
{
	return (pA->radius + pB->radius) * (pA->radius + pB->radius) > D3DXVec2LengthSq(&(pA->position - pB->position));
}
