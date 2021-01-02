#include "App.h"
#include "AudioBass.h"
#include "FileSystem.h"
#include <memory>
#include <fstream>
#include <iostream>
using namespace std;  using namespace sf;


App::App()
{
}

App::~App()
{
}


//  Init
bool App::Init()
{
	FileSystem::Init();

	//  log
	AppLog::Init();
	
	Log(FileSystem::GetInfo());
	
	
	//  set
	if (!set.Load())
		return false;
	
	timeColors.Load();
	
	
	//  audio
	audio = make_unique<AudioBass>();
	audio->Init();

	Playlist::audio = audio.get();

	audio->SetPls(&Pls());

	LoadPls();
	
	LoadState();
	
	return true;
}

//  apply set State to player
void App::LoadState()
{
	audio->iVolume = set.st.iVolume;
	plsId = set.st.idPls;
	plsPlId = set.st.idPlay;
	if (set.st.bPlay && plsPlId < vPls.size())
		vPls[plsPlId].Play(false);
}

void App::SaveState()
{
	set.st.bPlay = audio->IsPlaying();
	set.st.iVolume = audio->iVolume;
	set.st.idPls = plsId;
	set.st.idPlay = plsPlId;
}

//  Playlists
void App::LoadPls()
{
	all.Clear();
	allFull.Clear();
	vPls.clear();
	
	for (const auto& sp: set.vSetPls)
	{
		Playlist p(sp.name);
		p.bookm = sp.bookm;
		p.Load();
		all += p.stats;  allFull += p.stFull;
		vPls.push_back(move(p));
	}
	//Pls().Update();
	// todo: on thread after AddDir..
	//for (auto& trk: Pls().GetTracks())
	//	audio->GetTrkTime(trk);
	
	if (vPls.empty())
		vPls.push_back(Playlist("1"));
}

void App::SavePls()
{
	set.vSetPls.clear();
	for (const auto& p: vPls)
	{
		SetPls s;
		s.name = p.name;
		s.bookm = p.bookm;
		set.vSetPls.push_back(move(s));
	}
}

//  Destroy
bool App::Destroy()
{
	//  set
	SavePls();
	bool ok = set.Save();

	//  audio
	audio->Destroy();
	
	return ok;
}


//  Update dims
//------------------------------------------------------------------------
void App::UpdDim()
{
	ViewSet& v = set.view;
	if (v.iVisH > v.ySize)
		v.iVisH = v.ySize;  //vis max

	//  file info
	yB_inf = 0;
	yE_plr_btn = 20;  // btnsH |< >|

	//  visualization
	bool noVis = v.eVis == viNone;
	yB_vis = 16;  //yFvi = 64;
	yE_vis = min(v.ySize, 32 + (noVis ? 0 : v.iVisH) );
	
	//  pos bar
	yB_pos = yE_vis;  yE_pos = yB_pos + 9;
	xW_pos = 0.03f;

	//  playlist tabs
	yB_tabs = yE_pos;  /*par+- +2 8pos*/
	xW_tabs_btn = 13;  // btnsW up,dn
		xW_tabs = (v.xSize - xW_tabs_btn) / v.xNpt;
		yH_tabs = /*cfont[v.cfT]->*/v.Fy + 2;
	yE_tabs = yB_tabs + v.yNpt * yH_tabs + 4;

	//  playlist, tracks
	yB_pl_inf = yE_tabs;
	yB_pl = yB_pl_inf + /*cfont[v.cfP]->*/v.Fy + 2/*yHpli*/;
	yE_pl = v.ySize - /*cfont[v.cfP]->*/v.Fy;
	
	yH_pl = yE_pl-1-yB_pl;
	yL_pl = max(0, yH_pl / /*cfont[v.cfP]->*/v.Fy );  // lines
	yE_pl = yL_pl * /*cfont[v.cfP]->*/v.Fy + yB_pl;
	yH_pl = yE_pl-1-yB_pl;

	xWex_plS = 40;  // slider extra
	xE_pl_inf = v.xSize-21;

	Redraw();
}
