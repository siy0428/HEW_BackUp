#include "input.h"
#include "dinput.h"
#include "JoyInput.h"

//=====================================================
//グローバル変数
//=====================================================
static float g_stick_y = 0;

//=====================================================
//初期化
//=====================================================
void Joycon_Init(void)
{
	g_stick_y = 0.0f;
}

//=====================================================
//ジョイコン操作
//=====================================================
float Joycon_Operator(void)
{
	//スティック操作
	g_stick_y += GetStick().x / 1000;

	//デバッグ用回転
#if (DEBUG) || (_DEBUG)
	if (Keyboard_IsPress(DIK_LEFTARROW))
	{
		g_stick_y -= 1.1f;
	}
	else if (Keyboard_IsPress(DIK_RIGHTARROW))
	{
		g_stick_y += 1.1f;
	}
#endif	(DEBUG) || (_DEBUG)

	return g_stick_y;
}

//=====================================================
//回転量リセット
//=====================================================
void Joycon_Reset(void)
{
	g_stick_y = 0.0f;
}