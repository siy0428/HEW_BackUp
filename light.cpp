#include <d3dx9.h>
#include "mydirectx.h"

//=====================================================
//ライトの設定
//=====================================================
void Light_Set(void)
{
	//デバイスのポインタ取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	D3DLIGHT9 light = {};	//ライト構造体
	light.Type = D3DLIGHT_DIRECTIONAL;
	D3DXVECTOR3 vecDirLight(-1.0f, -1.0f, 0.0f);		//ライトベクトル(単位ベクトルにすること)
	D3DXVec3Normalize(&vecDirLight, &vecDirLight);	//正規化 (出力, 入力);	
	light.Direction = vecDirLight;					//ベクトルの代入
	//ライトカラーの設定
	light.Diffuse.r = 1.0f;
	light.Diffuse.g = 1.0f;
	light.Diffuse.b = 1.0f;
	light.Diffuse.a = 1.0f;

	pDevice->SetLight(0, &light);	//(ライトスロット, ライトの設定)
	pDevice->LightEnable(0, TRUE);	//ライト別ON/OFF
}
