#ifndef _DXENGINE2D_H_
#define _DXENGINE2D_H_


#include <windows.h>
#include <d3d9.h>
#include <d3dx9.h>

#include "DXSpriteSheet.h"
#include "DXBackground.h"
#include "DXFont.h"
#include "DXButton.h"
#include <map>
#include <algorithm>

using namespace std;

#pragma comment (lib, "d3d9.lib")
#pragma comment (lib, "d3dx9.lib")

class DXEngine2D
{
	LPDIRECT3D9 d3d;
	LPDIRECT3DDEVICE9 pDevice;

	map<string, DXSpriteSheet> spriteSheets;
	map<string, DXFont> fontStyles;
	map<string, DXButton> buttons;

public:

	DXBackground *background;

	DXEngine2D();
	~DXEngine2D();

	bool init(HWND hWnd, int screenWidth, int screenHeight, bool fullscreen = true, bool restart = false);
	LPDIRECT3DDEVICE9 getDevice();

	bool clear(D3DCOLOR color);
	bool present();
	bool beginScene();
	bool endScene();

	// background
	bool createBackground(LPCOMSTRING srcFile);

	// font
	bool createFontStyle(const char* styleName, const DXFont::FontStyle *fontStyle);
	DXFont * getFontStyle(const char *styleName);
	bool fontExists(const char *styleName);

	// sprite 
	bool createSpriteSheet(const char *sheetName, LPCOMSTRING srcFile, const char *dataFile, D3DCOLOR transColor);
	DXSpriteSheet * getSpriteSheet(const char *sheetName);
	bool spriteSheetExists(const char *sheetName);
	void releaseSpriteSheet(const char *sheetName);

	// buttons
	bool createButton(const char* buttonName, const char *sheetName, const char *spriteName, LPCOMSTRING caption, const char *fontStyle);
	DXButton * getButton(const char *btnName);
	bool DXEngine2D::buttonExists(const char *btnName);
};

#endif