//=========================================================================
//						�V���[�e�B���O�Q�[��
//
//	�o�Ȕԍ� 45
//	�R��@����
//=========================================================================

#include <windows.h>
#include <d3dx9.h>
#include <d3d9.h>
#include "common.h"
#include "mydirectx.h"
#include "texture.h"
#include "sprite.h"
#include "system_timer.h"
#include "debug_font.h"
#include "input.h"
#include "game.h"
#include "sound.h"
#include "debug.h"

//=====================================================
//�}�N����`
//=====================================================
#define CLASS_NAME		"GameWindow"			//�E�B���h�E�̃N���X��
#define WINDOW_CAPTION	"3DObject"				//�E�B���h�E�ɕ\������^�C�g��
#define FPS_MEASUREMENT_TIME (1.0)				//�v������b��
#define FPS (60)								//fps�l

//=====================================================
//�v���g�^�C�v�錾
//=====================================================
LRESULT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
static bool Init(HINSTANCE hInstance, HWND hwnd);//������
static void Uninit(void);	//�I������
static void Update(void);	//�X�V����
static void Draw(void);		//�`�揈��

//=====================================================
//�O���[�o���ϐ�
//=====================================================
static LPDIRECT3DTEXTURE9 g_pTexture[3] = { NULL };//�e�N�X�`���̎擾
static int tex[TEXTUREDATA_MAX];
static int g_FrameCount = 0;			//�t���[���J�E���^�[
static int g_FPSBaseFrameCount = 0;		//fps�v���̂��߂̕ϐ�
static double g_FPSBaseTime = 0.0;		//fps�v���̂��߂̎���
static double g_StaticFrameTime = 0.0;	//�t���[���Œ�p�v������
static float g_FPS = 0.0f;				//1�b�Ԃ̃t���[����

//====================================================
//�\���̐錾
//====================================================


//=====================================================
//���C���v���O����
//=====================================================
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	WNDCLASS wc = {};				//�\���̓��̎g���ĂȂ����̂����ׂď��������Ȃ��ƃG���[�ɂȂ�
	wc.lpfnWndProc = WndProc;		//�E�B���h�E�v���V�[�W���̓o�^
	wc.lpszClassName = CLASS_NAME;	//�N���X��
	wc.hInstance = hInstance;		//�A�v���P�[�V�����̃C���X�^���X�ԍ�������
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);	//�J�[�\���̎��
	wc.hbrBackground = (HBRUSH)(COLOR_BACKGROUND + 1);		//�E�B���h�E�̐F

	RegisterClass(&wc);		//�I���W�i���̃N���X(�E�B���h�E)��o�^

	const DWORD window_style = WS_OVERLAPPEDWINDOW ^ WS_THICKFRAME ^ WS_MAXIMIZEBOX;		//�E�B���h�E�`���̒萔��
	//WS_OVERLAPPEDWINDOW ^ WS_THICKFRAME ^ WS_MAXIMIZEBOX
	//WS_VISIBLE | WS_POPUP

	RECT window_rect = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };		//���ƍ������v�Z����

	AdjustWindowRect(&window_rect, window_style, FALSE);

	int window_width = window_rect.right - window_rect.left;
	int window_height = window_rect.bottom - window_rect.top;

	int desktop_width = GetSystemMetrics(SM_CXSCREEN);				//���j�^�[�̉��̉𑜓x
	int desktop_height = GetSystemMetrics(SM_CYSCREEN);				//���j�^�[�̏c�̉𑜓x

	int window_x = max((desktop_width - window_width) / 2, 0);		//�E�B���h�E��x��ݒ�
	int window_y = max((desktop_height - window_height) / 2, 0);	//�E�B���h�E��y��ݒ�

	HWND hwnd = CreateWindow(
		CLASS_NAME,				//�E�B���h�E�̃N���X��
		WINDOW_CAPTION,			//�E�B���h�E�ɕ\������^�C�g��
		window_style,			//�E�B���h�E�̌`���̎w��,�E�B���h�E�T�C�Y��ύX�����Ȃ�
		//WS_THICFRAME = ��ʃT�C�Y��ύX�ł��Ȃ��B	WS_MAXIMIZEBOX = �ő剻�������Ȃ��Ȃ�B
		window_x,				//x
		window_y,				//y
		window_width,			//��
		window_height,			//����
		//0, 
		//0,
		//desktop_width,
		//desktop_height,
		NULL,
		NULL,
		hInstance,
		NULL
	);

	ShowWindow(hwnd, nCmdShow);
	UpdateWindow(hwnd);			//�ǉ�

	//�������Ŏ��s�����ꍇ
	if (!Init(hInstance, hwnd))
	{
		return -1;	//WinMain�̏I��
	}

	MSG msg = {};
	while (WM_QUIT != msg.message)		//GetMessage WM_QUIT��������false
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			//�Q�[������
			double time = SystemTimer_GetTime();
			if (time - g_StaticFrameTime < 1.0 / FPS)
			{
				Sleep(0);
			}
			else
			{
				g_StaticFrameTime = time;
				Update();	//�X�V
				Draw();		//�`��
			}
		}
	}

	Uninit();	//�I������

	return (int)msg.wParam;
}

//=====================================================
//���b�Z�[�W�̎󂯎��
//=====================================================
LRESULT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	//WM = �E�B���h�E���b�Z�[�W
	switch (uMsg)
	{
	case WM_KEYDOWN:
		if (wParam == VK_ESCAPE)	//wParam�ɂ͂Ȃ�̃L�[�������ꂽ��������B
		{
			SendMessage(hwnd, WM_CLOSE, 0, 0);	//�E�B���h�E�ɑ΂��ă��b�Z�[�W�𑗂�B
		}
		break;

	case WM_CLOSE:
		if (MessageBox(hwnd, "�{���ɏI�����Ă���낵���ł����H", "�m�F", MB_OKCANCEL | MB_DEFBUTTON2) == IDOK)
		{
			DestroyWindow(hwnd);	//�E�B���h�E�̔j�������WM_DESTROY��������B
		}
		return 0;

	case WM_DESTROY:
		PostQuitMessage(0);			//�I��
		return 0;
	};

	return DefWindowProc(hwnd, uMsg, wParam, lParam);	//��������������ƃf�t�H���g��Ƃ��s��
}

//=====================================================
//����������
//=====================================================
bool Init(HINSTANCE hInstance, HWND hwnd)
{
	//�������Ɏ��s
	if (!MyDirectX_Init(hwnd))
	{
		return false;
	}

	//fps�\���p�t�H���g�̏�����
	DebugFont_Initialize();

	// �L�[�{�[�h���̓��W���[���̏�����
	Keyboard_Initialize(hInstance, hwnd);

	//�Q�[�����[�`���p������
	Game_Init(hwnd);

	//�V�X�e���^�C�}�[�̏�����
	SystemTimer_Initialize();

	//�V�X�e���^�C�}�[�̋N��
	SystemTimer_Start();

	//�ϐ�������
	g_FrameCount = 0;
	g_FPSBaseFrameCount = 0;
	g_FPS = 0.0f;
	g_FPSBaseTime = SystemTimer_GetTime();
	g_StaticFrameTime = g_FPSBaseTime;

	return true;
}

//=====================================================
//�I������
//=====================================================
void Uninit(void)
{
	MyDirectX_Uninit();
	DebugFont_Finalize();
	Texture_Destroy();
	Keyboard_Finalize();
	Game_Uninit();
	Debug_Uninit();
}


//=====================================================
//�X�V����
//=====================================================
void Update(void)
{
	//�t���[���J�E���g�̍X�V
	g_FrameCount++;
	//���݂̃V�X�e�����Ԃ̎擾
	double time = SystemTimer_GetTime();
	//�O���FPS�v�����Ԃ���K�莞�Ԍo���Ă�����FPS�v��
	if (time - g_FPSBaseTime >= FPS_MEASUREMENT_TIME)	//���̎��Ԃ���1�O�̎��Ԃ��}�C�i�X
	{
		//FPS�̌v�Z(�O�񂩂�̌o�߃t���[�� / �o�ߎ���)
		g_FPS = (float)(g_FrameCount - g_FPSBaseFrameCount / (time - g_FPSBaseTime));
		//FPS�v���̊�ɂȂ�ϐ����X�V
		g_FPSBaseFrameCount = g_FrameCount;
		g_FPSBaseTime = time;
	}
	Game_Update();
}

//=====================================================
//�`�揈��
//=====================================================
void Draw(void)
{
	//�f�o�C�X�̒l�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);			//�A���t�@�u�����h��L����
																	//D3DRS_ALPHABLENDENABLE�ŗL��,TRUE�ŗL����
	pDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);	//���{�摜��菬�����摜�̃s�N�Z�������킹��
	pDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);	//�ł����摜�̃s�N�Z�������킹��
	pDevice->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);	//2�ׂ̂���̃T�C�Y�̉摜��p�ӂ��čœK�ȃT�C�Y�ɐ؂�ւ���
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);//D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA
	pDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);	//MODULATE�|���Z
	//�|���S���̃��l�𖳎����Ȃ��ݒ�

	//��ʃN���A
	pDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_RGBA(0, 0, 255, 0), 1.0f, 0);//, �S�̂��N���A����ꍇ��NULL�Brect�^�̔z��̗v�f��������Ƃ��̌`�ɂ��蔲����, TARGET(�F)ZBUFFER(�[�x)�̃N���A, 
	//��O����������ΐF�̎w��, ZBUFFER�������0����O�A1����(��Ԃ�����ŃN���A),0��ok
	pDevice->BeginScene();						//�V�[���̎n�܂�
	Debug_Begin();
	Game_Draw();
#if (DEBUG) | (_DEBUG)
	DebugFont_Draw(SCREEN_WIDTH - 75, 30, "%.01lf", g_FPS);	//fps�\��
#endif
	Debug_End();
	pDevice->EndScene();						//�V�[���̏I��� Beginscene�Ƒ�
	pDevice->Present(NULL, NULL, NULL, NULL);	//�V�[���̓n��(1F/1�񂱂��ȊO�ŏ����Ȃ�)
}