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
//�O���[�o���ϐ�
//=====================================================
static int g_rank[PLAYER_MAX_NUM] = { 0 };
static int g_score[2][PLAYER_MAX_NUM] = { 0 };
static int g_same_rank[PLAYER_MAX_NUM] = { 0 };

//=====================================================
//������
//=====================================================
void Result_Init(void)
{
	for (int i = 0; i < PLAYER_MAX_NUM; i++)
	{
		g_same_rank[i] = i + 1;
	}
}

//=====================================================
//�I��
//=====================================================
void Result_Uninit(void)
{

}

//=====================================================
//�X�V
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
//�`��
//=====================================================
void Result_Draw(void)
{
	//DebugFont_Draw(0, 32 * 0, "1�� = %dP", g_rank[0] + 1);
	//DebugFont_Draw(0, 32 * 1, "2�� = %dP", g_rank[1] + 1);
	//DebugFont_Draw(0, 32 * 2, "3�� = %dP", g_rank[2] + 1);
	//DebugFont_Draw(0, 32 * 3, "4�� = %dP", g_rank[3] + 1);
	DebugFont_Draw(0, 32 * 0, "%d�� = %dP(SCORE:%d)", g_same_rank[0], g_rank[0] + 1, Stone_GetScore(g_rank[0]));
	DebugFont_Draw(0, 32 * 1, "%d�� = %dP(SCORE:%d)", g_same_rank[1], g_rank[1] + 1, Stone_GetScore(g_rank[1]));
	DebugFont_Draw(0, 32 * 2, "%d�� = %dP(SCORE:%d)", g_same_rank[2], g_rank[2] + 1, Stone_GetScore(g_rank[2]));
	DebugFont_Draw(0, 32 * 3, "%d�� = %dP(SCORE:%d)", g_same_rank[3], g_rank[3] + 1, Stone_GetScore(g_rank[3]));
}

//=====================================================
//�X�R�A�擾
//=====================================================
void Result_SetScore(int index, int score)
{
	for (int i = 0; i < 2; i++)
	{
		g_score[i][index] = score;
	}
}

//=====================================================
//���ʎ擾
//=====================================================
void Result_GetRanking(void)
{
	//�X�R�A�̏��������\�[�g
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

	//���ʕ\��
	for (int i = 1; i < PLAYER_MAX_NUM; i++)
	{
		//�����X�R�A�Ȃ瓯�����ʐݒ�
		if (g_score[0][i] == g_score[0][i - 1])
		{
			g_same_rank[i] = g_same_rank[i - 1];
		}
	}

	//���ʃ\�[�g
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