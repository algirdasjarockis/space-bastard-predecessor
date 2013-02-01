#ifndef _PROPERTIES_H_
#define _PROPERTIES_H_

#include <string>
#include <string>
#include <map>
#include <algorithm>

using namespace std;

class Properties
{
	map<string, string> props;
public:
	void setValue(const char *name, char const *value);
	string getValue(const char *name);
};

#endif