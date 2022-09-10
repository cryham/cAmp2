#include "App.h"
#include "AppActions.h"
#include "../Audio/AudioBass.h"
#include "../System/FileSystem.h"
#include <memory>
#include <fstream>
#include <iostream>
using namespace std;  using namespace sf;


//  Init
//------------------------------------------------------------------------
bool App::Init()
{
	FileSystem::Init();

	//  log
	LogFile::Init();

	Log(FileSystem::GetInfo());
	
	
	//  audio
	audio = make_unique<AudioBass>();
	audio->Init();

	Playlist::audio = audio.get();


	//  set
	if (!set.Load())
	{
		//LoadPls();
		//return false;
	}
	
	themes.Load();
	ApplyThemes();
	

	//  keys
	if (!act->Load())
		act->DefaultBindings();
	

	//  pls
	LoadPls();
	audio->SetPls(&Pls());
	
	LoadState();
	
	return true;
}


App::App()
{
	act = make_unique<ActionsMap>(this);
}

App::~App()
{
}


//  copy cur theme names to view vis
void App::ApplyThemes()
{
	auto& v = set.view.vis;
	themes.curFFT = themes.GetVisId(v.fft.theme);
	themes.curOsc = themes.GetVisId(v.osc.theme);
	themes.curSpect = themes.GetVisId(v.spect.theme);

	v.fft.clr = themes.CurFFT();
	v.osc.clr = themes.CurOsc();
	v.spect.clr = themes.CurSpect();
	
	themes.curFonts = themes.GetFontId(set.view.fontSet);
	set.view.fnt = themes.CurFont();
}


//  apply set State to player
//------------------------------------------------------------------------
void App::LoadState()
{
	const auto& st = set.state;
	plsId = st.idPls;
	plsPlId = st.idPlay;
	queId = st.idQue;

	audio->bRepPls = st.bRepPls;
	audio->bDirNext = st.bDirNext;
	
	audio->iVolume = st.iVolume;
	if (st.bPlay && plsPlId < vPls.size())
		vPls[plsPlId].Play(false);
}

void App::SaveState()
{
	auto& st = set.state;
	st.idPls = plsId;
	st.idPlay = plsPlId;
	st.idQue = queId;
	
	st.bRepPls = audio->bRepPls;
	st.bDirNext = audio->bDirNext;

	st.iVolume = audio->iVolume;
	st.bPlay = audio->IsPlaying();
}


//  Playlists
//------------------------------------------------------------------------
void App::LoadPls()
{
	all.Clear();
	allFull.Clear();
	vPls.clear();
	
	for (const auto& sp: set.vSetPls)
	{
		Playlist p(sp);
		p.Load();
		all += p.stats;  allFull += p.statsAll;
		vPls.emplace_back(move(p));
	}
	//Pls().Update();
	// todo: on thread after AddDir..
	//for (auto& trk: Pls().GetTracks())
	//	audio->GetTrackTime(trk);
	
	if (vPls.empty())
		vPls.emplace_back(Playlist("1"));
}

void App::SavePls()
{
	set.vSetPls.clear();
	for (const auto& p: vPls)
	{
		set.vSetPls.emplace_back(p.name);
	}
}

//  Destroy
bool App::Destroy()
{
	//  set save
	SavePls();
	bool ok = set.Save();
	themes.Save();  //

	//  audio
	audio->Destroy();
	
	return ok;
}


//  Update dims, layout
//------------------------------------------------------------------------
void App::UpdDim()
{
	ViewSet& v = set.view;
	if (v.vis.yH > v.wnd.ySize)
		v.vis.yH = v.wnd.ySize;  //vis max

	//  file info
	yB_inf = 0;
	yE_plr_btn = 60;  // btnsH |< >|

	//  visualization
	bool noVis = v.vis.eType == VisT_None;
	yB_vis = v.FontH(Fnt_TimeBig) - 3;
	yE_vis = min(v.wnd.ySize,
		yB_vis + (noVis ? v.FontH(Fnt_Player) + 5 : v.vis.yH) );
	
	//  pos bar
	yB_pos = yE_vis;  yE_pos = yB_pos + 11;  //par
	xW_pos = 0.03f;

	//  playlist tabs
	yB_tabs = yE_pos;  /*par+- +2 8pos*/
	xW_tabs_btn = 16;  // btnsW up,dn
		xW_tabs = (v.wnd.xSize - xW_tabs_btn) / v.tabs.xCols;
		yH_tabs = v.FontH(Fnt_Tabs) + 2;
	yE_tabs = yB_tabs + v.tabs.yRows * yH_tabs + 4;

	//  playlist, tracks
	int yL = v.FontH(Fnt_Tracks);
	yB_pl_inf = yE_tabs;  // header
	yB_pl = yB_pl_inf + v.FontH(Fnt_Player) + 2;
	yE_pl = v.wnd.ySize - yL;
	
	yH_pl = yE_pl-1-yB_pl;
	yL_pl = max(0, yH_pl / yL );  // lines
	yE_pl = yL_pl * yL + yB_pl;
	yH_pl = yE_pl-1-yB_pl;

	xWex_plS = 40;  // slider extra
	xE_pl_inf = v.wnd.xSize-21;
	xM_pl_filt = xE_pl_inf*11/20;

	Redraw();
}
