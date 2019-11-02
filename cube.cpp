#include <d3dx9.h>
#include "mydirectx.h"
#include "common.h"
#include "debug_font.h"
#include "input.h"
#include <stdio.h>

//====================================================
//構造体宣言
//====================================================
typedef struct Cube_Vertex_tag
{
	D3DXVECTOR3 position;	//座標
	D3DCOLOR color;			//色情報
}CubeVertex;
#define FVF_CUBE (D3DFVF_XYZ | D3DFVF_DIFFUSE)

//=====================================================
//グローバル変数
//=====================================================
static const CubeVertex g_cube_vertex[] = {						//頂点構造体
	//正面
	{D3DXVECTOR3(-0.5f, 0.5f, -0.5f), D3DCOLOR_RGBA(255, 255, 255, 255)},
	{D3DXVECTOR3(0.5f, 0.5f, -0.5f), D3DCOLOR_RGBA(255, 255, 255, 255)},
	{D3DXVECTOR3(0.5f, -0.5f, -0.5f), D3DCOLOR_RGBA(255, 255, 255, 255)},
	{D3DXVECTOR3(0.5f, -0.5f, -0.5f), D3DCOLOR_RGBA(255, 255, 255, 255)},
	{D3DXVECTOR3(-0.5f, -0.5f, -0.5f), D3DCOLOR_RGBA(255, 255, 255, 255)},
	{D3DXVECTOR3(-0.5f, 0.5f, -0.5f), D3DCOLOR_RGBA(255, 255, 255, 255)},
	//右面
	{D3DXVECTOR3(0.5f, 0.5f, -0.5f), D3DCOLOR_RGBA(0, 255, 0, 255)},
	{D3DXVECTOR3(0.5f, 0.5f, 0.5f), D3DCOLOR_RGBA(0, 255, 0, 255)},
	{D3DXVECTOR3(0.5f, -0.5f, 0.5f), D3DCOLOR_RGBA(0, 255, 0, 255)},
	{D3DXVECTOR3(0.5f, -0.5f, 0.5f), D3DCOLOR_RGBA(0, 255, 0, 255)},
	{D3DXVECTOR3(0.5f, -0.5f, -0.5f), D3DCOLOR_RGBA(0, 255, 0, 255)},
	{D3DXVECTOR3(0.5f, 0.5f, -0.5f), D3DCOLOR_RGBA(0, 255, 0, 255)},
	//左面
	{D3DXVECTOR3(-0.5f, 0.5f, 0.5f), D3DCOLOR_RGBA(255, 0, 0, 255)},
	{D3DXVECTOR3(-0.5f, 0.5f, -0.5f), D3DCOLOR_RGBA(255, 0, 0, 255)},
	{D3DXVECTOR3(-0.5f, -0.5f, 0.5f), D3DCOLOR_RGBA(255, 0, 0, 255)},
	{D3DXVECTOR3(-0.5f, -0.5f, 0.5f), D3DCOLOR_RGBA(255, 0, 0, 255)},
	{D3DXVECTOR3(-0.5f, 0.5f, -0.5f), D3DCOLOR_RGBA(255, 0, 0, 255)},
	{D3DXVECTOR3(-0.5f, -0.5f, -0.5f), D3DCOLOR_RGBA(255, 0, 0, 255)},
	//奥面
	{D3DXVECTOR3(0.5f, 0.5f, 0.5f), D3DCOLOR_RGBA(64, 128, 0, 255)},
	{D3DXVECTOR3(-0.5f, 0.5f, 0.5f), D3DCOLOR_RGBA(64, 128, 0, 255)},
	{D3DXVECTOR3(-0.5f, -0.5f, 0.5f), D3DCOLOR_RGBA(64, 128, 0, 255)},
	{D3DXVECTOR3(-0.5f, -0.5f, 0.5f), D3DCOLOR_RGBA(64, 128, 0, 255)},
	{D3DXVECTOR3(0.5f, -0.5f, 0.5f), D3DCOLOR_RGBA(64, 128, 0, 255)},
	{D3DXVECTOR3(0.5f, 0.5f, 0.5f), D3DCOLOR_RGBA(64, 128, 0, 255)},
	//底面
	{D3DXVECTOR3(0.5f, -0.5f, 0.5f), D3DCOLOR_RGBA(128, 32, 64, 255)},
	{D3DXVECTOR3(-0.5f, -0.5f, 0.5f), D3DCOLOR_RGBA(128, 32, 64, 255)},
	{D3DXVECTOR3(0.5f, -0.5f, -0.5f), D3DCOLOR_RGBA(128, 32, 64, 255)},
	{D3DXVECTOR3(0.5f, -0.5f, -0.5f), D3DCOLOR_RGBA(128, 32, 64, 255)},
	{D3DXVECTOR3(-0.5f, -0.5f, 0.5f), D3DCOLOR_RGBA(128, 32, 64, 255)},
	{D3DXVECTOR3(-0.5f, -0.5f, -0.5f), D3DCOLOR_RGBA(128, 32, 64, 255)},
	//上面
	{D3DXVECTOR3(-0.5f, 0.5f, 0.5f), D3DCOLOR_RGBA(255, 255, 0, 255)},
	{D3DXVECTOR3(0.5f, 0.5f, 0.5f), D3DCOLOR_RGBA(255, 255, 0, 255)},
	{D3DXVECTOR3(-0.5f, 0.5f, -0.5f), D3DCOLOR_RGBA(255, 255, 0, 255)},
	{D3DXVECTOR3(-0.5f, 0.5f, -0.5f), D3DCOLOR_RGBA(255, 255, 0, 255)},
	{D3DXVECTOR3(0.5f, 0.5f, 0.5f), D3DCOLOR_RGBA(255, 255, 0, 255)},
	{D3DXVECTOR3(0.5f, 0.5f, -0.5f), D3DCOLOR_RGBA(255, 255, 0, 255)},
};
static D3DXVECTOR3 g_pos = { 4.5f, 0.5f, -4.5f };
//static D3DXVECTOR3 g_pos = { 0.5f, 0.0f, 0.5f };
static int g_state = 0;
static float g_angle = 0.0f;
static float g_scale = 0.0f;
static float g_fall[5] = {
	10.0f,
	10.0f,
	10.0f,
	10.0f,
	10.0f
};

//サイコロ用
static bool g_bRotate = false;

//=====================================================
//初期化
//=====================================================
void Cube_Init(void)
{

}

//=====================================================
//描画
//=====================================================
void Cube_Draw(const D3DXMATRIX *pMtx)
{
	//デバイスのポインタ取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	pDevice->SetTransform(D3DTS_WORLD, pMtx);

	//描画設定
	pDevice->SetFVF(FVF_CUBE);						//デバイスに頂点データを渡す
	pDevice->SetTexture(0, NULL);					//テクスチャをデバイスに渡す
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);	//FALSE:ライトOFF TRUE:ライトON
	pDevice->DrawPrimitiveUP(D3DPT_TRIANGLELIST, 12, g_cube_vertex, sizeof(CubeVertex));
}

//=====================================================
//サイコロ
//=====================================================
void Cube_Dice(void)
{
	Keyboard_Update();

	D3DXMATRIX mtxWorld, mtxRotation[3], mtxTrans[3];		//行列用変数
	D3DXMatrixIdentity(&mtxWorld);

	//右方向　→
	if (Keyboard_IsPress(DIK_D))
	{

	}
	//左方向　←
	else if (Keyboard_IsPress(DIK_A))
	{
		g_bRotate = true;
	}
	if (g_bRotate)
	{
		g_angle -= 1.0f;
		if (g_angle < -90.0f)
		{
			g_bRotate = false;
		}
	}
	D3DXMatrixTranslation(&mtxTrans[0], -0.5f, 0.0f, 0.5f);	//平行移動
	D3DXMatrixRotationZ(&mtxRotation[2], g_angle * D3DX_PI / 180);	//z軸回転
	D3DXMatrixTranslation(&mtxTrans[1], 1.5f, 0.5f, 0.5f);	//平行移動
	D3DXMatrixRotationY(&mtxRotation[1], g_angle * D3DX_PI / 180);	//y軸回転
	D3DXMatrixRotationX(&mtxRotation[0], g_angle * D3DX_PI / 180);	//x軸回転
	D3DXMatrixTranslation(&mtxTrans[1], g_pos.x, g_pos.y, g_pos.z);	//平行移動
	mtxWorld = mtxTrans[0] * mtxRotation[2] * mtxTrans[1];						//行列の合成

//D3DXMatrixRotationY(&mtxRotation[1], g_angle * D3DX_PI / 180);	//y軸回転
//D3DXMatrixRotationX(&mtxRotation[0], g_angle * D3DX_PI / 180);	//x軸回転
//D3DXMatrixRotationZ(&mtxRotation[2], g_angle * D3DX_PI / 180);		//z軸回転
//D3DXMatrixTranslation(&mtxTrans[1], g_pos.x, g_pos.y, g_pos.z);		//平行移動
//mtxWorld = mtxRotation[2] * mtxTrans[1];				//行列の合成

	Cube_Draw(&mtxWorld);
}

//=====================================================
//授業課題
//=====================================================
void Cube_jyugyou(void)
{
	//ワールド座標変換
	D3DXMATRIX mtxWorld[8], mtxRotation[2][3], mtxTrans[8], mtxScaling[2];		//行列用変数
	for (int i = 0; i < 8; i++)
	{
		D3DXMatrixIdentity(&mtxWorld[i]);										//単位行列を作る
	}

	D3DXMatrixRotationX(&mtxRotation[0][0], 0 * D3DX_PI / 180);					//x軸回転
	D3DXMatrixRotationY(&mtxRotation[0][1], 0 * D3DX_PI / 180);					//y軸回転
	D3DXMatrixRotationZ(&mtxRotation[0][2], 0 * D3DX_PI / 180);					//z軸回転
	D3DXMatrixRotationY(&mtxRotation[1][1], g_angle * D3DX_PI / 180);			//y軸回転
	D3DXMatrixScaling(&mtxScaling[0], 1.0f, 1.0f, 1.0f);
	D3DXMatrixScaling(&mtxScaling[1], sin(g_scale) + 2, sin(g_scale) + 2, sin(g_scale) + 2);
	D3DXMatrixTranslation(&mtxTrans[0], g_pos.x, g_pos.y, g_pos.z);				//平行移動
	D3DXMatrixTranslation(&mtxTrans[1], -4.5f, 0.5f, -4.5f);					//平行移動

	D3DXMatrixTranslation(&mtxTrans[2], 4.5f - (sin(g_scale) + 1) / 2, 0.5f + (sin(g_scale) + 1) / 2, 4.5f - (sin(g_scale) + 1) / 2);	//平行移動

	D3DXMatrixTranslation(&mtxTrans[3], -4.5f, g_fall[0], 4.5f);				//平行移動
	D3DXMatrixTranslation(&mtxTrans[4], -4.5f, g_fall[1], 4.5f);				//平行移動
	D3DXMatrixTranslation(&mtxTrans[5], -4.5f, g_fall[2], 4.5f);				//平行移動
	D3DXMatrixTranslation(&mtxTrans[6], -4.5f, g_fall[3], 4.5f);				//平行移動
	D3DXMatrixTranslation(&mtxTrans[7], -4.5f, g_fall[4], 4.5f);				//平行移動

	//DebugFont_Draw(0, 32, "sin補正値 = %.02lf", (sin(g_scale) + 1) / 2);

	g_scale += 0.05f;

	//xが0～9でzが0の間は左移動
	//xが9でzが0～9の間は上移動
	//Xが9～0でzが0の間は右移動
	//xが0でzが9～0の間は下移動

	mtxWorld[0] = mtxScaling[0] * mtxRotation[0][0] * mtxRotation[0][1] * mtxRotation[0][2] * mtxTrans[0];		//行列の合成
	mtxWorld[1] = mtxScaling[0] * mtxRotation[1][1] * mtxTrans[1];												//行列の合成
	mtxWorld[2] = mtxScaling[1] * mtxRotation[0][0] * mtxRotation[0][1] * mtxRotation[0][2] * mtxTrans[2];		//行列の合成

	mtxWorld[3] = mtxScaling[0] * mtxRotation[0][0] * mtxRotation[0][1] * mtxRotation[0][2] * mtxTrans[3];		//行列の合成
	mtxWorld[4] = mtxScaling[0] * mtxRotation[0][0] * mtxRotation[0][1] * mtxRotation[0][2] * mtxTrans[4];		//行列の合成
	mtxWorld[5] = mtxScaling[0] * mtxRotation[0][0] * mtxRotation[0][1] * mtxRotation[0][2] * mtxTrans[5];		//行列の合成
	mtxWorld[6] = mtxScaling[0] * mtxRotation[0][0] * mtxRotation[0][1] * mtxRotation[0][2] * mtxTrans[6];		//行列の合成
	mtxWorld[7] = mtxScaling[0] * mtxRotation[0][0] * mtxRotation[0][1] * mtxRotation[0][2] * mtxTrans[7];		//行列の合成


	g_angle += 2.0f;

	switch (g_state)
	{
	case 0:
		g_pos.x -= 0.1f;
		if (g_pos.x < -4.4f)
		{
			g_state = 1;
		}
		break;
	case 1:
		g_pos.z += 0.1f;
		if (g_pos.z > 4.4f)
		{
			g_state = 2;
		}
		break;
	case 2:
		g_pos.x += 0.1f;
		if (g_pos.x > 4.4f)
		{
			g_state = 3;
		}
		break;
	case 3:
		g_pos.z -= 0.1f;
		if (g_pos.z < -4.4f)
		{
			g_state = 0;
		}
		break;
	}


	//積む処理
	//1個目
	if (g_fall[0] >= 0.5f)
	{
		g_fall[0] -= 0.1f;
	}
	//2個目
	else if (g_fall[1] >= 1.4f)
	{
		g_fall[1] -= 0.1f;
	}
	//3個目
	else if (g_fall[2] >= 2.3f)
	{
		g_fall[2] -= 0.1f;
	}
	//4個目
	else if (g_fall[3] >= 3.2f)
	{
		g_fall[3] -= 0.1f;
	}
	//5個目
	else if (g_fall[4] >= 4.1f)
	{
		g_fall[4] -= 0.1f;
		if (g_fall[4] < 4.1f)
		{
			for (int i = 0; i < 5; i++)
			{
				g_fall[i] = 10.0f;
			}
		}
	}

	Cube_Draw(&mtxWorld[0]);
	Cube_Draw(&mtxWorld[1]);
	Cube_Draw(&mtxWorld[2]);
	Cube_Draw(&mtxWorld[3]);	//1
	Cube_Draw(&mtxWorld[4]);	//2
	Cube_Draw(&mtxWorld[5]);	//3
	Cube_Draw(&mtxWorld[6]);	//4
	Cube_Draw(&mtxWorld[7]);	//5
}