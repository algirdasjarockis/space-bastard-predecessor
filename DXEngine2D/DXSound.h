#ifndef _DXSOUND_H_
#define _DXSOUND_H_

#include <dsound.h>
#include <dmusici.h>
#include <assert.h>

#pragma comment (lib, "dsound.lib")

#ifdef UNICODE
typedef LPCWSTR LPCOMSTRING;
#else
typedef LPCSTR LPCOMSTRING;
#endif

class DXSound
{
	IDirectMusicPerformance8 *winamp;
	IDirectMusicLoader8* loader;
	IDirectMusicSegment8* segment;

public:
	DXSound();
	~DXSound();
	void release();
	bool init(HWND hwnd);
	bool load(const char * srcFile);
	bool play(bool loop = true);
	bool stop();
	bool silence();
	bool isPlaying();
};

#endif