#ifndef _MOUSE_H_
#define _MOUSE_H_

void Mouse_Init(void);		//������
void Mouse_Uninit(void);	//�I��
void Mouse_Update(void);	//�X�V
void Mouse_Draw(void);		//�`��
int Mouse_GetPos_X(void);	//�}�E�X��x���������W�擾
int Mouse_GetPos_Y(void);	//�}�E�X��y���������W�擾
int Mouse_GetForce(void);	//�}�E�X�̃X�N���[����0�`100�̒l��Ԃ�

#endif