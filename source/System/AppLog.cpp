#include "AppLog.h"
#include "FileSystem.h"
#include <fstream>
#include <iostream>
using namespace std;


//  vars  ----
ofstream AppLog::ofLog;
bool AppLog::noLog = true;
int AppLog::iErrors = 0;


//  init  ----
void AppLog::Init()
{
	ofLog.open(FileSystem::Log());
	noLog = !ofLog.is_open();
	if (noLog)
		cerr << "Error! Can't open log file: " << FileSystem::Log() << endl;
}

//  log
void AppLog::Log(string s)
{
	if (noLog)  return;
	ofLog << s << endl;
	ofLog.flush();
}

//  error
void AppLog::Error(string s)
{
	++iErrors;
	Log("Error! " + s);
}
