#include "helpers.h"
#include <iostream>

wchar_t * toWide(const char *text)
{
	if(!text)
		return NULL;

	wchar_t *wtext = new wchar_t[strlen(text)+1];
    size_t convertedChars = 0;
    mbstowcs_s(&convertedChars, wtext, strlen(text) + 1, text, _TRUNCATE);
	wtext[ convertedChars ] = L'\0';

	return wtext;
}

char * toString(const wchar_t *text)
{
	if(!text)
		return NULL;

	char *ctext = new char[wcslen(text)+1];
    size_t convertedChars = 0;
    wcstombs_s(&convertedChars, ctext, wcslen(text)+1, text, _TRUNCATE);
	ctext[ convertedChars ] = '\0';

	return ctext;
}