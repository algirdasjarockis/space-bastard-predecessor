#include "stdafx.h"
#include "DXSprite2.h"

DXSprite2::DXSprite2()
{
	ZeroMemory(&imgInfo, sizeof(imgInfo));
	device = NULL;
	x = y = 0;
	xmov = ymov = 3;
	totalFrames = 1;
	currFrame = 0;
	sizeFactor = 1;
}

DXSprite2::~DXSprite2()
{
	//sprite->Release();
}

void DXSprite2::setDevice(LPDIRECT3DDEVICE9 dv)
{
	device = dv;
}

bool DXSprite2::loadSprite(LPCOMSTRING filename, D3DCOLOR transColor, int frameCount)
{
	HRESULT hResult = D3DXGetImageInfoFromFile(filename, &imgInfo);
	if (FAILED(hResult))
		return false;
//assert(false);
	if(frameCount > 0)
		fWidth = imgInfo.Width / frameCount;
	else fWidth = imgInfo.Width;
	
	totalFrames = frameCount;
	fHeight = imgInfo.Height;

	hResult = device->CreateOffscreenPlainSurface(imgInfo.Width, imgInfo.Height, 
											D3DFMT_X8R8G8B8, D3DPOOL_DEFAULT, &sprite, NULL);
	if (FAILED(hResult))
		return false;

	hResult = D3DXLoadSurfaceFromFile(sprite, NULL, NULL, filename, NULL, D3DX_DEFAULT, transColor, NULL);
	if (FAILED(hResult))
		return false;

	device->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &backbuffer );

	return true;
}

void DXSprite2::nextFrame()
{
	currFrame++;
	show();
}

void DXSprite2::show()
{
	RECT src, dest;

	if(currFrame >= totalFrames)
		currFrame = 0;

	src.left = currFrame * fWidth;
	src.top = 0;
	src.right = src.left + fWidth;
	src.bottom = src.top + fHeight;

	dest.left = x;
	dest.top = y;
	dest.right = dest.left + fWidth * sizeFactor;
	dest.bottom = dest.top + fHeight * sizeFactor;

	device->StretchRect(sprite, &src, backbuffer, &dest, D3DTEXF_NONE );
}

void DXSprite2::setxy(int x, int y)
{
	this->x = x;
	this->y = y;
}

void DXSprite2::setSize(int size)
{
	sizeFactor = size;
}