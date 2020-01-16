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
#include "result.h"
#include "light.h"
#include "mesh.h"
#include "guide_line.h"
#include "penguin.h"
#include "OXAllocateHierarchy.h"
#include "anime_test.h"
#include "ui_player_turn.h"
#include "title.h"
#include "common.h"
#include "band.h"
#include "tutorial.h"
#include "course_camera.h"
#include "white_bear.h"
#include "turara.h"
#include "save_load.h"
#include "stage.h"
#include "joycon.h"
#include "quest.h"

//=====================================================
//グローバル変数
//=====================================================
//static SCENE g_Scene = SCENE_TITLE;
static SCENE g_Scene = SCENE_MAIN;
static bool g_TexLoad = true;
static HWND g_hwnd = NULL;

//=====================================================
//初期化
//=====================================================
void Game_Init(HWND hwnd)
{
	g_hwnd = hwnd;
	Goal_Init();
	Debug_Init();
	Fade_init();
	EfectInit();
	Cube_Init();
	//Grid_Init(10);
	Mouse_Init();
	Camera_Init();
	dCamera_Init();
	Stone_Init();
	Pow_Gauge_Init();
	InitDirectInput(hwnd);
	Result_Init();
	Light_Set();
	Score_Init();
	Mesh_Init();
	GuideLine_Init();
	Ui_PlayerTurn_Init();
	Title_Init();
	Band_Init();
	Tutorial_Init();
	Penguin_Init();
	WhiteBear_Init();
	Turara_Init();
	LoadStageDate();
	Stage_Init();
	Course_Camera_Init();
	Quest_Init();
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
	Result_Uninit();
	Mesh_Uninit();
	GuideLine_Uninit();
	Texture_Destroy();
	Ui_PlayerTurn_Uninit();
	Tutorial_Uninit();
	Course_Camera_Uninit();
	ModelAnime_Uninit();
}

//=====================================================
//更新
//=====================================================
void Game_Update(void)
{
	//キーボード更新
	Keyboard_Update();
	//ジョイコン更新
	UpdateInput();
	//フェード更新
	Fade_Update();

	switch (g_Scene)
	{
	case SCENE_TITLE:
		Title_Update();
		break;
	case SCENE_TUTORIAL:
		Tutorial_Update();
		break;
	case SCENE_CAMERA:
		Course_Camera_Update();
		Penguin_Update();
		WhiteBear_Update();
		Goal_Update();
		Stone_Update();
		break;
	case SCENE_MAIN:
		Mouse_Update();
		Stone_Update();
		Camera_Change();
		Goal_Update();
		GuideLine_Update();
		Ui_PlayerTurn_Update();
		Penguin_Update();
		WhiteBear_Update();
		Turara_Update();
		Quest_Update();
		break;
	case SCENE_RESULT:
		Result_Update();
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
	case SCENE_TITLE:
		Title_Draw();
		break;
	case SCENE_TUTORIAL:
		Tutorial_Draw();
		break;
	case SCENE_CAMERA:
		Course_Camera_Draw();
		WhiteBear_Draw();
		Turara_Draw();
		Penguin_Draw();
		Stone_Draw();
		Stage_Draw();
		Goal_Draw();
		break;
	case SCENE_MAIN:
		//Grid_Draw();
		Mouse_Draw();
		Stone_Draw();
		Goal_Draw();
		Pow_Gauge_Draw();
		Mesh_Draw();
		GuideLine_Draw();
		Ui_PlayerTurn_Draw();
		Penguin_Draw();
		WhiteBear_Draw();
		Turara_Draw();
		Stage_Draw();
		break;
	case SCENE_RESULT:
		Result_Draw();
		break;
	}
	Fade_Draw();
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