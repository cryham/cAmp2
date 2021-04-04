#include <vector>
#include "FileSystem.h"
#include "Playlist.h"
#include "Audio.h"
#include "Stats.h"
#include "def.h"
#include "Util.h"
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
	cur = play;
	ofs = cur - lin/2;
	Ofs();  Cur();
}

bool Playlist::Play(bool set)
{
	if (IsEmpty())  return false;
	
	auto old = play;
	if (set)  play = cur;
	if (play < 0 || play >= tracksVis.size())
		return false;  //Check()
	bDraw = true;

	Track& t = GetTracks()[play];
	if (t.IsDir())
	{	play = old;  return false;  }

	audio->SetPls(this);
	return audio->Play(t);
}

bool Playlist::Next(int add)
{
	if (IsEmpty())  return false;

	audio->bNextPrev = add > 0;
	bool dn = false;
	int adds = 0;
	int p = play, last = Length()-1;
	while (!dn && adds < Length())
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
		if (!tracksVis[p].IsDir() &&
			!tracksVis[p].IsDisabled())
			dn = true;
	}
	if (!dn)  return false;  // looped all, none playabe
	play = p;
	
	bDraw = true;
	return Play();
}


//  update
//--------------------------------------------------------------------
void Playlist::Update()
{
	bool emptyDirs = false;

	int i2v = 0,
		//l2 = lin/2,   // zoom to middle
		zoomTo = cur - ofs,  // zoom to cursor
		iAll = LengthAll(), iVis = (int)tracksVis.size(),
		i2 = 0;
	bool sh = iAll < lin || iVis == 0;  // short pls
	if (!sh)
		i2 = tracksVis[min(iVis-1, ofs + zoomTo)].idAll;

	tracksVis.clear();
	stats.Clear();
	fs::path prev;
	
	for (int i=0; i < iAll; ++i)
	{
		//  id
		if (i == i2)
			i2v = tracksVis.size();

		/*const*/ Track& t = tracksAll[i];
		bool out = t.rate < filterLow || t.rate > filterHigh;
		if (out && !emptyDirs)
			continue;
		
		fs::path path = t.path.parent_path();
		//  todo: dir rate, dir hide  map[path]..
		if (path != prev)
		{	//  add dir
			Track d(t.path.parent_path(), true);
			tracksVis.emplace_back(move(d));
			stats.AddDir();
		}
		prev = path;
		if (out)
			continue;
		
		//  add file
		t.idAll = i;
		tracksVis.emplace_back(t);
		stats.Add(&t);
	}
		
	cur = i2v;
	//  adjust ofs
	if (sh)
		ofs = 0;
	else
		ofs = i2v - zoomTo;
		
	int all = (int)(tracksVis.size());
	cur = mia(0, all, cur);
	if (ofs > all-lin)  ofs = all-lin;  //  no view past last track
	if (ofs < 0)  ofs = 0;  //  cur stays in view
	bDraw = true; 
	//Cur();  Ofs();
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
	Update();

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
	audio->GetTrkTime(t);  // todo: on thread ..
	switch (where)
	{
	case Ins_Cursor:
		//tracks.insert(cur,t);  // insert(vec
		break;
	case Ins_Top:
		tracksAll.push_front(move(t));
	case Ins_End:
		tracksAll.emplace_back(move(t));
	}
	return true;
}


//  list move (keys)
//--------------------------------------------------------------------
void Playlist::Cur()
{
	int all = (int)(tracksVis.size())-1;
	cur = mia(0, all, cur);
	bDraw = true; 
}
void Playlist::Ofs()
{
	int all = (int)(tracksVis.size());
	if (ofs > all-lin)  ofs = all-lin;  //  no view past last track
	if (ofs < 0)  ofs = 0;  //  cur stays in view
	bDraw = true;
}
void Playlist::Up		(int m){  cur -= m;  Cur();  int d= cur-ofs;        if (d < 0) {  ofs += d;  Ofs();  }  }
void Playlist::Dn		(int m){  cur += m;  Cur();  int d= cur-ofs-lin+1;  if (d > 0) {  ofs += d;  Ofs();  }  }

void Playlist::PgOfsUp	(int m){  ofs -= m;  Ofs();  int d= cur-ofs-lin+1;  if (d > 0) {  cur -= d;  Cur();  }  }
void Playlist::PgOfsDn	(int m){  ofs += m;  Ofs();  int d= cur-ofs;        if (d < 0) {  cur -= d;  Cur();  }  }

void Playlist::PgUp		(int m){  cur -= m;  ofs -= m;  Cur();  Ofs();  }
void Playlist::PgDn		(int m){  cur += m;  ofs += m;  Cur();  Ofs();  }

void Playlist::Home (int m)
{
	switch(m)
	{
	case 2:  cur = 0;  ofs = 0;  break;  // list top
	case 1:  cur = ofs;  break;  // view

	case 0:
		do  --cur;
		while (cur-1 > 0 && !tracksVis[cur].IsDir());
		Up(0);  break;
	}
	bDraw = true;
}

void Playlist::End (int m)
{
	int all = (int)(tracksVis.size());
	switch(m)
	{
	case 2:  cur = all-1;  ofs = all-1;  Cur();  Ofs();  break;  // list end
	case 1:  cur = ofs+lin-1;  Cur();  break;  // view

	case 0:
		do  ++cur;
		while (cur+1 < all && !tracksVis[cur].IsDir());
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
		int b = tracksVis[cur].bookm;  // todo: on All, id ..
		b += add;  b = mia(0,6, b);
		tracksVis[cur].bookm = b;
	}
}

void Playlist::Rate(bool playing, char add)
{
	int pos = playing ? play : cur;
	int r = tracksVis[pos].rate;
	r += add;  r = mia(0,6, r);
	tracksVis[pos].rate = r;
}

void Playlist::Filter(bool lower, char add)
{
	int& f = lower ? filterLow : filterHigh;
	f += add;  f = mia(cRateMin,cRateMax, f);
	Update();
}


//  Find
//--------------------------------------------------------------------
void Playlist::FindClear()
{
	//  clear marks
	for (auto& trk : tracksAll)
		trk.found = false;
	Update();  //-
}
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
		
	auto& trks = opt.bUnfiltered ? tracksAll : tracksVis;
	if (!low)
		for (auto& trk : trks)
		{
			auto& name = opt.bFullPath ? trk.GetPath() : trk.GetName();
			trk.found = name.find(find) != string::npos;
		}
	else
		for (auto& trk : trks)
		{
			auto name = opt.bFullPath ? trk.GetPath() : trk.GetName();
			strlower(name);
			trk.found = name.find(find) != string::npos;
		}
	Update();  //-
}
