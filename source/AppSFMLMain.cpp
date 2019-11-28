#include "AppSFMLDraw.h"
#include "FileSystem.h"
#include <memory>
#include <iostream>
using namespace sf;  using namespace std;


bool AppSFMLDraw::Run()
{
	Init();

	//  Create window
	//------------------------------------------------
	//VideoMode vm = VideoMode::getDesktopMode();

	pWindow = make_unique<RenderWindow>(
		VideoMode(set.xwSize, set.ywSize),
		"cAmp2",  // Title
		Style::Default, ContextSettings());

	pWindow->setVerticalSyncEnabled(true);
//	pWindow->setFramerateLimit(60);
	pWindow->setPosition(Vector2i(set.xwPos, set.ywPos));

	
	//  Load data
	//------------------------------------------------
	string data = FileSystem::Data(), file;
	file = data + "/cAmp.png";
	Image icon;
	if (icon.loadFromFile(file))
		pWindow->setIcon(32, 32, icon.getPixelsPtr());
	else
		cout << "Can't load icon: " << file << endl;

	//  font
	file = data + "/DejaVuLGCSans.ttf";
	pFont = make_unique<Font>();
	if (!pFont->loadFromFile(file))
	{
		cout << "Can't load font: " << file << endl;
		return false;
	}

	Texture tex;
	file = data + "/player.png";
	if (!tex.loadFromFile(file))
	{
		cout << "Can't load texture: " << file << endl;
		return false;
	}

	pBackgr = make_unique<Sprite>(tex);

	text.setFont(*pFont.get());
	text.setCharacterSize(16);  //set.iFontH
	//font.getLineSpacing();


	//  Loop
	//------------------------------------------------
	Clock timer;
	while (pWindow->isOpen())
	{
		//  Process events
		//------------------
		Event e;
		while (pWindow->pollEvent(e))
		{
			//ProcessEvent(e);

			switch (e.type)
			{
			// todo: input
//			case Event::MouseMoved:				Mouse(e.mouseMove.x, e.mouseMove.y);  break;
//			case Event::MouseWheelScrolled:		Wheel(e.mouseWheelScroll.delta);  break;

//			case Event::MouseButtonPressed:		mb = e.mouseButton.button + 1;  break;
//			case Event::MouseButtonReleased:	mb = 0;  break;

//			case Event::KeyPressed:		KeyDown(e.key);  break;
//			case Event::KeyReleased:	KeyUp(e.key);  break;

			case Event::Resized:
				if (e.type == sf::Event::Resized)
				{
					// update the view to the new size of the window
					sf::FloatRect vis(0, 0, e.size.width, e.size.height);
					pWindow->setView(sf::View(vis));
				}
				// save new size
				//set.GetWndDim(pWindow.get());
				break;

			case Event::Closed:
				pWindow->close();
				break;
			}
		}
		pWindow->clear();
		sf::Time time = timer.restart();
		//Update(*window, time);
		dt = time.asSeconds();

		//  Draw
		//------------------
		DrawPlayer();
		
		pWindow->display();
	}
	
	set.GetWndDim(pWindow.get());
	Destroy();
	return true;
}
