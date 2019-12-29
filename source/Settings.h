#pragma once
#include "AppLog.h"
#include "View.h"
#include <SFML/Config.hpp>

namespace sf {  class Window;  }


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


	//  window  -----

	ViewSet view;
	
	void SetDimFromWnd(sf::Window* wnd);


	bool escQuit = false;
};
