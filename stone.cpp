#include <d3dx9.h>
#include "mydirectx.h"
#include "common.h"
#include "debug_font.h"
#include "input.h"
#include "joycon.h"
#include "mouse.h"

//====================================================
//マクロ定義
//====================================================
#define PLAYER_MAX_NUM (4)	//プレイ人数

//====================================================
//構造体宣言
//====================================================
typedef struct Stone_Vertex_tag
{
	D3DXVECTOR3 position;	//座標
	D3DCOLOR color;			//色情報
}StoneVertex;
#define FVF_CUBE (D3DFVF_XYZ | D3DFVF_DIFFUSE)

//ストーン構造体
typedef struct
{
	D3DXMATRIX mtxWorld;		//ワールド行列
	D3DXMATRIX mtxRotation;		//回転行列
	D3DXMATRIX mtxTrans;		//平行移動
	D3DXMATRIX mtxMove;			//移動量
	D3DXVECTOR3 pos = { 0.0f, 0.5f, 0.0f };		//位置座標
	float move;
	bool isTurn;
}Stone;

//=====================================================
//グローバル変数
//=====================================================
static const StoneVertex g_stone_vertex[] = {						//頂点構造体
	//正面
	{D3DXVECTOR3(-0.5f, 0.5f, -0.5f), D3DCOLOR_RGBA(255, 255, 255, 255)},
	{D3DXVECTOR3(0.5f, 0.5f, -0.5f), D3DCOLOR_RGBA(255, 255, 255, 255)},
	{D3DXVECTOR3(0.5f, -0.5f, -0.5f), D3DCOLOR_RGBA(255, 255, 255, 255)},
	{D3DXVECTOR3(0.5f, -0.5f, -0.5f), D3DCOLOR_RGBA(255, 255, 255, 255)},
	{D3DXVECTOR3(-0.5f, -0.5f, -0.5f), D3DCOLOR_RGBA(255, 255, 255, 255)},
	{D3DXVECTOR3(-0.5f, 0.5f, -0.5f), D3DCOLOR_RGBA(255, 255, 255, 255)},
	//右面
	{D3DXVECTOR3(0.5f, 0.5f, -0.5f), D3DCOLOR_RGBA(0, 255, 0, 255)},
	{D3DXVECTOR3(0.5f, 0.5f, 0.5f), D3DCOLOR_RGBA(0, 255, 0, 255)},
	{D3DXVECTOR3(0.5f, -0.5f, 0.5f), D3DCOLOR_RGBA(0, 255, 0, 255)},
	{D3DXVECTOR3(0.5f, -0.5f, 0.5f), D3DCOLOR_RGBA(0, 255, 0, 255)},
	{D3DXVECTOR3(0.5f, -0.5f, -0.5f), D3DCOLOR_RGBA(0, 255, 0, 255)},
	{D3DXVECTOR3(0.5f, 0.5f, -0.5f), D3DCOLOR_RGBA(0, 255, 0, 255)},
	//左面
	{D3DXVECTOR3(-0.5f, 0.5f, 0.5f), D3DCOLOR_RGBA(255, 0, 0, 255)},
	{D3DXVECTOR3(-0.5f, 0.5f, -0.5f), D3DCOLOR_RGBA(255, 0, 0, 255)},
	{D3DXVECTOR3(-0.5f, -0.5f, 0.5f), D3DCOLOR_RGBA(255, 0, 0, 255)},
	{D3DXVECTOR3(-0.5f, -0.5f, 0.5f), D3DCOLOR_RGBA(255, 0, 0, 255)},
	{D3DXVECTOR3(-0.5f, 0.5f, -0.5f), D3DCOLOR_RGBA(255, 0, 0, 255)},
	{D3DXVECTOR3(-0.5f, -0.5f, -0.5f), D3DCOLOR_RGBA(255, 0, 0, 255)},
	//奥面
	{D3DXVECTOR3(0.5f, 0.5f, 0.5f), D3DCOLOR_RGBA(64, 128, 0, 255)},
	{D3DXVECTOR3(-0.5f, 0.5f, 0.5f), D3DCOLOR_RGBA(64, 128, 0, 255)},
	{D3DXVECTOR3(-0.5f, -0.5f, 0.5f), D3DCOLOR_RGBA(64, 128, 0, 255)},
	{D3DXVECTOR3(-0.5f, -0.5f, 0.5f), D3DCOLOR_RGBA(64, 128, 0, 255)},
	{D3DXVECTOR3(0.5f, -0.5f, 0.5f), D3DCOLOR_RGBA(64, 128, 0, 255)},
	{D3DXVECTOR3(0.5f, 0.5f, 0.5f), D3DCOLOR_RGBA(64, 128, 0, 255)},
	//底面
	{D3DXVECTOR3(0.5f, -0.5f, 0.5f), D3DCOLOR_RGBA(128, 32, 64, 255)},
	{D3DXVECTOR3(-0.5f, -0.5f, 0.5f), D3DCOLOR_RGBA(128, 32, 64, 255)},
	{D3DXVECTOR3(0.5f, -0.5f, -0.5f), D3DCOLOR_RGBA(128, 32, 64, 255)},
	{D3DXVECTOR3(0.5f, -0.5f, -0.5f), D3DCOLOR_RGBA(128, 32, 64, 255)},
	{D3DXVECTOR3(-0.5f, -0.5f, 0.5f), D3DCOLOR_RGBA(128, 32, 64, 255)},
	{D3DXVECTOR3(-0.5f, -0.5f, -0.5f), D3DCOLOR_RGBA(128, 32, 64, 255)},
	//上面
	{D3DXVECTOR3(-0.5f, 0.5f, 0.5f), D3DCOLOR_RGBA(255, 255, 0, 255)},
	{D3DXVECTOR3(0.5f, 0.5f, 0.5f), D3DCOLOR_RGBA(255, 255, 0, 255)},
	{D3DXVECTOR3(-0.5f, 0.5f, -0.5f), D3DCOLOR_RGBA(255, 255, 0, 255)},
	{D3DXVECTOR3(-0.5f, 0.5f, -0.5f), D3DCOLOR_RGBA(255, 255, 0, 255)},
	{D3DXVECTOR3(0.5f, 0.5f, 0.5f), D3DCOLOR_RGBA(255, 255, 0, 255)},
	{D3DXVECTOR3(0.5f, 0.5f, -0.5f), D3DCOLOR_RGBA(255, 255, 0, 255)}

};

static Stone g_Stone[PLAYER_MAX_NUM];
static int g_PlayerTurn = 0;

//=====================================================
//初期化
//=====================================================
void Stone_Init(void)
{
	//ストーンの初期化
	for (int i = 0; i < PLAYER_MAX_NUM; i++)
	{
		g_Stone[i].pos = D3DXVECTOR3(-5.0f + i * 2.0f, 0.5f, 0.0f);	//座標
		D3DXMatrixTranslation(&g_Stone[i].mtxWorld, g_Stone[i].pos.x, g_Stone[i].pos.y, g_Stone[i].pos.z);		//平行移動
		g_Stone[i].move = 0.0f;			//移動量
		g_Stone[i].isTurn = false;
	}
	//最初のストーンのターン設定
	g_Stone[0].isTurn = true;
	g_PlayerTurn = 0;
}

//=====================================================
//終了
//=====================================================
void Stone_Uninit(void)
{

}

//=====================================================
//更新
//=====================================================
void Stone_Update(void)
{
	Keyboard_Update();

	//どのプレイヤーのターンか判別
	for (int i = 0; i < PLAYER_MAX_NUM; i++)
	{
		//ターンが来てなければ次のプレイヤー比較
		if (!g_Stone[i].isTurn)
		{
			continue;
		}
		//左クリック時に移動量の設定
		if (GetAsyncKeyState(VK_LBUTTON))
		{
			g_Stone[i].move = 0.1f * Mouse_GetForce();
		}
		//ストーンの移動
		else
		{
			g_Stone[i].move *= 0.98f;
			D3DXMatrixTranslation(&g_Stone[i].mtxMove, 0, 0, g_Stone[i].move);
		}
		//ワールド座標変換
		D3DXMatrixIdentity(&g_Stone[i].mtxWorld);	//単位行列を作る

		D3DXMatrixRotationY(&g_Stone[i].mtxRotation, Joycon_Operator() * D3DX_PI / 180);		//y軸回転
		D3DXMatrixTranslation(&g_Stone[i].mtxWorld, g_Stone[i].pos.x, g_Stone[i].pos.y, g_Stone[i].pos.z);		//平行移動
		g_Stone[i].mtxWorld = g_Stone[i].mtxMove * g_Stone[i].mtxRotation * g_Stone[i].mtxWorld;

		//行列合成した状態での座標取得
		g_Stone[i].pos.x = g_Stone[i].mtxWorld._41;
		g_Stone[i].pos.y = g_Stone[i].mtxWorld._42;
		g_Stone[i].pos.z = g_Stone[i].mtxWorld._43;
	}
}

//=====================================================
//描画
//=====================================================
void Stone_Draw(void)
{
	DebugFont_Draw(0, 32 * 6, "回転量 = %.02lf", Joycon_Operator() * D3DX_PI / 180);
	//デバイスのポインタ取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//どのプレイヤーのターンか判別
	for (int i = 0; i < PLAYER_MAX_NUM; i++)
	{
		pDevice->SetTransform(D3DTS_WORLD, &g_Stone[i].mtxWorld);
		//描画設定
		pDevice->SetFVF(FVF_CUBE);						//デバイスに頂点データを渡す
		pDevice->SetTexture(0, NULL);					//テクスチャをデバイスに渡す
		pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);	//FALSE:ライトOFF TRUE:ライトON
		pDevice->DrawPrimitiveUP(D3DPT_TRIANGLELIST, 12, g_stone_vertex, sizeof(StoneVertex));
	}
}

//=====================================================
//座標取得
//=====================================================
D3DXVECTOR3 Stone_GetPos(int index)
{
	return g_Stone[index].pos;
}

//=====================================================
//何Pが操作中か
//=====================================================
int Stone_PlayerTurn(void)
{
	return g_PlayerTurn;
}