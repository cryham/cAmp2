#include "LogFile.h"
#include "FileSystem.h"
#include <fstream>
#include <iostream>
using namespace std;


//  vars  ----
ofstream LogFile::ofLog;
bool LogFile::noLog = true;
int LogFile::iErrors = 0;


//  init  ----
void LogFile::Init()
{
	ofLog.open(FileSystem::Log());
	noLog = !ofLog.is_open();
	if (noLog)
		cerr << "Error! Can't open log file: " << FileSystem::Log() << endl;
}

//  log
void LogFile::Log(string s)
{
	if (noLog)  return;
	ofLog << s << endl;
	ofLog.flush();
}

//  error
void LogFile::Error(string s)
{
	++iErrors;
	Log("Error! " + s);
}
