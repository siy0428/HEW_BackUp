#ifndef _STONE_H_
#define _STONE_H_

#include <d3dx9.h>

void Stone_Init(void);				//初期化
void Stone_Uninit(void);			//終了
void Stone_Update(void);			//更新
void Stone_Draw(void);				//描画
D3DXVECTOR3 Stone_GetPos(void);		//座標取得

#endif