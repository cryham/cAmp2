#pragma once
#include "AppLog.h"
#include "View.h"
#include <SFML/Config.hpp>

namespace sf {  class Window;  }
typedef unsigned int uint; 


//  App Settings
//------------------------------------------------
class Settings : public LogErr
{
public:
	const static int ver = 202;  // version of file

	//  main  -----
	Settings();
	void Default();

	bool Load(), Save();


	//  paths  -----

	//  window  -----
	ViewSet view;
	
	void GetWndDim(sf::Window* wnd);


	bool escQuit = false;
};
