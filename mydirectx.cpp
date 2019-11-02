//=======================================================================
//							��{�ݒ�
//
//	�o�Ȕԍ� 45	
//	�R�� ����
//=======================================================================

#include <d3d9.h>
#include <windows.h>
#include "common.h"

//=====================================================
//�O���[�o���ϐ�
//=====================================================
static LPDIRECT3D9 g_pD3D = NULL;			//�C���^�t�F�[�X�̎擾
static LPDIRECT3DDEVICE9 g_pDevice = NULL;	//�f�o�C�X�̎擾

//=====================================================
//������
//=====================================================
bool MyDirectX_Init(HWND hwnd)
{
	g_pD3D = Direct3DCreate9(D3D_SDK_VERSION);	//�_�C���N�g3d�̃C���^�t�F�[�X���擾

	//�C���^�t�F�[�X�̎擾�Ɏ��s�����ꍇ�̃G���[����
	if (g_pD3D == NULL)
	{
		MessageBox(hwnd, "�C���^�t�F�[�X�̎擾�Ɏ��s���܂���", "���s", MB_OK);
		return false;
	}

	D3DPRESENT_PARAMETERS d3dpp = {};			//�f�o�C�X���̏�����
	d3dpp.BackBufferWidth = SCREEN_WIDTH;		//���o�b�t�@�̕�
	d3dpp.BackBufferHeight = SCREEN_HEIGHT;		//���o�b�t�@�̍���
	d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;	//os�ɍ��킹����ʃT�C�Y
	d3dpp.BackBufferCount = 1;					//�o�b�t�@�̐�
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;	//�\�Ɨ��̓���ւ�
	d3dpp.Windowed = TRUE;						//�E�B���h�E���[�h�ō��ꍇ��true,�t���X�N���[����false
	d3dpp.EnableAutoDepthStencil = TRUE;		//�[�x�o�b�t�@�A�X�e���V���o�b�t�@��L���ɂ���
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;	//�[�x�o�b�t�@�̐ݒ��������t�H�[�}�b�g�̎w��(Depth16bit)
	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;	//�t���X�N���[���̂ݓK��(��ʂ��L�ł��邽��)
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;	//��ʂ̃��t���b�V�����[�g�ɍ��킹��GPU�ɃA�N�Z�X
																//D3DPRESENT_INTERVAL_IMMEDIATE			D3DPRESENT_INTERVAL_DEFAULT
																//��ʂ̐����������܂����ɂ����`��

	HRESULT hr = g_pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hwnd, D3DCREATE_HARDWARE_VERTEXPROCESSING, &d3dpp, &g_pDevice);
	//�ǂ̃A�_�v�^�����w��(���j�^�[�������̏ꍇ�͕ύX), �n�[�h�E�F�A�œ������̂��ǂ���, 
	//�Ώۂ̃E�B���h�E�n���h��, ���_�̉��Z���n�[�h�E�F�A���\�t�g�E�F�A, �f�o�C�X�̏����̃A�h���X, �����������̃A�h���X�̓����

	//�f�o�C�X���擾�ł��Ȃ������ꍇ�̃G���[����
	if (FAILED(hr))
	{
		MessageBox(hwnd, "�f�o�C�X�̎擾�Ɏ��s���܂���", "���s", MB_OK);
		return false;
	}

	return true;
}

//=====================================================
//������
//=====================================================
void MyDirectX_Uninit(void)
{
	if (g_pDevice != NULL)
	{
		g_pDevice->Release();
		g_pDevice = NULL;
	}
	if (g_pD3D != NULL)
	{
		g_pD3D->Release();
		g_pD3D = NULL;
	}
}

//=====================================================
//�f�o�C�X�̒l�擾
//=====================================================
LPDIRECT3DDEVICE9 GetDevice(void)
{
	return g_pDevice;
}