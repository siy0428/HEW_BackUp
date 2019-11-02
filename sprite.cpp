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
	D3DXVECTOR4 position;	//���_���W
	D3DCOLOR color;			//���_�̐F���
	D3DXVECTOR2 UV;			//uv���W(texcoord)
}Vertex2d;
#define FVF_VERTEX2D (D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1)	//x,y,z,RHW�������Ē��_�f�[�^��\��

//====================================================
//�|���S���`��(�ʏ�)
//====================================================
void Sprite_Draw(int texId, float dx, float dy)
{
	//���_�f�[�^
	int w = Texture_GetWidth(texId);
	int h = Texture_GetHeight(texId);

	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	Vertex2d v[32] =
	{
		{ D3DXVECTOR4(dx - 0.5f, dy - 0.5f, 0.0f, 1.0f),g_color, D3DXVECTOR2(0, 0) },
		{ D3DXVECTOR4(dx + w - 0.5f, dy - 0.5f, 0.0f, 1.0f),g_color, D3DXVECTOR2(1.0f, 0) },
		{ D3DXVECTOR4(dx - 0.5f, dy + h - 0.5f, 0.0f, 1.0f),g_color, D3DXVECTOR2(0.0f, 1.0f) },
		{ D3DXVECTOR4(dx + w - 0.5f, dy + h - 0.5f, 0.0f, 1.0f),g_color, D3DXVECTOR2(1.0f,1.0f) },
	};
	pDevice->SetFVF(FVF_VERTEX2D);			//�f�o�C�X�ɒ��_�f�[�^��n��
	pDevice->SetTexture(0, Texture_GetTexture(texId));	//�e�N�X�`�����f�o�C�X�ɓn��
	pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, v, sizeof(Vertex2d));//�}�`�̕`���(�h��Ԃ�), �}�`�̐�, ���_�f�[�^�̐擪�A�h���X, ���_�f�[�^�̃T�C�Y
																		  //D3DPT_TRIANGLELIST�ɂ���ƎO�p�`�̕`��
																		  //D3DPT_POINTLIST�ɂ���Ɠ_�̕`��i�_1�Ő}�`1���Ƃ݂Ȃ��j
																		  //D3DPT_LINELIST�ɂ����2���_��1���̕`��
																		  //D3DPT_LINESTRIP�ɂ���ƘA�����Đ���`��
																		  //D3DPT_TRIANGLESTRIP�ɂ���Ǝl�p�`��`��(�O�p�`�̐���������ɓ����)
																		  //D3DPT_TRIANGLEFAN��`
																		  //�~�`��
}

//====================================================
//�|���S���`��(�ʏ�)(�����x�ݒ�)
//====================================================
void Sprite_Draw(int texId, float dx, float dy, int alpha)
{
	//���_�f�[�^
	int w = Texture_GetWidth(texId);
	int h = Texture_GetHeight(texId);

	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	Vertex2d v[32] =
	{
	{ D3DXVECTOR4(dx - 0.5f, dy - 0.5f, 0.0f, 1.0f),D3DCOLOR_RGBA(255, 255, 255, alpha), D3DXVECTOR2(0, 0) },
	{ D3DXVECTOR4(dx + w - 0.5f, dy - 0.5f, 0.0f, 1.0f),D3DCOLOR_RGBA(255, 255, 255, alpha), D3DXVECTOR2(1.0f, 0) },
	{ D3DXVECTOR4(dx - 0.5f, dy + h - 0.5f, 0.0f, 1.0f),D3DCOLOR_RGBA(255, 255, 255, alpha), D3DXVECTOR2(0.0f, 1.0f) },
	{ D3DXVECTOR4(dx + w - 0.5f, dy + h - 0.5f, 0.0f, 1.0f),D3DCOLOR_RGBA(255, 255, 255, alpha), D3DXVECTOR2(1.0f,1.0f) },
	};
	pDevice->SetFVF(FVF_VERTEX2D);			//�f�o�C�X�ɒ��_�f�[�^��n��
	pDevice->SetTexture(0, Texture_GetTexture(texId));	//�e�N�X�`�����f�o�C�X�ɓn��
	pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, v, sizeof(Vertex2d));//�}�`�̕`���(�h��Ԃ�), �}�`�̐�, ���_�f�[�^�̐擪�A�h���X, ���_�f�[�^�̃T�C�Y
																		  //D3DPT_TRIANGLELIST�ɂ���ƎO�p�`�̕`��
																		  //D3DPT_POINTLIST�ɂ���Ɠ_�̕`��i�_1�Ő}�`1���Ƃ݂Ȃ��j
																		  //D3DPT_LINELIST�ɂ����2���_��1���̕`��
																		  //D3DPT_LINESTRIP�ɂ���ƘA�����Đ���`��
																		  //D3DPT_TRIANGLESTRIP�ɂ���Ǝl�p�`��`��(�O�p�`�̐���������ɓ����)
																		  //D3DPT_TRIANGLEFAN��`
																		  //�~�`��
}

//====================================================
//�|���S���`��(UV�l)
//====================================================
void Sprite_Draw(int texId, float dx, float dy, int cx, int cy, int cw, int ch)
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
		{ D3DXVECTOR4(dx - 0.5f, dy - 0.5f, 0.0f, 1.0f),g_color, D3DXVECTOR2(u0, v0) },
		{ D3DXVECTOR4(dx + cw - 0.5f, dy - 0.5f, 0.0f, 1.0f),g_color, D3DXVECTOR2(u1, v0) },
		{ D3DXVECTOR4(dx - 0.5f, dy + ch - 0.5f, 0.0f, 1.0f),g_color, D3DXVECTOR2(u0, v1) },
		{ D3DXVECTOR4(dx + cw - 0.5f, dy + ch - 0.5f, 0.0f, 1.0f),g_color, D3DXVECTOR2(u1,v1) },
	};
	pDevice->SetFVF(FVF_VERTEX2D);			//�f�o�C�X�ɒ��_�f�[�^��n��
	pDevice->SetTexture(0, Texture_GetTexture(texId));	//�e�N�X�`�����f�o�C�X�ɓn��
	pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, v, sizeof(Vertex2d));//�}�`�̕`���(�h��Ԃ�), �}�`�̐�, ���_�f�[�^�̐擪�A�h���X, ���_�f�[�^�̃T�C�Y
																		  //D3DPT_TRIANGLELIST�ɂ���ƎO�p�`�̕`��
																		  //D3DPT_POINTLIST�ɂ���Ɠ_�̕`��i�_1�Ő}�`1���Ƃ݂Ȃ��j
																		  //D3DPT_LINELIST�ɂ����2���_��1���̕`��
																		  //D3DPT_LINESTRIP�ɂ���ƘA�����Đ���`��
																		  //D3DPT_TRIANGLESTRIP�ɂ���Ǝl�p�`��`��(�O�p�`�̐���������ɓ����)
																		  //D3DPT_TRIANGLEFAN��`
																		  //�~�`��
}

//====================================================
//�|���S���`��(UV�l)(�����x�ݒ�)
//====================================================
void Sprite_Draw(int texId, float dx, float dy, int cx, int cy, int cw, int ch, int alpha)
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
		{ D3DXVECTOR4(dx - 0.5f, dy - 0.5f, 0.0f, 1.0f),D3DCOLOR_RGBA(255, 255, 255, alpha), D3DXVECTOR2(u0, v0) },
	{ D3DXVECTOR4(dx + cw - 0.5f, dy - 0.5f, 0.0f, 1.0f),D3DCOLOR_RGBA(255, 255, 255, alpha), D3DXVECTOR2(u1, v0) },
	{ D3DXVECTOR4(dx - 0.5f, dy + ch - 0.5f, 0.0f, 1.0f),D3DCOLOR_RGBA(255, 255, 255, alpha), D3DXVECTOR2(u0, v1) },
	{ D3DXVECTOR4(dx + cw - 0.5f, dy + ch - 0.5f, 0.0f, 1.0f),D3DCOLOR_RGBA(255, 255, 255, alpha), D3DXVECTOR2(u1,v1) },
	};
	pDevice->SetFVF(FVF_VERTEX2D);			//�f�o�C�X�ɒ��_�f�[�^��n��
	pDevice->SetTexture(0, Texture_GetTexture(texId));	//�e�N�X�`�����f�o�C�X�ɓn��
	pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, v, sizeof(Vertex2d));//�}�`�̕`���(�h��Ԃ�), �}�`�̐�, ���_�f�[�^�̐擪�A�h���X, ���_�f�[�^�̃T�C�Y
																		  //D3DPT_TRIANGLELIST�ɂ���ƎO�p�`�̕`��
																		  //D3DPT_POINTLIST�ɂ���Ɠ_�̕`��i�_1�Ő}�`1���Ƃ݂Ȃ��j
																		  //D3DPT_LINELIST�ɂ����2���_��1���̕`��
																		  //D3DPT_LINESTRIP�ɂ���ƘA�����Đ���`��
																		  //D3DPT_TRIANGLESTRIP�ɂ���Ǝl�p�`��`��(�O�p�`�̐���������ɓ����)
																		  //D3DPT_TRIANGLEFAN��`
																		  //�~�`��
}

//====================================================
//�|���S���`��(��])(�g��)
//====================================================
void Sprite_Draw(int texId, float dx, float dy, int cx, int cy, int cw, int ch, float angle, float center_x, float center_y, float zoom)
{
	//���_�f�[�^
	int w = Texture_GetWidth(texId);
	int h = Texture_GetHeight(texId);

	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxW, mtxR, mtxT, mtxIT, mtxS;
	D3DXMatrixScaling(&mtxS, 1.0f + zoom, 1.0f + zoom, 1.0f + zoom);	//�s��̃|�C���^,x,y,z
	D3DXMatrixTranslation(&mtxT, -center_x - dx, -center_y - dy, 0);	//���s�ړ�
	D3DXMatrixTranslation(&mtxIT, center_x + dx, center_y + dy, 0);	//���̏ꏊ�Ɉړ�
	D3DXMatrixRotationZ(&mtxR, angle);						//��]
	mtxW = mtxT * mtxS * mtxR * mtxIT;

	float u0 = cx / (float)w;
	float v0 = cy / (float)h;
	float u1 = (cx + cw) / (float)w;
	float v1 = (cy + ch) / (float)h;

	Vertex2d v[32] =
	{
		{ D3DXVECTOR4(dx - 0.5f, dy - 0.5f, 0.0f, 1.0f),g_color, D3DXVECTOR2(u0, v0) },
		{ D3DXVECTOR4(dx + cw - 0.5f, dy - 0.5f, 0.0f, 1.0f),g_color, D3DXVECTOR2(u1, v0) },
		{ D3DXVECTOR4(dx - 0.5f, dy + ch - 0.5f, 0.0f, 1.0f),g_color, D3DXVECTOR2(u0, v1) },
		{ D3DXVECTOR4(dx + cw - 0.5f, dy + ch - 0.5f, 0.0f, 1.0f),g_color, D3DXVECTOR2(u1,v1) },
	};

	for (int i = 0; i < 4; i++)
	{
		D3DXVec4Transform(&v[i].position, &v[i].position, &mtxW);
	}

	pDevice->SetFVF(FVF_VERTEX2D);						//�f�o�C�X�ɒ��_�f�[�^��n��
	pDevice->SetTexture(0, Texture_GetTexture(texId));	//�e�N�X�`�����f�o�C�X�ɓn��
	pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, v, sizeof(Vertex2d));//�}�`�̕`���(�h��Ԃ�), �}�`�̐�, ���_�f�[�^�̐擪�A�h���X, ���_�f�[�^�̃T�C�Y
																		  //D3DPT_TRIANGLELIST�ɂ���ƎO�p�`�̕`��
																		  //D3DPT_POINTLIST�ɂ���Ɠ_�̕`��i�_1�Ő}�`1���Ƃ݂Ȃ��j
																		  //D3DPT_LINELIST�ɂ����2���_��1���̕`��
																		  //D3DPT_LINESTRIP�ɂ���ƘA�����Đ���`��
																		  //D3DPT_TRIANGLESTRIP�ɂ���Ǝl�p�`��`��(�O�p�`�̐���������ɓ����)
																		  //D3DPT_TRIANGLEFAN��`
																		  //�~�`��
}

//====================================================
//�|���S���`��(��])(�g��)(�F�w��)
//====================================================
void Sprite_Draw(int texId, float dx, float dy, int cx, int cy, int cw, int ch, float angle, float center_x, float center_y, float zoom, D3DXCOLOR color)
{
	//���_�f�[�^
	int w = Texture_GetWidth(texId);
	int h = Texture_GetHeight(texId);

	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxW, mtxR, mtxT, mtxIT, mtxS;
	D3DXMatrixScaling(&mtxS, 1.0f + zoom, 1.0f + zoom, 1.0f + zoom);	//�s��̃|�C���^,x,y,z
	D3DXMatrixTranslation(&mtxT, -center_x - dx, -center_y - dy, 0);	//���s�ړ�
	D3DXMatrixTranslation(&mtxIT, center_x + dx, center_y + dy, 0);	//���̏ꏊ�Ɉړ�
	D3DXMatrixRotationZ(&mtxR, angle);						//��]
	mtxW = mtxT * mtxS * mtxR * mtxIT;

	float u0 = cx / (float)w;
	float v0 = cy / (float)h;
	float u1 = (cx + cw) / (float)w;
	float v1 = (cy + ch) / (float)h;

	Vertex2d v[32] =
	{
		{ D3DXVECTOR4(dx - 0.5f, dy - 0.5f, 0.0f, 1.0f),color, D3DXVECTOR2(u0, v0) },
	{ D3DXVECTOR4(dx + cw - 0.5f, dy - 0.5f, 0.0f, 1.0f),color, D3DXVECTOR2(u1, v0) },
	{ D3DXVECTOR4(dx - 0.5f, dy + ch - 0.5f, 0.0f, 1.0f),color, D3DXVECTOR2(u0, v1) },
	{ D3DXVECTOR4(dx + cw - 0.5f, dy + ch - 0.5f, 0.0f, 1.0f),color, D3DXVECTOR2(u1,v1) },
	};

	for (int i = 0; i < 4; i++)
	{
		D3DXVec4Transform(&v[i].position, &v[i].position, &mtxW);
	}

	pDevice->SetFVF(FVF_VERTEX2D);						//�f�o�C�X�ɒ��_�f�[�^��n��
	pDevice->SetTexture(0, Texture_GetTexture(texId));	//�e�N�X�`�����f�o�C�X�ɓn��
	pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, v, sizeof(Vertex2d));//�}�`�̕`���(�h��Ԃ�), �}�`�̐�, ���_�f�[�^�̐擪�A�h���X, ���_�f�[�^�̃T�C�Y
																		  //D3DPT_TRIANGLELIST�ɂ���ƎO�p�`�̕`��
																		  //D3DPT_POINTLIST�ɂ���Ɠ_�̕`��i�_1�Ő}�`1���Ƃ݂Ȃ��j
																		  //D3DPT_LINELIST�ɂ����2���_��1���̕`��
																		  //D3DPT_LINESTRIP�ɂ���ƘA�����Đ���`��
																		  //D3DPT_TRIANGLESTRIP�ɂ���Ǝl�p�`��`��(�O�p�`�̐���������ɓ����)
																		  //D3DPT_TRIANGLEFAN��`
																		  //�~�`��
}

//====================================================
//�|���S���`��(UV�l)(���E���])
//====================================================
void Sprite_Reverse_Draw(int texId, float dx, float dy, int cx, int cy, int cw, int ch)
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
	{ D3DXVECTOR4(dx - 0.5f, dy - 0.5f, 0.0f, 1.0f),g_color, D3DXVECTOR2(u1, v0) },
	{ D3DXVECTOR4(dx + cw - 0.5f, dy - 0.5f, 0.0f, 1.0f),g_color, D3DXVECTOR2(u0, v0) },
	{ D3DXVECTOR4(dx - 0.5f, dy + ch - 0.5f, 0.0f, 1.0f),g_color, D3DXVECTOR2(u1, v1) },
	{ D3DXVECTOR4(dx + cw - 0.5f, dy + ch - 0.5f, 0.0f, 1.0f),g_color, D3DXVECTOR2(u0,v1) },
	};
	pDevice->SetFVF(FVF_VERTEX2D);			//�f�o�C�X�ɒ��_�f�[�^��n��
	pDevice->SetTexture(0, Texture_GetTexture(texId));	//�e�N�X�`�����f�o�C�X�ɓn��
	pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, v, sizeof(Vertex2d));//�}�`�̕`���(�h��Ԃ�), �}�`�̐�, ���_�f�[�^�̐擪�A�h���X, ���_�f�[�^�̃T�C�Y
																		  //D3DPT_TRIANGLELIST�ɂ���ƎO�p�`�̕`��
																		  //D3DPT_POINTLIST�ɂ���Ɠ_�̕`��i�_1�Ő}�`1���Ƃ݂Ȃ��j
																		  //D3DPT_LINELIST�ɂ����2���_��1���̕`��
																		  //D3DPT_LINESTRIP�ɂ���ƘA�����Đ���`��
																		  //D3DPT_TRIANGLESTRIP�ɂ���Ǝl�p�`��`��(�O�p�`�̐���������ɓ����)
																		  //D3DPT_TRIANGLEFAN��`
																		  //�~�`��
}

//====================================================
//�|���S���`��(UV�l)(���E���])
//====================================================
void Sprite_Upside_Draw(int texId, float dx, float dy, int cx, int cy, int cw, int ch)
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
	{ D3DXVECTOR4(dx - 0.5f, dy - 0.5f, 0.0f, 1.0f),g_color, D3DXVECTOR2(u0, v1) },
	{ D3DXVECTOR4(dx + cw - 0.5f, dy - 0.5f, 0.0f, 1.0f),g_color, D3DXVECTOR2(u1, v1) },
	{ D3DXVECTOR4(dx - 0.5f, dy + ch - 0.5f, 0.0f, 1.0f),g_color, D3DXVECTOR2(u0, v0) },
	{ D3DXVECTOR4(dx + cw - 0.5f, dy + ch - 0.5f, 0.0f, 1.0f),g_color, D3DXVECTOR2(u1,v0) },
	};
	pDevice->SetFVF(FVF_VERTEX2D);			//�f�o�C�X�ɒ��_�f�[�^��n��
	pDevice->SetTexture(0, Texture_GetTexture(texId));	//�e�N�X�`�����f�o�C�X�ɓn��
	pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, v, sizeof(Vertex2d));//�}�`�̕`���(�h��Ԃ�), �}�`�̐�, ���_�f�[�^�̐擪�A�h���X, ���_�f�[�^�̃T�C�Y
																		  //D3DPT_TRIANGLELIST�ɂ���ƎO�p�`�̕`��
																		  //D3DPT_POINTLIST�ɂ���Ɠ_�̕`��i�_1�Ő}�`1���Ƃ݂Ȃ��j
																		  //D3DPT_LINELIST�ɂ����2���_��1���̕`��
																		  //D3DPT_LINESTRIP�ɂ���ƘA�����Đ���`��
																		  //D3DPT_TRIANGLESTRIP�ɂ���Ǝl�p�`��`��(�O�p�`�̐���������ɓ����)
																		  //D3DPT_TRIANGLEFAN��`
																		  //�~�`��
}

void Sprite_SetColor(D3DXCOLOR color)
{
	g_color = color;
}
