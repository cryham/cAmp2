#include "Playlist.h"
#include "../Audio/Audio.h"
#include "../System/FileSystem.h"
#include "../System/Stats.h"
#include "../System/defines.h"
#include "../../libs/imgui.h"  // ColorConvert
#include <vector>
#include <iostream>
using namespace std;


void Playlist::UpdPlayVis()
{
	iPlayVis    = tracks[iPlay].idPlayVis;
	bPlayVisOut = !tracks[iPlay].visible;
	bDraw = true;
}

//--------------------------------------------------------------------------------------------------------
///  Update Visible
//   Fills visible Ids from tracks,  filters rating, adds dirs etc.
//--------------------------------------------------------------------------------------------------------
void Playlist::UpdateVis(int outDebug, bool bZoom)
{
	const bool noEmptyDirs = true;  // par

	int iZoomNew = 0,
		iZoomOld = 0;
	const int
		//zoomTo = lin/2,   // zoom to middle
		zoomTo = iCur - iOfs,  // zoom to cursor
		iAll = LengthAll(), iVis = LengthVis();
	const bool small = iAll < iLinVis || iVis == 0;  // all visible, no scroll
	if (!small && bZoom)
		iZoomOld = GetTrackVisIdAll(min(iVis-1, iOfs + zoomTo));  // even if dir

	if (outDebug) 
	{	cout << "--------------" << endl;
		cout << "len all: " << iAll << endl;
	}

	auto Out = [&](const Track& t)
	{
		cout << "Trk " << t.idDir << "  " << t.name.substr(0, 22) << endl;
	};


	visible.clear();
	stats.Clear();
	
	fs::path prev;
	bool dirHide = false, dirShow = false;
	int dirId = 0;
	Between btw;
	
	//------------------------------------------------
	for (int i=0; i < iAll; ++i)
	{
		Track& t = tracks[i];
		int iVis = LengthVis();

		if (i == iZoomOld)
			iZoomNew = iVis;

		//  Track hide/show
		bool out = t.rate < filterLow || t.rate > filterHigh;  // filtered, hide
		bool show = t.GetHide() == Hid_Show;
		
		if (dirShow || show)  // force showing all
			out = false;
		
		if (i == iPlay)
		{
			iPlayVis = iVis;  // marks closest if out
			bPlayVisOut = out;
		}
		t.idPlayVis = iVis;
		t.visible = !out;

		btw.Add(t);

		if (out && noEmptyDirs)
			continue;

		
		fs::path path = t.path.parent_path();
		if (path != prev)
		{
			VisId id;  id.dir = true;

			int idDir = mapPathToDirs[path];
			if (idDir == 0)  // not found
			{
				///  Add Dir  + + +
				Track d(path, true);
				dirs.emplace_back(move(d));

				int size = dirs.size();
				mapPathToDirs[path] = size;  // 1..
				idDir = size;
				
				id.i = size - 1;  // last
				id.iAll = i;
			}else
				id.i = idDir - 1;

			id.iAll = i;  // closest track

			if (outDebug)  /// D
			{
				int iD = mapPathToDirs[path]-1;
				if (iD < 0)
					cout << "Dir -1" << endl;
				
				const Track& d = dirs[iD];
				cout << "Dir " << iD << "  " << d.name << endl;
			}

			//  Dir hide/show
			auto h = dirs[id.i].GetHide();
			dirHide = h == Hid_Hide;
			dirShow = h == Hid_Show;
				
			visible.emplace_back(move(id));
			
			t.idPlayVis = iVis+1;
			dirId = id.i;

			//if (outDebug)  Out(t);

			stats.AddDir();
		}
		prev = path;

		t.idDir = dirId;
		
		if (!show)
		{
			if (dirHide)
				t.visible = false;
			
			if (out || dirHide)
				continue;
		}

		///  Add Track  + + +
		btw.Sub(t);  t.btw = btw;  btw.Clear();

		VisId id;  id.iAll = id.i = i;
		visible.emplace_back(id);
		
		if (outDebug)  Out(t);  /// D
		
		stats.Add(&t);
	}
	//------------------------------------------------

	if (bZoom)  // set cur, ofs
	{
		iCur = iZoomNew;
		if (small)
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
