#ifndef _GOAL_H_
#define _GOAL_H_

#include <d3dx9.h>

void Goal_Init(void);			//������
void Goal_Uninit(void);			//�I��
void Goal_Update(void);			//�X�V
void Goal_Draw(void);			//�`��
float Goal_Range(D3DXVECTOR3 pos);	//�X�g�[�����Ƃ̃S�[���܂ł̋���

#endif