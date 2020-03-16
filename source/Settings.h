#pragma once
#include "AppLog.h"
#include "View.h"
#include <SFML/Config.hpp>
#include <string>
#include <vector>

namespace sf {  class Window;  }


//  settings only playlist
struct SetPls
{
	std::string name;
	char bookm = 0;
};


//  App Settings
//------------------------------------------------
class Settings : public LogErr
{
public:
	const static int ver = 203;  // version of file

	//  main  -----
	Settings();
	
	void Default();

	bool Load(), Save();


	//  window  -----
	const static int MaxViews = 8;

	ViewSet view, views[MaxViews];
	
	void SetDimFromWnd(sf::Window* wnd);


	bool escQuit = false;

	//  playlists
	int cntrPls = 1;  // new playlist counter

	std::vector<SetPls> vSetPls;
};
