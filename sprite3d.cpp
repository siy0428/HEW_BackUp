#include <d3d9.h>
#include <d3dx9.h>
#include "texture.h"
#include "mydirectx.h"

//=====================================================
//グローバル変数
//=====================================================
static D3DCOLOR g_color = D3DCOLOR_RGBA(255, 255, 255, 255);

//====================================================
//構造体宣言
//====================================================
typedef struct
{
	D3DXVECTOR3 position;	//頂点座標
	D3DCOLOR color;			//頂点の色情報
	D3DXVECTOR2 UV;			//uv座標(texcoord)
}Vertex2d;
#define FVF_VERTEX3D (D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX1)	//x,y,z,RHWをもって頂点データを表す

//====================================================
//ポリゴン描画(通常)
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
	pDevice->SetFVF(FVF_VERTEX3D);			//デバイスに頂点データを渡す
	pDevice->SetTexture(0, Texture_GetTexture(texId));	//テクスチャをデバイスに渡す
	pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, v, sizeof(Vertex2d));
}


//====================================================
//ポリゴン描画(UV値)
//====================================================
void Sprite3d_Draw(int texId, D3DXMATRIX mtxWorld, int cx, int cy, int cw, int ch)
{
	//頂点データ
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
	pDevice->SetFVF(FVF_VERTEX3D);			//デバイスに頂点データを渡す
	pDevice->SetTexture(0, Texture_GetTexture(texId));	//テクスチャをデバイスに渡す
	pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, v, sizeof(Vertex2d));
}