#ifndef _STONE_H_
#define _STONE_H_

#include <d3dx9.h>

void Stone_Init(void);				//������
void Stone_Uninit(void);			//�I��
void Stone_Update(void);			//�X�V
void Stone_Draw(void);				//�`��
D3DXVECTOR3 Stone_GetPos(void);		//���W�擾

#endif