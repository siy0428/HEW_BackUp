#include "stone.h"
#include "texture.h"
#include "sprite.h"
#include "band.h"
#include <d3dx9.h>
#include "common.h"
#include "ui_player_turn.h"
#include "debug_font.h"
#include "score.h"

//=====================================================
//�}�N����`
//=====================================================
#define ANIME_SPEED (10.0f)

//=====================================================
//�\���̐錾
//=====================================================
typedef struct
{
	int frame;
	D3DXVECTOR3 pos[32];
	UI_STATE type_now;
	UI_STATE type_next;
	bool b_band;
}UI;

//=====================================================
//�O���[�o���ϐ�
//=====================================================
static unsigned int g_tex;
static UI g_ui;

//=====================================================
//������
//=====================================================
void Ui_PlayerTurn_Init(void)
{
	//��̑�
	g_ui.pos[0] = D3DXVECTOR3(0, -50.0f, 0.0f);
	//UI_FIRST_TURN
	g_ui.pos[1] = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT / 4, 0.0f);
	//UI_DIR_CHANGE
	g_ui.pos[2] = D3DXVECTOR3(-50.0f, SCREEN_HEIGHT - 200.0f, 0.0f);
	g_ui.pos[3] = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT - 200.0f, 0.0f);
	//UI_RANGE
	g_ui.pos[4] = D3DXVECTOR3(SCREEN_WIDTH - 100.0f, SCREEN_HEIGHT + 50.0f, 0.0f);
	//UI_STOP
	g_ui.pos[5] = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT / 4, 0.0f);

	g_ui.frame = 0;
	g_ui.type_now = UI_FIRST_TURN;
	g_ui.type_next = UI_DIR_CHANGE;
	g_ui.b_band = false;
	g_tex = Texture_SetLoadFile("Texture\\white.jpeg", SCREEN_WIDTH, SCREEN_HEIGHT);
}

//=====================================================
//�I��
//=====================================================
void Ui_PlayerTurn_Uninit(void)
{

}

//=====================================================
//�X�V
//=====================================================
void Ui_PlayerTurn_Update(void)
{
	//��̑ѕ\��
	if (g_ui.type_now != UI_FIRST_TURN)
	{
		if (g_ui.pos[0].y <= 0.0f)
		{
			g_ui.pos[0].y += ANIME_SPEED;
		}
		g_ui.b_band = true;
	}
	//��\��
	else
	{
		(g_ui.pos[0].y >= -50.0f) ? g_ui.pos[0].y -= ANIME_SPEED : g_ui.b_band = false;
	}

	//�^�C�v�ʃA�j���[�V����
	switch (g_ui.type_now)
	{
	case UI_FIRST_TURN:
		g_ui.pos[1].x -= ANIME_SPEED;
		//��ʒ����Ɏ~�܂�����ꎞ��~
		if (g_ui.pos[1].x <= 0.0f && g_ui.frame <= 100)
		{
			g_ui.pos[1].x = 0.0f;
			g_ui.frame++;
		}

		//��ʒ[�ɏ�������I��
		if (g_ui.pos[1].x <= -SCREEN_WIDTH)
		{
			g_ui.type_next = UI_DIR_CHANGE;
			g_ui.type_now = g_ui.type_next;	//�؂�ւ��I������玟�̃A�j���[�V����
			g_ui.frame = 0;
			g_ui.pos[1] = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT / 4, 0.0f);
		}
		break;
	case UI_DIR_CHANGE:
		//�؂�ւ��A�j���[�V����
		if (g_ui.type_now != g_ui.type_next)
		{
			g_ui.pos[2].x -= ANIME_SPEED;
			g_ui.pos[3].x += ANIME_SPEED;
			if (g_ui.pos[2].x <= -50.0f)
			{
				g_ui.type_now = g_ui.type_next;	//�؂�ւ��I������玟�̃A�j���[�V����
				g_ui.frame = 0;
			}
		}
		//�ʏ�A�j���[�V����
		else
		{
			if (g_ui.pos[2].x <= 300.0f)
			{
				g_ui.pos[2].x += ANIME_SPEED;
				g_ui.pos[3].x -= ANIME_SPEED;
			}
		}
		g_ui.frame++;
		break;
	case UI_RANGE:
		//�؂�ւ��A�j���[�V����
		if (g_ui.type_now != g_ui.type_next)
		{
			g_ui.pos[4].y += ANIME_SPEED;
			if (g_ui.pos[4].y >= SCREEN_HEIGHT)
			{
				g_ui.type_now = g_ui.type_next;	//�؂�ւ��I������玟�̃A�j���[�V����
				g_ui.frame = 0;
			}
		}
		//�ʏ�A�j���[�V����
		else
		{
			if (g_ui.pos[4].y >= SCREEN_HEIGHT - 100.0f)
			{
				g_ui.pos[4].y -= ANIME_SPEED;
			}
		}
		g_ui.frame++;
		break;
	case UI_STOP:
		g_ui.pos[5].x -= ANIME_SPEED;
		//��ʒ����Ɏ~�܂�����ꎞ��~
		if (g_ui.pos[5].x <= 0.0f && g_ui.frame <= 100)
		{
			g_ui.pos[5].x = 0.0f;
			g_ui.frame++;
		}

		//��ʒ[�ɏ�������I��
		if (g_ui.pos[5].x <= -SCREEN_WIDTH)
		{
			g_ui.type_next = UI_FIRST_TURN;
			g_ui.type_now = g_ui.type_next;	//�؂�ւ��I������玟�̃A�j���[�V����
			g_ui.frame = 0;
			g_ui.pos[5] = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT / 4, 0.0f);
			Stone_SetTurn();
		}
		break;
	default:
		g_ui.frame++;
		break;
	}
}

//=====================================================
//�`��
//=====================================================
void Ui_PlayerTurn_Draw(void)
{
	int stone_id = Stone_PlayerTurn();

	DebugFont_Draw(0, 32 * 17, "now_type = %d", g_ui.type_now);
	DebugFont_Draw(0, 32 * 18, "next_type = %d", g_ui.type_next);

	//��̑ѕ\��
	Sprite_Draw(g_tex, g_ui.pos[0].x, g_ui.pos[0].y, 0, 0, SCREEN_WIDTH, 50.0f);
	//�X�R�A�\��
	Score_Draw(Stone_GetScore(stone_id), 50.0f, g_ui.pos[0].y, 2);

	//�^�C�v�ʃA�j���[�V����
	switch (g_ui.type_now)
	{
	case UI_FIRST_TURN:
		Band_Draw(g_ui.pos[1]);
		break;
	case UI_DIR_CHANGE:
		Sprite_Draw(g_tex, g_ui.pos[2].x, g_ui.pos[2].y, 0,  0, 128, 128);
		Sprite_Draw(g_tex, g_ui.pos[3].x, g_ui.pos[3].y, 0, 0, 128, 128);
		break;
	case UI_RANGE:
		Sprite_Draw(g_tex, g_ui.pos[4].x, g_ui.pos[4].y, 0, 0, 64, 64);
		Score_Draw(Stone_GetRange(stone_id), g_ui.pos[4].x, g_ui.pos[4].y, 2);
		break;
	case UI_STOP:
		Band_Draw(g_ui.pos[5]);
		break;
	default:
		break;
	}
}

//=====================================================
//�g�p����
//=====================================================
void Ui_PlayerTurn_Create(UI_STATE type)
{
	g_ui.type_next = type;
}

//=====================================================
//�g�p�����ǂ���
//=====================================================
UI_STATE Ui_PlayerTurn_GetType(void)
{
	return g_ui.type_now;
}

//=====================================================
//�g�p�����ǂ���
//=====================================================
UI_STATE Ui_PlayerTurn_GetNextType(void)
{
	return g_ui.type_next;
}