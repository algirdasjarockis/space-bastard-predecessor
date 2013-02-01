#ifndef _SBLEVEL_H_
#define _SBLEVEL_H_

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

class SBLevel
{
	int _level;
	bool _levelLoaded;
	bool _levelFinished;

	DWORD _time, _realTime;

	DXEngine2D *_dx2d;
	DXSoundManager *_soundMgr;
	Logger *_lg;
	Properties *_props;

	/**
	 *	Duomenu struktura is lygio failo
	 */
	struct LevelEntity
	{
		// objekto vardas
		string name;
		// koordinates
		int x, y;
		// abieju asiu greiciai (jei judejimas tik viena asimi, tai abu turi buti lygus)
		float xspeed, yspeed;
		// judejimo tipas
		string moving;
		// gyvybiu kiekis
		int health;
		// svoris
		int weight;
		// lygio minimumas, pagal kuri nustatoma kokiame sunkumo lygyje rodyti
		unsigned int minDifficulty;
		
		enum LevelEntityType {
			enemy = 0,
			powerup
		} type;
	};

	// lygio failo duomenys masyve
	map<int, vector<LevelEntity> > _lvlPipe;

	// kelias iki background'o failo
	string _bgFile;

	// priesu prototipai
	map<string, SBEnemy*> enemies;

	// powerup'su prototipai
	map<string, SBPowerup*> powerups;

	unsigned int _currentSquad;
	unsigned int _squadCount;

	bool _loadEnemyData();
	bool _loadPowerupData();
	void _showSquad(unsigned int squadNo);
	
	SBEntity * _topSquadEntity,		// auksciausiai esantis objektas
			 * _boss;				// bosas arba minibosas
public:

	SBHero *hero;

	// masyvas, kuriame saugomi ginklu prototipai priesams
	map<string, map<int, SBAmmo>> *ammo;

	map<string, deque<SBEntity*>> *renderPipe;

	DXCollision<SBEntity> *collisions;

	SBLevel(DXEngine2D *dx2d, DXSoundManager *sndMgr, Logger *lg, Properties *prps);
	bool load(int level);
	bool unLoad();
	bool isLoaded();
	const char * getBackgroundFile();

	void run();
	void startTiming();
	DWORD getTime();
	bool levelFinished();
};

#endif