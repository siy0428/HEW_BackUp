#include "mydirectx.h"
#include <d3dx9.h>
#include "texture.h"

//====================================================
//�}�N����`
//====================================================
#define WIDTH  (100)	//1�ȏ�
#define HEIGHT (100)	//1�ȏ�
#define VERTEX_NUM ((WIDTH + 1) * (HEIGHT + 1))		//���_��
#define INDEX_NUM (VERTEX_NUM + (HEIGHT - 1) * (WIDTH + 1) + (HEIGHT - 1) * 2)	//�C���f�b�N�X��

//====================================================
//�\���̐錾
//====================================================
typedef struct Mesh_Vertex_tag
{
	D3DXVECTOR3 position;	//���W
	D3DXVECTOR3 normal;		//�@���x�N�g��
	D3DCOLOR color;			//�F���
	D3DXVECTOR2 UV;			//�e�N�X�`��
}MeshVertex;
#define FVF_MESH (D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_NORMAL | D3DFVF_TEX1)

//====================================================
//�O���[�o���ϐ�
//====================================================
static LPDIRECT3DVERTEXBUFFER9 lpVtxBuf;   // ���_�o�b�t�@
static LPDIRECT3DINDEXBUFFER9 lpIdxBuf;    // ���_�C���f�b�N�X�o�b�t�@
static unsigned int g_tex = NULL;

//=====================================================
//������
//=====================================================
void Mesh_Init(void)
{
	//�f�o�C�X�̃|�C���^�擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//g_tex = Texture_SetLoadFile("Texture\\f_mesh.jpg", 1300, 866);
	g_tex = Texture_SetLoadFile("Texture\\ice_field.jpg", 612, 612);

	pDevice->CreateVertexBuffer(sizeof(MeshVertex) * VERTEX_NUM, 0, (D3DFORMAT)FVF_MESH, D3DPOOL_MANAGED, &lpVtxBuf, NULL);

	// ���b�N
	MeshVertex *vbuf;
	lpVtxBuf->Lock(0, 0, (void**)(&vbuf), D3DLOCK_NOSYSLOCK);

	//���_�f�[�^������
	for (int i = 0; i < VERTEX_NUM; i++)
	{
		vbuf[i].position = D3DXVECTOR3((FLOAT)(i % (WIDTH + 1)) + -WIDTH / 2, 0.0f, (FLOAT)(-i / (WIDTH + 1)) + HEIGHT / 2);
		vbuf[i].normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		vbuf[i].color = D3DCOLOR_RGBA(255, 255, 255, 255);
		vbuf[i].UV = D3DXVECTOR2((FLOAT)(i % (WIDTH + 1)) + -WIDTH / 2, (FLOAT)(-i / (WIDTH + 1)) + HEIGHT / 2);
	}
	// ���
	lpVtxBuf->Unlock();

	pDevice->CreateIndexBuffer(INDEX_NUM * sizeof(MeshVertex), 0, D3DFMT_INDEX16, D3DPOOL_MANAGED, &lpIdxBuf, NULL);

	// �m�ۂ��钸�_�C���f�b�N�X�o�b�t�@��錾
	unsigned short*    ibuf;
	// ���b�N
	lpIdxBuf->Lock(0, 0, (void**)(&ibuf), D3DLOCK_NOSYSLOCK);

	// ���_�C���f�b�N�X�̊m��
	unsigned short index_num = 0;	//�k�ރ|���S���̐ݒ肪�s��ꂽ��
	for (unsigned short i = 0; i < INDEX_NUM / 2; i++)
	{
		//�k�ރ|���S��
		if ((WIDTH + 1) + (WIDTH + 2) * index_num == i && i != 0 && i != INDEX_NUM / 2 && HEIGHT > 1)
		{
			ibuf[i + i] = i - 1 - index_num;
			ibuf[i + i + 1] = (WIDTH + 1) + i - index_num;
			index_num++;
		}
		//�ʏ�C���f�b�N�X
		else
		{
			ibuf[i + i] = (WIDTH + 1) + (i - index_num);
			ibuf[i + i + 1] = (i - index_num);
		}
	}

	// ���
	lpIdxBuf->Unlock();

	// �f�o�C�X�ɃC���f�b�N�X���Z�b�g
	pDevice->SetIndices(lpIdxBuf);
}

//=====================================================
//�I��
//=====================================================
void Mesh_Uninit(void)
{
	if (lpVtxBuf)
	{
		lpVtxBuf->Release();
		lpVtxBuf = NULL;
	}
	if (lpIdxBuf)
	{
		lpIdxBuf->Release();
		lpIdxBuf = NULL;
	}
}

//=====================================================
//�`��
//=====================================================
void Mesh_Draw(void)
{
	//�f�o�C�X�̃|�C���^�擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//���O�̍s��̏�����
	D3DXMATRIX mtxWorld;
	D3DXMatrixIdentity(&mtxWorld);
	pDevice->SetTransform(D3DTS_WORLD, &mtxWorld);

	//pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
	//���C�e�B���O�ݒ�
	pDevice->SetRenderState(D3DRS_DIFFUSEMATERIALSOURCE, D3DMCS_COLOR1);	//D3DMCS_COLOR1�͒��_�J���[, D3DMCS_MATERIAL�̓��f���J���[
	pDevice->SetRenderState(D3DRS_NORMALIZENORMALS, TRUE);					//(���K��, ���邩���Ȃ���)
	//�A���r�G���g���C�g
	pDevice->SetRenderState(D3DRS_AMBIENT, D3DCOLOR_XRGB(167, 98, 255));
	pDevice->SetRenderState(D3DRS_AMBIENTMATERIALSOURCE, D3DMCS_COLOR1);

	//���b�s���O���[�h�̎w��
	pDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
	pDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);
	//�`��ݒ�
	pDevice->SetStreamSource(0, lpVtxBuf, 0, sizeof(MeshVertex));
	pDevice->SetIndices(lpIdxBuf);
	pDevice->SetFVF(FVF_MESH);								//�f�o�C�X�ɒ��_�f�[�^��n��
	pDevice->SetTexture(0, Texture_GetTexture(g_tex));							//�e�N�X�`�����f�o�C�X�ɓn��
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);			//FALSE:���C�gOFF TRUE:���C�gON
	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0, VERTEX_NUM - 2, 0, INDEX_NUM - 2);

	//���C�e�B���O�ݒ�
	pDevice->SetRenderState(D3DRS_DIFFUSEMATERIALSOURCE, D3DMCS_MATERIAL);	//D3DMCS_COLOR1�͒��_�J���[, D3DMCS_MATERIAL�̓��f���J���[
	pDevice->SetRenderState(D3DRS_NORMALIZENORMALS, TRUE);					//(���K��, ���邩���Ȃ���)
	//�A���r�G���g���C�g
	pDevice->SetRenderState(D3DRS_AMBIENT, D3DCOLOR_XRGB(167, 98, 100));
	pDevice->SetRenderState(D3DRS_AMBIENTMATERIALSOURCE, D3DMCS_MATERIAL);
}