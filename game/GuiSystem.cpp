#include "GuiSystem.h"

/***
 *
 */
GuiSystem::GuiSystem(Logger *lg, DXSoundManager *sndMgr, Properties *ps)
{
	_dx2d = NULL;
	this->lg = lg;
	props = ps;
	_soundMgr = sndMgr;
}

/***
 *
 */
bool GuiSystem::_createFonts()
{
	TiXmlDocument xmlDoc( (string("./") + props->getValue("mod") + string("styles.xml")).c_str() );
	if(!xmlDoc.LoadFile()) {
		xmlDoc.Clear();
		lg->log("ERROR: styles.xml not found.");
		return false;
	}

	if(!xmlDoc.FirstChildElement("styles")) {
		lg->log("ERROR: invalid font config");
		xmlDoc.Clear();
		return false;
	}

	TiXmlElement *style = xmlDoc.FirstChildElement("styles")->FirstChildElement("style");
	
	while(style) {
		DXFont::FontStyle fs;
		ZeroMemory(&fs, sizeof(fs));
		
		fs.fontName = toWide(style->FirstChildElement("font")->FirstChild()->Value());
		fs.height = atoi(style->FirstChildElement("size")->FirstChild()->Value());

		// colors
		int red = atoi(style->FirstChildElement("color")->FirstChildElement("red")->FirstChild()->Value());
		int green = atoi(style->FirstChildElement("color")->FirstChildElement("green")->FirstChild()->Value());
		int blue = atoi(style->FirstChildElement("color")->FirstChildElement("blue")->FirstChild()->Value());
		int alpha = atoi(style->FirstChildElement("color")->FirstChildElement("alpha")->FirstChild()->Value());
		fs.color = D3DCOLOR_ARGB(alpha, red, green, blue);

		fs.charset = atoi(style->FirstChildElement("charset")->FirstChild()->Value());
		if(!strcmp("1", style->FirstChildElement("italic")->FirstChild()->Value()))
			fs.italic = true;
		else fs.italic = false;

		fs.weight = atoi(style->FirstChildElement("bold")->FirstChild()->Value());

		fs.pitchAndFamily = DEFAULT_PITCH || FF_MODERN;

		// uzsetinamas fontas
		if(!_dx2d->createFontStyle(style->FirstAttribute()->Value(), &fs)) {
			string str(toString(fs.fontName));
			string str1 = "WARNING: font '" + str + "' was not created!";
			lg->log(str.c_str());
		}

		style = style->NextSiblingElement("style");
	}

	return true;
}

/***
 *
 */
bool GuiSystem::_createDebugFont()
{
	DXFont::FontStyle fs;
	ZeroMemory(&fs, sizeof(fs));
	fs.fontName = toWide("Courier New");
	fs.height = 16;
	fs.italic = false;
	fs.color = D3DCOLOR_ARGB(170, 50, 250, 50);
	fs.pitchAndFamily = DEFAULT_PITCH || FF_MODERN;

	// uzsetinamas fontas
	if(!_dx2d->createFontStyle("debugFont", &fs)) {
		string str(toString(fs.fontName));
		string str1 = "WARNING: font '" + str + "' was not created (debug font)!";
		lg->log(str.c_str());
		return false;
	}

	return true;
}

/***
 *
 */
bool GuiSystem::init(DXEngine2D *dx2d)
{
	_dx2d = dx2d;
	if(!_dx2d) {
		lg->log("ERROR: bad graphics.");
		return false;
	}

	lg->log("Setting fonts..");
	if(!_createFonts())
		return false;

	if(_createDebugFont()) {
		RECT debugMsgRect = {10, 8, 780, 600};
		_dx2d->getFontStyle("debugFont")->setLabel(NULL, toWide(" "), "debugMessage", debugMsgRect, DT_RIGHT);
	}

	TiXmlDocument config( (string("./") + string(props->getValue("mod")) + string("gui.xml")).c_str());
	if(!config.LoadFile()) {
		config.Clear();
		lg->log("ERROR: gui.xml not found.");
		return false;
	}

	TiXmlElement *gui = config.FirstChildElement("gui");

	if(!gui) {
		lg->log("ERROR: invalid config.");
		config.Clear();
		return false;
	}

	renderPipe.clear();

	TiXmlElement *screen = gui->FirstChildElement("screen");

	while (screen) {

		string screenName = string(screen->Attribute("name"));
	
		// kelias iki gui katalogo
		string guiDir = string("./") + props->getValue("mod") + props->getValue("gui_dir");

		// kelias iki zaidimo gui fono
		_bgFile = guiDir + string(screen->FirstChildElement("background")->FirstChild()->Value());

		// kelias iki sounds 
		string soundsDir = string("./") + props->getValue("mod") + props->getValue("sounds_dir");

		// sukuriamas fonas
		if(!_dx2d->createBackground(toWide( _bgFile.c_str() )) )
			lg->log("WARNING: background was not created.");

		if(!_soundMgr->createTrack(screenName.c_str(), (soundsDir + string(
			screen->FirstChildElement("sound")->FirstChild()->Value())).c_str() ) )
			lg->log("WARNING: main track was not created.");
	

		// gaunama mygtuku info
		TiXmlElement *buttons = screen->FirstChildElement("buttons");
		if(buttons) {		
		
			// mygtuku spritesheeto kurimas
			TiXmlElement *sprSheet = buttons->FirstChildElement("spritesheet");
			if(sprSheet) {
				buttonSheetName = sprSheet->FirstChildElement("name")->FirstChild()->Value();

				if(!_dx2d->createSpriteSheet(
					buttonSheetName.c_str(),			
					toWide((guiDir + string(sprSheet->FirstChildElement("path")->FirstChild()->Value())).c_str() ),	
					(guiDir + string(sprSheet->FirstChildElement("datafile")->FirstChild()->Value())).c_str(),
					D3DCOLOR_XRGB(
						atoi(sprSheet->FirstChildElement("transcolor")->FirstChildElement("red")->FirstChild()->Value()),			
						atoi(sprSheet->FirstChildElement("transcolor")->FirstChildElement("green")->FirstChild()->Value()),			
						atoi(sprSheet->FirstChildElement("transcolor")->FirstChildElement("blue")->FirstChild()->Value()) 
					)
				)
				) lg->log("WARNING: spritesheet object for buttons was not created.");			
			}
			else lg->log("WARNING: definition for spritesheet of buttons not found.");		

			TiXmlElement *button = buttons->FirstChildElement("button");

			renderPipe[screenName][buttonSheetName].type = "button";
		
			while(button) {

				const char *btnName = button->FirstAttribute()->Value();
				if(!_dx2d->createButton(
						btnName, 
						buttonSheetName.c_str(), 
						button->FirstChildElement("sprite")->FirstChild()->Value(), 
						toWide(button->FirstChildElement("label")->FirstChildElement("caption")->FirstChild()->Value()), 
						button->FirstChildElement("label")->FirstChildElement("style")->FirstChild()->Value())
				) lg->log( (string("WARNING: button '") + string(btnName) + string("' was not created.") ).c_str() );
				else {
					// mygtukas sekmingai sukurtas, galim keisti jo savybes
					//renderPipe[buttonSheetName].insert(renderPipe[buttonSheetName].end(), btnName);
					renderPipe[screenName][buttonSheetName].objects.insert(renderPipe[screenName][buttonSheetName].objects.end(), btnName);

					_dx2d->getButton(btnName)->setPosition(
						atoi(button->FirstChildElement("position")->FirstChildElement("left")->FirstChild()->Value()),
						atoi(button->FirstChildElement("position")->FirstChildElement("top")->FirstChild()->Value())
					);
			
					_dx2d->getButton(btnName)->setOverColor(D3DCOLOR_ARGB(
						atoi(button->FirstChildElement("overcolor")->FirstChildElement("alpha")->FirstChild()->Value()),
						atoi(button->FirstChildElement("overcolor")->FirstChildElement("red")->FirstChild()->Value()),
						atoi(button->FirstChildElement("overcolor")->FirstChildElement("green")->FirstChild()->Value()),
						atoi(button->FirstChildElement("overcolor")->FirstChildElement("blue")->FirstChild()->Value())				
						)
					);
				
					const char *sound = button->FirstChildElement("click")->FirstChild()->Value();
					_dx2d->getButton(btnName)->setSound(sound);

					if(!_soundMgr->trackExists(sound))
						if(!_soundMgr->createTrack(sound, (soundsDir + string(sound)).c_str() ))
							lg->log( (string("WARNING: sound '") + string(soundsDir) + string(sound) + string("' was not created.")).c_str() );
				}

				button = button->NextSiblingElement("button");
			}
		}	// if buttons
		else lg->log("WARNING: definition for buttons not found.");

		// sukuriamas peles zymeklis
		dx2d->createSpriteSheet(props->getValue("sheet_guielements").c_str(), 
				toWide( (guiDir + string("elements.png")).c_str() ), 
				(guiDir + string("elements.txt")).c_str(),
				D3DCOLOR_XRGB(0, 0, 0));

		// gaunami tekstai
		TiXmlElement *labels = screen->FirstChildElement("labels");
		if(labels) {
			// numeris, pridedamas prie label vardo, taip padarant ji unikaliu
			int labelNo = 0;

			// generuojamas label'io vardas
			string labelName;
			TiXmlElement *label = labels->FirstChildElement("label");

			while(label) {
				labelName = "_gui_label_";
			
				const char *fStyle = label->FirstChildElement("style")->FirstChild()->Value();
				renderPipe[screenName][fStyle].type = "label";

				RECT rect;
				rect.top = atoi(label->FirstChildElement("position")->FirstChildElement("top")->FirstChild()->Value());
				rect.left = atoi(label->FirstChildElement("position")->FirstChildElement("left")->FirstChild()->Value());
				rect.bottom = atoi(label->FirstChildElement("position")->FirstChildElement("bottom")->FirstChild()->Value());
				rect.right = atoi(label->FirstChildElement("position")->FirstChildElement("right")->FirstChild()->Value());
			
				wchar_t *caption = toWide(label->FirstChildElement("caption")->FirstChild()->Value());

				char chLabelNo[4];
				itoa(labelNo++, chLabelNo, 10);

				// sukuriamas naujas label'io vardas
				labelName += string(chLabelNo);

				_dx2d->getFontStyle(fStyle)->setLabel(NULL, caption, labelName.c_str(), rect, DT_LEFT);

				// issaugom i vaizduojamu objektu sarasa (tekstas turi buti aukciau visko, todel issaugome pradzioj)
				renderPipe[screenName][fStyle].objects.insert(renderPipe[screenName][fStyle].objects.begin(), labelName);

				label = label->NextSiblingElement("label");
			}
		}
	
		screen = screen->NextSiblingElement("screen");
	}

	// nustatomi reikalingi gyvybes matuoklio parametrai
	dx2d->getSpriteSheet(props->getValue("sheet_guielements").c_str())->setPosition("health_bar", 20, 560);

	// sukuriame gyvybes matuoklio padalas
	string hbsName = "health_bar_step";
	D3DXVECTOR3 hbPos = dx2d->getSpriteSheet(props->getValue("sheet_guielements").c_str())->getPosition("health_bar");
	int hbsWidth, hbsHeight;
	dx2d->getSpriteSheet(props->getValue("sheet_guielements").c_str())->getSize(hbsName.c_str(), &hbsWidth, &hbsHeight);
		
	hbPos.x += 3;
	for(char i = 'a'; i <= 'a' + 20; i++) {
		dx2d->getSpriteSheet(props->getValue("sheet_guielements").c_str())->createCopy(hbsName.c_str(), (hbsName+i).c_str() );
		dx2d->getSpriteSheet(props->getValue("sheet_guielements").c_str())->setPosition((hbsName+i).c_str(), hbPos.x, hbPos.y + 3);
		hbPos.x += (hbsWidth + 1);
	}

	dx2d->getSpriteSheet(props->getValue("sheet_guielements").c_str())->setPosition("live", 10, 30);

	return true;
}

/***
 *
 */
bool GuiSystem::render(string screen)
{
	if(renderPipe.size() <= 0)
		return false;

	if(!_dx2d->background->isCreated())
		if(!_dx2d->createBackground(toWide( _bgFile.c_str() )) )
			lg->log("WARNING: background was not created at rendering.");

	_dx2d->background->scroll(0);

	map<string, RenderData>::iterator it = renderPipe[screen].begin();
	while(it != renderPipe[screen].end()) {

		// mygtukai
		if(it->second.type == "button") {
			_dx2d->getSpriteSheet(it->first.c_str())->getSpriteInterface()->Begin(D3DXSPRITE_ALPHABLEND);
			for(unsigned int i = 0; i < it->second.objects.size(); i++) {
				//..
				if(_dx2d->buttonExists(it->second.objects[i].c_str() ))
					_dx2d->getButton( it->second.objects[i].c_str() )->draw();
			}
			_dx2d->getSpriteSheet(it->first.c_str())->getSpriteInterface()->End();
		}
		else {
			// label'iai
			for(unsigned int i = 0; i < it->second.objects.size(); i++) {
				_dx2d->getFontStyle(it->first.c_str())->drawLabel(it->second.objects[i].c_str());
			}			
		}
		it++;
	}

	return true;
}

/***
 *
 */
void GuiSystem::showCursor()
{
	_dx2d->getSpriteSheet(props->getValue("sheet_guielements").c_str())->getSpriteInterface()->Begin(D3DXSPRITE_ALPHABLEND);
	_dx2d->getSpriteSheet(props->getValue("sheet_guielements").c_str())->showNextFrame("pointer");
	_dx2d->getSpriteSheet(props->getValue("sheet_guielements").c_str())->getSpriteInterface()->End();
}

/***
 *
 */
void GuiSystem::showHealthBar(int value, int maxValue)
{
	DXSpriteSheet *sheet = _dx2d->getSpriteSheet(props->getValue("sheet_guielements").c_str());

	if(sheet) {
		sheet->getSpriteInterface()->Begin(D3DXSPRITE_ALPHABLEND);
		sheet->showNextFrame("health_bar");
		
		int stepValue = maxValue / 20;
		char stepCount = (char)floor((float)(value / stepValue));
		string hbsName = "health_bar_step";
		for(char i = 'a'; i < 'a' + stepCount; i++) {
			sheet->showNextFrame((hbsName+i).c_str());
		}

		sheet->getSpriteInterface()->End();
	}
}

/***
 *
 */
void GuiSystem::showLives(char count)
{
	DXSpriteSheet *sheet = _dx2d->getSpriteSheet(props->getValue("sheet_guielements").c_str());
	if(!sheet) return;

	D3DXVECTOR3 beginPos = sheet->getPosition("live");
	int lWidth, lHeight;
	sheet->getSize("live", &lWidth, &lHeight);

	sheet->getSpriteInterface()->Begin(D3DXSPRITE_ALPHABLEND);
	for(char i = 'a'; i < 'a'+count; i++) {
		// sukuriam spraitu kopijas, jei jos dar nebuvo sukurtos
		if(!sheet->spriteExists( (string("live") + i).c_str() ) ) {
			sheet->createCopy("live", (string("live") + i).c_str() );
			sheet->setPosition( (string("live") + i).c_str(), beginPos.x + (lWidth * (i-'a')), beginPos.y);
		}

		sheet->showNextFrame( (string("live") + i).c_str() );
	}
	sheet->getSpriteInterface()->End();
}