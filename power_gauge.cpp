#include "sprite.h"
#include "texture.h"
#include "mouse.h"
#include "common.h"

//=====================================================
//グローバル変数
//=====================================================
static unsigned int g_tex[4] = {};
static int g_gauge;

//=====================================================
//初期化
//=====================================================
void Pow_Gauge_Init(void)
{
	g_tex[0] = Texture_SetLoadFile("Texture\\gauge.png", 128 * SCREEN_WIDTH / 1280, 512 * SCREEN_HEIGHT / 720);		//ゲージ
	g_tex[1] = Texture_SetLoadFile("Texture\\gauge_frame.png", 128 * SCREEN_WIDTH / 1280, 512 * SCREEN_HEIGHT / 720);		//ゲージ枠
	g_gauge = 0;
}

//=====================================================
//終了
//=====================================================
void Pow_Gauge_Uninit(void)
{

}

//=====================================================
//更新
//=====================================================
void Pow_Gauge_Update(void)
{
	//マウスの値取得
	g_gauge = Mouse_GetForce();
}

//=====================================================
//描画
//=====================================================
void Pow_Gauge_Draw(void)
{
	int width = Texture_GetWidth(g_tex[0]);
	int height = Texture_GetHeight(g_tex[0]);

	Sprite_Draw(NULL, SCREEN_WIDTH / 25, SCREEN_HEIGHT / 6, 0, 0, width, height, D3DCOLOR_RGBA(0, 0, 0, 255));
	Sprite_Draw(g_tex[0], SCREEN_WIDTH / 25, (SCREEN_HEIGHT / 6 + height) - height * g_gauge / 100, width, height * g_gauge / 100, 0, 0, width, height);
	Sprite_Draw(g_tex[1], SCREEN_WIDTH / 25, SCREEN_HEIGHT / 6);
}