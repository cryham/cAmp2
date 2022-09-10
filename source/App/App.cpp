#include "App.h"
#include "AppActions.h"
#include "../Audio/AudioBass.h"
#include "../System/FileSystem.h"
#include "../../libs/nfd.h"
#include <memory>
#include <fstream>
#include <iostream>
using namespace std;  using namespace sf;


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


//  Osd
//------------------------------------------------------------------------
void App::Osd(std::string msg)
{
	osdStr = msg;
	dtOsd = dtOsdShow;
}


//  Insert dir or files, add to Pls
//------------------------------------------------------------------------
void App::OpenDirFile(bool files, EInsert where/*, defaultPath=NULL par?*/)
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
			Pls().UpdateVis(0, true);
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


//  Queue
//------------------------------------------------------------------------
void App::AddTrkToQue()
{
	if (queId >= vPls.size())
	{	queId = -1;  Osd("Queue outside, reset.");  return;  }
	if (queId < 0){  Osd("Queue tab not set.");  return;  }

	// Pls().vSel
	
	Pls().InsertCurToPls(vPls[queId], Ins_End);
	Osd("Added to Queue at end.");
	Redraw();
}
