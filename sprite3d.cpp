#include <d3d9.h>
#include <d3dx9.h>
#include "texture.h"
#include "mydirectx.h"

//=====================================================
//�O���[�o���ϐ�
//=====================================================
static D3DCOLOR g_color = D3DCOLOR_RGBA(255, 255, 255, 255);

//====================================================
//�\���̐錾
//====================================================
typedef struct
{
	D3DXVECTOR3 position;	//���_���W
	D3DCOLOR color;			//���_�̐F���
	D3DXVECTOR2 UV;			//uv���W(texcoord)
}Vertex2d;
#define FVF_VERTEX3D (D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX1)	//x,y,z,RHW�������Ē��_�f�[�^��\��

//====================================================
//�|���S���`��(�ʏ�)
//====================================================
void Sprite3d_Draw(int texId, D3DXMATRIX mtxWorld)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	Vertex2d v[32] =
	{
		{ D3DXVECTOR3(-0.5f, 0.5f, 0.0f),g_color, D3DXVECTOR2(0, 0) },
		{ D3DXVECTOR3(0.5f, 0.5f, 0.0f),g_color, D3DXVECTOR2(1.0f, 0) },
		{ D3DXVECTOR3(-0.5f, -0.5f, 0.0f),g_color, D3DXVECTOR2(0.0f, 1.0f) },
		{ D3DXVECTOR3(0.5f, -0.5f, 0.0f),g_color, D3DXVECTOR2(1.0f,1.0f) },
	};
	pDevice->SetTransform(D3DTS_WORLD, &mtxWorld);
	pDevice->SetFVF(FVF_VERTEX3D);			//�f�o�C�X�ɒ��_�f�[�^��n��
	pDevice->SetTexture(0, Texture_GetTexture(texId));	//�e�N�X�`�����f�o�C�X�ɓn��
	pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, v, sizeof(Vertex2d));
}


//====================================================
//�|���S���`��(UV�l)
//====================================================
void Sprite3d_Draw(int texId, D3DXMATRIX mtxWorld, int cx, int cy, int cw, int ch)
{
	//���_�f�[�^
	int w = Texture_GetWidth(texId);
	int h = Texture_GetHeight(texId);

	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	float u0 = cx / (float)w;
	float v0 = cy / (float)h;
	float u1 = (cx + cw) / (float)w;
	float v1 = (cy + ch) / (float)h;

	Vertex2d v[32] =
	{
	{D3DXVECTOR3(-0.5f, 0.5f, 0.0f), D3DCOLOR_RGBA(255, 255, 255, 255) , D3DXVECTOR2(u0, v0)},
	{D3DXVECTOR3(0.5f, 0.5f, 0.0f), D3DCOLOR_RGBA(255, 255, 255, 255)  , D3DXVECTOR2(u1, v0)},
	{D3DXVECTOR3(-0.5f, -0.5f, 0.0f), D3DCOLOR_RGBA(255, 255, 255, 255), D3DXVECTOR2(u0, v1)},
	{D3DXVECTOR3(0.5f, -0.5f, 0.0f), D3DCOLOR_RGBA(255, 255, 255, 255) , D3DXVECTOR2(u1, v1)}
	};
	pDevice->SetTransform(D3DTS_WORLD, &mtxWorld);
	pDevice->SetFVF(FVF_VERTEX3D);			//�f�o�C�X�ɒ��_�f�[�^��n��
	pDevice->SetTexture(0, Texture_GetTexture(texId));	//�e�N�X�`�����f�o�C�X�ɓn��
	pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, v, sizeof(Vertex2d));
}