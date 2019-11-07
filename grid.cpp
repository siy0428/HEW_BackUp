#include <d3dx9.h>
#include "mydirectx.h"
#include "common.h"
#include "camera.h"

//====================================================
//�}�N����`
//====================================================
#define GRID_VERTEX (1024)	//�O���b�h���_��

//====================================================
//�\���̐錾
//====================================================
typedef struct Grid_Vertex_tag
{
	D3DXVECTOR3 position;	//���W
	D3DCOLOR color;			//�F���
}GridVertex;
#define FVF_GRID (D3DFVF_XYZ | D3DFVF_DIFFUSE)

//=====================================================
//�O���[�o���ϐ�
//=====================================================
static GridVertex g_grid_vertex[GRID_VERTEX] = { };
static int g_Length = 0;

//=====================================================
//������
//=====================================================
void Grid_Init(int length)
{
	//����
	for (int i = 0; i < length + 1; i++)
	{
		g_grid_vertex[i * 2].position = D3DXVECTOR3(-(float)length / 2, 0.0f, (float)length / 2 - i * 1.0f);		//��
		g_grid_vertex[i * 2 + 1].position = D3DXVECTOR3((float)length / 2, 0.0f, (float)length / 2 - i * 1.0f);	//�E
		//�����̐Ԑ�
		if (i == length / 2)
		{
			g_grid_vertex[i * 2].color = D3DCOLOR_RGBA(255, 0, 0, 255);
			g_grid_vertex[i * 2 + 1].color = D3DCOLOR_RGBA(255, 0, 0, 255);
		}
		else
		{
			g_grid_vertex[i * 2].color = D3DCOLOR_RGBA(111, 191, 0, 255);
			g_grid_vertex[i * 2 + 1].color = D3DCOLOR_RGBA(111, 191, 0, 255);
		}
	}
	//�c��
	for (int i = 0; i < length + 1; i++)
	{
		g_grid_vertex[i * 2 + (length + 1) * 2].position = D3DXVECTOR3((float)length / 2 - i * 1.0f, 0.0f, -(float)length / 2);	
		g_grid_vertex[i * 2 + 1 + (length + 1) * 2].position = D3DXVECTOR3((float)length / 2 - i * 1.0f, 0.0f, (float)length / 2);
		//�����̐Ԑ�
		if (i == length / 2)
		{
			g_grid_vertex[i * 2 + (length + 1) * 2].color = D3DCOLOR_RGBA(255, 0, 0, 255);
			g_grid_vertex[i * 2 + 1 + (length + 1) * 2].color = D3DCOLOR_RGBA(255, 0, 0, 255);
		}
		else
		{
			g_grid_vertex[i * 2 + (length + 1) * 2].color = D3DCOLOR_RGBA(111, 191, 0, 255);
			g_grid_vertex[i * 2 + 1 + (length + 1) * 2].color = D3DCOLOR_RGBA(111, 191, 0, 255);
		}
	}
	g_Length = length;
}

//=====================================================
//�I��
//=====================================================
void Grid_Uninit(void)
{

}

//=====================================================
//�`��
//=====================================================
void Grid_Draw(void)
{
	//�f�o�C�X�̃|�C���^�擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//���[���h���W�ϊ�
	D3DXMATRIX mtxWorld, mtxTrans;							//�s��p�ϐ�
	D3DXMatrixIdentity(&mtxWorld);							//�P�ʍs������
	D3DXMatrixTranslation(&mtxTrans, 0.0f, 0.0f, 0.0f);		//���s�ړ�
	mtxWorld = mtxTrans;									//�s��̍���
	pDevice->SetTransform(D3DTS_WORLD, &mtxWorld);

	//�`��ݒ�
	pDevice->SetFVF(FVF_GRID);						//�f�o�C�X�ɒ��_�f�[�^��n��
	pDevice->SetTexture(0, NULL);					//�e�N�X�`�����f�o�C�X�ɓn��
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);	//FALSE:���C�gOFF TRUE:���C�gON
	pDevice->DrawPrimitiveUP(D3DPT_LINELIST, g_Length * 2 + 2, g_grid_vertex, sizeof(GridVertex));
}

////����
//for (int i = 0; i < 11; i++)
//{
//	g_grid_vertex[i * 2].position = D3DXVECTOR3(-5.0f, 0.0f, 5.0f - i * 1.0f);		//��
//	g_grid_vertex[i * 2 + 1].position = D3DXVECTOR3(5.0f, 0.0f, 5.0f - i * 1.0f);	//�E
//	//�����̐Ԑ�
//	if (i == 5)
//	{
//		g_grid_vertex[i * 2].color = D3DCOLOR_RGBA(255, 0, 0, 255);
//		g_grid_vertex[i * 2 + 1].color = D3DCOLOR_RGBA(255, 0, 0, 255);
//	}
//	else
//	{
//		g_grid_vertex[i * 2].color = D3DCOLOR_RGBA(111, 191, 0, 255);
//		g_grid_vertex[i * 2 + 1].color = D3DCOLOR_RGBA(111, 191, 0, 255);
//	}
//}
////�c��
//for (int i = 0; i < 11; i++)
//{
//	g_grid_vertex[i * 2 + 22].position = D3DXVECTOR3(5.0f - i * 1.0f, 0.0f, -5.0f);		//��
//	g_grid_vertex[i * 2 + 1 + 22].position = D3DXVECTOR3(5.0f - i * 1.0f, 0.0f, 5.0f);	//�E
//	//�����̐Ԑ�
//	if (i == 5)
//	{
//		g_grid_vertex[i * 2 + 22].color = D3DCOLOR_RGBA(255, 0, 0, 255);
//		g_grid_vertex[i * 2 + 1 + 22].color = D3DCOLOR_RGBA(255, 0, 0, 255);
//	}
//	else
//	{
//		g_grid_vertex[i * 2 + 22].color = D3DCOLOR_RGBA(111, 191, 0, 255);
//		g_grid_vertex[i * 2 + 1 + 22].color = D3DCOLOR_RGBA(111, 191, 0, 255);
//	}
//}