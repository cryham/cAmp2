#pragma once
#include <deque>
#include "Track.h"


class Audio;

class Playlist
{
protected:
    std::deque<Track> tracks;

public:
	//  cursor pos, playing pos, ids to tracks
	int cur = 0, ofs = 0;  // offset view
	int play = 0;
	int lin = 10;  // view visible lines

	//  move cur, ofs
	void PgUp(int), PgDn(int),  // (int mul)
		PgOfsUp(int), PgOfsDn(int),
		Up(int), Dn(int),
		Home(int), End(int),
		Cur(), Ofs();
	//int all() {  return (int)(tracks.size()-1);  }  // last
	
    enum EInsert
    {  Ins_Cursor, Ins_Top, Ins_End  };

	
	//  main
    Playlist();

    bool AddDir(fs::path dir, bool recursive = true, const EInsert& where = Ins_End);
	
	static Audio* audio;  // for IsPlayable

	
	//  get
    std::deque<Track>& GetTracks()
    {   return tracks;  }

	const std::deque<Track>& GetTracks() const
    {   return tracks;  }
	
	bool IsEmpty() const
	{	return tracks.empty();  }

	int Length() const
	{	return (int)tracks.size();  }

    // copy, move selected tracks from other

};
