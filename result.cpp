#include "texture.h"
#include "sprite.h"
#include "stone.h"
#include "common.h"
#include "input.h"
#include "fade.h"
#include "game.h"
#include "stone.h"
#include "debug_font.h"

//=====================================================
//グローバル変数
//=====================================================
static int g_rank[PLAYER_MAX_NUM] = { 0 };
static int g_score[2][PLAYER_MAX_NUM] = { 0 };
static int g_same_rank[PLAYER_MAX_NUM] = { 0 };

//=====================================================
//初期化
//=====================================================
void Result_Init(void)
{
	for (int i = 0; i < PLAYER_MAX_NUM; i++)
	{
		g_same_rank[i] = i + 1;
	}
}

//=====================================================
//終了
//=====================================================
void Result_Uninit(void)
{

}

//=====================================================
//更新
//=====================================================
void Result_Update(void)
{
	if (Keyboard_IsTrigger(DIK_SPACE))
	{
		Game_Init(Game_GetHwnd());
		Fade_InOut(SCENE_TITLE);
	}
}

//=====================================================
//描画
//=====================================================
void Result_Draw(void)
{
	//DebugFont_Draw(0, 32 * 0, "1位 = %dP", g_rank[0] + 1);
	//DebugFont_Draw(0, 32 * 1, "2位 = %dP", g_rank[1] + 1);
	//DebugFont_Draw(0, 32 * 2, "3位 = %dP", g_rank[2] + 1);
	//DebugFont_Draw(0, 32 * 3, "4位 = %dP", g_rank[3] + 1);
	DebugFont_Draw(0, 32 * 0, "%d位 = %dP(SCORE:%d)", g_same_rank[0], g_rank[0] + 1, Stone_GetScore(g_rank[0]));
	DebugFont_Draw(0, 32 * 1, "%d位 = %dP(SCORE:%d)", g_same_rank[1], g_rank[1] + 1, Stone_GetScore(g_rank[1]));
	DebugFont_Draw(0, 32 * 2, "%d位 = %dP(SCORE:%d)", g_same_rank[2], g_rank[2] + 1, Stone_GetScore(g_rank[2]));
	DebugFont_Draw(0, 32 * 3, "%d位 = %dP(SCORE:%d)", g_same_rank[3], g_rank[3] + 1, Stone_GetScore(g_rank[3]));
}

//=====================================================
//スコア取得
//=====================================================
void Result_SetScore(int index, int score)
{
	for (int i = 0; i < 2; i++)
	{
		g_score[i][index] = score;
	}
}

//=====================================================
//順位取得
//=====================================================
void Result_GetRanking(void)
{
	//スコアの小さい順ソート
	for (int i = 0; i < PLAYER_MAX_NUM - 1; i++)
	{
		for (int j = i + 1; j < PLAYER_MAX_NUM; j++)
		{
			if (g_score[0][i] > g_score[0][j])
			{
				int work = g_score[0][i];
				g_score[0][i] = g_score[0][j];
				g_score[0][j] = work;
			}
		}
	}

	//順位表示
	for (int i = 1; i < PLAYER_MAX_NUM; i++)
	{
		//同じスコアなら同率順位設定
		if (g_score[0][i] == g_score[0][i - 1])
		{
			g_same_rank[i] = g_same_rank[i - 1];
		}
	}

	//順位ソート
	for (int i = 0; i < PLAYER_MAX_NUM; i++)
	{
		g_rank[i] = 0;
		for (int j = 1; j < PLAYER_MAX_NUM; j++)
		{
			if (g_score[1][g_rank[i]] > g_score[1][j])
			{
				g_rank[i] = j;
			}
		}
		g_score[1][g_rank[i]] = 99;
	}
}