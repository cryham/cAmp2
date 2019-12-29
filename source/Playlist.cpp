#include <vector>
#include "FileSystem.h"
#include "Playlist.h"
#include "Audio.h"
#include "def.h"
#include "Util.h"
#include <iostream>
using namespace std;


Audio* Playlist::audio = nullptr;

Playlist::Playlist()
{

}


//  play
//--------------------------------------------------------------------
bool Playlist::Play(bool set)
{
	if (IsEmpty())  return false;
	if (set)  play = cur;
	if (play < 0 || play >= tracks.size())  return false;  //Check()
	
	Track& t = GetTracks()[play];
	audio->SetPls(this);
	bool ok = audio->Play(t);
	return ok;
}

bool Playlist::Next(int add)
{
	if (IsEmpty())  return false;
	play += add;
	int last = tracks.size()-1;
	if (play < 0)  play = last;
	if (play >= last)  play = 0;
	// todo: skip while dirs and disabled
	return Play();
}


//  add dir
//--------------------------------------------------------------------
bool Playlist::AddDir(fs::path dir, bool recursive, const EInsert& where)
{

    vector<fs::path> files = FileSystem::ListDir(dir, recursive);
    for (const auto& file: files)
    {
        if (fs::is_directory(file))
            continue;

        //  get ext
		string ext = file.extension();
		if (ext.length() < 2)
			continue;
		ext = file.extension();
		ext = ext.substr(1);  // no .
		strupper(ext);
		
		//  skip if if unplayable
		if (audio != nullptr &&
			!audio->IsPlayable(ext))
			continue;

        Track t(file);
        switch (where)
        {
        case Ins_Cursor:
            //tracks.insert(cur,t);  // insert(vec
            break;
        case Ins_Top:
            tracks.push_front(move(t));
        case Ins_End:
            tracks.push_back(move(t));
        }
    }

    return true;
}


//  list move (keys)
//--------------------------------------------------------------------
void Playlist::Cur()
{
	int all = (int)(tracks.size())-1;
	cur = mia(0, all, cur);
}
void Playlist::Ofs()
{
	int all = (int)(tracks.size());
	if (ofs > all-lin)  ofs = all-lin;  //  no view past last track
	if (ofs < 0)  ofs = 0;  //  cur stays in view
}
void Playlist::Up		(int m){	cur -= m;  Cur();  int d= cur-ofs;        if (d < 0) {  ofs += d;  Ofs();  }  }
void Playlist::Dn		(int m){	cur += m;  Cur();  int d= cur-ofs-lin+1;  if (d > 0) {  ofs += d;  Ofs();  }  }

void Playlist::PgOfsUp	(int m){	ofs -= m;  Ofs();  int d= cur-ofs-lin+1;  if (d > 0) {  cur -= d;  Cur();  }  }
void Playlist::PgOfsDn	(int m){	ofs += m;  Ofs();  int d= cur-ofs;        if (d < 0) {  cur -= d;  Cur();  }  }

void Playlist::PgUp	(int m){	cur -= m;  ofs -= m;  Cur();  Ofs();  }
void Playlist::PgDn	(int m){	cur += m;  ofs += m;  Cur();  Ofs();  }

void Playlist::Home (int m)
{
	switch(m)
	{
	case 2:  cur = 0;  ofs = 0;  break;  //list
	case 1:  cur = ofs;  break;  //view

	/*case 0:
		do  --cur;
		while (cur-1 > 0 && !tracks[cur]->isDir());
		Up(0);  break;*/
	}
}

void Playlist::End (int m)
{
	int all = (int)(tracks.size());
	switch(m)
	{
	case 2:  cur = all-1;  ofs = all-1;  Cur();  Ofs();  break;  //list
	case 1:  cur = ofs+lin-1;  Cur();  break;  //view

	/*case 0:
		do  ++cur;
		while (cur+1 < all && !tracks[cur]->isDir());
		Dn(0);  break;*/
	}
}
