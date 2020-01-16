//------------------------------------------------------------------------------
//	共通事項用ヘッダ
//	
//	クラス	：AT-12E-353
//	出席番号：16
//	名前	：木村　和
//
//------------------------------------------------------------------------------
#ifndef _COMMON_H_
#define _COMMON_H_

#include "mydirectx.h"
#include "texture.h"
#include "sprite.h"
#include "model.h"

//----------------------
//定数定義
//----------------------
#define SCREEN_WIDTH (1280)				//クライアントの幅	1280
#define SCREEN_HEIGHT (720)				//クライアントの高さ 720
#define CENTER_X (SCREEN_WIDTH / 2)	//画面中央X
#define CENTER_Y (SCREEN_HEIGHT / 2)	//画面中央Y
#define RIGHT (true)
#define LEFT (false)
#define UP (true)
#define DOWN (false)


#define DEF_COLOR D3DCOLOR_RGBA(255, 255, 255, 255)

#endif	//_COMMON_H_