#include <d3dx9.h>
#include "mydirectx.h"
#include "common.h"
#include "debug_font.h"
#include "input.h"
#include "joycon.h"
#include "mouse.h"
#include "camera.h"
#include "stone.h"
#include "goal.h"

//====================================================
//�}�N����`
//====================================================
#define PLAYER_MAX_NUM (4)	//�v���C�l��
#define DEAD_ZONE (0.001f)	//�����_�덷�p

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
	D3DXVECTOR3 pos = { 0.0f, 0.5f, 0.0f };				//�ʒu���W
	D3DXVECTOR3 start_pos = { 0.0f, 0.5f, 0.0f };		//1F�O�̈ʒu���W
	float move;
	bool is_turn;
	bool is_move;
	float goal_range;
	float stone_range;
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
		g_stone[i].start_pos = D3DXVECTOR3(-5.0f + i * 2.0f, 0.5f, 0.0f);	//���W
		D3DXMatrixTranslation(&g_stone[i].mtx_world, g_stone[i].pos.x, g_stone[i].pos.y, g_stone[i].pos.z);		//���s�ړ�
		g_stone[i].move = 0.0f;			//�ړ���
		g_stone[i].is_turn = false;
		g_stone[i].is_move = false;
		g_stone[i].goal_range = Goal_Range(g_stone[i].pos);
		g_stone[i].stone_range = 0.0f;
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
		//���N���b�N���Ŋ������ĂȂ�����Έړ��ʂ̐ݒ�
		if (GetAsyncKeyState(VK_LBUTTON) && Stone_Range(g_stone[i].pos, g_stone[i].start_pos) <= DEAD_ZONE)
		{
			g_stone[i].move = 0.1f * Mouse_GetForce();
		}
		//�X�g�[���̈ړ�
		else
		{
			g_stone[i].move *= 0.98f;
			D3DXMatrixTranslation(&g_stone[i].mtx_move, 0, 0, g_stone[i].move);
			g_stone[i].goal_range = Goal_Range(g_stone[i].pos);	//�S�[���܂ł̋����v�Z
			if (g_stone[i].move >= DEAD_ZONE)
			{
				g_throw_start = true;
			}
			//�ړ��ʂ�DEAD_ZONE�ȉ��Ŏ��̃^�[��
			if (g_stone[i].move <= DEAD_ZONE && g_throw_start)
			{
				g_stone[g_player_turn].is_turn = false;							//���ݑ��삵�Ă���X�g�[���̃^�[����false
				g_stone[g_player_turn].start_pos = g_stone[g_player_turn].pos;	//�X�^�[�g�ʒu�ɍ��̍��W����
				g_stone[g_player_turn].move = 0.0f;								//�ړ��ʂ̏�����
				g_player_turn++;												//���̃v���C���[�̃^�[��
				g_player_turn %= 4;												//0�`3�ŌJ��Ԃ����߂̗]�艉�Z�q
				g_stone[g_player_turn].is_turn = true;							//�v���X���ꂽ���݂̃v���C���[�̃^�[����true��
				g_throw_start = false;											//�X�^�[�g�p�ϐ���false
				Joycon_Reset();	//��]�l���Z�b�g
			}
		}
		//�X�g�[���̐i�񂾋���
		g_stone[i].stone_range = Stone_Range(g_stone[i].pos, g_stone[i].start_pos);

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
	//�f�o�C�X�̃|�C���^�擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	for (int i = 0; i < PLAYER_MAX_NUM; i++)
	{
		//DebugFont_Draw(0, 32 * (6 + i), "%dP���i�񂾋��� = %.02lf", i + 1, g_stone[i].range);
		//DebugFont_Draw(0, 32 * (6 + i), "%dP�̃S�[���܂ł̋��� = %.02lf", i + 1, g_stone[i].goal_range);
		DebugFont_Draw(0, 32 * (6 + i), "%dP�̐i�񂾋��� = %.02lf", i + 1, g_stone[i].stone_range);

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

//=====================================================
//�X�g�[���̐i�񂾋���
//=====================================================
float Stone_Range(D3DXVECTOR3 pos, D3DXVECTOR3 start_pos)
{
	return sqrt((pos.x - start_pos.x) * (pos.x - start_pos.x) + (pos.y - start_pos.y) * (pos.y - start_pos.y) + (pos.z - start_pos.z) * (pos.z - start_pos.z));
}

//=====================================================
//�X�g�[���������Ă��邩�ǂ���
//=====================================================
bool Stone_Move(int index)
{
	//�X�g�[���̈ړ��ʂ�DEAD_ZONE�ȉ��ł���ΐÎ~���Ɣ��f
	if (g_stone[index].move <= DEAD_ZONE)
	{
		return false;
	}
	return true;
}