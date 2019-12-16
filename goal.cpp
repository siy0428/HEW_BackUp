#include <d3dx9.h>
#include "mydirectx.h"
#include "common.h"
#include "debug_font.h"
#include "input.h"
#include "joycon.h"
#include "mouse.h"
#include "camera.h"

//====================================================
//マクロ定義
//====================================================
#define GOAL_HEIGHT (3.0f)	//ゴールの仮高さ
#define DEAD_ZONE (0.001f)	//小数点誤差用
#define GOAL_RANGE (1.0f)	//ゴール判定用マクロ定義

//====================================================
//構造体宣言
//====================================================
typedef struct Goal_Vertex_tag
{
	D3DXVECTOR3 position;	//座標
	D3DCOLOR color;			//色情報
}GoalVertex;
#define FVF_CUBE (D3DFVF_XYZ | D3DFVF_DIFFUSE)

//=====================================================
//グローバル変数
//=====================================================
static const GoalVertex g_goal_vertex[] = {						//頂点構造体
	//正面
	{D3DXVECTOR3(-1.0f, GOAL_HEIGHT, -1.0f), D3DCOLOR_RGBA(255, 255, 255, 255)},
	{D3DXVECTOR3(1.0f, GOAL_HEIGHT, -1.0f),  D3DCOLOR_RGBA(255, 255, 255, 255)},
	{D3DXVECTOR3(1.0f, -GOAL_HEIGHT, -1.0f), D3DCOLOR_RGBA(255, 255, 255, 255)},
	{D3DXVECTOR3(1.0f, -GOAL_HEIGHT, -1.0f), D3DCOLOR_RGBA(255, 255, 255, 255)},
	{D3DXVECTOR3(-1.0f, -GOAL_HEIGHT, -1.0f),D3DCOLOR_RGBA(255, 255, 255, 255)},
	{D3DXVECTOR3(-1.0f, GOAL_HEIGHT, -1.0f), D3DCOLOR_RGBA(255, 255, 255, 255)},
	//右面
	{D3DXVECTOR3(1.0f, GOAL_HEIGHT, -1.0f), D3DCOLOR_RGBA(255, 255, 255, 255)},
	{D3DXVECTOR3(1.0f, GOAL_HEIGHT, 1.0f),  D3DCOLOR_RGBA(255, 255, 255, 255)},
	{D3DXVECTOR3(1.0f, -GOAL_HEIGHT, 1.0f), D3DCOLOR_RGBA(255, 255, 255, 255)},
	{D3DXVECTOR3(1.0f, -GOAL_HEIGHT, 1.0f), D3DCOLOR_RGBA(255, 255, 255, 255)},
	{D3DXVECTOR3(1.0f, -GOAL_HEIGHT, -1.0f),D3DCOLOR_RGBA(255, 255, 255, 255)},
	{D3DXVECTOR3(1.0f, GOAL_HEIGHT, -1.0f), D3DCOLOR_RGBA(255, 255, 255, 255)},
	//左面
	{D3DXVECTOR3(-1.0f, GOAL_HEIGHT, 1.0f), D3DCOLOR_RGBA(255, 255, 255, 255)},
	{D3DXVECTOR3(-1.0f, GOAL_HEIGHT, -1.0f),D3DCOLOR_RGBA(255, 255, 255, 255)},
	{D3DXVECTOR3(-1.0f, -GOAL_HEIGHT, 1.0f),D3DCOLOR_RGBA(255, 255, 255, 255)},
	{D3DXVECTOR3(-1.0f, -GOAL_HEIGHT, 1.0f),D3DCOLOR_RGBA(255, 255, 255, 255)},
	{D3DXVECTOR3(-1.0f, GOAL_HEIGHT, -1.0f),D3DCOLOR_RGBA(255, 255, 255, 255)},
	{D3DXVECTOR3(-1.0f, -GOAL_HEIGHT, -1.0f),D3DCOLOR_RGBA(255, 255, 255, 255)},
	//奥面
	{D3DXVECTOR3(1.0f, GOAL_HEIGHT, 1.0f),  D3DCOLOR_RGBA(255, 255, 255, 255)},
	{D3DXVECTOR3(-1.0f, GOAL_HEIGHT, 1.0f), D3DCOLOR_RGBA(255, 255, 255, 255)},
	{D3DXVECTOR3(-1.0f, -GOAL_HEIGHT, 1.0f),D3DCOLOR_RGBA(255, 255, 255, 255)},
	{D3DXVECTOR3(-1.0f, -GOAL_HEIGHT, 1.0f),D3DCOLOR_RGBA(255, 255, 255, 255)},
	{D3DXVECTOR3(1.0f, -GOAL_HEIGHT, 1.0f), D3DCOLOR_RGBA(255, 255, 255, 255)},
	{D3DXVECTOR3(1.0f, GOAL_HEIGHT, 1.0f),  D3DCOLOR_RGBA(255, 255, 255, 255)},
	//底面
	{D3DXVECTOR3(1.0f, -GOAL_HEIGHT, 1.0f),  D3DCOLOR_RGBA(255, 255, 255, 255)},
	{D3DXVECTOR3(-1.0f, -GOAL_HEIGHT, 1.0f), D3DCOLOR_RGBA(255, 255, 255, 255)},
	{D3DXVECTOR3(1.0f, -GOAL_HEIGHT, -1.0f), D3DCOLOR_RGBA(255, 255, 255, 255)},
	{D3DXVECTOR3(1.0f, -GOAL_HEIGHT, -1.0f), D3DCOLOR_RGBA(255, 255, 255, 255)},
	{D3DXVECTOR3(-1.0f, -GOAL_HEIGHT, 1.0f), D3DCOLOR_RGBA(255, 255, 255, 255)},
	{D3DXVECTOR3(-1.0f, -GOAL_HEIGHT, -1.0f),D3DCOLOR_RGBA(255, 255, 255, 255)},
	//上面
	{D3DXVECTOR3(-1.0f, GOAL_HEIGHT, 1.0f), D3DCOLOR_RGBA(255, 255, 255, 255)},
	{D3DXVECTOR3(1.0f, GOAL_HEIGHT, 1.0f),  D3DCOLOR_RGBA(255, 255, 255, 255)},
	{D3DXVECTOR3(-1.0f, GOAL_HEIGHT, -1.0f),D3DCOLOR_RGBA(255, 255, 255, 255)},
	{D3DXVECTOR3(-1.0f, GOAL_HEIGHT, -1.0f),D3DCOLOR_RGBA(255, 255, 255, 255)},
	{D3DXVECTOR3(1.0f, GOAL_HEIGHT, 1.0f),  D3DCOLOR_RGBA(255, 255, 255, 255)},
	{D3DXVECTOR3(1.0f, GOAL_HEIGHT, -1.0f), D3DCOLOR_RGBA(255, 255, 255, 255)}
};
static D3DXVECTOR3 g_pos(0.0f, 0.5f, 0.0f);
static D3DXMATRIX g_mtxWorld;

//=====================================================
//初期化
//=====================================================
void Goal_Init(void)
{
	g_pos = D3DXVECTOR3(0.0f, 0.5f, 30.0f);
}

//=====================================================
//終了
//=====================================================
void Goal_Uninit(void)
{

}

//=====================================================
//更新
//=====================================================
void Goal_Update(void)
{
	//単位行列
	D3DXMatrixIdentity(&g_mtxWorld);
	D3DXMatrixTranslation(&g_mtxWorld, g_pos.x, g_pos.y, g_pos.z);
}

//=====================================================
//描画
//=====================================================
void Goal_Draw(void)
{
	//デバイスのポインタ取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//描画設定
	pDevice->SetTransform(D3DTS_WORLD, &g_mtxWorld);
	pDevice->SetFVF(FVF_CUBE);						//デバイスに頂点データを渡す
	pDevice->SetTexture(0, NULL);					//テクスチャをデバイスに渡す
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);	//FALSE:ライトOFF TRUE:ライトON
	pDevice->DrawPrimitiveUP(D3DPT_LINELIST, 16, g_goal_vertex, sizeof(GoalVertex));
}

//=====================================================
//ストーンごとのゴールまでの距離
//=====================================================
float Goal_Range(D3DXVECTOR3 pos)
{
	return (float)sqrt((g_pos.x - pos.x) * (g_pos.x - pos.x) + (g_pos.y - pos.y) * (g_pos.y - pos.y) + (g_pos.z - pos.z) * (g_pos.z - pos.z));
}

//=====================================================
//ゴールしたかどうか
//=====================================================
bool Goal_Flag(float goal_range, float move)
{
	//ゴールしていた場合true
	if (goal_range <= GOAL_RANGE && move <= DEAD_ZONE)
	{
		return true;
	}
	return false;
}