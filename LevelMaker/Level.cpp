#include "Level.h"
#include <stdio.h>

bool saveToFile(const char* fileName, Level level)
{
    ofstream fr(fileName);
    if(fr.fail())
        return false;

    string xml = "<?xml version=\"1.0\" encoding=\"utf-8\"?>\n<level background=\"bg1.jpg\" sound=\"metal3.wav\">\n\t<squads difficulty=\"1\">\n";
    fr << xml;

    for(Level::iterator it = level.begin(); it != level.end(); ++it) {
        fr << "\t\t<squad>\n";
        for(int i = 0; i < 14; i++) {
            for(int j = 0; j < 10; j++) {
                if(it->second[i][j]) {
                    if(it->second[i][j]->type == enemy) {
                        // enemy
                        char str[255];
                        sprintf(str, "\t\t\t<enemy name=\"%s\" row=\"%d\" col=\"%d\" health=\"%d\" moving=\"%s\" yspeed=\"%0.2f\" xspeed=\"%2.2f\" weight=\"%d\"/>\n",
                                    it->second[i][j]->enemyInfo.name.c_str(),
                                    10 - j, i,
                                    it->second[i][j]->enemyInfo.health,
                                    it->second[i][j]->enemyInfo.moving.c_str(),
                                    it->second[i][j]->yspeed, it->second[i][j]->xspeed,
                                    it->second[i][j]->enemyInfo.weight);
                        fr << str;
                    }
                    else if(it->second[i][j]->type == powerup) {
                        // powerup
                        char str[255];
                        sprintf(str, "\t\t\t<pickup name=\"%s\" row=\"%d\" col=\"%d\" speed=\"%2.2f\"/>\n",
                                    it->second[i][j]->powerupInfo.name.c_str(),
                                    10 - j, i,
                                    it->second[i][j]->powerupInfo.speed, it->second[i][j]->powerupInfo.value);
                        fr << str;
                    }
                }
            }
        }
        fr << "\t\t</squad>\n";
    }

    fr << "\t</squads>\n</level>\n";

    return true;
}


Level readFromFile(const char* fileName)
{
	unsigned int squadNo = 0;
    vector<string>lvlEntitiesTags;
    lvlEntitiesTags.push_back("enemy");
    lvlEntitiesTags.push_back("pickup");
    string noData = "<N/A>";

    Level level;

	TiXmlDocument xmlDoc(fileName);

	if(!xmlDoc.LoadFile()) {
		xmlDoc.Clear();
		return level;
	}

	TiXmlElement *txeLevel = xmlDoc.FirstChildElement("level");
	if(!txeLevel) {
		xmlDoc.Clear();
		return level;
	}

	TiXmlElement *txeSquads = txeLevel->FirstChildElement("squads");
	if(txeSquads) {
		TiXmlElement *squad = txeSquads->FirstChildElement("squad");
		while(squad) {
			for(int i = 0; i < lvlEntitiesTags.size(); i++) {
				TiXmlElement *ent = squad->FirstChildElement(lvlEntitiesTags[i].c_str());
				while(ent) {
					// ent elementu apdorojimas
                    LevelEntity le;
					le.xspeed = le.yspeed = 1;
					le.minDiff = 1;
					if(lvlEntitiesTags[i] == "enemy") {
						le.type = enemy;
                        le.enemyInfo.health = 100;
                        le.enemyInfo.moving = "y";
                        le.enemyInfo.ammo = noData;
                        le.enemyInfo.weight = 200;
                    }
					else {
                        le.type = powerup;
                        le.powerupInfo.sprite = noData;
                        le.powerupInfo.type = noData;
                        le.powerupInfo.value = noData;
                        le.powerupInfo.speed = 0;
                    }

					int coordRow, coordCol = 0;
					if(ent->Attribute("row")) {
						// todo: apskaiciuoti y
						coordRow = atoi(ent->Attribute("row"));
					}
					else ;

					if(ent->Attribute("col")) {
						// todo: apskaiciuoto x
						coordCol = atoi(ent->Attribute("col"));
					}
					else ;

                    if(ent->Attribute("name"))
                        if(le.type == enemy)
                            le.enemyInfo.name = ent->Attribute("name");
                        else le.powerupInfo.name = ent->Attribute("name");

					if(ent->Attribute("speed"))
                        if(le.type == powerup)
						    le.powerupInfo.speed = atof(ent->Attribute("speed"));

					if(ent->Attribute("yspeed"))
						le.yspeed = atof(ent->Attribute("yspeed"));
					if(ent->Attribute("xspeed"))
						le.xspeed = atof(ent->Attribute("xspeed"));

					if(ent->Attribute("name")) {
                        if(le.type == enemy)
    						le.enemyInfo.name = string(ent->Attribute("name"));
                        else le.powerupInfo.name = string(ent->Attribute("name"));
                    }

					if(ent->Attribute("moving"))
						if(strlen(ent->Attribute("moving")) < 5)
                            le.enemyInfo.moving = ent->Attribute("moving");

					if(ent->Attribute("mindifficulty"))
						le.minDiff = atoi(ent->Attribute("mindifficulty"));

                    if(ent->Attribute("weight"))
                        le.enemyInfo.weight = atoi(ent->Attribute("weight"));

                    if(ent->Attribute("health"))
                        le.enemyInfo.health = atoi(ent->Attribute("health"));

                    level[squadNo][coordCol][10-coordRow] = new LevelEntity(le);

					ent = ent->NextSiblingElement(lvlEntitiesTags[i].c_str());
				}
			}

			squad = squad->NextSiblingElement("squad");
			squadNo++;
		} // while(squad)
	}
//    else return false;
    return level;
}
