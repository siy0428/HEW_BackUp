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

//=====================================================
//�O���[�o���ϐ�
//=====================================================
static SCENE g_Scene = SCENE_TITLE;
static bool g_TexLoad = true;
static HWND g_hwnd = NULL;
//�J�����s��ݒ�
static D3DXVECTOR3 g_CameraRotate = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//45, 0, 0
static D3DXVECTOR3 g_CameraPosition = D3DXVECTOR3(0.0f, 10.0f, -10.0f);
static D3DXVECTOR3 g_CameraAt = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

//=====================================================
//������
//=====================================================
void Game_Init(HWND hwnd)
{
	g_hwnd = hwnd;
	Goal_Init();	//Stone����ɌĂ΂Ȃ��Ə���̋��������m�ł͂Ȃ�
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
	Anime_Test_Init();
	Ui_PlayerTurn_Init();
	Title_Init();
	Band_Init();
	if (g_TexLoad)
	{
		if (Texture_Load() > 0)
		{
			MessageBox(hwnd, "�e�N�X�`�����ǂݍ��߂܂���ł���", "�G���[", MB_OK);
			exit(0);
		}
		g_TexLoad = false;
	}
}

//=====================================================
//�I��
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
	Anime_Test_Uninit();
	Texture_Destroy();
	Ui_PlayerTurn_Uninit();
}

//=====================================================
//�X�V
//=====================================================
void Game_Update(void)
{
	//�L�[�{�[�h�X�V
	Keyboard_Update();
	//�W���C�R���X�V
	UpdateInput();

	Fade_Update();

	switch (g_Scene)
	{
	case SCENE_TITLE:
		Title_Update();
		break;
	case SCENE_TUTORIAL:
		break;
	case SCENE_MAIN:
		Mouse_Update();
		Stone_Update();
		Camera_Change();
		Goal_Update();
		GuideLine_Update();
		Ui_PlayerTurn_Update();
		break;
	case SCENE_RESULT:
		Result_Update();
		break;
	default:
		break;
	}
}

//=====================================================
//�`��
//=====================================================
void Game_Draw(void)
{
	switch (g_Scene)
	{
	case SCENE_TITLE:
		Title_Draw();
		break;
	case SCENE_TUTORIAL:
		break;
	case SCENE_MAIN:
		//Grid_Draw();
		Mouse_Draw();
		Stone_Draw();
		Goal_Draw();
		//Pow_Gauge_Draw();
		Mesh_Draw();
		GuideLine_Draw();
		Anime_Test_Draw();
		Ui_PlayerTurn_Draw();
		break;
	case SCENE_RESULT:
		Result_Draw();
		break;
	}
	Fade_Draw();
}

//=====================================================
//�V�[���̐؂�ւ�
//=====================================================
void Game_SceneChange(SCENE scene)
{
	g_Scene = scene;
}

//=====================================================
//�V�[���̎擾
//=====================================================
SCENE Game_GetScene(void)
{
	return g_Scene;
}

//=====================================================
//�E�B���h�E�n���h���̎擾
//=====================================================
HWND Game_GetHwnd(void)
{
	return g_hwnd;
}