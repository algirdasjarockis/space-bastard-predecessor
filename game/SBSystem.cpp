#include "SBSystem.h"

/***
 *
 */
SBSystem::SBSystem()
{
	_width = 800;
	_height = 600;
	_fullscreen = true;
	_configLoaded = false;
	_ammoDataLoaded = false;
	_hwnd = 0;
	_hinst = 0;

	_dx2d = new DXEngine2D;
	_input = NULL;
	level = NULL;
	soundMgr = NULL;

	lg = new Logger("log.txt");
	gui = NULL;

	collisions = new DXCollision<SBEntity>;

	_player = new SBPlayer(0);

	_inGame = false;
	_inGui = true;
	_inLoadingProgress = false;

	_guiScreen = "main";
	_guiMusic = _guiScreen;

	props = new Properties;
	props->setValue("base_mod", "base/");
	props->setValue("gui_dir", "graphics/GUI/");
	props->setValue("sounds_dir", "sounds/");
	props->setValue("bgr_dir", "graphics/backgrounds/");
	props->setValue("sprites_dir", "graphics/sprites/");
	props->setValue("levels_dir", "levels/");
	props->setValue("scenes_dir", "scenes/");

	props->setValue("sheet_ammo", "z_ammo");
	props->setValue("sheet_hero", "hero");
	props->setValue("sheet_enemies", "enemies");
	props->setValue("sheet_powerups", "powerups");
	props->setValue("sheet_explosions", "explosions");
	props->setValue("sheet_guielements", "elements");
}

/***
 *
 */
void SBSystem::release()
{
	lg->log("Releasing graphics..");
	if(_dx2d) {
		delete _dx2d;
	}

	lg->log("Shutdowning input system..");
	if(_input)
		delete _input;

	lg->log("Releasing sound manager..");
	if(soundMgr)
		delete soundMgr;

	lg->log("Shutdowning..");
	lg->close();
	delete lg;
	delete _player;
}

/***
 *
 */
void SBSystem::setWindowHandle(HWND hwnd)
{
	_hwnd = hwnd;
}

/***
 *
 */
void SBSystem::setInstance(HINSTANCE hinst)
{
	_hinst = hinst;
}

/***
 *
 */
bool SBSystem::loadConfig()
{
	TiXmlDocument config("config.xml");

	lg->log("Loading Config..");
	if(!config.LoadFile()) {
		lg->log("ERROR: config file not found");
		config.Clear();
		return false;
	}

	TiXmlString val = config.FirstChildElement("config")->FirstChild("resolution")->FirstChildElement("width")->FirstChild()->ValueTStr();
	if(!val.empty())
		_width = atoi(val.c_str());
	
	val = config.FirstChildElement("config")->FirstChild("resolution")->FirstChildElement("height")->FirstChild()->ValueTStr();
	if(!val.empty())
		_height = atoi(val.c_str());

	val = config.FirstChildElement("config")->FirstChildElement("fullscreen")->FirstChild()->ValueTStr();
	_fullscreen = (val == "1") ? true : false;

	if(config.FirstChildElement("config")->FirstChildElement("mod")) {
		val = config.FirstChildElement("config")->FirstChildElement("mod")->FirstChild()->ValueTStr();
		string value(val.c_str());

		value.insert(value.length(), "/");
		props->setValue("mod", value.c_str() );
		lg->log( (string("Selected mod: ") + value).c_str() );
	}
	else props->setValue("mod", props->getValue("base_mod").c_str() );

	_configLoaded = true;
	return true;
}

/***
 *
 */
bool SBSystem::init()
{
	wchar_t currDir[255];
	GetCurrentDirectory(255, currDir);

	lg->log( (string("Working directory is ") + string( toString(currDir) )).c_str() );

	if(!_configLoaded)
		if(!loadConfig())
			return false;

	lg->log("Initializing graphics..");
	if(!_dx2d->init(_hwnd, _width, _height, _fullscreen)) {
		lg->log("FAILED!!");
		return false;
	}

	_input = new DXInput(_hinst);
	lg->log("Initializing mouse..");
	if(!_input->initMouse(_hwnd)) {
		lg->log("FAILED!!");
		return false;
	}

	lg->log("Initializing keyboard..");
	if(!_input->initKeyboard(_hwnd)) {
		lg->log("FAILED!!");
		return false;
	}

	lg->log("Initializing sound manager..");
	soundMgr = new DXSoundManager(_hwnd);

	if(!initGui())
		return false;

	return true;
}

/**
 *
 */
int SBSystem::getWidth()
{
	return _width;
}

/**
 *
 */
int SBSystem::getHeight()
{
	return _height;
}

/***
 *
 */
bool SBSystem::isFullScreen()
{
	return _fullscreen;
}

/***
 *
 */
bool SBSystem::startRendering()
{
	_dx2d->clear(D3DCOLOR_XRGB(0, 0, 0));
	return _dx2d->beginScene();
}

/***
 *
 */
bool SBSystem::stopRendering()
{
	if(!_dx2d->endScene())
		return false;

	return _dx2d->present();
}

/***
 *
 */
bool SBSystem::initGui()
{
	gui = new GuiSystem(lg, soundMgr, props);

	lg->log("Initializing GUI..");
	gui->init(_dx2d);

	if(soundMgr->trackExists(_guiMusic.c_str()))
		soundMgr->play(_guiMusic.c_str());

	return true;
}

/***
 *
 */
bool SBSystem::renderGui()
{	
	_guiScreen = (_inGame) ? "game_menu" : "main";
	return gui->render(_guiScreen);
}

/***
 *
 */
bool SBSystem::inGame()
{
	return _inGame;
}

/***
 *
 */
bool SBSystem::inGui()
{
	return _inGui;
}

/***
 *	"Klausosi" peles ir klavos, ir kartu vykdo tam tikrus metodus 
 *	priklausomus nuo paspaudimo ir t.t.
 */
void SBSystem::listenForInput()
{
	_mouseState = _input->getMouseInput(_xMouseMov, _yMouseMov, _zMouseMov);

	if(!_inGame && _inGui) {
		// jei rodomas pagrindinis GUI tai ziurim, koks mygtukas paspaustas..
		D3DXVECTOR3 cursorPos = _dx2d->getSpriteSheet(props->getValue("sheet_guielements").c_str())->getPosition("pointer");
		int startStatus = _dx2d->getButton("start")->getStatus(cursorPos.x + _xMouseMov, cursorPos.y + _yMouseMov);
		int fScreenStatus = _dx2d->getButton("fullscreen")->getStatus(cursorPos.x + _xMouseMov, cursorPos.y + _yMouseMov);
		int exitStatus = _dx2d->getButton("exit")->getStatus(cursorPos.x + _xMouseMov, cursorPos.y + _yMouseMov);

		_buttonState = CLICKED_NONE;
		switch (_mouseState) {
			case PRESSED_LEFT :
				if(startStatus == DXButton::MOUSE_OVER) {
					_buttonState = CLICKED_START;
					break;
				}
				if(fScreenStatus == DXButton::MOUSE_OVER) {
					_buttonState = CLICKED_FULLSCREEN;
					break;
				}
				if(exitStatus == DXButton::MOUSE_OVER) {
					_buttonState = CLICKED_EXIT;
					break;
				}
				break;
		}
	}
	else if (_inGame && _inGui){
		// jei zaidziama ir rodomas gui
		D3DXVECTOR3 cursorPos = _dx2d->getSpriteSheet(props->getValue("sheet_guielements").c_str())->getPosition("pointer");
		int resumeStatus = _dx2d->getButton("resume")->getStatus(cursorPos.x + _xMouseMov, cursorPos.y + _yMouseMov);
		int stopStatus = _dx2d->getButton("stop")->getStatus(cursorPos.x + _xMouseMov, cursorPos.y + _yMouseMov);
		int fScreenStatus = _dx2d->getButton("fullscreen2")->getStatus(cursorPos.x + _xMouseMov, cursorPos.y + _yMouseMov);
		int exitStatus = _dx2d->getButton("exit2")->getStatus(cursorPos.x + _xMouseMov, cursorPos.y + _yMouseMov);

		_buttonState = CLICKED_NONE;
		switch (_mouseState) {
			case PRESSED_LEFT :
				if(resumeStatus == DXButton::MOUSE_OVER) {
					_buttonState = CLICKED_RESUME;
					break;
				}
				if(stopStatus == DXButton::MOUSE_OVER) {
					_buttonState = CLICKED_STOP;
					break;
				}
				if(fScreenStatus == DXButton::MOUSE_OVER) {
					_buttonState = CLICKED_FULLSCREEN;
					break;
				}
				if(exitStatus == DXButton::MOUSE_OVER) {
					_buttonState = CLICKED_EXIT;
					break;
				}
				break;
		}
	}
	else if (!_inGui && _inGame){
		// zaidimo metu..
		//if(_input->keyPressed(DIK_ESCAPE)) {
		//	returnToGui();
		//}
	}
}

/***
 *
 */
void SBSystem::updateCursor()
{
		D3DXVECTOR3 cursorPos = _dx2d->getSpriteSheet(props->getValue("sheet_guielements").c_str())->getPosition("pointer");
		
		if((cursorPos.x + _xMouseMov) <= 0 || (cursorPos.x + _xMouseMov) >= _width)
			_xMouseMov = 0;

		if((cursorPos.y + _yMouseMov) <= 0 || (cursorPos.y + _yMouseMov) >= _height)
			_yMouseMov = 0;

		_dx2d->getSpriteSheet(props->getValue("sheet_guielements").c_str())->move("pointer", 
				(float)_xMouseMov, (float)_yMouseMov);
}

/***
 *
 */
int SBSystem::buttonClicked()
{
	return _buttonState;
}

/***
 *
 */
int SBSystem::mouseClicked()
{
	return _mouseState;
}

/***
 *
 */
bool SBSystem::keyPressed(long dikCode)
{
	return _input->keyPressed(dikCode);
}

/***
 *
 */
void SBSystem::playButtonSound(const char *btnName)
{
	soundMgr->play(_dx2d->getButton(btnName)->getSound(), false);
}

/***
 *
 */
void SBSystem::startLoading()
{
	level = new SBLevel(_dx2d, soundMgr, lg, props);
	level->renderPipe = &renderPipe;
	//level->hero = this->
	level->ammo = &this->ammo;

	if(!collisions)
		collisions = new DXCollision<SBEntity>;
	level->collisions = collisions;
	collisions->resetNamingId();

	_inGame = false;
	_inLoadingProgress = true;
	soundMgr->stop(_guiMusic.c_str());

	// kelias iki muzikos
	string sndDir = string("./") + props->getValue("mod") + props->getValue("sounds_dir");

	// duru garsai
	if(!soundMgr->trackExists("doors_open"))
		if(!soundMgr->createTrack("doors_open", (sndDir + string("doors_open.wav")).c_str() ))
			lg->log("WARNING: doors_open track was not created.");

	if(!soundMgr->trackExists("doors_close")) 
		if(!soundMgr->createTrack("doors_close", (sndDir + string("doors_close.wav")).c_str() ))
			lg->log("WARNING: doors_close track was not created.");

	// saudymo garsai
	if(!soundMgr->trackExists("hero_shoot")) 
		if(!soundMgr->createTrack("hero_shoot", (sndDir + string("pulse.wav")).c_str() ))
			lg->log("WARNING: hero_shoot track was not created.");	

	// sprogimai
	if(!soundMgr->trackExists("explosion")) 
		if(!soundMgr->createTrack("explosion", (sndDir + string("boom.wav")).c_str() ))
			lg->log("WARNING: explosion track was not created.");

	// powerups
	if(!soundMgr->trackExists("powerup")) 
		if(!soundMgr->createTrack("powerup", (sndDir + string("Tek_Open-Marco-7541.wav")).c_str() ))
			lg->log("WARNING: powerup track was not created.");

	Sleep(250);

	if(soundMgr->trackExists("doors_open"))
		soundMgr->play("doors_open", false);


	// kelias iki fonu katalogo
	string bgDir = string("./") + props->getValue("mod") + props->getValue("bgr_dir");

	// sukuriamos "durys"
	_dx2d->createBackground(toWide((bgDir + string("doors.jpg") ).c_str()) );

		lg->log("Loading sprites..");
		loadSprites();

		if(!_ammoDataLoaded) {
			lg->log("Loading ammo data..");
			loadAmmoData();
		}

	_player->setLives(3);
	createHero();

	lg->log("Loading level..");
	if(level->load(1)) {

	}

	_inGui = true;
}

/***
 *
 */
bool SBSystem::loadSprites()
{
	// ... 
	string sprDir = string("./") + props->getValue("mod") + props->getValue("sprites_dir");
	string heroSprite = sprDir + string("hero.png");

	// hero sprite
	if(!_dx2d->spriteSheetExists(props->getValue("sheet_hero").c_str()))
		if(!_dx2d->createSpriteSheet(props->getValue("sheet_hero").c_str(), toWide(heroSprite.c_str()), 
			(sprDir + string("hero.txt")).c_str(), D3DCOLOR_XRGB(0, 0, 0)) ) {
			lg->log("WARNING: hero was not created successfully.");
			return false;
		}

	// ammo sprite
	string ammoSprite = sprDir + string("ammo.png");
	if(!_dx2d->spriteSheetExists(props->getValue("sheet_ammo").c_str()))
		if(!_dx2d->createSpriteSheet(props->getValue("sheet_ammo").c_str(), toWide(ammoSprite.c_str()), 
			(sprDir + string("ammo.txt")).c_str(), D3DCOLOR_XRGB(0, 0, 0)) ) {
			lg->log("WARNING: ammo was not created successfully.");
			return false;
		}

	return true;
}

/***
 *
 */
bool SBSystem::loadAmmoData()
{
	_ammoDataLoaded = false;

	string ammoData = string("./") + string(props->getValue("mod")) + string("ammo.xml");
	TiXmlDocument xmlDoc( ammoData.c_str() );

	if(!xmlDoc.LoadFile()) {
		lg->log("ERROR: ammo data file not found.");
		xmlDoc.Clear();
		return false;
	}

	TiXmlElement *ammunition = xmlDoc.FirstChildElement("ammunition");
	if(!ammunition) {
		lg->log("ERROR: bad ammo data file.");
		return false;
	}
/*
	TiXmlElement *ammo = ammunition->FirstChildElement("ammo");
	while(ammo) {
		SBAmmo *am = new SBAmmo;

		am->sprite = ammo->Attribute("sprite");
		am->explSprite = ammo->Attribute("explsprite");

		am->yspeed = atof(ammo->FirstChildElement("yspeed")->FirstChild()->Value());
		am->xspeed = atof(ammo->FirstChildElement("xspeed")->FirstChild()->Value());
		am->damage = atoi(ammo->FirstChildElement("damage")->FirstChild()->Value());
		am->delay = atoi(ammo->FirstChildElement("delay")->FirstChild()->Value());

		am->delayed = 0;
		am->type = "ammo";
		am->weight = 1;
		am->sprSheetName = props->getValue("sheet_ammo").c_str();
		am->spriteSheet = _dx2d->getSpriteSheet(props->getValue("sheet_ammo").c_str());
		am->health = 1;
		am->renderPipe = &renderPipe;

		this->ammo[ammo->Attribute("name")] = am;
		ammo = ammo->NextSiblingElement("ammo");
	}
*/

	TiXmlElement *ammoSet = ammunition->FirstChildElement("ammoset");
	while(ammoSet) {
		TiXmlElement *ammo = ammoSet->FirstChildElement("ammo");
		while(ammo) {
			SBAmmo *am = new SBAmmo;

			am->lg = lg;

			am->sprite = ammo->Attribute("sprite");
			am->explSprite = ammo->Attribute("explsprite");

			am->yspeed = atof(ammo->FirstChildElement("yspeed")->FirstChild()->Value());
			am->xspeed = atof(ammo->FirstChildElement("xspeed")->FirstChild()->Value());
			am->damage = atoi(ammo->FirstChildElement("damage")->FirstChild()->Value());
			am->delay = atoi(ammo->FirstChildElement("delay")->FirstChild()->Value());
			am->isDouble = atoi(ammo->FirstChildElement("double")->FirstChild()->Value()) == 1 ? true : false;

			am->delayed = 0;
			am->type = "ammo";
			am->weight = 1;
			am->sprSheetName = props->getValue("sheet_ammo").c_str();
			am->spriteSheet = _dx2d->getSpriteSheet(props->getValue("sheet_ammo").c_str());
			am->health = 1;
			am->renderPipe = &renderPipe;

			string ammoSetName = ammoSet->Attribute("name");
			int ammoNo = atoi(ammo->Attribute("id"));

			this->ammo[ammoSet->Attribute("name")][ammoNo] = *am;
			ammo = ammo->NextSiblingElement("ammo");
		}
		ammoSet = ammoSet->NextSiblingElement("ammoset");
	}

	_ammoDataLoaded = true;
	_player->ammo = &ammo;
	return true;
}

/***
 *
 */
bool SBSystem::inLoadingProgress()
{
	return _inLoadingProgress;
}

/***
 *
 */
void SBSystem::showLoadingProgress()
{
	if(_dx2d->background->effectDoorCloseVertical(40)) {

		_inLoadingProgress = true;

		if(level->isLoaded()) {
			if(soundMgr->trackExists("doors_close") && !soundMgr->isPlaying("doors_close"))
				soundMgr->play("doors_close", false);
			
			if(_dx2d->background->effectDoorOpenVertical(40)) {
				// pradeti zaidima
				startGame();
			}
		}
	}
}

/***
 *
 */
void SBSystem::startGame()
{
	_inLoadingProgress = false;
	_inGame = true;
	_inGui = false;
	_guiScreen = "game_menu";
	_guiMusic = _guiScreen;	

	if(!_dx2d->createBackground(toWide(level->getBackgroundFile())) )
		lg->log("WARNING: level background was not created.");
	
	if(soundMgr->trackExists("lvlmusic"))
		soundMgr->play("lvlmusic");

	level->startTiming();
}

/***
 *
 */
void SBSystem::stopGame()
{
	_inGame = false;
	_inGui = true;
	_guiScreen = "main";
	_guiMusic = _guiScreen;

	renderPipe.clear();
	if(collisions == NULL)
		delete collisions;

	level->unLoad();
}

/***
 *
 */
void SBSystem::returnToGui()
{
	//level->unLoad();
	_dx2d->getSpriteSheet(props->getValue("sheet_guielements").c_str())->setPosition("pointer", _width / 2, _height / 2);
	soundMgr->stopAll();
	soundMgr->play(_guiMusic.c_str());
	_inGui = true;
}

/***
 *
 */
void SBSystem::returnToGame()
{
	soundMgr->stopAll();
	soundMgr->play("lvlmusic");
	_inGame = true;
	_inGui = false;
	_inLoadingProgress = false;
}

/***
 *
 */
void SBSystem::createHero()
{
	_heroDied = false;
	//renderPipe.erase(props->getValue("sheet_hero").c_str());
	SBHero *hero = new SBHero;

	hero->spriteSheet = _dx2d->getSpriteSheet(props->getValue("sheet_hero").c_str());

	hero->lg = lg;
	hero->health = 500;
	hero->speed = 10;
	hero->weight = 300;
	hero->sprite = "hero";
	hero->type = "hero";
	hero->explSprite = "explosion";
	hero->secWepData.chargeTime = 6000;
	hero->secWepData.fireTime = 5000;
	hero->sprSheetName = props->getValue("sheet_hero").c_str();
	hero->spriteSheet->setPosition("hero", 400, 500);
	hero->spriteSheet->setCenterAuto("hero");
	hero->ammoSheet = _dx2d->getSpriteSheet(props->getValue("sheet_ammo").c_str());
	hero->renderPipe = &renderPipe;

	hero->collisions = collisions;
	hero->addToCollisionList();

	hero->ammo["meteor_shower"][1] = this->ammo["meteor_shower"][1];
	/*hero->ammo["meteor_shower"][2] = this->ammo["meteor_shower"][2];
	hero->ammo["meteor_shower"][3] = this->ammo["meteor_shower"][3];
	hero->ammo["meteor_shower"][4] = this->ammo["meteor_shower"][4];
	hero->ammo["meteor_shower"][5] = this->ammo["meteor_shower"][5];*/

	hero->addToRenderPipe();

	_player->setHero(hero);
	_player->setMaxHealth(hero->health);
	level->hero = hero;
}

/***
 *
 */
void SBSystem::updateHero()
{
	if(_heroDied) 
		return;

	SBHero *sb = dynamic_cast<SBHero*>(renderPipe[props->getValue("sheet_hero")][0]);
	if(_mouseState == PRESSED_LEFT) {
		if(sb) {
			if(sb->shoot())
				soundMgr->play("hero_shoot", false);
			//sb->shootSecondary();
		}
	}
	else if(_mouseState == PRESSED_RIGHT) {
		if(sb) {
			//sb->shootSecondary();
		}
	}

	D3DXVECTOR3 cursorPos = _dx2d->getSpriteSheet(props->getValue("sheet_hero").c_str())->getPosition("hero");
		
	if((cursorPos.x + _xMouseMov) <= 0 || (cursorPos.x + _xMouseMov) >= _width)
		_xMouseMov = 0;

	if((cursorPos.y + _yMouseMov) <= 0 || (cursorPos.y + _yMouseMov) >= _height)
		_yMouseMov = 0;
	
	/*
	DXSpriteSheet *hero = _dx2d->getSpriteSheet(props->getValue("sheet_hero").c_str());
	float destX, destY;
	destX = cursorPos.x + (float)_xMouseMov;
	destY = cursorPos.y + (float)_yMouseMov;
	
	_dx2d->getSpriteSheet("hero")->moveTo("hero", destX, destY, 1.0f);
	
	char msg[200];
	sprintf(msg, "xMouse = %3.3f, yMouse = %3.3f\n", (float)_xMouseMov, (float)_yMouseMov);
	lg->setVisualText(msg);*/
	_dx2d->getSpriteSheet(props->getValue("sheet_hero").c_str())->move("hero", _xMouseMov/2, _yMouseMov/2);
/*
	if(_player->isRecharging())
		_player->recharge();
	else if(_player->isSecShooting())
		_player->updateSecShooting();*/
}

/***
 *
 */
void SBSystem::updateEntities()
{
	map<string, deque<SBEntity*>>::iterator it;

	//vector<string> collidedEntities = collisions->getOponentCollisions();
	//map<string, void*> collidedEntities = collisions->getOponentCollisions();
	collisions->getOponentCollisions();

	for(it = renderPipe.begin(); it != renderPipe.end(); it++) {
		for(unsigned int i = 0; i < it->second.size(); i++) {		

			// tikrinam visu objektu busenas
			switch(it->second[i]->update()) {
				
				// jei objektas dingo uz ekrano ribu
				case SBEntity::STATUS_GONE:
					_recycleBin.insert(_recycleBin.begin(), it->second[i]);
					it->second.erase(it->second.begin() + i);
					break;

				// jei priesas mire
				case SBEntity::STATUS_ENEMY_DIED:
					_recycleBin.insert(_recycleBin.begin(), it->second[i]);

					_player->addScore( ((SBEnemy*)it->second[i])->maxHealth );

					soundMgr->play("explosion", false);
					it->second.erase(it->second.begin() + i);
					break;

				case SBEntity::STATUS_HERO_DIED:
					_recycleBin.insert(_recycleBin.begin(), it->second[i]);
					soundMgr->play("explosion", false);
					it->second.erase(it->second.begin() + i);
					_heroDied = true;
					break;

				// jei tai buvo ammo tipo objektas ir jis sprogo kazkur atsitrenkes
				case SBEntity::STATUS_SMALL_AMMO_EXPLODED:
					_recycleBin.insert(_recycleBin.begin(), it->second[i]);
					it->second.erase(it->second.begin() + i);
					break;

				case SBEntity::STATUS_POWERUP_PICKED_UP:
					_recycleBin.insert(_recycleBin.begin(), it->second[i]);
					
					if(soundMgr->trackExists("powerup"))
						soundMgr->play("powerup", false);
					_player->upgrade((SBPowerup*)it->second[i]);

					it->second.erase(it->second.begin() + i);
					break;
				case SBEntity::STATUS_SUPER_WEAPON_STOPPED:
					_recycleBin.insert(_recycleBin.begin(), it->second[i]);
					it->second.erase(it->second.begin() + i);
					_player->startRecharging();
					break;
				case SBEntity::STATUS_SUPER_WEAPON_CHARGE_STOPPED:
					_player->makeSecondaryReady();
					break;
			}
		}
	}	

	// atlaisvinam visu objektu, kurie mire, dingo uz ekrano ir t.t., atminti
	unique(_recycleBin.begin(), _recycleBin.end());
	for(int i = 0; i < _recycleBin.size(); i++)
		delete _recycleBin[i];
	_recycleBin.clear();

	if(_heroDied) {
		// jei nebera gyvybiu
		if(!_player->takeLive()) {
			stopGame();
			returnToGui();

			return;
		}
		createHero();
	}
}

/***
 *
 */
void SBSystem::renderEntities()
{
	map<string, deque<SBEntity*>>::iterator it;

	for(it = renderPipe.begin(); it != renderPipe.end(); it++) {
		_dx2d->getSpriteSheet(it->first.c_str())->getSpriteInterface()->Begin(D3DXSPRITE_ALPHABLEND | D3DXSPRITE_SORT_TEXTURE | D3DXSPRITE_SORT_DEPTH_BACKTOFRONT);
		for(unsigned int i = 0; i < it->second.size(); i++) {		
			if(it->second[i])
				it->second[i]->render();
		}
		_dx2d->getSpriteSheet(it->first.c_str())->getSpriteInterface()->End();
	}
}

/***
 *
 */
void SBSystem::renderElements()
{
	gui->showHealthBar(_player->getHealth(), _player->getMaxHealth());
	gui->showLives(_player->getLives());

	char chScore[10];
	itoa(_player->getScore(), chScore, 10);

	RECT pos = {10, 3, 100, 30};

	//_dx2d->getFontStyle("score")->drawText(NULL, toWide(chScore), pos, DT_LEFT);

	if(lg->getVisualText() == NULL)
		lg->setVisualText("");

	_dx2d->getFontStyle("debugFont")->setLabelText("debugMessage", 
		toWide(lg->getVisualText()));
	_dx2d->getFontStyle("debugFont")->drawLabel("debugMessage");
}