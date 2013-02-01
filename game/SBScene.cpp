#include "SBScene.h"

/***
 *
 */
SBScene::SBScene(DXEngine2D *dx2d, DXSoundManager *sndMgr, Logger *lg, Properties *prps)
{
	_dx2d = dx2d;
	_soundMgr = sndMgr;
	_lg = lg;
	_props = prps;
}


/***
 *	Uzloadina nurodytos scenos objektus
 */
bool SBScene::load(string fName)
{
	string sceneFile = string("./") + _props->getValue("mod") + _props->getValue("scenes_dir") + fName;

	TiXmlDocument xmlDoc( sceneFile.c_str() );

	if(!xmlDoc.LoadFile()) {
		xmlDoc.Clear();
		_lg->log( (string("ERROR: ") + sceneFile + string(" was not found.")).c_str() );
		return false;
	}

	TiXmlElement *txeScene = xmlDoc.FirstChildElement("scene");
	if(!txeScene) {
		xmlDoc.Clear();
		_lg->log( (string("ERROR: bad scene file [") + fName + "]").c_str() );
		return false;
	}

	TiXmlElement *txeSceneItem = txeScene->FirstChildElement("item");
	while(txeSceneItem) {

		
		txeSceneItem = txeSceneItem->NextSiblingElement("item");
	}

	return true;
}

/***
 *
 */
bool SBScene::render()
{
	if(renderPipe.size() <= 0)
		return false;

	/*
	if(!_dx2d->background->isCreated())
		if(!_dx2d->createBackground(toWide( _bgFile.c_str() )) )
			lg->log("WARNING: background was not created at rendering.");

	_dx2d->background->scroll(0);
	*/

	map<string, RenderData>::iterator it = renderPipe.begin();
	while(it != renderPipe.end()) {

		// mygtukai
		if(0/*it->second.type == "button"*/) {
			_dx2d->getSpriteSheet(it->first.c_str())->getSpriteInterface()->Begin(D3DXSPRITE_ALPHABLEND);
			for(unsigned int i = 0; i < it->second.objects.size(); i++) {
				//..
				if(_dx2d->buttonExists(it->second.objects[i].c_str() ))
					_dx2d->getButton( it->second.objects[i].c_str() )->draw();
			}
			_dx2d->getSpriteSheet(it->first.c_str())->getSpriteInterface()->End();
		}
		else {
			// tekstai
			for(unsigned int i = 0; i < it->second.objects.size(); i++) {
				_dx2d->getFontStyle(it->first.c_str())->drawLabel(it->second.objects[i].c_str());
			}			
		}
		it++;
	}

	return true;
}

/***
 *
 */
void SBSceneEntity::alpha(vector<string> args)
{
	D3DCOLOR color = this->spriteSheet->getColor(this->sprSheetName.c_str());
	
}