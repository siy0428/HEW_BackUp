#include "texture.h"
#include "texture.h"
#include "sprite.h"
#include "stone.h"
#include "mydirectx.h"
#include "joycon.h"

//====================================================
//�}�N����`
//====================================================
#define GAUGE_WIDTH (32)
#define GAUGE_HEIGHT (256)

//====================================================
//�\���̐錾
//====================================================
typedef struct GLine_Vertex_tag
{
	D3DXVECTOR3 position;	//���W
	D3DXVECTOR3 normal;		//�@���x�N�g��
	D3DCOLOR color;			//�F���
	D3DXVECTOR2 UV;			//�e�N�X�`��
}GLineVertex;
#define FVF_CUBE (D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_NORMAL | D3DFVF_TEX1)

//====================================================
//�O���[�o���ϐ�
//====================================================
static unsigned int g_tex;
static const GLineVertex g_guide_line[] = {
	//����
	{D3DXVECTOR3(-0.5f, 0.5f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, -1.0f), D3DCOLOR_RGBA(255, 255, 255, 255), D3DXVECTOR2(0.0f, 0.0f)},
	{D3DXVECTOR3(0.5f + GAUGE_WIDTH, 0.5f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, -1.0f), D3DCOLOR_RGBA(255, 255, 255, 255), D3DXVECTOR2(1.0f, 0.0f)},
	{D3DXVECTOR3(-0.5f, -0.5f - GAUGE_HEIGHT, 0.0f), D3DXVECTOR3(0.0f, 0.0f, -1.0f), D3DCOLOR_RGBA(255, 255, 255, 255), D3DXVECTOR2(0.0f, 1.0f)},
	{D3DXVECTOR3(0.5f + GAUGE_WIDTH, -0.5f - GAUGE_HEIGHT, 0.0f),D3DXVECTOR3(0.0f, 0.0f, -1.0f),  D3DCOLOR_RGBA(255, 255, 255, 255), D3DXVECTOR2(1.0f, 1.0f)},
};
static D3DXVECTOR3 g_pos(0.0f, 10.5f, 0.0f);
static D3DXMATRIX g_mtxWorld, g_mtxScaling, g_mtxRotate[2], g_mtxTrans[2];

//====================================================
//������
//====================================================
void GuideLine_Init(void)
{
	g_tex = Texture_SetLoadFile("Texture\\guide_line.png", GAUGE_WIDTH, GAUGE_HEIGHT);
}

//====================================================
//�I��
//====================================================
void GuideLine_Uninit(void)
{

}

//====================================================
//�X�V
//====================================================
void GuideLine_Update(void)
{
	int stone_turn = Stone_PlayerTurn();	//���ݑ��삵�Ă���v���C���[�̗v�f���擾

	//�X�g�[���̍��W�擾
	g_pos = Stone_GetPos(stone_turn);

	D3DXMatrixIdentity(&g_mtxWorld);
	D3DXMatrixTranslation(&g_mtxTrans[0], g_pos.x, g_pos.y, g_pos.z);
	D3DXMatrixTranslation(&g_mtxTrans[1], -0.2f, 1.0f, 3.0f);
	D3DXMatrixScaling(&g_mtxScaling, 0.015f, 0.015f, 1.0f);
	D3DXMatrixRotationX(&g_mtxRotate[0], 90 * D3DX_PI / 180);
	D3DXMatrixRotationY(&g_mtxRotate[1], Stone_Rot(stone_turn));

	g_mtxWorld = g_mtxScaling * g_mtxRotate[0] * g_mtxTrans[1]* g_mtxRotate[1] * g_mtxTrans[0];
}

//====================================================
//�`��
//====================================================
void GuideLine_Draw(void)
{
	//�f�o�C�X�̃|�C���^�擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);	//FALSE:���C�gOFF TRUE:���C�gON

	//�X�g�[���������Ă��Ȃ���΃K�C�h���C����`�悷��
	if (Stone_GetRange(Stone_PlayerTurn()) >= 0.001f)
	{
		return;
	}
	pDevice->SetTransform(D3DTS_WORLD, &g_mtxWorld);

	//�`��ݒ�
	pDevice->SetFVF(FVF_CUBE);						//�f�o�C�X�ɒ��_�f�[�^��n��
	pDevice->SetTexture(0, Texture_GetTexture(g_tex));	//�e�N�X�`�����f�o�C�X�ɓn��
	pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, g_guide_line, sizeof(GLineVertex));
}