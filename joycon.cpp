#include "input.h"
#include "dinput.h"
#include "JoyInput.h"

//=====================================================
//�}�N����`
//=====================================================
#define STICK_SPEED (1.0f)

//=====================================================
//�O���[�o���ϐ�
//=====================================================
static float g_stick_y = 0;

//=====================================================
//������
//=====================================================
void Joycon_Init(void)
{
	g_stick_y = 0.0f;
}

//=====================================================
//�W���C�R������
//=====================================================
float Joycon_Operator(void)
{
	//�X�e�B�b�N����
	g_stick_y += GetStick().x / 500;

	//�f�o�b�O�p��]
#if (DEBUG) || (_DEBUG)
	if (Keyboard_IsPress(DIK_LEFTARROW))
	{
		g_stick_y -= STICK_SPEED;
	}
	else if (Keyboard_IsPress(DIK_RIGHTARROW))
	{
		g_stick_y += STICK_SPEED;
	}
#endif	(DEBUG) || (_DEBUG)

	return g_stick_y;
}

//=====================================================
//��]�ʃ��Z�b�g
//=====================================================
void Joycon_Reset(void)
{
	g_stick_y = 0.0f;
}