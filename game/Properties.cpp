#include "Properties.h"

void Properties::setValue(const char *name, const char *value)
{
	props[string(name)] = string(value);
}

string Properties::getValue(const char *name)
{
	return props[name];
}