#include "stone.h"
#include "texture.h"
#include "sprite.h"
#include "band.h"
#include <d3dx9.h>
#include "common.h"
#include "ui_player_turn.h"
#include "debug_font.h"
#include "score.h"
#include "mydirectx.h"

//=====================================================
//�}�N����`
//=====================================================
#define ANIME_SPEED (10.0f)

//=====================================================
//�񋓌^
//=====================================================
enum
{
	TOPBAND,		//��̑�
	BAND,			//������
	PLAYER_TURN,	//��P�̔�
	QUEST,			//�N�G�X�g�\��
	STONE_CHANGE,	//�X�g�[���؂�ւ�
	MINI_MAP,		//�~�j�}�b�v
	RANGE,			//�����\��
	STOP,			//��~���̑�
};

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
static unsigned int g_tex[16];
static UI g_ui;
static int g_stone_change[3];

//=====================================================
//������
//=====================================================
void Ui_PlayerTurn_Init(void)
{
	//��̑�
	g_ui.pos[TOPBAND] = D3DXVECTOR3(0.0f, -400.0f, 0.0f);
	//UI_FIRST_TURN
	g_ui.pos[BAND] = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT / 4, 0.0f);
	g_ui.pos[PLAYER_TURN] = D3DXVECTOR3(SCREEN_WIDTH + 120.0f, SCREEN_HEIGHT / 3, 0.0f);
	g_ui.pos[QUEST] = D3DXVECTOR3(SCREEN_WIDTH + 120.0f, SCREEN_HEIGHT / 4, 0.0f);
	//UI_DIR_CHANGE
	g_ui.pos[STONE_CHANGE] = D3DXVECTOR3(-50.0f, SCREEN_HEIGHT - 150.0f, 0.0f);
	g_ui.pos[MINI_MAP] = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT - 200.0f, 0.0f);
	//UI_RANGE 
	g_ui.pos[RANGE] = D3DXVECTOR3(SCREEN_WIDTH - 300.0f, SCREEN_HEIGHT + 50.0f, 0.0f);
	//UI_STOP
	g_ui.pos[STOP] = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT / 4, 0.0f);

	g_ui.frame = 0;
	g_ui.type_now = UI_FIRST_TURN;
	g_ui.type_next = UI_DIR_CHANGE;
	g_ui.b_band = false;
	g_stone_change[0] = 8;		//�w�r�[
	g_stone_change[1] = 9;		//�m�[�}��
	g_stone_change[2] = 10;		//�t���[�g

	g_tex[0] = Texture_SetLoadFile("Texture\\ueobi.png", SCREEN_WIDTH, 256);		//��̑�
	g_tex[1] = Texture_SetLoadFile("Texture\\1p.png", SCREEN_WIDTH, 256);			//1p�̔�
	g_tex[2] = Texture_SetLoadFile("Texture\\2p.png", SCREEN_WIDTH, 256);			//2p�̔�
	g_tex[3] = Texture_SetLoadFile("Texture\\3p.png", SCREEN_WIDTH, 256);			//3p�̔�
	g_tex[4] = Texture_SetLoadFile("Texture\\4p.png", SCREEN_WIDTH, 256);			//4p�̔�
	g_tex[5] = Texture_SetLoadFile("Texture\\quest.png", SCREEN_WIDTH, 256);		//�N�G�X�g�\��
	g_tex[6] = Texture_SetLoadFile("Texture\\���[�^�[�p����.png", 256, 128);		//���[�^�[�p����
	g_tex[7] = Texture_SetLoadFile("Texture\\�����p����.png", 960, 256);			//�ŏI���[�^�[�p����
	g_tex[g_stone_change[0]] = Texture_SetLoadFile("Texture\\type_change_stone.png", 960 / 4, 320 / 4);		//�X�g�[���؂�ւ�(�m�[�}��)
	g_tex[g_stone_change[1]] = Texture_SetLoadFile("Texture\\type_change_uiteru.png", 960 / 4, 320 / 4);	//�X�g�[���؂�ւ�(�t���[�g)
	g_tex[g_stone_change[2]] = Texture_SetLoadFile("Texture\\type_change_omoi.png", 960 / 4, 320 / 4);		//�X�g�[���؂�ւ�(�w�r�[)


	g_tex[15] = Texture_SetLoadFile("Texture\\white.jpeg", SCREEN_WIDTH, SCREEN_HEIGHT);
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
		if (g_ui.pos[TOPBAND].y < 0.0f)
		{
			g_ui.pos[TOPBAND].y += ANIME_SPEED;
		}
		g_ui.b_band = true;
	}
	//��\��
	else
	{
		(g_ui.pos[TOPBAND].y >= -400.0f) ? g_ui.pos[TOPBAND].y -= ANIME_SPEED : g_ui.b_band = false;
	}

	//�^�C�v�ʃA�j���[�V����
	switch (g_ui.type_now)
	{
	case UI_FIRST_TURN:
		g_ui.pos[BAND].x -= ANIME_SPEED;
		g_ui.pos[PLAYER_TURN].x -= ANIME_SPEED;
		//�ѓ��̕����������Ɏ~�܂�����ꎞ��~
		if (g_ui.pos[PLAYER_TURN].x <= SCREEN_WIDTH / 12)
		{
			if (g_ui.frame <= 100)
			{
				g_ui.pos[PLAYER_TURN].x = SCREEN_WIDTH / 12;
			}
		}
		//��ʒ[�ɏ������玟�̕����A�j���[�V����
		if (g_ui.pos[PLAYER_TURN].x <= -SCREEN_WIDTH + 200.0f)
		{
			g_ui.pos[QUEST].x -= ANIME_SPEED;
			if (g_ui.pos[QUEST].x <= SCREEN_WIDTH / 12)
			{
				if (g_ui.frame <= 600)
				{
					g_ui.pos[QUEST].x = SCREEN_WIDTH / 12;
				}
			}
		}

		//�т���ʒ����Ɏ~�܂�����ꎞ��~
		if (g_ui.pos[BAND].x <= 0.0f)
		{
			//�����̑т���~���Ĉ��t���[���o�ߌ�Ɉړ�
			if (g_ui.frame <= 700)
			{
				g_ui.pos[BAND].x = 0.0f;
				g_ui.frame++;
			}
		}

		//��ʒ[�ɏ�������I��
		if (g_ui.pos[BAND].x <= -SCREEN_WIDTH)
		{
			g_ui.type_next = UI_DIR_CHANGE;
			g_ui.type_now = g_ui.type_next;	//�؂�ւ��I������玟�̃A�j���[�V����
			g_ui.frame = 0;
			g_ui.pos[BAND] = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT / 4, 0.0f);
			g_ui.pos[PLAYER_TURN] = D3DXVECTOR3(SCREEN_WIDTH + 120.0f, SCREEN_HEIGHT / 3, 0.0f);
			g_ui.pos[QUEST] = D3DXVECTOR3(SCREEN_WIDTH + 120.0f, SCREEN_HEIGHT / 4, 0.0f);
		}
		break;
	case UI_DIR_CHANGE:
		//�؂�ւ��A�j���[�V����
		if (g_ui.type_now != g_ui.type_next)
		{
			g_ui.pos[STONE_CHANGE].x -= ANIME_SPEED;
			g_ui.pos[MINI_MAP].x += ANIME_SPEED;
			if (g_ui.pos[STONE_CHANGE].x <= -50.0f)
			{
				g_ui.type_now = g_ui.type_next;	//�؂�ւ��I������玟�̃A�j���[�V����
				g_ui.frame = 0;
			}
		}
		//�ʏ�A�j���[�V����
		else
		{
			if (g_ui.pos[STONE_CHANGE].x <= 150.0f)
			{
				g_ui.pos[STONE_CHANGE].x += ANIME_SPEED;
				g_ui.pos[MINI_MAP].x -= ANIME_SPEED;
			}
		}
		g_ui.frame++;
		break;
	case UI_RANGE:
		//�؂�ւ��A�j���[�V����
		if (g_ui.type_now != g_ui.type_next)
		{
			g_ui.pos[RANGE].y += ANIME_SPEED;
			if (g_ui.pos[RANGE].y >= SCREEN_HEIGHT)
			{
				g_ui.type_now = g_ui.type_next;	//�؂�ւ��I������玟�̃A�j���[�V����
				g_ui.frame = 0;
			}
		}
		//�ʏ�A�j���[�V����
		else
		{
			if (g_ui.pos[RANGE].y >= SCREEN_HEIGHT - 200.0f)
			{
				g_ui.pos[RANGE].y -= ANIME_SPEED;
			}
		}
		g_ui.frame++;
		break;
	case UI_STOP:
		g_ui.pos[STOP].x -= ANIME_SPEED;

		//��ʒ����Ɏ~�܂�����ꎞ��~
		if (g_ui.pos[STOP].x <= 0.0f && g_ui.frame <= 150)
		{
			g_ui.pos[STOP].x = 0.0f;
			g_ui.frame++;
		}

		//��ʒ[�ɏ�������I��
		if (g_ui.pos[STOP].x <= -SCREEN_WIDTH)
		{
			g_ui.type_next = UI_FIRST_TURN;
			g_ui.type_now = g_ui.type_next;	//�؂�ւ��I������玟�̃A�j���[�V����
			g_ui.frame = 0;
			g_ui.pos[STOP] = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT / 4, 0.0f);
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
	//���삵�Ă���X�g�[���̗v�f���擾
	int stone_id = Stone_PlayerTurn();
	//�f�o�C�X�̃|�C���^�擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	int width = Texture_GetWidth(g_tex[8]) / 4;
	int height = Texture_GetHeight(g_tex[8]);

	DebugFont_Draw(0, 32 * 17, "stone_id = %d", stone_id);
	DebugFont_Draw(0, 32 * 18, "�� = %d", width * stone_id);
	DebugFont_Draw(0, 32 * 19, "��2 = %d", width * (stone_id + 1));

	//���e�X�g
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 128);

	//��̑ѕ\��
	Sprite_Draw(g_tex[0], g_ui.pos[TOPBAND].x, g_ui.pos[TOPBAND].y);
	//�X�R�A�\��
	Score_Draw(Stone_GetScore(stone_id), 50.0f, g_ui.pos[TOPBAND].y, 2);

	//�^�C�v�ʃA�j���[�V����
	switch (g_ui.type_now)
	{
	case UI_FIRST_TURN:
		Band_Draw(g_ui.pos[BAND]);
		Sprite_Draw(g_tex[stone_id + 1], g_ui.pos[PLAYER_TURN].x, g_ui.pos[PLAYER_TURN].y);
		Sprite_Draw(g_tex[5], g_ui.pos[QUEST].x, g_ui.pos[QUEST].y);
		break;
	case UI_DIR_CHANGE:
		//�X�g�[���؂�ւ�
		Sprite_Draw(g_tex[g_stone_change[0]], g_ui.pos[STONE_CHANGE].x, g_ui.pos[STONE_CHANGE].y - 50.0f, width * stone_id, 0, width, height, 0.0f, 0.0f, 0.0f, 1.0f);
		Sprite_Draw(g_tex[g_stone_change[1]], g_ui.pos[STONE_CHANGE].x - 100.0f, g_ui.pos[STONE_CHANGE].y, width * stone_id, 0, width, height);
		Sprite_Draw(g_tex[g_stone_change[2]], g_ui.pos[STONE_CHANGE].x + 160.0f, g_ui.pos[STONE_CHANGE].y, width * stone_id, 0, width, height);
		//�~�j�}�b�v
		Sprite_Draw(g_tex[15], g_ui.pos[MINI_MAP].x, g_ui.pos[MINI_MAP].y, 0, 0, 128, 128);
		break;
	case UI_RANGE:
		Sprite_Draw(g_tex[6], g_ui.pos[RANGE].x, g_ui.pos[RANGE].y);
		Score_Draw(Stone_GetRange(stone_id), g_ui.pos[RANGE].x, g_ui.pos[RANGE].y, 2);
		break;
	case UI_STOP:
		Band_Draw(g_ui.pos[STOP]);
		Sprite_Draw(g_tex[7], g_ui.pos[STOP].x + 250.0f, g_ui.pos[STOP].y);
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

//=====================================================
//�X�g�[���؂�ւ��Ńe�N�X�`���؂�ւ�
//=====================================================
void Ui_PlayerTurn_StoneChange(bool right)
{
	for (int i = 0; i < 3; i++)
	{
		if (right)
		{
			g_stone_change[i]++;
			if (g_stone_change[i] > 10)
			{
				g_stone_change[i] = 8;
			}
		}
		else
		{
			g_stone_change[i]--;
			if (g_stone_change[i] < 8)
			{
				g_stone_change[i] = 10;
			}
		}
	}
}