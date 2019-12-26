#ifndef _UI_PLAYER_TURN_H_
#define _UI_PLAYER_TURN_H_

//=====================================================
//—ñ‹“Œ^
//=====================================================
typedef enum
{
	UI_FIRST_TURN,
	UI_DIR_CHANGE,
	UI_RANGE,
	UI_STOP,
	UI_MAX,
	UI_NONE,
}UI_STATE;

void Ui_PlayerTurn_Init(void);
void Ui_PlayerTurn_Uninit(void);
void Ui_PlayerTurn_Update(void);
void Ui_PlayerTurn_Draw(void);
void Ui_PlayerTurn_Create(UI_STATE type);
UI_STATE Ui_PlayerTurn_GetType(void);
UI_STATE Ui_PlayerTurn_GetNextType(void);
bool UI_Player_Turn_Stop(void);

#endif