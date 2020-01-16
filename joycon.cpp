#include "input.h"
#include "dinput.h"
#include "JoyInput.h"
#include "debug_font.h"

//=====================================================
//マクロ定義
//=====================================================
#define STICK_SPEED (1.0f)
#define STICK_DEFAULT_STICK (0.0f)

//=====================================================
//グローバル変数
//=====================================================

//=====================================================
//初期化
//=====================================================
void Joycon_Init(void)
{
	
}

//=====================================================
//ジョイコン操作
//=====================================================
float Joycon_Operator(void)
{
	//スティック操作
	float stick_y;
	stick_y = GetStick().x / 500;

	//デバッグ用回転
#if (DEBUG) || (_DEBUG)
	if (Keyboard_IsPress(DIK_LEFTARROW))
	{
		stick_y = -STICK_SPEED;
	}
	else if (Keyboard_IsPress(DIK_RIGHTARROW))
	{
		stick_y = STICK_SPEED;
	}
#endif	(DEBUG) || (_DEBUG)

	return stick_y;
}