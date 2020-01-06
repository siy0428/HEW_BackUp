#ifndef _GAME_H_
#define _GAME_H_

typedef enum
{
	SCENE_NONE,
	SCENE_TITLE,
	SCENE_TUTORIAL,
	SCENE_CAMERA,
	SCENE_MAIN,
	SCENE_RESULT
}SCENE;

void Game_Init(HWND hwnd);
void Game_Uninit(void);
void Game_Update(void);
void Game_Draw(void);
void Game_SceneChange(SCENE scene);
SCENE Game_GetScene(void);
HWND Game_GetHwnd(void);

#endif