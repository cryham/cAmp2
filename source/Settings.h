#pragma once
#include "AppLog.h"
#include "View.h"
#include <SFML/Config.hpp>
#include <string>
#include <vector>

namespace sf {  class Window;  }


enum EDirView
{
	DV_Fullpath, DV_Path, DV_Path2, DV_Path3, /*DV_None,*/ DV_All
};

//  settings only playlist
struct SetPls
{
	std::string name;
	char bookm = 0;
};

//  old player state
struct SetState
{
	bool bPlay = false;
	int iVolume = 500;
	int idPls = 0, idPlay = 0;
	void Default();
};


//  App Settings
//------------------------------------------------
class Settings : public LogErr
{
public:
	const static int ver = 205;  // version of file

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

	EDirView eDirView = DV_Path;
	void NextDirView(int i);

	//  playlists
	int cntrPls = 1;  // new playlist counter

	std::vector<SetPls> vSetPls;
	
	
	//  old state
	SetState state;
};
