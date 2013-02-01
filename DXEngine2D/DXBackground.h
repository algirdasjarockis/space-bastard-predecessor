#ifndef _DXBACKGROUND_H_
#define _DXBACKGROUND_H_

#include <windows.h>
#include <d3d9.h>
#include <d3dx9.h>

#ifdef UNICODE
typedef LPCWSTR LPCOMSTRING;
#else
typedef LPCSTR LPCOMSTRING;
#endif

#pragma comment (lib, "d3d9.lib")
#pragma comment (lib, "d3dx9.lib")

class DXBackground
{
	LPDIRECT3DDEVICE9 device;

	IDirect3DSurface9 *image, *backbuffer;
	D3DXIMAGE_INFO imgInfo;

	int scrollY,
		scrollXov,
		scrollXcv;

	bool ovFinished,
		 cvFinished;

	bool _isCreated;

public:
	DXBackground();
	~DXBackground();
	void release();
	void setDevice(LPDIRECT3DDEVICE9 dv);
	bool load(LPCOMSTRING filename);
	bool display();
	void scroll(int speed = 5);
	
	void resetEffects();

	bool effectDoorCloseVertical(int speed);
	bool effectDoorOpenVertical(int speed);

	bool isCreated();
};

#endif