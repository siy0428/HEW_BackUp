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
	bool live;
}CAMERA;

//=====================================================
//�O���[�o���ϐ�
//=====================================================
static CAMERA g_camera;

//=====================================================
//������
//=====================================================
void Camera_Init(void)
{
	g_camera.pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_camera.live = true;
}

//=====================================================
//�J�����̐ݒ�
//=====================================================
void Camera_Set(D3DXVECTOR3 rotate, D3DXVECTOR3 pos, D3DXVECTOR3 At)
{
	//�f�o�C�X�̃|�C���^�擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//�J�����؂�ւ�
	if (Keyboard_IsTrigger(DIK_C))
	{
		g_camera.live = (g_camera.live) ? false : true;
	}

	g_camera.pos = (g_camera.live) ? D3DXVECTOR3(0.0f, 10.0f, 0.0f) : pos;

	//�r���[�ϊ�
	D3DXMATRIX mtxView, mtxRotation[XYZ_MAX];		//�r���[�ϊ��s��p�ϐ�
	D3DXVECTOR3 eye(g_camera.pos.x, g_camera.pos.y, g_camera.pos.z);			//�J�����̍��W
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