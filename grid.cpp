#include <d3dx9.h>
#include "mydirectx.h"
#include "common.h"
#include "camera.h"

//====================================================
//マクロ定義
//====================================================
#define GRID_VERTEX (1024)	//グリッド頂点数

//====================================================
//構造体宣言
//====================================================
typedef struct Grid_Vertex_tag
{
	D3DXVECTOR3 position;	//座標
	D3DCOLOR color;			//色情報
}GridVertex;
#define FVF_GRID (D3DFVF_XYZ | D3DFVF_DIFFUSE)

//=====================================================
//グローバル変数
//=====================================================
static GridVertex g_grid_vertex[GRID_VERTEX] = { };
static int g_Length = 0;

//=====================================================
//初期化
//=====================================================
void Grid_Init(int length)
{
	//横線
	for (int i = 0; i < length + 1; i++)
	{
		g_grid_vertex[i * 2].position = D3DXVECTOR3(-(float)length / 2, 0.0f, (float)length / 2 - i * 1.0f);		//左
		g_grid_vertex[i * 2 + 1].position = D3DXVECTOR3((float)length / 2, 0.0f, (float)length / 2 - i * 1.0f);	//右
		//中央の赤線
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
	//縦線
	for (int i = 0; i < length + 1; i++)
	{
		g_grid_vertex[i * 2 + (length + 1) * 2].position = D3DXVECTOR3((float)length / 2 - i * 1.0f, 0.0f, -(float)length / 2);	
		g_grid_vertex[i * 2 + 1 + (length + 1) * 2].position = D3DXVECTOR3((float)length / 2 - i * 1.0f, 0.0f, (float)length / 2);
		//中央の赤線
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
//終了
//=====================================================
void Grid_Uninit(void)
{

}

//=====================================================
//描画
//=====================================================
void Grid_Draw(void)
{
	//デバイスのポインタ取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//ワールド座標変換
	D3DXMATRIX mtxWorld, mtxTrans;							//行列用変数
	D3DXMatrixIdentity(&mtxWorld);							//単位行列を作る
	D3DXMatrixTranslation(&mtxTrans, 0.0f, 0.0f, 0.0f);		//平行移動
	mtxWorld = mtxTrans;									//行列の合成
	pDevice->SetTransform(D3DTS_WORLD, &mtxWorld);

	//描画設定
	pDevice->SetFVF(FVF_GRID);						//デバイスに頂点データを渡す
	pDevice->SetTexture(0, NULL);					//テクスチャをデバイスに渡す
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);	//FALSE:ライトOFF TRUE:ライトON
	pDevice->DrawPrimitiveUP(D3DPT_LINELIST, g_Length * 2 + 2, g_grid_vertex, sizeof(GridVertex));
}

////横線
//for (int i = 0; i < 11; i++)
//{
//	g_grid_vertex[i * 2].position = D3DXVECTOR3(-5.0f, 0.0f, 5.0f - i * 1.0f);		//左
//	g_grid_vertex[i * 2 + 1].position = D3DXVECTOR3(5.0f, 0.0f, 5.0f - i * 1.0f);	//右
//	//中央の赤線
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
////縦線
//for (int i = 0; i < 11; i++)
//{
//	g_grid_vertex[i * 2 + 22].position = D3DXVECTOR3(5.0f - i * 1.0f, 0.0f, -5.0f);		//左
//	g_grid_vertex[i * 2 + 1 + 22].position = D3DXVECTOR3(5.0f - i * 1.0f, 0.0f, 5.0f);	//右
//	//中央の赤線
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