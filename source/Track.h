#pragma once
#include <string>
#include <filesystem>
namespace fs = std::filesystem;


class Track
{
	friend class AudioBass;
	friend class Playlist;
protected:

    //  shown in gui,  got from path
    std::string name;  // no rating, bookm
	std::string ext;   // uppercase, no dot .

    //  file on disk, saved in playlist
    fs::path path;
	//std::string path2;  // subdir-1
	
	//  time
	bool gotTime = false;  // needs update
	double time = 0.0;  // in seconds
	uintmax_t size = 0;

	///  extra  ----
	bool disabled = false;  // can't play
	bool mod = false;  // xm, mod etc.
	char hide = 0, sel = 0;  // todo:
public:
	char rate = 0;  // rating
	char bokm = 0;  // bookmark
	//byte srch;   // match
	//bool dir = false;

public:
    Track(fs::path file);

	//  get for draw
    const std::string& GetName() const
    {   return name;  }

	bool GotTime() const
	{	return gotTime;  }
	
	double GetTime() const
	{	return time;  }
	
	bool IsDisabled() const
	{	return disabled;  }
	
	void SetNameFromPath();
	void CleanNameRating();
	static void GetNameRating(const std::string& name, char& pRate, char& pBokm);
};


///  rating chars in name
const int chFnAll = 7;  // special -2 -- 
const char cFnCharRates[chFnAll] = {'=','-', '`','^','~','+','#'};
const char cFnNumRates[chFnAll] = {-3,-1, 1,2,3,4,5};

const int cR0 = 3/*-*/, cR1 = 5/*+*/, chRall = cR0+cR1+1;
const static std::string chFRateAdd[chRall] = {"=","--","-","","`","^","~","+","#"};  // rename add, file name
const static std::string chFRateVis[chRall] = {"=","--","-","","`","^","~","+","*"};  // draw text, sybols
