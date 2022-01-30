#pragma once
#include "Track.h"
#include "../System/LogFile.h"
#include "../System/Stats.h"
#include "../System/Rating.h"
#include "../Settings/Settings.h"  // for SetFind
#include <deque>
#include <map>


class Audio;

class Playlist : public Logger
{
protected:
	// 
	std::deque<Track> tracks;  // All, unfiltered, from .cp
	
	std::vector<Track> dirs;  // Dir added when path changes  auto, saved in .cp
	std::map<fs::path, int> mapPathToDirs;  // dirs id+1, 0 not found
	
	struct VisId
	{
		bool dir = false;
		int i = 0;     // id to tracks or dirs
		int iAll = 0;  // id to tracks  (to first track for dirs)
	};
	std::vector<VisId> visible;  // only visible, filtered tracks with dirs

public:
	//  vars  ----
	std::string name = "none";  // filename .cp, also for tab

//----  properties saved in .cp  ----
//private:
	int iCur = 0;   //  cursor pos          id to Vis   cur >= ofs
	int iOfs = 0;   //  offset, view start  id to Vis
	int iPlay = 0;  //  playing pos	        id to All
public:
	//struct Filter
	
	int filterLow = Ratings::valMin;  // lower, upper
	int filterHigh = Ratings::valMax;
	int bookm = 0;  //  bookmark

	//  tab color
	float hue = 0.55f, sat = 0.8f, val = 0.9f;  // skyblue
//----  properties end  ----

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
	Stats stats, statsAll, statsSel;  // filtered, All not filtered, Selected
	// statsAll updated only in Load()..
	
	enum EInsert
	{  Ins_Cursor, Ins_Top, Ins_End  };

	
//  ctor, main  ------------
public:
	Playlist(std::string name1);

	bool AddDir(fs::path dir, bool recursive = true, const EInsert& where = Ins_End);
	bool AddFile(fs::path file, const EInsert& where = Ins_End);
	
	static Audio* audio;  // Play, GetTrackTime, IsPlayable(ext) etc.
	

	//  Play  ----
	bool Play(bool set = false);
	bool Next(int add = 1);
	void GotoPlay();


	//  File
	bool Load(), Save();
	void Clear();
	
	//  Update  Fills visible Ids
	void UpdateVis(int outDebug = 0, bool bZoom = true);
	
	
	//  Basic  ----
	void DeleteCur();
	void DuplicateCur();
	void HideCur(EHide hide);

	
	//  Change  ----
	void Bookm(bool pls, int add);  //  bookmarks
	void Rate(bool playing, int add);

	void Filter(bool lower, int add);
	int GetFilter(bool lower)
	{	return lower ? filterLow : filterHigh;  }


	//  Select  ----
	int ylastSel = -1;
	void Pick(int cur), Select1(int cur);
	void SelRange(int cur, bool unselect), SelDir(int cur);
	
	void SelTrk(Track& t, bool sel);
	void Sel(int ivL, bool sel);
	void Unsel0(), UnSel();

	//  Operation  ----
	//  todo:  copy, move selected tracks, from other ..
	#if 0
	void // Move1(int m, pTrk npos), MoveSel(int m, pTrk npos, CList* pL=NULL/*from*/),  // npos,cur = destin
		Del(bool disk=false), DelSel(bool disk=false), del(pTrk q,bool disk=false),  //from disk
		Insert1(int m, pTrk nt), ins(int m, pTrk cur, pTrk n/*what*/),
		insertList(int m, pTrk npos, pTrk first,pTrk last),
		CopySelFiles();
	#endif

	
	//  Advanced  ----
	bool DeleteCurFile(bool playNext);
	
	void Find(std::string &find, const SetFind& opt);
	void FindClear();
	
	int RenameRate(bool playing);

	
	//  getters	etc.
	//----------------------------------------------------------------------------------------
	bool IsEmpty() const	{	return tracks.empty();  }
	bool HasSelected() const{	return statsSel.GetFiles() > 0;  }

	int LengthAll() const	{	return (int)tracks.size();  }
	int LengthVis() const	{	return (int)visible.size();  }
	
	const Track& GetTrackAll(int id) const	{   return tracks[id];  }
	      Track& GetTrackAll(int id)		{   return tracks[id];  }

	const Track& GetTrackVis(int id) const	{   return Get(visible[id]);  }
	      Track& GetTrackVis(int id)		{   return Get(visible[id]);  }
	
	const Track& Get(VisId id) const	{	return id.dir ? dirs[id.i] : tracks[id.i];  }
	      Track& Get(VisId id)			{	return id.dir ? dirs[id.i] : tracks[id.i];  }
	
	//  get all from vis
	int GetTrackVisIdAll(int id) const	{   return visible[id].iAll;  }   // All  always track
	int GetTrackVisId(int id) const		{   return visible[id].dir ? -1 : visible[id].i;  }
};
