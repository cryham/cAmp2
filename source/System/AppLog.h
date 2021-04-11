#pragma once
#include <fstream>


//  Log file  ----
class AppLog
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
class LogErr
{
public:
	static void Log  (std::string s) {  AppLog::Log(s);    }
	static void Error(std::string s) {  AppLog::Error(s);  }
};
