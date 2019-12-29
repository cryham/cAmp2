#include "AppSFML_Draw.h"
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
//	pWindow->setFramerateLimit(60);  // par
	pWindow->setPosition(Vector2i(set.xwPos, set.ywPos));

	
	//  Load data
	//------------------------------------------------
	string data = FileSystem::Data(), file;
	file = data + "/cAmp.png";
	Image icon;
	if (icon.loadFromFile(file))
		pWindow->setIcon(32, 32, icon.getPixelsPtr());
	else
		Error("Can't load icon: " + file);

	
	//  texture
	Texture tex;
	file = data + "/player.png";
	if (!tex.loadFromFile(file))
	{
		Error("Can't load texture: " + file);
		return false;
	}

	pBackgr = make_unique<Sprite>(tex);

	
	//  font
	const static string sFntName[Fnt_All] = {
		"DejaVuSans.ttf", //Fnt_Info, Fnt_Track, 
		"DejaVuSans.ttf",
		"NotoMono.ttf", //Fnt_Time, Fnt_TimeBig
		"NotoMono.ttf",
	};
	const static int iFntSize[Fnt_All] = {
		15, //14
		14, //15
		16, //16
		20, //20
	};
	
	for (int i=0; i < Fnt_All; ++i)
	{
		file = data + "/" + sFntName[i];
		pFont[i] = make_unique<Font>();
		if (!pFont[i]->loadFromFile(file))
		{
			Error("Can't load font: " + file);
			return false;
		}
		//SetFont(Fnt_Info, view.Fy);
		text[i].setFont(*pFont[i].get());
		text[i].setCharacterSize(iFntSize[i]);  // view.Fy -3);
		//text[i].setStyle(bold ? Text::Bold : Text::Regular);
		//font.getLineSpacing();
	}


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
			case Event::MouseMoved:				Mouse(e.mouseMove.x, e.mouseMove.y);  break;
			case Event::MouseWheelScrolled:		Wheel(e.mouseWheelScroll.delta);  break;

			case Event::MouseButtonPressed:		MouseDown((int)e.mouseButton.button + 1);  break;
			case Event::MouseButtonReleased:	MouseUp((int)e.mouseButton.button + 1);  break;

			
			case Event::KeyPressed:
				if (set.escQuit && e.key.code == Keyboard::Escape)
					pWindow->close();
				KeyDown(e.key);  break;
			
			case Event::KeyReleased:
				KeyUp(e.key);  break;

				
			case Event::Resized:
				if (e.type == sf::Event::Resized)
				{
					// update the view to the new size of the window
					sf::FloatRect vis(0, 0, e.size.width, e.size.height);
					pWindow->setView(sf::View(vis));
				}
				// save new size
				set.GetWndDim(pWindow.get());
				UpdDim();
				break;

			case Event::Closed:
				pWindow->close();
				break;
			}
		}

		//  Draw
		//------------------
		pWindow->clear();
		sf::Time time = timer.restart();
		//Update(*window, time);
		dt = time.asSeconds();

		DrawPlayer();
		Mouse();		
		
		pWindow->display();
	}
	
	set.GetWndDim(pWindow.get());
	Destroy();

	return true;
}
