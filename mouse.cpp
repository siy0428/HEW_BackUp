#include "debug_font.h"
#include <windows.h>
#include "d3dx9.h"
#include "input.h"
#include "xinput.h"
#include "mouse.h"

//=====================================================
//マクロ定義
//=====================================================
#define MOUSE_MAX_HEIGHT (767)	//マウス移動上限(高さ:y軸)
#define MOUSE_SAVE_FRAME (10)	//マウスの座標の記憶フレーム数
#define MOUSE_MAX_FORCE (250 * MOUSE_SAVE_FRAME)	//最大パワー

//=====================================================
//構造体宣言
//=====================================================
typedef struct
{
	int pos_x;
	int pos_y;
	BOOL cursor;
}MOUSE;

//=====================================================
//グローバル変数
//=====================================================
MOUSE g_mouse;
POINT pt;
static int g_loop_count = 0;
static int g_mouse_save[MOUSE_SAVE_FRAME] = { 0 };
static int g_index = 0;

//=====================================================
//初期化
//=====================================================
void Mouse_Init(void)
{
	g_mouse.pos_x = 0;
	g_mouse.pos_y = 0;
	g_mouse.cursor = TRUE;
}

//=====================================================
//終了
//=====================================================
void Mouse_Uninit(void)
{

}

//=====================================================
//更新
//=====================================================
void Mouse_Update(void)
{
	//マウスの座標取得
	GetCursorPos(&pt);

	//ゲーム内マウスパラメータ
	g_mouse.pos_x = pt.x;
	g_mouse.pos_y = MOUSE_MAX_HEIGHT - pt.y + (MOUSE_MAX_HEIGHT * g_loop_count);	//上下反転

	//マウス上ループ
	if (g_mouse.pos_y >= MOUSE_MAX_HEIGHT * (g_loop_count + 1))
	{
		SetCursorPos(pt.x, MOUSE_MAX_HEIGHT);	//画面下に持っていく
		g_loop_count++;	//画面内ループ回数
	}
	//マウス下ループ
	else if ((g_mouse.pos_y - MOUSE_MAX_HEIGHT * g_loop_count <= 0.0f) && (g_mouse.pos_y > 0.0f))
	{
		SetCursorPos(pt.x, 0);	//画面上に持っていく
		g_loop_count--;	//画面内ループ回数
	}

	//マウスパラメータリセット
	if (Keyboard_IsTrigger(DIK_R))
	{
		g_mouse.pos_y = 0;
		g_loop_count = 0;
	}

	//マウスの座標を10F間記憶
	if (g_index < MOUSE_SAVE_FRAME)
	{
		g_mouse_save[MOUSE_SAVE_FRAME - 1 - g_index] = g_mouse.pos_y;
		g_index++;
	}
	//10F記憶したら後ろから整列
	else
	{
		for (int i = MOUSE_SAVE_FRAME - 1; i > 0; i--)
		{
			g_mouse_save[i] = g_mouse_save[i - 1];
		}
		g_mouse_save[0] = g_mouse.pos_y;
	}
}

//=====================================================
//描画
//=====================================================
void Mouse_Draw(void)
{
	//マウスの座標表示
	DebugFont_Draw(0, 32 * 0, "force = %d", Mouse_GetForce());
	DebugFont_Draw(0, 32 * 1, "x:%d", g_mouse.pos_x);
	DebugFont_Draw(0, 32 * 2, "y:%d(反転)", g_mouse.pos_y);

}

//=====================================================
//マウスのx軸座標取得
//=====================================================
int Mouse_GetPos_X(void)
{
	return pt.x;
}

//=====================================================
//マウスのy軸座標取得
//=====================================================
int Mouse_GetPos_Y(void)
{
	return pt.y;
}

//=====================================================
//マウスのスクロールで0～100の値を返す
//=====================================================
int Mouse_GetForce(void)
{
	//今の座標から10F前の座標を引いてパワー算出
	int force = g_mouse_save[0] - g_mouse_save[MOUSE_SAVE_FRAME - 1];

	//下限設定
	if (force < 0)
	{
		force = 0;
	}

	//上限設定
	else if (force >= MOUSE_MAX_FORCE)
	{
		force = MOUSE_MAX_FORCE;
	}

	//0～100に設定
	force = 100 * force / MOUSE_MAX_FORCE;

	return force;
}

//=====================================================
//マウスパラメータリセット
//=====================================================
void Mouse_Reset(void)
{
	for (int i = 0; i < MOUSE_SAVE_FRAME - 1; i++)
	{
		g_mouse_save[i] = 0;
	}
	g_index = 0;
}