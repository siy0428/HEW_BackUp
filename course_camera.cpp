#include "mydirectx.h"
#include <d3dx9.h>
#include "common.h"
#include "fade.h"

//=====================================================
//�O���[�o���ϐ�
//=====================================================
static D3DXVECTOR3 g_pos;

//=====================================================
//������
//=====================================================
void Course_Camera_Init(void)
{
	g_pos = D3DXVECTOR3(0.0f, 10.0f, -10.0f);
}

//=====================================================
//�I��
//=====================================================
void Course_Camera_Uninit(void)
{

}

//=====================================================
//�X�V
//=====================================================
void Course_Camera_Update(void)
{
	//�f�o�C�X�̃|�C���^�擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxRotation;

	//�r���[�ϊ�
	D3DXMATRIX mtxView;		//�r���[�ϊ��s��p�ϐ�
	D3DXVECTOR3 eye(g_pos.x, g_pos.y, g_pos.z);		//�J�����̍��W
	D3DXVECTOR3 at(g_pos.x, g_pos.y - 3.0f, g_pos.z + 5.0f);//����ꏊ(�����_)
	D3DXVECTOR3 up(0.0f, 1.0f, 0.0f);				//�J�����̏�����x�N�g��
	D3DXMatrixLookAtLH(&mtxView, &eye, &at, &up);	//�s��v�Z

	//���W�ϊ�
	g_pos.z += 0.1f;
	//���̋��������񂾂玟�̃V�[��
	if (g_pos.z >= 30.0f)
	{
		Fade_InOut(SCENE_MAIN);
	}

	pDevice->SetTransform(D3DTS_VIEW, &mtxView);

	//�v���W�F�N�V�����ϊ��s��
	D3DXMATRIX mtxProj;								//�v���W�F�N�V�����ϊ��s��p�ϐ�
	D3DXMatrixPerspectiveFovLH(&mtxProj, D3DXToRadian(60), (float)SCREEN_WIDTH / SCREEN_HEIGHT, 0.1f, 100.0f);
	//�v���W�F�N�V�����̃A�h���X, ����p�̔����̊p�x(120�� = 60), �A�X�y�N�g��, �����near near > 0, �����far
	pDevice->SetTransform(D3DTS_PROJECTION, &mtxProj);
}

//=====================================================
//�`��
//=====================================================
void Course_Camera_Draw(void)
{

}