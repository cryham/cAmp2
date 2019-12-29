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
//	v.xSize = set.xwSize;
//	view.ySize = set.ywSize;
//	view.visH = set.visH;  // par
	
	//Graphics::SetupStates(pDev);
	ViewSet& v = set.view;
	if (v.visH > v.ySize)
		v.visH = v.ySize;  //vis max
	int Fy = v.Fy;

	//  file info
	yB_fi = 0;	yE_pl_btn = 20;  // btnsH |< >|

	//  visualization
	yB_vis = 16;  //yFvi = 64;
	yE_vis = min(v.ySize, 28 + ((v.eVis!=viNone)? v.visH: 0) );
	
	//  pos bar
	yB_pos = yE_vis;  yE_pos = yB_pos + 9;
	xW_pos = 0.03f;

	//  pt = playlist tabs
	yB_pt = yE_pos;  /*par+- +2 8pos*/
	xW_pt_btn = 13;  // btnsW up,dn
		xW_pt = (v.xSize - xW_pt_btn) / v.xNpt;
		yH_pt = /*cfont[v.cfT]->*/Fy + 2;
	yE_pt = yB_pt + v.yNpt * yH_pt + 4;

	//  pl = playlist, tracks
	yB_pli = yE_pt;
	yB_pl = yB_pli + /*cfont[v.cfP]->*/Fy + 2/*yHpli*/;
	yE_pl = v.ySize - /*cfont[v.cfP]->*/Fy;
	
	yH_pl = yE_pl-1-yB_pl;  yL_pl = max(0, yH_pl / /*cfont[v.cfP]->*/Fy );
	yE_pl = yL_pl * /*cfont[v.cfP]->*/Fy + yB_pl;
	yH_pl = yE_pl-1-yB_pl;

	/*xWplS = 14;*/  xW_plSm = 40;  //|sliderW, mW
	xTm = v.xSize - v.xW_plS-3;  //|
	//CList::Lin = yL_pl;

	//if (rfrFq > 0.f)
	//	tmD = int(rfrFq*1.f);	clear=1;
	
	// for..
	//cfont[v.cfP]->StrCopy(" 0:00");  xt_ = cfont[v.cfP]->GetWidth();

	//  gui checks pos
	xB_gc = 106;  yB_gc = yB_pt+90;  xW_gc = 18;
	xB_gck = 170;  yH_gc = /*cfont[v.cfG]->*/Fy+6; //[0
	
	//GuiReInit();
}
