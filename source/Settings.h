#pragma once
#include "AppLog.h"
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

	//  dimensions  -----
//	float fSplit = 0.22f;   // splitter 0..1
//	int   iFontH = 18;      // font height in list
//	int   iFontGui = 17;    // font height for Gui
//	int   iLineH = 2;       // line spacing


	//  window  -----
	int xwPos = 0, ywPos = 0;
	uint xwSize = 350, ywSize = 768;
	void GetWndDim(sf::Window* wnd);

	bool escQuit = false;
};
