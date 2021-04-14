#pragma once
#include "LogFile.h"
#include <vector>
#include <string>
#include <filesystem>
namespace fs = std::filesystem;


class FileSystem : public Logger // and ConfigPaths
{
private:
	static std::string
		home_dir, user_config, app_config,
		app_data/*, user_data, cache_dir*/;

public:
	//  File system utility  ----

	//  List files and subdirs in path
	static std::vector<fs::path> ListDir(const fs::path start, bool recursive=true);

	static bool Exists(const std::string & filename);

	static uintmax_t Size(const std::string & filename);

	static bool CreateDir(const std::string& path);

	static fs::path ExecName();  // application path


	//  Call first to have paths  ----
	static void Init();

	static std::string GetInfo();

	//  Paths
	static std::string Home() {       return home_dir; }
	//static std::string Cache() {    return cache_dir; }

	static std::string Config() {     return user_config;  }
	static std::string AppConfig() {  return app_config;  }
	static std::string Data() {       return app_data; }
	
	static std::string Colors() {     return app_data + "/colors.xml"; }
	static std::string ColorsUser() { return user_config + "/colors.xml"; }
	
	static std::string Settings() {   return user_config + "/cAmp2.xml"; }
	static std::string Log() {        return user_config + "/cAmp2.log"; }
	static std::string Playlists() {  return user_config + "/playlists/"; }
};
