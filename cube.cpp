#include <d3dx9.h>
#include "mydirectx.h"
#include "common.h"
#include "debug_font.h"
#include "input.h"
#include <stdio.h>

//====================================================
//�\���̐錾
//====================================================
typedef struct Cube_Vertex_tag
{
	D3DXVECTOR3 position;	//���W
	D3DCOLOR color;			//�F���
}CubeVertex;
#define FVF_CUBE (D3DFVF_XYZ | D3DFVF_DIFFUSE)

//=====================================================
//�O���[�o���ϐ�
//=====================================================
static const CubeVertex g_cube_vertex[] = {						//���_�\����
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
	{D3DXVECTOR3(0.5f, 0.5f, -0.5f), D3DCOLOR_RGBA(255, 255, 0, 255)},
};
static D3DXVECTOR3 g_pos = { 4.5f, 0.5f, -4.5f };
//static D3DXVECTOR3 g_pos = { 0.5f, 0.0f, 0.5f };
static int g_state = 0;
static float g_angle = 0.0f;
static float g_scale = 0.0f;
static float g_fall[5] = {
	10.0f,
	10.0f,
	10.0f,
	10.0f,
	10.0f
};

//�T�C�R���p
static bool g_bRotate = false;

//=====================================================
//������
//=====================================================
void Cube_Init(void)
{

}

//=====================================================
//�`��
//=====================================================
void Cube_Draw(const D3DXMATRIX *pMtx)
{
	//�f�o�C�X�̃|�C���^�擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	pDevice->SetTransform(D3DTS_WORLD, pMtx);

	//�`��ݒ�
	pDevice->SetFVF(FVF_CUBE);						//�f�o�C�X�ɒ��_�f�[�^��n��
	pDevice->SetTexture(0, NULL);					//�e�N�X�`�����f�o�C�X�ɓn��
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);	//FALSE:���C�gOFF TRUE:���C�gON
	pDevice->DrawPrimitiveUP(D3DPT_TRIANGLELIST, 12, g_cube_vertex, sizeof(CubeVertex));
}

//=====================================================
//�T�C�R��
//=====================================================
void Cube_Dice(void)
{
	Keyboard_Update();

	D3DXMATRIX mtxWorld, mtxRotation[3], mtxTrans[3];		//�s��p�ϐ�
	D3DXMatrixIdentity(&mtxWorld);

	//�E�����@��
	if (Keyboard_IsPress(DIK_D))
	{

	}
	//�������@��
	else if (Keyboard_IsPress(DIK_A))
	{
		g_bRotate = true;
	}
	if (g_bRotate)
	{
		g_angle -= 1.0f;
		if (g_angle < -90.0f)
		{
			g_bRotate = false;
		}
	}
	D3DXMatrixTranslation(&mtxTrans[0], -0.5f, 0.0f, 0.5f);	//���s�ړ�
	D3DXMatrixRotationZ(&mtxRotation[2], g_angle * D3DX_PI / 180);	//z����]
	D3DXMatrixTranslation(&mtxTrans[1], 1.5f, 0.5f, 0.5f);	//���s�ړ�
	D3DXMatrixRotationY(&mtxRotation[1], g_angle * D3DX_PI / 180);	//y����]
	D3DXMatrixRotationX(&mtxRotation[0], g_angle * D3DX_PI / 180);	//x����]
	D3DXMatrixTranslation(&mtxTrans[1], g_pos.x, g_pos.y, g_pos.z);	//���s�ړ�
	mtxWorld = mtxTrans[0] * mtxRotation[2] * mtxTrans[1];						//�s��̍���

//D3DXMatrixRotationY(&mtxRotation[1], g_angle * D3DX_PI / 180);	//y����]
//D3DXMatrixRotationX(&mtxRotation[0], g_angle * D3DX_PI / 180);	//x����]
//D3DXMatrixRotationZ(&mtxRotation[2], g_angle * D3DX_PI / 180);		//z����]
//D3DXMatrixTranslation(&mtxTrans[1], g_pos.x, g_pos.y, g_pos.z);		//���s�ړ�
//mtxWorld = mtxRotation[2] * mtxTrans[1];				//�s��̍���

	Cube_Draw(&mtxWorld);
}

//=====================================================
//���Ɖۑ�
//=====================================================
void Cube_jyugyou(void)
{
	//���[���h���W�ϊ�
	D3DXMATRIX mtxWorld[8], mtxRotation[2][3], mtxTrans[8], mtxScaling[2];		//�s��p�ϐ�
	for (int i = 0; i < 8; i++)
	{
		D3DXMatrixIdentity(&mtxWorld[i]);										//�P�ʍs������
	}

	D3DXMatrixRotationX(&mtxRotation[0][0], 0 * D3DX_PI / 180);					//x����]
	D3DXMatrixRotationY(&mtxRotation[0][1], 0 * D3DX_PI / 180);					//y����]
	D3DXMatrixRotationZ(&mtxRotation[0][2], 0 * D3DX_PI / 180);					//z����]
	D3DXMatrixRotationY(&mtxRotation[1][1], g_angle * D3DX_PI / 180);			//y����]
	D3DXMatrixScaling(&mtxScaling[0], 1.0f, 1.0f, 1.0f);
	D3DXMatrixScaling(&mtxScaling[1], sin(g_scale) + 2, sin(g_scale) + 2, sin(g_scale) + 2);
	D3DXMatrixTranslation(&mtxTrans[0], g_pos.x, g_pos.y, g_pos.z);				//���s�ړ�
	D3DXMatrixTranslation(&mtxTrans[1], -4.5f, 0.5f, -4.5f);					//���s�ړ�

	D3DXMatrixTranslation(&mtxTrans[2], 4.5f - (sin(g_scale) + 1) / 2, 0.5f + (sin(g_scale) + 1) / 2, 4.5f - (sin(g_scale) + 1) / 2);	//���s�ړ�

	D3DXMatrixTranslation(&mtxTrans[3], -4.5f, g_fall[0], 4.5f);				//���s�ړ�
	D3DXMatrixTranslation(&mtxTrans[4], -4.5f, g_fall[1], 4.5f);				//���s�ړ�
	D3DXMatrixTranslation(&mtxTrans[5], -4.5f, g_fall[2], 4.5f);				//���s�ړ�
	D3DXMatrixTranslation(&mtxTrans[6], -4.5f, g_fall[3], 4.5f);				//���s�ړ�
	D3DXMatrixTranslation(&mtxTrans[7], -4.5f, g_fall[4], 4.5f);				//���s�ړ�

	//DebugFont_Draw(0, 32, "sin�␳�l = %.02lf", (sin(g_scale) + 1) / 2);

	g_scale += 0.05f;

	//x��0�`9��z��0�̊Ԃ͍��ړ�
	//x��9��z��0�`9�̊Ԃ͏�ړ�
	//X��9�`0��z��0�̊Ԃ͉E�ړ�
	//x��0��z��9�`0�̊Ԃ͉��ړ�

	mtxWorld[0] = mtxScaling[0] * mtxRotation[0][0] * mtxRotation[0][1] * mtxRotation[0][2] * mtxTrans[0];		//�s��̍���
	mtxWorld[1] = mtxScaling[0] * mtxRotation[1][1] * mtxTrans[1];												//�s��̍���
	mtxWorld[2] = mtxScaling[1] * mtxRotation[0][0] * mtxRotation[0][1] * mtxRotation[0][2] * mtxTrans[2];		//�s��̍���

	mtxWorld[3] = mtxScaling[0] * mtxRotation[0][0] * mtxRotation[0][1] * mtxRotation[0][2] * mtxTrans[3];		//�s��̍���
	mtxWorld[4] = mtxScaling[0] * mtxRotation[0][0] * mtxRotation[0][1] * mtxRotation[0][2] * mtxTrans[4];		//�s��̍���
	mtxWorld[5] = mtxScaling[0] * mtxRotation[0][0] * mtxRotation[0][1] * mtxRotation[0][2] * mtxTrans[5];		//�s��̍���
	mtxWorld[6] = mtxScaling[0] * mtxRotation[0][0] * mtxRotation[0][1] * mtxRotation[0][2] * mtxTrans[6];		//�s��̍���
	mtxWorld[7] = mtxScaling[0] * mtxRotation[0][0] * mtxRotation[0][1] * mtxRotation[0][2] * mtxTrans[7];		//�s��̍���


	g_angle += 2.0f;

	switch (g_state)
	{
	case 0:
		g_pos.x -= 0.1f;
		if (g_pos.x < -4.4f)
		{
			g_state = 1;
		}
		break;
	case 1:
		g_pos.z += 0.1f;
		if (g_pos.z > 4.4f)
		{
			g_state = 2;
		}
		break;
	case 2:
		g_pos.x += 0.1f;
		if (g_pos.x > 4.4f)
		{
			g_state = 3;
		}
		break;
	case 3:
		g_pos.z -= 0.1f;
		if (g_pos.z < -4.4f)
		{
			g_state = 0;
		}
		break;
	}


	//�ςޏ���
	//1��
	if (g_fall[0] >= 0.5f)
	{
		g_fall[0] -= 0.1f;
	}
	//2��
	else if (g_fall[1] >= 1.4f)
	{
		g_fall[1] -= 0.1f;
	}
	//3��
	else if (g_fall[2] >= 2.3f)
	{
		g_fall[2] -= 0.1f;
	}
	//4��
	else if (g_fall[3] >= 3.2f)
	{
		g_fall[3] -= 0.1f;
	}
	//5��
	else if (g_fall[4] >= 4.1f)
	{
		g_fall[4] -= 0.1f;
		if (g_fall[4] < 4.1f)
		{
			for (int i = 0; i < 5; i++)
			{
				g_fall[i] = 10.0f;
			}
		}
	}

	Cube_Draw(&mtxWorld[0]);
	Cube_Draw(&mtxWorld[1]);
	Cube_Draw(&mtxWorld[2]);
	Cube_Draw(&mtxWorld[3]);	//1
	Cube_Draw(&mtxWorld[4]);	//2
	Cube_Draw(&mtxWorld[5]);	//3
	Cube_Draw(&mtxWorld[6]);	//4
	Cube_Draw(&mtxWorld[7]);	//5
}