#include "App.h"
#include "AudioBass.h"
#include "FileSystem.h"
#include "../libs/nfd.h"
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

App::App()
{
}

App::~App()
{
}

//  apply set State to player
void App::LoadState()
{
	audio->iVolume = set.state.iVolume;
	plsId = set.state.idPls;
	plsPlId = set.state.idPlay;
	if (set.state.bPlay && plsPlId < vPls.size())
		vPls[plsPlId].Play(false);
}

void App::SaveState()
{
	set.state.bPlay = audio->IsPlaying();
	set.state.iVolume = audio->iVolume;
	set.state.idPls = plsId;
	set.state.idPlay = plsPlId;
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
		all += p.stats;  allFull += p.stAll;
		vPls.emplace_back(move(p));
	}
	//Pls().Update();
	// todo: on thread after AddDir..
	//for (auto& trk: Pls().GetTracks())
	//	audio->GetTrkTime(trk);
	
	if (vPls.empty())
		vPls.emplace_back(Playlist("1"));
}

void App::SavePls()
{
	set.vSetPls.clear();
	for (const auto& p: vPls)
	{
		SetPls s;
		s.name = p.name;
		s.bookm = p.bookm;
		set.vSetPls.emplace_back(move(s));
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
	yE_plr_btn = 30;  // btnsH |< >|

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


//  Osd
//------------------------------------------------------------------------
void App::Osd(std::string msg)
{
	osdStr = msg;
	dtOsd = dtOsdShow;
}


//  Insert dir or files, add to Pls
//------------------------------------------------------------------------
void App::OpenDirFile(bool files, Playlist::EInsert where/*, defaultPath=NULL par?*/)
{
	if (files)
	{
		auto exts = audio->GetAllExtStr();
		nfdpathset_t paths;
		auto r = NFD_OpenDialogMultiple(exts, NULL, &paths);
		if (r != NFD_OKAY)  return;

		size_t cnt = NFD_PathSet_GetCount(&paths);
		for (size_t i=0; i < paths.count; ++i)
		{
			char* file = NFD_PathSet_GetPath(&paths, i);
			Pls().AddFile(file, where);
			Pls().Update();
		}
		NFD_PathSet_Free(&paths);
	}
	else
	{
		char* outPath = 0;
		auto r = NFD_PickFolder(NULL, &outPath);
		if (r != NFD_OKAY)  return;

		Pls().AddDir(outPath, where);
		free(outPath);
	}
}

//  Find
//------------------------------------------------------------------------
void App::Find()
{
	if (set.find.bAllPls)
		for (auto& pls : vPls)
			pls.Find(sFind, set.find);
	else
		Pls().Find(sFind, set.find);
	Redraw();
}
