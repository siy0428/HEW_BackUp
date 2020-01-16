#include "texture.h"
#include "texture.h"
#include "sprite.h"
#include "stone.h"
#include "mydirectx.h"
#include "joycon.h"

//====================================================
//マクロ定義
//====================================================
#define GAUGE_WIDTH (32)
#define GAUGE_HEIGHT (256)

//====================================================
//構造体宣言
//====================================================
typedef struct GLine_Vertex_tag
{
	D3DXVECTOR3 position;	//座標
	D3DXVECTOR3 normal;		//法線ベクトル
	D3DCOLOR color;			//色情報
	D3DXVECTOR2 UV;			//テクスチャ
}GLineVertex;
#define FVF_CUBE (D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_NORMAL | D3DFVF_TEX1)

//====================================================
//グローバル変数
//====================================================
static unsigned int g_tex;
static const GLineVertex g_guide_line[] = {
	//正面
	{D3DXVECTOR3(-0.5f, 0.5f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, -1.0f), D3DCOLOR_RGBA(255, 255, 255, 255), D3DXVECTOR2(0.0f, 0.0f)},
	{D3DXVECTOR3(0.5f + GAUGE_WIDTH, 0.5f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, -1.0f), D3DCOLOR_RGBA(255, 255, 255, 255), D3DXVECTOR2(1.0f, 0.0f)},
	{D3DXVECTOR3(-0.5f, -0.5f - GAUGE_HEIGHT, 0.0f), D3DXVECTOR3(0.0f, 0.0f, -1.0f), D3DCOLOR_RGBA(255, 255, 255, 255), D3DXVECTOR2(0.0f, 1.0f)},
	{D3DXVECTOR3(0.5f + GAUGE_WIDTH, -0.5f - GAUGE_HEIGHT, 0.0f),D3DXVECTOR3(0.0f, 0.0f, -1.0f),  D3DCOLOR_RGBA(255, 255, 255, 255), D3DXVECTOR2(1.0f, 1.0f)},
};
static D3DXVECTOR3 g_pos(0.0f, 10.5f, 0.0f);
static D3DXMATRIX g_mtxWorld, g_mtxScaling, g_mtxRotate, g_mtxTrans;

//====================================================
//初期化
//====================================================
void GuideLine_Init(void)
{
	g_tex = Texture_SetLoadFile("Texture\\guide_line.png", GAUGE_WIDTH, GAUGE_HEIGHT);
}

//====================================================
//終了
//====================================================
void GuideLine_Uninit(void)
{

}

//====================================================
//更新
//====================================================
void GuideLine_Update(void)
{
	D3DXMatrixIdentity(&g_mtxWorld);
	D3DXMatrixScaling(&g_mtxScaling, 0.015f, 0.015f, 1.0f);		//大きさ
	D3DXMatrixRotationX(&g_mtxRotate, 90 * D3DX_PI / 180);		//x軸回転
	D3DXMatrixTranslation(&g_mtxTrans, -0.2f, 1.0f, 3.0f);		//描画位置調整用

	g_mtxWorld = g_mtxScaling * g_mtxRotate * g_mtxTrans * Stone_GetMtx(Stone_PlayerTurn());
}

//====================================================
//描画
//====================================================
void GuideLine_Draw(void)
{
	//デバイスのポインタ取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);	//FALSE:ライトOFF TRUE:ライトON

	//ストーンが動いていなければガイドラインを描画する
	if (Stone_GetRange(Stone_PlayerTurn()) >= 0.001f)
	{
		return;
	}
	pDevice->SetTransform(D3DTS_WORLD, &g_mtxWorld);

	//描画設定
	pDevice->SetFVF(FVF_CUBE);						//デバイスに頂点データを渡す
	pDevice->SetTexture(0, Texture_GetTexture(g_tex));	//テクスチャをデバイスに渡す
	pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, g_guide_line, sizeof(GLineVertex));
}