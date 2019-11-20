#include <d3dx9.h>
#include "mydirectx.h"
#include "common.h"
#include "input.h"
#include "debug_font.h"

//=====================================================
//マクロ定義
//=====================================================
#define MOVE_SPEED (0.2f)	//移動量
#define ROT_SPEED (0.02f)	//回転量

//=====================================================
//グローバル変数
//=====================================================
static D3DXVECTOR3 g_vecFront(0.0f, 0.0f, 0.0f);
static D3DXVECTOR3 g_vecRight(0.0f, 0.0f, 0.0f);
static D3DXVECTOR3 g_vecUp(0.0f, 0.0f, 0.0f);
static D3DXVECTOR3 g_Pos(0.0f, 0.0f, 0.0f);
static float g_moveSpd = 0.0f;
static Float3 g_rotSpd = { 0.0f, 0.0f, 0.0f };
static float g_Fov = 0.0f;
static D3DXVECTOR3 g_Move(0.0f, 0.0f, 0.0f);
D3DXVECTOR3 vecDir(0.0f, 0.0f, 0.0f);
static D3DXVECTOR3 g_LookAt(0.0f, 0.0f, 0.0f);

//=====================================================
//初期化
//=====================================================
void dCamera_Init(void)
{
	g_vecFront = D3DXVECTOR3(0.0f, -1.0f, 1.0f);
	g_vecRight = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
	D3DXVec3Normalize(&g_vecFront, &g_vecFront);			//前ベクトルの正規化
	D3DXVec3Cross(&g_vecUp, &g_vecFront, &g_vecRight);		//直角のベクトルを得るための外積
	D3DXVec3Normalize(&g_vecUp, &g_vecUp);					//念の為正規化
	g_Pos = D3DXVECTOR3(0.0f, 10.0f, -10.0f);
	g_moveSpd = MOVE_SPEED;
	g_rotSpd = Float3{ 0.0f, 0.0f, 0.0f };
	g_Fov = 1.0f;
}

//=====================================================
//カメラ設定
//=====================================================
void dCamera_Set(void)
{

	//前方向
	if (Keyboard_IsPress(DIK_W))
	{
		vecDir += g_vecFront;
		g_moveSpd = MOVE_SPEED;
	}
	//後ろ方向
	if (Keyboard_IsPress(DIK_S))
	{
		vecDir += -g_vecFront;
		g_moveSpd = MOVE_SPEED;
	}
	//左方向
	if (Keyboard_IsPress(DIK_A))
	{
		vecDir += -g_vecRight;
		g_moveSpd = MOVE_SPEED;
	}
	//右方向
	if (Keyboard_IsPress(DIK_D))
	{
		vecDir += g_vecRight;
		g_moveSpd = MOVE_SPEED;
	}
	//上方向
	if (Keyboard_IsPress(DIK_LSHIFT) && Keyboard_IsPress(DIK_W))
	{
		vecDir += g_vecUp;
		g_moveSpd = MOVE_SPEED;
	}
	//下方向
	if (Keyboard_IsPress(DIK_LSHIFT) && Keyboard_IsPress(DIK_S))
	{
		vecDir += -g_vecUp;
		g_moveSpd = MOVE_SPEED;
	}

	//正規化
	D3DXVec3Normalize(&vecDir, &vecDir);
	g_Pos += vecDir * g_moveSpd;
	g_moveSpd *= 0.9f;

	D3DXMATRIX mtxRot;

	//注視点右回転
	if (Keyboard_IsPress(DIK_LSHIFT) && Keyboard_IsPress(DIK_RIGHTARROW))
	{
		g_rotSpd.y = ROT_SPEED * 2;
	}
	//注視点左回転
	else if (Keyboard_IsPress(DIK_LSHIFT) && Keyboard_IsPress(DIK_LEFTARROW))
	{
		g_rotSpd.y = -ROT_SPEED * 2;
	}
	//右回転
	else if (Keyboard_IsPress(DIK_RIGHTARROW))
	{
		g_rotSpd.x = ROT_SPEED;
	}
	//左回転
	else if (Keyboard_IsPress(DIK_LEFTARROW))
	{
		g_rotSpd.x = -ROT_SPEED;
	}
	//上回転
	else if (Keyboard_IsPress(DIK_DOWNARROW))
	{
		g_rotSpd.z = ROT_SPEED;
	}
	//下回転
	else if (Keyboard_IsPress(DIK_UPARROW))
	{
		g_rotSpd.z = -ROT_SPEED;
	}

	D3DXMatrixRotationAxis(&mtxRot, &g_vecRight, g_rotSpd.z);
	D3DXVec3TransformNormal(&g_vecFront, &g_vecFront, &mtxRot);
	D3DXVec3TransformNormal(&g_vecUp, &g_vecUp, &mtxRot);

	const int magic_num = 10;
	g_LookAt = g_Pos + g_vecFront * magic_num;
	g_Pos += g_LookAt;
	D3DXMatrixRotationY(&mtxRot, g_rotSpd.y);
	D3DXVec3TransformNormal(&g_vecFront, &g_vecFront, &mtxRot);
	D3DXVec3TransformNormal(&g_vecRight, &g_vecRight, &mtxRot);
	D3DXVec3TransformNormal(&g_vecUp, &g_vecUp, &mtxRot);
	g_Pos = g_LookAt - g_vecFront * magic_num;

	D3DXMatrixRotationY(&mtxRot, g_rotSpd.x);
	D3DXVec3TransformNormal(&g_vecFront, &g_vecFront, &mtxRot);
	D3DXVec3TransformNormal(&g_vecRight, &g_vecRight, &mtxRot);
	D3DXVec3TransformNormal(&g_vecUp, &g_vecUp, &mtxRot);

	g_rotSpd.x *= 0.85f;
	g_rotSpd.y *= 0.85f;
	g_rotSpd.z *= 0.85f;

	//ベクトルの正規化
	D3DXVec3Normalize(&g_vecFront, &g_vecFront);
	D3DXVec3Normalize(&g_vecRight, &g_vecRight);
	D3DXVec3Normalize(&g_vecUp, &g_vecUp);

	D3DXVECTOR3 eye = g_Pos;
	D3DXVECTOR3 at = g_Pos + g_vecFront;
	D3DXVECTOR3 up = g_vecUp;

	//ズーム
	if (Keyboard_IsPress(DIK_Z))
	{
		g_Fov += 0.01f;
	}
	if (Keyboard_IsPress(DIK_X))
	{
		g_Fov -= 0.01f;
	}

	//デバイスのポインタ取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//ビュー変換
	D3DXMATRIX mtxView;		//ビュー変換行列用変数
	D3DXMatrixLookAtLH(&mtxView, &eye, &at, &up);	//行列計算

	pDevice->SetTransform(D3DTS_VIEW, &mtxView);

	//プロジェクション変換行列
	D3DXMATRIX mtxProj;								//プロジェクション変換行列用変数
	D3DXMatrixPerspectiveFovLH(&mtxProj, g_Fov, (float)SCREEN_WIDTH / SCREEN_HEIGHT, 0.1f, 100.0f);
	//プロジェクションのアドレス, 視野角の半分の角度(120℃ = 60), アスペクト比, 視野のnear near > 0, 視野のfar
	pDevice->SetTransform(D3DTS_PROJECTION, &mtxProj);
}

//=====================================================
//カメラ情報
//=====================================================
void dCamera_Debug_Info(void)
{
	//座標
	DebugFont_Draw(0, 32 * 0, "eye = %.02lf  %.02lf  %.02lf", g_Pos.x, g_Pos.y, g_Pos.z);
	//注視点
	DebugFont_Draw(0, 32 * 1, "at = %.02lf  %.02lf  %.02lf", g_Pos.x + g_vecFront.x, g_Pos.y + g_vecFront.y, g_Pos.z + g_vecFront.z);
	//ズーム
	DebugFont_Draw(0, 32 * 2, "zoom = %.02lf", g_Fov);
}