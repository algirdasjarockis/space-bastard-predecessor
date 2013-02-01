#include "DXSpriteSheet.h"

/**
 * Konstruktorius
 */
DXSpriteSheet::DXSpriteSheet()
{
	pDevice = NULL;
	spriteObj = NULL;
	texture = NULL;
	
	ZeroMemory(&imageInfo, sizeof(imageInfo));
}

/**
 *	Destruktorius
 */
DXSpriteSheet::~DXSpriteSheet()
{
	//..
}

/***
 *
 */
void DXSpriteSheet::release()
{
	if(texture)
		texture->Release();
	if(spriteObj)
		spriteObj->Release();
}

/***
 *
 */
bool DXSpriteSheet::readSpriteData(const char *dataFile)
{
	ifstream fd(dataFile);

	string name;
	SpriteData sd;
	int left, top, right, bottom;

	if(fd.fail())
		return false;

	ZeroMemory(&sd, sizeof(sd));
	while(!fd.eof()) {	

		getline(fd, name);
		
		fd >> left >> top >> right >> bottom;
		SetRect(&sd.src, left, top, right, bottom);
	
		fd >> sd.totalFrames;
		sd.currFrame = 0;

		sd.center.x = sd.center.y = sd.center.z = 0;
		sd.position.x = sd.position.y = sd.position.z = 0;
		sd.color = D3DCOLOR_XRGB(255, 255, 255);

		sprites.insert( make_pair(name, sd) );
		fd.ignore();
	}

	fd.close();
	return true;
}

/**
 *
 */
bool DXSpriteSheet::setInterfaces(const LPDIRECT3DDEVICE9 pDevice)
{
	this->pDevice = pDevice;

	if(!this->pDevice)
		return false;

	if(SUCCEEDED(D3DXCreateSprite(this->pDevice, &spriteObj)))
		return true;

	return false;
}

/***
 *
 */
LPD3DXSPRITE DXSpriteSheet::getSpriteInterface()
{
	return spriteObj;
}

/***
 *
 */
void DXSpriteSheet::setCenter(const char * sprName, float x, float y)
{
	sprites[sprName].center.x = (float)x;
	sprites[sprName].center.y = (float)y;
	sprites[sprName].center.z = 0.0f;
}

/***
 *
 */
void DXSpriteSheet::setCenterAuto(const char * sprName)
{
	SpriteData *sd = &sprites[sprName];
	sd->center.x = float((sd->src.right - sd->src.left) / sd->totalFrames / 2);
	sd->center.y = float((sd->src.bottom - sd->src.top) / 2);

}

/***
 *
 */
void DXSpriteSheet::setPosition(const char * sprName, float x, float y, float depth)
{
	sprites[sprName].position.x = (float)x;
	sprites[sprName].position.y = (float)y;
	sprites[sprName].position.z = depth;
}

/***
 *
 */
void DXSpriteSheet::setColor(const char *sprName, D3DCOLOR color)
{
	sprites[sprName].color = color;
}

/***
 *
 */
D3DCOLOR DXSpriteSheet::getColor(const char *sprName)
{
	return sprites[sprName].color;
}

/***
 *
 */
D3DXVECTOR3 DXSpriteSheet::getPosition(const char *sprName)
{
	return sprites[sprName].position;
}

/***
 *
 */
D3DXVECTOR3 * DXSpriteSheet::getPosition(const char *sprName, int heh)
{
	return &sprites[sprName].position;
}

/***
 *
 */
void  DXSpriteSheet::getSize(const char *sprName, int *width, int *height)
{
	*width = (sprites[sprName].src.right - sprites[sprName].src.left) / sprites[sprName].totalFrames;
	*height = sprites[sprName].src.bottom - sprites[sprName].src.top;
}

/***
 *
 */
int DXSpriteSheet::getCurrentFrame(const char *sprName)
{
	return sprites[sprName].currFrame;
}

/***
 *
 */
int DXSpriteSheet::getFrameCount(const char *sprName)
{
	return sprites[sprName].totalFrames;
}

/***
 *
 */
void DXSpriteSheet::move(const char * sprName, float x, float y)
{
	sprites[sprName].position.x += x;
	sprites[sprName].position.y += y;
}

/***
 *
 */
void DXSpriteSheet::moveTo(const char * sprName, float x, float y, float speed)
{
	float vx = x;
	float vy = y;
	float l = sqrt(vx*vx + vy*vy);
	float nx = vx / l;
	float ny = vy / l;
	move(sprName, nx*speed, ny*speed);
	/*
	if(abs(x) < 1 || abs(y) < 1)
		return ;
	float destx = sprites[sprName].position.x + x;
	float desty = sprites[sprName].position.y + y;

	float projx = fabs(destx - sprites[sprName].position.x);
	float projy = fabs(desty - sprites[sprName].position.y);

	float xmov;
	float ymov;

	if(projy > projx) {
		speed = (speed > projy) ? projy : speed;
		ymov = speed;
		xmov = projx / projy * speed;
	} else {
		speed = (speed > projx) ? projx : speed;
		xmov = speed;
		ymov = projy / projx * speed;
	}

	if(destx < sprites[sprName].position.x)
		xmov = -xmov;

	if(desty < sprites[sprName].position.y)
		ymov = -ymov;

	move(sprName, xmov, ymov);
	*/
}

/***
 *
 */
bool DXSpriteSheet::create(LPCOMSTRING srcFile, D3DCOLOR transColor)
{
	if(!pDevice)
		return NULL;

	HRESULT hr = D3DXCreateTextureFromFileEx(pDevice,    // the device pointer
                            srcFile,    // the new file name
                            D3DX_DEFAULT_NONPOW2,    // default width
                            D3DX_DEFAULT_NONPOW2,   // default height
                            D3DX_DEFAULT,    // no mip mapping
                            NULL,    // regular usage
                            D3DFMT_A8R8G8B8,    // 32-bit pixels with alpha
                            D3DPOOL_MANAGED,    // typical memory handling
                            D3DX_DEFAULT,    // no filtering
                            D3DX_DEFAULT,    // no mip filtering
                            transColor,    // the hot-pink color key
                            &imageInfo,    // no image info struct
                            NULL,    // not using 256 colors
                            &texture);    // load to sprite

	if(SUCCEEDED(hr)) {
		return true;
	}
	
	return false;
}

/***
 *
 */
bool DXSpriteSheet::createCopy(const char *spriteName, const char *newSpriteName)
{
	map<string, SpriteData>::iterator it;
	it = sprites.find(spriteName);
	
	if(it == sprites.end())
		return false;

	sprites.insert( make_pair(newSpriteName, sprites[spriteName]) );
	return true;
}

/***
 *
 */
void DXSpriteSheet::setCurrentFrame(const char *spriteName, int frame)
{
	sprites[spriteName].currFrame = frame;
}

/***
 *
 */
bool DXSpriteSheet::showNextFrame(const char * spriteName)
{
	return showFrame(spriteName, ++sprites[spriteName].currFrame);
}

/***
 *
 */
bool DXSpriteSheet::showFrame(const char * spriteName, int frame)
{
	int frameWidth, frameHeight;

	if(sprites[spriteName].totalFrames > 0) {
		frameWidth = (sprites[spriteName].src.right - sprites[spriteName].src.left) 
						/ sprites[spriteName].totalFrames;
		frameHeight = sprites[spriteName].src.bottom - sprites[spriteName].src.top;
	}
	else return false;

	// neigiama reiksme nekeicia kadro :)
	if(frame >= 0)
		sprites[spriteName].currFrame = frame;

	if(sprites[spriteName].currFrame >= sprites[spriteName].totalFrames)
		sprites[spriteName].currFrame = 0;

	RECT src;
	int xpos, ypos;
	
	xpos = sprites[spriteName].currFrame * frameWidth + sprites[spriteName].src.left;
	ypos = sprites[spriteName].src.top;
	SetRect(&src, xpos, ypos, xpos + frameWidth, ypos + frameHeight);

	return draw(spriteName, &src, sprites[spriteName].color);	
}

/***
 *
 */
bool DXSpriteSheet::draw(const char * spriteName, const RECT * source, D3DCOLOR color)
{
	RECT src;
	D3DXVECTOR3 center(sprites[spriteName].center);
	D3DXVECTOR3 position(sprites[spriteName].position);

	if(source == NULL)
		src = sprites[spriteName].src;
	else src = *source;
	
	return SUCCEEDED(spriteObj->Draw(texture, &src, &center, &position, color) );
}

/***
 *
 */
bool DXSpriteSheet::scale(float x, float y)
{
	D3DXMATRIX scal;
	D3DXMatrixScaling(&scal, x, y, 1);

	return SUCCEEDED(spriteObj->SetTransform(&scal));
}

bool DXSpriteSheet::rotate(float degree)
{
	D3DXMATRIX rot;
	D3DXMatrixRotationZ(&rot, D3DXToRadian(degree));

	return SUCCEEDED(spriteObj->SetTransform(&rot));
}

/***
 *
 */
bool DXSpriteSheet::spriteExists(const char *spriteName)
{
	return sprites.find(spriteName) != sprites.end();
}

/***
 *
 */
void DXSpriteSheet::removeSprite(const char *spriteName)
{
	sprites.erase(spriteName);
}