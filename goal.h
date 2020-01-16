#ifndef _GOAL_H_
#define _GOAL_H_

#include <d3dx9.h>

void Goal_Init(void);			//������
void Goal_Uninit(void);			//�I��
void Goal_Update(void);			//�X�V
void Goal_Draw(void);			//�`��
float Goal_Range(D3DXVECTOR3 pos);	//�X�g�[�����Ƃ̃S�[���܂ł̋���
bool Goal_Flag(float goal_range, float move);			//�S�[���������ǂ���
void Goal_GetWinPlayer(int num);
void Goal_Set(D3DXMATRIX mtxWorld);
D3DXVECTOR3 Goal_GetPos(void);

#endif