#ifndef _COURSE_CAMERA_H_
#define _COURSE_CAMERA_H_

#include <d3dx9.h>

void Course_Camera_Init(void);
void Course_Camera_Uninit(void);
void Course_Camera_Update(void);
void Course_Camera_Draw(void);
D3DXMATRIX Course_Camera_Billboard(void);

#endif