#include "AppSFML_Draw.h"
#include "../Audio/Audio.h"
#include "../System/Utilities.h"
#include "../Settings/Settings.h"
#include "../Settings/VisualColors.h"
#include "../../libs/imgui.h"
#include "../../libs/imgui-SFML.h"
using namespace sf;  using namespace std;  using namespace ImGui;


//  About
//------------------------------------------------------------------
void AppSFMLDraw::WndDraw_AppAbout()
{
	Sep(20);
	TextColored(ImVec4(0.5f, 0.97f, 1.0f, 1.f), "Crystal AMP 2  (cAmp2)");
	Sep(10);
	TextColored(ImVec4(0.7f, 0.67f, 1.0f, 1.f), "by Crystal Hammer");
	Sep(20);
	auto s = "Version: "+ i2s(Settings::version/100) + "." + i2s(Settings::version % 100);
	TextColored(ImVec4(0.8f, 0.9f, 1.0f, 1.f), s.c_str());
	Sep(40);
	PushItemWidth(400);

	TextG("Sources, Issues etc.");
	auto url1 = "https://github.com/cryham/cAmp2";
	if (Button(url1))  OpenBrowserUrl(url1);

	Sep(10);
	TextG("Project description");
	auto url2 = "https://cryham.org/portfolio/2020-camp2/";
	if (Button(url2))  OpenBrowserUrl(url2);
	
	PopItemWidth();
}


//  Help, mouse
//------------------------------------------------------------------
void AppSFMLDraw::WndDraw_AppHelp()
{
	const int x = 150;
	const ImVec4 c{0.5f, 0.75f, 1.0f, 1.f}, w{1.f, 1.f, 1.f, 1.f};

	TextG("Help with Mouse actions on areas");
	Sep(20);
	TextColored(c, "Player");
	Line();
	TextG("LMB above");  SameLine(x);  TextG("left: Previous, right: Next track");
	TextG("RMB above");  SameLine(x);  TextG("left: Dec, right: Increase track rating");
	TextG("LMB below");  SameLine(x);  TextG("Seek track time");
	TextG("shift-MMB");  SameLine(x);  TextG("Change visualization height");

	Sep(20);
	TextColored(c, "Tabs");
	Line();
	TextG("RMB");  SameLine(x);  TextG("Open: Tab properties");
	TextG("ctrl-LMB");  SameLine(x);  TextG("Move tab here");

	Sep(20);
	TextColored(c, "Tabs Arrows on right ^ v");
	Line();
	TextG("RMB");  SameLine(x);  TextG("Open: All Tabs settings");
	TextG("ctrl-LMB");  SameLine(x);  TextG("Dec/Increase rows count");
	TextG("shift-LMB");  SameLine(x);  TextG("Dec/Increase columns count");
	TextG("LMB");  SameLine(x);  TextG("Dec/Inc tabs offset");

	Sep(20);
	TextColored(c, "Playlist Header");
	Line();
	TextG("RMB in middle");  SameLine(x);  TextG("Open: Playlist Filter");
	TextG("RMB left or right");  SameLine(x);  TextG("Open: Playlist Statistics");
	
	Sep(20);
	TextColored(c, "Playlist");
	Line();
	TextG("RMB");  SameLine(x);  TextColored(w, "Play track under cursor");
	TextG("RMB on slider");  SameLine(x);  TextG("Open: View");
	TextG("LMB");  SameLine(x);  TextG("Pick track");
	TextG("ctrl-LMB");  SameLine(x);  TextG("Select track");
	TextG("shift-LMB");  SameLine(x);  TextG("Select range of tracks");
	TextG("MMB");  SameLine(x);  TextG("Accelerated move playlist");
}
