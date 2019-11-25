#ifndef _STONE_H_
#define _STONE_H_

#include <d3dx9.h>

#define PLAYER_MAX_NUM (4)	//プレイ人数

void Stone_Init(void);										//初期化
void Stone_Uninit(void);									//終了
void Stone_Update(void);									//更新
void Stone_Draw(void);										//描画
D3DXVECTOR3 Stone_GetPos(int index);						//座標取得
int Stone_PlayerTurn(void);									//何Pが操作しているか
float Stone_Range(D3DXVECTOR3 pos, D3DXVECTOR3 old_pos);	//ストーンの進んだ距離
bool Stone_Move(int index);									//ストーンが動いてるかどうか
int Stone_Turn(void);

#endif