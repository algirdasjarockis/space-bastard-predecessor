#include "DXSoundManager.h"

/***
 *
 */
DXSoundManager::DXSoundManager(HWND hWnd)
{
	this->hWnd = hWnd;
}

/***
 *
 */
DXSoundManager::~DXSoundManager()
{
	while(!tracks.empty())
		tracks.erase(tracks.begin());
}

/***
 *
 */
bool DXSoundManager::createTrack(const char *trackName, const char *srcFile)
{
	DXSound sound;

	if(!sound.init(hWnd))
		return false;

	if(!sound.load(srcFile))
		return false;

	tracks.insert(make_pair(trackName, sound));
	return true;
}

/***
 *
 */
bool DXSoundManager::trackExists(const char *trackName)
{
	return (tracks.find(trackName) != tracks.end());
}

/***
 *
 */
bool DXSoundManager::play(const char *trackName, bool loop)
{
	return tracks[trackName].play(loop);
}

/***
 *
 */
bool DXSoundManager::stop(const char *trackName)
{
	return tracks[trackName].stop();
}

/***
 *
 */
bool DXSoundManager::stopAll()
{
	map<string, DXSound>::iterator it;

	for(it = tracks.begin(); it != tracks.end(); it++) {
		it->second.silence();
	}

	return true;
}

/***
 *
 */
void DXSoundManager::removeTrack(const char *trackName)
{
	tracks[trackName].release();
	tracks.erase(trackName);
}

/***
 *
 */
bool DXSoundManager::exists(const char *trackName)
{
	map<string, DXSound>::iterator it;
	it = tracks.find(trackName);

	return it != tracks.end();
}

/***
 *
 */
bool DXSoundManager::isPlaying(const char *trackName)
{
	return tracks[trackName].isPlaying();
}