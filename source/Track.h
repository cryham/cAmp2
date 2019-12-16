#pragma once

#include <string>
#include <filesystem>
namespace fs = std::filesystem;


class Track
{
	friend class AudioBass;
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
	bool dis = false;  // can't play, disabled
	bool mod = false;  // xm, mod etc.
	//byte hide, sel;
	///char rate;  // rating
	///char bokm;  // bookmark
	//byte srch;   // match
	//bool dir = false;

public:
    //Track();
    Track(fs::path file);

    void SetNameFromPath();

	//  get for draw
    std::string GetName() const
    {   return name;  }

	bool GotTime() const
	{	return gotTime;  }
	
	double GetTime() const
	{	return time;  }
	
};
