#include <d3dx9.h>
#include "mydirectx.h"
#include "common.h"
#include "debug_font.h"
#include "input.h"
#include "joycon.h"
#include "JoyInput.h"
#include "mouse.h"
#include "camera.h"
#include "stone.h"
#include "goal.h"
#include "joyInput.h"
#include "power_gauge.h"
#include "game.h"
#include "result.h"
#include "model.h"
#include "OXAllocateHierarchy.h"
#include "ui_player_turn.h"
#include "fade.h"
#include "penguin.h"

//====================================================
//マクロ定義
//====================================================
#define DEAD_ZONE (0.001f)	//小数点誤差用

//====================================================
//列挙型
//====================================================
typedef enum
{
	STONE_NORMAL,	//通常
	STONE_HEAVY,	//重い
	STONE_FLOAT,	//浮遊
	STONE_TYPE_MAX
}STONE_TYPE;

//=====================================================
//構造体宣言
//=====================================================
typedef struct
{
	D3DXMATRIX mtx_world;		//ワールド行列
	D3DXMATRIX mtx_rotation;	//回転行列
	D3DXMATRIX mtx_trans;		//平行移動
	D3DXMATRIX mtx_move;		//移動量
	D3DXVECTOR3 pos = { 0.0f, 0.5f, 0.0f };				//位置座標
	D3DXVECTOR3 start_pos = { 0.0f, 0.5f, 0.0f };		//1F前の位置座標
	unsigned int model[STONE_TYPE_MAX];
	STONE_TYPE type;
	int type_index;
	float move;
	float rot_move;
	bool is_turn;
	float goal_range;
	float stone_range;
	int score;
	int rank;
}Stone;

//=====================================================
//グローバル変数
//=====================================================
static Stone g_stone[PLAYER_MAX_NUM];
static int g_player_turn = 0;
static bool g_throw_start = false;
static float gyro_rotation = 0.0f;
static float rot_count = 0;
static int g_turn_count = 0;
static const char g_filename[16][256]
{
	//1P
	"model\\stone\\stone1p\\stone1p.x",
	"model\\stone\\omoi1p\\omoi1p.x",
	"model\\stone\\uiteru1p\\uiteru1p.x",
	//2P
	"model\\stone\\stone2p\\stone2p.x",
	"model\\stone\\omoi2p\\omoi2p.x",
	"model\\stone\\uiteru2p\\uiteru2p.x",
	//3P
	"model\\stone\\stone3p\\stone3p.x",
	"model\\stone\\omoi3p\\omoi3p.x",
	"model\\stone\\uiteru3p\\uiteru3p.x",
	//4P
	"model\\stone\\stone4p\\stone4p.x",
	"model\\stone\\omoi4p\\omoi4p.x",
	"model\\stone\\uiteru4p\\uiteru4p.x"
};
static const char g_pathname[16][256]
{
	//1P
	"model\\stone\\stone1p\\",
	"model\\stone\\omoi1p\\",
	"model\\stone\\uiteru1p\\",
	//2P
	"model\\stone\\stone2p\\",
	"model\\stone\\omoi2p\\",
	"model\\stone\\uiteru2p\\",
	//3P
	"model\\stone\\stone3p\\",
	"model\\stone\\omoi3p\\",
	"model\\stone\\uiteru3p\\",
	//4P
	"model\\stone\\stone4p\\",
	"model\\stone\\omoi4p\\",
	"model\\stone\\uiteru4p\\"
};

//=====================================================
//初期化
//=====================================================
void Stone_Init(void)
{
	//ストーンの初期化
	for (int i = 0; i < PLAYER_MAX_NUM; i++)
	{
		g_stone[i].pos = D3DXVECTOR3(-5.0f + i * 2.0f, 0.5f, -10.0f);	//座標
		g_stone[i].start_pos = g_stone[i].pos;
		D3DXMatrixTranslation(&g_stone[i].mtx_world, g_stone[i].pos.x, g_stone[i].pos.y, g_stone[i].pos.z);	//平行移動
		g_stone[i].move = 0.0f;			//移動量
		g_stone[i].rot_move = 0.0f;
		g_stone[i].is_turn = false;
		g_stone[i].goal_range = Goal_Range(g_stone[i].pos);
		g_stone[i].stone_range = 0.0f;
		g_stone[i].type = STONE_NORMAL;
		g_stone[i].type_index = g_stone[i].type;
		g_stone[i].score = 0;
		if (g_filename[i] != '\0')
		{
			g_stone[i].model[STONE_NORMAL] = Model_Load(g_filename[i * 3], g_pathname[i * 3]);			//通常タイプ
			g_stone[i].model[STONE_HEAVY] = Model_Load(g_filename[1 + i * 3], g_pathname[1 + i * 3]);	//重いタイプ
			g_stone[i].model[STONE_FLOAT] = Model_Load(g_filename[2 + i * 3], g_pathname[2 + i * 3]);	//浮遊タイプ
		}
	}
	g_stone[0].is_turn = true;		//最初のストーンのターン設定
	g_player_turn = 0;// Stone_Turn();
	g_throw_start = false;
	gyro_rotation = 0.0f;
	g_turn_count = 0;
}

//=====================================================
//終了
//=====================================================
void Stone_Uninit(void)
{

}
static int click;
//=====================================================
//更新
//=====================================================
void Stone_Update(void)
{
	//デバッグ用シーン切り替え
#if (DEBUG) || (_DEBUG)
	if (Keyboard_IsTrigger(DIK_SPACE))
	{
		for (int j = 0; j < 4; j++)
		{
			Result_SetScore(j, g_stone[j].score);
		}
		Result_GetRanking();	//順位ソート
		Goal_GetWinPlayer(g_player_turn);
		Fade_InOut(SCENE_RESULT);
	}
#endif
	//どのプレイヤーのターンか判別
	for (int i = 0; i < PLAYER_MAX_NUM; i++)
	{
		//ターンが来てなければ次のプレイヤーへ
		if (!g_stone[i].is_turn)
		{
			continue;
		}

		//UIが消えるまで操作不能
		if (Ui_PlayerTurn_GetType() == UI_FIRST_TURN || Ui_PlayerTurn_GetNextType() == UI_FIRST_TURN ||
			Ui_PlayerTurn_GetType() == UI_STOP || Ui_PlayerTurn_GetNextType() == UI_STOP)
		{
			return;
		}

		//ストーン切り替え
		switch (g_stone[i].type_index)
		{
		case 0:
			g_stone[i].type = STONE_NORMAL;
			break;
		case 1:
			g_stone[i].type = STONE_HEAVY;
			break;
		case 2:
			g_stone[i].type = STONE_FLOAT;
			break;
		default:
			break;
		}

		//ストーン切り替え(左)
		if (GetButton_isTrigger(JC_Y) && !Stone_Move(i) && !Penguin_OnStone(i))
		{
			g_stone[i].type_index = (g_stone[i].type_index + 1) % 3;
			Ui_PlayerTurn_StoneChange(false);
		}
		//ストーン切り替え(右)
		if (GetButton_isTrigger(JC_A) && !Stone_Move(i) && !Penguin_OnStone(i))
		{
			g_stone[i].type_index--;
			if (g_stone[i].type_index < 0)
			{
				g_stone[i].type_index = STONE_TYPE_MAX - 1;
			}
			Ui_PlayerTurn_StoneChange(true);
		}

		//マウスパラメータリセット
		if (GetButton_isTrigger(JC_X))
		{
			Mouse_Reset();
		}
		click = (bool)GetAsyncKeyState(VK_LBUTTON);
		//動いてないければ移動量の設定
		if (((bool)GetAsyncKeyState(VK_LBUTTON) || GetButton(JC_X)) && Stone_Range(g_stone[i].pos, g_stone[i].start_pos) <= DEAD_ZONE)
		{
			g_stone[i].move = 0.01f * Mouse_GetForce();
			rot_count = 0.075f * GetGyro().y / 1000;
			Pow_Gauge_Update();
			Ui_PlayerTurn_Create(UI_RANGE);
		}
		//ストーンの移動
		else
		{
			g_stone[i].rot_move = Joycon_Operator() * D3DX_PI / 180;	//回転値
			g_stone[i].move *= 0.98f;		//摩擦係数
			D3DXMatrixTranslation(&g_stone[i].mtx_move, 0, 0, g_stone[i].move);
			g_stone[i].goal_range = Goal_Range(g_stone[i].pos);	//ゴールまでの距離計算
			//投げていないときの処理
			if (!g_throw_start)
			{
				Ui_PlayerTurn_Create(UI_DIR_CHANGE);
				Pow_Gauge_Update();
			}
			//動いているとき
			if (g_stone[i].move >= DEAD_ZONE)
			{
				//投げるときUI
				g_throw_start = true;
				Ui_PlayerTurn_Create(UI_RANGE);
			}
			//ジャイロ回転のリセット
			else
			{
				rot_count = 0.0f;
			}
			//移動量がDEAD_ZONE以下で次のターン
			if (g_stone[i].move <= DEAD_ZONE && g_throw_start)
			{
				//ゴール判定
				if (Goal_Flag(g_stone[i].goal_range, g_stone[i].move))
				{
					for (int j = 0; j < 4; j++)
					{
						Result_SetScore(j, g_stone[i].score);
					}
					Result_GetRanking();	//順位ソート
					Goal_GetWinPlayer(g_player_turn);
					Fade_InOut(SCENE_RESULT);
					return;
				}
				Ui_PlayerTurn_Create(UI_STOP);
			}
			gyro_rotation += rot_count;
			rot_count *= 0.85f;
			D3DXMatrixRotationY(&g_stone[i].mtx_rotation, g_stone[i].rot_move + gyro_rotation);	//y軸回転
		}

		//ストーンの進んだ距離
		g_stone[i].stone_range = Stone_Range(g_stone[i].pos, g_stone[i].start_pos);

		//ワールド座標変換
		D3DXMatrixIdentity(&g_stone[i].mtx_world);	//単位行列を作る
		D3DXMatrixTranslation(&g_stone[i].mtx_world, g_stone[i].pos.x, g_stone[i].pos.y, g_stone[i].pos.z);		//平行移動

		g_stone[i].mtx_world = g_stone[i].mtx_move * g_stone[i].mtx_rotation * g_stone[i].mtx_world;

		//行列合成した状態での座標取得
		g_stone[i].pos.x = g_stone[i].mtx_world._41;
		g_stone[i].pos.y = g_stone[i].mtx_world._42;
		g_stone[i].pos.z = g_stone[i].mtx_world._43;
	}
}

//=====================================================
//描画
//=====================================================
void Stone_Draw(void)
{
	DebugFont_Draw(0, 32 * 20, "クリック = %d", click);
	for (int i = 0; i < PLAYER_MAX_NUM; i++)
	{
		DebugFont_Draw(0, 32 * (6 + i), "%dPのゴールまでの距離 = %.02lf", i + 1, g_stone[i].goal_range);
		//DebugFont_Draw(0, 32 * (6 + i), "%dPの進んだ距離 = %.02lf", i + 1, g_stone[i].stone_range);

		Model_Draw(g_stone[i].model[g_stone[i].type], g_stone[i].mtx_world);
	}
}

//=====================================================
//座標取得
//=====================================================
D3DXVECTOR3 Stone_GetPos(int index)
{
	return g_stone[index].pos;
}

//=====================================================
//何Pが操作中か
//=====================================================
int Stone_PlayerTurn(void)
{
	return g_player_turn;
}

//=====================================================
//ストーンの進んだ距離
//=====================================================
float Stone_Range(D3DXVECTOR3 pos, D3DXVECTOR3 start_pos)
{
	return (float)sqrt((pos.x - start_pos.x) * (pos.x - start_pos.x) + (pos.y - start_pos.y) * (pos.y - start_pos.y) + (pos.z - start_pos.z) * (pos.z - start_pos.z));
}

//=====================================================
//ストーンの進んだ距離の取得
//=====================================================
float Stone_GetRange(int index)
{
	return g_stone[index].stone_range;
}

//=====================================================
//ストーンが動いているかどうか
//=====================================================
bool Stone_Move(int index)
{
	//ストーンの移動量がDEAD_ZONE以下であれば静止中と判断
	if (g_stone[index].move <= DEAD_ZONE)
	{
		return false;
	}
	return true;
}

//=====================================================
//ゴールの距離に応じてストーンの順番処理
//=====================================================
int Stone_Turn(void)
{
	int index = 0;

	for (int i = 1; i < PLAYER_MAX_NUM; i++)
	{
		//次の比較対象の方がゴールより遠い場合
		if (Goal_Range(g_stone[i].pos) > Goal_Range(g_stone[index].pos))
		{
			index = i;
		}
	}
	return index;
}

//=====================================================
//回転量取得
//=====================================================
float Stone_Rot(int index)
{
	return g_stone[index].rot_move;
}

//=====================================================
//次のストーンの設定
//=====================================================
void Stone_SetTurn(void)
{
	g_stone[g_player_turn].is_turn = false;									//現在操作しているストーンのターンをfalse
	g_stone[g_player_turn].start_pos = g_stone[g_player_turn].pos;			//スタート位置に今の座標を代入
	g_stone[g_player_turn].move = 0.0f;										//移動量の初期化
	g_stone[g_player_turn].score++;											//スコアの加算
	g_turn_count++;			//ターン数カウント
	(g_turn_count < 4) ? g_player_turn++ : g_player_turn = Stone_Turn();	//1巡目は1～4Pの順で、それ以降はゴールから遠い順
	g_stone[g_player_turn].is_turn = true;									//プラスされた現在のプレイヤーのターンをtrueに
	g_throw_start = false;													//スタート用変数をfalse
	Joycon_Reset();			//スティック回転値リセット
	rot_count = 0.0f;		//回転量リセット
	gyro_rotation = 0.0f;	//回転値リセット
}

//=====================================================
//スコアの取得
//=====================================================
int Stone_GetScore(int index)
{
	return g_stone[index].score;
}