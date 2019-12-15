#pragma once
#include <memory>
#include <deque>
#include "Playlist.h"
#include "Settings.h"
#include <SFML/Window/Event.hpp>  // key,mouse

class Audio;


class App
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

	bool KeyDown(sf::Event::KeyEvent key);  // use own enum?
	bool KeyUp(sf::Event::KeyEvent key);
};
