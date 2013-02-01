#ifndef _SBSYSTEM_H_
#define _SBSYSTEM_H_

#include "../DXEngine2D/DXEngine2D.h"
#include "../DXEngine2D/DXInput.h"
#include "../DXEngine2D/DXSoundManager.h"
#include "../DXEngine2D/DXCollision.h"
#include "../TinyXml/tinyxml.h"

#include "logger.h"
#include "Properties.h"
#include "SBEntity.h"
#include "GuiSystem.h"
#include "SBLevel.h"
#include "SBPlayer.h"

#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <vector>
#include <deque>
#include <algorithm>
#include <time.h>

using namespace std;

class SBSystem
{
	HWND _hwnd;
	HINSTANCE _hinst;

	int _width, _height;
	bool _fullscreen;
	bool _configLoaded;

	DXEngine2D *_dx2d;
	DXInput *_input;	
	DXCollision<SBEntity> *collisions;

	long _xMouseMov, _yMouseMov, _zMouseMov;
	int _mouseState;
	int _buttonState;

	bool _inGame;
	bool _inGui;
	bool _inLoadingProgress;
	bool _loadStarted;
	bool _loadFinished;
	bool _ammoDataLoaded;

	bool _heroDied;
	SBPlayer *_player;

	Logger *lg;
	Properties *props;

	map<string, deque<SBEntity*>> renderPipe,	// zaidimo grafiniai objektai
								  scene;		// scenos grafiniai objektai

	// masyvas, kuriame saugomi ginklu prototipai
	map<string, map<int, SBAmmo>> ammo;

	// masyvas rodykliu, kurias reiktu trinti :)
	vector<SBEntity*> _recycleBin;

	string _guiScreen;
	string _guiMusic;
public:

	enum {
		CLICKED_NONE,
		CLICKED_START,
		CLICKED_RESUME,
		CLICKED_FULLSCREEN,
		CLICKED_STOP,
		CLICKED_EXIT
	};

	DXSoundManager *soundMgr;
	GuiSystem *gui;
	SBLevel *level;

	SBSystem();
	void release();
	void setWindowHandle(HWND hwnd);
	void setInstance(HINSTANCE hinst);
	bool loadConfig();
	bool init();

	int getWidth();
	int getHeight();
	bool isFullScreen();

	bool startRendering();
	bool stopRendering();

	bool initGui();
	bool renderGui();

	bool inGame();
	bool inGui();

	void listenForInput();
	void updateCursor();
	int buttonClicked();
	int mouseClicked();
	bool keyPressed(long dikCode);

	void playButtonSound(const char *btnName);

	void startLoading();
	bool loadSprites();
	bool loadAmmoData();
	bool inLoadingProgress();
	void showLoadingProgress();
	void startGame();
	void stopGame();
	void returnToGui();
	void returnToGame();

	void createHero();
	void updateHero();
	void updateEntities();

	void renderEntities();
	void renderElements();

	void loadScene();
	void renderScene();
};

#endif