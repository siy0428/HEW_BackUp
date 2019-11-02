//=======================================================================
//							基本設定
//
//	出席番号 45	
//	山崎 聖矢
//=======================================================================

#include <d3d9.h>
#include <windows.h>
#include "common.h"

//=====================================================
//グローバル変数
//=====================================================
static LPDIRECT3D9 g_pD3D = NULL;			//インタフェースの取得
static LPDIRECT3DDEVICE9 g_pDevice = NULL;	//デバイスの取得

//=====================================================
//初期化
//=====================================================
bool MyDirectX_Init(HWND hwnd)
{
	g_pD3D = Direct3DCreate9(D3D_SDK_VERSION);	//ダイレクト3dのインタフェースを取得

	//インタフェースの取得に失敗した場合のエラー処理
	if (g_pD3D == NULL)
	{
		MessageBox(hwnd, "インタフェースの取得に失敗しました", "失敗", MB_OK);
		return false;
	}

	D3DPRESENT_PARAMETERS d3dpp = {};			//デバイス情報の初期化
	d3dpp.BackBufferWidth = SCREEN_WIDTH;		//裏バッファの幅
	d3dpp.BackBufferHeight = SCREEN_HEIGHT;		//裏バッファの高さ
	d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;	//osに合わせた画面サイズ
	d3dpp.BackBufferCount = 1;					//バッファの数
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;	//表と裏の入れ替え
	d3dpp.Windowed = TRUE;						//ウィンドウモードで作る場合はtrue,フルスクリーンはfalse
	d3dpp.EnableAutoDepthStencil = TRUE;		//深度バッファ、ステンシルバッファを有効にする
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;	//深度バッファの設定をしたらフォーマットの指定(Depth16bit)
	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;	//フルスクリーンのみ適応(画面を専有できるため)
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;	//画面のリフレッシュレートに合わせてGPUにアクセス
																//D3DPRESENT_INTERVAL_IMMEDIATE			D3DPRESENT_INTERVAL_DEFAULT
																//画面の垂直同期をまたずにすぐ描写

	HRESULT hr = g_pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hwnd, D3DCREATE_HARDWARE_VERTEXPROCESSING, &d3dpp, &g_pDevice);
	//どのアダプタかを指定(モニターが複数個の場合は変更), ハードウェアで動かすのかどうか, 
	//対象のウィンドウハンドル, 頂点の演算をハードウェアかソフトウェア, デバイスの条件のアドレス, 完成した情報のアドレスの入れ先

	//デバイスを取得できなかった場合のエラー処理
	if (FAILED(hr))
	{
		MessageBox(hwnd, "デバイスの取得に失敗しました", "失敗", MB_OK);
		return false;
	}

	return true;
}

//=====================================================
//初期化
//=====================================================
void MyDirectX_Uninit(void)
{
	if (g_pDevice != NULL)
	{
		g_pDevice->Release();
		g_pDevice = NULL;
	}
	if (g_pD3D != NULL)
	{
		g_pD3D->Release();
		g_pD3D = NULL;
	}
}

//=====================================================
//デバイスの値取得
//=====================================================
LPDIRECT3DDEVICE9 GetDevice(void)
{
	return g_pDevice;
}