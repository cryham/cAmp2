#pragma once
#include <memory>
#include <deque>
#include "Playlist.h"
#include "Settings.h"


class App
{
public:
    App();
	virtual ~App();

    virtual bool Run() = 0;
	
	Settings set;
	
	//Audio audio;

protected:

	std::unique_ptr<Playlist> pls;

	//std::unique_ptr<Skin> skin;
	
	bool Init();
	bool Destroy();

};
