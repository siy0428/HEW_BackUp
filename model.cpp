#include <string.h>
#include "mydirectx.h"
#include "texture.h"
#include "model.h"

//=====================================================
//�}�N����`
//=====================================================
#define MODEL_MAX (32)	//���f���̍ő吔

//=====================================================
//�\����
//=====================================================
typedef struct
{
	char filename[256];
	LPD3DXMESH pMesh;
	DWORD MaterialCount;
	LPD3DXBUFFER pMaterial;
	int *pTextureIds;
	D3DXVECTOR3 pos;
}MODEL;

//=====================================================
//�O���[�o���ϐ�
//=====================================================
static MODEL g_Model[MODEL_MAX] = {};

static LPD3DXMESH land_mesh;

//=====================================================
//������
//=====================================================
int Model_Load(const char *filename, const char *path_name)
{
	//���łɂ������ꍇ
	for (int i = 0; i < MODEL_MAX; i++)
	{
		if (strcmp(g_Model[i].filename, filename) == 0)
		{
			return i;
		}
	}
	//�V�K�o�^
	for (int i = 0; i < MODEL_MAX; i++)
	{
		if (g_Model[i].filename[0] != '\0')
		{
			continue;
		}
		strcpy(g_Model[i].filename, filename);
		//�f�o�C�X�̃|�C���^�擾
		LPDIRECT3DDEVICE9 pDevice = GetDevice();
		g_Model[i].pTextureIds = new int[g_Model[i].MaterialCount];
		HRESULT hr = D3DXLoadMeshFromX(filename, D3DXMESH_MANAGED, pDevice, NULL, &g_Model[i].pMaterial, NULL, &g_Model[i].MaterialCount, &g_Model[i].pMesh);	//| D3DXMESH_32BIT
		//�G���[����
		if (FAILED(hr))
		{
			MessageBox(NULL, "���f�����ǂݍ��߂܂���ł���", "�G���[", MB_OK);
			return -1;
		}

		D3DXMATERIAL *pMat = (D3DXMATERIAL*)g_Model[i].pMaterial->GetBufferPointer();
		//�e�}�e���A�����̎擾
		for (DWORD j = 0; j < g_Model[i].MaterialCount; j++)
		{
			//�F
			pMat[j].MatD3D.Diffuse.r = 1.0f;
			pMat[j].MatD3D.Diffuse.g = 1.0f;
			pMat[j].MatD3D.Diffuse.b = 1.0f;
			pMat[j].MatD3D.Diffuse.a = 1.0f;

			pMat[j].MatD3D.Ambient.r = 1.0f;
			pMat[j].MatD3D.Ambient.g = 1.0f;
			pMat[j].MatD3D.Ambient.b = 1.0f;
			pMat[j].MatD3D.Ambient.a = 1.0f;
			//�e�N�X�`���̓ǂݍ���
			if (pMat[j].pTextureFilename == NULL)
			{
				g_Model[i].pTextureIds[j] = -1;
			}
			//�e�N�X�`���L��
			else
			{
				//�t�@�C�����̉��H(�p�X�̒ǉ�)
				char buf[256];
				strcpy(buf, path_name);
				strcat(buf, pMat[j].pTextureFilename);
				g_Model[i].pTextureIds[j] = Texture_SetLoadFile(buf, 0, 0);
			}
		}
		return i;
	}
	return -1;
}

//=====================================================
//�I��
//=====================================================
void Model_Uninit(void)
{
	for (int i = 0; i < MODEL_MAX; i++)
	{
		g_Model[i].pMesh->Release();
		g_Model[i].pMesh = NULL;
		g_Model[i].pMaterial->Release();
		g_Model[i].pMaterial = NULL;
		g_Model[i].pTextureIds = NULL;
		delete[] g_Model[i].pTextureIds;
	}
}

//=====================================================
//�`��
//=====================================================
void Model_Draw(int modelId, D3DMATRIX mtxWorld)
{
	//�f�o�C�X�̃|�C���^�擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//�}�e���A���̌������[�v
	for (DWORD i = 0; i < g_Model[modelId].MaterialCount; i++)
	{
		//�}�e���A���̐ݒ�
		if (g_Model[modelId].pTextureIds[i] < 0)	//�e�N�X�`�����\���Ă��Ȃ��ꍇ
		{
			D3DXMATERIAL *pMat = (D3DXMATERIAL*)g_Model[modelId].pMaterial->GetBufferPointer();

			//�A���r�G���g�ƃf�B�t���[�Y�J���[�̐ݒ�
			pMat[i].MatD3D.Ambient = pMat[i].MatD3D.Diffuse;

			//�}�e���A���A�e�N�X�`���̐ݒ�
			pDevice->SetMaterial(&pMat[i].MatD3D);
			pDevice->SetTexture(0, NULL);
		}
		else
		{
			//�}�e���A���J���[�𔒁A�e�N�X�`����\��
			pDevice->SetTexture(0, Texture_GetTexture(g_Model[modelId].pTextureIds[i]));
		}

		pDevice->SetTransform(D3DTS_WORLD, &mtxWorld);
		//���b�V��������
		g_Model[modelId].pMesh->DrawSubset(i);
	}
}

//=====================================================
//�`��(�F�w��)
//=====================================================
void Model_Draw(int modelId, D3DMATRIX mtxWorld, D3DXCOLOR color)
{
	//�f�o�C�X�̃|�C���^�擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//�}�e���A���̌������[�v
	for (DWORD i = 0; i < g_Model[modelId].MaterialCount; i++)
	{
		//�}�e���A���̐ݒ�
		if (g_Model[modelId].pTextureIds[i] < 0)	//�e�N�X�`�����\���Ă��Ȃ��ꍇ
		{
			pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

			//�}�e���A���ݒ�
			D3DMATERIAL9 mat = {};
			mat.Diffuse.r = color.r;
			mat.Diffuse.g = color.g;
			mat.Diffuse.b = color.b;
			mat.Diffuse.a = color.a;

			mat.Ambient = mat.Diffuse;

			pDevice->SetMaterial(&mat);
		}
		else
		{
			//�}�e���A���J���[�𔒁A�e�N�X�`����\��
			pDevice->SetTexture(0, Texture_GetTexture(g_Model[modelId].pTextureIds[i]));
		}

		pDevice->SetTransform(D3DTS_WORLD, &mtxWorld);
		//���b�V��������
		g_Model[modelId].pMesh->DrawSubset(i);
	}
}


void SetStage(int modelId)
{
	land_mesh = g_Model[modelId].pMesh;

	return;
}

D3DXVECTOR3 collisionNormal(const D3DXVECTOR3* position, BOOL *hit, float* distance, bool* x, bool* z)
{
	D3DXVECTOR3 under = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	float distance1, distance2, distance1_xm, distance2_zm;
	BOOL hit1, hit2;
	float lag = 0.1f;
	D3DXVECTOR3 position1 = *position;
	position1.x += lag;
	D3DXVECTOR3 position1_xm = *position;
	position1_xm.x -= lag;
	D3DXVECTOR3 position2 = *position;
	position2.z += lag;
	D3DXVECTOR3 position2_zm = *position;
	position2_zm.z -= lag;


	D3DXIntersect(land_mesh, position, &under, hit, NULL, NULL, NULL, distance, NULL, NULL);
	D3DXIntersect(land_mesh, &position1, &under, &hit1, NULL, NULL, NULL, &distance1, NULL, NULL);
	D3DXIntersect(land_mesh, &position1_xm, &under, &hit1, NULL, NULL, NULL, &distance1_xm, NULL, NULL);
	D3DXIntersect(land_mesh, &position2, &under, &hit2, NULL, NULL, NULL, &distance2, NULL, NULL);
	D3DXIntersect(land_mesh, &position2_zm, &under, &hit2, NULL, NULL, NULL, &distance2_zm, NULL, NULL);

	*x = !(sign(*distance, distance1) ^ sign(*distance, distance1_xm));
	*z = !(sign(*distance, distance2) ^ sign(*distance, distance2_zm));

	D3DXVECTOR3 VecA, VecB, Normal;
	VecA = { lag , distance1 - *distance,  0.0f };
	VecB = { 0.0f , distance2 - *distance,lag };
	D3DXVec3Cross(&Normal, &VecA, &VecB);
	if (Normal.y < 0.0f) {
		Normal = -Normal;
	}


	//return distance,hit;
	return Normal;
}


bool sign(float a, float b) 
{
	if (a >= b)return true;
	return false;
}

D3DXMATRIX VectorMatrix(D3DXVECTOR3 vector)
{
	D3DXMATRIX transform, ret;
	D3DXVECTOR3 v_transform;
	float angle = 90 * (D3DX_PI / 180);

	D3DXMatrixIdentity(&transform);

	transform._21 = vector.x;
	transform._22 = vector.y;
	transform._23 = vector.z;

	//x
	D3DXMatrixRotationZ(&ret, -angle);
	//v_transform = vector * ret;
	D3DXVec3TransformCoord(&v_transform, &vector, &ret);
	transform._11 = v_transform.x;
	transform._12 = v_transform.y;
	transform._13 = v_transform.z;

	//z
	D3DXVec3Cross(&v_transform, &v_transform, &vector);
	//D3DXVec3TransformCoord(&v_transform, &vector, &ret);
	transform._31 = v_transform.x;
	transform._32 = v_transform.y;
	transform._33 = v_transform.z;

	return transform;
}