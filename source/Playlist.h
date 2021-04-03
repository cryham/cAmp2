#pragma once
#include "Track.h"
#include "AppLog.h"
#include "Stats.h"
#include <deque>


class Audio;

class Playlist : public LogErr
{
protected:
	std::deque<Track> tracksAll;  // all, unfiltered
    std::deque<Track> tracksVis;  // only visible

public:
	//  vars  ----
	int cur = 0;   //  cursor pos   :ids to tracks
	int ofs = 0;   //  offset, view start
	int play = 0;  //  playing pos
	
	int lin = 10;  //  view visible lines, for page keys
	bool bDraw = true;  // needs redraw, changed

	int filterLow = cRateMin, filterHigh = cRateMax;  // lower, upper
	int bookm = 0; //  bookmark
	
	//  move cur, ofs
	void PgUp(int), PgDn(int),  // (int multiplier)
		PgOfsUp(int), PgOfsDn(int),
		Up(int), Dn(int),
		Home(int), End(int),
		Cur(), Ofs();

	//  stats
	Stats stats, stAll;  // All = not filtered
	
    enum EInsert
    {  Ins_Cursor, Ins_Top, Ins_End  };

	
	//  main  ----
	Playlist();
    Playlist(std::string name1);

    bool AddDir(fs::path dir, bool recursive = true, const EInsert& where = Ins_End);
	bool AddFile(fs::path file, const EInsert& where = Ins_End);
	
	static Audio* audio;  // for IsPlayable
	

	//  play  ----
	bool Play(bool set = false);
	bool Next(int add = 1);
	void GotoPlay();

	
	//  get  ----
    std::deque<Track>& GetTracks()
    {   return tracksVis;  }

	const std::deque<Track>& GetTracks() const
    {   return tracksVis;  }
	
	bool IsEmpty() const
	{	return tracksAll.empty();  }

	int Length() const
	{	return (int)tracksVis.size();  }

	int LengthAll() const
	{	return (int)tracksAll.size();  }
	
    //  todo:  copy, move selected tracks, from other ..

	
	//  file  ----
	std::string name = "none";  // filename, also for tab

	bool Load(), Save();
	void Clear();
	
	void Update();  // for view, filter, add dirs etc.
	//  fills tracksVis from tracksAll
	
	
	//  advanced  ----
	void Bookm(bool pls, char add);  //  bookmarks
	void Rate(bool playing, char add);
	
	void Filter(bool lower, char add);
	int GetFilter(bool lower)
	{	return lower ? filterLow : filterHigh;  }
};
