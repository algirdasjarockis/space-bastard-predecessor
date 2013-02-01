#ifndef _DXFONT_H_
#define _DXFONT_H_

#pragma once
#include <windows.h>
#include <windowsx.h>
#include <strsafe.h>
#include <d3d9.h>
#include <d3dx9.h>

#pragma warning (disable:4995)

#include <map>
#include <string>

using namespace std;

#ifdef UNICODE
typedef LPCWSTR LPCOMSTRING;
#else
typedef LPCSTR LPCOMSTRING;
#endif

class DXFont 
{
	LPDIRECT3DDEVICE9 pDevice;

	struct Label
	{
		LPD3DXSPRITE sprite;
		LPCOMSTRING labelText;
		RECT rect;
		DWORD format;
		DWORD color;
	};
	map<string, Label> labels;

public:

	struct FontStyle
	{
		int height, width;
		UINT weight;
		DWORD charset, pitchAndFamily;
		BOOL italic;
		LPCOMSTRING fontName;
		DWORD color;
	} fs;

	LPD3DXFONT font;
	DXFont();

	void setDevice(LPDIRECT3DDEVICE9 device);
	bool create(const FontStyle *fontStyle);
	bool drawText(LPD3DXSPRITE sprite, LPCOMSTRING text, RECT rect, DWORD format, DWORD color = 0);

	//void setLabel(const char *labelName, LPCOMSTRING labelText);
	void setLabel(LPD3DXSPRITE sprite, LPCOMSTRING labelText, const char * labelName, RECT rect, DWORD format, DWORD color = 0);
	void setLabelText(const char *labelName, LPCOMSTRING labelText);
	void removeLabel(const char *labelName);
	bool drawLabel(const char * labelName);
};

#endif