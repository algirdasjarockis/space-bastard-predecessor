#include "DXEngine2D.h"

/***
 *	Konstruktorius
 */
DXEngine2D::DXEngine2D()
{
	if( NULL == (d3d = Direct3DCreate9(D3D_SDK_VERSION)))
		return;

	pDevice = NULL;
}

/***
 *	Destruktorius
 */
DXEngine2D::~DXEngine2D()
{
	if(pDevice != NULL)
		pDevice->Release();

	d3d->Release();
}

/***
 *
 */
LPDIRECT3DDEVICE9 DXEngine2D::getDevice()
{
	return pDevice;
}


/***
 *	
 */
bool DXEngine2D::init(HWND hWnd, int screenWidth, int screenHeight, bool fullscreen, bool restart)
{
	if(!d3d)
		return false;

	HRESULT hr;
	D3DPRESENT_PARAMETERS d3dpParam;
	ZeroMemory(&d3dpParam, sizeof(d3dpParam));

	d3dpParam.Windowed = !fullscreen;
	d3dpParam.hDeviceWindow = hWnd;
	d3dpParam.SwapEffect = D3DSWAPEFFECT_DISCARD;
    d3dpParam.BackBufferFormat = D3DFMT_X8R8G8B8;
    d3dpParam.BackBufferWidth = screenWidth;
    d3dpParam.BackBufferHeight = screenHeight;

	if(restart)
		return SUCCEEDED(pDevice->Reset(&d3dpParam));

	hr = d3d->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, 
				D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpParam, &pDevice); 

	return SUCCEEDED(hr); 
}

/***
 *
 */
bool DXEngine2D::clear(D3DCOLOR color)
{
	if(!pDevice)
		return false;

	if(SUCCEEDED(pDevice->Clear(0, NULL, D3DCLEAR_TARGET, color, 1.0f, 0)) )
		return true;

	return true;
}

/**
 *
 */
bool DXEngine2D::present()
{
	if(!pDevice)
		return false;

	if(SUCCEEDED(pDevice->Present(NULL, NULL, NULL, NULL) ))
		return true;

	return false;
}

/***
 *
 */
bool DXEngine2D::beginScene()
{
	if(!pDevice)
		return false;

	if(SUCCEEDED(pDevice->BeginScene()) )
		return true;

	return false;
}

/***
 *
 */
bool DXEngine2D::endScene()
{
	if(!pDevice)
		return false;

	if(SUCCEEDED(pDevice->EndScene()) )
		return true;

	return false;
}

/***
 *
 */
bool DXEngine2D::createBackground(LPCOMSTRING srcFile)
{
	if(!pDevice)
		return false;

	background = new DXBackground;
	background->setDevice(pDevice);

	return background->load(srcFile);
}

/**
 *
 */
bool DXEngine2D::createFontStyle(const char* styleName, const DXFont::FontStyle *fontStyle)
{
	DXFont font;
	font.setDevice(pDevice);
	
	if(!font.create(fontStyle))
		return false;

	fontStyles.insert( make_pair(styleName, font) );
		
	return true;
}

/***
 *
 */
DXFont * DXEngine2D::getFontStyle(const char *styleName)
{
	return &fontStyles[styleName];
}

/***
 *
 */
bool DXEngine2D::fontExists(const char *styleName)
{
	map<string, DXFont>::iterator it;
	it = fontStyles.find(styleName);

	return it != fontStyles.end();
}

/***
 *
 */
bool DXEngine2D::createSpriteSheet(const char *sheetName, LPCOMSTRING srcFile, const char *dataFile, D3DCOLOR transColor)
{
	DXSpriteSheet spr;
	if(!spr.setInterfaces(pDevice))
		return false;
	
	if(!spr.readSpriteData(dataFile))
		return false;
	
	if(!spr.create(srcFile, transColor))
		return false;

	spriteSheets.insert( make_pair(sheetName, spr) );

	return true;
}

/***
 *
 */
DXSpriteSheet * DXEngine2D::getSpriteSheet(const char *sheetName)
{
	return &spriteSheets[sheetName];
}

/*** 
 *
 */
bool DXEngine2D::spriteSheetExists(const char *sheetName)
{
	map<string, DXSpriteSheet>::iterator it;
	it = spriteSheets.find(sheetName);

	return it != spriteSheets.end();
}

/***
 *
 */
void DXEngine2D::releaseSpriteSheet(const char *sheetName)
{
	spriteSheets[sheetName].release();
	spriteSheets.erase(sheetName);
}

/***
 *	public method createButton()
 **
 *	Sukuria mygtuka
 **
 *  buttonName		- mygtuko pavadinimas
 *	sheetName	- egzistuojancio lapo, kuriame yra mygtuko spraitai
 *	spriteName	- egzistuojancio sprite objekto vardas. Taip pat pagal 
 *					ji bus pasiekiamas mygtukas 
 *	caption		- mygtuko tekstas
 *	fontStyle	- sukurto srifto vardas
 **
 *	Grazina true sekmes atveju, false - jei nepavyko sukurti
 */
bool DXEngine2D::createButton(const char *buttonName, const char *sheetName, const char *spriteName, LPCOMSTRING caption, const char *fontStyle)
{
	if(!spriteSheetExists(sheetName))
		return false;

	if(!spriteSheets[sheetName].createCopy(spriteName, buttonName))
		return false;

	DXButton btn;
	btn.setSprite(&spriteSheets[sheetName], buttonName);

	if(fontExists(fontStyle))
		btn.setFont(&fontStyles[fontStyle]);

	btn.setCaption(caption);

	buttons.insert( make_pair(buttonName, btn) );
	return true;
}

/***
 *
 */
DXButton * DXEngine2D::getButton(const char *btnName)
{
	return &buttons[btnName];
}

/***
 *
 */
bool DXEngine2D::buttonExists(const char *btnName)
{
	map<string, DXButton>::iterator it;
	it = buttons.find(btnName);

	return it != buttons.end();
}