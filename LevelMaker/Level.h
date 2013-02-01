#ifndef _LEVEL_H_
#define _LEVEL_H_

#include "funcs.h"
#include <vector>
#include <map>
#include <fstream>
#include <string>

using namespace std;

enum EntityType {enemy, powerup};

struct LevelEntity
{
    Enemy enemyInfo;
    Powerup powerupInfo;
    int minDiff;
    float xspeed, yspeed, speed;
    int col, row;
    EntityType type;
};

typedef map<int, LevelEntity*> LevelEntities;    // vienmatis objektu masyvas
typedef map<int, LevelEntities> Squad;           // squad'as (dvimatis masyvas)
typedef map<int, Squad> Level;                   // visas lygis is squad'u

bool saveToFile(const char* fileName, Level level);
Level readFromFile(const char* fileName);

#endif
