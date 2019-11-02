#ifndef _CAMERA_H_
#define _CAMERA_H_

void Camera_Init(void);
void Camera_Set(D3DXVECTOR3 rotate, D3DXVECTOR3 pos, D3DXVECTOR3 at);		//�J�����ݒ�
D3DXVECTOR3 Camera_Input_Pos(D3DXVECTOR3 pos);								//�J�������W����
D3DXVECTOR3 Camera_Input_At(D3DXVECTOR3 at);								//�J���������_����
D3DXVECTOR3 Camera_Input_Rot(D3DXVECTOR3 rot);								//�J������]����
void Camera_Debug_Info(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 at);	//�J�������

#endif