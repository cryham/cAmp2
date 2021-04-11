#include "Playlist.h"
#include "../System/FileSystem.h"
#include "../Audio/Audio.h"
#include "../System/Stats.h"
#include "../System/defines.h"
#include "../System/Rating.h"
#include "../System/Utilities.h"
#include "../../libs/imgui.h"  // ColorConvert
#include <vector>
#include <iostream>
using namespace std;


Audio* Playlist::audio = nullptr;

Playlist::Playlist(string name1)
{
	name = name1;
}

//  Play
//--------------------------------------------------------------------
void Playlist::GotoPlay()
{
	iCur = iPlayVis;
	iOfs = iCur - iLinVis/2;
	Ofs();  Cur();
}

bool Playlist::Play(bool set)
{
	if (IsEmpty())  return false;
	
	auto old = iPlay;
	if (set)  iPlay = GetTrackVisId(iCur);
	if (iPlay < 0 || iPlay >= LengthAll())
		return false;  //Check()

	Track& t = GetTrackAll(iPlay);
	if (t.IsDir())
	{	iPlay = old;  return false;  }
	UpdPlayVis();

	audio->SetPls(this);
	return audio->Play(t);
}

//  Next
//--------------------------------------------------------------------
bool Playlist::Next(int add)
{
	if (IsEmpty())  return false;

	audio->bNextPrev = add > 0;
	bool found = false;
	int adds = 0;
	int p = iPlay, last = LengthAll()-1;
	while (!found && adds < LengthAll())
	{
		++adds;  p += add;
		if (add > 0)
		{
			if (p > last)
				if (!audio->bRepPls)  return false;  // no repeat
				else  p = 0;
		}else{
			if (p < 0)
				if (!audio->bRepPls)  return false;
				else  p = last;
		}
		///  skip while  dirs or disabled or not visible etc..
		const auto& t = tracks[p];
		if (!t.IsDir() && t.visible && !t.IsDisabled() &&
			t.GetHide() != Hid_Hide)
			found = true;
	}
	if (!found)  return false;  // looped all, none playabe
	iPlay = p;
	//UpdPlay();
	
	//bDraw = true;
	return Play();
}


//  Add Dir
//--------------------------------------------------------------------
bool Playlist::AddDir(fs::path dir, bool recursive, const EInsert& where)
{
	auto files = FileSystem::ListDir(dir, recursive);
	for (const auto& file: files)
	{
		AddFile(file, where);
	}
	UpdateVis();

	return true;
}

bool Playlist::AddFile(fs::path file, const EInsert &where)
{
	if (fs::is_directory(file))
		return false;

	//  get ext
	string ext = file.extension();
	if (ext.length() < 2)
		return false;
	ext = file.extension();
	ext = ext.substr(1);  // no .
	strupper(ext);
	
	//  skip if unplayable
	if (audio != nullptr &&
		!audio->IsPlayable(ext))
		return false;

	Track t(file, false);
	audio->GetTrackTime(t);  // todo: on thread ..
	switch (where)
	{
	case Ins_Cursor:
		//tracks.insert(cur,t);  // insert(vec
		break;
	case Ins_Top:
		tracks.emplace_front(move(t));
	case Ins_End:
		tracks.emplace_back(move(t));
	}
	return true;
}


//  update color
//--------------------------------------------------------------------
void Playlist::UpdateColor()
{
	float r=0, g=0, b=0;

	ImGui::ColorConvertHSVtoRGB(
		hue, sat, val, r, g, b);
	bck[0] = r*255.f;  bck[1] = g*255.f;  bck[2] = b*255.f;

	ImGui::ColorConvertHSVtoRGB(  // par 2__
		hue, max(0.f, sat-0.3f), min(1.f, val+0.2f), r, g, b);
	txt[0] = r*255.f;  txt[1] = g*255.f;  txt[2] = b*255.f;
}


//  change  bookmark, rating, filter
//--------------------------------------------------------------------
void Playlist::Bookm(bool pls, int add)
{
	if (pls)
	{	//  playlist
		bookm += add;  Bookmarks::Range(bookm);
	}else
	{	//  track
		auto& t = GetTrackVis(iCur);
		t.bookm += add;  Bookmarks::Range(t.bookm);
	}
}

void Playlist::Rate(bool playing, int add)
{
	auto& t = playing ? GetTrackAll(iPlay) : GetTrackVis(iCur);
	int& r = t.rate;
	r += add;  Ratings::Range(r);
	bDraw = true;
}

void Playlist::Filter(bool lower, int add)
{
	int& f = lower ? filterLow : filterHigh;
	f += add;  Ratings::Range(f);
	UpdateVis();
}
