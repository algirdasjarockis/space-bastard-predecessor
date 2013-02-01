#include "funcs.h"

Enemies * loadEnemies(const char* fileName)
{
    Enemies * enems = new Enemies;
	TiXmlDocument xmlDoc( fileName );

	if(!xmlDoc.LoadFile()) {
		xmlDoc.Clear();
        return NULL;
	}

	TiXmlElement *txeEnemies = xmlDoc.FirstChildElement("enemies");
	if(!txeEnemies) {
		xmlDoc.Clear();
		return NULL;
	}

	TiXmlElement *txeEnemy = txeEnemies->FirstChildElement("enemy");
	while(txeEnemy) {
        Enemy enem;

        enem.name = txeEnemy->Attribute("name");
		enem.health = atoi(txeEnemy->FirstChildElement("health")->FirstChild()->Value());
		enem.weight = atoi(txeEnemy->FirstChildElement("weight")->FirstChild()->Value());
		enem.speed = atof(txeEnemy->FirstChildElement("speed")->FirstChild()->Value());

		enem.moving = string(txeEnemy->FirstChildElement("moving")->FirstChild()->Value());
		enem.sprite = txeEnemy->Attribute("sprite");
		enem.ammo = string(txeEnemy->FirstChildElement("ammo")->FirstChild()->Value());

        enems->push_back(enem);

        txeEnemy = txeEnemy->NextSiblingElement("enemy");
    }

    return enems;
}

Powerups * loadPowerups(const char* fileName)
{
    Powerups * pwups = new Powerups;
	TiXmlDocument xmlDoc( fileName );

	if(!xmlDoc.LoadFile()) {
		xmlDoc.Clear();
        return NULL;
	}

	TiXmlElement *txePowerups = xmlDoc.FirstChildElement("powerups");
	if(!txePowerups) {
		xmlDoc.Clear();
		return NULL;
	}

	TiXmlElement *txePwup = txePowerups->FirstChildElement("powerup");
	while(txePwup) {
        Powerup pwup;

        pwup.name = txePwup->Attribute("name");
        pwup.type = txePwup->Attribute("type");
		pwup.value = txePwup->FirstChildElement("value")->FirstChild()->Value();
		pwup.speed = atof(txePwup->FirstChildElement("speed")->FirstChild()->Value());

		pwup.sprite = txePwup->Attribute("sprite");

        pwups->push_back(pwup);

        txePwup = txePwup->NextSiblingElement("powerup");
    }

    return pwups;
}


