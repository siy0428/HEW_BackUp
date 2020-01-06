#include <d3dx9.h>
#include "mydirectx.h"
#include "common.h"
#include "debug_font.h"
#include "input.h"
#include "joycon.h"
#include "mouse.h"
#include "camera.h"
#include "stone.h"
#include "texture.h"
#include "sprite.h"

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
static unsigned int g_tex[PLAYER_MAX_NUM] = { 0 };
static int g_win_player = -1;
static bool g_goal_flag;

//=====================================================
//初期化
//=====================================================
void Goal_Init(void)
{
	g_pos = D3DXVECTOR3(0.0f, 0.5f, 30.0f);
	g_tex[0] = Texture_SetLoadFile("Texture\\1p_goal.png", SCREEN_WIDTH, SCREEN_HEIGHT);
	g_tex[1] = Texture_SetLoadFile("Texture\\2p_goal.png", SCREEN_WIDTH, SCREEN_HEIGHT);
	g_tex[2] = Texture_SetLoadFile("Texture\\3p_goal.png", SCREEN_WIDTH, SCREEN_HEIGHT);
	g_tex[3] = Texture_SetLoadFile("Texture\\4p_goal.png", SCREEN_WIDTH, SCREEN_HEIGHT);
	g_goal_flag = false;
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

	if (g_goal_flag)
	{
		Sprite_Draw(g_tex[g_win_player], 0, 0);
	}

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
	////ゴールしていた場合true
	//if (goal_range <= GOAL_RANGE && move <= DEAD_ZONE)
	//{
	//	return true;
	//}
	//return false;

	g_goal_flag = (goal_range <= GOAL_RANGE && move <= DEAD_ZONE) ? true : false;

	return g_goal_flag;
}

//=====================================================
//勝利時のプレイヤーの要素数取得
//=====================================================
void Goal_GetWinPlayer(int num)
{
	g_win_player = num;
}

//=====================================================
//ステージエディタのデータ取得
//=====================================================
void Goal_Set(D3DXMATRIX mtxWorld)
{
	g_mtxWorld = mtxWorld;
	g_pos.x = mtxWorld._41;
	g_pos.y = mtxWorld._42;
	g_pos.z = mtxWorld._43;
}