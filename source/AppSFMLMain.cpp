#include "AppSFMLDraw.h"
#include "FileSystem.h"
#include <memory>
#include <iomanip>

using namespace sf;
using namespace std;


bool AppSFMLDraw::Run()
{

	//  Create window
	//------------------------------------------------
	//VideoMode vm = VideoMode::getDesktopMode();

	pWindow = make_unique<RenderWindow>(
		VideoMode(350,1140), //set.xwSize, set.ywSize),
		"cAmp2", // Title
		Style::Default, ContextSettings());

	pWindow->setVerticalSyncEnabled(true);
//	pWindow->setFramerateLimit(60);
	pWindow->setPosition(Vector2i(0, 0));
	//window->setPosition(Vector2i(set.xwPos, set.ywPos));

	
	//  icon
	string data = FileSystem::Data(), file;
	file = data + "/cAmp.png";
	Image icon;
	if (icon.loadFromFile(file))
	{
		pWindow->setIcon(32, 32, icon.getPixelsPtr());
		cout << "Can't load icon: " << file << endl;
	}


	//  Load data
	//------------------------------------------------
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

	//  pass to app
	text.setFont(*pFont.get());
	text.setCharacterSize(16);  //app->set.iFontH);
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
//			case Event::MouseMoved:				app->Mouse(e.mouseMove.x, e.mouseMove.y);  break;
//			case Event::MouseWheelScrolled:		app->Wheel(e.mouseWheelScroll.delta);  break;

//			case Event::MouseButtonPressed:		app->mb = e.mouseButton.button + 1;  break;
//			case Event::MouseButtonReleased:	app->mb = 0;  break;

//			case Event::KeyPressed:		app->KeyDown(e.key);  break;
//			case Event::KeyReleased:	app->KeyUp(e.key);  break;

			case Event::Resized:
				if (e.type == sf::Event::Resized)
				{
					// update the view to the new size of the window
					sf::FloatRect vis(0, 0, e.size.width, e.size.height);
					pWindow->setView(sf::View(vis));
				}
				break;
				// todo: save new size
				//set.GetWndDim(window);
			case Event::Closed:
				pWindow->close();
				break;
			}
		}
		pWindow->clear();
		sf::Time time = timer.restart();
		//Update(*window, time);
		//app->dt = time.asSeconds();

		//  Draw
		//------------------
		//app->Gui();
		Clr(120,160,240);
		ostringstream ss;  ss.width(2);
		ss << fixed << setprecision(2) << 1.f/time.asSeconds();
		str = "Fps: " + ss.str();
		bold = false;
		Txt(10,10);
		bold = true;
		
		Clr(180,180,40);
		str = "Cool";
		Txt(10,30);
		
		Rect(0,60,350,20, 0,70,241,10, 255,255,255);
		
		Rect(0,80,350,20, 0,90,251,12, 255,255,255);

		//app->Graph();

		pWindow->display();
	}
	return true;
}
