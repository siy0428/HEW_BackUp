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
//�O���[�o���ϐ�
//=====================================================
static SCENE g_Scene = SCENE_NONE;
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
	g_Scene = SCENE_3D;
	g_hwnd = hwnd;
	Goal_Init();	//Stone����ɌĂ΂Ȃ��Ə���̋��������m�ł͂Ȃ�
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
}

//=====================================================
//�X�V
//=====================================================
void Game_Update(void)
{
	//�L�[�{�[�h�X�V
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
//�`��
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
		//�W���C�R���f�o�b�O
		D3DXVECTOR3 gyro = GetGyro();
		D3DXVECTOR2 stick = GetStick();
		DebugFont_Draw(0, 32 * 10, "gyro.x = %.02lf", gyro.x);
		DebugFont_Draw(0, 32 * 11, "gyro.y = %.02lf", gyro.y);
		DebugFont_Draw(0, 32 * 12, "gyro.z = %.02lf", gyro.z);
		DebugFont_Draw(0, 32 * 13, "stick.x = %.02lf", stick.x);
		DebugFont_Draw(0, 32 * 14, "stick.y = %.02lf", stick.y);
		DebugFont_Draw(0, 32 * 15, "�{�^�� = %d", GetButton(JC_X));
		break;
	}
}
//�J�������o��
//Camera_Debug_Info(g_CameraPosition, g_CameraRotate, g_CameraAt);

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