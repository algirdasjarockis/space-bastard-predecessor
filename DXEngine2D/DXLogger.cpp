#include "DXLogger.h"

/*
bool DXLogger2::open(const char *filename)
{
	fr = new ofstream;

	fr->open(filename);

	return !fr->fail();
}

/***
 *
 /
void DXLogger2::log(char *text)
{
	time_t rawtime;
	struct tm * timeinfo;
	char timeStr [20];

	time ( &rawtime );
	localtime_s(timeinfo, &rawtime );

	strftime (timeStr,80,"[%X:%S] ",timeinfo);
	
	*fr << timeStr << text << endl;
}
*/