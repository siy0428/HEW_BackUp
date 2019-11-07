#include <d3dx9.h>
#include "mydirectx.h"
#include "common.h"
#include "input.h"
#include "debug_font.h"
#include "mouse.h"
#include "joycon.h"
#include "stone.h"

//=====================================================
//列挙型
//=====================================================
typedef enum
{
	X,
	Y,
	Z,
	XYZ_MAX
}Rotate_XYZ;

//=====================================================
//構造体
//=====================================================
typedef struct
{
	D3DXVECTOR3 pos;
}CAMERA;

//=====================================================
//グローバル変数
//=====================================================
static CAMERA g_Camera;

//=====================================================
//初期化
//=====================================================
void Camera_Init(void)
{

}

//=====================================================
//カメラの設定
//=====================================================
void Camera_Set(D3DXVECTOR3 rotate, D3DXVECTOR3 pos, D3DXVECTOR3 At)
{
	//デバイスのポインタ取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//ビュー変換
	D3DXMATRIX mtxView, mtxRotation[XYZ_MAX];		//ビュー変換行列用変数
	D3DXVECTOR3 eye(pos.x, pos.y, pos.z);			//カメラの座標
	D3DXVECTOR3 at(At.x, At.y, At.z);				//見る場所(注視点)
	D3DXVECTOR3 up(0.0f, 1.0f, 0.0f);				//カメラの上方向ベクトル
	D3DXMatrixLookAtLH(&mtxView, &eye, &at, &up);	//行列計算
	D3DXMatrixRotationX(&mtxRotation[X], rotate.x * D3DX_PI / 180);				//X軸回転
	D3DXMatrixRotationY(&mtxRotation[Y], rotate.y * D3DX_PI / 180);				//Y軸回転
	//D3DXMatrixRotationY(&mtxRotation[Y], -Joycon_Operator() * D3DX_PI / 180);	//y軸回転
	D3DXMatrixRotationZ(&mtxRotation[Z], rotate.z * D3DX_PI / 180);				//Z軸回転

	mtxView = mtxRotation[X] * mtxRotation[Y] * mtxRotation[Z] * mtxView;

	pDevice->SetTransform(D3DTS_VIEW, &mtxView);

	//プロジェクション変換行列
	D3DXMATRIX mtxProj;								//プロジェクション変換行列用変数
	D3DXMatrixPerspectiveFovLH(&mtxProj, D3DXToRadian(60), (float)SCREEN_WIDTH / SCREEN_HEIGHT, 0.1f, 100.0f);
	//プロジェクションのアドレス, 視野角の半分の角度(120℃ = 60), アスペクト比, 視野のnear near > 0, 視野のfar
	pDevice->SetTransform(D3DTS_PROJECTION, &mtxProj);
}


//=====================================================
//カメラ座標操作
//=====================================================
D3DXVECTOR3 Camera_Input_Pos(D3DXVECTOR3 pos)
{
	//右移動(中心移動)
	if (Keyboard_IsPress(DIK_D))
	{
		pos.x += 0.25f;
	}
	//左移動(中心移動)
	else if (Keyboard_IsPress(DIK_A))
	{
		pos.x -= 0.25f;
	}

	return pos;
}

//=====================================================
//カメラ回転操作
//=====================================================
D3DXVECTOR3 Camera_Input_Rot(D3DXVECTOR3 rot)
{
	//右回転
	if (Keyboard_IsPress(DIK_E))
	{
		rot.y += 1.0f;
	}
	//左回転
	else if (Keyboard_IsPress(DIK_Q))
	{
		rot.y -= 1.0f;
	}

	return rot;
}

//=====================================================
//カメラ注視点操作
//=====================================================
D3DXVECTOR3 Camera_Input_At(D3DXVECTOR3 at)
{
	//右移動(中心移動)
	if (Keyboard_IsPress(DIK_D))
	{
		at.x += 0.25f;
	}
	//左移動(中心移動)
	else if (Keyboard_IsPress(DIK_A))
	{
		at.x -= 0.25f;
	}

	return at;
}

//=====================================================
//カメラ情報
//=====================================================
void Camera_Debug_Info(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 at)
{
	//座標
	DebugFont_Draw(0, 32 * 3, "eye = %.02lf  %.02lf  %.02lf", pos.x, pos.y, pos.z);
	//注視点
	DebugFont_Draw(0, 32 * 4, "at = %.02lf  %.02lf  %.02lf", at.x, at.y, at.z);
	//回転
	DebugFont_Draw(0, 32 * 5, "rot = %.02lf  %.02lf  %.02lf", rot.x, rot.y, rot.z);
}