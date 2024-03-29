#pragma once
#include "../Playlist/Playlist.h"
#include "../Settings/Settings.h"
#include "../Settings/Themes.h"
#include "../System/LogFile.h"
#include <SFML/Window/Event.hpp>  // input, use own enum?
#include <memory>
#include <vector>

class Audio;
class ActionsMap;


class App : public Logger
{
public:
	App();
	virtual ~App();

	virtual bool Run() = 0;

protected:
	
	Settings set;
	//ViewSet view;  // in set
	Themes themes;
	//std::unique_ptr<Skin> skin;  // todo:
	
	std::unique_ptr<Audio> audio;

	bool Init();
	bool Destroy();

	
	///  PlayLists Tabs  ------------------------------------------------
	std::vector<Playlist> vPls;  // always at least 1
	Playlist& Pls()   {  return vPls[plsId];  }
	Playlist& PlsPl() {  return vPls[plsPlId];  }
	Playlist& PlsSel(){  return plsSelId < 0 ? Pls() : vPls[plsSelId];  }

	void LoadPls(), SavePls();  // for set
	Stats all, allFull;
	
	int plsId = 0, plsPlId = 0,  // current, playing
		plsSelId = -1,  // selected, copy from
		nTabMov = -1;
	
	int queId = 0;  // queue, to add
	EInsert queIns = Ins_End;

	void updSelId(int clear);
	void clrSelId();

	void TabNext(int i, bool row, bool ofs);
	void TabMove(int n);
	void TabNew(int m);
	void TabCloses();

	
	//  Advanced  ----
	//  Find
	std::string sFind;
	bool bFind = false;  // hide
	int iFoundSee = 0, iFoundAll = 0;
	void Find();
	
	bool Play(bool set);
	void Save();
	void LoadState(), SaveState();  // set.st

	//  File Operations
	void OpenDirFile(bool files, EInsert where = Ins_End);
	int RenameRate(bool allPls=false);
	void OpenBrowserUrl(std::string url);

	
	///  Input
	//------------------------------------------------
	//  Keys
	bool alt = false, ctrl = false, shift = false;
	void UpdModifiers();
	void KeyPress(sf::Event::KeyEvent key);
	
	std::unique_ptr<ActionsMap> act;
	
	virtual void UpdateView(bool load, int v) = 0;
	void NextVisual();
	void ApplyThemes();  // fill vis clr, by id from xml

	virtual void WndOpen(EWndOpt type, bool center = true) = 0;


	///  Mouse  pos
	//------------------------------------------------
	int xMpos=0,yMpos=0, xMold=0,yMold=0;  //, xWclick,yWclick
	int xm=0,ym=0, xms=0,yms=0;  // pos,old
	
	//  mmb vars
	int xMs=0,yMs=0, yM_visH=0, xL_ofs=0, yL_sl=0;
	float mti=0.f, mtiv=0.f;

	//  buttons, s = old state
	bool bLs=false,bL=false, bRs=false,bR=false, bMs=false,bM=false;
	bool bL_sl=false;

	
	void MouseUpdate(), MouseUpdateEnd();  // process, update
	void WheelMove(int d);

	void MouseMove(int x, int y)  // events
	{	xMpos = x;  yMpos = y;  }
	void MouseDown(int b);
	void MouseUp(int b);

	
	int iDraw = 2;  // clean and redraw, when needed
	void Redraw() {  iDraw = 2;  }
	
	
	//  Toggle
	//------------------------------------------------
	float dt = 0.1f;   // frame time, ms
	bool bFps = false;   // show Fps
	bool bDebug = false; // show debug text
	/*ETimeTest*/int eTimeTest = TimT_Off;   // test pls time colors

	bool bAllStats = false;  // all playlists
	bool bFullStats = false;  // unfiltered
	

	//  Osd  ----
	//  short info msg on player
	std::string osdStr = "";
	float dtOsd = 0.f, dtOsdShow = 1.f;
	void Osd(std::string msg);
	
	
	///  Dimensions
	//------------------------------------------------
	//  B = begin  E = end  W = width  H = height
	int yB_inf=0,  // file info
		yE_plr_btn=0,  // player buttons |< >|
		
		yB_vis=0,yE_vis=0,  // visualization
		yB_pos=0,yE_pos=0;  // seek pos bar
	float xW_pos = 0.03f;  // seek marker
		
		//  pls tabs, size
	int yB_tabs=0,yE_tabs=0,
		xW_tabs=0,yH_tabs=0,
		xW_tabs_btn=0,  // tabs btns ^ v
		
		//  pls header
		yB_pl_inf=0, xE_pl_inf=0,  // info
		xM_pl_filt=0,  // filter / * middle
		//  pls tracks
		yB_pl=0,yE_pl=0, yH_pl=0,
		yL_pl = 10,  // playlist view lines, auto set
		xWex_plS=0;  // extra width for slider drag
	
	void UpdDim();
	// todo:? struct Area xB,yB..  class Dimensions

	
protected:  //  actions for key binds
	//------------------------------------------------------------------------------------------------
	friend class ActionsMap;
	
	void
	PlaySetCur(), Play(), Pause(), Stop(), Prev(), Next(),
	NextPls(), PrevPls(),
	
	Backward(), BackwardSlow(), BackwardFast(),
	Forward(), ForwardSlow(), ForwardFast(),

	VolumeUp(), VolumeUpSlow(), VolumeUpFast(),
	VolumeDown(), VolumeDownSlow(), VolumeDownFast(),
	
	GotoPlay(), GotoPlaySetPls(),
	Unselect(),

	PlsUp(), PlsUpBy8(), PlsDown(), PlsDownBy8(),
	PlsPageOfsUp  (),PlsPageOfsUp4  (),PlsPageUp4  (),PlsPageUp  (),
	PlsPageOfsDown(),PlsPageOfsDown4(),PlsPageDown4(),PlsPageDown(),
	PlsListHome(),PlsDirUp  (),PlsHome(),PlsHome4(),
	PlsListEnd (),PlsDirDown(),PlsEnd (),PlsEnd4 (),

	RateInc(), RateIncCur(), RateDec(), RateDecCur(),
	FilterLowUp(), FilterHighUp(), FilterLowDown(), FilterHighDown(),
	
	BookmInc(), BookmDec(), BookmIncTab(), BookmDecTab(),

	TabNext(), TabPrev(), TabNextRow(), TabPrevRow(), 
	TabOfsNext(), TabOfsPrev(), TabOfsNextRow(), TabOfsPrevRow(), 

	TabNew(), TabNewBegin(), TabNewEnd(), TabNewBefore(), TabClose(),
	
	RepeatTrack(), RepeatPls(),
	Fps(), Debug(), Times(),
	
	AllStats(), FullStats(), NextVis(), DirViewNext(), FileInfo(),

	FindHide(), FindOpen(),
	DuplicateCur(),
	HideCur(), ShowCur(),
		
	SaveAll(), ReloadPls(),
	
	InsertDir(), InsertFiles(),
	ClearPls(), DeleteCurFile(), DeleteCur(),

	SetTabAsQue(), AddTrkToQue(),
	
	//  ----
	GUIMain(), GUIAppAudio(), GUIAppKeys(),	GUIAppAbout(), GUIAppHelp(),
	GUIView(), GUIViewFonts(), GUIVis(), GUIVisThemes(), GUIViewTimes(),
	GUITab(), GUITabsAll(), GUIPlsFind(), GUIPlsFilter(), GUIAppStats(), GUIAppTest(),
	
	SaveCurView(),
	LoadCurView0(),	LoadCurView1(),	LoadCurView2(),	LoadCurView3(),	LoadCurView4(),
	LoadCurView5(),	LoadCurView6(),	LoadCurView7(),	LoadCurView8(),	LoadCurView9(),
	LoadCurView10(), LoadCurView11(), LoadCurView12(), LoadCurView13(), LoadCurView14(),
	LoadCurView15(), LoadCurView16(), LoadCurView17(), LoadCurView18(), LoadCurView19();
};
