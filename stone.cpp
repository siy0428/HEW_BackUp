#include <d3dx9.h>
#include "mydirectx.h"
#include "common.h"
#include "debug_font.h"
#include "input.h"
#include "joycon.h"
#include "mouse.h"
#include "camera.h"

//====================================================
//�}�N����`
//====================================================
#define PLAYER_MAX_NUM (4)	//�v���C�l��

//====================================================
//�\���̐錾
//====================================================
typedef struct Stone_Vertex_tag
{
	D3DXVECTOR3 position;	//���W
	D3DCOLOR color;			//�F���
}StoneVertex;
#define FVF_CUBE (D3DFVF_XYZ | D3DFVF_DIFFUSE)

//�X�g�[���\����
typedef struct
{
	D3DXMATRIX mtx_world;		//���[���h�s��
	D3DXMATRIX mtx_rotation;	//��]�s��
	D3DXMATRIX mtx_trans;		//���s�ړ�
	D3DXMATRIX mtx_move;		//�ړ���
	D3DXVECTOR3 pos = { 0.0f, 0.5f, 0.0f };		//�ʒu���W
	float move;
	bool is_turn;
}Stone;

//=====================================================
//�O���[�o���ϐ�
//=====================================================
static const StoneVertex g_stone_vertex[] = {						//���_�\����
	//����
	{D3DXVECTOR3(-0.5f, 0.5f, -0.5f), D3DCOLOR_RGBA(255, 255, 255, 255)},
	{D3DXVECTOR3(0.5f, 0.5f, -0.5f), D3DCOLOR_RGBA(255, 255, 255, 255)},
	{D3DXVECTOR3(0.5f, -0.5f, -0.5f), D3DCOLOR_RGBA(255, 255, 255, 255)},
	{D3DXVECTOR3(0.5f, -0.5f, -0.5f), D3DCOLOR_RGBA(255, 255, 255, 255)},
	{D3DXVECTOR3(-0.5f, -0.5f, -0.5f), D3DCOLOR_RGBA(255, 255, 255, 255)},
	{D3DXVECTOR3(-0.5f, 0.5f, -0.5f), D3DCOLOR_RGBA(255, 255, 255, 255)},
	//�E��
	{D3DXVECTOR3(0.5f, 0.5f, -0.5f), D3DCOLOR_RGBA(0, 255, 0, 255)},
	{D3DXVECTOR3(0.5f, 0.5f, 0.5f), D3DCOLOR_RGBA(0, 255, 0, 255)},
	{D3DXVECTOR3(0.5f, -0.5f, 0.5f), D3DCOLOR_RGBA(0, 255, 0, 255)},
	{D3DXVECTOR3(0.5f, -0.5f, 0.5f), D3DCOLOR_RGBA(0, 255, 0, 255)},
	{D3DXVECTOR3(0.5f, -0.5f, -0.5f), D3DCOLOR_RGBA(0, 255, 0, 255)},
	{D3DXVECTOR3(0.5f, 0.5f, -0.5f), D3DCOLOR_RGBA(0, 255, 0, 255)},
	//����
	{D3DXVECTOR3(-0.5f, 0.5f, 0.5f), D3DCOLOR_RGBA(255, 0, 0, 255)},
	{D3DXVECTOR3(-0.5f, 0.5f, -0.5f), D3DCOLOR_RGBA(255, 0, 0, 255)},
	{D3DXVECTOR3(-0.5f, -0.5f, 0.5f), D3DCOLOR_RGBA(255, 0, 0, 255)},
	{D3DXVECTOR3(-0.5f, -0.5f, 0.5f), D3DCOLOR_RGBA(255, 0, 0, 255)},
	{D3DXVECTOR3(-0.5f, 0.5f, -0.5f), D3DCOLOR_RGBA(255, 0, 0, 255)},
	{D3DXVECTOR3(-0.5f, -0.5f, -0.5f), D3DCOLOR_RGBA(255, 0, 0, 255)},
	//����
	{D3DXVECTOR3(0.5f, 0.5f, 0.5f), D3DCOLOR_RGBA(64, 128, 0, 255)},
	{D3DXVECTOR3(-0.5f, 0.5f, 0.5f), D3DCOLOR_RGBA(64, 128, 0, 255)},
	{D3DXVECTOR3(-0.5f, -0.5f, 0.5f), D3DCOLOR_RGBA(64, 128, 0, 255)},
	{D3DXVECTOR3(-0.5f, -0.5f, 0.5f), D3DCOLOR_RGBA(64, 128, 0, 255)},
	{D3DXVECTOR3(0.5f, -0.5f, 0.5f), D3DCOLOR_RGBA(64, 128, 0, 255)},
	{D3DXVECTOR3(0.5f, 0.5f, 0.5f), D3DCOLOR_RGBA(64, 128, 0, 255)},
	//���
	{D3DXVECTOR3(0.5f, -0.5f, 0.5f), D3DCOLOR_RGBA(128, 32, 64, 255)},
	{D3DXVECTOR3(-0.5f, -0.5f, 0.5f), D3DCOLOR_RGBA(128, 32, 64, 255)},
	{D3DXVECTOR3(0.5f, -0.5f, -0.5f), D3DCOLOR_RGBA(128, 32, 64, 255)},
	{D3DXVECTOR3(0.5f, -0.5f, -0.5f), D3DCOLOR_RGBA(128, 32, 64, 255)},
	{D3DXVECTOR3(-0.5f, -0.5f, 0.5f), D3DCOLOR_RGBA(128, 32, 64, 255)},
	{D3DXVECTOR3(-0.5f, -0.5f, -0.5f), D3DCOLOR_RGBA(128, 32, 64, 255)},
	//���
	{D3DXVECTOR3(-0.5f, 0.5f, 0.5f), D3DCOLOR_RGBA(255, 255, 0, 255)},
	{D3DXVECTOR3(0.5f, 0.5f, 0.5f), D3DCOLOR_RGBA(255, 255, 0, 255)},
	{D3DXVECTOR3(-0.5f, 0.5f, -0.5f), D3DCOLOR_RGBA(255, 255, 0, 255)},
	{D3DXVECTOR3(-0.5f, 0.5f, -0.5f), D3DCOLOR_RGBA(255, 255, 0, 255)},
	{D3DXVECTOR3(0.5f, 0.5f, 0.5f), D3DCOLOR_RGBA(255, 255, 0, 255)},
	{D3DXVECTOR3(0.5f, 0.5f, -0.5f), D3DCOLOR_RGBA(255, 255, 0, 255)}

};

static Stone g_stone[PLAYER_MAX_NUM];
static int g_player_turn = 0;
static bool g_throw_start = false;

//=====================================================
//������
//=====================================================
void Stone_Init(void)
{
	//�X�g�[���̏�����
	for (int i = 0; i < PLAYER_MAX_NUM; i++)
	{
		g_stone[i].pos = D3DXVECTOR3(-5.0f + i * 2.0f, 0.5f, 0.0f);	//���W
		D3DXMatrixTranslation(&g_stone[i].mtx_world, g_stone[i].pos.x, g_stone[i].pos.y, g_stone[i].pos.z);		//���s�ړ�
		g_stone[i].move = 0.0f;			//�ړ���
		g_stone[i].is_turn = false;
	}
	//�ŏ��̃X�g�[���̃^�[���ݒ�
	g_stone[0].is_turn = true;
	g_player_turn = 0;
	g_throw_start = false;
}

//=====================================================
//�I��
//=====================================================
void Stone_Uninit(void)
{

}

//=====================================================
//�X�V
//=====================================================
void Stone_Update(void)
{
	//�ǂ̃v���C���[�̃^�[��������
	for (int i = 0; i < PLAYER_MAX_NUM; i++)
	{
		//�^�[�������ĂȂ���Ύ��̃v���C���[��r
		if (!g_stone[i].is_turn)
		{
			continue;
		}
		//���N���b�N���Ɉړ��ʂ̐ݒ�
		if (GetAsyncKeyState(VK_LBUTTON))
		{
			g_stone[i].move = 0.1f * Mouse_GetForce();
			g_throw_start = true;	//�X�^�[�g�p�ϐ�
		}
		//�X�g�[���̈ړ�
		else
		{
			g_stone[i].move *= 0.98f;
			D3DXMatrixTranslation(&g_stone[i].mtx_move, 0, 0, g_stone[i].move);
			//�ړ��ʂ�0�ȉ��ɂȂ�����
			if (g_stone[i].move <= 0.001f && g_throw_start)
			{
				g_stone[g_player_turn].is_turn = false;
				g_player_turn++;
				g_player_turn %= 4;
				g_stone[g_player_turn].is_turn = true;
				g_throw_start = false;
				Joycon_Reset();	//��]���Z�b�g
			}
		}

		//���[���h���W�ϊ�
		D3DXMatrixIdentity(&g_stone[i].mtx_world);	//�P�ʍs������
		D3DXMatrixRotationY(&g_stone[i].mtx_rotation, Joycon_Operator() * D3DX_PI / 180);		//y����]
		D3DXMatrixTranslation(&g_stone[i].mtx_world, g_stone[i].pos.x, g_stone[i].pos.y, g_stone[i].pos.z);		//���s�ړ�
		g_stone[i].mtx_world = g_stone[i].mtx_move * g_stone[i].mtx_rotation * g_stone[i].mtx_world;

		//�s�񍇐�������Ԃł̍��W�擾
		g_stone[i].pos.x = g_stone[i].mtx_world._41;
		g_stone[i].pos.y = g_stone[i].mtx_world._42;
		g_stone[i].pos.z = g_stone[i].mtx_world._43;
	}
}

//=====================================================
//�`��
//=====================================================
void Stone_Draw(void)
{
	DebugFont_Draw(0, 32 * 6, "��]�� = %.02lf", Joycon_Operator() * D3DX_PI / 180);
	//�f�o�C�X�̃|�C���^�擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	for (int i = 0; i < PLAYER_MAX_NUM; i++)
	{
		//�`��ݒ�
		pDevice->SetTransform(D3DTS_WORLD, &g_stone[i].mtx_world);
		pDevice->SetFVF(FVF_CUBE);						//�f�o�C�X�ɒ��_�f�[�^��n��
		pDevice->SetTexture(0, NULL);					//�e�N�X�`�����f�o�C�X�ɓn��
		pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);	//FALSE:���C�gOFF TRUE:���C�gON
		pDevice->DrawPrimitiveUP(D3DPT_TRIANGLELIST, 12, g_stone_vertex, sizeof(StoneVertex));
	}
}

//=====================================================
//���W�擾
//=====================================================
D3DXVECTOR3 Stone_GetPos(int index)
{
	return g_stone[index].pos;
}

//=====================================================
//��P�����쒆��
//=====================================================
int Stone_PlayerTurn(void)
{
	return g_player_turn;
}