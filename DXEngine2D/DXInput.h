#ifndef _DXINPUT_H_
#define _DXINPUT_H_

#include <windows.h>
#include <dinput.h>

#pragma comment (lib, "dinput.lib")
#pragma comment (lib, "dinput8.lib")
#pragma comment (lib, "dxguid.lib")

#define PRESSED_LEFT	1
#define PRESSED_RIGHT	2
#define PRESSED_NONE	0

#define MOVED_X 5
#define MOVED_Y 6


class DXInput
{
	LPDIRECTINPUT8 din;
	LPDIRECTINPUTDEVICE8 devMouse;
	LPDIRECTINPUTDEVICE8 devKeyboard;

	HANDLE mouseEventHandle;
public:
	DXInput(HINSTANCE hInst);
	~DXInput();
	bool initMouse(HWND hWnd);
	bool initKeyboard(HWND hWnd);
	void acquireMouse();
	void unacquireMouse();
	int getMouseInput(long &x, long &y, long &z);
	bool keyPressed(long dikCode);
};

#endif