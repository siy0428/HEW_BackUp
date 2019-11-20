#include "sprite.h"
#include "texture.h"
#include "mouse.h"

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
	g_tex[0] = Texture_SetLoadFile("Texture\\HP_Gauge_bg.png", 256, 32);		//黒背景
	g_tex[1] = Texture_SetLoadFile("Texture\\HP_Gauge_01_green.png", 256, 32);	//ゲージ
	g_tex[2] = Texture_SetLoadFile("Texture\\HP_Gauge_01_frame.png", 256, 32);	//ゲージ枠
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
	int width = Texture_GetWidth(g_tex[1]);
	int height = Texture_GetHeight(g_tex[1]);

	Sprite_Draw(g_tex[0], 640.0f, 10.0f);
	Sprite_Draw(g_tex[1], 640.0f, 10.0f, 0, 0, width * g_gauge / 100, height);
	Sprite_Draw(g_tex[2], 640.0f, 10.0f);
}