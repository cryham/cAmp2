#pragma once
#include <fstream>


//  Log file  ----
class LogFile
{
	static std::ofstream ofLog;
	static bool noLog;// = true;
public:
	static int iErrors;// = 0;

	static void Init();
	static void Log(std::string s);
	static void Error(std::string s);
};


//  helper for adding log
class Logger
{
public:
	static void Log  (std::string s) {  LogFile::Log(s);    }
	static void Error(std::string s) {  LogFile::Error(s);  }
};
