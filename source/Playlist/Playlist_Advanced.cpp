#include "Playlist.h"
#include "../Audio/Audio.h"
#include "../System/FileSystem.h"
#include "../System/Stats.h"
#include "../System/defines.h"
#include "../System/Utilities.h"
#include "../../libs/imgui.h"  // ColorConvert
#include <vector>
#include <iostream>
using namespace std;


//  Find  clear marks
void Playlist::FindClear()
{
	for (auto& trk : tracks)
		trk.found = false;
	iFound = 0;
	UpdateVis(0, false);  // for between
	//bDraw = true;
}

///  Find
//--------------------------------------------------------------------------------------------------------
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
	{	for (auto& trk : tracks)
		if (opt.bUnfiltered || trk.visible)
		{
			auto& name = opt.bFullPath ? trk.GetPath() : trk.GetName();
			bool f = name.find(find) != string::npos;
			if (f)  ++iFound;
			trk.found = f;
		}
	}else
	{	for (auto& trk : tracks)
		if (opt.bUnfiltered || trk.visible)
		{
			auto name = opt.bFullPath ? trk.GetPath() : trk.GetName();
			strlower(name);
			bool f = name.find(find) != string::npos;
			if (f)  ++iFound;
			trk.found = f;
		}
	}
	UpdateVis(0, false);  // for between
	//bDraw = true;
}


///  Rename files
//   Saves rating and bookmark in filename.
//--------------------------------------------------------------------------------------------------------
int Playlist::RenameRate(bool playing)
{
	int i = 0, cnt = 0;
	for (auto& t : tracks)
	{
		if (t.NeedsRename())
		{
			//  get new filename
			fs::path name = t.name;
			if (t.bookm > 0)
			{	name += "%";  name += '0'+t.bookm;  }
			if (t.rate)
				name += Ratings::GetAdd(t.rate);
			name += t.path.extension();
			
			fs::path from = t.path;
			fs::path to = t.path.replace_filename(name);
			
			bool play = playing && i == iPlay;
			double time = 0.0;
			if (play)  // stop if playing
			{
				audio->GetPos();
				time = audio->timePlay;
				audio->Stop();
			}
			
			//  rename file finally
			error_code ec;
			fs::rename(from, to, ec);  ++cnt;
			if (ec)
				Log("Error renaming: "+from.u8string()+"\n"+ec.message());
			
			if (play)  // resume play
			{
				audio->Play(t);
				audio->SetPosAbs(time);
			}
			
			if (!ec)  // set same after`
			{
				//t.GetNameRating();
				t.rateInName  = t.rate;
				t.bookmInName = t.bookm;
			}
		}
		++i;
	}
	return cnt;
}
