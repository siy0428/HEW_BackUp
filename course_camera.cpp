#include "mydirectx.h"
#include <d3dx9.h>
#include "common.h"
#include "fade.h"
#include "stone.h"
#include "goal.h"
#include "debug_font.h"

//=====================================================
//グローバル変数
//=====================================================
static D3DXVECTOR3 g_pos;
static D3DXMATRIX g_mtxView;		//ビュー変換行列用変数

//=====================================================
//初期化
//=====================================================
void Course_Camera_Init(void)
{
	g_pos = Stone_GetPos(0);
	g_pos.y += 30.0f;
}

//=====================================================
//終了
//=====================================================
void Course_Camera_Uninit(void)
{

}

//=====================================================
//更新
//=====================================================
void Course_Camera_Update(void)
{
	//デバイスのポインタ取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//ビュー変換
	D3DXVECTOR3 eye(g_pos.x, g_pos.y, g_pos.z);		//カメラの座標
	D3DXVECTOR3 at(Goal_GetPos().x, Goal_GetPos().y, Goal_GetPos().z);
	D3DXVECTOR3 up(0.0f, 1.0f, 0.0f);				//カメラの上方向ベクトル
	D3DXMatrixLookAtLH(&g_mtxView, &eye, &at, &up);	//行列計算

	D3DXVECTOR3 start = Stone_GetPos(0);	//スタート座標
	D3DXVECTOR3 goal = Goal_GetPos();		//ゴール座標
	float angle = atan2((goal.x - start.x), (goal.z - start.z));

	//座標変換
	g_pos.x += cos(angle) * 0.25f;
	g_pos.z += sin(angle) * 0.25f;
	//一定の距離すすんだら次のシーン
	if (g_pos.z <= Goal_GetPos().z + 30.0f)
	{
		Fade_InOut(SCENE_MAIN);
	}

	pDevice->SetTransform(D3DTS_VIEW, &g_mtxView);

	//プロジェクション変換行列
	D3DXMATRIX mtxProj;								//プロジェクション変換行列用変数
	D3DXMatrixPerspectiveFovLH(&mtxProj, D3DXToRadian(60), (float)SCREEN_WIDTH / SCREEN_HEIGHT, 0.1f, 100.0f);
	//プロジェクションのアドレス, 視野角の半分の角度(120℃ = 60), アスペクト比, 視野のnear near > 0, 視野のfar
	pDevice->SetTransform(D3DTS_PROJECTION, &mtxProj);
}

//=====================================================
//描画
//=====================================================
void Course_Camera_Draw(void)
{

}

//=====================================================
//ビルボード取得
//=====================================================
D3DXMATRIX Course_Camera_Billboard(void)
{
	D3DXMATRIX mtxInv;

	D3DXMatrixIdentity(&mtxInv);
	D3DXMatrixTranspose(&mtxInv, &g_mtxView);	//転置行列

	mtxInv._14 = 0.0f;
	mtxInv._24 = 0.0f;
	mtxInv._34 = 0.0f;

	return mtxInv;
}

//#include "mydirectx.h"
//#include <d3dx9.h>
//#include "common.h"
//#include "fade.h"
//
////=====================================================
////グローバル変数
////=====================================================
//static D3DXVECTOR3 g_pos;
//
////=====================================================
////初期化
////=====================================================
//void Course_Camera_Init(void)
//{
//	g_pos = D3DXVECTOR3(0.0f, 10.0f, -10.0f);
//}
//
////=====================================================
////終了
////=====================================================
//void Course_Camera_Uninit(void)
//{
//
//}
//
////=====================================================
////更新
////=====================================================
//void Course_Camera_Update(void)
//{
//	//デバイスのポインタ取得
//	LPDIRECT3DDEVICE9 pDevice = GetDevice();
//	D3DXMATRIX mtxRotation;
//
//	//ビュー変換
//	D3DXMATRIX mtxView;		//ビュー変換行列用変数
//	D3DXVECTOR3 eye(g_pos.x, g_pos.y, g_pos.z);		//カメラの座標
//	D3DXVECTOR3 at(g_pos.x, g_pos.y - 3.0f, g_pos.z + 5.0f);//見る場所(注視点)
//	D3DXVECTOR3 up(0.0f, 1.0f, 0.0f);				//カメラの上方向ベクトル
//	D3DXMatrixLookAtLH(&mtxView, &eye, &at, &up);	//行列計算
//
//	//座標変換
//	g_pos.z += 0.1f;
//	//一定の距離すすんだら次のシーン

//	if (g_pos.z >= 30.0f)
//	{
//		Fade_InOut(SCENE_MAIN);
//	}
//
//	pDevice->SetTransform(D3DTS_VIEW, &mtxView);
//
//	//プロジェクション変換行列
//	D3DXMATRIX mtxProj;								//プロジェクション変換行列用変数
//	D3DXMatrixPerspectiveFovLH(&mtxProj, D3DXToRadian(60), (float)SCREEN_WIDTH / SCREEN_HEIGHT, 0.1f, 100.0f);
//	//プロジェクションのアドレス, 視野角の半分の角度(120℃ = 60), アスペクト比, 視野のnear near > 0, 視野のfar
//	pDevice->SetTransform(D3DTS_PROJECTION, &mtxProj);
//}
//
////=====================================================
////描画
////=====================================================
//void Course_Camera_Draw(void)
//{
//
//}