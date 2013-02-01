#include "SBPlayer.h"

/***
 *
 */
SBPlayer::SBPlayer(unsigned short maxHealth = 500)
{
	setMaxHealth(maxHealth);
	zeroScore();
	_isRecharging = false;
	_isSecShooting = false;
}

/***
 *
 */
void SBPlayer::zeroScore()
{
	_score = 0;
}

/***
 *
 */
void SBPlayer::setHero(SBHero *hero)
{
	_hero = hero;
}

/***
 *
 */
void SBPlayer::setLives(unsigned short count)
{
	_lives = count;
}

/***
 *
 */
bool SBPlayer::takeLive(unsigned short count)
{
	if(_lives - count <= 0) {
		_lives = 0;
		return false;
	}
	
	_lives--;
	return true;
}

/***
 *
 */
void SBPlayer::setMaxHealth(unsigned short maxHealth)
{
	_maxHealth = maxHealth;
}

/***
 *
 */
void SBPlayer::addScore(unsigned long points)
{
	_score += points;
}

/***
 *
 */
unsigned long SBPlayer::getScore()
{
	return _score;
}

/***
 *
 */
unsigned short SBPlayer::getHealth()
{
	return _hero->health;
}

/***
 *
 */
unsigned short SBPlayer::getMaxHealth()
{
	return _maxHealth;
}

/***
 *
 */
unsigned short SBPlayer::getLives()
{
	return _lives;
}

/***
 *
 */
void SBPlayer::upgrade(SBPowerup *pwup)
{
	if(pwup->upgradeType != "live") {
		if(pwup->upgradeType == "ammo") {
			// jei tokia rusis yra
			if(_hero->ammo.find(pwup->ammoName) != _hero->ammo.end()) {
				int ammoNo = _hero->ammo[pwup->ammoName].size()+1;

				if(ammoNo <= (*ammo)[pwup->ammoName].size()) {
					_hero->ammo[pwup->ammoName][ammoNo] = 
						(*ammo)[pwup->ammoName][ammoNo];

					if((*ammo)[pwup->ammoName][ammoNo].isDouble) {
						_hero->ammo[pwup->ammoName][ammoNo+1] = 
							(*ammo)[pwup->ammoName][ammoNo+1];
					}
				}
			}
		}
		else if(pwup->upgradeType == "health") {
			int addHealth = _maxHealth * ((float)pwup->health/100);
			_hero->health += addHealth;
			if(_hero->health > _maxHealth)
				_hero->health = _maxHealth;
		}
		
	}
	else _lives++;
}

/***
 *
 */
bool SBPlayer::isRecharging()
{ 
	return _isRecharging;
}

/***
 *
 */
bool SBPlayer::isSecShooting()
{
	if(_hero->secWepData.recharged == 100 && _hero->secWepData.used > 0 &&
		_hero->secWepData.used < 100)
		_isSecShooting = true;

	return _isSecShooting;
}

/**
 *
 */
void SBPlayer::updateSecShooting()
{
	//_hero->secWepData.used++;
}

/***
 *
 */
void SBPlayer::startRecharging()
{
	if(!_isRecharging) {
		_hero->secWepData.chargeStarted = GetTickCount();
		_hero->secWepData.recharged = 0;

		_isRecharging = true;
		_isSecShooting = false;
	}
}

/***
 *
 */
void SBPlayer::makeSecondaryReady()
{
	_hero->secWepData.used = 0;
	_hero->secWepData.fireStarted = GetTickCount();
	_hero->secWepData.chargeStarted = 0;

	_isRecharging = false;
	_isSecShooting = true;
}

/*** 
 *
 */
void SBPlayer::recharge()
{/*
	if(_hero->secWepData.recharged <= 100) {
		_hero->secWepData.recharged = (GetTickCount() - _hero->secWepData.chargeStarted 
			/ _hero->secWepData.chargeTime) * 100;
		
		// TODO: ideti recharginimo juosta
	}
	else _isRecharging = false;*/
}