#include "Track.h"
#include "../System/Utilities.h"
#include "../System/Rating.h"
#include <iostream>
using namespace std;


Track::Track(fs::path file, bool dir1)
{
	dir = dir1;
	path = file;
	SetNameFromPath();
}

void Track::SetNameFromPath()
{
	name = path.filename().filename().string();  // u8string?
	//  parent dirs
	auto p1 = path.parent_path();
	parent = p1.filename().string();
	parent2 = p1.parent_path().filename().string();

	//  ext
	ext = path.extension().string();
	if (ext.empty())
		return;
	ext = ext.substr(1);  // no .
	strupper(ext);

	//  rem ext from name
	auto i = name.find_last_of(".");
	if (i != string::npos)
		name = name.substr(0, i);
	
	//  get rating, bookmark from name
	Ratings::GetFromName(name.c_str(), rate, bookm);
	rateInName = rate;
	bookmInName	= bookm;
	Ratings::CleanName(name);
}


void Between::Clear()
{
	sel = 0;
	found = 0;
}

void Between::Add(const Track& t)
{
	if (t.IsSelected())  ++sel;
	if (t.IsFound())   ++found;
	//if (t.bookm > 0)  ++bookm[t.bookm];
}

void Between::Sub(const Track& t)
{
	if (t.IsSelected())  --sel;
	if (t.IsFound())   --found;
}
