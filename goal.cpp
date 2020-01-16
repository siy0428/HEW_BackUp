#include "common.h"
#include "debug_font.h"
#include "input.h"
#include "joycon.h"
#include "mouse.h"
#include "camera.h"
#include "stone.h"
#include "sprite3d.h"
#include "debug_camera.h"
#include "game.h"
#include "course_camera.h"
#include "mydirectx.h"

//====================================================
//マクロ定義
//====================================================
#define GOAL_HEIGHT (3.0f)	//ゴールの仮高さ
#define DEAD_ZONE (0.001f)	//小数点誤差用
#define GOAL_RANGE (1.0f)	//ゴール判定用マクロ定義

//====================================================
//構造体宣言
//====================================================

//=====================================================
//グローバル変数
//=====================================================
static D3DXVECTOR3 g_pos(0.0f, 0.5f, 0.0f);
static D3DXMATRIX g_mtxWorld;
//static unsigned int g_tex[PLAYER_MAX_NUM] = { 0 };
static unsigned int g_tex;
static int g_win_player = -1;
static bool g_goal_flag;

//=====================================================
//初期化
//=====================================================
void Goal_Init(void)
{
	g_pos = D3DXVECTOR3(0.0f, 0.5f, 30.0f);
	//g_tex[0] = Texture_SetLoadFile("Texture\\1p_goal.png", SCREEN_WIDTH, SCREEN_HEIGHT);
	//g_tex[1] = Texture_SetLoadFile("Texture\\2p_goal.png", SCREEN_WIDTH, SCREEN_HEIGHT);
	//g_tex[2] = Texture_SetLoadFile("Texture\\3p_goal.png", SCREEN_WIDTH, SCREEN_HEIGHT);
	//g_tex[3] = Texture_SetLoadFile("Texture\\4p_goal.png", SCREEN_WIDTH, SCREEN_HEIGHT);
	g_goal_flag = false;
	g_tex = Texture_SetLoadFile("Texture\\goal_flag.png", 64, 256);
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
	D3DXMATRIX mtxScale;
	//単位行列
	D3DXMatrixIdentity(&g_mtxWorld);
	D3DXMatrixTranslation(&g_mtxWorld, g_pos.x, g_pos.y, g_pos.z);
	//大きさ
	D3DXMatrixScaling(&mtxScale, 1.0f, 4.0f, 4.0f);

	//ビルボード
	D3DXMATRIX mtxBill = (Game_GetScene() == SCENE_CAMERA) ? Course_Camera_Billboard() : Camera_Billbord();
	g_mtxWorld = mtxBill * g_mtxWorld;
	g_mtxWorld = mtxScale * g_mtxWorld;

}

//=====================================================
//描画
//=====================================================
void Goal_Draw(void)
{
	//デバイスのポインタ取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);	//FALSE:ライトOFF TRUE:ライトON
	//UV反転
	pDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_MIRROR);
	pDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_MIRROR);
	//描画
	Sprite3d_Draw(g_tex, g_mtxWorld);
	//WRAP
	pDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
	pDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);
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
	return (goal_range <= GOAL_RANGE && move <= DEAD_ZONE) ? true : false;
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
	g_pos.y = mtxWorld._42 + 1.5f;
	g_pos.z = mtxWorld._43;
}

//=====================================================
//ゴールの座標取得
//=====================================================
D3DXVECTOR3 Goal_GetPos(void)
{
	return g_pos;
}