#include "DXSound.h"

/*** 
 *
 */
DXSound::DXSound()
{
	winamp = NULL;
	loader = NULL;
	segment = NULL;
}

/***
 *
 */
DXSound::~DXSound()
{
	/*
	winamp->CloseDown();
	loader->Release();
	winamp->Release();

	CoUninitialize()*/;
}

/***
 *
 */
void DXSound::release()
{
	winamp->CloseDown();
	loader->Release();
	winamp->Release();

	CoUninitialize();
}

/***
 *
 */
bool DXSound::init(HWND hwnd)
{
	if(FAILED(CoInitialize(NULL) ))
		return false;

	if(FAILED(CoCreateInstance(CLSID_DirectMusicPerformance, NULL, CLSCTX_INPROC, 
			IID_IDirectMusicPerformance8, (void**) &winamp) ))
		return false;

	if(FAILED(winamp->InitAudio(NULL, NULL, hwnd, DMUS_APATH_SHARED_STEREOPLUSREVERB, 64, DMUS_AUDIOF_ALL, NULL)))
		return false;

	if(FAILED(CoCreateInstance(CLSID_DirectMusicLoader, NULL, CLSCTX_INPROC, 
			IID_IDirectMusicLoader8, (void**) &loader) ))
		return false;

	if(FAILED(CoCreateInstance(CLSID_DirectMusicSegment, NULL, CLSCTX_INPROC, 
				IID_IDirectMusicSegment8, (void**) &segment)))
		return false;

	WCHAR searchPath[MAX_PATH];
	GetCurrentDirectory(MAX_PATH, searchPath);

	loader->SetSearchDirectory(GUID_DirectMusicAllTypes, searchPath, FALSE);

	return true;
}

/***
 *
 */
bool DXSound::load(const char *srcFile)
{
	WCHAR wFilename[MAX_PATH];
	MultiByteToWideChar(CP_ACP, 0, srcFile, -1, wFilename, MAX_PATH);

	if(FAILED(loader->LoadObjectFromFile(CLSID_DirectMusicSegment, IID_IDirectMusicSegment8, 
			wFilename, (void**)&segment) ))
		return false;

	if(FAILED(segment->Download(winamp)))
		return false;

	return true;
}

/***
 *
 */
bool DXSound::play(bool loop)
{
	if(loop) 
		segment->SetRepeats(DMUS_SEG_REPEAT_INFINITE);

	if(FAILED(winamp->PlaySegmentEx(segment, NULL, NULL, 0, 0, NULL, NULL, NULL) ))
		return false;

	return true;
}

/***
 *
 */
bool DXSound::stop()
{
	return SUCCEEDED(winamp->StopEx(segment, 0, 0));
}

/***
 *
 */
bool DXSound::silence()
{
	return SUCCEEDED(winamp->Stop(NULL, NULL, 0, 0));
}

/***
 *
 */
bool DXSound::isPlaying()
{
	return (winamp->IsPlaying(segment, NULL) == S_OK);
}