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
	view.xSize = set.xwSize;
	view.ySize = set.ywSize;
	view.visH = 96;  // par
	
	//Graphics::SetupStates(pDev);
	if (view.visH > view.ySize)  view.visH = view.ySize;  //vis max
	int Fy = view.Fy;

	//  file info
	yB_fi = 0;	yE_pl_btn = 20;  // btnsH |< >|

	//  visualization
	yB_vis = 16;  //yFvi = 64;
	yE_vis = min(view.ySize, 28 + ((view.eVis!=viNone)? view.visH: 0) );
	
	//  pos bar
	yB_pos = yE_vis;  yE_pos = yB_pos +9;
	xW_pos = 0.03f;

	//  pt = playlist tabs
	yB_pt = yE_pos;  /*par+- +2 8pos*/
	xW_pt_btn = 13;  // btnsW up,dn
		xW_pt = (view.xSize - xW_pt_btn) / view.xNpt;
		yH_pt = /*cfont[view.cfT]->*/Fy +2;
	yE_pt = yB_pt + view.yNpt * yH_pt +4;

	//  pl = playlist, tracks
	yB_pli = yE_pt;
	yB_pl = yB_pli + /*cfont[view.cfP]->*/Fy +2/*yHpli*/;
	yE_pl = view.ySize - /*cfont[view.cfP]->*/Fy;
	
	yH_pl = yE_pl-1-yB_pl;  yL_pl = max(0, yH_pl / /*cfont[view.cfP]->*/Fy );
		//delete[] Lxm;  Lxm = new int[yL_pl+4];
	yE_pl = yL_pl * /*cfont[view.cfP]->*/Fy + yB_pl;
	yH_pl = yE_pl-1-yB_pl;

	/*xWplS = 14;*/  xW_plSm = 40;  //|sliderW, mW
	xTm = view.xSize - view.xW_plS-3;  //|
	//CList::Lin = yL_pl;

	//if (rfrFq > 0.f)
	//	tmD = int(rfrFq*1.f);	clear=1;
	
	// for..
	//cfont[view.cfP]->StrCopy(" 0:00");  xt_ = cfont[view.cfP]->GetWidth();

	//  gui checks pos
	xB_gc = 106;  yB_gc = yB_pt+90;  xW_gc = 18;
	xB_gck = 170;  yH_gc = /*cfont[view.cfG]->*/Fy+6; //[0
	
	//GuiReInit();
}
