#pragma once
#include "../System/LogFile.h"
#include "View.h"
#include <SFML/Config.hpp>
#include <string>
#include <vector>

namespace sf {  class Window;  }


enum EDirView
{
	DirV_Fullpath, DirV_Path, DirV_Path2, DirV_Path3, /*DirV_None,*/ DirV_All
};
const static char* sDirView[DirV_All] =
{
	"Full path", "Path", "Path / Parent", "Path / Parent / Parent2", /*"None"*/
};
const static char* sTimesTest[3] =
{	"off", "short", "long"  };


//  old player state
struct SetState
{
	bool bPlay = false;
	int iVolume = 500;
	int idPls = 0, idPlay = 0;
	void Default();
};

//  find in pls opts
struct SetFind
{
	bool bCaseSens = false;
	bool bFullPath = false;  // todo: only fname, path..
	bool bUnfiltered = true;
	bool bAllPls = true;
	void Default();
};


//  App Settings
//------------------------------------------------
class Settings : public Logger
{
public:
	const static int ver = 210;  // version of file

	//  main  -----
	Settings();
	
	void Default();

	bool Load();
	bool Save() const;  // cAmp2.xml


	//  views  -----
	const static int cMaxViews = 10;
	ViewSet view, views[cMaxViews];

	
	//  debug
	bool escQuit = false;

	//  view
	bool bFileInfo = false;
	EDirView eDirView = DirV_Path;
	void NextDirView(int i);

	//  playlists
	int cntrPls = 1;  // new playlist counter
	std::vector<std::string> vSetPls;
	
	SetFind find;
	SetState state;  // player

	int dimTabTxt = 16, dimTabBck = 16;  // in viewSet?
};
