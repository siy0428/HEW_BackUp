#include "texture.h"
#include "sprite.h"
#include "stone.h"
#include "common.h"

//=====================================================
//�O���[�o���ϐ�
//=====================================================
static int g_win_player = -1;
static unsigned int g_tex[PLAYER_MAX_NUM] = { 0 };

//=====================================================
//������
//=====================================================
void Result_Init(void)
{
	g_tex[0] = Texture_SetLoadFile("Texture\\1p_goal.png", SCREEN_WIDTH, SCREEN_HEIGHT);
	g_tex[1] = Texture_SetLoadFile("Texture\\2p_goal.png", SCREEN_WIDTH, SCREEN_HEIGHT);
	g_tex[2] = Texture_SetLoadFile("Texture\\3p_goal.png", SCREEN_WIDTH, SCREEN_HEIGHT);
	g_tex[3] = Texture_SetLoadFile("Texture\\4p_goal.png", SCREEN_WIDTH, SCREEN_HEIGHT);
	g_win_player = -1;
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

}

//=====================================================
//�`��
//=====================================================
void Result_Draw(void)
{
	if (g_win_player >= 0)
	{
		Sprite_Draw(g_tex[g_win_player], 0, 0);
	}
}

//=====================================================
//�������̃v���C���[�̗v�f���擾
//=====================================================
void Result_GetWinPlayer(int num)
{
	g_win_player = num;
}