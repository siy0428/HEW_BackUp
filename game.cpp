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
#include "debug_camera.h"
#include "camera_generator.h"
#include "mouse.h"
#include "stone.h"
#include "input.h"
#include "goal.h"
#include "power_gauge.h"
#include "JoyInput.h"
#include "number.h"
#include "sprite.h"

//=====================================================
//グローバル変数
//=====================================================
static SCENE g_Scene = SCENE_NONE;
static bool g_TexLoad = true;
static HWND g_hwnd = NULL;
//カメラ行列設定
static D3DXVECTOR3 g_CameraRotate = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//45, 0, 0
static D3DXVECTOR3 g_CameraPosition = D3DXVECTOR3(0.0f, 10.0f, -10.0f);
static D3DXVECTOR3 g_CameraAt = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

//=====================================================
//初期化
//=====================================================
void Game_Init(HWND hwnd)
{
	g_Scene = SCENE_3D;
	g_hwnd = hwnd;
	Goal_Init();	//Stoneより先に呼ばないと初回の距離が正確ではない
	Score_Init();
	Debug_Init();
	Fade_init();
	EfectInit();
	Cube_Init();
	Grid_Init(30);
	Mouse_Init();
	Camera_Init();
	dCamera_Init();
	Stone_Init();
	Pow_Gauge_Init();
	InitDirectInput(hwnd);
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
	Stone_Uninit();
	Goal_Uninit();
	Pow_Gauge_Uninit();
	UninitDirectInput();
}

//=====================================================
//更新
//=====================================================
void Game_Update(void)
{
	//キーボード更新
	Keyboard_Update();

	switch (g_Scene)
	{
	case SCENE_3D:
		Mouse_Update();
		Stone_Update();
		Camera_Change();
		Goal_Update();
		UpdateInput();
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
		Grid_Draw();
		Mouse_Draw();
		Stone_Draw();
		Goal_Draw();
		Pow_Gauge_Draw();
		//ジョイコンデバッグ
		D3DXVECTOR3 gyro = GetGyro();
		D3DXVECTOR2 stick = GetStick();
		DebugFont_Draw(0, 32 * 10, "gyro.x = %.02lf", gyro.x);
		DebugFont_Draw(0, 32 * 11, "gyro.y = %.02lf", gyro.y);
		DebugFont_Draw(0, 32 * 12, "gyro.z = %.02lf", gyro.z);
		DebugFont_Draw(0, 32 * 13, "stick.x = %.02lf", stick.x);
		DebugFont_Draw(0, 32 * 14, "stick.y = %.02lf", stick.y);
		DebugFont_Draw(0, 32 * 15, "ボタン = %d", GetButton(JC_X));
		break;
	}
}
//カメラ情報出力
//Camera_Debug_Info(g_CameraPosition, g_CameraRotate, g_CameraAt);

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