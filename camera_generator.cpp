#include "input.h"
#include "camera.h"
#include "debug_camera.h"

//=====================================================
//�񋓌^
//=====================================================
typedef enum
{
	NORMAL_CAMERA,
	DEBUG_CAMERA,
	LIVE_CAMERA
}CAMERA_TYPE;

//=====================================================
//�O���[�o���ϐ�
//=====================================================
static CAMERA_TYPE g_camera_type;

//=====================================================
//�J�����؂�ւ�
//=====================================================
void Camera_Change(void)
{
	//�L�[����ł̃J�����؂�ւ�
	//�ʏ�J����
	if (Keyboard_IsTrigger(DIK_1))
	{
		g_camera_type = NORMAL_CAMERA;
	}
	//�f�o�b�O�J����
	else if (Keyboard_IsTrigger(DIK_2))
	{
		g_camera_type = DEBUG_CAMERA;
	}

	//�J�����؂�ւ�
	switch (g_camera_type)
	{
	//�ʏ�J����
	case NORMAL_CAMERA:
		Camera_Set();
		break;
	//�f�o�b�O�p�J����
	case DEBUG_CAMERA:
		dCamera_Set();
		break;
	//���C�u���_�J����
	case LIVE_CAMERA:

		break;
	}
}