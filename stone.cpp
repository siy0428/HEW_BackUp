#include <d3dx9.h>
#include "mydirectx.h"
#include "common.h"
#include "debug_font.h"
#include "input.h"
#include "joycon.h"
#include "mouse.h"

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
	D3DXMATRIX mtxWorld;		//���[���h�s��
	D3DXMATRIX mtxRotation;		//��]�s��
	D3DXMATRIX mtxTrans;		//���s�ړ�
	D3DXMATRIX mtxMove;			//�ړ���
	D3DXVECTOR3 pos = { 0.0f, 0.5f, 0.0f };		//�ʒu���W
	float move;
	bool isTurn;
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

static Stone g_Stone[PLAYER_MAX_NUM];
static int g_PlayerTurn = 0;

//=====================================================
//������
//=====================================================
void Stone_Init(void)
{
	//�X�g�[���̏�����
	for (int i = 0; i < PLAYER_MAX_NUM; i++)
	{
		g_Stone[i].pos = D3DXVECTOR3(-5.0f + i * 2.0f, 0.5f, 0.0f);	//���W
		D3DXMatrixTranslation(&g_Stone[i].mtxWorld, g_Stone[i].pos.x, g_Stone[i].pos.y, g_Stone[i].pos.z);		//���s�ړ�
		g_Stone[i].move = 0.0f;			//�ړ���
		g_Stone[i].isTurn = false;
	}
	//�ŏ��̃X�g�[���̃^�[���ݒ�
	g_Stone[0].isTurn = true;
	g_PlayerTurn = 0;
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
	Keyboard_Update();

	//�ǂ̃v���C���[�̃^�[��������
	for (int i = 0; i < PLAYER_MAX_NUM; i++)
	{
		//�^�[�������ĂȂ���Ύ��̃v���C���[��r
		if (!g_Stone[i].isTurn)
		{
			continue;
		}
		//���N���b�N���Ɉړ��ʂ̐ݒ�
		if (GetAsyncKeyState(VK_LBUTTON))
		{
			g_Stone[i].move = 0.1f * Mouse_GetForce();
		}
		//�X�g�[���̈ړ�
		else
		{
			g_Stone[i].move *= 0.98f;
			D3DXMatrixTranslation(&g_Stone[i].mtxMove, 0, 0, g_Stone[i].move);
		}
		//���[���h���W�ϊ�
		D3DXMatrixIdentity(&g_Stone[i].mtxWorld);	//�P�ʍs������

		D3DXMatrixRotationY(&g_Stone[i].mtxRotation, Joycon_Operator() * D3DX_PI / 180);		//y����]
		D3DXMatrixTranslation(&g_Stone[i].mtxWorld, g_Stone[i].pos.x, g_Stone[i].pos.y, g_Stone[i].pos.z);		//���s�ړ�
		g_Stone[i].mtxWorld = g_Stone[i].mtxMove * g_Stone[i].mtxRotation * g_Stone[i].mtxWorld;

		//�s�񍇐�������Ԃł̍��W�擾
		g_Stone[i].pos.x = g_Stone[i].mtxWorld._41;
		g_Stone[i].pos.y = g_Stone[i].mtxWorld._42;
		g_Stone[i].pos.z = g_Stone[i].mtxWorld._43;
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

	//�ǂ̃v���C���[�̃^�[��������
	for (int i = 0; i < PLAYER_MAX_NUM; i++)
	{
		pDevice->SetTransform(D3DTS_WORLD, &g_Stone[i].mtxWorld);
		//�`��ݒ�
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
	return g_Stone[index].pos;
}

//=====================================================
//��P�����쒆��
//=====================================================
int Stone_PlayerTurn(void)
{
	return g_PlayerTurn;
}