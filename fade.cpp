#include "texture.h"
#include "sprite.h"
#include "fade.h"
#include "common.h"

//=====================================================
//マクロ定義
//=====================================================
#define FADE_INCRE (4)	//補正値

//=====================================================
//グローバル変数
//=====================================================
static unsigned int g_Tex;
static int g_fade = 0;
static bool g_isUse = false;
static int g_incre = FADE_INCRE;
static SCENE g_scene = SCENE_NONE;
static bool g_half = false;

//=====================================================
//初期化
//=====================================================
void Fade_init(void)
{
	g_Tex = Texture_SetLoadFile("Texture\\black.jpeg", SCREEN_WIDTH, SCREEN_HEIGHT);
	g_fade = FADE_INCRE;
	g_isUse = false;
	g_incre = FADE_INCRE;
	g_scene = SCENE_NONE;
	g_half = false;
}

//=====================================================
//終了
//=====================================================
void Fade_Uninit(void)
{

}

//=====================================================
//更新
//=====================================================
void Fade_Update(void)
{
	if (!g_isUse)
	{
		return;
	}

	//半分の明度
	if (g_half)
	{
		g_fade += FADE_INCRE;
		if (g_fade > 128 - FADE_INCRE)
		{
			g_fade = 128 - FADE_INCRE;
		}
	}
	//フェードインアウト
	else
	{
		g_fade += g_incre;
		//フェードイン
		if (g_fade > 255 - FADE_INCRE)
		{
			g_incre *= -1;
			Game_SceneChange(g_scene);
		}
		//フェードアウト
		else if (g_fade < FADE_INCRE)
		{
			g_incre *= -1;
			g_isUse = false;
		}
	}
}

//=====================================================
//描画
//=====================================================
void Fade_Draw(void)
{
	if (g_isUse)
	{
		Sprite_Draw(g_Tex, 0, 0, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, g_fade);
	}
}

//=====================================================
//使用中かどうか
//=====================================================
bool Fade_isUse(void)
{
	return g_isUse;
}

//=====================================================
//フェード生成
//=====================================================
void Fade_InOut(SCENE scene)
{
	if (g_isUse)
	{
		return;
	}

	g_fade = FADE_INCRE;
	g_isUse = true;
	g_half = false;
	g_scene = scene;
}

//=====================================================
//半分の明度でフェード
//=====================================================
void Fade_Half(void)
{
	if (g_isUse)
	{
		return;
	}

	g_fade = FADE_INCRE;
	g_isUse = true;
	g_half = true;
}

//=====================================================
//削除
//=====================================================
void Fade_Destroy(void)
{
	g_half = false;
}