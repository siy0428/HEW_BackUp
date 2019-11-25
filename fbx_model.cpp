#include <d3dx9.h>
#include "mydirectx.h"
#include "texture.h"
#include <string.h>
#include "fbx.h"

//=====================================================
//マクロ定義
//=====================================================
#define MODEL_MAX (8)	//モデルの最大数

//=====================================================
//構造体
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
//グローバル変数
//=====================================================
static MODEL g_Model[MODEL_MAX] = {};

//=====================================================
//初期化
//=====================================================
int Fbx_Model_Load(const char *filename, const char *path_name, float scale = 1.0f)
{
	//すでにあった場合
	for (int i = 0; i < MODEL_MAX; i++)
	{
		if (strcmp(g_Model[i].filename, filename) == 0)
		{
			return i;
		}
	}

	//新規登録
	for (int i = 0; i < MODEL_MAX; i++)
	{
		if (g_Model[i].filename[0] != '\0')
		{
			continue;
		}
		//デバイスのポインタ取得
		LPDIRECT3DDEVICE9 pDevice = GetDevice();
		HRESULT hr = D3DXLoadMeshFromFBX(filename, D3DXMESH_MANAGED, pDevice, NULL, &g_Model[i].pMaterial, NULL, &g_Model[i].MaterialCount, &g_Model[i].pMesh, scale);	//| D3DXMESH_32BIT
		//エラー処理
		if (FAILED(hr))
		{
			MessageBox(NULL, "モデルが読み込めませんでした", "エラー", MB_OK);
			return -1;
		}
		g_Model[i].pTextureIds = new int[g_Model[i].MaterialCount];
		D3DXMATERIAL *pMat = (D3DXMATERIAL*)g_Model[i].pMaterial->GetBufferPointer();
		strcpy(g_Model[i].filename, filename);

		//各マテリアル情報の取得
		for (DWORD j = 0; j < g_Model[i].MaterialCount; j++)
		{
			//色
			pMat[j].MatD3D.Diffuse.r = 1.0f;
			pMat[j].MatD3D.Diffuse.g = 1.0f;
			pMat[j].MatD3D.Diffuse.b = 1.0f;
			pMat[j].MatD3D.Diffuse.a = 0.1f;

			pMat[j].MatD3D.Ambient.r = 1.0f;
			pMat[j].MatD3D.Ambient.g = 1.0f;
			pMat[j].MatD3D.Ambient.b = 1.0f;
			pMat[j].MatD3D.Ambient.a = 0.1f;
			//テクスチャの読み込み
			if (pMat[j].pTextureFilename == NULL)
			{
				g_Model[i].pTextureIds[j] = -1;
			}
			//テクスチャ有り
			else
			{
				//ファイル名の加工(パスの追加)
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
//終了
//=====================================================
void Fbx_Model_Uninit(void)
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
//描画
//=====================================================
void Fbx_Model_Draw(int modelId, D3DMATRIX mtxWorld, D3DXCOLOR color)
{
	//デバイスのポインタ取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//マテリアルの個数分ループ
	for (DWORD i = 0; i < g_Model[modelId].MaterialCount; i++)
	{
		//マテリアルの設定
		//if (g_Model[modelId].pTextureIds[i] < 0)	//テクスチャが貼られていない場合
		//{
			pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
			//マテリアル設定
			D3DMATERIAL9 mat = {};
			mat.Diffuse.r = color.r;
			mat.Diffuse.g = color.g;
			mat.Diffuse.b = color.b;
			mat.Diffuse.a = color.a;

			mat.Ambient = mat.Diffuse;

			pDevice->SetMaterial(&mat);
		//}
		//else
		//{
		//	//マテリアルカラーを白、テクスチャを貼る
		//	pDevice->SetTexture(0, Texture_GetTexture(g_Model[modelId].pTextureIds[i]));
		//}

		pDevice->SetTransform(D3DTS_WORLD, &mtxWorld);
		//メッシュを書く
		g_Model[modelId].pMesh->DrawSubset(i);
	}
}