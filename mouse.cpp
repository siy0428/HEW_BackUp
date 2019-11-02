#include "debug_font.h"
#include <windows.h>
#include "d3dx9.h"
#include "input.h"
#include "xinput.h"
#include "mouse.h"

//=====================================================
//�}�N����`
//=====================================================
#define MOUSE_MAX_HEIGHT (767)	//�}�E�X�ړ����(����:y��)
#define MOUSE_SAVE_FRAME (10)	//�}�E�X�̍��W�̋L���t���[����
#define MOUSE_MAX_FORCE (3800)	//�ő�p���[

//=====================================================
//�\���̐錾
//=====================================================
typedef struct
{
	int pos_x;
	int pos_y;
	BOOL cursor;
}MOUSE;

//=====================================================
//�O���[�o���ϐ�
//=====================================================
MOUSE g_Mouse;
POINT pt;
static int g_LoopCount = 0;
static int g_Mouse_Save[MOUSE_SAVE_FRAME] = { 0 };
static int g_Index = 0;

//=====================================================
//������
//=====================================================
void Mouse_Init(void)
{
	g_Mouse.pos_x = 0;
	g_Mouse.pos_y = 0;
	g_Mouse.cursor = TRUE;
}

//=====================================================
//�I��
//=====================================================
void Mouse_Uninit(void)
{

}

//=====================================================
//�X�V
//=====================================================
void Mouse_Update(void)
{
	//�L�[�{�[�h�X�V
	Keyboard_Update();

	//�}�E�X�̍��W�擾
	GetCursorPos(&pt);

	//�Q�[�����}�E�X�p�����[�^
	g_Mouse.pos_x = pt.x;
	g_Mouse.pos_y = MOUSE_MAX_HEIGHT + pt.y * -1 + (MOUSE_MAX_HEIGHT * g_LoopCount);	//�㉺���]
	//Sleep(250);

	//�}�E�X�ニ�[�v
	if (g_Mouse.pos_y >= MOUSE_MAX_HEIGHT * (g_LoopCount + 1))
	{
		SetCursorPos(pt.x, MOUSE_MAX_HEIGHT);	//��ʉ��Ɏ����Ă���
		g_LoopCount++;	//��ʓ����[�v��
	}
	//�}�E�X�����[�v
	else if ((g_Mouse.pos_y - MOUSE_MAX_HEIGHT * g_LoopCount <= 0.0f) && (g_Mouse.pos_y > 0.0f))
	{
		SetCursorPos(pt.x, 0);	//��ʏ�Ɏ����Ă���
		g_LoopCount--;	//��ʓ����[�v��
	}

	//�}�E�X�p�����[�^���Z�b�g
	if (Keyboard_IsTrigger(DIK_R))
	{
		g_Mouse.pos_y = 0;
		g_LoopCount = 0;
	}

	//�}�E�X�̍��W��10F�ԋL��
	if (g_Index < MOUSE_SAVE_FRAME)
	{
		g_Mouse_Save[g_Index] = g_Mouse.pos_y;
		g_Index++;
	}
	//10F�L���������납�琮��
	else
	{
		for (int i = MOUSE_SAVE_FRAME - 1; i > 0; i--)
		{
			g_Mouse_Save[i] = g_Mouse_Save[i - 1];
		}
		g_Mouse_Save[0] = g_Mouse.pos_y;
	}
}

//=====================================================
//�`��
//=====================================================
void Mouse_Draw(void)
{
	//�}�E�X�̍��W�\��
	DebugFont_Draw(0, 32 * 0, "R�L�[:Y���p�����[�^���Z�b�g");
	DebugFont_Draw(0, 32 * 1, "x:%d", g_Mouse.pos_x);
	DebugFont_Draw(0, 32 * 2, "y:%d(���])", g_Mouse.pos_y);
	for (int i = 0; i < MOUSE_SAVE_FRAME; i++)
	{
		DebugFont_Draw(0, 32 * (i + 3), "[%d] = %d", i, g_Mouse_Save[i]);
	}
	DebugFont_Draw(0, 32 * 13, "�� = %d", Mouse_GetForce());
}

//=====================================================
//x�����W�擾
//=====================================================
int Mouse_GetPos_X(void)
{
	return pt.x;
}

//=====================================================
//y�����W�擾
//=====================================================
int Mouse_GetPos_Y(void)
{
	return pt.y;
}

//=====================================================
//�}�E�X�̃X�N���[����0�`100�̒l��Ԃ�
//=====================================================
int Mouse_GetForce(void)
{
	//���̍��W����10F�O�̍��W�������ăp���[�Z�o
	int force = g_Mouse_Save[0] - g_Mouse_Save[MOUSE_SAVE_FRAME - 1];

	//�����ݒ�
	if (force < 0)
	{
		force = 0;
	}
	//����ݒ�
	else if (force >= MOUSE_MAX_FORCE)
	{
		force = MOUSE_MAX_FORCE;
	}

	//0�`100�ɐݒ�
	force = 100 * force / MOUSE_MAX_FORCE;

	return force;
}