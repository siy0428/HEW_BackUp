#ifndef _DEBUG_CAMERA_H_
#define _DEBUG_CAMERA_H_

#include <d3dx9.h>

void dCamera_Init(void);
void dCamera_Set(void);			//カメラ設定
void dCamera_Debug_Info(void);	//カメラ情報
D3DXMATRIX dCamera_Billbord(void);

#endif