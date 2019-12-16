#pragma once
#include <deque>
#include "Track.h"


class Playlist
{
protected:

    std::deque<Track> tracks;
    //std::deque<Track>::iterator cur, play;

public:
	int cur = 0, play = 0;  // ids to tracks

    enum EInsert
    {  Ins_Cursor, Ins_Top, Ins_End  };


    Playlist();

    bool AddDir(fs::path dir, bool recursive = true, const EInsert& where = Ins_End);


    std::deque<Track>& GetTracks()
    {   return tracks;  }

	const std::deque<Track>& GetTracks() const
    {   return tracks;  }
	
	bool IsEmpty() const
	{	return tracks.empty();  }


    // copy, move selected tracks from other

};
