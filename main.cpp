//=========================================================================
//						シューティングゲーム
//
//	出席番号 45
//	山崎　聖矢
//=========================================================================

#include <windows.h>
#include <d3dx9.h>
#include <d3d9.h>
#include "common.h"
#include "mydirectx.h"
#include "texture.h"
#include "sprite.h"
#include "system_timer.h"
#include "debug_font.h"
#include "input.h"
#include "game.h"
#include "sound.h"
#include "debug.h"

//=====================================================
//マクロ定義
//=====================================================
#define CLASS_NAME		"GameWindow"			//ウィンドウのクラス名
#define WINDOW_CAPTION	"3DObject"				//ウィンドウに表示するタイトル
#define FPS_MEASUREMENT_TIME (1.0)				//計測する秒数
#define FPS (60)								//fps値

//=====================================================
//プロトタイプ宣言
//=====================================================
LRESULT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
static bool Init(HINSTANCE hInstance, HWND hwnd);//初期化
static void Uninit(void);	//終了処理
static void Update(void);	//更新処理
static void Draw(void);		//描画処理

//=====================================================
//グローバル変数
//=====================================================
static LPDIRECT3DTEXTURE9 g_pTexture[3] = { NULL };//テクスチャの取得
static int tex[TEXTUREDATA_MAX];
static int g_FrameCount = 0;			//フレームカウンター
static int g_FPSBaseFrameCount = 0;		//fps計測のための変数
static double g_FPSBaseTime = 0.0;		//fps計測のための時間
static double g_StaticFrameTime = 0.0;	//フレーム固定用計測時間
static float g_FPS = 0.0f;				//1秒間のフレーム数

//====================================================
//構造体宣言
//====================================================


//=====================================================
//メインプログラム
//=====================================================
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	WNDCLASS wc = {};				//構造体内の使ってないものもすべて初期化しないとエラーになる
	wc.lpfnWndProc = WndProc;		//ウィンドウプロシージャの登録
	wc.lpszClassName = CLASS_NAME;	//クラス名
	wc.hInstance = hInstance;		//アプリケーションのインスタンス番号を入れる
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);	//カーソルの種類
	wc.hbrBackground = (HBRUSH)(COLOR_BACKGROUND + 1);		//ウィンドウの色

	RegisterClass(&wc);		//オリジナルのクラス(ウィンドウ)を登録

	const DWORD window_style = WS_OVERLAPPEDWINDOW ^ WS_THICKFRAME ^ WS_MAXIMIZEBOX;		//ウィンドウ形式の定数化
	//WS_OVERLAPPEDWINDOW ^ WS_THICKFRAME ^ WS_MAXIMIZEBOX
	//WS_VISIBLE | WS_POPUP

	RECT window_rect = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };		//幅と高さを計算する

	AdjustWindowRect(&window_rect, window_style, FALSE);

	int window_width = window_rect.right - window_rect.left;
	int window_height = window_rect.bottom - window_rect.top;

	int desktop_width = GetSystemMetrics(SM_CXSCREEN);				//モニターの横の解像度
	int desktop_height = GetSystemMetrics(SM_CYSCREEN);				//モニターの縦の解像度

	int window_x = max((desktop_width - window_width) / 2, 0);		//ウィンドウのxを設定
	int window_y = max((desktop_height - window_height) / 2, 0);	//ウィンドウのyを設定

	HWND hwnd = CreateWindow(
		CLASS_NAME,				//ウィンドウのクラス名
		WINDOW_CAPTION,			//ウィンドウに表示するタイトル
		window_style,			//ウィンドウの形式の指定,ウィンドウサイズを変更させない
		//WS_THICFRAME = 画面サイズを変更できない。	WS_MAXIMIZEBOX = 最大化が押せなくなる。
		window_x,				//x
		window_y,				//y
		window_width,			//幅
		window_height,			//高さ
		//0, 
		//0,
		//desktop_width,
		//desktop_height,
		NULL,
		NULL,
		hInstance,
		NULL
	);

	ShowWindow(hwnd, nCmdShow);
	UpdateWindow(hwnd);			//追加

	//初期化で失敗した場合
	if (!Init(hInstance, hwnd))
	{
		return -1;	//WinMainの終了
	}

	MSG msg = {};
	while (WM_QUIT != msg.message)		//GetMessage WM_QUITだったらfalse
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			//ゲーム処理
			double time = SystemTimer_GetTime();
			if (time - g_StaticFrameTime < 1.0 / FPS)
			{
				Sleep(0);
			}
			else
			{
				g_StaticFrameTime = time;
				Update();	//更新
				Draw();		//描画
			}
		}
	}

	Uninit();	//終了処理

	return (int)msg.wParam;
}

//=====================================================
//メッセージの受け取り
//=====================================================
LRESULT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	//WM = ウィンドウメッセージ
	switch (uMsg)
	{
	case WM_KEYDOWN:
		if (wParam == VK_ESCAPE)	//wParamにはなんのキーが押されたかが入る。
		{
			SendMessage(hwnd, WM_CLOSE, 0, 0);	//ウィンドウに対してメッセージを送る。
		}
		break;

	case WM_CLOSE:
		if (MessageBox(hwnd, "本当に終了してもよろしいですか？", "確認", MB_OKCANCEL | MB_DEFBUTTON2) == IDOK)
		{
			DestroyWindow(hwnd);	//ウィンドウの破壊をしてWM_DESTROYが送られる。
		}
		return 0;

	case WM_DESTROY:
		PostQuitMessage(0);			//終了
		return 0;
	};

	return DefWindowProc(hwnd, uMsg, wParam, lParam);	//同じ引数を入れるとデフォルト作業を行う
}

//=====================================================
//初期化処理
//=====================================================
bool Init(HINSTANCE hInstance, HWND hwnd)
{
	//初期化に失敗
	if (!MyDirectX_Init(hwnd))
	{
		return false;
	}

	//fps表示用フォントの初期化
	DebugFont_Initialize();

	// キーボード入力モジュールの初期化
	Keyboard_Initialize(hInstance, hwnd);

	//ゲームルーチン用初期化
	Game_Init(hwnd);

	//システムタイマーの初期化
	SystemTimer_Initialize();

	//システムタイマーの起動
	SystemTimer_Start();

	//変数初期化
	g_FrameCount = 0;
	g_FPSBaseFrameCount = 0;
	g_FPS = 0.0f;
	g_FPSBaseTime = SystemTimer_GetTime();
	g_StaticFrameTime = g_FPSBaseTime;

	return true;
}

//=====================================================
//終了処理
//=====================================================
void Uninit(void)
{
	MyDirectX_Uninit();
	DebugFont_Finalize();
	Texture_Destroy();
	Keyboard_Finalize();
	Game_Uninit();
	Debug_Uninit();
}


//=====================================================
//更新処理
//=====================================================
void Update(void)
{
	//フレームカウントの更新
	g_FrameCount++;
	//現在のシステム時間の取得
	double time = SystemTimer_GetTime();
	//前回のFPS計測時間から規定時間経っていたらFPS計測
	if (time - g_FPSBaseTime >= FPS_MEASUREMENT_TIME)	//今の時間から1個前の時間をマイナス
	{
		//FPSの計算(前回からの経過フレーム / 経過時間)
		g_FPS = (float)(g_FrameCount - g_FPSBaseFrameCount / (time - g_FPSBaseTime));
		//FPS計測の基になる変数を更新
		g_FPSBaseFrameCount = g_FrameCount;
		g_FPSBaseTime = time;
	}
	Game_Update();
}

//=====================================================
//描画処理
//=====================================================
void Draw(void)
{
	//デバイスの値の取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);			//アルファブレンドを有効化
																	//D3DRS_ALPHABLENDENABLEで有効,TRUEで有効化
	pDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);	//等倍画像より小さい画像のピクセルを合わせる
	pDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);	//でかい画像のピクセルを合わせる
	pDevice->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);	//2のべき乗のサイズの画像を用意して最適なサイズに切り替える
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);//D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA
	pDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);	//MODULATE掛け算
	//ポリゴンのα値を無視しない設定

	//画面クリア
	pDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_RGBA(0, 0, 255, 0), 1.0f, 0);//, 全体をクリアする場合はNULL。rect型の配列の要素数を入れるとその形にくり抜ける, TARGET(色)ZBUFFER(深度)のクリア, 
	//第三引数があれば色の指定, ZBUFFERがあれば0が手前、1が奥(一番うしろでクリア),0でok
	pDevice->BeginScene();						//シーンの始まり
	Debug_Begin();
	Game_Draw();
#if (DEBUG) | (_DEBUG)
	DebugFont_Draw(SCREEN_WIDTH - 75, 30, "%.01lf", g_FPS);	//fps表示
#endif
	Debug_End();
	pDevice->EndScene();						//シーンの終わり Beginsceneと対
	pDevice->Present(NULL, NULL, NULL, NULL);	//シーンの渡し(1F/1回ここ以外で書かない)
}