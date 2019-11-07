#include <d3dx9.h>
#include "mydirectx.h"
#include "common.h"
#include "input.h"
#include "debug_font.h"
#include "mouse.h"
#include "joycon.h"
#include "stone.h"

//=====================================================
//�񋓌^
//=====================================================
typedef enum
{
	X,
	Y,
	Z,
	XYZ_MAX
}Rotate_XYZ;

//=====================================================
//�\����
//=====================================================
typedef struct
{
	D3DXVECTOR3 pos;
}CAMERA;

//=====================================================
//�O���[�o���ϐ�
//=====================================================
static CAMERA g_Camera;

//=====================================================
//������
//=====================================================
void Camera_Init(void)
{

}

//=====================================================
//�J�����̐ݒ�
//=====================================================
void Camera_Set(D3DXVECTOR3 rotate, D3DXVECTOR3 pos, D3DXVECTOR3 At)
{
	//�f�o�C�X�̃|�C���^�擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//�r���[�ϊ�
	D3DXMATRIX mtxView, mtxRotation[XYZ_MAX];		//�r���[�ϊ��s��p�ϐ�
	D3DXVECTOR3 eye(pos.x, pos.y, pos.z);			//�J�����̍��W
	D3DXVECTOR3 at(At.x, At.y, At.z);				//����ꏊ(�����_)
	D3DXVECTOR3 up(0.0f, 1.0f, 0.0f);				//�J�����̏�����x�N�g��
	D3DXMatrixLookAtLH(&mtxView, &eye, &at, &up);	//�s��v�Z
	D3DXMatrixRotationX(&mtxRotation[X], rotate.x * D3DX_PI / 180);				//X����]
	D3DXMatrixRotationY(&mtxRotation[Y], rotate.y * D3DX_PI / 180);				//Y����]
	//D3DXMatrixRotationY(&mtxRotation[Y], -Joycon_Operator() * D3DX_PI / 180);	//y����]
	D3DXMatrixRotationZ(&mtxRotation[Z], rotate.z * D3DX_PI / 180);				//Z����]

	mtxView = mtxRotation[X] * mtxRotation[Y] * mtxRotation[Z] * mtxView;

	pDevice->SetTransform(D3DTS_VIEW, &mtxView);

	//�v���W�F�N�V�����ϊ��s��
	D3DXMATRIX mtxProj;								//�v���W�F�N�V�����ϊ��s��p�ϐ�
	D3DXMatrixPerspectiveFovLH(&mtxProj, D3DXToRadian(60), (float)SCREEN_WIDTH / SCREEN_HEIGHT, 0.1f, 100.0f);
	//�v���W�F�N�V�����̃A�h���X, ����p�̔����̊p�x(120�� = 60), �A�X�y�N�g��, �����near near > 0, �����far
	pDevice->SetTransform(D3DTS_PROJECTION, &mtxProj);
}


//=====================================================
//�J�������W����
//=====================================================
D3DXVECTOR3 Camera_Input_Pos(D3DXVECTOR3 pos)
{
	//�E�ړ�(���S�ړ�)
	if (Keyboard_IsPress(DIK_D))
	{
		pos.x += 0.25f;
	}
	//���ړ�(���S�ړ�)
	else if (Keyboard_IsPress(DIK_A))
	{
		pos.x -= 0.25f;
	}

	return pos;
}

//=====================================================
//�J������]����
//=====================================================
D3DXVECTOR3 Camera_Input_Rot(D3DXVECTOR3 rot)
{
	//�E��]
	if (Keyboard_IsPress(DIK_E))
	{
		rot.y += 1.0f;
	}
	//����]
	else if (Keyboard_IsPress(DIK_Q))
	{
		rot.y -= 1.0f;
	}

	return rot;
}

//=====================================================
//�J���������_����
//=====================================================
D3DXVECTOR3 Camera_Input_At(D3DXVECTOR3 at)
{
	//�E�ړ�(���S�ړ�)
	if (Keyboard_IsPress(DIK_D))
	{
		at.x += 0.25f;
	}
	//���ړ�(���S�ړ�)
	else if (Keyboard_IsPress(DIK_A))
	{
		at.x -= 0.25f;
	}

	return at;
}

//=====================================================
//�J�������
//=====================================================
void Camera_Debug_Info(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 at)
{
	//���W
	DebugFont_Draw(0, 32 * 3, "eye = %.02lf  %.02lf  %.02lf", pos.x, pos.y, pos.z);
	//�����_
	DebugFont_Draw(0, 32 * 4, "at = %.02lf  %.02lf  %.02lf", at.x, at.y, at.z);
	//��]
	DebugFont_Draw(0, 32 * 5, "rot = %.02lf  %.02lf  %.02lf", rot.x, rot.y, rot.z);
}