#ifndef _STONE_H_
#define _STONE_H_

#include <d3dx9.h>

#define PLAYER_MAX_NUM (4)	//�v���C�l��

void Stone_Init(void);										//������
void Stone_Uninit(void);									//�I��
void Stone_Update(void);									//�X�V
void Stone_Draw(void);										//�`��
D3DXVECTOR3 Stone_GetPos(int index);						//���W�擾
int Stone_PlayerTurn(void);									//��P�����삵�Ă��邩
float Stone_Range(D3DXVECTOR3 pos, D3DXVECTOR3 old_pos);	//�X�g�[���̐i�񂾋���
float Stone_GetRange(int index);							//�X�g�[���̐i�񂾋����̎擾
bool Stone_Move(int index);									//�X�g�[���������Ă邩�ǂ���
int Stone_Turn(void);										//�S�[���̋����ɉ����ăX�g�[���̏��ԏ���
float Stone_Rot(int index);									//��]�ʎ擾
void Stone_SetTurn(void);									//���̃^�[���̐ݒ�
int Stone_GetScore(int index);								//�X�R�A�̎擾

#endif