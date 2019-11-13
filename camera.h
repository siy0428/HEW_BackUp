#ifndef _CAMERA_H_
#define _CAMERA_H_

void Camera_Init(void);
void Camera_Set(D3DXVECTOR3 rotate, D3DXVECTOR3 pos, D3DXVECTOR3 at);		//ƒJƒƒ‰İ’è
void Camera_Debug_Info(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 at);	//ƒJƒƒ‰î•ñ

#endif