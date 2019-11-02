
#ifndef _TEXTURE_H_
#define _TEXTURE_H_

#include <d3d9.h>

int Texture_SetLoadFile(const char* pFilename, int width, int height);	//(戻り値)予約番号 読み込むファイルの予約(ファイル名,テクスチャの幅,高さ)
int Texture_Load();														//(戻り値)読み込めなかった数 ファイルの読み込み
LPDIRECT3DTEXTURE9 Texture_GetTexture(int id);							//(戻り値)テクスチャの番号 予約したテクスチャを取得
int Texture_GetWidth(int id);
int Texture_GetHeight(int id);
void Texture_Destroy(int ids[], int count);								//テクスチャを選択して解放(解放したいテクスチャのidの先頭アドレス, 個数分アドレスから選択)
void Texture_Destroy(void);												//テクスチャをすべて解放
int Texture_GetWidth(int id);											//(戻り値)テクスチャの幅 テクスチャの予約番号
int Texture_GetHeight(int id);											//(戻り値)テクスチャの高さ テクスチャの予約番号

#define TEXTUREDATA_MAX (64)	//テクスチャの最大数
#define FILENAME_MAX (128)		//ファイル名の最大文字数

#endif