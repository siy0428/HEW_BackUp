#include <d3dx9.h>
#include "collision.h"
#include "mydirectx.h"
#include "debug.h"

//=====================================================
//マクロ定義
//=====================================================
#define DEBUG_CIRCLE_VERTEX_NUM (8)	//当たり判定の頂点数
#define DEBUG_CIRCLE_MAX (4096)		//表示する当たり判定の数

//=====================================================
//構造体宣言
//=====================================================


//=====================================================
//グローバル変数
//=====================================================
static LPDIRECT3DVERTEXBUFFER9 g_pVertexBuffer = NULL;
static LPDIRECT3DINDEXBUFFER9 g_pIndexBuffer = NULL;
static CIRCLE g_Circles[DEBUG_CIRCLE_MAX];
static D3DCOLOR g_Color = 0xFFFFFFFF;
static D3DCOLOR g_Colors[DEBUG_CIRCLE_MAX];
static int g_CircleCount = 0;
static int g_DebugCount = 0;
static int g_Index = 0;

//=====================================================
//初期化処理
//=====================================================
void Debug_Init(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//8頂点分確保                         //(sizeof ,             D3DUSAGE_WRITEONLY  , 0でも可
	pDevice->CreateVertexBuffer(DEBUG_CIRCLE_VERTEX_NUM * DEBUG_CIRCLE_MAX * sizeof(DEBUGCIRCLE),
		D3DUSAGE_WRITEONLY, FVF_DEBUG_CIRCLE_VERTEX, D3DPOOL_MANAGED, &g_pVertexBuffer, NULL);//D3DUSAGE_WRITEONLY,0でも可,)

	pDevice->CreateIndexBuffer(DEBUG_CIRCLE_VERTEX_NUM * 2 * DEBUG_CIRCLE_MAX * sizeof(WORD),
		D3DUSAGE_WRITEONLY, D3DFMT_INDEX16, D3DPOOL_MANAGED, &g_pIndexBuffer, NULL);//D3DUSAGE_WRITEONLY,0でも可,)
}
//=====================================================
//終了処理
//=====================================================
void Debug_Uninit(void)
{

}
//=====================================================
//開始
//=====================================================
void Debug_Begin(void)
{
	g_CircleCount = 0;
}
//=====================================================
//円の描画
//=====================================================
void Debug_End(void)
{
	static const float A = D3DX_PI * 2.0f / DEBUG_CIRCLE_VERTEX_NUM;
	DEBUGCIRCLE *pV;

	g_pVertexBuffer->Lock(0, 0, (void**)&pV, D3DLOCK_DISCARD);
	for (int i = 0; i < g_CircleCount; i++)
	{
		int startIndex = i * DEBUG_CIRCLE_VERTEX_NUM;
		for (int j = 0; j < DEBUG_CIRCLE_VERTEX_NUM; j++)
		{
			pV[startIndex + j].position.x = cosf(A * j) * g_Circles[i].radius + g_Circles[i].position.x;
			pV[startIndex + j].position.y = sinf(A * j) * g_Circles[i].radius + g_Circles[i].position.y;
			pV[startIndex + j].position.z = 0.0f;
			pV[startIndex + j].position.w = 1.0f;
			pV[startIndex + j].color = g_Colors[i];
		}
	}
	g_pVertexBuffer->Unlock();

	WORD *pI;
	g_pIndexBuffer->Lock(0, 0, (void**)&pI, D3DLOCK_DISCARD);
	for (int i = 0; i < g_CircleCount; i++)
	{
		int startIndex = (DEBUG_CIRCLE_VERTEX_NUM * 2) * i;
		for (int j = 0; j < DEBUG_CIRCLE_VERTEX_NUM; j++)
		{
			pI[startIndex + j * 2 + 0] = (WORD)(i * DEBUG_CIRCLE_VERTEX_NUM + j);
			pI[startIndex + j * 2 + 1] = (WORD)(i * DEBUG_CIRCLE_VERTEX_NUM + ((j + 1) % DEBUG_CIRCLE_VERTEX_NUM));
		}
	}
	g_pIndexBuffer->Unlock();

	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	pDevice->SetStreamSource(0, g_pVertexBuffer, 0, sizeof(DEBUGCIRCLE));
	pDevice->SetIndices(g_pIndexBuffer);
	pDevice->SetFVF(FVF_DEBUG_CIRCLE_VERTEX);
	pDevice->SetTexture(0, NULL);
	pDevice->DrawIndexedPrimitive(D3DPT_LINELIST, 0, 0, g_CircleCount *  DEBUG_CIRCLE_VERTEX_NUM, 0, DEBUG_CIRCLE_VERTEX_NUM);
}
//=====================================================
//円の登録
//=====================================================
void Debug_DrawCircle(float x, float y, float radius)
{
	g_Colors [g_CircleCount] = g_Color;
	g_Circles[g_CircleCount].position.x = x;
	g_Circles[g_CircleCount].position.y = y;
	g_Circles[g_CircleCount].radius = radius;
	g_CircleCount++;
}
//=====================================================
//色の設定
//=====================================================
void Debug_SetColor(D3DCOLOR color)
{
	g_Color = color;
}

void Debug_CountUp(void)
{
	g_DebugCount++;
}

int Debug_GetCount(void)
{
	return g_DebugCount;
}

void Debug_index(int index)
{
	g_Index = index;
}

int Debug_GetIndex(void)
{
	return g_Index;
}