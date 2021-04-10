#pragma once
#include "Track.h"
#include "AppLog.h"
#include "Stats.h"
#include "Settings.h"  // for SetFind
#include <deque>
#include <map>


class Audio;

class Playlist : public LogErr
{
protected:
	std::deque<Track> tracksAll;  // All, unfiltered, from .cp
	std::deque<Track> tracksDirs;  // Dir added when path changes
	std::map<fs::path, int> mapDirs;  // map path to tracksDirs id+1, 0 not found
	struct VisId
	{
		bool dir = false;
		int i = 0;     // id to All or Dirs
		int iAll = 0;  // id to All  (to first track for dirs)
	};
	std::deque<VisId> tracksVis;  // only visible, filtered All, with dirs

public:
	//  vars  ----
	std::string name = "none";  // filename, also for tab

//  properties saved in .cp
//private:
	int iCur = 0;   //  cursor pos          id to Vis   cur >= ofs
	int iOfs = 0;   //  offset, view start  id to Vis
	int iPlay = 0;  //  playing pos	        id to All
public:	
	int filterLow = cRateMin;  // lower, upper
	int filterHigh = cRateMax;
	int bookm = 0;  //  bookmark

	//  tab color
	float hue = 0.f, sat = 0.f, val = 0.f;
//  properties end

	uint8_t bck[3]={0,0,0};  // bckgr,text colors  after dim from hsv
	uint8_t txt[3]={0,0,0};
	void UpdateColor();

	
	int iPlayVis = 0;   //  playing pos		id to Vis
	bool bPlayVisOut = false;  //  not visible, filtered out
	void UpdPlayVis();

	bool bDraw = true;  // needs redraw, changed
private:	
	int iLinVis = 10;   //  view visible lines, for page keys, set outside
	int iFound = 0;  //  find count
public:
	void SetVisLines(int l)
	{	iLinVis = l;  }
	int GetFound() const
	{	return iFound;  }
	
	
	//  move cur, ofs
	void PgUp(int), PgDn(int),  // (int multiplier)
		PgOfsUp(int), PgOfsDn(int),
		Up(int), Dn(int),
		Home(int), End(int),
		Cur(), Ofs();  // check range

	//  stats
	Stats stats, stAll;  // All = not filtered
	
	enum EInsert
	{  Ins_Cursor, Ins_Top, Ins_End  };

	
//  ctor, main  ----
	Playlist();
	Playlist(std::string name1);

	bool AddDir(fs::path dir, bool recursive = true, const EInsert& where = Ins_End);
	bool AddFile(fs::path file, const EInsert& where = Ins_End);
	
	static Audio* audio;  // Play, GetTrackTime, IsPlayable(ext) etc.
	

	//  play  ----
	bool Play(bool set = false);
	bool Next(int add = 1);
	void GotoPlay();

	
public:
	bool Load(), Save();
	void Clear();
	
	//  Update  Fill tracksVis Ids from tracksAll,
	void UpdateVis(bool bZoom = true);  // for view,  filter, add dirs etc.
	
	
	//  basic  ----
	//  todo:  copy, move selected tracks, from other ..
	void DeleteCur();

	
	//  change  ----
	void Bookm(bool pls, char add);  //  bookmarks
	void Rate(bool playing, char add);

	void Filter(bool lower, char add);
	int GetFilter(bool lower)
	{	return lower ? filterLow : filterHigh;  }

	
	//  advanced  ----
	bool DeleteCurFile(bool playNext);
	void Find(std::string &find, const SetFind& opt);
	void FindClear();
	int RenameRate(bool playing);

	
//  getters	etc.  ----
	const Track& GetTrackAll(int id) const	{   return tracksAll[id];  }
		  Track& GetTrackAll(int id)		{   return tracksAll[id];  }

	const Track& GetTrackVis(int id) const	{   return Get(tracksVis[id]);  }
		  Track& GetTrackVis(int id)		{   return Get(tracksVis[id]);  }
	
	int LengthAll() const	{	return (int)tracksAll.size();  }
	int LengthVis() const	{	return (int)tracksVis.size();  }
	bool IsEmpty() const	{	return tracksAll.empty();  }
	
	const Track& Get(VisId id) const	{	return id.dir ? tracksDirs[id.i] : tracksAll[id.i];  }
		  Track& Get(VisId id)			{	return id.dir ? tracksDirs[id.i] : tracksAll[id.i];  }
	
	//  get all from vis
	int GetTrackVisIdAll(int id) const	{   return tracksVis[id].iAll;  }   // all  always track
	int GetTrackVisId(int id) const		{   return tracksVis[id].dir ? -1 : tracksVis[id].i;  }
};
