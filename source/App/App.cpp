#include "App.h"
#include "AppActions.h"
#include "../Audio/AudioBass.h"
#include "../System/FileSystem.h"
#include "../../libs/nfd.h"
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


//  key command utils
//------------------------------------------------------------------------
bool App::Play(bool set)
{
	bool b = Pls().Play(set);
	if (b)
		plsPlId = plsId;
	return b;
}

void App::NextVisual()
{
	auto& v = set.view.vis.eType;
	++v;
	if (v >= VisT_ALL)
		v = VisT_None;
	UpdDim();
}


//  apply set State to player
//------------------------------------------------------------------------
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
		all += p.stats;  allFull += p.stAll;
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
	yE_plr_btn = 30;  // btnsH |< >|

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
			Pls().UpdateVis();
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
	iFoundAll = 0;
	if (set.find.bAllPls)
		for (auto& pls : vPls)
		{
			pls.Find(sFind, set.find);
			iFoundAll += pls.GetFound();
		}
	else
	{	Pls().Find(sFind, set.find);
		iFoundAll += Pls().GetFound();
	}
	bFind = true;
	Redraw();
}


//  Rename
//------------------------------------------------------------------------
int App::RenameRate(bool allPls)
{
	int ntab = vPls.size();
	int all = 0;
	if (allPls)
		for (int i=0; i < ntab; ++i)
			all += vPls[i].RenameRate(i == plsPlId && audio->IsPlaying());
	else
		all += Pls().RenameRate(plsId == plsPlId && audio->IsPlaying());
	return all;
}


//  util
void App::OpenBrowserUrl(std::string url)
{
#ifdef WIN32
	string cmd = "system ";
#else
	string cmd = "xdg-open ";
#endif
	string s = cmd + url;
	system(s.c_str());
}
