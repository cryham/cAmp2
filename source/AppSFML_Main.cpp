#include "../libs/imgui.h"
#include "../libs/imgui-SFML.h"

#include "AppSFML_Draw.h"
#include "FileSystem.h"
#include <memory>
#include <iostream>
using namespace sf;  using namespace std;


//  Run
//------------------------------------------------
bool AppSFMLDraw::Run()
{
	Init();
	
	CreateWindow();
	
	if (!LoadResources())
		return false;

	LoopMain();

	DestroyAll();

	return true;
}


//  Create window
//------------------------------------------------------------------------------------------------
void AppSFMLDraw::CreateWindow()
{
	//VideoMode vm = VideoMode::getDesktopMode();

	pWindow = make_unique<RenderWindow>(
		VideoMode(set.view.xSize, set.view.ySize),
		"cAmp2",  // Title
		Style::Default, ContextSettings());

	pWindow->setVerticalSyncEnabled(true);
//	pWindow->setFramerateLimit(60);  // par
	pWindow->setPosition(Vector2i(set.view.xPos, set.view.yPos));
}
	
//  Load data
//------------------------------------------------------------------------------------------------
bool AppSFMLDraw::LoadResources()
{
	string data = FileSystem::Data(), file;
	file = data + "/cAmp.png";

	if (icon.loadFromFile(file))
		pWindow->setIcon(32, 32, icon.getPixelsPtr());
	else
		Error("Can't load icon: " + file);

	
	//  texture
	pTexture = make_unique<Texture>();
	file = data + "/player.png";
	if (!pTexture->loadFromFile(file))
	{
		Error("Can't load texture: " + file);
		return false;
	}

	pBackgr = make_unique<Sprite>(*pTexture.get());

	
	//  font  // todo: fonts in xml
	const static string sFntName[Fnt_All] = {
		"DejaVuLGCSans.ttf", //Fnt_Info, Fnt_Track, 
		"DejaVuLGCSans.ttf",
		"NotoMono.ttf", //Fnt_Time, Fnt_TimeBig
		"NotoMono.ttf",
	};
	const static int iFntSize[Fnt_All] = {
		14, //14
		14, //15
		15, //16
		20, //20
	};
	
	for (int i=0; i < Fnt_All; ++i)
	{
		file = data + "/" + sFntName[i];
		pFont[i] = make_unique<Font>();
		if (!pFont[i]->loadFromFile(file))
		{
			Error("Can't load font: " + file);
			//return false;
		}
		//SetFont(Fnt_Info, view.Fy);
		text[i].setFont(*pFont[i].get());
		text[i].setCharacterSize(iFntSize[i]);  // view.Fy -3);
		//text[i].setStyle(bold ? Text::Bold : Text::Regular);
		//font.getLineSpacing();
	}
	return true;
}


//------------------------------------------------------------------------------------------------
//  Loop
//------------------------------------------------------------------------------------------------
void AppSFMLDraw::LoopMain()
{
	Clock timer;
	while (pWindow->isOpen())
	{
		//  Process events
		//------------------
		WndProcessAll();

		Event e;
		while (pWindow->pollEvent(e))
		{
			//ImGui::SFML::ProcessEvent(e);

			//ProcessEvent(e);
			UpdModifiers();

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

				
			case Event::Resized:
				if (e.type == Event::Resized)
				{
					FloatRect vis(0, 0, e.size.width, e.size.height);
					pWindow->setView(sf::View(vis));
				}
				// save new size
				set.SetDimFromWnd(pWindow.get());
				UpdDim();
				break;

			case Event::Closed:
				pWindow->close();
				break;
			}
		}

		//  Draw
		//------------------
		Time time = timer.restart();
		dt = time.asSeconds();

		DrawAll();
		Mouse();

		pWindow->display();

		WndDrawAll(time);
	}
}


//  Destroy All
//------------------------------------------------------------------------------------------------
void AppSFMLDraw::DestroyAll()
{
	if (wndInited)
		ImGui::SFML::Shutdown();
	
	set.SetDimFromWnd(pWindow.get());
	Destroy();
}
