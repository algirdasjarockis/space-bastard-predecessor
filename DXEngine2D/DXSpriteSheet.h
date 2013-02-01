#ifndef _DXSPRITESHEET_H_
#define _DXSPRITESHEET_H_

#include <windows.h>
#include <d3d9.h>
#include <d3dx9.h>
#include <map>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <string>

using namespace std;

#pragma comment (lib, "d3d9.lib")
#pragma comment (lib, "d3dx9.lib")

#ifdef UNICODE
typedef LPCWSTR LPCOMSTRING;
#else
typedef LPCSTR LPCOMSTRING;
#endif

class DXSpriteSheet
{
protected:
	struct SpriteData
	{
		RECT src;
		int currFrame, totalFrames;
		D3DXVECTOR3 center, position;
		D3DCOLOR color;
	};

	LPDIRECT3DDEVICE9 pDevice;
	LPD3DXSPRITE spriteObj;

	LPDIRECT3DTEXTURE9 texture;
	D3DXIMAGE_INFO imageInfo;
	
	map<string, SpriteData> sprites;
	
public:

	DXSpriteSheet();
	~DXSpriteSheet();
	void release();
	bool readSpriteData(const char *dataFile);
	bool setInterfaces(const LPDIRECT3DDEVICE9 pDevice);
	LPD3DXSPRITE getSpriteInterface();

	void setCenter(const char * sprName, float x, float y);
	void setCenterAuto(const char *sprName);
	void setPosition(const char * sprName, float x, float y, float depth = 0);
	void setColor(const char * sprName, D3DCOLOR color);
	D3DXVECTOR3 getPosition(const char * sprName);
	D3DXVECTOR3 * getPosition(const char * sprName, int heh);
	D3DCOLOR getColor(const char *sprName);
	void getSize(const char *sprName, int *width, int *height);
	int getCurrentFrame(const char *sprName);
	int getFrameCount(const char *sprName);
	void move(const char * sprName, float x, float y = 0);
	void moveTo(const char * sprName, float x, float y = 0, float speed = 1);

	bool create(LPCOMSTRING srcFile, D3DCOLOR transColor);
	bool createCopy(const char * spriteName, const char * newSpriteName);
	void setCurrentFrame(const char *spriteName, int frame);

	// render funkcijos
	bool showNextFrame(const char * spriteName);
	bool showFrame(const char * spriteName, int frame);
	bool draw(const char * spriteName, const RECT * source, D3DCOLOR color = D3DCOLOR_XRGB(255, 255, 255));

	bool scale(float x, float y);
	bool rotate(float degree);

	bool spriteExists(const char *spriteName);
	void removeSprite(const char * spriteName);
};

#endif