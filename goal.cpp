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
#define GOAL_HEIGHT (3.0f)	//�S�[���̉�����
#define DEAD_ZONE (0.001f)	//�����_�덷�p
#define GOAL_RANGE (1.0f)	//�S�[������p�}�N����`

//====================================================
//�\���̐錾
//====================================================
typedef struct Goal_Vertex_tag
{
	D3DXVECTOR3 position;	//���W
	D3DCOLOR color;			//�F���
}GoalVertex;
#define FVF_CUBE (D3DFVF_XYZ | D3DFVF_DIFFUSE)

//=====================================================
//�O���[�o���ϐ�
//=====================================================
static const GoalVertex g_goal_vertex[] = {						//���_�\����
	//����
	{D3DXVECTOR3(-1.0f, GOAL_HEIGHT, -1.0f), D3DCOLOR_RGBA(255, 255, 255, 255)},
	{D3DXVECTOR3(1.0f, GOAL_HEIGHT, -1.0f),  D3DCOLOR_RGBA(255, 255, 255, 255)},
	{D3DXVECTOR3(1.0f, -GOAL_HEIGHT, -1.0f), D3DCOLOR_RGBA(255, 255, 255, 255)},
	{D3DXVECTOR3(1.0f, -GOAL_HEIGHT, -1.0f), D3DCOLOR_RGBA(255, 255, 255, 255)},
	{D3DXVECTOR3(-1.0f, -GOAL_HEIGHT, -1.0f),D3DCOLOR_RGBA(255, 255, 255, 255)},
	{D3DXVECTOR3(-1.0f, GOAL_HEIGHT, -1.0f), D3DCOLOR_RGBA(255, 255, 255, 255)},
	//�E��
	{D3DXVECTOR3(1.0f, GOAL_HEIGHT, -1.0f), D3DCOLOR_RGBA(255, 255, 255, 255)},
	{D3DXVECTOR3(1.0f, GOAL_HEIGHT, 1.0f),  D3DCOLOR_RGBA(255, 255, 255, 255)},
	{D3DXVECTOR3(1.0f, -GOAL_HEIGHT, 1.0f), D3DCOLOR_RGBA(255, 255, 255, 255)},
	{D3DXVECTOR3(1.0f, -GOAL_HEIGHT, 1.0f), D3DCOLOR_RGBA(255, 255, 255, 255)},
	{D3DXVECTOR3(1.0f, -GOAL_HEIGHT, -1.0f),D3DCOLOR_RGBA(255, 255, 255, 255)},
	{D3DXVECTOR3(1.0f, GOAL_HEIGHT, -1.0f), D3DCOLOR_RGBA(255, 255, 255, 255)},
	//����
	{D3DXVECTOR3(-1.0f, GOAL_HEIGHT, 1.0f), D3DCOLOR_RGBA(255, 255, 255, 255)},
	{D3DXVECTOR3(-1.0f, GOAL_HEIGHT, -1.0f),D3DCOLOR_RGBA(255, 255, 255, 255)},
	{D3DXVECTOR3(-1.0f, -GOAL_HEIGHT, 1.0f),D3DCOLOR_RGBA(255, 255, 255, 255)},
	{D3DXVECTOR3(-1.0f, -GOAL_HEIGHT, 1.0f),D3DCOLOR_RGBA(255, 255, 255, 255)},
	{D3DXVECTOR3(-1.0f, GOAL_HEIGHT, -1.0f),D3DCOLOR_RGBA(255, 255, 255, 255)},
	{D3DXVECTOR3(-1.0f, -GOAL_HEIGHT, -1.0f),D3DCOLOR_RGBA(255, 255, 255, 255)},
	//����
	{D3DXVECTOR3(1.0f, GOAL_HEIGHT, 1.0f),  D3DCOLOR_RGBA(255, 255, 255, 255)},
	{D3DXVECTOR3(-1.0f, GOAL_HEIGHT, 1.0f), D3DCOLOR_RGBA(255, 255, 255, 255)},
	{D3DXVECTOR3(-1.0f, -GOAL_HEIGHT, 1.0f),D3DCOLOR_RGBA(255, 255, 255, 255)},
	{D3DXVECTOR3(-1.0f, -GOAL_HEIGHT, 1.0f),D3DCOLOR_RGBA(255, 255, 255, 255)},
	{D3DXVECTOR3(1.0f, -GOAL_HEIGHT, 1.0f), D3DCOLOR_RGBA(255, 255, 255, 255)},
	{D3DXVECTOR3(1.0f, GOAL_HEIGHT, 1.0f),  D3DCOLOR_RGBA(255, 255, 255, 255)},
	//���
	{D3DXVECTOR3(1.0f, -GOAL_HEIGHT, 1.0f),  D3DCOLOR_RGBA(255, 255, 255, 255)},
	{D3DXVECTOR3(-1.0f, -GOAL_HEIGHT, 1.0f), D3DCOLOR_RGBA(255, 255, 255, 255)},
	{D3DXVECTOR3(1.0f, -GOAL_HEIGHT, -1.0f), D3DCOLOR_RGBA(255, 255, 255, 255)},
	{D3DXVECTOR3(1.0f, -GOAL_HEIGHT, -1.0f), D3DCOLOR_RGBA(255, 255, 255, 255)},
	{D3DXVECTOR3(-1.0f, -GOAL_HEIGHT, 1.0f), D3DCOLOR_RGBA(255, 255, 255, 255)},
	{D3DXVECTOR3(-1.0f, -GOAL_HEIGHT, -1.0f),D3DCOLOR_RGBA(255, 255, 255, 255)},
	//���
	{D3DXVECTOR3(-1.0f, GOAL_HEIGHT, 1.0f), D3DCOLOR_RGBA(255, 255, 255, 255)},
	{D3DXVECTOR3(1.0f, GOAL_HEIGHT, 1.0f),  D3DCOLOR_RGBA(255, 255, 255, 255)},
	{D3DXVECTOR3(-1.0f, GOAL_HEIGHT, -1.0f),D3DCOLOR_RGBA(255, 255, 255, 255)},
	{D3DXVECTOR3(-1.0f, GOAL_HEIGHT, -1.0f),D3DCOLOR_RGBA(255, 255, 255, 255)},
	{D3DXVECTOR3(1.0f, GOAL_HEIGHT, 1.0f),  D3DCOLOR_RGBA(255, 255, 255, 255)},
	{D3DXVECTOR3(1.0f, GOAL_HEIGHT, -1.0f), D3DCOLOR_RGBA(255, 255, 255, 255)}
};
static D3DXVECTOR3 g_pos(0.0f, 0.5f, 0.0f);
static D3DXMATRIX g_mtxWorld;

//=====================================================
//������
//=====================================================
void Goal_Init(void)
{
	g_pos = D3DXVECTOR3(0.0f, 0.5f, 30.0f);
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
	//�P�ʍs��
	D3DXMatrixIdentity(&g_mtxWorld);
	D3DXMatrixTranslation(&g_mtxWorld, g_pos.x, g_pos.y, g_pos.z);
}

//=====================================================
//�`��
//=====================================================
void Goal_Draw(void)
{
	//�f�o�C�X�̃|�C���^�擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//�`��ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &g_mtxWorld);
	pDevice->SetFVF(FVF_CUBE);						//�f�o�C�X�ɒ��_�f�[�^��n��
	pDevice->SetTexture(0, NULL);					//�e�N�X�`�����f�o�C�X�ɓn��
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);	//FALSE:���C�gOFF TRUE:���C�gON
	pDevice->DrawPrimitiveUP(D3DPT_LINELIST, 16, g_goal_vertex, sizeof(GoalVertex));
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
	//�S�[�����Ă����ꍇtrue
	if (goal_range <= GOAL_RANGE && move <= DEAD_ZONE)
	{
		return true;
	}
	return false;
}