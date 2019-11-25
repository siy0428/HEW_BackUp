#include <d3dx9.h>
#include "mydirectx.h"

//=====================================================
//���C�g�̐ݒ�
//=====================================================
void Light_Set(void)
{
	//�f�o�C�X�̃|�C���^�擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	D3DLIGHT9 light = {};	//���C�g�\����
	light.Type = D3DLIGHT_DIRECTIONAL;
	D3DXVECTOR3 vecDirLight(-1.0f, -1.0f, 0.0f);		//���C�g�x�N�g��(�P�ʃx�N�g���ɂ��邱��)
	D3DXVec3Normalize(&vecDirLight, &vecDirLight);	//���K�� (�o��, ����);	
	light.Direction = vecDirLight;					//�x�N�g���̑��
	//���C�g�J���[�̐ݒ�
	light.Diffuse.r = 1.0f;
	light.Diffuse.g = 1.0f;
	light.Diffuse.b = 1.0f;
	light.Diffuse.a = 1.0f;

	pDevice->SetLight(0, &light);	//(���C�g�X���b�g, ���C�g�̐ݒ�)
	pDevice->LightEnable(0, TRUE);	//���C�g��ON/OFF
}
