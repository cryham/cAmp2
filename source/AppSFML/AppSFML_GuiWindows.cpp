#include "AppSFML_Draw.h"
#include "../App/AppActions.h"
#include "../System/FileSystem.h"
#include "../System/Utilities.h"
#include "../../libs/imgui.h"
#include "../../libs/imgui-SFML.h"
using namespace sf;  using namespace std;  using namespace ImGui;


//  const windows data
const AppSFMLDraw::SWndConst AppSFMLDraw::wndConst[WO_ALL] =
{
	{"Main", "All Options",    420,400, Act_WO_Main},  //-

	{"Player", "Audio",        600,350, Act_WO_AppAudio},
	{"Player", "Keyboard",     650,855, Act_WO_AppKeys},
	{"Player", "About",        600,400, Act_WO_AppAbout},
	{"Player", "Help",         600,855, Act_WO_AppHelp},

	{"View", "View",           600,450, Act_WO_View},
	{"View", "View Fonts",     600,750, Act_WO_ViewFonts},
	{"View", "Visualization",  900,750, Act_WO_Vis},
	{"View", "Visual Themes",  900,750, Act_WO_VisThemes},
	{"View", "Times Coloring", 600,750, Act_WO_ViewTimes},  //-

	{"Tabs", "Current",        420,600, Act_WO_Tab},
	{"Tabs", "All",            420,370, Act_WO_TabsAll},

	{"Playlist", "Find",       420,300, Act_WO_PlsFind},
	{"Playlist", "Filter",     420,300, Act_WO_PlsFilter},
	{"Playlist", "Statistics", 420,300, Act_WO_AppStats},
	{"Playlist", "Test",       420,300, Act_WO_AppTest},
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
		case WO_AppHelp:    WndDraw_AppHelp();  break;

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
			const auto& k = act->GetKeyForAct(wndConst[wnd].act);
			if (MenuItem(wndConst[wnd].title.c_str(), k.c_str()))
				wndOpen = wnd;
		};

		if (BeginMenu("File"))
		{
			if (MenuItem("Insert files.."))  OpenDirFile(true);
			if (MenuItem("Insert dir.."))   OpenDirFile(false);
			Separator();
			if (MenuItem("Exit"))  pWindow->close();
			EndMenu();
		}
		if (BeginMenu("Player"))
		{
			MenuWnd(WO_AppAudio);
			MenuWnd(WO_AppKeys);
			Separator();
			MenuWnd(WO_AppAbout);
			MenuWnd(WO_AppHelp);
			MenuWnd(WO_Main);
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
