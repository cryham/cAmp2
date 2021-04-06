#pragma once
#include "AppLog.h"
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
class Settings : public LogErr
{
public:
	const static int ver = 208;  // version of file

	//  main  -----
	Settings();
	
	void Default();

	bool Load(), Save();


	//  window  -----
	const static int MaxViews = 8;

	ViewSet view, views[MaxViews];
	
	void SetDimFromWnd(sf::Window* wnd);


	bool escQuit = false;
	bool bFileInfo = false;

	EDirView eDirView = DirV_Path;
	void NextDirView(int i);

	//  playlists
	int cntrPls = 1;  // new playlist counter

	std::vector<std::string> vSetPls;
	
	SetFind find;
	
	//  old state
	SetState state;

	int dimTabTxt = 16, dimTabBck = 16;  // in viewSet?
};
