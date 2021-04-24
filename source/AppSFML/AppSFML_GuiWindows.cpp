#include "AppSFML_Draw.h"
#include "../System/FileSystem.h"
#include "../System/Utilities.h"
#include "../../libs/imgui.h"
#include "../../libs/imgui-SFML.h"
using namespace sf;  using namespace std;  using namespace ImGui;


//  const windows data
const AppSFMLDraw::SWndConst AppSFMLDraw::wndConst[WO_ALL] =
{
	{"Main", "All Options", 420,400},

	{"Player", "Audio", 600,350},
	{"Player", "Keyboard", 600,750},
	{"Player", "About", 600,400},
	{"Player", "Help", 600,750},

	{"View", "View", 500,750},
	{"View", "View Fonts", 500,750},
	{"View", "Visualization", 900,750},
	{"View", "Visualization Themes", 900,750},
	{"View", "Times Coloring", 500,750},

	{"Tabs", "Current", 420,570},
	{"Tabs", "All", 420,370},

	{"Playlist", "Find", 420,300},
	{"Playlist", "Filter", 420,300},
	{"Playlist", "Statistics", 420,300},
	{"Playlist", "Test", 420,300},
};

//  Draw All
//---------------------------------------------------------------------------------------------
void AppSFMLDraw::WndDrawAll(Time time)
{
	for (int i=0; i < WO_ALL; ++i)
	{
		if (!WndVisible((EWndOpt)i))  continue;

		auto& wnd = vWindows[i];	
		auto& wc = wndConst[i];
		ImGui::SFML::Update(*wnd, time);

		//  controls  wnd  =====
		SetNextWindowPos( ImVec2(0, 0),  ImGuiCond_Always);
		SetNextWindowSize(ImVec2(wndSizeX, wndSizeY), ImGuiCond_Always);
	
		bool open = true;
		const int wfl = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_MenuBar;
		Begin("Controls", &open, wfl);
		
		WndMainMenu();
		Sep(5);

		switch (i)
		{
		case WO_Main:		WndDraw_Main();  break;
		
		case WO_AppAudio:	WndDraw_AppAudio();  break;
		case WO_AppKeys:	WndDraw_AppKeys();  break;
		case WO_AppAbout:   WndDraw_AppAbout();  break;

		case WO_View:		WndDraw_View();  break;
		case WO_ViewFonts:	WndDraw_ViewFonts();  break;
		case WO_Vis:		WndDraw_Vis();  break;
		case WO_VisThemes:	WndDraw_VisThemes();  break;
		case WO_ViewTimes:	WndDraw_ViewTimes();  break;
			
		case WO_Tab:		WndDraw_Tab();  break;
		case WO_TabsAll:	WndDraw_TabsAll();  break;

		case WO_PlsFind:	WndDraw_PlsFind();  break;
		case WO_PlsFilter:	WndDraw_PlsFilter();  break;
		case WO_AppStats:	WndDraw_PlsStats();  break;
		case WO_AppTest:	WndDraw_AppTest();  break;
		}

		End();

		wnd->clear();
		ImGui::SFML::Render(*wnd.get());
	
		wnd->display();
	}
	if (wndOpen != WO_ALL)
	{
		WndOpen(wndOpen);
		wndOpen = WO_ALL;
	}
}

//  Main Menu top
//---------------------------------------------------------------------------------------------
void AppSFMLDraw::WndMainMenu()
{
	if (BeginMenuBar())
	{
		auto MenuWnd = [&](auto wnd)
		{
			if (MenuItem(wndConst[wnd].title.c_str()))  wndOpen = wnd;
		};

		if (BeginMenu("File"))
		{
			if (MenuItem("Insert files.."))  OpenDirFile(true);
			if (MenuItem("Insert dir.."))   OpenDirFile(false);
			//Separator();  //Exit
			EndMenu();
		}
		if (BeginMenu("Player"))
		{
			MenuWnd(WO_AppAudio);
			MenuWnd(WO_AppKeys);
			Separator();
			MenuWnd(WO_AppAbout);
			//MenuWnd(WO_AppHelp);
			EndMenu();
		}
		if (BeginMenu("View"))
		{
			MenuWnd(WO_View);
			MenuWnd(WO_ViewFonts);
			Separator();
			MenuWnd(WO_Vis);
			MenuWnd(WO_VisThemes);
			Separator();
			MenuWnd(WO_ViewTimes);
			EndMenu();
		}
		if (BeginMenu("Tabs"))
		{
			MenuWnd(WO_Tab);
			MenuWnd(WO_TabsAll);
			EndMenu();
		}
		if (BeginMenu("Playlist"))
		{
			MenuWnd(WO_PlsFind);
			MenuWnd(WO_PlsFilter);
			Separator();
			MenuWnd(WO_AppStats);
			MenuWnd(WO_AppTest);
			EndMenu();
		}
		EndMenuBar();
	}
}
