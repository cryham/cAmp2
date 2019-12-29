#pragma once
#include "Playlist.h"
#include "Settings.h"
#include "AppLog.h"
#include "TimeColors.h"

#include <SFML/Window/Event.hpp>  // input
#include <memory>

class Audio;


class App : public LogErr
{
public:
    App();
	virtual ~App();

    virtual bool Run() = 0;

protected:
	
	Settings set;
	//ViewSet view;  // in set
	TimeClrSets timeColors;
	//std::unique_ptr<Skin> skin;
	
	std::unique_ptr<Audio> audio;
	std::unique_ptr<Playlist> pls;

	bool Init();
	bool Destroy();

	
	//  keys
	bool alt = false, ctrl = false, shift = false;
	void UpdModifiers();
	bool KeyDown(sf::Event::KeyEvent key);  // use own enum?

	
	//  mouse pos
	int xMpos=0,yMpos=0, xMold=0,yMold=0;  //, xWclick,yWclick
	int xm=0,ym=0, xms=0,yms=0;  // pos,old
	
	//  mmb vars
	int xMs=0,yMs=0, yM_visH=0, xL_ofs=0,yL_sl=0;
	float mti=0.f, mtiv=0.f;

	//  buttons, s old state
	bool bLs=false,bL=false, bRs=false,bR=false, bMs=false,bM=false;
	bool bL_sl=false;
	bool bDrawPlst = true;
	
	void Mouse();  // process, update
	void Mouse(int x, int y)
	{	xMpos = x;  yMpos = y;  }
	void Wheel(int d);
	void MouseDown(int b);
	void MouseUp(int b);
	
	
	//  toggle
	float dt = 0.1f;   // frame time, ms
	bool bFps = false;   // show Fps
	bool bDebug = false; // show debug text
	int iTimeTest = 0;   // test pls time colors

	
	//  dimensions
	//  B = begin  E = end  W = width  H = height
	float xW_pos=0.03f;  // seek, position marker
	int yB_inf=0,  // file info
		yE_plr_btn=0,  // player buttons |< >|
		
		yB_vis=0,yE_vis=0,  // visualization
		yB_pos=0,yE_pos=0,  // seek pos bar
		
		//  pls tabs, size, btns
		yB_tabs=0,yE_tabs=0, xW_tabs=0,yH_tabs=0, xW_tabs_btn=0,
		//  pls tracks  lines, |sliderW,mW, xTime
		yB_pl_inf=0, yB_pl=0,yE_pl=0, yH_pl=0,
		yL_pl = 10,  // playlist view lines, auto set
		
		xWex_plS=0;  // extra width for slider drag
	
	void UpdDim(/*float rfrFq=-1.f*/);
};
