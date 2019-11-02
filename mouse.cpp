#include "debug_font.h"
#include <windows.h>
#include "d3dx9.h"
#include "input.h"

//=====================================================
//マクロ定義
//=====================================================
#define MOUSE_MAX_HEIGHT (767)	//マウス移動上限(高さ:y軸)

//=====================================================
//構造体宣言
//=====================================================
typedef struct
{
	D3DXVECTOR2 pos;
	BOOL cursor;
}MOUSE;

//=====================================================
//グローバル変数
//=====================================================
MOUSE g_Mouse;
POINT pt;
static int g_LoopCount = 0;
static char g_DebugString[64] = {"表示"};

//=====================================================
//初期化
//=====================================================
void Mouse_Init(void)
{
	g_Mouse.pos = D3DXVECTOR2(0.0f, 0.0f);
	g_Mouse.cursor = TRUE;
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
	//キーボード更新
	Keyboard_Update();

	//マウスカーソル切り替え
	ShowCursor(g_Mouse.cursor);
	if (Keyboard_IsTrigger(DIK_M))
	{
		if (g_Mouse.cursor)
		{
			strcpy(g_DebugString, "表示");
		}
		else
		{
			strcpy(g_DebugString, "非表示");
		}
		(g_Mouse.cursor) ? g_Mouse.cursor = FALSE : g_Mouse.cursor = TRUE;
	}

	//マウスの座標取得
	GetCursorPos(&pt);

	//ゲーム内マウスパラメータ
	g_Mouse.pos.x = (float)pt.x;
	g_Mouse.pos.y = MOUSE_MAX_HEIGHT + (float)pt.y * -1 + (MOUSE_MAX_HEIGHT * g_LoopCount);

	//マウス上ループ
	if (g_Mouse.pos.y >= MOUSE_MAX_HEIGHT * (g_LoopCount + 1))
	{
		SetCursorPos((int)pt.x, MOUSE_MAX_HEIGHT);
		g_LoopCount++;
	}
	//マウス下ループ
	else if ((g_Mouse.pos.y - MOUSE_MAX_HEIGHT * g_LoopCount <= 0.0f) && (g_Mouse.pos.y > 0.0f))
	{
		SetCursorPos((int)pt.x, 0);
		g_LoopCount--;
	}
	
}

//=====================================================
//描画
//=====================================================
void Mouse_Draw(void)
{
	//マウスの座標表示
	DebugFont_Draw(0, 32 * 0, "Mキー:カーソル%s", g_DebugString);
	DebugFont_Draw(0, 32 * 1, "x:%.02lf", g_Mouse.pos.x);
	DebugFont_Draw(0, 32 * 2, "y:%.02lf(反転)", g_Mouse.pos.y);
}

//=====================================================
//x軸座標取得
//=====================================================
int Mouse_GetPos_X(void)
{
	return (int)pt.x;
}

//=====================================================
//y軸座標取得
//=====================================================
int Mouse_GetPos_Y(void)
{
	return (int)pt.y;
}