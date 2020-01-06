#include "texture.h"
#include "sprite.h"
#include "common.h"
#include "fade.h"
#include "input.h"

//=====================================================
//グローバル変数
//=====================================================
static unsigned int g_tex;

//=====================================================
//初期化
//=====================================================
void Tutorial_Init(void)
{
	g_tex = Texture_SetLoadFile("Texture\\tutorial.png", 1280, 720);
}

//=====================================================
//終了
//=====================================================
void Tutorial_Uninit(void)
{

}

//=====================================================
//更新
//=====================================================
void Tutorial_Update(void)
{
	if (Keyboard_IsTrigger(DIK_SPACE))
	{
		Fade_InOut(SCENE_CAMERA);
	}
}

//=====================================================
//描画
//=====================================================
void Tutorial_Draw(void)
{
	Sprite_Draw(g_tex, 0, 0);
}