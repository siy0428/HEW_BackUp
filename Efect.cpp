#include"sprite.h"
#include"Texture.h"
#include"input.h"
#include"common.h"
#include"collision.h"
#include "Efect.h"
#include"mydirectx.h"

typedef struct Efect_tag
{
	D3DXVECTOR2 Efectpos;

	bool g_Efectflag;
	int lifeFrame;//�G�t�F�N�g�̖�
	int spawnFrame;//�a����
	D3DCOLOR color;
	TYPE type;
}Efect;

static int g_Efect_tex[2];
static float g_Efectsp = 30.0f;
static int g_EfectFrameCount = 0;
static Efect hen[EFECTMAX] = {};

void EfectInit()
{
	g_EfectFrameCount = 0;
	g_Efect_tex[0] = Texture_SetLoadFile("Texture\\bullet.png", 64, 64);
	g_Efect_tex[1] = Texture_SetLoadFile("Texture\\player.png", 192, 128);

	for (int i = 0; i < EFECTMAX; i++)
	{
		hen[i].Efectpos.x = 0.0f;
		hen[i].Efectpos.y = 0.0f;
		hen[i].g_Efectflag = false;//���g�p
		hen[i].lifeFrame = 0;
		hen[i].spawnFrame = -1;
	}
}
void EfectUpdate()
{
	for (int i = 0; i < EFECTMAX; i++)
	{
		if (hen[i].lifeFrame < 0)
		{
			continue;
		}
		//�e�̍��W�̍X�V
		int elapsedFrame = g_EfectFrameCount - hen[i].spawnFrame;
		float e = elapsedFrame / (float)hen[i].lifeFrame;
		//���͊e�����Ə�Z�����
		if (e > 1.0f)
		{
			e = 1.0f;
			hen[i].lifeFrame = 0;
			hen[i].spawnFrame = -1;
			continue;
		}

		D3DXCOLOR color = hen[i].color;
		color.a = 1.0f - e;
		hen[i].color = color;

		//�e����ʊO��������false
		if (hen[i].Efectpos.x < 0)
		{
			hen[i].g_Efectflag = false;
		}
	}
	g_EfectFrameCount++;
}

void EfectDraw()
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);		// �u�����f�B���O����(�f�t�H���g)
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);	// ���\�[�X�J���[�̎w��
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);		// ���f�X�e�B�l�[�V�����J���[�̎w��(1.0)

	for (int i = 0; i < EFECTMAX; i++)
	{
		if (hen[i].lifeFrame > 0)
		{
			Sprite_SetColor(hen[i].color);
			if (hen[i].type == EFFECT_PLAYER)
			{
				Sprite_Draw(g_Efect_tex[1], hen[i].Efectpos.x, hen[i].Efectpos.y, 0, 0, 64, 64);
			}
			else if (hen[i].type == EFFECT_BULLET)
			{
				//Sprite_Draw(g_Efect_tex[0], hen[i].Efectpos.x, hen[i].Efectpos.y);
			}
		}
	}
	Sprite_SetColor(0xffffffff);
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);		// �u�����f�B���O����(�f�t�H���g)
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);	// ���\�[�X�J���[�̎w��
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);	// ���f�X�e�B�l�[�V�����J���[�̎w��(1.0 - SRC)
}

void EfectUninit()
{

}
void Efect_Create(float x, float y, D3DCOLOR color, int lifeFrame, TYPE type)
{
	for (int i = 0; i < EFECTMAX; i++)
	{
		if (hen[i].lifeFrame > 0)
		{
			continue;
		}

		hen[i].Efectpos.x = x;
		hen[i].Efectpos.y = y;
		hen[i].color = color;
		hen[i].lifeFrame = lifeFrame;
		hen[i].spawnFrame = g_EfectFrameCount;
		hen[i].type = type;
		break;
	}
}
