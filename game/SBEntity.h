#ifndef _SBENTITY_H_
#define _SBENTITY_H_

#include "../DXEngine2D/DXSpriteSheet.h"
#include "../DXEngine2D/DXCollision.h"

#include <string>
#include <map>
#include <deque>
#include <vector>
#include <assert.h>
#include "logger.h"

using namespace std;

class SBEntity
{
protected:
	DXCollision<SBEntity>::CollisionData _getCollisionData();
public:

	enum {
		STATUS_STILL_ALIVE,
		STATUS_GONE,
		STATUS_HERO_DIED,
		STATUS_ENEMY_DIED,
		STATUS_SMALL_AMMO_EXPLODED,
		STATUS_BIG_AMMO_EXPLODED,
		STATUS_POWERUP_PICKED_UP,
		STATUS_SUPER_WEAPON_STOPPED,
		STATUS_SUPER_WEAPON_CHARGE_STOPPED
	};

	SBEntity();
	virtual void render() =0;
	virtual int update() =0;
	virtual void addToRenderPipe() =0;
	
	void addToCollisionList();
	void removeFromCollisionList();

	string sprSheetName;
	DXSpriteSheet *spriteSheet;

	DXCollision<SBEntity> *collisions;
	map<string, deque<SBEntity*>> *renderPipe;	

	Logger *lg;

	string collisionId;

	string type;
	string sprite;

	// sprogimo spraitas
	string explSprite;

	// judejimo zingsniai ivairiomis kryptimis
	float speed, xspeed, yspeed;
	
	// gyvybiu kiekis, kuris is pradziu buna lygus maxHealth
	// jei tai yra SBEnemy. Jis yra mazinamas po zalos
	int health;

	// mase skirta apskaiciuoti zalai atsitrenkus i kita laiva
	int weight;

	bool isEnemy;

	// kai mustDie = true, lygis arba squad'as pasibaigia tik mirus objektui
	// aktualu tik kai isEnemy = true :)
	bool mustDie;
};

/*** 
 *	Amunicijos klase
 */
class SBAmmo : public SBEntity
{
public:
	int damage;
	int delay, delayed;
	bool isDouble;
	bool isFinished;	// tik paveldimom SecondaryWeapon klasem

	void render();
	int update();
	void addToRenderPipe();
};

/***
 *	Zaidimo herojaus klase
 */
class SBHero : public SBEntity
{
protected:
	int ammoCount;
public:
	// turimos amunicijos arsenalas
	map<string, map<int, SBAmmo>> ammo;

	struct SecondWeaponData {
		bool charging;			// vyksta krovimas
		bool firing;			// vyksta saudymas
		int recharged;			// kiek pakrauta %
		int used;				// kiek panaudota %
		DWORD chargeStarted;	// kada pradeta krauti
		DWORD fireStarted;		// kada pradeta saudyti
		DWORD chargeTime;		// krovimo trukme
		DWORD fireTime;			// saudymo trukme

		SBAmmo *weapon;			// naudojamas ginklas
	} secWepData;

	DXSpriteSheet *ammoSheet;
	string ammoSound;
	int fireDelayed;

	SBHero();
	bool load();
	void render();
	bool shoot();
	bool shootSecondary();
	int update();
	void addToRenderPipe();
};

/***
 *	Prieso klase, paveldinti kosminio laivo (tiksliau SBHero) savybes
 */
class SBEnemy : public SBHero
{
public:
	// gyvybiu kiekis, jis nemazinamas po zalos, skirtas apskaiciuoti taskam po nuzudymo 
	int maxHealth;
	// ammo seto pavadinimas (visuomet bus vienas)
	string ammoSetName;

	// judejimo tipas
	string moving;

	// jei judejimo tipas x, tai cia saugoma joo y koordinate
	int destY;

	SBHero *myWorstEnemyEver;
	
	SBEnemy();
	void render();
	int update();
	void addToRenderPipe();
};

class SBBoss : public SBEnemy
{
public:
	// 
};

/***
 *	Sprogimo klase
 */
class SBExplosion : public SBEntity
{
public:
	void render();
	int update();
	void addToRenderPipe();
};

/***
 *
 */
class SBPowerup : public SBEntity
{
public:
	// laukas, nurodantis ka turi herojui duoti sis power up
	// galimos reiksmes:
	//		health, ammo (jo pavadinimas buna ammoName)
	//		live
	// jei tai live, hero nieko negauna, tiesiog reikia prideti live
	// jei tai health, hero gauna sio objekto health reiksme
	// jei tai gun, prideda i hero arsenala. 
	// Ginklai yra triju rusiu, jei gaunamas kitokios rusies ginklas
	// tai visas arsenalas trinamas ir pridedamas naujas ginklas
	string upgradeType;
	string ammoName;

	void render();
	int update();
	void addToRenderPipe();
};

/***
 *
 */
class SBLaser : public SBAmmo
{
	int _spriteCount;
	DWORD _started;
	DWORD _currentTime;

	SBLaser *_superParent;
	SBEntity *_source;
public:
	
	DWORD usageTime;

	SBLaser(SBEntity *source);
	void createSprites();
	bool isTime();
	void render();
	int update();
	void addToRenderPipe();
	void removeFromPipes();
};
#endif