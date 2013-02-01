#ifndef _LOGGER_H_
#define _LOGGER_H_

#include <iostream>
#include <fstream>
#include <time.h>
#include <string>

using namespace std;

class Logger
{
	ofstream fr;
	const char *filename;

	// siaip tekstas.. paprastai jis skirtas tik saugoti stringui, kuris gali buti 
	// naudojamas debug informacijai isvesti zaidimo metu ant pacio ekrano
	string visualText;
public:
	Logger(const char *fName);
	~Logger();
	void log(const char *text);
	void setVisualText(const char *text);
	void appendVisualText(const char *text);
	const char * getVisualText();
	void close();
};

#endif