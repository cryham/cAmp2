#include "AppSFML_Draw.h"
#include "../System/FileSystem.h"
#include "../System/Utilities.h"
#include "../../libs/imgui.h"
#include "../../libs/imgui-SFML.h"
using namespace sf;  using namespace std;  using namespace ImGui;


//  const windows data
const AppSFMLDraw::SWndConst AppSFMLDraw::wndConst[WO_All] = {
	{"Playlist Find", 320,250},
	{"Playlist Filter", 350,200},
	
	{"Playlist Tab", 400,520},
	{"Player Tabs", 350,320},
	
	{"Player View", 500,700},
	{"Playlist Statistics", 400,250},
	
	{"Player Audio", 400,300},
	{"Player Visualization", 900,600},
	
	{"Player Test", 300,200},
	{"About", 450,340},

	{"All Options windows", 250,400},
};

//  gui utils  -----
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

void AppSFMLDraw::TextG(const char* s) {  ImGui::Text(s);  }
void AppSFMLDraw::TextG(string s)      {  ImGui::Text(s.c_str());  }

//  text value slider
void AppSFMLDraw::SliderValue(const string& text, const string& val)
{
	if (xText == 0)
	{
		string s = text + val;
		TextG(s);
	}else
	{	TextG(text);  SameLine(xText);
		TextG(val);
	}
}
		
bool AppSFMLDraw::SliderF(
	float& val, float vmin, float vmax,
	const string& text, const string& uid, const string& value)
{
	float f = val;
	SliderValue(text, value.empty() ? f2s(f, 3, 5) : value);
	if (xSlider > 0)  SameLine(xSlider);
	
	bool e = SliderFloat(uid.c_str(), &f, vmin, vmax, "");
	if (e)  val = f;
	return e;
}

bool AppSFMLDraw::SliderI(
	int& val, float vmin, float vmax,
	const string& text, const string& uid, const string& value)
{
	int i = val;
	SliderValue(text, value.empty() ? i2s(i) : value);
	if (xSlider > 0)  SameLine(xSlider);

	bool e = SliderInt(uid.c_str(), &i, vmin, vmax, "");
	if (e)  val = i;
	return e;
}


//  ctor
//------------------------------------------------------------------
AppSFMLDraw::AppSFMLDraw()
{
	for (int i=0; i < WO_All; ++i)
		vWindows[i] = nullptr;
}

bool AppSFMLDraw::WndVisible(EWndOpt w)
{
	return vWindows[w] && vWindows[w]->isOpen();
}

void AppSFMLDraw::WndFocus()
{
	if (wndFocus)
	{	wndFocus = false;
		SetKeyboardFocusHere();
	}
}


//  Wnd Open
//------------------------------------------------------------------
void AppSFMLDraw::WndOpen(EWndOpt w, bool center)
{
	if (wndInited)  //return;
	// todo: resize same window cant focus ..
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
	if (center)
	{
		int width = (vm.width - wc.width) / 2,  // center screen
			height = (vm.height - wc.height) / 2;
		wnd->setPosition(Vector2i(width, height));
	}else
		wnd->setPosition(Vector2i(xm+10, max(ym - wc.height/2, 0)));
	
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
	wndFocus = true;
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

		case WO_AppView:	WndDraw_AppView();  break;
		case WO_AppStats:	WndDraw_AppStats();  break;
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
