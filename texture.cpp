//=======================================================================
//							テクスチャ
//
//	出席番号 45	
//	山崎 聖矢
//=======================================================================

#include <d3d9.h>
#include <d3dx9.h>
#include "common.h"
#include "mydirectx.h"
#include "texture.h"

typedef struct TextureDate_tag
{
	char filename[FILENAME_MAX];
	int width;
	int height;
	LPDIRECT3DTEXTURE9 pTexture;
}TextureData;

//===========================================
//グローバル変数
//===========================================
static TextureData g_TextureData[TEXTUREDATA_MAX] = {};

//===========================================
//テクスチャの空きさがし
//===========================================
int Texture_SetLoadFile(const char* pFilename, int width, int height)
{
	//同じファイル名を探す
	for (int i = 0; i < TEXTUREDATA_MAX; i++)
	{
		//同じファイル名がヒットしたときの処理
		if (strcmp(g_TextureData[i].filename, pFilename) == 0)
		{
			return i;	//ヒットした管理番号を返す
		}
	}
	//新規登録
	for (int i = 0; i < TEXTUREDATA_MAX; i++)
	{
		//名前がすでに入っていたら次のループ
		if (g_TextureData[i].filename[0] != '\0')
		{
			continue;
		}
		//登録
		strcpy(g_TextureData[i].filename, pFilename);	//名前のコピー
		g_TextureData[i].width = width;					//幅のコピー
		g_TextureData[i].height = height;				//高さのコピー

		return i;
	}
	return -1;	//空き不足
}

//===========================================
//テクスチャの読み込み
//===========================================
int Texture_Load()
{
	int error_count = 0;
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	for (int i = 0; i < TEXTUREDATA_MAX; i++)
	{
		//ファイル名が設定されてれば次のループ
		if (g_TextureData[i].filename[0] == '\0')
		{
			continue;
		}

		//読み込まれたテクスチャは次のループ
		if (g_TextureData[i].pTexture != NULL)
		{
			error_count++;
			continue;
		}

		//テクスチャの取得
		D3DXCreateTextureFromFile(pDevice, g_TextureData[i].filename, &g_TextureData[i].pTexture);

	}
	return error_count;
}

//===========================================
//テクスチャを取得
//===========================================
LPDIRECT3DTEXTURE9 Texture_GetTexture(int id)
{
	return g_TextureData[id].pTexture;
}

//===========================================
//テクスチャの幅を取得
//===========================================
int Texture_GetWidth(int id)
{
	return g_TextureData[id].width;
}

//===========================================
//テクスチャの高さを取得
//===========================================
int Texture_GetHeight(int id)
{
	return g_TextureData[id].height;
}

//===========================================
//指定したテクスチャを消す
//===========================================
void Texture_Destroy(int ids[], int count)
{
	for (int i = 0; i < count; i++)
	{
		if (g_TextureData[ids[i]].pTexture != NULL)
		{
			g_TextureData[ids[i]].pTexture->Release();
			g_TextureData[ids[i]].pTexture = NULL;
			g_TextureData[ids[i]].filename[0] = '\0';
		}
	}
}

//===========================================
//テクスチャをすべて消す
//===========================================
void Texture_Destroy(void)
{
	for (int i = 0; i < TEXTUREDATA_MAX; i++)
	{
		if (g_TextureData[i].pTexture != NULL)
		{
			g_TextureData[i].pTexture->Release();
			g_TextureData[i].pTexture = NULL;
			g_TextureData[i].filename[0] = '\0';
		}
	}
}