#pragma once
#include "Playlist.h"
#include "Settings.h"
#include "AppLog.h"
#include "TimeColors.h"
//#include "View.h"

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
	
	std::unique_ptr<Audio> audio;

	std::unique_ptr<Playlist> pls;

	//std::unique_ptr<Skin> skin;

	bool Init();
	bool Destroy();

	
	//  keys
	bool alt = false, ctrl = false, shift = false;
	bool KeyDown(sf::Event::KeyEvent key);  // use own enum?
	bool KeyUp(sf::Event::KeyEvent key);

	//  mouse pos
	int xMpos=0,yMpos=0, xMold=0,yMold=0;  //, xWclick,yWclick
	int xm=0,ym=0, xms=0,yms=0;  // pos,old
	int xMs,yMs, yMFvi, xLs,yLs;

	//  buttons, s old state
	bool bLs=false,bL=false, bRs=false,bR=false, bMs=false,bM=false;
	bool bLsl, bMInWnd;  float mti=0.f,mtiv=0.f;
	
	void Mouse();  // process, update
	void Mouse(int x, int y)
	{	xMpos = x;  yMpos = y;  }
	void Wheel(int d);
	void MouseDown(int b);
	void MouseUp(int b);
	
	
	float dt = 0.1f;   // frame time, ms
	bool bFps = false;   // show Fps
	bool bDebug = false; // show debug text
	int iTimeTest = 0;   // test pls time colors

	//int yL_pl = 10;  // playlist view lines, auto set
	//  dimensions
	//  B = begin  E = end  W = width  H = height
	float xW_pos=0;  // position marker
	int yB_fi=0,  // file info
		yB_vis=0,yE_vis=0,  // visualization
		yB_pos=0,yE_pos=0, yE_pl_btn=0,  // posbar, btns
		//  pls tabs, size, btns
		yB_pt=0,yE_pt=0, xW_pt=0,yH_pt=0, xW_pt_btn=0,
		// pls tracks  lines, |sliderW,mW, xTime
		yB_pli=0, yB_pl=0,yE_pl=0, yH_pl=0,yL_pl=0, xW_plSm=0,xTm=0,
		// gui checks
		xB_gc=0,xB_gck=0,yB_gc=0, xW_gc=0,yH_gc=0;

	void UpdDim(/*float rfrFq=-1.f*/);
	
	//  player
	void Play();
};
