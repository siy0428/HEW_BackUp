//基本
#include "debug_font.h"
#include "input.h"

//乱数用
#include <windows.h>
#include <time.h>

//描画
#include "texture.h"
#include "sprite.h"

//====================================================
//マクロ定義
//====================================================


//====================================================
//列挙型
//====================================================
typedef enum
{
	PLAYER_1_THROW,			//1Pのストーンにぶつける
	PLAYER_2_THROW,			//2Pのストーンにぶつける
	PLAYER_3_THROW,			//3Pのストーンにぶつける
	PLAYER_4_THROW,			//4Pのストーンにぶつける
	STONE_THROW,			//指定したストーンで投げる
	RANGE_THROW,			//ストーンを一定の距離投げる
	CURVE_THROW,			//カーブをかけて投げる
	STONE_HIT_POND,			//他のストーンを弾いて池に落とす
	STONE_HIT_BEAR,			//他のストーンを弾いて白熊にぶつける
	STONE_HIT_PENGUIN,		//他のストーンを弾いてペンギンにぶつける
	FALL_ICICLES,			//つららを落とす
	QUEST_MAX				//最大クエスト数
}QUEST_TYPE;

//====================================================
//グローバル変数
//====================================================
static int g_tex[QUEST_MAX];
static QUEST_TYPE g_type;

//====================================================
//初期化
//====================================================
void Quest_Init(void)
{
	//秒数で乱数のシード値取得
	srand((unsigned int)time(NULL));
	//各クエストのテクスチャ取得
	g_tex[PLAYER_1_THROW] = Texture_SetLoadFile("Texture\\", 0, 0);
	g_tex[PLAYER_2_THROW] = Texture_SetLoadFile("Texture\\", 0, 0);
	g_tex[PLAYER_3_THROW] = Texture_SetLoadFile("Texture\\", 0, 0);
	g_tex[PLAYER_4_THROW] = Texture_SetLoadFile("Texture\\", 0, 0);
	g_tex[STONE_THROW] = Texture_SetLoadFile("Texture\\", 0, 0);
	g_tex[RANGE_THROW] = Texture_SetLoadFile("Texture\\", 0, 0);
	g_tex[CURVE_THROW] = Texture_SetLoadFile("Texture\\", 0, 0);
	g_tex[STONE_HIT_POND] = Texture_SetLoadFile("Texture\\", 0, 0);
	g_tex[STONE_HIT_BEAR] = Texture_SetLoadFile("Texture\\", 0, 0);
	g_tex[STONE_HIT_PENGUIN] = Texture_SetLoadFile("Texture\\", 0, 0);
	g_tex[FALL_ICICLES] = Texture_SetLoadFile("Texture\\", 0, 0);
	g_type = (QUEST_TYPE)1;
}

//====================================================
//終了
//====================================================
void Quest_Uninit(void)
{

}

//====================================================
//更新
//====================================================
void Quest_Update(void)
{
	int randum = rand() % QUEST_MAX;
	if (Keyboard_IsTrigger(DIK_Q))
	{
		g_type = (QUEST_TYPE)((int)g_type + 1);
	}
}

//====================================================
//描画
//====================================================
void Quest_Draw(D3DXVECTOR3 pos)
{
	DebugFont_Draw(0, 32 * 5, "type = %d", g_type);
	Sprite_Draw(g_tex[g_type], pos.x, pos.y);
}

//====================================================
//生成
//====================================================
void Quest_Create(void)
{
	int randum = rand() % QUEST_MAX;
	g_type = (QUEST_TYPE)randum;
}

//タイプごとにクエスト描画
//switch (g_type)
//{
//case PLAYER_1_THROW:
//	break;
//case PLAYER_2_THROW:
//	break;
//case PLAYER_3_THROW:
//	break;
//case PLAYER_4_THROW:
//	break;
//case STONE_THROW:
//	break;
//case RANGE_THROW:
//	break;
//case CURVE_THROW:
//	break;
//case STONE_HIT_POND:
//	break;
//case STONE_HIT_BEAR:
//	break;
//case STONE_HIT_PENGUIN:
//	break;
//case FALL_ICICLES:
//	break;
//}