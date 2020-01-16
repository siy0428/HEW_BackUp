#include "common.h"
#include "debug_font.h"
#include "input.h"
#include "joycon.h"
#include "mouse.h"
#include "camera.h"
#include "stone.h"
#include "sprite3d.h"
#include "debug_camera.h"
#include "game.h"
#include "course_camera.h"
#include "mydirectx.h"

//====================================================
//�}�N����`
//====================================================
#define GOAL_HEIGHT (3.0f)	//�S�[���̉�����
#define DEAD_ZONE (0.001f)	//�����_�덷�p
#define GOAL_RANGE (1.0f)	//�S�[������p�}�N����`

//====================================================
//�\���̐錾
//====================================================

//=====================================================
//�O���[�o���ϐ�
//=====================================================
static D3DXVECTOR3 g_pos(0.0f, 0.5f, 0.0f);
static D3DXMATRIX g_mtxWorld;
//static unsigned int g_tex[PLAYER_MAX_NUM] = { 0 };
static unsigned int g_tex;
static int g_win_player = -1;
static bool g_goal_flag;

//=====================================================
//������
//=====================================================
void Goal_Init(void)
{
	g_pos = D3DXVECTOR3(0.0f, 0.5f, 30.0f);
	//g_tex[0] = Texture_SetLoadFile("Texture\\1p_goal.png", SCREEN_WIDTH, SCREEN_HEIGHT);
	//g_tex[1] = Texture_SetLoadFile("Texture\\2p_goal.png", SCREEN_WIDTH, SCREEN_HEIGHT);
	//g_tex[2] = Texture_SetLoadFile("Texture\\3p_goal.png", SCREEN_WIDTH, SCREEN_HEIGHT);
	//g_tex[3] = Texture_SetLoadFile("Texture\\4p_goal.png", SCREEN_WIDTH, SCREEN_HEIGHT);
	g_goal_flag = false;
	g_tex = Texture_SetLoadFile("Texture\\goal_flag.png", 64, 256);
}

//=====================================================
//�I��
//=====================================================
void Goal_Uninit(void)
{

}

//=====================================================
//�X�V
//=====================================================
void Goal_Update(void)
{
	D3DXMATRIX mtxScale;
	//�P�ʍs��
	D3DXMatrixIdentity(&g_mtxWorld);
	D3DXMatrixTranslation(&g_mtxWorld, g_pos.x, g_pos.y, g_pos.z);
	//�傫��
	D3DXMatrixScaling(&mtxScale, 1.0f, 4.0f, 4.0f);

	//�r���{�[�h
	D3DXMATRIX mtxBill = (Game_GetScene() == SCENE_CAMERA) ? Course_Camera_Billboard() : Camera_Billbord();
	g_mtxWorld = mtxBill * g_mtxWorld;
	g_mtxWorld = mtxScale * g_mtxWorld;

}

//=====================================================
//�`��
//=====================================================
void Goal_Draw(void)
{
	//�f�o�C�X�̃|�C���^�擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);	//FALSE:���C�gOFF TRUE:���C�gON
	//UV���]
	pDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_MIRROR);
	pDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_MIRROR);
	//�`��
	Sprite3d_Draw(g_tex, g_mtxWorld);
	//WRAP
	pDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
	pDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);
}

//=====================================================
//�X�g�[�����Ƃ̃S�[���܂ł̋���
//=====================================================
float Goal_Range(D3DXVECTOR3 pos)
{
	return (float)sqrt((g_pos.x - pos.x) * (g_pos.x - pos.x) + (g_pos.y - pos.y) * (g_pos.y - pos.y) + (g_pos.z - pos.z) * (g_pos.z - pos.z));
}

//=====================================================
//�S�[���������ǂ���
//=====================================================
bool Goal_Flag(float goal_range, float move)
{
	return (goal_range <= GOAL_RANGE && move <= DEAD_ZONE) ? true : false;
}

//=====================================================
//�������̃v���C���[�̗v�f���擾
//=====================================================
void Goal_GetWinPlayer(int num)
{
	g_win_player = num;
}

//=====================================================
//�X�e�[�W�G�f�B�^�̃f�[�^�擾
//=====================================================
void Goal_Set(D3DXMATRIX mtxWorld)
{
	g_mtxWorld = mtxWorld;
	g_pos.x = mtxWorld._41;
	g_pos.y = mtxWorld._42 + 1.5f;
	g_pos.z = mtxWorld._43;
}

//=====================================================
//�S�[���̍��W�擾
//=====================================================
D3DXVECTOR3 Goal_GetPos(void)
{
	return g_pos;
}