#include "sprite.h"
#include "texture.h"

static unsigned int g_Tex;

//=====================================================
//初期化
//=====================================================
void Number_Init(void)
{
	g_Tex = Texture_SetLoadFile("Texture\\number256.tga", 256 * 2, 256 * 2);
}

//=====================================================
//描画
//=====================================================
void Number_Draw(int number, float x, float y)
{
	if (number < 0 || number > 9)
	{
		return;
	}
	Sprite_Draw(g_Tex, x, y, number * Texture_GetWidth(g_Tex) / 16, Texture_GetHeight(g_Tex), Texture_GetWidth(g_Tex) / 16, Texture_GetHeight(g_Tex));
}

//=====================================================
//描画(サイズ指定)
//=====================================================
void Number_Draw(int number, float x, float y, int cx, int cy, int cw, int ch)
{
	if (number < 0 || number > 9)
	{
		return;
	}
	Sprite_Draw(g_Tex, x, y, cx, cy, cw, ch);
}