#ifndef _SBSCENE_H_
#define _SBSCENE_H_

#include "../DXEngine2D/DXEngine2D.h"
#include "../DXEngine2D/DXSoundManager.h"
#include "../TinyXml/tinyxml.h"

#include "logger.h"
#include "Properties.h"
#include "SBEntity.h"
#include "../DXEngine2D/DXCollision.h"

#include <windows.h>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include "helpers.h"

using namespace std;

class SBScene
{
	DXEngine2D *_dx2d;
	DXSoundManager *_soundMgr;
	Logger *_lg;
	Properties *_props;

	struct RenderData 
	{
		string type;
		vector<string> objects;
	};
	map<string, RenderData> renderPipe;

	SBScene(DXEngine2D *dx2d, DXSoundManager *sndMgr, Logger *lg, Properties *prps);
	bool load(string fName);
	bool render();
};

/////////////////////////////////////////////////////////
// SCENE
/////////////////////////////////////////////////////////

/**
 *	Scenos tekstas
 */
class SBSceneText
{
public:
	virtual void render() =0;
	virtual int update() =0;
	virtual void addToRenderPipe() =0;
};

typedef int func(vector<string> args), (ordinary_func);
/**
 *	Scenos objektas (paveikslelis)
 */
class SBSceneEntity
{
public:

	SBSceneEntity();
	virtual void render() =0;
	virtual int update() =0;
	virtual void addToRenderPipe() =0;

//#define SCENE_ENTITY_FUNC_ARGS (vector<string> args);
//	void alpha SCENE_ENTITY_FUNC_ARGS;
	void alpha(vector<string> args);
/**
    funkcija [arg1 [, argN..]];
    
    ** keicia teksto permatomuma
    alpha galutine_reiksme keitimo_zingsnis

    ** keicia teksto dydi
    size galutine_reiksme keitimo_zingsnis

    ** judina x asimi
    movex zingsnis [galutine_reiksme_kuria_pasiekus_sustoja]
    
    ** judina y asimi
    movey zingsnis [galutine_reiksme_kuria_pasiekus_sustoja]
    
    ** slenka iki nurodyto tasko
    moveto greitis tikslas_x tikslas_y
**/
	string sprSheetName;
	DXSpriteSheet *spriteSheet;

	DXCollision<SBEntity> *collisions;
	map<string, deque<SBEntity*>> *scene;	

	Logger *lg;

	string sprite;

	// judejimo zingsniai ivairiomis kryptimis
	float speed, xspeed, yspeed;
};
#endif