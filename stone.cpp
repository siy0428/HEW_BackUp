#include <d3dx9.h>
#include "mydirectx.h"
#include "common.h"
#include "debug_font.h"
#include "input.h"
#include "joycon.h"
#include "mouse.h"

//====================================================
//構造体宣言
//====================================================
typedef struct Stone_Vertex_tag
{
	D3DXVECTOR3 position;	//座標
	D3DCOLOR color;			//色情報
}StoneVertex;
#define FVF_CUBE (D3DFVF_XYZ | D3DFVF_DIFFUSE)

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
	{D3DXVECTOR3(0.5f, 0.5f, -0.5f), D3DCOLOR_RGBA(255, 255, 0, 255)},
};
static D3DXVECTOR3 g_pos = { 0.0f, 0.5f, 0.0f };
static 	float g_move = 0;
static 	D3DXMATRIX g_mtxWorld, g_mtxRotation, g_mtxTrans, s_matrix;		//行列用変数

//=====================================================
//初期化
//=====================================================
void Stone_Init(void)
{
	g_pos = D3DXVECTOR3(0.0f, 0.5f, 0.0f);
	g_move = 0;
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

	//左クリック
	//if (GetAsyncKeyState(VK_LBUTTON))
	if(Keyboard_IsPress(DIK_W))
	{
		//g_move = 0.25f * (Mouse_GetForce());
		D3DXMatrixTranslation(&s_matrix, 0, 0, 0.2f);
	}
	else
	{
		//g_move *= 0.95f;
		//g_pos.z += g_move;
		D3DXMatrixTranslation(&s_matrix, 0, 0, 0);
	}
}

//=====================================================
//描画
//=====================================================
void Stone_Draw(void)
{
	DebugFont_Draw(0, 32 * 6, "回転量 = %.02lf", Joycon_Operator() * D3DX_PI / 180);
	//DebugFont_Draw(0, 32 * 4, "z座標 = %.02lf", g_pos.z);
	//デバイスのポインタ取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//ワールド座標変換
	D3DXMatrixIdentity(&g_mtxWorld);					//単位行列を作る

	D3DXMatrixRotationY(&g_mtxRotation, Joycon_Operator() * D3DX_PI / 180);		//y軸回転
	//D3DXMatrixTranslation(&g_mtxTrans, g_pos.x, g_pos.y, g_pos.z);				//平行移動
	D3DXMatrixTranslation(&g_mtxWorld, g_pos.x, g_pos.y, g_pos.z);
	//g_mtxWorld = g_mtxRotation * g_mtxTrans;			//行列の合成
	g_mtxWorld = s_matrix * g_mtxRotation * g_mtxWorld;

	g_pos.x = g_mtxWorld._41;
	g_pos.y = g_mtxWorld._42;
	g_pos.z = g_mtxWorld._43;

	pDevice->SetTransform(D3DTS_WORLD, &g_mtxWorld);

	//描画設定
	pDevice->SetFVF(FVF_CUBE);						//デバイスに頂点データを渡す
	pDevice->SetTexture(0, NULL);					//テクスチャをデバイスに渡す
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);	//FALSE:ライトOFF TRUE:ライトON
	pDevice->DrawPrimitiveUP(D3DPT_TRIANGLELIST, 12, g_stone_vertex, sizeof(StoneVertex));
}

//=====================================================
//座標取得
//=====================================================
D3DXVECTOR3 Stone_GetPos(void)
{
	return g_pos;
}