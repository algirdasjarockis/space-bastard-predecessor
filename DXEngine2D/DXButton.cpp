#include "DXButton.h"

/***
 *
 */
DXButton::DXButton()
{
	buttonSheet = NULL;
	currentColor = normalColor = overColor = D3DCOLOR_XRGB(0, 0, 0);
	clickSound = NULL;
}

/***
 *
 */
void DXButton::setSprite(DXSpriteSheet *sheet, const char *sprite)
{
	buttonSheet = sheet;
	//spriteName = (char*)sprite;
	spriteName = new char [strlen(sprite)+1];
	strcpy(spriteName, sprite);
}

/***
 *
 */
void DXButton::setFont(DXFont *font)
{
	this->font = font;
	normalColor = font->fs.color;
}

/***
 *
 */
int DXButton::getStatus(int mouseX, int mouseY)
{
	D3DXVECTOR3 pos = buttonSheet->getPosition(spriteName);
	int width, height;
	buttonSheet->getSize(spriteName, &width, &height);

	//int frameWidth = width / buttonSheet->getFrameCount(spriteName);
	//int frameHeight = height;

	if( (mouseX > pos.x) && (mouseX < (pos.x + width)) &&
	   (mouseY > pos.y) && (mouseY < (pos.y + height)) )
	{
		// peles zymeklis virs mygtuko
		buttonSheet->setCurrentFrame(spriteName, 1);
		currentColor = overColor;
		return MOUSE_OVER;
	}
	else {
		buttonSheet->setCurrentFrame(spriteName, 0);
		currentColor = normalColor;
	}

	return STATUS_IDLE;
}

/***
 *
 */
void DXButton::setCaption(LPCOMSTRING caption)
{
	this->caption = caption;
}

/***
 *
 */
void DXButton::setOverColor(D3DCOLOR overColor)
{
	this->overColor = overColor;
}

/***
 *
 */
void DXButton::setPosition(int x, int y, float depth)
{
	buttonSheet->setPosition(spriteName, x, y, depth);
}

/***
 *
 */
void DXButton::setSound(const char *path)
{
	if(clickSound)
		delete clickSound;

	clickSound = new char[strlen(path)];
	strcpy(clickSound, path);
}

/***
 *
 */
char * DXButton::getSound() const
{
	return clickSound;
}

/***
 *
 */
bool DXButton::draw()
{
	if(!buttonSheet->showFrame(spriteName, -1))
		return false;

	RECT rect;
	D3DXVECTOR3 position = buttonSheet->getPosition(spriteName);

	int width, height;
	buttonSheet->getSize(spriteName, &width, &height);

	rect.left = position.x;
	rect.top = position.y;
	rect.bottom = rect.top + height;
	rect.right = rect.left + width;

	return font->drawText(buttonSheet->getSpriteInterface(), caption, rect, DT_VCENTER | DT_CENTER, currentColor );
}