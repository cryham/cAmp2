#include "AppSFML_Draw.h"
#include "../App/AppActions.h"
#include "../System/FileSystem.h"
#include "../System/Utilities.h"
#include "../../libs/imgui.h"
#include "../../libs/imgui-SFML.h"
using namespace sf;  using namespace std;  using namespace ImGui;


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
	for (int w=0; w < WO_ALL; ++w)
		vWindows[w] = nullptr;
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
		for (int i=0; i < WO_ALL; ++i)
			WndClose(i);
	}
	if (w < 0 || w >= WO_ALL)  return;
	auto& wnd = vWindows[w];
	if (wnd)  return;
	const auto& wc = wndConst[w];

	wnd = make_unique<RenderWindow>(
		VideoMode(wc.width, wc.height), wc.FullTitle(),
		Style::Default, ContextSettings());
	wnd->clear();
	wndSizeX = wc.width;  wndSizeY = wc.height;
	
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

void AppSFMLDraw::WndClose(int w)
{
	auto& wnd = vWindows[w];
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
	for (int w=0; w < WO_ALL; ++w)
	{
		if (!WndVisible((EWndOpt)w))  continue;
		auto& wnd = vWindows[w];

		Event e;
		while (wnd->pollEvent(e))
		{
			ImGui::SFML::ProcessEvent(e);
		
			switch (e.type)
			{
			case Event::KeyPressed:
				WndKeyPress(w, e.key);
				break;
			case Event::Closed:
				WndClose(w);
				return;
			
			case Event::Resized:
				FloatRect r(0, 0, e.size.width, e.size.height);
				wnd->setView(sf::View(r));
				wndSizeX = e.size.width;
				wndSizeY = e.size.height;

				//  save new size
				SetViewFromWnd();
				UpdDim();
				break;
			}
		}
	}
}

//  Key press
void AppSFMLDraw::WndKeyPress(int w, Event::KeyEvent k)
{
	#define key(m)  k.code == Keyboard::m
	if (key(LAlt) || key(RAlt) ||
		key(LControl) || key(RControl) ||
		key(LShift) || key(RShift) ||
		key(LSystem) || key(RSystem))
		return;  // only modifier
	#undef key
	
	auto mkey = ModsKey(k.shift,k.control,k.alt, k.code);
	if (act->binding && w == WO_AppKeys)
	{
		act->binding = false;
		act->Bind(mkey, act->bindAct);
		act->bGuiUpdate = true;
	}
	else  // normal
		if (k.code == Keyboard::Escape)
			WndClose(w);
}
