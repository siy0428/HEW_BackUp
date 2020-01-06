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
//マクロ定義
//=====================================================
#define ANIME_SPEED (10.0f)

//=====================================================
//列挙型
//=====================================================
enum
{
	TOPBAND,		//上の帯
	BAND,			//中央帯
	PLAYER_TURN,	//何Pの番
	QUEST,			//クエスト表示
	STONE_CHANGE,	//ストーン切り替え
	MINI_MAP,		//ミニマップ
	RANGE,			//距離表示
	STOP,			//停止時の帯
};

//=====================================================
//構造体宣言
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
//グローバル変数
//=====================================================
static unsigned int g_tex[16];
static UI g_ui;
static int g_stone_change[3];

//=====================================================
//初期化
//=====================================================
void Ui_PlayerTurn_Init(void)
{
	//上の帯
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
	g_stone_change[0] = 8;		//ヘビー
	g_stone_change[1] = 9;		//ノーマル
	g_stone_change[2] = 10;		//フロート

	g_tex[0] = Texture_SetLoadFile("Texture\\ueobi.png", SCREEN_WIDTH, 256);		//上の帯
	g_tex[1] = Texture_SetLoadFile("Texture\\1p.png", SCREEN_WIDTH, 256);			//1pの番
	g_tex[2] = Texture_SetLoadFile("Texture\\2p.png", SCREEN_WIDTH, 256);			//2pの番
	g_tex[3] = Texture_SetLoadFile("Texture\\3p.png", SCREEN_WIDTH, 256);			//3pの番
	g_tex[4] = Texture_SetLoadFile("Texture\\4p.png", SCREEN_WIDTH, 256);			//4pの番
	g_tex[5] = Texture_SetLoadFile("Texture\\quest.png", SCREEN_WIDTH, 256);		//クエスト表示
	g_tex[6] = Texture_SetLoadFile("Texture\\メーター用距離.png", 256, 128);		//メーター用距離
	g_tex[7] = Texture_SetLoadFile("Texture\\中央用距離.png", 960, 256);			//最終メーター用距離
	g_tex[g_stone_change[0]] = Texture_SetLoadFile("Texture\\type_change_stone.png", 960 / 4, 320 / 4);		//ストーン切り替え(ノーマル)
	g_tex[g_stone_change[1]] = Texture_SetLoadFile("Texture\\type_change_uiteru.png", 960 / 4, 320 / 4);	//ストーン切り替え(フロート)
	g_tex[g_stone_change[2]] = Texture_SetLoadFile("Texture\\type_change_omoi.png", 960 / 4, 320 / 4);		//ストーン切り替え(ヘビー)


	g_tex[15] = Texture_SetLoadFile("Texture\\white.jpeg", SCREEN_WIDTH, SCREEN_HEIGHT);
}

//=====================================================
//終了
//=====================================================
void Ui_PlayerTurn_Uninit(void)
{

}

//=====================================================
//更新
//=====================================================
void Ui_PlayerTurn_Update(void)
{
	//上の帯表示
	if (g_ui.type_now != UI_FIRST_TURN)
	{
		if (g_ui.pos[TOPBAND].y < 0.0f)
		{
			g_ui.pos[TOPBAND].y += ANIME_SPEED;
		}
		g_ui.b_band = true;
	}
	//非表示
	else
	{
		(g_ui.pos[TOPBAND].y >= -400.0f) ? g_ui.pos[TOPBAND].y -= ANIME_SPEED : g_ui.b_band = false;
	}

	//タイプ別アニメーション
	switch (g_ui.type_now)
	{
	case UI_FIRST_TURN:
		g_ui.pos[BAND].x -= ANIME_SPEED;
		g_ui.pos[PLAYER_TURN].x -= ANIME_SPEED;
		//帯内の文字が中央に止まったら一時停止
		if (g_ui.pos[PLAYER_TURN].x <= SCREEN_WIDTH / 12)
		{
			if (g_ui.frame <= 100)
			{
				g_ui.pos[PLAYER_TURN].x = SCREEN_WIDTH / 12;
			}
		}
		//画面端に消えたら次の文字アニメーション
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

		//帯が画面中央に止まったら一時停止
		if (g_ui.pos[BAND].x <= 0.0f)
		{
			//中央の帯が停止して一定フレーム経過後に移動
			if (g_ui.frame <= 700)
			{
				g_ui.pos[BAND].x = 0.0f;
				g_ui.frame++;
			}
		}

		//画面端に消えたら終了
		if (g_ui.pos[BAND].x <= -SCREEN_WIDTH)
		{
			g_ui.type_next = UI_DIR_CHANGE;
			g_ui.type_now = g_ui.type_next;	//切り替え終わったら次のアニメーション
			g_ui.frame = 0;
			g_ui.pos[BAND] = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT / 4, 0.0f);
			g_ui.pos[PLAYER_TURN] = D3DXVECTOR3(SCREEN_WIDTH + 120.0f, SCREEN_HEIGHT / 3, 0.0f);
			g_ui.pos[QUEST] = D3DXVECTOR3(SCREEN_WIDTH + 120.0f, SCREEN_HEIGHT / 4, 0.0f);
		}
		break;
	case UI_DIR_CHANGE:
		//切り替えアニメーション
		if (g_ui.type_now != g_ui.type_next)
		{
			g_ui.pos[STONE_CHANGE].x -= ANIME_SPEED;
			g_ui.pos[MINI_MAP].x += ANIME_SPEED;
			if (g_ui.pos[STONE_CHANGE].x <= -50.0f)
			{
				g_ui.type_now = g_ui.type_next;	//切り替え終わったら次のアニメーション
				g_ui.frame = 0;
			}
		}
		//通常アニメーション
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
		//切り替えアニメーション
		if (g_ui.type_now != g_ui.type_next)
		{
			g_ui.pos[RANGE].y += ANIME_SPEED;
			if (g_ui.pos[RANGE].y >= SCREEN_HEIGHT)
			{
				g_ui.type_now = g_ui.type_next;	//切り替え終わったら次のアニメーション
				g_ui.frame = 0;
			}
		}
		//通常アニメーション
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

		//画面中央に止まったら一時停止
		if (g_ui.pos[STOP].x <= 0.0f && g_ui.frame <= 150)
		{
			g_ui.pos[STOP].x = 0.0f;
			g_ui.frame++;
		}

		//画面端に消えたら終了
		if (g_ui.pos[STOP].x <= -SCREEN_WIDTH)
		{
			g_ui.type_next = UI_FIRST_TURN;
			g_ui.type_now = g_ui.type_next;	//切り替え終わったら次のアニメーション
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
//描画
//=====================================================
void Ui_PlayerTurn_Draw(void)
{
	//操作しているストーンの要素数取得
	int stone_id = Stone_PlayerTurn();
	//デバイスのポインタ取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	int width = Texture_GetWidth(g_tex[8]) / 4;
	int height = Texture_GetHeight(g_tex[8]);

	DebugFont_Draw(0, 32 * 17, "stone_id = %d", stone_id);
	DebugFont_Draw(0, 32 * 18, "幅 = %d", width * stone_id);
	DebugFont_Draw(0, 32 * 19, "幅2 = %d", width * (stone_id + 1));

	//αテスト
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 128);

	//上の帯表示
	Sprite_Draw(g_tex[0], g_ui.pos[TOPBAND].x, g_ui.pos[TOPBAND].y);
	//スコア表示
	Score_Draw(Stone_GetScore(stone_id), 50.0f, g_ui.pos[TOPBAND].y, 2);

	//タイプ別アニメーション
	switch (g_ui.type_now)
	{
	case UI_FIRST_TURN:
		Band_Draw(g_ui.pos[BAND]);
		Sprite_Draw(g_tex[stone_id + 1], g_ui.pos[PLAYER_TURN].x, g_ui.pos[PLAYER_TURN].y);
		Sprite_Draw(g_tex[5], g_ui.pos[QUEST].x, g_ui.pos[QUEST].y);
		break;
	case UI_DIR_CHANGE:
		//ストーン切り替え
		Sprite_Draw(g_tex[g_stone_change[0]], g_ui.pos[STONE_CHANGE].x, g_ui.pos[STONE_CHANGE].y - 50.0f, width * stone_id, 0, width, height, 0.0f, 0.0f, 0.0f, 1.0f);
		Sprite_Draw(g_tex[g_stone_change[1]], g_ui.pos[STONE_CHANGE].x - 100.0f, g_ui.pos[STONE_CHANGE].y, width * stone_id, 0, width, height);
		Sprite_Draw(g_tex[g_stone_change[2]], g_ui.pos[STONE_CHANGE].x + 160.0f, g_ui.pos[STONE_CHANGE].y, width * stone_id, 0, width, height);
		//ミニマップ
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
//使用する
//=====================================================
void Ui_PlayerTurn_Create(UI_STATE type)
{
	g_ui.type_next = type;
}

//=====================================================
//使用中かどうか
//=====================================================
UI_STATE Ui_PlayerTurn_GetType(void)
{
	return g_ui.type_now;
}

//=====================================================
//使用中かどうか
//=====================================================
UI_STATE Ui_PlayerTurn_GetNextType(void)
{
	return g_ui.type_next;
}

//=====================================================
//ストーン切り替えでテクスチャ切り替え
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