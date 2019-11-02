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
	D3DXVECTOR4 position;	//頂点座標
	D3DCOLOR color;			//頂点の色情報
	D3DXVECTOR2 UV;			//uv座標(texcoord)
}Vertex2d;
#define FVF_VERTEX2D (D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1)	//x,y,z,RHWをもって頂点データを表す

//====================================================
//ポリゴン描画(通常)
//====================================================
void Sprite_Draw(int texId, float dx, float dy)
{
	//頂点データ
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
	pDevice->SetFVF(FVF_VERTEX2D);			//デバイスに頂点データを渡す
	pDevice->SetTexture(0, Texture_GetTexture(texId));	//テクスチャをデバイスに渡す
	pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, v, sizeof(Vertex2d));//図形の描画方(塗りつぶし), 図形の数, 頂点データの先頭アドレス, 頂点データのサイズ
																		  //D3DPT_TRIANGLELISTにすると三角形の描画
																		  //D3DPT_POINTLISTにすると点の描画（点1つで図形1つ分とみなす）
																		  //D3DPT_LINELISTにすると2頂点で1線の描画
																		  //D3DPT_LINESTRIPにすると連続して線を描画
																		  //D3DPT_TRIANGLESTRIPにすると四角形を描画(三角形の数を第二引数に入れる)
																		  //D3DPT_TRIANGLEFAN扇形
																		  //円描画
}

//====================================================
//ポリゴン描画(通常)(透明度設定)
//====================================================
void Sprite_Draw(int texId, float dx, float dy, int alpha)
{
	//頂点データ
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
	pDevice->SetFVF(FVF_VERTEX2D);			//デバイスに頂点データを渡す
	pDevice->SetTexture(0, Texture_GetTexture(texId));	//テクスチャをデバイスに渡す
	pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, v, sizeof(Vertex2d));//図形の描画方(塗りつぶし), 図形の数, 頂点データの先頭アドレス, 頂点データのサイズ
																		  //D3DPT_TRIANGLELISTにすると三角形の描画
																		  //D3DPT_POINTLISTにすると点の描画（点1つで図形1つ分とみなす）
																		  //D3DPT_LINELISTにすると2頂点で1線の描画
																		  //D3DPT_LINESTRIPにすると連続して線を描画
																		  //D3DPT_TRIANGLESTRIPにすると四角形を描画(三角形の数を第二引数に入れる)
																		  //D3DPT_TRIANGLEFAN扇形
																		  //円描画
}

//====================================================
//ポリゴン描画(UV値)
//====================================================
void Sprite_Draw(int texId, float dx, float dy, int cx, int cy, int cw, int ch)
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
		{ D3DXVECTOR4(dx - 0.5f, dy - 0.5f, 0.0f, 1.0f),g_color, D3DXVECTOR2(u0, v0) },
		{ D3DXVECTOR4(dx + cw - 0.5f, dy - 0.5f, 0.0f, 1.0f),g_color, D3DXVECTOR2(u1, v0) },
		{ D3DXVECTOR4(dx - 0.5f, dy + ch - 0.5f, 0.0f, 1.0f),g_color, D3DXVECTOR2(u0, v1) },
		{ D3DXVECTOR4(dx + cw - 0.5f, dy + ch - 0.5f, 0.0f, 1.0f),g_color, D3DXVECTOR2(u1,v1) },
	};
	pDevice->SetFVF(FVF_VERTEX2D);			//デバイスに頂点データを渡す
	pDevice->SetTexture(0, Texture_GetTexture(texId));	//テクスチャをデバイスに渡す
	pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, v, sizeof(Vertex2d));//図形の描画方(塗りつぶし), 図形の数, 頂点データの先頭アドレス, 頂点データのサイズ
																		  //D3DPT_TRIANGLELISTにすると三角形の描画
																		  //D3DPT_POINTLISTにすると点の描画（点1つで図形1つ分とみなす）
																		  //D3DPT_LINELISTにすると2頂点で1線の描画
																		  //D3DPT_LINESTRIPにすると連続して線を描画
																		  //D3DPT_TRIANGLESTRIPにすると四角形を描画(三角形の数を第二引数に入れる)
																		  //D3DPT_TRIANGLEFAN扇形
																		  //円描画
}

//====================================================
//ポリゴン描画(UV値)(透明度設定)
//====================================================
void Sprite_Draw(int texId, float dx, float dy, int cx, int cy, int cw, int ch, int alpha)
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
		{ D3DXVECTOR4(dx - 0.5f, dy - 0.5f, 0.0f, 1.0f),D3DCOLOR_RGBA(255, 255, 255, alpha), D3DXVECTOR2(u0, v0) },
	{ D3DXVECTOR4(dx + cw - 0.5f, dy - 0.5f, 0.0f, 1.0f),D3DCOLOR_RGBA(255, 255, 255, alpha), D3DXVECTOR2(u1, v0) },
	{ D3DXVECTOR4(dx - 0.5f, dy + ch - 0.5f, 0.0f, 1.0f),D3DCOLOR_RGBA(255, 255, 255, alpha), D3DXVECTOR2(u0, v1) },
	{ D3DXVECTOR4(dx + cw - 0.5f, dy + ch - 0.5f, 0.0f, 1.0f),D3DCOLOR_RGBA(255, 255, 255, alpha), D3DXVECTOR2(u1,v1) },
	};
	pDevice->SetFVF(FVF_VERTEX2D);			//デバイスに頂点データを渡す
	pDevice->SetTexture(0, Texture_GetTexture(texId));	//テクスチャをデバイスに渡す
	pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, v, sizeof(Vertex2d));//図形の描画方(塗りつぶし), 図形の数, 頂点データの先頭アドレス, 頂点データのサイズ
																		  //D3DPT_TRIANGLELISTにすると三角形の描画
																		  //D3DPT_POINTLISTにすると点の描画（点1つで図形1つ分とみなす）
																		  //D3DPT_LINELISTにすると2頂点で1線の描画
																		  //D3DPT_LINESTRIPにすると連続して線を描画
																		  //D3DPT_TRIANGLESTRIPにすると四角形を描画(三角形の数を第二引数に入れる)
																		  //D3DPT_TRIANGLEFAN扇形
																		  //円描画
}

//====================================================
//ポリゴン描画(回転)(拡大)
//====================================================
void Sprite_Draw(int texId, float dx, float dy, int cx, int cy, int cw, int ch, float angle, float center_x, float center_y, float zoom)
{
	//頂点データ
	int w = Texture_GetWidth(texId);
	int h = Texture_GetHeight(texId);

	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxW, mtxR, mtxT, mtxIT, mtxS;
	D3DXMatrixScaling(&mtxS, 1.0f + zoom, 1.0f + zoom, 1.0f + zoom);	//行列のポインタ,x,y,z
	D3DXMatrixTranslation(&mtxT, -center_x - dx, -center_y - dy, 0);	//平行移動
	D3DXMatrixTranslation(&mtxIT, center_x + dx, center_y + dy, 0);	//元の場所に移動
	D3DXMatrixRotationZ(&mtxR, angle);						//回転
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

	pDevice->SetFVF(FVF_VERTEX2D);						//デバイスに頂点データを渡す
	pDevice->SetTexture(0, Texture_GetTexture(texId));	//テクスチャをデバイスに渡す
	pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, v, sizeof(Vertex2d));//図形の描画方(塗りつぶし), 図形の数, 頂点データの先頭アドレス, 頂点データのサイズ
																		  //D3DPT_TRIANGLELISTにすると三角形の描画
																		  //D3DPT_POINTLISTにすると点の描画（点1つで図形1つ分とみなす）
																		  //D3DPT_LINELISTにすると2頂点で1線の描画
																		  //D3DPT_LINESTRIPにすると連続して線を描画
																		  //D3DPT_TRIANGLESTRIPにすると四角形を描画(三角形の数を第二引数に入れる)
																		  //D3DPT_TRIANGLEFAN扇形
																		  //円描画
}

//====================================================
//ポリゴン描画(回転)(拡大)(色指定)
//====================================================
void Sprite_Draw(int texId, float dx, float dy, int cx, int cy, int cw, int ch, float angle, float center_x, float center_y, float zoom, D3DXCOLOR color)
{
	//頂点データ
	int w = Texture_GetWidth(texId);
	int h = Texture_GetHeight(texId);

	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxW, mtxR, mtxT, mtxIT, mtxS;
	D3DXMatrixScaling(&mtxS, 1.0f + zoom, 1.0f + zoom, 1.0f + zoom);	//行列のポインタ,x,y,z
	D3DXMatrixTranslation(&mtxT, -center_x - dx, -center_y - dy, 0);	//平行移動
	D3DXMatrixTranslation(&mtxIT, center_x + dx, center_y + dy, 0);	//元の場所に移動
	D3DXMatrixRotationZ(&mtxR, angle);						//回転
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

	pDevice->SetFVF(FVF_VERTEX2D);						//デバイスに頂点データを渡す
	pDevice->SetTexture(0, Texture_GetTexture(texId));	//テクスチャをデバイスに渡す
	pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, v, sizeof(Vertex2d));//図形の描画方(塗りつぶし), 図形の数, 頂点データの先頭アドレス, 頂点データのサイズ
																		  //D3DPT_TRIANGLELISTにすると三角形の描画
																		  //D3DPT_POINTLISTにすると点の描画（点1つで図形1つ分とみなす）
																		  //D3DPT_LINELISTにすると2頂点で1線の描画
																		  //D3DPT_LINESTRIPにすると連続して線を描画
																		  //D3DPT_TRIANGLESTRIPにすると四角形を描画(三角形の数を第二引数に入れる)
																		  //D3DPT_TRIANGLEFAN扇形
																		  //円描画
}

//====================================================
//ポリゴン描画(UV値)(左右反転)
//====================================================
void Sprite_Reverse_Draw(int texId, float dx, float dy, int cx, int cy, int cw, int ch)
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
	{ D3DXVECTOR4(dx - 0.5f, dy - 0.5f, 0.0f, 1.0f),g_color, D3DXVECTOR2(u1, v0) },
	{ D3DXVECTOR4(dx + cw - 0.5f, dy - 0.5f, 0.0f, 1.0f),g_color, D3DXVECTOR2(u0, v0) },
	{ D3DXVECTOR4(dx - 0.5f, dy + ch - 0.5f, 0.0f, 1.0f),g_color, D3DXVECTOR2(u1, v1) },
	{ D3DXVECTOR4(dx + cw - 0.5f, dy + ch - 0.5f, 0.0f, 1.0f),g_color, D3DXVECTOR2(u0,v1) },
	};
	pDevice->SetFVF(FVF_VERTEX2D);			//デバイスに頂点データを渡す
	pDevice->SetTexture(0, Texture_GetTexture(texId));	//テクスチャをデバイスに渡す
	pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, v, sizeof(Vertex2d));//図形の描画方(塗りつぶし), 図形の数, 頂点データの先頭アドレス, 頂点データのサイズ
																		  //D3DPT_TRIANGLELISTにすると三角形の描画
																		  //D3DPT_POINTLISTにすると点の描画（点1つで図形1つ分とみなす）
																		  //D3DPT_LINELISTにすると2頂点で1線の描画
																		  //D3DPT_LINESTRIPにすると連続して線を描画
																		  //D3DPT_TRIANGLESTRIPにすると四角形を描画(三角形の数を第二引数に入れる)
																		  //D3DPT_TRIANGLEFAN扇形
																		  //円描画
}

//====================================================
//ポリゴン描画(UV値)(左右反転)
//====================================================
void Sprite_Upside_Draw(int texId, float dx, float dy, int cx, int cy, int cw, int ch)
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
	{ D3DXVECTOR4(dx - 0.5f, dy - 0.5f, 0.0f, 1.0f),g_color, D3DXVECTOR2(u0, v1) },
	{ D3DXVECTOR4(dx + cw - 0.5f, dy - 0.5f, 0.0f, 1.0f),g_color, D3DXVECTOR2(u1, v1) },
	{ D3DXVECTOR4(dx - 0.5f, dy + ch - 0.5f, 0.0f, 1.0f),g_color, D3DXVECTOR2(u0, v0) },
	{ D3DXVECTOR4(dx + cw - 0.5f, dy + ch - 0.5f, 0.0f, 1.0f),g_color, D3DXVECTOR2(u1,v0) },
	};
	pDevice->SetFVF(FVF_VERTEX2D);			//デバイスに頂点データを渡す
	pDevice->SetTexture(0, Texture_GetTexture(texId));	//テクスチャをデバイスに渡す
	pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, v, sizeof(Vertex2d));//図形の描画方(塗りつぶし), 図形の数, 頂点データの先頭アドレス, 頂点データのサイズ
																		  //D3DPT_TRIANGLELISTにすると三角形の描画
																		  //D3DPT_POINTLISTにすると点の描画（点1つで図形1つ分とみなす）
																		  //D3DPT_LINELISTにすると2頂点で1線の描画
																		  //D3DPT_LINESTRIPにすると連続して線を描画
																		  //D3DPT_TRIANGLESTRIPにすると四角形を描画(三角形の数を第二引数に入れる)
																		  //D3DPT_TRIANGLEFAN扇形
																		  //円描画
}

void Sprite_SetColor(D3DXCOLOR color)
{
	g_color = color;
}
