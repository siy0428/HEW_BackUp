//------------------------------------------------------------------------------//
//																				//
//		名前：		JoyInput.cpp												//
//		制作日：		2019.11.03													//
//		制作人：		フォン														//
//		最新修正日：	2019.11.09													//
//		説明：		ジョイスティック用のdirectinputインターフェースを作って、			//
//					Get関数でコントローラーのデータをもらえる。						//
//																				//
//																				//
//------------------------------------------------------------------------------//





#include"JoyInput.h"
#include<d3d9.h>
#include<dinput.h>
#include <dinputd.h>
#include <stdio.h>
#include "debug_font.h"

#pragma region MicrosoftのDInputサンプルプログラム（できるだけ動かない）

struct DI_ENUM_CONTEXT
{
	DIJOYCONFIG* pPreferredJoyCfg;
	bool bPreferredJoyCfgValid;
};

LPDIRECTINPUT8          glbDI = nullptr;
LPDIRECTINPUTDEVICE8    glbJoystick = nullptr;
DIDEVCAPS				glbdiDevCaps;

DIJOYSTATE2 nowInput;
DIJOYSTATE2 lastInput;

static bool g_old_press[128] = { false };
static bool g_now_buttons[128] = { false };

BOOL CALLBACK EnumJoysticksCallback(const DIDEVICEINSTANCE* pdidInstance, VOID* pContext);
BOOL CALLBACK EnumObjectsCallback(const DIDEVICEOBJECTINSTANCE* pdidoi, VOID* pContext);

HRESULT InitDirectInput(HWND hWnd) {
	HRESULT hr;
	DIJOYCONFIG PreferredJoyCfg = { 0 };
	DI_ENUM_CONTEXT enumContext;

	enumContext.pPreferredJoyCfg = &PreferredJoyCfg;
	enumContext.bPreferredJoyCfgValid = false;

	if (FAILED(hr = DirectInput8Create(
		GetModuleHandle(nullptr),
		DIRECTINPUT_VERSION,
		IID_IDirectInput8,
		(VOID**)&glbDI,
		nullptr)))
	{
		return hr;
	}

	IDirectInputJoyConfig8* pJoyConfig = nullptr;
	if (FAILED(hr = glbDI->QueryInterface(IID_IDirectInputJoyConfig8, (void**)&pJoyConfig)))
		return hr;

	PreferredJoyCfg.dwSize = sizeof(PreferredJoyCfg);
	if (SUCCEEDED(pJoyConfig->GetConfig(0, &PreferredJoyCfg, DIJC_GUIDINSTANCE)))
	{
		enumContext.bPreferredJoyCfgValid = true;
	}
	SAFE_RELEASE(pJoyConfig);

	if (FAILED(hr = glbDI->EnumDevices(
		DI8DEVCLASS_GAMECTRL,
		EnumJoysticksCallback,
		&enumContext, DIEDFL_ATTACHEDONLY)))
	{
		return hr;
	}

	glbdiDevCaps.dwSize = sizeof(DIDEVCAPS);
	if (FAILED(hr = glbJoystick->GetCapabilities(&glbdiDevCaps)))
	{
		return hr;
	}

	if (!glbJoystick) {
		MessageBox(nullptr, TEXT("Joystick not found."),
			TEXT("Warning!"),
			MB_ICONERROR | MB_OK);
		return S_OK;
	}

	if (FAILED(hr = glbJoystick->SetDataFormat(&c_dfDIJoystick2)))
	{
		return hr;
	}
	if (FAILED(hr = glbJoystick->SetCooperativeLevel(hWnd, DISCL_EXCLUSIVE |
		DISCL_FOREGROUND)))
	{
		return hr;
	}
	if (FAILED(hr = glbJoystick->EnumObjects(EnumObjectsCallback,
		(VOID*)hWnd, DIDFT_ALL)))
	{
		return hr;
	}


	return S_OK;

}

void UninitDirectInput()
{
	if (glbJoystick)
		glbJoystick->Unacquire();

	SAFE_RELEASE(glbJoystick);
	SAFE_RELEASE(glbDI);
}

BOOL CALLBACK EnumJoysticksCallback(const DIDEVICEINSTANCE* pdidInstance,
	VOID* pContext)
{
	auto pEnumContext = reinterpret_cast<DI_ENUM_CONTEXT*>(pContext);
	HRESULT hr;

	if (pEnumContext->bPreferredJoyCfgValid &&
		!IsEqualGUID(pdidInstance->guidInstance, pEnumContext->pPreferredJoyCfg->guidInstance))
	{
		return DIENUM_CONTINUE;
	}
	hr = glbDI->CreateDevice(pdidInstance->guidInstance, &glbJoystick, nullptr);

	if (FAILED(hr))
	{
		return DIENUM_CONTINUE;
	}
	return DIENUM_STOP;
}
BOOL CALLBACK EnumObjectsCallback(const DIDEVICEOBJECTINSTANCE* pdidoi,
	VOID* pContext)
{
	HWND hDlg = (HWND)pContext;

	static int nSliderCount = 0;
	static int nPOVCount = 0;

	if (pdidoi->dwType & DIDFT_AXIS)
	{
		DIPROPRANGE diprg;
		diprg.diph.dwSize = sizeof(DIPROPRANGE);
		diprg.diph.dwHeaderSize = sizeof(DIPROPHEADER);
		diprg.diph.dwHow = DIPH_BYID;
		diprg.diph.dwObj = pdidoi->dwType;
		diprg.lMin = -1000;
		diprg.lMax = +1000;

		if (FAILED(glbJoystick->SetProperty(DIPROP_RANGE, &diprg.diph)))
		{
			return DIENUM_STOP;
		}
	}

	if (pdidoi->guidType == GUID_XAxis)
	{
		EnableWindow(GetDlgItem(hDlg, IDC_X_AXIS), TRUE);
		EnableWindow(GetDlgItem(hDlg, IDC_X_AXIS_TEXT), TRUE);
	}
	if (pdidoi->guidType == GUID_YAxis)
	{
		EnableWindow(GetDlgItem(hDlg, IDC_Y_AXIS), TRUE);
		EnableWindow(GetDlgItem(hDlg, IDC_Y_AXIS_TEXT), TRUE);
	}
	if (pdidoi->guidType == GUID_ZAxis)
	{
		EnableWindow(GetDlgItem(hDlg, IDC_Z_AXIS), TRUE);
		EnableWindow(GetDlgItem(hDlg, IDC_Z_AXIS_TEXT), TRUE);
	}
	if (pdidoi->guidType == GUID_RxAxis)
	{
		EnableWindow(GetDlgItem(hDlg, IDC_X_ROT), TRUE);
		EnableWindow(GetDlgItem(hDlg, IDC_X_ROT_TEXT), TRUE);
	}
	if (pdidoi->guidType == GUID_RyAxis)
	{
		EnableWindow(GetDlgItem(hDlg, IDC_Y_ROT), TRUE);
		EnableWindow(GetDlgItem(hDlg, IDC_Y_ROT_TEXT), TRUE);
	}
	if (pdidoi->guidType == GUID_RzAxis)
	{
		EnableWindow(GetDlgItem(hDlg, IDC_Z_ROT), TRUE);
		EnableWindow(GetDlgItem(hDlg, IDC_Z_ROT_TEXT), TRUE);
	}
	if (pdidoi->guidType == GUID_Slider)
	{
		switch (nSliderCount++)
		{
		case 0:
			EnableWindow(GetDlgItem(hDlg, IDC_SLIDER0), TRUE);
			EnableWindow(GetDlgItem(hDlg, IDC_SLIDER0_TEXT), TRUE);
			break;

		case 1:
			EnableWindow(GetDlgItem(hDlg, IDC_SLIDER1), TRUE);
			EnableWindow(GetDlgItem(hDlg, IDC_SLIDER1_TEXT), TRUE);
			break;
		}
	}
	if (pdidoi->guidType == GUID_POV)
	{
		switch (nPOVCount++)
		{
		case 0:
			EnableWindow(GetDlgItem(hDlg, IDC_POV0), TRUE);
			EnableWindow(GetDlgItem(hDlg, IDC_POV0_TEXT), TRUE);
			break;

		case 1:
			EnableWindow(GetDlgItem(hDlg, IDC_POV1), TRUE);
			EnableWindow(GetDlgItem(hDlg, IDC_POV1_TEXT), TRUE);
			break;

		case 2:
			EnableWindow(GetDlgItem(hDlg, IDC_POV2), TRUE);
			EnableWindow(GetDlgItem(hDlg, IDC_POV2_TEXT), TRUE);
			break;

		case 3:
			EnableWindow(GetDlgItem(hDlg, IDC_POV3), TRUE);
			EnableWindow(GetDlgItem(hDlg, IDC_POV3_TEXT), TRUE);
			break;
		}
	}

	return DIENUM_CONTINUE;
}

HRESULT UpdateInput()
{
	HRESULT     hr;
	DIJOYSTATE2 js;

	lastInput = nowInput;

	if (NULL == glbJoystick) {
		return S_OK;
	}

	hr = glbJoystick->Poll();
	if (FAILED(hr))
	{
		hr = glbJoystick->Acquire();
		while (hr == DIERR_INPUTLOST)
			hr = glbJoystick->Acquire();

	}
	hr = glbJoystick->GetDeviceState(sizeof(DIJOYSTATE2), &js);
	if (FAILED(hr)) {
		return hr;
	}

	nowInput = js;

	return S_OK;

}

#pragma endregion


bool GetButton(int buttonIndex) {
	return nowInput.rgbButtons[buttonIndex];
}

bool GetButton_isTrigger(int buttonIndex)
{
	//1F前がfalseで現在がtrueであればTrigger
	g_now_buttons[buttonIndex] = nowInput.rgbButtons[buttonIndex] && !g_old_press[buttonIndex];
	//1F前の入力状態記憶
	g_old_press[buttonIndex] = nowInput.rgbButtons[buttonIndex];

	return g_now_buttons[buttonIndex];
}

D3DXVECTOR3 GetGyro() {
	return { (float)nowInput.rglSlider[0] ,(float)nowInput.rglSlider[1] ,(float)nowInput.lRz };
}

D3DXVECTOR2 GetStick() {
	return { (float)nowInput.lX, (float)nowInput.lY };
}

void Draw(void)
{

}