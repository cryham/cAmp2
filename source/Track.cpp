#include "Track.h"
#include "Util.h"
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
    //name = path.generic_wstring();
    name = path.filename().filename().string();
	//  ext
	ext = path.extension();
	if (ext.empty())
		return;
	ext = ext.substr(1);  // no .
	strupper(ext);

	//  rem ext from name
	size_t i = name.find_last_of(".");
	if (i != string::npos)
		name = name.substr(0, i);
	
	//  get rating, bookmark from name
	GetNameRating(name.c_str(), rate, bookm);
	CleanNameRating();
}


void Track::CleanNameRating()
{
	size_t p = name.find_last_not_of(cFnStrRates);
	if (p == string::npos)  return;
	
	if (p > 1 && name[p-1]=='%')
		name.erase(p-1);  // = substr(0,p-1);
	else
		name.erase(p+1);  // = substr(0,p+1);
}

void Track::GetNameRating(const string& name, char& pRate, char& pBookm)
{
	//  get rating in file name at end
	int len = name.length();  // no ext
	if (len == 0/*<= 2*/)  return;

	bool dn = true;
	int r = 0;
	while (dn && r < chFnAll)
	{
		if (name[len-1] == cFnCharRates[r])
		{
			char rate = cFnNumRates[r];
			if (rate==-1 && len >= 2)
			{	if (name[len-2] == cFnCharRates[r])   rate=-2;  //- to --
			}
			pRate = rate;
			dn = false;
		}
		++r;
	}
	
	//  get bookmark %1..%6
	string s(name);
	size_t p = s.find_last_not_of(cFnCharRates);
	if (p != string::npos && p > 1 && s[p-1]=='%' && s[p]>='1' && s[p]<='6')
		pBookm = s[p]-'0';
}
