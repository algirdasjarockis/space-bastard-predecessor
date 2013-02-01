#include "SBLevel.h"
/***
 *
 */
SBLevel::SBLevel(DXEngine2D *dx2d, DXSoundManager *sndMgr, Logger *lg, Properties *prps)
{
	_dx2d = dx2d;
	_soundMgr = sndMgr;
	_lg = lg;
	_props = prps;
	_topSquadEntity = _boss = NULL;
	_squadCount = 0;

	_levelLoaded = false;
}

/***
 *
 */
bool SBLevel::_loadEnemyData()
{
	// kuriamas spritesheet
	string sprDir = string("./") + _props->getValue("mod") + _props->getValue("sprites_dir");

	if(!_dx2d->createSpriteSheet(_props->getValue("sheet_enemies").c_str(), 
		toWide((sprDir+"enemies.png").c_str()), (sprDir+"enemies.txt").c_str(), D3DCOLOR_XRGB(0, 0, 0)) ) 
		_lg->log("WARNING: enemies spritesheet was not created.");

	string enemDataFile = string("./") + _props->getValue("mod") + "enemies.xml";

	TiXmlDocument xmlDoc( enemDataFile.c_str() );

	if(!xmlDoc.LoadFile()) {
		xmlDoc.Clear();
		_lg->log( (string("ERROR: ") + enemDataFile + string(" was not found.")).c_str() );
		return false;
	}

	TiXmlElement *txeEnemies = xmlDoc.FirstChildElement("enemies");
	if(!txeEnemies) {
		xmlDoc.Clear();
		_lg->log("ERROR: bad enemies file");
		return false;
	}

	TiXmlElement *txeEnemy = txeEnemies->FirstChildElement("enemy");
	while(txeEnemy) {
		SBEnemy *enem = new SBEnemy;
		enem->lg = _lg;
		enem->type = "enemy";
		enem->maxHealth = enem->health = atoi(txeEnemy->FirstChildElement("health")->FirstChild()->Value());
		enem->weight = atoi(txeEnemy->FirstChildElement("weight")->FirstChild()->Value());
		enem->speed = atof(txeEnemy->FirstChildElement("speed")->FirstChild()->Value());

		if(txeEnemy->FirstChildElement("mustdie"))
			enem->mustDie = true;
		else enem->mustDie = false;

		enem->moving = string(txeEnemy->FirstChildElement("moving")->FirstChild()->Value());
		enem->sprite = txeEnemy->Attribute("sprite");
		enem->sprSheetName = _props->getValue("sheet_enemies");
		enem->spriteSheet = _dx2d->getSpriteSheet(_props->getValue("sheet_enemies").c_str());

		enem->ammoSetName = string(txeEnemy->FirstChildElement("ammo")->FirstChild()->Value());
/*
enem->spriteSheet->setPosition(enem->sprite.c_str(), 300, 40, 0);
enem->spriteSheet->setCenterAuto(enem->sprite.c_str());
enem->renderPipe = renderPipe;
enem->addToRenderPipe();

enem->ammo["enemy_shooting1"] = (*ammo)["enemy_shooting1"];

enem->collisions = collisions;
enem->addToCollisionList();

enem->explSprite = "explosion1";
*/
		string enemName = txeEnemy->Attribute("name");
		enemies[enemName] = enem;

		txeEnemy = txeEnemy->NextSiblingElement("enemy");
	}

	return true;
}

/***
 *
 */
bool SBLevel::_loadPowerupData()
{
	// kuriamas spritesheet
	string sprDir = string("./") + _props->getValue("mod") + _props->getValue("sprites_dir");
	if(!_dx2d->createSpriteSheet(_props->getValue("sheet_powerups").c_str(), 
		toWide((sprDir+"powerups.png").c_str()), (sprDir+"powerups.txt").c_str(), D3DCOLOR_XRGB(255, 0, 255)) ) 
		_lg->log("WARNING: powerups spritesheet was not created.");

	string pwupDataFile = string("./") + _props->getValue("mod") + "powerups.xml";
	TiXmlDocument xmlDoc( pwupDataFile.c_str() );
	if(!xmlDoc.LoadFile()) {
		xmlDoc.Clear();
		_lg->log( (string("ERROR: ") + pwupDataFile + string(" was not found.")).c_str() );
		return false;
	}

	TiXmlElement *txePowerups = xmlDoc.FirstChildElement("powerups");
	if(!txePowerups) {
		xmlDoc.Clear();
		_lg->log("ERROR: bad powerups file");
		return false;
	}

	TiXmlElement *txePwup = txePowerups->FirstChildElement("powerup");
	while(txePwup) {
		SBPowerup *pwup = new SBPowerup;

		pwup->isEnemy = true;		// tam, kad veiktu collisionai
		pwup->weight = 0;			// kad nedarytu zalos atsitrenkus
		pwup->speed = 3;
		pwup->type = "powerup";
		pwup->collisions = collisions;
		pwup->sprSheetName = _props->getValue("sheet_powerups");
		pwup->spriteSheet = _dx2d->getSpriteSheet(_props->getValue("sheet_powerups").c_str());

		pwup->sprite = txePwup->Attribute("sprite");

		string type = txePwup->Attribute("type");
		pwup->upgradeType = type;
		if(type == "health")
			pwup->health = atoi(txePwup->FirstChildElement("value")->FirstChild()->Value());
		else if(type == "ammo") 
			pwup->ammoName = txePwup->FirstChildElement("value")->FirstChild()->Value();

		powerups[txePwup->Attribute("name")] = pwup;
/*
pwup->spriteSheet->setPosition(pwup->sprite.c_str(), 650, -20);
pwup->spriteSheet->setCenterAuto(pwup->sprite.c_str());
pwup->addToCollisionList();
pwup->renderPipe = renderPipe;
pwup->addToRenderPipe();
*/
		txePwup = txePwup->NextSiblingElement("powerup");
	}

	return true;
}

void SBLevel::_showSquad(unsigned int squadNo)
{
	_topSquadEntity = NULL;
	for(int i = 0; i < _lvlPipe[squadNo].size(); i++) {
		if(_lvlPipe[squadNo][i].type == LevelEntity::LevelEntityType::enemy) {
			SBEnemy *enem = new SBEnemy(*(enemies[_lvlPipe[squadNo][i].name]));

			char spriteNo[10];
			itoa(i*10000+squadNo, spriteNo, 10);

			if(enem->spriteSheet->createCopy(enem->sprite.c_str(), (enem->sprite+string(spriteNo)).c_str()) ) {

				enem->sprite = (enem->sprite+spriteNo).c_str();
				enem->moving = _lvlPipe[squadNo][i].moving;

				if(_lvlPipe[squadNo][i].health > 0)
					enem->health = enem->maxHealth = _lvlPipe[squadNo][i].health;

				if(_lvlPipe[squadNo][i].weight > 0)
					enem->weight = _lvlPipe[squadNo][i].weight;

				enem->yspeed = enem->speed = _lvlPipe[squadNo][i].yspeed;
				enem->xspeed = _lvlPipe[squadNo][i].xspeed;

				if(_lvlPipe[squadNo][i].moving[0] == 'y' && _lvlPipe[squadNo][i].yspeed > 0)
					enem->speed = enem->yspeed = _lvlPipe[squadNo][i].yspeed;

				enem->myWorstEnemyEver = hero;

				enem->spriteSheet->setPosition(enem->sprite.c_str(), _lvlPipe[squadNo][i].x, _lvlPipe[squadNo][i].y - 600, 0);
				enem->spriteSheet->setCenterAuto(enem->sprite.c_str());
				enem->renderPipe = renderPipe;
				enem->addToRenderPipe();

				enem->ammo[enem->ammoSetName] = (*ammo)[enem->ammoSetName];
				//enem->ammo["enemy_shooting1"] = (*ammo)["enemy_shooting1"];
				enem->collisions = collisions;
				enem->addToCollisionList();

				enem->explSprite = "explosion1";

				if(!_topSquadEntity)
					_topSquadEntity = enem;
				else if(enem->spriteSheet->getPosition(enem->sprite.c_str()).y < 
					_topSquadEntity->spriteSheet->getPosition(_topSquadEntity->sprite.c_str()).y)
					_topSquadEntity = enem;

				if(!_boss && enem->mustDie)
					_boss = enem;
			}
		}
		else if(_lvlPipe[squadNo][i].type == LevelEntity::LevelEntityType::powerup) {
			SBPowerup* pwup = new SBPowerup(*(powerups[_lvlPipe[squadNo][i].name]));

			char spriteNo[10];
			itoa(i, spriteNo, 10);

			if(pwup->spriteSheet->createCopy(pwup->sprite.c_str(), (pwup->sprite+string(spriteNo)).c_str()) ) {
				pwup->sprite = pwup->sprite+string(spriteNo);
				pwup->spriteSheet->setPosition(pwup->sprite.c_str(), _lvlPipe[squadNo][i].x, _lvlPipe[squadNo][i].y - 600);
				pwup->spriteSheet->setCenterAuto(pwup->sprite.c_str());
				pwup->addToCollisionList();
				pwup->renderPipe = renderPipe;
				pwup->addToRenderPipe();
			}
		}	
	}
}

/***
 *
 */
bool SBLevel::load(int level)
{
	_levelLoaded = false;
	_levelFinished = false;
	_level = level;
	_time = 0;
	char chLevel[3];
	itoa(_level, chLevel, 10);

	_lg->log("Loading enemy data..");
	if(!_loadEnemyData()) {
		_lg->log("FAILED!");
		return false;
	}

	_lg->log("Loading powerup data..");
	if(!_loadPowerupData()) {
		_lg->log("FAILED!");
		return false;
	}

	string lvlDir = string("./") + _props->getValue("mod") + _props->getValue("levels_dir");
	string lvlFile = lvlDir + string( chLevel ) + string(".xml");

	TiXmlDocument xmlDoc( lvlFile.c_str() );

	if(!xmlDoc.LoadFile()) {
		xmlDoc.Clear();
		_lg->log( (string("ERROR: ") + lvlFile + string(" was not found.")).c_str() );
		return false;
	}

	TiXmlElement *txeLevel = xmlDoc.FirstChildElement("level");
	if(!txeLevel) {
		xmlDoc.Clear();
		_lg->log("ERROR: bad level file");
		return false;
	}

	// kelias iki fonu katalogo
	string bgDir = string("./") + _props->getValue("mod") + _props->getValue("bgr_dir");

	// kelias iki fono
	_bgFile = bgDir + string(txeLevel->Attribute("background"));
	
	// kelias iki muzikos
	string soundDir = string("./") + _props->getValue("mod") + _props->getValue("sounds_dir");	
	if(!_soundMgr->createTrack("lvlmusic", (soundDir + string(txeLevel->Attribute("sound"))).c_str() ) )
		_lg->log("WARNING: level music was not loaded..");

	_lvlPipe.clear();
	_currentSquad = 0;
	vector<string> lvlEntitiesTags;
	lvlEntitiesTags.push_back("enemy");
	lvlEntitiesTags.push_back("pickup");

	unsigned int squadNo = 0;
	TiXmlElement *txeSquads = txeLevel->FirstChildElement("squads");
	if(txeSquads) {
		TiXmlElement *squad = txeSquads->FirstChildElement("squad");
		while(squad) {
			for(int i = 0; i < lvlEntitiesTags.size(); i++) {
				TiXmlElement *ent = squad->FirstChildElement(lvlEntitiesTags[i].c_str());
				while(ent) {
					// ent elementu apdorojimas
					LevelEntity le;
					le.xspeed = le.yspeed = 0;
					le.health = 0;
					le.weight = 0;
//					strcpy(le.moving, "y");
					le.moving = "y";
					le.minDifficulty = 1;
					if(lvlEntitiesTags[i] == "enemy")
						le.type = LevelEntity::LevelEntityType::enemy;
					else le.type = LevelEntity::LevelEntityType::powerup;

	int langelioPlotis = 60;
	int langelioAukstis = 60;

					int coordRow, coordCol = 0;
					if(ent->Attribute("row")) {
						coordRow = atoi(ent->Attribute("row"));
						le.y = 600 - (langelioAukstis + coordRow * langelioAukstis);
					}
					else ;

				if(ent->Attribute("col")) {
						coordCol = atoi(ent->Attribute("col"));
						le.x = /*langelioPlotis + */coordCol * langelioPlotis;
					}
					else ;

					if(ent->Attribute("speed"))
						le.xspeed = le.yspeed = atof(ent->Attribute("speed"));

					if(ent->Attribute("yspeed"))
						le.yspeed = atof(ent->Attribute("yspeed"));

					if(ent->Attribute("xspeed"))
						le.xspeed = atof(ent->Attribute("xspeed"));

					if(ent->Attribute("name"))
						le.name = string(ent->Attribute("name"));
					else ;

					if(ent->Attribute("moving")) 
						if(strlen(ent->Attribute("moving")) < 5)
							//strcpy(le.moving, ent->Attribute("moving"));
							le.moving = ent->Attribute("moving");
						else ;

					if(ent->Attribute("health"))
						le.health = atoi(ent->Attribute("health"));

					if(ent->Attribute("weight"))
						le.weight = atoi(ent->Attribute("weight"));

					if(ent->Attribute("mindifficulty"))
						le.minDifficulty = atoi(ent->Attribute("mindifficulty"));

					_lvlPipe[squadNo].push_back(le);

					ent = ent->NextSiblingElement(lvlEntitiesTags[i].c_str());
				}
			}

			squad = squad->NextSiblingElement("squad");
			squadNo++;
		} // while(squad)
	}

	_squadCount = squadNo;
	_showSquad(_currentSquad);
	_levelLoaded = true;
	return true;
}

/***
 *
 */
bool SBLevel::unLoad()
{
	if(!isLoaded())
		return false;

	_soundMgr->stopAll();
	_soundMgr->removeTrack("lvlmusic");

	_dx2d->releaseSpriteSheet(_props->getValue("sheet_enemies").c_str());
	_dx2d->background->release();

	_lvlPipe.clear();
	_bgFile.clear();

	//map<string, SBEnemy*>::iterator it;
	_levelFinished = true;
	_levelLoaded = false;

	enemies.clear();

	collisions->removeFriends();
	collisions->removeEnemies();

	_squadCount = 0;

	return true;
}

/***
 *
 */
bool SBLevel::isLoaded()
{
	return _levelLoaded;
}

/***
 *
 */
const char * SBLevel::getBackgroundFile()
{
	return _bgFile.c_str();
}

/***
 *
 */
void SBLevel::run()
{
	if(!_levelLoaded || _levelFinished)
		return;

	if(_boss && _boss->health <= 0) {
		_levelFinished = true;
		return;
		/*
		if(_currentSquad >= _squadCount-1) {
			// baigesi levelis :( :D
			_levelFinished = true;
			return;
		}
		*/
	}
	if(_topSquadEntity && _topSquadEntity->spriteSheet->getPosition(_topSquadEntity->sprite.c_str()).y >= 0) {
	//	_lg->appendVisualText("SQUAD ENDED!!!\n");
		if(!_boss && _currentSquad >= _squadCount-1) {
			// baigesi levelis :( :D
			//unLoad();
			_levelFinished = true;
			return;
		}
		_showSquad(++_currentSquad);
	}

	_dx2d->background->scroll(1);
}

/***
 *
 */
void SBLevel::startTiming()
{
	_time = GetTickCount();
}

/***
 *
 */
DWORD SBLevel::getTime()
{
	return _realTime;
}

/***
 *
 */
bool SBLevel::levelFinished()
{
	return _levelFinished;
}