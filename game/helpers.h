/***
 *	Ivairios "helpines" funkcijos
 */

#ifndef _HELPERS_H_
#define _HELPERS_H_

#include <stdlib.h>

#ifndef _STRING_
#include <string>
#endif

// char * tekstas konvertuojamas i wide
wchar_t * toWide(const char *text);

// wide tekstas konvertuojamas i char
char * toString(const wchar_t *text);

#endif