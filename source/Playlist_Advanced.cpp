#include <vector>
#include "FileSystem.h"
#include "Playlist.h"
#include "Audio.h"
#include "Stats.h"
#include "def.h"
#include "Util.h"
#include "../libs/imgui.h"  // ColorConvert
#include <iostream>
using namespace std;


//--------------------------------------------------------------------------------------------------------
///  Update Visible
//   Fill tracksVis Ids from tracksAll, filters rating, adds dirs etc.
//--------------------------------------------------------------------------------------------------------
void Playlist::UpdateVis(bool bZoom)
{
	bool emptyDirs = false;

	int iZoomNew = 0,
		iZoomOld = 0,
		//zoomTo = lin/2,   // zoom to middle
		zoomTo = iCur - iOfs,  // zoom to cursor
		iAll = LengthAll(), iVis = LengthVis();
	bool sh = iAll < iLinVis || iVis == 0;  // all visible, no scroll
	if (!sh && bZoom)
		iZoomOld = GetTrackVisIdAll(min(iVis-1, iOfs + zoomTo));  // even if dir

	tracksVis.clear();
	stats.Clear();
	fs::path prev;
	//playVisOut = true;
	
	for (int i=0; i < iAll; ++i)
	{
		Track& t = tracksAll[i];
		int iVis = LengthVis();

		if (i == iZoomOld)
			iZoomNew = iVis;

		bool out = t.rate < filterLow || t.rate > filterHigh;
		if (i == iPlay)
		{
			iPlayVis = iVis;  // marks closest if out
			bPlayVisOut = out;
		}
		t.idPlayVis = iVis;
		t.vis = !out;

		if (out && !emptyDirs)
			continue;
		
		fs::path path = t.path.parent_path();
		if (path != prev)
		{
			VisId id;  id.dir = true;

			int im = mapDirs[path];
			if (im == 0)  // not found
			{
				//  Add Dir  +++
				Track d(path, true);
				tracksDirs.emplace_back(move(d));
				int si = tracksDirs.size();
				mapDirs[path] = si;  // 1..
				
				id.i = si - 1;  // last
				id.iAll = i;
	
			}else
				id.i = im - 1;

			id.iAll = i;  // closest track

			tracksVis.emplace_back(move(id));

			t.idPlayVis = iVis+1;
				
			stats.AddDir();
		}
		prev = path;
		
		if (out)
			continue;
		
		//  Add File  +++
		VisId id;  id.iAll = id.i = i;
		tracksVis.emplace_back(id);
		
		stats.Add(&t);
	}

	if (bZoom)  // set cur, ofs
	{
		iCur = iZoomNew;
		if (sh)
			iOfs = 0;
		else
			iOfs = iZoomNew - zoomTo;
	}
	
	int all = LengthVis();
	iCur = mia(0, all, iCur);
	if (iOfs > all-iLinVis)  iOfs = all-iLinVis;  //  no view past last track
	if (iOfs < 0)  iOfs = 0;  //  cur stays in view
	bDraw = true;
	//Cur();  Ofs();
}
//--------------------------------------------------------------------------------------------------------

void Playlist::UpdPlayVis()
{
	iPlayVis    = tracksAll[iPlay].idPlayVis;
	bPlayVisOut = !tracksAll[iPlay].vis;
	bDraw = true;
}


//  Find  clear marks
void Playlist::FindClear()
{
	for (auto& trk : tracksAll)
		trk.found = false;
	iFound = 0;
	bDraw = true;
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
	{	for (auto& trk : tracksAll)
		if (opt.bUnfiltered || trk.vis)
		{
			auto& name = opt.bFullPath ? trk.GetPath() : trk.GetName();
			bool f = name.find(find) != string::npos;
			if (f)  ++iFound;
			trk.found = f;
		}
	}else
	{	for (auto& trk : tracksAll)
		if (opt.bUnfiltered || trk.vis)
		{
			auto name = opt.bFullPath ? trk.GetPath() : trk.GetName();
			strlower(name);
			bool f = name.find(find) != string::npos;
			if (f)  ++iFound;
			trk.found = f;
		}
	}
	bDraw = true;
}


///  Rename files
//   Saves rating and bookmark in filename.
//--------------------------------------------------------------------------------------------------------
int Playlist::RenameRate(bool playing)
{
	int i = 0, cnt = 0;
	for (auto& t : tracksAll)
	{
		if (t.rate  != t.rateInName ||
			t.bookm != t.bookmInName)  // user changed
		{
			//  get new filename
			fs::path name = t.name;
			if (t.bookm > 0)
			{	name += "%";  name += '0'+t.bookm;  }
			if (t.rate)
				name += chFRateAdd[t.rate + cRmin];
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
