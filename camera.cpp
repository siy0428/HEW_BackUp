#include <d3dx9.h>
#include "mydirectx.h"
#include "common.h"
#include "input.h"
#include "debug_font.h"
#include "mouse.h"
#include "joycon.h"
#include "stone.h"
#include "goal.h"

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
static CAMERA g_camera;
static D3DXVECTOR3 g_look(0.0f, 0.0f, 0.0f);

//=====================================================
//初期化
//=====================================================
void Camera_Init(void)
{
	g_camera.pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_look = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
}

//=====================================================
//カメラの設定
//=====================================================
void Camera_Set(void)
{
	//デバイスのポインタ取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxRotation[XYZ_MAX];
	D3DXVECTOR3 At = Stone_GetPos(Stone_PlayerTurn());

	g_camera.pos = D3DXVECTOR3(0.0f, 5.0f, -10.0f);
	//注視点回転
	D3DXMatrixRotationY(&mtxRotation[Y], Joycon_Operator() * D3DX_PI / 180);
	D3DXVec3TransformNormal(&g_camera.pos, &g_camera.pos, &mtxRotation[Y]);
	g_camera.pos += At;

	//ビュー変換
	D3DXMATRIX mtxView;		//ビュー変換行列用変数
	D3DXVECTOR3 eye(g_camera.pos.x, g_camera.pos.y, g_camera.pos.z);	//カメラの座標
	D3DXVECTOR3 at(At.x, At.y, At.z);				//見る場所(注視点)
	D3DXVECTOR3 up(0.0f, 1.0f, 0.0f);				//カメラの上方向ベクトル
	D3DXMatrixLookAtLH(&mtxView, &eye, &at, &up);	//行列計算

	pDevice->SetTransform(D3DTS_VIEW, &mtxView);

	//プロジェクション変換行列
	D3DXMATRIX mtxProj;								//プロジェクション変換行列用変数
	D3DXMatrixPerspectiveFovLH(&mtxProj, D3DXToRadian(60), (float)SCREEN_WIDTH / SCREEN_HEIGHT, 0.1f, 100.0f);
	//プロジェクションのアドレス, 視野角の半分の角度(120℃ = 60), アスペクト比, 視野のnear near > 0, 視野のfar
	pDevice->SetTransform(D3DTS_PROJECTION, &mtxProj);
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