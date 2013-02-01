#ifndef _DXCOLLISION_H_
#define _DXCOLLISION_H_

#include <math.h>
#include <vector>
#include <map>
#include <string>

using namespace std;

template <class T>
class DXCollision
{
	unsigned long int _id;

	string _generateName();

	map<string, T*> _collided;
public:
	struct CollisionData 
	{
		string name;
		float *x, *y; 
		float r;
		T *obj;
	};
	DXCollision();

	void resetNamingId();

	string addToFriends(CollisionData cd, bool generateName = false);
	string addToEnemies(CollisionData cd, bool generateName = false);
	string addToNeutral(CollisionData cd, bool generateName = false);

	void removeFromFriends(const char *name);
	void removeFromEnemies(const char *name);
	void removeFromNeutral(const char *name);
	void removeFriends();
	void removeEnemies();
	void removeNeutral();

	//vector<string> getOponentCollisions();
	void getOponentCollisions();
	vector<string> getNeutralCollisions();
//private:
	vector<CollisionData> friends, enemies, neutral;

	T * getCollided(string name);
};

/***
 *
 */
template <class T>
DXCollision<T>::DXCollision()
{
	_id = 0;
}

/***
 *
 */
template <class T>
string DXCollision<T>::_generateName()
{
	char chId[10];
	itoa(_id++, chId,10);
	return string("obj") + string(chId);
}

/***
 *
 */
template <class T>
void DXCollision<T>::resetNamingId()
{
	_id = 0;
}

/***
 *
 */
template <class T>
string DXCollision<T>::addToFriends(CollisionData cd, bool generateName)
{
	if(generateName)
		cd.name = _generateName();

	friends.insert(friends.begin(), cd);
	return cd.name;
}

/***
 *
 */
template <class T>
string DXCollision<T>::addToEnemies(CollisionData cd, bool generateName)
{
	if(generateName)
		cd.name = _generateName();

	enemies.insert(enemies.begin(), cd);
	return cd.name;
}

/***
 *
 */
template <class T>
string DXCollision<T>::addToNeutral(CollisionData cd, bool generateName)
{
	if(generateName)
		cd.name = _generateName();

	neutral.insert(neutral.begin(), cd);
	return cd.name;
}

/***
 *
 */
template <class T>
void DXCollision<T>::removeFromFriends(const char *name)
{
	for(unsigned int i = 0; i < friends.size(); i++)
		if(friends[i].name == name) {
			friends.erase(friends.begin()+i);
			break;
		}
}

/***
 *
 */
template <class T>
void DXCollision<T>::removeFromEnemies(const char *name)
{
	for(unsigned int i = 0; i < enemies.size(); i++)
		if(enemies[i].name == name) {
			enemies.erase(enemies.begin()+i);
			break;
		}
}

/***
 *
 */
template <class T>
void DXCollision<T>::removeFromNeutral(const char *name)
{
	for(unsigned int i = 0; i < neutral.size(); i++)
		if(neutral[i].name == name) {
			neutral.erase(neutral.begin()+i);
			break;
		}
}

/***
 *
 */
template <class T>
void DXCollision<T>::removeFriends()
{
	for(int i = 0; i < friends.size(); i++)
		friends.erase(friends.begin());
}

/***
 *
 */
template <class T>
void DXCollision<T>::removeEnemies()
{
	for(int i = 0; i < enemies.size(); i++)
		enemies.erase(enemies.begin());
}

/***
 *
 */
template <class T>
void DXCollision<T>::removeNeutral()
{
	for(int i = 0; i < neutral.size(); i++)
		neutral.erase(neutral.begin());
}

/***
 *
 */
template <class T>
void DXCollision<T>::getOponentCollisions()
{
	//map<string, void*> names;
	float l, vectx, vecty;

	_collided.clear();
	for(unsigned int i = 0; i < friends.size(); i++)
		for(unsigned int j = 0; j < enemies.size(); j++) {
			vectx = *(friends[i].x) - *(enemies[j].x);
			vecty = *(friends[i].y) - *(enemies[j].y);
			l = sqrt(vectx*vectx + vecty*vecty);

			if(l < (friends[i].r + enemies[j].r)) {
				_collided[friends[i].name] = enemies[j].obj;
				_collided[enemies[j].name] = friends[i].obj;
			}
		}

	//return names;
}

/***
 *
 *
vector<string> DXCollision::getOponentCollisions()
{
	vector<string> names;
	float l, vectx, vecty;

	for(unsigned int i = 0; i < friends.size(); i++)
		for(unsigned int j = 0; j < enemies.size(); j++) {
			vectx = *(friends[i].x) - *(enemies[j].x);
			vecty = *(friends[i].y) - *(enemies[j].y);
			l = sqrt(vectx*vectx + vecty*vecty);

			if(l < (friends[i].r + enemies[j].r)) {
				names.insert(names.end(), friends[i].name); 
				names.insert(names.end(), enemies[j].name);
			}
		}

	return names;
}

/***
 *
 */
template <class T>
vector<string> DXCollision<T>::getNeutralCollisions()
{
	vector<string> names;
	float l, vectx, vecty;

	for(unsigned int i = 0; i < neutral.size(); i++)
		for(unsigned int j = 0; j < neutral.size(); j++) {
			if(i != j) {
				vectx = neutral[i].x - neutral[j].x;
				vecty = neutral[i].y - neutral[j].y;
				l = sqrt(vectx*vectx + vecty*vecty);

				if(l < (neutral[i].r + neutral[j].r)) {
					names.insert(names.end(), neutral[i].name);
					names.insert(names.end(), neutral[j].name);
				}
			}
		}
	return names;
}

/***
 *
 */
template <class T>
T * DXCollision<T>::getCollided(string name)
{
	if(_collided.find(name) == _collided.end())
		return NULL;

	return _collided[name];
}

#endif