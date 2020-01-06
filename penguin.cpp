#include "model.h"
#include "cube.h"
#include "debug_font.h"
#include "input.h"
#include "OXAllocateHierarchy.h"
#include "mydirectx.h"

//====================================================
//マクロ定義
//====================================================
#define PENGUIN_MAX (16)

//====================================================
//列挙型
//====================================================
typedef enum
{
	STOP,
	IDOLING,
	STEP
}ANIMETION_TYPE;

//====================================================
//構造体宣言
//====================================================
typedef struct
{
	D3DXVECTOR3 pos;
	D3DXMATRIX mtxWorld;
	D3DXMATRIX mtxRot;
	D3DXMATRIX mtxScale;
	ANIMETION_TYPE type;
	int on_stone;
	unsigned int modelId;
	bool isUse;
}PENGUIN;

//====================================================
//グローバル変数
//====================================================
static PENGUIN g_penguin[PENGUIN_MAX];

//====================================================
//初期化
//====================================================
void Penguin_Init(void)
{
	for (int i = 0; i < PENGUIN_MAX; i++)
	{
		g_penguin[i].on_stone = -1;
		g_penguin[i].type = IDOLING;
		g_penguin[i].modelId = ModelAnime_Init("model\\pengin\\pengin.x", "model\\pengin\\pengin.tga");
		g_penguin[i].isUse = false;
		g_penguin[i].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	}
	//g_penguin[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	//g_penguin[1].pos = D3DXVECTOR3(-5.0f, 0.0f, 10.0f);
	//g_penguin[2].pos = D3DXVECTOR3(5.0f, 0.0f, -5.0f);
	//g_penguin[3].pos = D3DXVECTOR3(10.0f, 0.0f, 5.0f);
}

//====================================================
//終了
//====================================================
void Penguin_Uninit(void)
{

}

//====================================================
//更新
//====================================================
void Penguin_Update(void)
{
	for (int i = 0; i < PENGUIN_MAX; i++)
	{
		if (!g_penguin[i].isUse)
		{
			continue;
		}
		D3DXMatrixTranslation(&g_penguin[i].mtxWorld, g_penguin[i].pos.x, g_penguin[i].pos.y, g_penguin[i].pos.z);
	}
}

//====================================================
//描画
//====================================================
void Penguin_Draw(void)
{
	for (int i = 0; i < PENGUIN_MAX; i++)
	{
		if (!g_penguin[i].isUse)
		{
			continue;
		}

		ModelAnime_Draw(g_penguin[i].modelId, 0.0001f, g_penguin[i].mtxWorld, g_penguin[i].type);
	}
}

//====================================================
//何プレイヤーの上に乗っているか
//====================================================
void Penguin_OnStoneNum(int index, int stone_index)
{
	g_penguin[index].on_stone = stone_index;
}

//====================================================
//ペンギンのストーン上フラグの取得
//====================================================
bool Penguin_OnStone(int stone_index)
{
	for (int i = 0; i < PENGUIN_MAX; i++)
	{
		//引数の数のストーンの上にペンギンがいればtrue
		if (g_penguin[i].on_stone == stone_index)
		{
			return true;
		}
	}
	return false;
}

//====================================================
//ステージエディタのデータ取得
//====================================================
void Penguin_Set(D3DXMATRIX mtxWorld)
{
	for (int i = 0; i < PENGUIN_MAX; i++)
	{
		if (g_penguin[i].isUse)
		{
			continue;
		}
		g_penguin[i].isUse = true;
		g_penguin[i].mtxWorld = mtxWorld;
		g_penguin[i].pos.x = mtxWorld._41;
		g_penguin[i].pos.y = mtxWorld._42;
		g_penguin[i].pos.z = mtxWorld._43;
		return;
	}
}

//====================================================
//ステージエディタのデータ取得(要素数指定)
//====================================================
void Penguin_Set(D3DXMATRIX mtxWorld, int index)
{
	g_penguin[index].isUse = true;
	g_penguin[index].mtxWorld = mtxWorld;
	g_penguin[index].pos.x = mtxWorld._41;
	g_penguin[index].pos.y = mtxWorld._42;
	g_penguin[index].pos.z = mtxWorld._43;
}