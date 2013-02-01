#include "helpers.h"
#include "logger.h"

/***
 *
 */
Logger::Logger(const char *fName)
{
	filename = fName;
	visualText = toString(L" ");
}

/***
 *
 */
Logger::~Logger()
{
	if(fr.is_open())
		fr.close();
}

/***
 *
 */
void Logger::log(const char *text)
{
	if(!fr.is_open())
		fr.open("log.txt");

	time_t rawtime;
	struct tm * timeinfo;
	char timeStr [20];

	time ( &rawtime );
	timeinfo = localtime ( &rawtime );

	strftime (timeStr,80,"[%X] ",timeinfo);
	
	fr << timeStr << text << endl;
}

/***
 *
 */
void Logger::setVisualText(const char* text)
{
	visualText = text;
}

/***
 *
 */
void Logger::appendVisualText(const char* text)
{
	string old = visualText;
	string tail = text;
	//const char* _visualText = string(visualText).append(string(text)).c_str();
	//visualText = _visualText;
	visualText = old + tail;
}

/***
 *
 */
const char * Logger::getVisualText()
{
	return visualText.c_str();
}

/***
 *
 */
void Logger::close()
{
	if(fr.is_open())
		fr.close();
}