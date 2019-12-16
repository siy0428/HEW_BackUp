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
#define MOUSE_MAX_FORCE (250 * MOUSE_SAVE_FRAME)	//�ő�p���[

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
MOUSE g_mouse;
POINT pt;
static int g_loop_count = 0;
static int g_mouse_save[MOUSE_SAVE_FRAME] = { 0 };
static int g_index = 0;

//=====================================================
//������
//=====================================================
void Mouse_Init(void)
{
	g_mouse.pos_x = 0;
	g_mouse.pos_y = 0;
	g_mouse.cursor = TRUE;
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
	//�}�E�X�̍��W�擾
	GetCursorPos(&pt);

	//�Q�[�����}�E�X�p�����[�^
	g_mouse.pos_x = pt.x;
	g_mouse.pos_y = MOUSE_MAX_HEIGHT - pt.y + (MOUSE_MAX_HEIGHT * g_loop_count);	//�㉺���]

	//�}�E�X�ニ�[�v
	if (g_mouse.pos_y >= MOUSE_MAX_HEIGHT * (g_loop_count + 1))
	{
		SetCursorPos(pt.x, MOUSE_MAX_HEIGHT);	//��ʉ��Ɏ����Ă���
		g_loop_count++;	//��ʓ����[�v��
	}
	//�}�E�X�����[�v
	else if ((g_mouse.pos_y - MOUSE_MAX_HEIGHT * g_loop_count <= 0.0f) && (g_mouse.pos_y > 0.0f))
	{
		SetCursorPos(pt.x, 0);	//��ʏ�Ɏ����Ă���
		g_loop_count--;	//��ʓ����[�v��
	}

	//�}�E�X�p�����[�^���Z�b�g
	if (Keyboard_IsTrigger(DIK_R))
	{
		g_mouse.pos_y = 0;
		g_loop_count = 0;
	}

	//�}�E�X�̍��W��10F�ԋL��
	if (g_index < MOUSE_SAVE_FRAME)
	{
		g_mouse_save[MOUSE_SAVE_FRAME - 1 - g_index] = g_mouse.pos_y;
		g_index++;
	}
	//10F�L���������납�琮��
	else
	{
		for (int i = MOUSE_SAVE_FRAME - 1; i > 0; i--)
		{
			g_mouse_save[i] = g_mouse_save[i - 1];
		}
		g_mouse_save[0] = g_mouse.pos_y;
	}
}

//=====================================================
//�`��
//=====================================================
void Mouse_Draw(void)
{
	//�}�E�X�̍��W�\��
	DebugFont_Draw(0, 32 * 0, "force = %d", Mouse_GetForce());
	DebugFont_Draw(0, 32 * 1, "x:%d", g_mouse.pos_x);
	DebugFont_Draw(0, 32 * 2, "y:%d(���])", g_mouse.pos_y);

}

//=====================================================
//�}�E�X��x�����W�擾
//=====================================================
int Mouse_GetPos_X(void)
{
	return pt.x;
}

//=====================================================
//�}�E�X��y�����W�擾
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
	int force = g_mouse_save[0] - g_mouse_save[MOUSE_SAVE_FRAME - 1];

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

//=====================================================
//�}�E�X�p�����[�^���Z�b�g
//=====================================================
void Mouse_Reset(void)
{
	for (int i = 0; i < MOUSE_SAVE_FRAME - 1; i++)
	{
		g_mouse_save[i] = 0;
	}
	g_index = 0;
}