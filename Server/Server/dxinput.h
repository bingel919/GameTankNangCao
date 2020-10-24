#pragma once
#include <dinput.h>

#define KEYBOARD_BUFFER_SIZE 1024

#define KEY_DOWN(vk_code) ((GetAsyncKeyState(vk_code) & 0x8000)? 1:0)
#define	  KEY_UP(vk_code) ((GetAsyncKeyState(vk_code) & 0x8000)? 1:0)

int Init_DirectInput(HWND);
int Init_Keyboard(HWND);
void Poll_Keyboard();
int Key_Down(int);
int Key_Up(int);
void Kill_Keyboard();
void Poll_Mouse();
int Init_Mouse(HWND);
int Mouse_Button(int);
int Mouse_X();
int Mouse_Y();
void Kill_Mouse();

extern LPDIRECTINPUT8 dinput;
extern LPDIRECTINPUTDEVICE8 dimouse;
extern LPDIRECTINPUTDEVICE8 dikeyboard;
extern DIMOUSESTATE mouse_state;


