#include "DXFont.h"

/***
 *
 */
DXFont::DXFont()
{
	pDevice = NULL;
	font = NULL;
}

/***
 *
 */
void DXFont::setDevice(LPDIRECT3DDEVICE9 device)
{
	pDevice = device;
}

/***
 *
 */
bool DXFont::create(const FontStyle *fontStyle)
{
	if(!pDevice)
		return false;

	fs = *fontStyle;

	return(SUCCEEDED(D3DXCreateFont(pDevice, fontStyle->height, fontStyle->width, 
			fontStyle->weight, 1, fontStyle->italic, fontStyle->charset, OUT_DEFAULT_PRECIS, 
			DEFAULT_QUALITY, fontStyle->pitchAndFamily, fontStyle->fontName, &font) )); 
}

/***
 *
 */
bool DXFont::drawText(LPD3DXSPRITE sprite, LPCOMSTRING text, RECT rect, DWORD format, DWORD color)
{
	if(color == 0)
		color = fs.color;

	if(font->DrawText(sprite, text, lstrlen(text), &rect, format, color) != 0)
		return true;

	return false;
}

/***
 *
 */
void DXFont::setLabel(LPD3DXSPRITE sprite, LPCOMSTRING labelText, const char * labelName, RECT rect, DWORD format, DWORD color)
{
	DXFont::Label lb; 
	ZeroMemory(&lb, sizeof(lb));

	if(color == 0)
		color = fs.color;

	lb.sprite = sprite;
	lb.labelText = labelText;
	lb.rect = rect;
	lb.format = format;
	lb.color = color;

	labels[labelName] = lb;
}

/***
 *
 */
void DXFont::setLabelText(const char *labelName, LPCOMSTRING labelText)
{
	labels[labelName].labelText = labelText;
}

/***
 *
 */
void DXFont::removeLabel(const char *labelName)
{
	labels.erase(labelName);
}

/***
 *
 */
bool DXFont::drawLabel(const char * labelName)
{
	return drawText(labels[labelName].sprite, labels[labelName].labelText, labels[labelName].rect, 
			labels[labelName].format, labels[labelName].color);
}