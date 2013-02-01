#ifndef _FUNCS_H_
#define _FUNCS_H_

#include "../TinyXml/tinyxml.h"
#include <string>
#include <vector>

using namespace std;
/*
    Enemy basic struktura
*/
struct Enemy
{
    string name;
    string sprite;
    unsigned int health;
    unsigned int weight;
    float speed;
    string ammo;
    string moving;
};

/*
    Powerup basic struktura
*/
struct Powerup
{
    string name;
    string sprite;
    string type;
    string value;
    float speed;
};

typedef vector<Enemy> Enemies;
typedef vector<Powerup> Powerups;

Enemies * loadEnemies(const char* fileName);
Powerups * loadPowerups(const char* fileName);

#endif