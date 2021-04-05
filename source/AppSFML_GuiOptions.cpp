#include "../libs/imgui.h"
#include "../libs/imgui-SFML.h"
#include "AppSFML_Draw.h"
#include "FileSystem.h"
#include "Util.h"
using namespace sf;  using namespace std;  using namespace ImGui;


bool AppSFMLDraw::WndVisible(EWndOpt w)
{
	return vWindows[w] && vWindows[w]->isOpen();
}

//  gui util  -----
const ImVec4 clr1(0.2f,0.2f,0.7f, 0.5f);
const ImVec4 clr0(0.2f,0.2f,0.7f, 0.3f);

//  separators
void AppSFMLDraw::Sep(int h)
{
	Dummy(ImVec2(50, h));
}
void AppSFMLDraw::Line()
{
	Sep(5);  Separator();  Sep(5);
}

//  const
const AppSFMLDraw::SWndConst AppSFMLDraw::wndConst[WO_All] = {
	{"Playlist Find", 400,300},
	{"Playlist Filter", 400,200},
	{"Playlist Tab", 400,510},
	{"Player Tabs", 400,300},
	
	{"Player View & Statistics", 400,350},
	{"Player Audio", 400,300},
	{"Player Visualization", 400,300},
	
	{"Player Test", 300,200},
	{"About", 450,320},
	{"All Options windows", 300,400},
};

//  ctor
//------------------------------------------------------------------
AppSFMLDraw::AppSFMLDraw()
{
	for (int i=0; i < WO_All; ++i)
		vWindows[i] = nullptr;
}

//  Wnd Open
//------------------------------------------------------------------
void AppSFMLDraw::WndOpen(EWndOpt w)
{
	if (wndInited)  //return;  // todo: many opened windows..
	{
		for (int i=0; i < WO_All; ++i)
			WndClose(i);
	}
	if (w < 0 || w >= WO_All)  return;
	auto& wnd = vWindows[w];
	if (wnd)  return;
	const auto& wc = wndConst[w];
	
	wnd = make_unique<RenderWindow>(
		VideoMode(wc.width, wc.height), wc.title,
		Style::Default, ContextSettings());
	wnd->clear();
	
	VideoMode vm = VideoMode::getDesktopMode();
	int width = (vm.width - wc.width) / 2,  // center screen
		height = (vm.height - wc.height) / 2;
	wnd->setPosition(Vector2i(width, height));
	
	wnd->setVerticalSyncEnabled(true);
	wnd->setIcon(32, 32, icon.getPixelsPtr());
				
	//  ImGui Font
	//------------------
	wndInited = true;
	ImGui::SFML::Init(*wnd.get(), false);
	ImGuiIO& io = ImGui::GetIO();
	io.IniFilename = 0;  io.LogFilename = 0;  // none
	io.Fonts->ClearFonts();

	string p = FileSystem::Data() + "/DejaVuLGCSans.ttf";
	ImFont* fnt = io.Fonts->AddFontFromFileTTF(p.c_str(), 17);  // par size
	ImGui::SFML::UpdateFontTexture();

	SetupGuiStyle();
}

void AppSFMLDraw::WndClose(int i)
{
	auto& wnd = vWindows[i];
	if (!wnd)  return;
	
	wnd->close();
	wnd = nullptr;  // deletes

	ImGui::SFML::Shutdown();
	wndInited = false;
}

//  Process All
//---------------------------------------------------------------------------------------------
void AppSFMLDraw::WndProcessAll()
{
	for (int i=0; i < WO_All; ++i)
	{
		if (!WndVisible((EWndOpt)i))  continue;
		auto& wnd = vWindows[i];

		Event e;
		while (wnd->pollEvent(e))
		{
			ImGui::SFML::ProcessEvent(e);
		
			switch (e.type)
			{
			case Event::KeyPressed:
				if (e.key.code != Keyboard::Escape)
					break;
			case Event::Closed:
				WndClose(i);
				return;
			}
		}
	}
}

//  Draw All
//---------------------------------------------------------------------------------------------
void AppSFMLDraw::WndDrawAll(Time time)
{
	for (int i=0; i < WO_All; ++i)
	{
		if (!WndVisible((EWndOpt)i))  continue;
		
		auto& wnd = vWindows[i];
		auto& wc = wndConst[i];
		ImGui::SFML::Update(*wnd, time);

		//  controls  wnd  =====
		SetNextWindowPos( ImVec2(0, 0),  ImGuiCond_Always);
		SetNextWindowSize(ImVec2(wc.width, wc.height), ImGuiCond_Always);
	
		bool open = true;
		const int wfl = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize;
		Begin("Controls", &open, wfl);
		Sep(5);

		switch (i)
		{
		case WO_PlsFind:	WndDraw_PlsFind();  break;
		case WO_PlsFilter:	WndDraw_PlsFilter();  break;
		case WO_PlsTab:		WndDraw_PlsTab();  break;
		case WO_AppTabs:	WndDraw_AppTabs();  break;

		case WO_AppShow:	WndDraw_AppShow();  break;
		case WO_AppAudio:	WndDraw_AppAudio();  break;
		case WO_AppVis:		WndDraw_AppVis();  break;

		case WO_AppTest:	WndDraw_AppTest();  break;
		case WO_AppAbout:   WndDraw_AppAbout();  break;
		case WO_Main:		WndDraw_Main();  break;
		}

		End();

		wnd->clear();
		ImGui::SFML::Render(*wnd.get());
	
		wnd->display();
	}
	if (wndOpen != WO_All)
	{
		WndOpen(wndOpen);
		wndOpen = WO_All;
	}
}
