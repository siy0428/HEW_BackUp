#ifndef _DEBUG_CAMERA_H_
#define _DEBUG_CAMERA_H_

#include <d3dx9.h>

void dCamera_Init(void);
void dCamera_Set(void);			//�J�����ݒ�
void dCamera_Debug_Info(void);	//�J�������
D3DXMATRIX dCamera_Billbord(void);

#endif