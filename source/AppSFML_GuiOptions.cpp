#include "../libs/imgui.h"
#include "../libs/imgui-SFML.h"
#include "AppSFML_Draw.h"
#include "FileSystem.h"
#include "Util.h"
using namespace sf;  using namespace std;  using namespace ImGui;


bool AppSFMLDraw::WndVisible(EWndOpt w)
{
	return vWindows[w].ptr && vWindows[w].ptr->isOpen();
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

//  ctor
//------------------------------------------------------------------
AppSFMLDraw::AppSFMLDraw()
{
	vWindows.reserve(WO_All);
	for (int i=0; i < WO_All; ++i)
	{
		SWndOpt w;
		w.width = 400;  // dim
		w.height = 300;
		switch (i)
		{
		case WO_PlsFind:	w.title = "Playlist Find";  break;
		case WO_PlsFilter:	w.title = "Playlist Filter";  break;
		case WO_PlsTab:		w.title = "Playlist Tab";  w.height = 600;  break;
		case WO_AppShow:	w.title = "Player Show";  /*w.height = 400;*/  break;
		case WO_AppAudio:	w.title = "Player Audio";  break;
		case WO_AppVis:		w.title = "Player Visualization";  break;
		case WO_AppTest:	w.title = "Player Test";  break;
		}
		vWindows.emplace_back(move(w));
	}
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
	if (wnd.ptr)  return;
	
	wnd.ptr = make_unique<RenderWindow>(
		VideoMode(wnd.width, wnd.height),
		wnd.title,
		Style::Default, ContextSettings());
	
	VideoMode vm = VideoMode::getDesktopMode();
	int width = (vm.width - wnd.width) / 2,  // center screen
		height = (vm.height - wnd.height) / 2;
	wnd.ptr->setPosition(Vector2i(width, height));
	
	wnd.ptr->setVerticalSyncEnabled(true);
	wnd.ptr->setIcon(32, 32, icon.getPixelsPtr());
				
	//  ImGui Font
	//------------------
	wndInited = true;
	ImGui::SFML::Init(*wnd.ptr.get(), false);
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
	if (!wnd.ptr)  return;
	
	wnd.ptr->close();
	wnd.ptr = nullptr;  // deletes

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
		while (wnd.ptr->pollEvent(e))
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
		ImGui::SFML::Update(*wnd.ptr, time);

		//  controls  wnd  =====
		SetNextWindowPos( ImVec2(0, 0),  ImGuiCond_Always);
		SetNextWindowSize(ImVec2(wnd.width, wnd.height), ImGuiCond_Always);
	
		bool open = true;
		const int wfl = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize;
		Begin("Controls", &open, wfl);

		switch (i)
		{
		case WO_PlsFind:	WndDraw_PlsFind();  break;
		case WO_PlsFilter:	WndDraw_PlsFilter();  break;
		case WO_PlsTab:		WndDraw_PlsTab();  break;
		case WO_AppShow:	WndDraw_AppShow();  break;
		case WO_AppAudio:	WndDraw_AppAudio();  break;
		case WO_AppVis:		WndDraw_AppVis();  break;
		case WO_AppTest:	WndDraw_AppTest();  break;
		}

		End();

		wnd.ptr->clear();
		ImGui::SFML::Render(*wnd.ptr.get());
	
		wnd.ptr->display();
	}
}
