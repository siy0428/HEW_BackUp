#include "common.h"
#include "input.h"
#include "debug_font.h"

//=====================================================
//�}�N����`
//=====================================================
#define MOVE_SPEED (0.2f)	//�ړ���
#define ROT_SPEED (0.02f)	//��]��

//=====================================================
//�O���[�o���ϐ�
//=====================================================
static D3DXVECTOR3 g_vecFront(0.0f, 0.0f, 0.0f);
static D3DXVECTOR3 g_vecRight(0.0f, 0.0f, 0.0f);
static D3DXVECTOR3 g_vecUp(0.0f, 0.0f, 0.0f);
static D3DXVECTOR3 g_Pos(0.0f, 0.0f, 0.0f);
static float g_moveSpd = 0.0f;
static D3DXVECTOR3 g_rotSpd = { 0.0f, 0.0f, 0.0f };
static float g_Fov = 0.0f;
static D3DXVECTOR3 g_Move(0.0f, 0.0f, 0.0f);
D3DXVECTOR3 vecDir(0.0f, 0.0f, 0.0f);
static D3DXVECTOR3 g_LookAt(0.0f, 0.0f, 0.0f);
static D3DXMATRIX g_mtxView;		//�r���[�ϊ��s��p�ϐ�

//=====================================================
//������
//=====================================================
void dCamera_Init(void)
{
	g_vecFront = D3DXVECTOR3(0.0f, -1.0f, 1.0f);
	g_vecRight = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
	D3DXVec3Normalize(&g_vecFront, &g_vecFront);			//�O�x�N�g���̐��K��
	D3DXVec3Cross(&g_vecUp, &g_vecFront, &g_vecRight);		//���p�̃x�N�g���𓾂邽�߂̊O��
	D3DXVec3Normalize(&g_vecUp, &g_vecUp);					//�O�̈א��K��
	g_Pos = D3DXVECTOR3(0.0f, 10.0f, -10.0f);
	g_moveSpd = MOVE_SPEED;
	g_rotSpd = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_Fov = 1.0f;
}

//=====================================================
//�J�����ݒ�
//=====================================================
void dCamera_Set(void)
{

	//�O����
	if (Keyboard_IsPress(DIK_W))
	{
		vecDir += g_vecFront;
		g_moveSpd = MOVE_SPEED;
	}
	//������
	if (Keyboard_IsPress(DIK_S))
	{
		vecDir += -g_vecFront;
		g_moveSpd = MOVE_SPEED;
	}
	//������
	if (Keyboard_IsPress(DIK_A))
	{
		vecDir += -g_vecRight;
		g_moveSpd = MOVE_SPEED;
	}
	//�E����
	if (Keyboard_IsPress(DIK_D))
	{
		vecDir += g_vecRight;
		g_moveSpd = MOVE_SPEED;
	}
	//�����
	if (Keyboard_IsPress(DIK_LSHIFT) && Keyboard_IsPress(DIK_W))
	{
		vecDir += g_vecUp;
		g_moveSpd = MOVE_SPEED;
	}
	//������
	if (Keyboard_IsPress(DIK_LSHIFT) && Keyboard_IsPress(DIK_S))
	{
		vecDir += -g_vecUp;
		g_moveSpd = MOVE_SPEED;
	}

	//���K��
	D3DXVec3Normalize(&vecDir, &vecDir);
	g_Pos += vecDir * g_moveSpd;
	g_moveSpd *= 0.9f;

	D3DXMATRIX mtxRot;

	//�����_�E��]
	if (Keyboard_IsPress(DIK_LSHIFT) && Keyboard_IsPress(DIK_RIGHTARROW))
	{
		g_rotSpd.y = ROT_SPEED * 2;
	}
	//�����_����]
	else if (Keyboard_IsPress(DIK_LSHIFT) && Keyboard_IsPress(DIK_LEFTARROW))
	{
		g_rotSpd.y = -ROT_SPEED * 2;
	}
	//�E��]
	else if (Keyboard_IsPress(DIK_RIGHTARROW))
	{
		g_rotSpd.x = ROT_SPEED;
	}
	//����]
	else if (Keyboard_IsPress(DIK_LEFTARROW))
	{
		g_rotSpd.x = -ROT_SPEED;
	}
	//���]
	else if (Keyboard_IsPress(DIK_DOWNARROW))
	{
		g_rotSpd.z = ROT_SPEED;
	}
	//����]
	else if (Keyboard_IsPress(DIK_UPARROW))
	{
		g_rotSpd.z = -ROT_SPEED;
	}

	D3DXMatrixRotationAxis(&mtxRot, &g_vecRight, g_rotSpd.z);
	D3DXVec3TransformNormal(&g_vecFront, &g_vecFront, &mtxRot);
	D3DXVec3TransformNormal(&g_vecUp, &g_vecUp, &mtxRot);

	const int magic_num = 10;
	g_LookAt = g_Pos + g_vecFront * magic_num;
	g_Pos += g_LookAt;
	D3DXMatrixRotationY(&mtxRot, g_rotSpd.y);
	D3DXVec3TransformNormal(&g_vecFront, &g_vecFront, &mtxRot);
	D3DXVec3TransformNormal(&g_vecRight, &g_vecRight, &mtxRot);
	D3DXVec3TransformNormal(&g_vecUp, &g_vecUp, &mtxRot);
	g_Pos = g_LookAt - g_vecFront * magic_num;

	D3DXMatrixRotationY(&mtxRot, g_rotSpd.x);
	D3DXVec3TransformNormal(&g_vecFront, &g_vecFront, &mtxRot);
	D3DXVec3TransformNormal(&g_vecRight, &g_vecRight, &mtxRot);
	D3DXVec3TransformNormal(&g_vecUp, &g_vecUp, &mtxRot);

	g_rotSpd.x *= 0.85f;
	g_rotSpd.y *= 0.85f;
	g_rotSpd.z *= 0.85f;

	//�x�N�g���̐��K��
	D3DXVec3Normalize(&g_vecFront, &g_vecFront);
	D3DXVec3Normalize(&g_vecRight, &g_vecRight);
	D3DXVec3Normalize(&g_vecUp, &g_vecUp);

	D3DXVECTOR3 eye = g_Pos;
	D3DXVECTOR3 at = g_Pos + g_vecFront;
	D3DXVECTOR3 up = g_vecUp;

	//�Y�[��
	if (Keyboard_IsPress(DIK_Z))
	{
		g_Fov += 0.01f;
	}
	if (Keyboard_IsPress(DIK_X))
	{
		g_Fov -= 0.01f;
	}

	//�f�o�C�X�̃|�C���^�擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//�r���[�ϊ�
	D3DXMatrixLookAtLH(&g_mtxView, &eye, &at, &up);	//�s��v�Z

	pDevice->SetTransform(D3DTS_VIEW, &g_mtxView);

	//�v���W�F�N�V�����ϊ��s��
	D3DXMATRIX mtxProj;								//�v���W�F�N�V�����ϊ��s��p�ϐ�
	D3DXMatrixPerspectiveFovLH(&mtxProj, g_Fov, (float)SCREEN_WIDTH / SCREEN_HEIGHT, 0.1f, 100.0f);
	//�v���W�F�N�V�����̃A�h���X, ����p�̔����̊p�x(120�� = 60), �A�X�y�N�g��, �����near near > 0, �����far
	pDevice->SetTransform(D3DTS_PROJECTION, &mtxProj);
}

//=====================================================
//�J�������
//=====================================================
void dCamera_Debug_Info(void)
{
	//���W
	DebugFont_Draw(0, 32 * 0, "eye = %.02lf  %.02lf  %.02lf", g_Pos.x, g_Pos.y, g_Pos.z);
	//�����_
	DebugFont_Draw(0, 32 * 1, "at = %.02lf  %.02lf  %.02lf", g_Pos.x + g_vecFront.x, g_Pos.y + g_vecFront.y, g_Pos.z + g_vecFront.z);
	//�Y�[��
	DebugFont_Draw(0, 32 * 2, "zoom = %.02lf", g_Fov);
}

//=====================================================
//�r���{�[�h�擾
//=====================================================
D3DXMATRIX dCamera_Billbord(void)
{
	D3DXMATRIX mtxInv;

	D3DXMatrixIdentity(&mtxInv);
	D3DXMatrixTranspose(&mtxInv, &g_mtxView);	//�]�u�s��

	mtxInv._14 = 0.0f;
	mtxInv._24 = 0.0f;
	mtxInv._34 = 0.0f;

	return mtxInv;
}