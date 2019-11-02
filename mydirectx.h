#ifndef _MYDIRECTX_H_
#define _MYDIRECTX_H_

#include <windows.h>
#include <d3d9.h>

bool MyDirectX_Init(HWND hwnd);
void MyDirectX_Uninit(void);
LPDIRECT3DDEVICE9 GetDevice(void);

#endif