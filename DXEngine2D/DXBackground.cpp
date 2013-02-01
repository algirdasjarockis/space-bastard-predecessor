#include "DXBackground.h"

/*** 
 *
 */
DXBackground::DXBackground()
{
	ZeroMemory(&imgInfo, sizeof(imgInfo));
	device = NULL;
	image = backbuffer = NULL;
	scrollY = 0;

	_isCreated = false;
	
	resetEffects();
}

/*** 
 *
 */
DXBackground::~DXBackground()
{
	release();
}

/***
 *
 */
void DXBackground::release()
{
	if(image)
		image->Release();
	_isCreated = false;
}

/*** 
 *
 */
void DXBackground::setDevice(LPDIRECT3DDEVICE9 dv)
{
	device = dv;
	device->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &backbuffer );
}

/*** 
 *
 */
bool DXBackground::load(LPCOMSTRING filename)
{
	HRESULT hResult = D3DXGetImageInfoFromFile(filename, &imgInfo);
	if (FAILED(hResult))
		return false;
	
	release();

	hResult = device->CreateOffscreenPlainSurface(imgInfo.Width, imgInfo.Height, 
											D3DFMT_X8R8G8B8, D3DPOOL_DEFAULT, &image, NULL);
	
	if (FAILED(hResult))
		return false;

	hResult = D3DXLoadSurfaceFromFile(image, NULL, NULL, filename, NULL, D3DX_DEFAULT, 0, NULL);
	if (FAILED(hResult))
		return false;

	_isCreated = true;

	return true;
}

/***
 *
 */
bool DXBackground::display()
{
	RECT position = {0, 0, imgInfo.Width, imgInfo.Height};

	if(FAILED(device->StretchRect(image, NULL, backbuffer, &position, D3DTEXF_NONE )))
		return false;

	return true;
}

/***
 *
 */
void DXBackground::scroll(int speed)
{
	RECT src, dest;
	
	scrollY += speed;
	if(scrollY >= imgInfo.Height)
		scrollY = 0;

	src.left = dest.left = 0;
	src.top = 0;
	dest.top = scrollY;
	src.right = dest.right = imgInfo.Width;
	src.bottom = imgInfo.Height - scrollY;
	dest.bottom = imgInfo.Height;

	device->StretchRect(image, &src, backbuffer, &dest, D3DTEXF_NONE );

	if(scrollY > 0) {
		src.top = imgInfo.Height - scrollY;
		src.bottom = imgInfo.Height;
		dest.top = 0;
		dest.bottom = scrollY;

		device->StretchRect(image, &src, backbuffer, &dest, D3DTEXF_NONE );
	}
}

/***
 *
 */
void DXBackground::resetEffects()
{
	scrollXov = scrollXcv = 0;
	cvFinished = false;
	ovFinished = false;
}

/***
 *
 */
bool DXBackground::effectDoorCloseVertical(int speed)
{
	if(cvFinished)
		return true;

	RECT src, dest;

	scrollXcv += speed;
	if(scrollXcv > imgInfo.Width / 2) {
		scrollXov = 0;
		cvFinished = true;
		return true;
	}	

	src.left = imgInfo.Width / 2 - scrollXcv;
	src.top = 0;
	src.right = imgInfo.Width / 2;
	src.bottom = imgInfo.Height;

	dest.left = 0;
	dest.top = 0;
	dest.right = src.right - src.left;
	dest.bottom = imgInfo.Height;

	// kaires durys
	device->StretchRect(image, &src, backbuffer, &dest, D3DTEXF_NONE );

	src.left = imgInfo.Width / 2;
	src.top = 0;
	src.right = imgInfo.Width / 2 + scrollXcv;
	src.bottom = imgInfo.Height;

	dest.left = imgInfo.Width - (src.right - src.left);
	dest.top = 0;
	dest.right = imgInfo.Width;
	dest.bottom = imgInfo.Height;

	// desines durys
	device->StretchRect(image, &src, backbuffer, &dest, D3DTEXF_NONE );

	return false;
}

/***
 *
 */
bool DXBackground::effectDoorOpenVertical(int speed)
{
	if(ovFinished)
		return true;

	RECT src, dest;

	scrollXov += speed;
	if(scrollXov > imgInfo.Width / 2) {
		scrollXcv = 0;
		ovFinished = true;
		return true;
	}	

	src.left = scrollXov;
	src.top = 0;
	src.right = imgInfo.Width / 2;
	src.bottom = imgInfo.Height;

	dest.left = 0;
	dest.top = 0;
	dest.right = src.right - scrollXov;
	dest.bottom = imgInfo.Height;

	// kaire siena
	device->StretchRect(image, &src, backbuffer, &dest, D3DTEXF_NONE );

	src.left = imgInfo.Width / 2;
	src.top = 0;
	src.right = imgInfo.Width - scrollXov;
	src.bottom = imgInfo.Height;

	dest.left = src.left + scrollXov;
	dest.top = 0;
	dest.right = imgInfo.Width;
	dest.bottom = imgInfo.Height;

	// desine siena
	device->StretchRect(image, &src, backbuffer, &dest, D3DTEXF_NONE );
	
	return false;
}

/***
 *
 */
bool DXBackground::isCreated()
{
	return _isCreated;
}