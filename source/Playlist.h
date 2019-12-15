#pragma once
#include <deque>
#include "Track.h"


class Playlist
{
protected:

    std::deque<Track> tracks;
    //std::deque<Track>::iterator cur, play;

public:

    enum EInsert
    {  Ins_Cursor, Ins_Top, Ins_End  };


    Playlist();

    bool AddDir(fs::path dir, bool recursive = true, const EInsert& where = Ins_End);


    const std::deque<Track>& GetTracks()
    {   return tracks;  }


    // copy, move selected tracks from other

};
