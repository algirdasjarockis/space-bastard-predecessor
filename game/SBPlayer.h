#ifndef _SBPLAYER_H_
#define _SBPLAYER_H_

#include "SBEntity.h"

class SBPlayer
{
	SBHero *_hero;

	unsigned long _score;
	unsigned short _lives;
	unsigned short _maxHealth;
	bool _isRecharging;
	bool _isSecShooting;
public:
	map<string, map<int, SBAmmo>> *ammo;

	SBPlayer(unsigned short maxHealth);
	void zeroScore();
	void setHero(SBHero *hero);
	void setLives(unsigned short count);
	bool takeLive(unsigned short count = 1);
	void setMaxHealth(unsigned short maxHealth);
	void addScore(unsigned long points);

	unsigned long getScore();
	unsigned short getHealth();
	unsigned short getMaxHealth();
	unsigned short getLives();

	void upgrade(SBPowerup *pwup);

	bool isRecharging();
	bool isSecShooting();
	void updateSecShooting();
	void startRecharging();
	void makeSecondaryReady();
	void recharge();
};

#endif // _SBPLAYER_H_