#include "sprite.h"
#include "texture.h"

static unsigned int g_tex;

//=====================================================
//初期化
//=====================================================
void Number_Init(void)
{
	g_tex = Texture_SetLoadFile("Texture\\number.png", 1280 / 2, 256 / 2);
}

//=====================================================
//描画
//=====================================================
void Number_Draw(int number, float x, float y)
{
	int width = Texture_GetWidth(g_tex) / 10;
	int height = Texture_GetHeight(g_tex) / 2;

	if (number < 0 || number > 9)
	{
		return;
	}
	Sprite_Draw(g_tex, x, y, number * width, 0, width, height);
}

//=====================================================
//描画(サイズ調整)
//=====================================================
void Number_Draw(int number, float x, float y, float size)
{
	int width = Texture_GetWidth(g_tex) / 10;
	int height = Texture_GetHeight(g_tex) / 2;

	if (number < 0 || number > 9)
	{
		return;
	}
	Sprite_Draw(g_tex, x, y, number * width, 0, width, height, 0, 0, 0, size);
}

//=====================================================
//描画(マイナス)
//=====================================================
void Number_Draw(int x, int y)
{
	int width = Texture_GetWidth(g_tex) / 10;
	int height = Texture_GetHeight(g_tex) / 2;

	Sprite_Draw(g_tex, x, y, 0, height, width, height);
}