#include <d3dx9.h>
#include "mydirectx.h"
#include "common.h"
#include "input.h"
#include "debug_font.h"
#include "mouse.h"
#include "joycon.h"
#include "stone.h"
#include "goal.h"

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
static CAMERA g_camera;
static D3DXVECTOR3 g_look(0.0f, 0.0f, 0.0f);

//=====================================================
//������
//=====================================================
void Camera_Init(void)
{
	g_camera.pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_look = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
}

//=====================================================
//�J�����̐ݒ�
//=====================================================
void Camera_Set(void)
{
	//�f�o�C�X�̃|�C���^�擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxRotation[XYZ_MAX];
	D3DXVECTOR3 At = Stone_GetPos(Stone_PlayerTurn());

	g_camera.pos = D3DXVECTOR3(0.0f, 5.0f, -10.0f);
	//�����_��]
	D3DXMatrixRotationY(&mtxRotation[Y], Joycon_Operator() * D3DX_PI / 180);
	D3DXVec3TransformNormal(&g_camera.pos, &g_camera.pos, &mtxRotation[Y]);
	g_camera.pos += At;

	//�r���[�ϊ�
	D3DXMATRIX mtxView;		//�r���[�ϊ��s��p�ϐ�
	D3DXVECTOR3 eye(g_camera.pos.x, g_camera.pos.y, g_camera.pos.z);	//�J�����̍��W
	D3DXVECTOR3 at(At.x, At.y, At.z);				//����ꏊ(�����_)
	D3DXVECTOR3 up(0.0f, 1.0f, 0.0f);				//�J�����̏�����x�N�g��
	D3DXMatrixLookAtLH(&mtxView, &eye, &at, &up);	//�s��v�Z

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