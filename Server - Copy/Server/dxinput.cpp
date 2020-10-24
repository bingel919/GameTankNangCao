#include "dxinput.h"

#define BUTTON_DOWN(obj, button) (obj.rgbButtons[button] & 0x80)

LPDIRECTINPUT8 dinput;
LPDIRECTINPUTDEVICE8 dimouse;
LPDIRECTINPUTDEVICE8 dikeyboard;
DIMOUSESTATE mouse_state;

char keys[256];

int Init_DirectInput(HWND hwnd)
{
	//direct input object
	HRESULT result = DirectInput8Create(
		GetModuleHandle(NULL),
		DIRECTINPUT_VERSION,
		IID_IDirectInput8,
		(void**)&dinput,
		NULL
	);

	if (result != DI_OK)
		return 0;

	//mouse
	result = dinput->CreateDevice(GUID_SysMouse, &dimouse, NULL);
	if (result != DI_OK)
		return 0;

	//keyboard
	result = dinput->CreateDevice(GUID_SysKeyboard, &dikeyboard, NULL);
	if (result != DI_OK)
		return 0;

	//clean return
	return 1;

}

int Init_Mouse(HWND hwnd)
{
	//set mouse input's data format
	HRESULT result = dimouse->SetDataFormat(&c_dfDIMouse);
	if (result != DI_OK)
		return 0;

	//set the cooperative level
	//also hide mouse pointer
	result = dimouse->SetCooperativeLevel(hwnd, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND);
	if (result != DI_OK)
		return 0;
	
	
	//acquire mouse
	
	result = dimouse->Acquire();
	if (result != DI_OK)
		return 0;

	//give the go-ahead
	return 1;

}

int Mouse_X()
{
	/*POINT tmp;
	GetCursorPos(&tmp);
	return tmp.x;*/
	return mouse_state.lX;
}

int Mouse_Y()
{
	/*POINT tmp;
	GetCursorPos(&tmp);
	return tmp.y;*/
	return mouse_state.lY;
}

int Mouse_Button(int button)
{
	return BUTTON_DOWN(mouse_state, button);
}

void Poll_Mouse()
{
	dimouse->GetDeviceState(sizeof(mouse_state), (LPVOID)&mouse_state);
}

void Kill_Mouse()
{
	if (dimouse != NULL)
	{
		dimouse->Unacquire();
		dimouse->Release();
		dimouse = NULL;
	}
}

int Init_Keyboard(HWND hwnd)
{
	//set mouse input's data format
	HRESULT result = dikeyboard->SetDataFormat(&c_dfDIKeyboard);
	if (result != DI_OK)
		return 0;

	//set the cooperrative level
	result = dikeyboard->SetCooperativeLevel(hwnd, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND);
	if (result != DI_OK)
		return 0;

	//backbuffer input
	DIPROPDWORD dipdw;

	dipdw.diph.dwSize = sizeof(DIPROPDWORD);
	dipdw.diph.dwHeaderSize = sizeof(DIPROPHEADER);
	dipdw.diph.dwObj = 0;
	dipdw.diph.dwHow = DIPH_DEVICE;
	dipdw.dwData = KEYBOARD_BUFFER_SIZE; // Arbitary buffer size

	//acquire the keyboard
	result = dikeyboard->Acquire();
	if (result != DI_OK)
		return 0;
	
	//give the go ahead
	return 1;
}

void Poll_Keyboard()
{
	dikeyboard->GetDeviceState(sizeof(keys), (LPVOID)&keys);
}

int Key_Down(int key)
{
	return (keys[key] & 0x80);
}

int Key_Up(int key)
{
	return (keys[key] & 0x10);
}

void Kill_Keyboard()
{
	if (dikeyboard != NULL)
	{
		dikeyboard->Unacquire();
		dikeyboard->Release();
		dikeyboard = NULL;
	}
}
