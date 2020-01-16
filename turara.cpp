#include "common.h"
#include "stone.h"

//====================================================
//マクロ定義
//====================================================
#define TURARA_MAX (16)
#define MODEL_NAME "model\\turara\\turara.x"
#define MODEL_PATH "model\\turara\\"

#define TENJOU (20.0f)
#define FALL_SPEED (-0.2f)
#define FALL_SET_RANGE (1.5f)

//====================================================
//構造体宣言
//====================================================
typedef struct
{
	D3DXMATRIX mtxWorld;
	float stop_y;
	bool isUse;
	bool set_fall;
	bool fall;
}TURARA;

//====================================================
//グローバル変数
//====================================================
static int modelId;
static TURARA g_turara[TURARA_MAX];

//====================================================
//初期化
//====================================================
void Turara_Init(void)
{
	modelId = Model_Load(MODEL_NAME, MODEL_PATH);
	for (int i = 0; i < TURARA_MAX; i++)
	{
		g_turara[i].isUse = false;
		g_turara[i].set_fall = false;
		g_turara[i].fall = false;
	}
	return;
}

//====================================================
//終了
//====================================================
void Turara_Uninit(void)
{

}

//====================================================
//更新
//====================================================
void Turara_Update(void)
{
	D3DXMATRIX mtxWork1;
	D3DXVECTOR3 vecWork1;
	bool boolWork;

	D3DXVECTOR3 vec_stone_pos[4];


	D3DXMatrixTranslation(&mtxWork1, 0.0f, FALL_SPEED, 0.0f);

	for (int i = 0; i < 4; i++) {
		vec_stone_pos[i] = Stone_GetPos(i);
		vec_stone_pos[i].y = 0.0f;
	}


	for (int i = 0; i < TURARA_MAX; i++)
	{
		if (!g_turara[i].isUse)
		{
			continue;
		}
		vecWork1 = D3DXVECTOR3(g_turara[i].mtxWorld._41, 0.0f, g_turara[i].mtxWorld._43);
		if (!g_turara[i].set_fall) {
			for (int j = 0; j < 4; j++) {
				if (D3DXVec3LengthSq(&(vecWork1 - vec_stone_pos[j])) <= (FALL_SET_RANGE * FALL_SET_RANGE)) {
					g_turara[i].set_fall = true;
					break;
				}
			}
			continue;
		}
		if (!g_turara[i].fall) {
			boolWork = false;
			for (int j = 0; j < 4; j++) {
				if (D3DXVec3LengthSq(&(vecWork1 - vec_stone_pos[j])) <= (FALL_SET_RANGE * FALL_SET_RANGE)) {
					boolWork = true;
				}
			}
			if (! boolWork) {
				g_turara[i].fall = true;
			}

		}
		else {
			if (g_turara[i].mtxWorld._42 <= g_turara[i].stop_y) {
				continue;
			}
			g_turara[i].mtxWorld *= mtxWork1;
			if (g_turara[i].mtxWorld._42 <= g_turara[i].stop_y) {
				g_turara[i].mtxWorld._42 = g_turara[i].stop_y;
			}
		}
	}
}

//====================================================
//描画
//====================================================
void Turara_Draw(void)
{
	for (int i = 0; i < TURARA_MAX; i++)
	{
		if (!g_turara[i].isUse)
		{
			continue;
		}
		Model_Draw(modelId, g_turara[i].mtxWorld);
	}
}

//====================================================
//ステージエディタのデータ取得
//====================================================
void Turara_Set(D3DXMATRIX mtxWorld)
{
	for (int i = 0; i < TURARA_MAX; i++)
	{
		if (g_turara[i].isUse)
		{
			continue;
		}
		g_turara[i].mtxWorld = mtxWorld;
		g_turara[i].stop_y = g_turara[i].mtxWorld._42;
		g_turara[i].mtxWorld._42 = TENJOU;
		g_turara[i].isUse = true;
		return;
	}
}