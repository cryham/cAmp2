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
	parent = p1.filename();
	parent2 = p1.parent_path().filename();

	//  ext
	ext = path.extension();
	if (ext.empty())
		return;
	ext = ext.substr(1);  // no .
	strupper(ext);

	//  rem ext from name
	auto i = name.find_last_of(".");
	if (i != string::npos)
		name = name.substr(0, i);
	
	//  get rating, bookmark from name  // todo: for dirs too
	Ratings::GetFromName(name.c_str(), rate, bookm);
	rateInName = rate;
	bookmInName	= bookm;
	Ratings::CleanName(name);
}
