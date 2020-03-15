#pragma once
#include "Track.h"
#include "AppLog.h"
#include <deque>


class Audio;

class Playlist : public LogErr
{
protected:
    std::deque<Track> tracks;

public:
	//  vars  ----
	int cur = 0;   //  cursor pos   ids to tracks
	int ofs = 0;   //  offset view start
	int play = 0;  //  playing pos
	
	int lin = 10;  //  view visible lines, for page keys
	bool bDraw = true;  // needs redraw, changed

	int filterLow = -2, filterUp = 5;  // todo: filter lower, upper
	int bookm = 0; //  bookmark
	
	//  move cur, ofs
	void PgUp(int), PgDn(int),  // (int mul)
		PgOfsUp(int), PgOfsDn(int),
		Up(int), Dn(int),
		Home(int), End(int),
		Cur(), Ofs();

	//  stats, total  ----  // todo: 
	uintmax_t allSize = 0;
	double  allTime = 0.0;
	int  allDirs = 0, allFiles = 0;  // count
	
    enum EInsert
    {  Ins_Cursor, Ins_Top, Ins_End  };

	
	//  main  ----
	Playlist();
    Playlist(std::string name1);

    bool AddDir(fs::path dir, bool recursive = true, const EInsert& where = Ins_End);
	
	static Audio* audio;  // for IsPlayable
	

	//  play  ----
	bool Play(bool set=false);
	bool Next(int add=1);

	
	//  get  ----
    std::deque<Track>& GetTracks()
    {   return tracks;  }

	const std::deque<Track>& GetTracks() const
    {   return tracks;  }
	
	bool IsEmpty() const
	{	return tracks.empty();  }

	int Length() const
	{	return (int)tracks.size();  }

    //  todo:  copy, move selected tracks, from other ..

	
	//  file  ----
	std::string name = "none";  // filename, also for tab

	bool Load(), Save();
	void Clear();
	
	//  advanced  ----
	void Bookm(bool pls, char add);  //  bookmarks

};
