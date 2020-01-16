#ifndef _QUEST_H_
#define _QUEST_H_

#include <d3dx9.h>

void Quest_Init(void);
void Quest_Uninit(void);
void Quest_Update(void);
void Quest_Draw(D3DXVECTOR3 pos);
void Quest_Create(void);

#endif 