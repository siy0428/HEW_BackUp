#include "mydirectx.h"
#include <d3dx9.h>
#include "texture.h"

//====================================================
//マクロ定義
//====================================================
#define WIDTH  (100)	//1以上
#define HEIGHT (100)	//1以上
#define VERTEX_NUM ((WIDTH + 1) * (HEIGHT + 1))		//頂点数
#define INDEX_NUM (VERTEX_NUM + (HEIGHT - 1) * (WIDTH + 1) + (HEIGHT - 1) * 2)	//インデックス数

//====================================================
//構造体宣言
//====================================================
typedef struct Mesh_Vertex_tag
{
	D3DXVECTOR3 position;	//座標
	D3DXVECTOR3 normal;		//法線ベクトル
	D3DCOLOR color;			//色情報
	D3DXVECTOR2 UV;			//テクスチャ
}MeshVertex;
#define FVF_MESH (D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_NORMAL | D3DFVF_TEX1)

//====================================================
//グローバル変数
//====================================================
static LPDIRECT3DVERTEXBUFFER9 lpVtxBuf;   // 頂点バッファ
static LPDIRECT3DINDEXBUFFER9 lpIdxBuf;    // 頂点インデックスバッファ
static unsigned int g_tex = NULL;

//=====================================================
//初期化
//=====================================================
void Mesh_Init(void)
{
	//デバイスのポインタ取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//g_tex = Texture_SetLoadFile("Texture\\f_mesh.jpg", 1300, 866);
	g_tex = Texture_SetLoadFile("Texture\\ice_field.jpg", 612, 612);

	pDevice->CreateVertexBuffer(sizeof(MeshVertex) * VERTEX_NUM, 0, (D3DFORMAT)FVF_MESH, D3DPOOL_MANAGED, &lpVtxBuf, NULL);

	// ロック
	MeshVertex *vbuf;
	lpVtxBuf->Lock(0, 0, (void**)(&vbuf), D3DLOCK_NOSYSLOCK);

	//頂点データ初期化
	for (int i = 0; i < VERTEX_NUM; i++)
	{
		vbuf[i].position = D3DXVECTOR3((FLOAT)(i % (WIDTH + 1)) + -WIDTH / 2, 0.0f, (FLOAT)(-i / (WIDTH + 1)) + HEIGHT / 2);
		vbuf[i].normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		vbuf[i].color = D3DCOLOR_RGBA(255, 255, 255, 255);
		vbuf[i].UV = D3DXVECTOR2((FLOAT)(i % (WIDTH + 1)) + -WIDTH / 2, (FLOAT)(-i / (WIDTH + 1)) + HEIGHT / 2);
	}
	// 解放
	lpVtxBuf->Unlock();

	pDevice->CreateIndexBuffer(INDEX_NUM * sizeof(MeshVertex), 0, D3DFMT_INDEX16, D3DPOOL_MANAGED, &lpIdxBuf, NULL);

	// 確保する頂点インデックスバッファを宣言
	unsigned short*    ibuf;
	// ロック
	lpIdxBuf->Lock(0, 0, (void**)(&ibuf), D3DLOCK_NOSYSLOCK);

	// 頂点インデックスの確保
	unsigned short index_num = 0;	//縮退ポリゴンの設定が行われた回数
	for (unsigned short i = 0; i < INDEX_NUM / 2; i++)
	{
		//縮退ポリゴン
		if ((WIDTH + 1) + (WIDTH + 2) * index_num == i && i != 0 && i != INDEX_NUM / 2 && HEIGHT > 1)
		{
			ibuf[i + i] = i - 1 - index_num;
			ibuf[i + i + 1] = (WIDTH + 1) + i - index_num;
			index_num++;
		}
		//通常インデックス
		else
		{
			ibuf[i + i] = (WIDTH + 1) + (i - index_num);
			ibuf[i + i + 1] = (i - index_num);
		}
	}

	// 解放
	lpIdxBuf->Unlock();

	// デバイスにインデックスをセット
	pDevice->SetIndices(lpIdxBuf);
}

//=====================================================
//終了
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
//描画
//=====================================================
void Mesh_Draw(void)
{
	//デバイスのポインタ取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//直前の行列の初期化
	D3DXMATRIX mtxWorld;
	D3DXMatrixIdentity(&mtxWorld);
	pDevice->SetTransform(D3DTS_WORLD, &mtxWorld);

	//pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
	//ライティング設定
	pDevice->SetRenderState(D3DRS_DIFFUSEMATERIALSOURCE, D3DMCS_COLOR1);	//D3DMCS_COLOR1は頂点カラー, D3DMCS_MATERIALはモデルカラー
	pDevice->SetRenderState(D3DRS_NORMALIZENORMALS, TRUE);					//(正規化, するかしないか)
	//アンビエントライト
	pDevice->SetRenderState(D3DRS_AMBIENT, D3DCOLOR_XRGB(167, 98, 255));
	pDevice->SetRenderState(D3DRS_AMBIENTMATERIALSOURCE, D3DMCS_COLOR1);

	//ラッピングモードの指定
	pDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
	pDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);
	//描画設定
	pDevice->SetStreamSource(0, lpVtxBuf, 0, sizeof(MeshVertex));
	pDevice->SetIndices(lpIdxBuf);
	pDevice->SetFVF(FVF_MESH);								//デバイスに頂点データを渡す
	pDevice->SetTexture(0, Texture_GetTexture(g_tex));							//テクスチャをデバイスに渡す
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);			//FALSE:ライトOFF TRUE:ライトON
	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0, VERTEX_NUM - 2, 0, INDEX_NUM - 2);

	//ライティング設定
	pDevice->SetRenderState(D3DRS_DIFFUSEMATERIALSOURCE, D3DMCS_MATERIAL);	//D3DMCS_COLOR1は頂点カラー, D3DMCS_MATERIALはモデルカラー
	pDevice->SetRenderState(D3DRS_NORMALIZENORMALS, TRUE);					//(正規化, するかしないか)
	//アンビエントライト
	pDevice->SetRenderState(D3DRS_AMBIENT, D3DCOLOR_XRGB(167, 98, 100));
	pDevice->SetRenderState(D3DRS_AMBIENTMATERIALSOURCE, D3DMCS_MATERIAL);
}