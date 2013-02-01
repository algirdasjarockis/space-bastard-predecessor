#ifndef _GUISYSTEM_H_
#define _GUISYSTEM_H_
#include "../DXEngine2D/DXEngine2D.h"
#include "../DXEngine2D/DXSoundManager.h"
#include "../TinyXml/tinyxml.h"
#include "logger.h"
#include "helpers.h"
#include "Properties.h"

#include <algorithm>
#include <map>
#include <vector>
#include <string>
#include <math.h>

using namespace std;

class GuiSystem
{
	DXEngine2D *_dx2d;
	DXSoundManager *_soundMgr;

	Logger *lg;
	Properties *props;

	struct RenderData 
	{
		string type;
		vector<string> objects;
	};

	map<string, map<string, RenderData>> renderPipe;
	string buttonSheetName;

	string _bgFile;

	bool _createFonts();
	bool _createDebugFont();
public:
	GuiSystem(Logger *lg, DXSoundManager *sndMgr, Properties *ps);

	bool init(DXEngine2D *dx2d);
	bool render(string screen);
	void showCursor();

	void showHealthBar(int value, int maxValue);
	void showLives(char count);
};

#endif