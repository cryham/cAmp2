#pragma once
#include "Playlist.h"
#include "Settings.h"
#include "AppLog.h"

#include <SFML/Window/Event.hpp>  // input
#include <memory>

class Audio;


class App : public LogErr
{
public:
    App();
	virtual ~App();

    virtual bool Run() = 0;

	
	Settings set;
	
	std::unique_ptr<Audio> audio;

protected:

	std::unique_ptr<Playlist> pls;

	//std::unique_ptr<Skin> skin;

	
	bool Init();
	bool Destroy();

	//  input
	bool KeyDown(sf::Event::KeyEvent key);  // use own enum?
	bool KeyUp(sf::Event::KeyEvent key);

	//  mouse pos, button
	int mx = 0, my = 0, mb = 0;
	void Mouse(int x, int y)
	{	mx = x;  my = y;  }
	void Wheel(int d)
	{	}
	
	bool bFps = false;  // show Fps
	
	void Play();
};
