#include "texture.h"
#include "number.h"
#include "score.h"
#include "sound.h"
#include "debug.h"
#include "debug_font.h"
#include "game.h"
#include "fade.h"
#include "Efect.h"
#include "cube.h"
#include "grid.h"
#include "camera.h"
#include "mouse.h"

//=====================================================
//グローバル変数
//=====================================================
static SCENE g_Scene = SCENE_NONE;
static bool g_TexLoad = true;
static HWND g_hwnd = NULL;
//カメラ行列設定
static D3DXVECTOR3 g_CameraRotate = D3DXVECTOR3(-45.0f, 0.0f, 0.0f);		//45, 0, 0
static D3DXVECTOR3 g_CameraPosition = D3DXVECTOR3(0.0f, 0.0f, -12.0f);
static D3DXVECTOR3 g_CameraAt = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

//=====================================================
//初期化
//=====================================================
void Game_Init(HWND hwnd)
{
	g_Scene = SCENE_3D;
	g_hwnd = hwnd;
	Number_Init();
	Score_Init();
	Debug_Init();
	Fade_init();
	EfectInit();
	Cube_Init();
	Grid_Init();
	Mouse_Init();
	Camera_Init();
	if (g_TexLoad)
	{
		if (Texture_Load() > 0)
		{
			MessageBox(hwnd, "テクスチャが読み込めませんでした", "エラー", MB_OK);
			exit(0);
		}
		g_TexLoad = false;
	}
}

//=====================================================
//終了
//=====================================================
void Game_Uninit(void)
{
	Debug_Uninit();
	Fade_Uninit();
	EfectUninit();
	Grid_Uninit();
	Mouse_Uninit();
}

//=====================================================
//更新
//=====================================================
void Game_Update(void)
{
	switch (g_Scene)
	{
	case SCENE_3D:
		//カメラ設定
		Camera_Set(g_CameraRotate, g_CameraPosition, g_CameraAt);
		//カメラ入力操作
		g_CameraRotate = Camera_Input_Rot(g_CameraRotate);
		g_CameraPosition = Camera_Input_Pos(g_CameraPosition);
		g_CameraAt = Camera_Input_At(g_CameraAt);
		Mouse_Update();
		break;
	default:
		break;
	}
}

//=====================================================
//描画
//=====================================================
void Game_Draw(void)
{
	switch (g_Scene)
	{
	case SCENE_3D:
		//カメラ情報出力
		Camera_Debug_Info(g_CameraPosition, g_CameraRotate, g_CameraAt);
		//Cube_Dice();
		//Cube_jyugyou();
		Grid_Draw();
		Mouse_Draw();
		break;
	}
}

//=====================================================
//シーンの切り替え
//=====================================================
void Game_SceneChange(SCENE scene)
{
	g_Scene = scene;
}

//=====================================================
//シーンの取得
//=====================================================
SCENE Game_GetScene(void)
{
	return g_Scene;
}

//=====================================================
//ウィンドウハンドルの取得
//=====================================================
HWND Game_GetHwnd(void)
{
	return g_hwnd;
}