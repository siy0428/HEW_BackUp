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
	int lifeFrame;//エフェクトの命
	int spawnFrame;//誕生日
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
		hen[i].g_Efectflag = false;//未使用
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
		//弾の座標の更新
		int elapsedFrame = g_EfectFrameCount - hen[i].spawnFrame;
		float e = elapsedFrame / (float)hen[i].lifeFrame;
		//αは各成分と乗算される
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

		//弾が画面外だったらfalse
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
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);		// ブレンディング処理(デフォルト)
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);	// αソースカラーの指定
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);		// αデスティネーションカラーの指定(1.0)

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
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);		// ブレンディング処理(デフォルト)
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);	// αソースカラーの指定
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);	// αデスティネーションカラーの指定(1.0 - SRC)
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
