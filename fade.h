#ifndef _FADE_H_
#define _FADE_H_

#include "game.h"

void Fade_init(void);
void Fade_Uninit(void);
void Fade_Update(void);
void Fade_Draw(void);
bool Fade_isUse(void);
void Fade_InOut(SCENE scene);
void Fade_Half(void);
void Fade_Destroy(void);

#endif