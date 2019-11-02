#include "debug_font.h"
#include <windows.h>
#include "d3dx9.h"
#include "input.h"

//=====================================================
//�}�N����`
//=====================================================
#define MOUSE_MAX_HEIGHT (767)	//�}�E�X�ړ����(����:y��)

//=====================================================
//�\���̐錾
//=====================================================
typedef struct
{
	D3DXVECTOR2 pos;
	BOOL cursor;
}MOUSE;

//=====================================================
//�O���[�o���ϐ�
//=====================================================
MOUSE g_Mouse;
POINT pt;
static int g_LoopCount = 0;
static char g_DebugString[64] = {"�\��"};

//=====================================================
//������
//=====================================================
void Mouse_Init(void)
{
	g_Mouse.pos = D3DXVECTOR2(0.0f, 0.0f);
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

	//�}�E�X�J�[�\���؂�ւ�
	ShowCursor(g_Mouse.cursor);
	if (Keyboard_IsTrigger(DIK_M))
	{
		if (g_Mouse.cursor)
		{
			strcpy(g_DebugString, "�\��");
		}
		else
		{
			strcpy(g_DebugString, "��\��");
		}
		(g_Mouse.cursor) ? g_Mouse.cursor = FALSE : g_Mouse.cursor = TRUE;
	}

	//�}�E�X�̍��W�擾
	GetCursorPos(&pt);

	//�Q�[�����}�E�X�p�����[�^
	g_Mouse.pos.x = (float)pt.x;
	g_Mouse.pos.y = MOUSE_MAX_HEIGHT + (float)pt.y * -1 + (MOUSE_MAX_HEIGHT * g_LoopCount);

	//�}�E�X�ニ�[�v
	if (g_Mouse.pos.y >= MOUSE_MAX_HEIGHT * (g_LoopCount + 1))
	{
		SetCursorPos((int)pt.x, MOUSE_MAX_HEIGHT);
		g_LoopCount++;
	}
	//�}�E�X�����[�v
	else if ((g_Mouse.pos.y - MOUSE_MAX_HEIGHT * g_LoopCount <= 0.0f) && (g_Mouse.pos.y > 0.0f))
	{
		SetCursorPos((int)pt.x, 0);
		g_LoopCount--;
	}
	
}

//=====================================================
//�`��
//=====================================================
void Mouse_Draw(void)
{
	//�}�E�X�̍��W�\��
	DebugFont_Draw(0, 32 * 0, "M�L�[:�J�[�\��%s", g_DebugString);
	DebugFont_Draw(0, 32 * 1, "x:%.02lf", g_Mouse.pos.x);
	DebugFont_Draw(0, 32 * 2, "y:%.02lf(���])", g_Mouse.pos.y);
}

//=====================================================
//x�����W�擾
//=====================================================
int Mouse_GetPos_X(void)
{
	return (int)pt.x;
}

//=====================================================
//y�����W�擾
//=====================================================
int Mouse_GetPos_Y(void)
{
	return (int)pt.y;
}