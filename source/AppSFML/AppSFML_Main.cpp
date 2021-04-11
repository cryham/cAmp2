#include "AppSFML_Draw.h"
#include "../System/FileSystem.h"
#include "../../libs/imgui.h"
#include "../../libs/imgui-SFML.h"
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


//  Load and create all
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
	
	
	if (!LoadFonts())
		return false;
		
	if (!CreateTextures())
		return false;
	return true;
}

bool AppSFMLDraw::CreateTextures()
{
	//  vis tex for spectrogram fullscreen
	VideoMode vm = VideoMode::getDesktopMode();
	pVisTexture = make_unique<Texture>();
	if (!pVisTexture->create(vm.width, vm.height))
	{
		Error("Can't create fullscreen texture for vis spectrogram");
		//return false;
	}
	auto s = pVisTexture->getSize();
	Uint8 pixels[s.x * 4] = {0};  // alpha
	for (int y=0; y < s.y; ++y)
		pVisTexture->update(pixels, s.x, 1, 0, y);
	//pVisTexture->setRepeated(true);
	//pVisTexture->setSmooth(false);
	pVisSprite = make_unique<Sprite>(*pVisTexture.get());
	
	// todo: slider tex vm.height
	//setSmooth(true);
	return true;
}
	
bool AppSFMLDraw::LoadFonts()
{
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
	string data = FileSystem::Data(), file;
	
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
			case Event::MouseMoved:				MouseMove(e.mouseMove.x, e.mouseMove.y);  break;
			case Event::MouseWheelScrolled:		WheelMove(e.mouseWheelScroll.delta);  break;

			case Event::MouseButtonPressed:		MouseDown((int)e.mouseButton.button + 1);  break;
			case Event::MouseButtonReleased:	MouseUp((int)e.mouseButton.button + 1);  break;

			
			case Event::KeyPressed:
				if (set.escQuit && e.key.code == Keyboard::Escape)
					pWindow->close();
				KeyDown(e.key);  break;

				
			case Event::Resized:
				if (e.type == Event::Resized)
				{
					FloatRect r(0, 0, e.size.width, e.size.height);
					pWindow->setView(sf::View(r));
				}
				//  save new size
				SetViewFromWnd();
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
		MouseUpdate();

		pWindow->display();

		WndDrawAll(time);
	}
}


//  Destroy All
//------------------------------------------------
void AppSFMLDraw::DestroyAll()
{
	if (wndInited)
		ImGui::SFML::Shutdown();
	
	SetViewFromWnd();
	Destroy();
}


//------------------------------------------------------------------------------------------------
//  Create window
//------------------------------------------------------------------------------------------------
void AppSFMLDraw::CreateWindow()
{
	//VideoMode vm = VideoMode::getDesktopMode();
	const auto& v = set.view;

	pWindow = make_unique<RenderWindow>(
		VideoMode(v.wnd.xSize, v.wnd.ySize),
		"cAmp2",  // Title
		Style::Default, ContextSettings());

	pWindow->setVerticalSyncEnabled(true);
//	pWindow->setFramerateLimit(60);  // par
	pWindow->setPosition(Vector2i(v.wnd.xPos, v.wnd.yPos));
}


//  Views
void AppSFMLDraw::UpdateView(bool load, int v)
{
	if (load)
	{
		set.view = set.views[v];  // load preset
		const auto& w = set.view.wnd;
		pWindow->setPosition(Vector2i(w.xPos, w.yPos));
		pWindow->setSize(Vector2u(w.xSize, w.ySize));
		UpdDim();
	}else
	{
		SetViewFromWnd();
		set.views[v] = set.view;  // save preset
	}
}

void AppSFMLDraw::SetViewFromWnd()
{
	auto& w = set.view.wnd;
	auto p = pWindow->getPosition();
	auto s = pWindow->getSize();
	w.xPos = p.x;  w.yPos = p.y;
	w.xSize = s.x;  w.ySize = s.y;
}
