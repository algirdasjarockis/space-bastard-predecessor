#ifndef _DXBUTTON_H_
#define _DXBUTTON_H_

#include "DXFont.h"
#include "DXSpriteSheet.h"

#include <assert.h>

class DXButton
{
	// captions color
	D3DCOLOR currentColor,			// dabartine spalva
			 normalColor,			// 
			 overColor;  

	LPCOMSTRING caption;

	DXFont *font;	

	char *spriteName;
	char *clickSound;
public:
	DXSpriteSheet *buttonSheet;
	
	enum {
		STATUS_IDLE,
		MOUSE_OVER
	};

	DXButton();

	void setSprite(DXSpriteSheet *sheet, const char *sprite);
	int getStatus(int mouseX, int mouseY);
	void setCaption(LPCOMSTRING caption);
	void setFont(DXFont *font);
	void setOverColor(D3DCOLOR overColor);
	
//	void setCenter(char * sprName, int x, int y);
	void setPosition(int x, int y, float depth = 0);
//	D3DXVECTOR3 getPosition(const char * sprName);
//	void getSize(const char *sprName, int *width, int *height);
	void setSound(const char *path);
	char * getSound() const;

	bool draw();
};

#endif