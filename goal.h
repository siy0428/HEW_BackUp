#ifndef _GOAL_H_
#define _GOAL_H_

#include <d3dx9.h>

void Goal_Init(void);			//初期化
void Goal_Uninit(void);			//終了
void Goal_Update(void);			//更新
void Goal_Draw(void);			//描画
float Goal_Range(D3DXVECTOR3 pos);	//ストーンごとのゴールまでの距離
bool Goal_Flag(float goal_range, float move);			//ゴールしたかどうか

#endif