#pragma once
#include <windows.h>
#include <windowsx.h>
#include <d3d9.h>
#include <d3dx9.h>

#ifdef UNICODE
typedef LPCWSTR LPCOMSTRING;
#else
typedef LPCSTR LPCOMSTRING;
#endif

#pragma comment (lib, "d3d9.lib")
#pragma comment (lib, "d3dx9.lib")

class DXSprite2
{
	LPDIRECT3DDEVICE9 device;

	IDirect3DSurface9 *sprite, *backbuffer;
	D3DXIMAGE_INFO imgInfo;

	int x, y;			// pozicija
	int xmov, ymov;		// judejimo greiciai
	int currFrame,		// rodomas kadras
		totalFrames;	// viso kadru
	int fWidth,			// kadro plotis
		fHeight;		// kadro aukstis
	int sizeFactor;
public:
	DXSprite2();
	~DXSprite2();
	void setDevice(LPDIRECT3DDEVICE9 dv);
	bool loadSprite(LPCOMSTRING filename, D3DCOLOR transColor, int frameCount = 1);
	void show();
	void nextFrame();
	void setxy(int x, int y);
	void setSize(int size);
};