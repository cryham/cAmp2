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

	
	//  pls
	pls = make_unique<Playlist>();
	pls->AddDir("../../../zm", false);
	audio->SetPls(pls.get());
#if 0
	const auto& tracks = pls->GetTracks();
	Log("---- tracks");
	for (const auto& trk: tracks)
		Log(trk.GetName());
#endif

	// todo: on thread ..
	//pls->Update();
	for (auto& trk: pls->GetTracks())
		audio->GetTrkTime(trk);

	return true;
}


//  Destroy
bool App::Destroy()
{
	//  set
	bool ok = set.Save();

	//  audio
	audio->Destroy();
	
	return ok;
}


//  Update dims
//------------------------------------------------------------------------
void App::UpdDim(/*float rfrFq*/)
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
	//CList::Lin = yL_pl;

	//if (rfrFq > 0.f)
	//	tmD = int(rfrFq*1.f);	clear=1;
	
	// for..
	//cfont[v.cfP]->StrCopy(" 0:00");  xt_ = cfont[v.cfP]->GetWidth();
	bDraw = true;
}
