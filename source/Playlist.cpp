#include <vector>
#include "FileSystem.h"
#include "Playlist.h"
#include "Audio.h"
#include "Stats.h"
#include "def.h"
#include "Util.h"
#include "../libs/imgui.h"  // ColorConvert
#include <iostream>
using namespace std;


Audio* Playlist::audio = nullptr;

Playlist::Playlist()
{
}
Playlist::Playlist(string name1)
	:Playlist()
{
	name = name1;
}


//  play
//--------------------------------------------------------------------
void Playlist::GotoPlay()
{
	iCur = iPlayVis;  //!
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

bool Playlist::Next(int add)
{
	if (IsEmpty())  return false;

	audio->bNextPrev = add > 0;
	bool dn = false;
	int adds = 0;
	int p = iPlay, last = LengthAll()-1;
	while (!dn && adds < LengthAll())
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
		//  skip while dirs or disabled or not visible..
		if (!tracksAll[p].IsDir() && //
			tracksAll[p].vis &&
			!tracksAll[p].IsDisabled())
			dn = true;
	}
	if (!dn)  return false;  // looped all, none playabe
	iPlay = p;
	//UpdPlay();
	
	//bDraw = true;
	return Play();
}


//--------------------------------------------------------------------
//  Update Visible
//  Fill tracksVis Ids from tracksAll,
//  filters rating, adds dirs etc.
//--------------------------------------------------------------------
void Playlist::UpdateVis()
{
	bool emptyDirs = false;

	int iZoomNew = 0,
		iZoomOld = 0,
		//zoomTo = lin/2,   // zoom to middle
		zoomTo = iCur - iOfs,  // zoom to cursor
		iAll = LengthAll(), iVis = LengthVis();
	bool sh = iAll < iLinVis || iVis == 0;  // all visible, no scroll
	if (!sh)
		iZoomOld = GetTrackVisIdAll(min(iVis-1, iOfs + zoomTo));  // even if dir

	tracksVis.clear();
	tracksDirs.clear();
	stats.Clear();
	fs::path prev;
	//playVisOut = true;
	
	for (int i=0; i < iAll; ++i)
	{
		Track& t = tracksAll[i];
		int iVis = LengthVis();

		if (i == iZoomOld)
			iZoomNew = iVis;

		bool out = t.rate < filterLow || t.rate > filterHigh;
		if (i == iPlay)
		{
			iPlayVis = iVis;  // marks closest if out
			bPlayVisOut = out;
		}
		t.idPlayVis = iVis;
		t.vis = !out;

		if (out && !emptyDirs)
			continue;
		
		fs::path path = t.path.parent_path();
		//  todo: dir rate, dir hide  map[path]..
		if (path != prev)
		{
			//  Add Dir  +++
			Track d(t.path.parent_path(), true);
			tracksDirs.emplace_back(move(d));
			
			VisId id;  id.dir = true;  id.i = tracksDirs.size()-1;
			id.iAll = i;  // closest track
			tracksVis.emplace_back(move(id));

			t.idPlayVis = iVis+1;
			
			stats.AddDir();
		}
		prev = path;
		
		if (out)
			continue;
		
		//  Add File  +++
		VisId id;  id.iAll = id.i = i;
		tracksVis.emplace_back(id);
		
		stats.Add(&t);
	}
		
	iCur = iZoomNew;
	//  adjust ofs
	if (sh)
		iOfs = 0;
	else
		iOfs = iZoomNew - zoomTo;
		
	int all = LengthVis();
	iCur = mia(0, all, iCur);
	if (iOfs > all-iLinVis)  iOfs = all-iLinVis;  //  no view past last track
	if (iOfs < 0)  iOfs = 0;  //  cur stays in view
	bDraw = true;
	//Cur();  Ofs();
}

void Playlist::UpdPlayVis()
{
	iPlayVis    = tracksAll[iPlay].idPlayVis;
	bPlayVisOut = !tracksAll[iPlay].vis;
	bDraw = true;
}


//  add dir
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
		tracksAll.emplace_front(move(t));
	case Ins_End:
		tracksAll.emplace_back(move(t));
	}
	return true;
}


//  list move (keys)
//--------------------------------------------------------------------
void Playlist::Cur()
{
	int all = LengthVis()-1;
	iCur = mia(0, all, iCur);
	bDraw = true; 
}
void Playlist::Ofs()
{
	int all = LengthVis();
	if (iOfs > all-iLinVis)  iOfs = all-iLinVis;  //  no view past last track
	if (iOfs < 0)  iOfs = 0;  //  cur stays in view
	bDraw = true;
}
void Playlist::Up		(int m){  iCur -= m;  Cur();  int d= iCur-iOfs;            if (d < 0) {  iOfs += d;  Ofs();  }  }
void Playlist::Dn		(int m){  iCur += m;  Cur();  int d= iCur-iOfs-iLinVis+1;  if (d > 0) {  iOfs += d;  Ofs();  }  }

void Playlist::PgOfsUp	(int m){  iOfs -= m;  Ofs();  int d= iCur-iOfs-iLinVis+1;  if (d > 0) {  iCur -= d;  Cur();  }  }
void Playlist::PgOfsDn	(int m){  iOfs += m;  Ofs();  int d= iCur-iOfs;            if (d < 0) {  iCur -= d;  Cur();  }  }

void Playlist::PgUp		(int m){  iCur -= m;  iOfs -= m;  Cur();  Ofs();  }
void Playlist::PgDn		(int m){  iCur += m;  iOfs += m;  Cur();  Ofs();  }

void Playlist::Home (int m)
{
	switch(m)
	{
	case 2:  iCur = 0;  iOfs = 0;  break;  // list top
	case 1:  iCur = iOfs;  break;  // view

	case 0:
		do  --iCur;
		while (iCur-1 > 0 && !tracksVis[iCur].dir);
		Up(0);  break;
	}
	bDraw = true;
}

void Playlist::End (int m)
{
	int all = LengthVis();
	switch(m)
	{
	case 2:  iCur = all-1;  iOfs = all-1;  Cur();  Ofs();  break;  // list end
	case 1:  iCur = iOfs+iLinVis-1;  Cur();  break;  // view

	case 0:
		do  ++iCur;
		while (iCur+1 < all && !tracksVis[iCur].dir);
		Dn(0);  break;
	}
	bDraw = true;
}


//  advanced  bookmark, rating, filter
//--------------------------------------------------------------------
void Playlist::Bookm(bool pls, char add)
{
	if (pls)
	{	//  playlist
		bookm += add;  bookm = mia(0,6, bookm);
	}else
	{	//  track
		auto& t = GetTrackVis(iCur);
		char& b = t.bookm;
		b += add;  b = mia(char(0),char(6), b);
	}
}

void Playlist::Rate(bool playing, char add)
{
	auto& t = playing ? GetTrackAll(iPlay) : GetTrackVis(iCur);
	char& r = t.rate;
	r += add;  r = mia(char(cRateMin),char(cRateMax), r);
	bDraw = true;
}

void Playlist::Filter(bool lower, char add)
{
	int& f = lower ? filterLow : filterHigh;
	f += add;  f = mia(cRateMin,cRateMax, f);
	UpdateVis();
}


//  Find  clear marks
//--------------------------------------------------------------------
void Playlist::FindClear()
{
	for (auto& trk : tracksAll)
		trk.found = false;
	iFound = 0;
	bDraw = true;
}

//  Find
//--------------------------------------------------------------------
void Playlist::Find(std::string& find, const SetFind& opt)
{
	if (find.empty())
	{
		FindClear();
		return;
	}
	bool low = !opt.bCaseSens;
	if (low)
		strlower(find);
	iFound = 0;
		
	if (!low)
	{	for (auto& trk : tracksAll)
		if (opt.bUnfiltered || trk.vis)
		{
			auto& name = opt.bFullPath ? trk.GetPath() : trk.GetName();
			bool f = name.find(find) != string::npos;
			if (f)  ++iFound;
			trk.found = f;
		}
	}else
	{	for (auto& trk : tracksAll)
		if (opt.bUnfiltered || trk.vis)
		{
			auto name = opt.bFullPath ? trk.GetPath() : trk.GetName();
			strlower(name);
			bool f = name.find(find) != string::npos;
			if (f)  ++iFound;
			trk.found = f;
		}
	}
	bDraw = true;
}

//  pls clr
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
