#include "SBEntity.h"

/***
 *
 */
SBEntity::SBEntity()
{
	//
	renderPipe = NULL;
	spriteSheet = NULL;
	isEnemy = false;
	mustDie = false;
}


/***
 *
 */
DXCollision<SBEntity>::CollisionData SBEntity::_getCollisionData()
{
	DXCollision<SBEntity>::CollisionData cd;
	D3DVECTOR *pos = spriteSheet->getPosition(sprite.c_str(), 0);
	int w, h;

	spriteSheet->getSize(sprite.c_str(), &w, &h);

	cd.name = "";
	cd.obj = this;
	cd.x = &pos->x;
	cd.y = &pos->y;
	cd.r = (w > h) ? h / 2 : w / 2;
	
	return cd;
}

/***
 *
 */
void SBEntity::addToCollisionList()
{
	if(!collisions)
		return;

	if(isEnemy)
		collisionId = collisions->addToEnemies(_getCollisionData(), true);
	else
		collisionId = collisions->addToFriends(_getCollisionData(), true);
}

/***
 *
 */
void SBEntity::removeFromCollisionList()
{
	if(isEnemy)
		collisions->removeFromEnemies(collisionId.c_str());
	else 
		collisions->removeFromFriends(collisionId.c_str());
}

/////////////////////////////////////////////////////////////////////////////
/// SBAmmo
/////////////////////////////////////////////////////////////////////////////

/***
 *
 */
void SBAmmo::render()
{
	if(spriteSheet)
		spriteSheet->showNextFrame(sprite.c_str());
}

/***
 *
 */
int SBAmmo::update()
{
	D3DXVECTOR3 pos = spriteSheet->getPosition(sprite.c_str());
	SBEntity *obj;

	// jei i kazka atsitrenke..
	if( (obj = collisions->getCollided(collisionId)) ) {

		if(obj->type != "ammo") {
			removeFromCollisionList();

			// sukuriam sprogima
			SBExplosion *explosion = new SBExplosion;
			explosion->spriteSheet = spriteSheet;

			if(explosion->spriteSheet->createCopy(explSprite.c_str(), (sprite+explSprite).c_str()) ) {
				explosion->sprite = sprite+explSprite;
				explosion->sprSheetName = sprSheetName;

				explosion->spriteSheet->setCenterAuto(explosion->sprite.c_str());
				explosion->spriteSheet->setPosition(explosion->sprite.c_str(), pos.x, pos.y);
 
				explosion->renderPipe = renderPipe;
				explosion->addToRenderPipe();
			} 
			else delete explosion;
		
			spriteSheet->removeSprite(sprite.c_str());
			return STATUS_SMALL_AMMO_EXPLODED;
		}
	}

	if(pos.y <= - 5 || pos.y >= 650 || pos.x <= -50 || pos.x >= 850) {
		spriteSheet->removeSprite(sprite.c_str());
		removeFromCollisionList();
		return STATUS_GONE;
	}

	spriteSheet->move(sprite.c_str(), xspeed, yspeed);
	return STATUS_STILL_ALIVE;
}

/***
 *
 */
void SBAmmo::addToRenderPipe()
{
	if(!renderPipe)
		return;

	(*renderPipe)[sprSheetName].insert((*renderPipe)[sprSheetName].end(), 
		(dynamic_cast<SBEntity*>(this)) );
}

/////////////////////////////////////////////////////////////////////////////
/// SBHero
/////////////////////////////////////////////////////////////////////////////

/***
 *
 */
SBHero::SBHero()
{
	ammoCount = 0;
	fireDelayed = 0;
	isEnemy = false;

	secWepData.chargeStarted = 0;
	secWepData.chargeTime = 6000;
	secWepData.fireStarted = 0;
	secWepData.fireTime = 5000;
	secWepData.recharged = 100;		// 100% pakrauta
	secWepData.used = 0;
	secWepData.firing = false;
	secWepData.charging = false;
	secWepData.weapon = NULL;
}

/***
 *
 */
void SBHero::render()
{
	// ..
	if(spriteSheet)
		spriteSheet->showNextFrame(sprite.c_str());
}

/***
 *
 */
bool SBHero::shoot()
{
	map<string, map<int, SBAmmo>>::iterator gun = ammo.begin();
	map<int, SBAmmo>::iterator it;
	bool fired = false;

	//for(it = ammo.begin(); it != ammo.end(); it++) {
	for(it = gun->second.begin(); it != gun->second.end(); it++) {
		if(it->second.delayed < it->second.delay)
			continue;

		it->second.delayed = 1;
		fired = true;

		SBAmmo *am = new SBAmmo(it->second);	

		unsigned int ammoNo = ++ammoCount;
		// EXPERIMENT
		//unsigned long int ammoNo = GetTickCount(); 
		char chAmmoNo[30];

		itoa(ammoNo, chAmmoNo, 10);
		string strAmmoNo = string("_") + string(chAmmoNo);

		// sukuriama nauja "serija" :)
		if(am->spriteSheet->createCopy(am->sprite.c_str(), (sprite + strAmmoNo).c_str() ) ) {

			am->isEnemy = isEnemy;
			am->sprite = (sprite + strAmmoNo);
			am->spriteSheet->setCenterAuto(am->sprite.c_str());

			am->collisions = collisions;
			am->addToCollisionList();

			D3DXVECTOR3 pos = spriteSheet->getPosition(sprite.c_str());
		
			am->spriteSheet->setPosition(am->sprite.c_str(), pos.x, pos.y);

			am->renderPipe = renderPipe;
			am->addToRenderPipe();
		}
	}

	return fired;
}

/***
 *
 */
bool SBHero::shootSecondary()
{
	if(secWepData.used == 0 && secWepData.recharged == 100) {
		secWepData.firing = true;		
		secWepData.fireStarted = GetTickCount();
		secWepData.recharged = 0;
		secWepData.charging = false;
		secWepData.used = 1;

		lg->appendVisualText("pradedame saudyti\n");	
		
		if(secWepData.weapon)
			delete secWepData.weapon;

		SBLaser *laser = new SBLaser(this);
		laser->lg = lg;
		
		map<string, map<int, SBAmmo>>::iterator gun = ammo.begin();
		laser->sprite = "secondary1";
		laser->spriteSheet = ammoSheet;
		laser->sprSheetName = gun->second[1].sprSheetName;
		laser->collisions = collisions;
		laser->renderPipe = renderPipe;
		laser->damage = 100;
		laser->type = "ammo";
		laser->isEnemy = false;
		laser->isFinished = false;

		secWepData.weapon = laser;
		
		D3DXVECTOR3 pos = spriteSheet->getPosition(sprite.c_str());
		laser->spriteSheet->setPosition(laser->sprite.c_str(), pos.x, pos.y);
		
		laser->createSprites();
	}

	return true;
}

/***
 *
 */
int SBHero::update()
{
	SBEntity *obj;
	if((obj = collisions->getCollided(collisionId))) {
		// ..
		if(obj->type == "ammo") {
			health -= ((SBAmmo*)obj)->damage;
		}
		else if(obj->type == "enemy") {
			health -= ((SBEnemy*)obj)->weight;
		}
	}

	if(health <= 0) {
		D3DXVECTOR3 pos = spriteSheet->getPosition(sprite.c_str());

		// sukuriam sprogima
		SBExplosion *explosion = new SBExplosion;
		explosion->spriteSheet = spriteSheet;

		if(explosion->spriteSheet->createCopy(explSprite.c_str(), (sprite+explSprite).c_str()) ) {
			explosion->sprite = sprite+explSprite;
			explosion->sprSheetName = sprSheetName;

			explosion->spriteSheet->setCenterAuto(explosion->sprite.c_str());
			explosion->spriteSheet->setPosition(explosion->sprite.c_str(), pos.x, pos.y);

			explosion->renderPipe = renderPipe;
			explosion->addToRenderPipe();

		}		

		removeFromCollisionList();

		return STATUS_HERO_DIED;
	}

	map<string, map<int, SBAmmo>>::iterator gun = ammo.begin();
	map<int, SBAmmo>::iterator it;
	for(it = gun->second.begin(); it != gun->second.end(); it++) {
		it->second.delayed++;
	}

	// apdorojame antrinio ginklo saudymo situacijas
	if(secWepData.weapon) {
		
		if(GetTickCount() - secWepData.fireStarted < secWepData.fireTime) {
			// jei vis dar nesibaige laikas saudymui
			secWepData.used =  ((GetTickCount() - secWepData.fireStarted) / secWepData.fireTime) * 100;
			lg->setVisualText("dar vis saudome..");
		}
		else if(secWepData.recharged <= 0 && !secWepData.charging) {
			// jai saudymo laikas baiges, bet dar ir nepradetas charginimas
			secWepData.chargeStarted = GetTickCount();
			secWepData.weapon->isFinished = true;
			secWepData.charging = true; 
			lg->setVisualText("PSIO!!!");
		}
		else if(secWepData.recharged < 100) {
			// jei nebaigtas recharginimas
			secWepData.recharged =  ((GetTickCount() - secWepData.chargeStarted) / secWepData.chargeTime) * 100;
			lg->setVisualText("Kraunasi...");
		}
		else if(secWepData.recharged >= 100) {
			secWepData.recharged = 100;
			secWepData.charging = false;			
			lg->setVisualText("Krovimas baiges!");
		}
		/*
		// jei baigesi antrinio ginklo saudymo laikas/jei negalima sauti
		if(GetTickCount() - secWepData.fireStarted >= secWepData.fireTime) {
			if(!secWepData.weapon->isFinished) {
				secWepData.weapon->isFinished = true;
			}	
		
			// jei baigesi krovimas
			else if(!secWepData.chargeStarted && GetTickCount() - secWepData.chargeStarted >= secWepData.chargeTime) {
				return STATUS_SUPER_WEAPON_CHARGE_STOPPED;
			}
		}
		*/
	}

	return STATUS_STILL_ALIVE;
}

/***
 *
 */
void SBHero::addToRenderPipe()
{
	if(!renderPipe)
		return;

	(*renderPipe)[sprSheetName].insert((*renderPipe)[sprSheetName].end(), 
		(dynamic_cast<SBEntity*>(this)) );
}


/////////////////////////////////////////////////////////////////////////////
/// SBEnemy
/////////////////////////////////////////////////////////////////////////////

/***
 *
 */
SBEnemy::SBEnemy()
{
	isEnemy = true;
	destY = -1;
}

/***
 *
 */
int SBEnemy::update()
{
	SBEntity *obj;
	if((obj = collisions->getCollided(collisionId))) {
		// ..

		if(obj->type == "ammo") {
			health -= ((SBAmmo*)obj)->damage;
		}
		else if(obj->type == "hero") {
			health -= (obj->weight * 2);
		}
	}

	if(health <= 0) {
		D3DXVECTOR3 pos = spriteSheet->getPosition(sprite.c_str());

		// sukuriam sprogima
		SBExplosion *explosion = new SBExplosion;
		explosion->spriteSheet = spriteSheet;

		if(explosion->spriteSheet->createCopy(explSprite.c_str(), (sprite+explSprite).c_str()) ) {
			explosion->sprite = sprite+explSprite;
			explosion->sprSheetName = sprSheetName;

			explosion->spriteSheet->setCenterAuto(explosion->sprite.c_str());
			explosion->spriteSheet->setPosition(explosion->sprite.c_str(), pos.x, pos.y);

			explosion->renderPipe = renderPipe;
			explosion->addToRenderPipe();
		}		

		removeFromCollisionList();

		return STATUS_ENEMY_DIED;
	}

	if(spriteSheet->getPosition(sprite.c_str()).y > 0)
		shoot();

	// judejimo ypatumai :))
	D3DXVECTOR3 pos = spriteSheet->getPosition(sprite.c_str());
	
	if(destY < 0 && moving[0] == 'x' && moving[1] != 'z')
		destY = atoi(moving.substr(1).c_str());

	if(spriteSheet->getPosition(sprite.c_str()).y >= 0) {
		if(moving[0] == 'x' && moving[1] == 'y')
			// judejimas istrizai
			spriteSheet->move(sprite.c_str(), xspeed, yspeed);

		if(moving[1] == 'c' && (moving[0] == 'l' || moving[0] == 'r')) {
			// judejimas i apatinius kampus
			double vx = (double)800 - (double)spriteSheet->getPosition(sprite.c_str()).x;
			if(moving[0] == 'l')
				vx = -(double)spriteSheet->getPosition(sprite.c_str()).x;

			double vy = (double)(620 - spriteSheet->getPosition(sprite.c_str()).y);
			spriteSheet->moveTo(sprite.c_str(), vx, vy, speed);
		}
		else if(moving[0] == 'k') {
			// kamikadze :)))))
			double vx = (double)(myWorstEnemyEver->spriteSheet->getPosition(myWorstEnemyEver->sprite.c_str()).x - 
				spriteSheet->getPosition(sprite.c_str()).x);
			double vy = (double)(myWorstEnemyEver->spriteSheet->getPosition(myWorstEnemyEver->sprite.c_str()).y - 
				spriteSheet->getPosition(sprite.c_str()).y);
			spriteSheet->moveTo(sprite.c_str(), vx, vy, speed);
		}
		else if(moving[0] == 'x' && pos.y >= destY) {

			if(pos.x <= 0)
				xspeed = -xspeed;
			else if(pos.x >= 800)
				xspeed = -xspeed;

			spriteSheet->move(sprite.c_str(), xspeed);
		}
		else spriteSheet->move(sprite.c_str(), 0, speed);
	}
	else spriteSheet->move(sprite.c_str(), 0, speed);
	
	map<string, map<int, SBAmmo>>::iterator gun = ammo.begin();
	map<int, SBAmmo>::iterator it;
	for(it = gun->second.begin(); it != gun->second.end(); it++) {
		it->second.delayed++;
	}

	return STATUS_STILL_ALIVE;
}

/***
 *
 */
void SBEnemy::addToRenderPipe()
{
	if(!renderPipe)
		return;

	// random budu nustatoma kokia kryptimi is pradziu judes
	// kai judejimo tipas yra x
	if(moving[0] == 'x' && moving[1] != 'y') {
		srand(time(NULL));
		int val = rand() % 10;
		if(val % 2 == 0)
			xspeed = -xspeed;
	}

	(*renderPipe)[sprSheetName].insert((*renderPipe)[sprSheetName].end(), 
		(dynamic_cast<SBEntity*>(this)) );
}

/***
 *
 */
void SBEnemy::render()
{
	spriteSheet->showNextFrame(sprite.c_str());
}

////////////////////////////////////////////////////////////////////////////
/// SBExplosion
////////////////////////////////////////////////////////////////////////////

/***
 *
 */
void SBExplosion::render()
{
	spriteSheet->showNextFrame(sprite.c_str());
}

/***
 *
 */
int SBExplosion::update()
{
	if(spriteSheet->getCurrentFrame(sprite.c_str()) == spriteSheet->getFrameCount(sprite.c_str()) - 1) {
		spriteSheet->removeSprite(sprite.c_str());
		return STATUS_GONE;
	}
	return STATUS_STILL_ALIVE;
}

/***
 *
 */
void SBExplosion::addToRenderPipe()
{
	if(!renderPipe)
		return;

	(*renderPipe)[sprSheetName].insert((*renderPipe)[sprSheetName].end(), 
		(dynamic_cast<SBEntity*>(this)) );
}

////////////////////////////////////////////////////////////////////////////
/// SBPowerup
////////////////////////////////////////////////////////////////////////////

/***
 *
 */
void SBPowerup::render()
{
	spriteSheet->showNextFrame(sprite.c_str());
}

/***
 *
 */
int SBPowerup::update()
{
	SBEntity *obj;
	if((obj = collisions->getCollided(collisionId))) {
		// ..

		if(obj->type == "hero") {
			removeFromCollisionList();
			return STATUS_POWERUP_PICKED_UP;
		}
	}

	spriteSheet->move(sprite.c_str(), 0, speed);
	return STATUS_STILL_ALIVE;
}

/***
 *
 */
void SBPowerup::addToRenderPipe()
{
	if(!renderPipe)
		return;

	(*renderPipe)[sprSheetName].insert((*renderPipe)[sprSheetName].end(), 
		(dynamic_cast<SBEntity*>(this)) );
}

////////////////////////////////////////////////////////////////////////////
/// SBLaser
////////////////////////////////////////////////////////////////////////////

SBLaser::SBLaser(SBEntity *source)
{
	_source = source;
	// ar saudymas jau baigtas? Pagal si lauka objektas grazina saves naikinimo rezultata
	isFinished = false;
}

/***
 *
 */
void SBLaser::createSprites()
{
	int spWidth, spHeight;
	spriteSheet->getSize(sprite.c_str(), &spWidth, &spHeight);
	D3DXVECTOR3 pos = spriteSheet->getPosition(sprite.c_str());

	_spriteCount = pos.y / spHeight + 1;

	char sprNo[6];
	// sukuriami spraitai, kurie dar nebuvo sukurti
	for(int i = 0; i < _spriteCount; i++) {
		itoa(i, sprNo, 10);
		if(!spriteSheet->spriteExists( (sprite+sprNo).c_str() )) {
			spriteSheet->createCopy(sprite.c_str(), (sprite+sprNo).c_str() );			
			spriteSheet->setPosition( (sprite+sprNo).c_str(), pos.x, pos.y - (spHeight * i));
		}
	}

	for(int i = 0; i < _spriteCount; i++) {
		itoa(i, sprNo, 10);
		SBLaser *am = new SBLaser(_source);
		am->_superParent = this;
		am->xspeed = 0;
		am->yspeed = 0;
		am->health = 1;
		am->damage = damage;
		am->usageTime = 3000;

		am->spriteSheet = spriteSheet;
		am->sprSheetName = sprSheetName;
		am->sprite = sprite+sprNo;
		am->renderPipe = renderPipe;
		am->addToRenderPipe();

		am->collisions = collisions;
		am->addToCollisionList();
	}

	_started = GetTickCount();
	_currentTime = _started;
}

/***
 *
 */
void SBLaser::render()
{
	spriteSheet->showNextFrame( sprite.c_str() );
}	

/***
 *
 */
int SBLaser::update()
{
	if(_superParent->isFinished) {
		removeFromCollisionList();
		return STATUS_SUPER_WEAPON_STOPPED;
	}

	_currentTime = GetTickCount();

	D3DXVECTOR3 pos = _source->spriteSheet->getPosition(_source->sprite.c_str());
/*	if(_superParent == this) {
		ymov = _source->spriteSheet->getPosition(_source->sprite.c_str()).y - spriteSheet->getPosition(sprite.c_str()).y;
	}
	else {
		y = _superParent->spriteSheet->getPosition(sprite.c_str()).y;
		//
	}
	*/
	spriteSheet->setPosition( sprite.c_str(), pos.x, spriteSheet->getPosition(sprite.c_str()).y );

	return STATUS_STILL_ALIVE;
}

/***
 *
 */
void SBLaser::addToRenderPipe()
{
	if(!renderPipe)
		return;

	(*renderPipe)[sprSheetName].insert((*renderPipe)[sprSheetName].end(), 
		(dynamic_cast<SBEntity*>(this)) );
}