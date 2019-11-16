#ifndef _STONE_H_
#define _STONE_H_

#include <d3dx9.h>

void Stone_Init(void);										//������
void Stone_Uninit(void);									//�I��
void Stone_Update(void);									//�X�V
void Stone_Draw(void);										//�`��
D3DXVECTOR3 Stone_GetPos(int index);						//���W�擾
int Stone_PlayerTurn(void);									//��P�����삵�Ă��邩
float Stone_Range(D3DXVECTOR3 pos, D3DXVECTOR3 old_pos);	//�X�g�[���̐i�񂾋���
bool Stone_Move(int index);									//�X�g�[���������Ă邩�ǂ���

#endif