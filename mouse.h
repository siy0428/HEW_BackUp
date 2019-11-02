#ifndef _MOUSE_H_
#define _MOUSE_H_

void Mouse_Init(void);		//初期化
void Mouse_Uninit(void);	//終了
void Mouse_Update(void);	//更新
void Mouse_Draw(void);		//描画
int Mouse_GetPos_X(void);	//マウスのx軸方向座標取得
int Mouse_GetPos_Y(void);	//マウスのy軸方向座標取得
int Mouse_GetForce(void);	//マウスのスクロールで0～100の値を返す

#endif