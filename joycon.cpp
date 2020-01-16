#include "input.h"
#include "dinput.h"
#include "JoyInput.h"
#include "debug_font.h"

//=====================================================
//�}�N����`
//=====================================================
#define STICK_SPEED (1.0f)
#define STICK_DEFAULT_STICK (0.0f)

//=====================================================
//�O���[�o���ϐ�
//=====================================================

//=====================================================
//������
//=====================================================
void Joycon_Init(void)
{
	
}

//=====================================================
//�W���C�R������
//=====================================================
float Joycon_Operator(void)
{
	//�X�e�B�b�N����
	float stick_y;
	stick_y = GetStick().x / 500;

	//�f�o�b�O�p��]
#if (DEBUG) || (_DEBUG)
	if (Keyboard_IsPress(DIK_LEFTARROW))
	{
		stick_y = -STICK_SPEED;
	}
	else if (Keyboard_IsPress(DIK_RIGHTARROW))
	{
		stick_y = STICK_SPEED;
	}
#endif	(DEBUG) || (_DEBUG)

	return stick_y;
}