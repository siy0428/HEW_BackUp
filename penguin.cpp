#include "model.h"
#include "cube.h"
#include "debug_font.h"
#include "input.h"

//====================================================
//マクロ定義
//====================================================
#define PENGUIN_MAX (4)

//====================================================
//列挙型
//====================================================
//右と左
typedef enum
{
	LEFT,
	RIGHT,
	LEG_MAX
}LEG_LR;

//ペンギンのアニメーション状態
typedef enum
{
	IDOLING,
	STEP
}ANIMETION_TYPE;

//====================================================
//構造体宣言
//====================================================

//ペンギンの足
typedef struct
{
	D3DXMATRIX mtxWorld;
	D3DXMATRIX mtxMove;
	D3DXMATRIX mtxRot;
	D3DXMATRIX mtxScale;
	D3DXMATRIX mtxTransBody;
	D3DXMATRIX mtxSin;
	D3DXVECTOR3 pos;
}PENGUIN_LEG;

//ペンギンの体
typedef struct
{
	D3DXMATRIX  mtxWorld;
	D3DXMATRIX  mtxTrans;
	D3DXMATRIX  mtxMove;
	D3DXMATRIX  mtxRot;
	D3DXMATRIX  mtxRotCenter;
	D3DXMATRIX  mtxScale;
	D3DXVECTOR3 pos;
}PENGUIN_BODY;

//ペンギン
typedef struct
{
	PENGUIN_LEG penguin_leg[LEG_MAX];
	PENGUIN_BODY penguin_body;
	D3DXMATRIX rot;
	bool animation = false;
	float move;
	float rot_speed;
	float rot_center;
	float anim_move;
	ANIMETION_TYPE type;
	LEG_LR leg_index;
}PENGUIN;

//====================================================
//グローバル変数
//====================================================
static unsigned int g_modelId = NULL;
static PENGUIN *g_penguin;
static int g_frame = 0;

//====================================================
//初期化
//====================================================
void Penguin_Init(void)
{
	//動的メモリ確保
	g_penguin = new PENGUIN[PENGUIN_MAX];
	//体初期化
	for (int i = 0; i < PENGUIN_MAX; i++)
	{
		g_penguin[i].penguin_body.pos = D3DXVECTOR3(0.0f + i * -5.0f, 2.0f, 0.0f);
		g_penguin[i].move = 0.0f;
		g_penguin[i].rot_speed = 0.0f;
		g_penguin[i].animation = false;
		g_penguin[i].anim_move = 0.0f;
		g_penguin[i].rot_center = 0.5f;
		g_penguin[i].leg_index = LEFT;
		//両足の初期化
		for (int j = 0; j < LEG_MAX; j++)
		{
			g_penguin[i].penguin_leg[j].pos = D3DXVECTOR3(-1.0f + j * 2.0f, 1.0f, 0.0f);
		}
	}
	g_frame = 0;
}

//====================================================
//終了
//====================================================
void Penguin_Uninit(void)
{
	delete[] g_penguin;
}

//====================================================
//更新
//====================================================
void Penguin_Update(void)
{
	for (int i = 0; i < PENGUIN_MAX; i++)
	{
		//テスト用キー操作
		if (Keyboard_IsPress(DIK_U))
		{
			g_penguin[i].move = 0.05f;
		}
		else if (Keyboard_IsPress(DIK_J))
		{
			g_penguin[i].move = -0.05f;
		}
		D3DXMatrixTranslation(&g_penguin[i].penguin_body.mtxMove, 0.0f, 0.0f, g_penguin[i].move);
		g_penguin[i].move *= 0.8f;
		//左回転
		if (Keyboard_IsPress(DIK_H))
		{
			g_penguin[i].rot_speed += -0.05f;
		}
		//右回転
		else if (Keyboard_IsPress(DIK_K))
		{
			g_penguin[i].rot_speed += 0.05f;
		}
		D3DXMatrixRotationY(&g_penguin[i].rot, g_penguin[i].rot_speed);

		//体
		//D3DXMatrixTranslation(&g_penguin[i].penguin_body.mtxWorld, g_penguin[i].penguin_body.pos.x - g_penguin[i].rot_center, g_penguin[i].penguin_body.pos.y, g_penguin[i].penguin_body.pos.z);
		D3DXMatrixTranslation(&g_penguin[i].penguin_body.mtxTrans, g_penguin[i].penguin_body.pos.x, g_penguin[i].penguin_body.pos.y, g_penguin[i].penguin_body.pos.z);
		
		//両足
		for (int j = 0; j < LEG_MAX; j++)
		{
			D3DXMatrixTranslation(&g_penguin[i].penguin_leg[j].mtxWorld, g_penguin[i].penguin_leg[j].pos.x, g_penguin[i].penguin_leg[j].pos.y, g_penguin[i].penguin_leg[j].pos.z);
			if (j == LEFT)
			{
				D3DXMatrixTranslation(&g_penguin[i].penguin_leg[j].mtxTransBody, -1.0f, 0.0f, 0.0f);
			}
			else if (j = RIGHT)
			{
				D3DXMatrixTranslation(&g_penguin[i].penguin_leg[j].mtxTransBody, 1.0f, 0.0f, 0.0f);
			}
		}
		//体の回転
		D3DXMatrixRotationZ(&g_penguin[i].penguin_body.mtxRot, (FLOAT)sin(g_penguin[i].anim_move) / 8);

		//sinが0付近になったら中心切り替え
		//左足上げ
		if (sin(g_penguin[0].anim_move) / 8 >= 0.0f)
		{
			g_penguin[i].rot_center = 0.5f;
			g_penguin[i].leg_index = RIGHT;
		}
		//右足上げ
		else
		{
			g_penguin[i].rot_center = -0.5f;
			g_penguin[i].leg_index = LEFT;
		}
		//中心移動
		D3DXMatrixTranslation(&g_penguin[i].penguin_body.mtxRotCenter, g_penguin[i].rot_center, 0.0f, 0.0f);

		//足の動き
		D3DXMatrixTranslation(&g_penguin[i].penguin_leg[LEFT].mtxSin, 0.0f, 0.0f, (float)cos(g_penguin[i].anim_move) / 2);
		D3DXMatrixTranslation(&g_penguin[i].penguin_leg[RIGHT].mtxSin, 0.0f, 0.0f, -(float)cos(g_penguin[i].anim_move) / 2);

		g_penguin[i].penguin_leg[LEFT].pos.z = g_penguin[i].penguin_body.pos.z;
		g_penguin[i].penguin_leg[RIGHT].pos.z = g_penguin[i].penguin_body.pos.z;
		g_penguin[i].penguin_leg[LEFT].pos.y = g_penguin[i].penguin_body.pos.y - 1.0f - (float)sin(g_penguin[i].anim_move) / 4;
		g_penguin[i].penguin_leg[RIGHT].pos.y = g_penguin[i].penguin_body.pos.y - 1.0f + (float)sin(g_penguin[i].anim_move) / 4;
		g_penguin[i].penguin_leg[LEFT].pos.x = g_penguin[i].penguin_body.pos.x;
		g_penguin[i].penguin_leg[RIGHT].pos.x = g_penguin[i].penguin_body.pos.x;
		g_penguin[i].anim_move += 0.05f;

		//行列合成
		//体
		//g_penguin[i].penguin_body.mtxWorld = g_penguin[i].penguin_body.mtxMove * g_penguin[i].rot * g_penguin[i].penguin_body.mtxRotCenter * g_penguin[i].penguin_body.mtxRot * g_penguin[i].penguin_body.mtxWorld;
		g_penguin[i].penguin_body.mtxWorld = g_penguin[i].penguin_body.mtxRot * g_penguin[i].penguin_body.mtxMove * g_penguin[i].rot * g_penguin[i].penguin_body.mtxTrans;
		//足
		g_penguin[i].penguin_leg[g_penguin[i].leg_index].mtxWorld = g_penguin[i].penguin_body.mtxRot * g_penguin[i].penguin_leg[g_penguin[i].leg_index].mtxWorld;
		for (int j = 0; j < LEG_MAX; j++)
		{
			g_penguin[i].penguin_leg[j].mtxWorld = g_penguin[i].penguin_leg[j].mtxTransBody * g_penguin[i].penguin_leg[j].mtxSin * g_penguin[i].rot * g_penguin[i].penguin_leg[j].mtxWorld;
		}

		//行列合成後の座標更新
		g_penguin[i].penguin_body.pos.x = g_penguin[i].penguin_body.mtxWorld._41;
		g_penguin[i].penguin_body.pos.z = g_penguin[i].penguin_body.mtxWorld._43;
	}
}

//====================================================
//描画
//====================================================
void Penguin_Draw(void)
{
	for (int i = 0; i < PENGUIN_MAX; i++)
	{
		//体
		Cube_Draw(&g_penguin[i].penguin_body.mtxWorld);
		//足
		for (int j = 0; j < LEG_MAX; j++)
		{
			Cube_Draw(&g_penguin[i].penguin_leg[j].mtxWorld);
		}
	}
	DebugFont_Draw(0, 32 * 3, "体 = %.02lf", sin(g_penguin[0].anim_move) / 8);
	DebugFont_Draw(0, 32 * 4, "脚 = %.02lf", sin(g_penguin[0].anim_move) / 8);
}