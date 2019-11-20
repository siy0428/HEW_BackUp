#include "input.h"
#include "camera.h"
#include "debug_camera.h"

//=====================================================
//列挙型
//=====================================================
typedef enum
{
	NORMAl_CAMERA,
	DEBUG_CAMERA,
	LIVE_CAMERA
}CAMERA_TYPE;

//=====================================================
//グローバル変数
//=====================================================
static CAMERA_TYPE g_camera_type;

//=====================================================
//カメラ切り替え
//=====================================================
void Camera_Change(void)
{
	//キー操作でのカメラ切り替え
	//通常カメラ
	if (Keyboard_IsTrigger(DIK_1))
	{
		g_camera_type = NORMAl_CAMERA;
	}
	//デバッグカメラ
	else if (Keyboard_IsTrigger(DIK_2))
	{
		g_camera_type = DEBUG_CAMERA;
	}

	//カメラ切り替え
	switch (g_camera_type)
	{
	//通常カメラ
	case NORMAl_CAMERA:
		Camera_Set();
		break;
	//デバッグ用カメラ
	case DEBUG_CAMERA:
		dCamera_Set();
		break;
	//ライブ視点カメラ
	case LIVE_CAMERA:

		break;
	}
}