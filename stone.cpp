#include <d3dx9.h>
#include "mydirectx.h"
#include "common.h"
#include "debug_font.h"
#include "input.h"
#include "joycon.h"
#include "mouse.h"
#include "camera.h"
#include "stone.h"
#include "goal.h"

//====================================================
//マクロ定義
//====================================================
#define PLAYER_MAX_NUM (4)	//プレイ人数
#define DEAD_ZONE (0.001f)	//小数点誤差用

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
	D3DXMATRIX mtx_world;		//ワールド行列
	D3DXMATRIX mtx_rotation;	//回転行列
	D3DXMATRIX mtx_trans;		//平行移動
	D3DXMATRIX mtx_move;		//移動量
	D3DXVECTOR3 pos = { 0.0f, 0.5f, 0.0f };				//位置座標
	D3DXVECTOR3 start_pos = { 0.0f, 0.5f, 0.0f };		//1F前の位置座標
	float move;
	bool is_turn;
	bool is_move;
	float goal_range;
	float stone_range;
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

static Stone g_stone[PLAYER_MAX_NUM];
static int g_player_turn = 0;
static bool g_throw_start = false;

//=====================================================
//初期化
//=====================================================
void Stone_Init(void)
{
	//ストーンの初期化
	for (int i = 0; i < PLAYER_MAX_NUM; i++)
	{
		g_stone[i].pos = D3DXVECTOR3(-5.0f + i * 2.0f, 0.5f, 0.0f);	//座標
		g_stone[i].start_pos = D3DXVECTOR3(-5.0f + i * 2.0f, 0.5f, 0.0f);	//座標
		D3DXMatrixTranslation(&g_stone[i].mtx_world, g_stone[i].pos.x, g_stone[i].pos.y, g_stone[i].pos.z);		//平行移動
		g_stone[i].move = 0.0f;			//移動量
		g_stone[i].is_turn = false;
		g_stone[i].is_move = false;
		g_stone[i].goal_range = Goal_Range(g_stone[i].pos);
		g_stone[i].stone_range = 0.0f;
	}
	//最初のストーンのターン設定
	g_stone[0].is_turn = true;
	g_player_turn = 0;
	g_throw_start = false;
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
	//どのプレイヤーのターンか判別
	for (int i = 0; i < PLAYER_MAX_NUM; i++)
	{
		//ターンが来てなければ次のプレイヤー比較
		if (!g_stone[i].is_turn)
		{
			continue;
		}
		//左クリック時で且つ動いてないければ移動量の設定
		if (GetAsyncKeyState(VK_LBUTTON) && Stone_Range(g_stone[i].pos, g_stone[i].start_pos) <= DEAD_ZONE)
		{
			g_stone[i].move = 0.1f * Mouse_GetForce();
		}
		//ストーンの移動
		else
		{
			g_stone[i].move *= 0.98f;
			D3DXMatrixTranslation(&g_stone[i].mtx_move, 0, 0, g_stone[i].move);
			g_stone[i].goal_range = Goal_Range(g_stone[i].pos);	//ゴールまでの距離計算
			if (g_stone[i].move >= DEAD_ZONE)
			{
				g_throw_start = true;
			}
			//移動量がDEAD_ZONE以下で次のターン
			if (g_stone[i].move <= DEAD_ZONE && g_throw_start)
			{
				g_stone[g_player_turn].is_turn = false;							//現在操作しているストーンのターンをfalse
				g_stone[g_player_turn].start_pos = g_stone[g_player_turn].pos;	//スタート位置に今の座標を代入
				g_stone[g_player_turn].move = 0.0f;								//移動量の初期化
				g_player_turn++;												//次のプレイヤーのターン
				g_player_turn %= 4;												//0～3で繰り返すための余剰演算子
				g_stone[g_player_turn].is_turn = true;							//プラスされた現在のプレイヤーのターンをtrueに
				g_throw_start = false;											//スタート用変数をfalse
				Joycon_Reset();	//回転値リセット
			}
		}
		//ストーンの進んだ距離
		g_stone[i].stone_range = Stone_Range(g_stone[i].pos, g_stone[i].start_pos);

		//ワールド座標変換
		D3DXMatrixIdentity(&g_stone[i].mtx_world);	//単位行列を作る
		D3DXMatrixRotationY(&g_stone[i].mtx_rotation, Joycon_Operator() * D3DX_PI / 180);		//y軸回転
		D3DXMatrixTranslation(&g_stone[i].mtx_world, g_stone[i].pos.x, g_stone[i].pos.y, g_stone[i].pos.z);		//平行移動
		g_stone[i].mtx_world = g_stone[i].mtx_move * g_stone[i].mtx_rotation * g_stone[i].mtx_world;

		//行列合成した状態での座標取得
		g_stone[i].pos.x = g_stone[i].mtx_world._41;
		g_stone[i].pos.y = g_stone[i].mtx_world._42;
		g_stone[i].pos.z = g_stone[i].mtx_world._43;
	}
}

//=====================================================
//描画
//=====================================================
void Stone_Draw(void)
{
	//デバイスのポインタ取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	for (int i = 0; i < PLAYER_MAX_NUM; i++)
	{
		//DebugFont_Draw(0, 32 * (6 + i), "%dPが進んだ距離 = %.02lf", i + 1, g_stone[i].range);
		//DebugFont_Draw(0, 32 * (6 + i), "%dPのゴールまでの距離 = %.02lf", i + 1, g_stone[i].goal_range);
		DebugFont_Draw(0, 32 * (6 + i), "%dPの進んだ距離 = %.02lf", i + 1, g_stone[i].stone_range);

		//描画設定
		pDevice->SetTransform(D3DTS_WORLD, &g_stone[i].mtx_world);
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
	return g_stone[index].pos;
}

//=====================================================
//何Pが操作中か
//=====================================================
int Stone_PlayerTurn(void)
{
	return g_player_turn;
}

//=====================================================
//ストーンの進んだ距離
//=====================================================
float Stone_Range(D3DXVECTOR3 pos, D3DXVECTOR3 start_pos)
{
	return sqrt((pos.x - start_pos.x) * (pos.x - start_pos.x) + (pos.y - start_pos.y) * (pos.y - start_pos.y) + (pos.z - start_pos.z) * (pos.z - start_pos.z));
}

//=====================================================
//ストーンが動いているかどうか
//=====================================================
bool Stone_Move(int index)
{
	//ストーンの移動量がDEAD_ZONE以下であれば静止中と判断
	if (g_stone[index].move <= DEAD_ZONE)
	{
		return false;
	}
	return true;
}