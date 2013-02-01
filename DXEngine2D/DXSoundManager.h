#ifndef _DXSOUNDMANAGER_H_
#define _DXSOUNDMANAGER_H_

#include "DXSound.h"
#include <map>
#include <string>
#include <algorithm>
#include <assert.h>

using namespace std;

class DXSoundManager
{
	HWND hWnd;
	map<string, DXSound> tracks;
public:
	DXSoundManager(HWND hWnd);
	~DXSoundManager();
	bool createTrack(const char *trackName, const char *srcFile);
	bool trackExists(const char *trackName);
	bool play(const char *trackName, bool loop = true);
	bool stop(const char *trackName);
	bool stopAll();
	
	void removeTrack(const char *trackName);
	bool exists(const char *trackName);
	bool isPlaying(const char *trackName);
};

#endif