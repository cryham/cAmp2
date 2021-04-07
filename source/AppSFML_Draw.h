#pragma once
#include "App.h"
#include "AppConst.h"
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <memory>


class AppSFMLDraw : public App
{
public:
	AppSFMLDraw();
	bool Run();

protected:
	//  Run stages
	void CreateWindow();
	bool LoadResources(), CreateTextures(), LoadFonts();
	
	void LoopMain();
	void DestroyAll();

	//  Draw stages
	void DrawAll();
	void DrawPlayer(), DrawTabs(), DrawPlaylist();
	void DrawPlsHeader(), DrawPlsSlider();

	
	//  Windows Gui
	//----------------------------------------------------
	typedef std::unique_ptr<sf::RenderWindow> PWindow;
	PWindow pWindow = nullptr;  // main window

	//  Options windows
	PWindow vWindows[WO_All] = {nullptr,};
	struct SWndConst
	{
		std::string title = "no title";
		int width = 400, height = 300;
	};
	const static SWndConst wndConst[WO_All];

	bool wndInited = false;
	EWndOpt wndOpen = WO_All;  // open next from Main

	void WndOpen(EWndOpt type, bool center = true) override;
	bool WndVisible(EWndOpt w);
	void WndProcessAll();
	void WndClose(int w);
	
	void WndDrawAll(sf::Time time);
	void WndDraw_PlsFind(), WndDraw_PlsFilter(), WndDraw_PlsTab(), WndDraw_AppTabs();
	void WndDraw_AppViewStats(), WndDraw_AppAudio(), WndDraw_AppVis();
	void WndDraw_AppTest(), WndDraw_AppAbout(), WndDraw_Main();


	//  vis spectrogram
	std::unique_ptr<sf::Texture> pVisTexture = nullptr;
	std::unique_ptr<sf::Sprite> pVisSprite = nullptr;

	
	//  resources
	//--------------------------
	std::unique_ptr<sf::Texture> pTexture = nullptr;
	std::unique_ptr<sf::Sprite> pBackgr = nullptr;

	enum EFont
	{	Fnt_Info, Fnt_Track, Fnt_Time, Fnt_TimeBig, Fnt_All  };

	std::unique_ptr<sf::Font> pFont[Fnt_All] = {nullptr};
	sf::Text text[Fnt_All];
	sf::Image icon;
	
	//  text and color
	//--------------------------
	sf::String str;
	sf::Color clr;
	bool bold = false;

	void Clr(sf::Uint8 r, sf::Uint8 g, sf::Uint8 b)
	{
		clr = sf::Color(r,g,b);
	}
	sf::String Str(const std::string& s);

	///  draw utils
	//--------------------------
	//  write out text, from str
	//  returns width, x advance
	int Text(EFont n, int x, int y, bool draw=true);

	void Format(const char* format, ...);
	
	//  draw textured rect, stretched at
	//  x,y pos, width,height,  texture coords uv start,size,  color rgb
	void RectUV(int x, int y, int w, int h,  int ux, int uy, int uw, int uh,  bool add=false,
			  sf::Uint8 r=255, sf::Uint8 g=255, sf::Uint8 b=255);
	void Rect(int x, int y, int w, int h,  ETexUV uv,  bool add=false,
			  sf::Uint8 r=255, sf::Uint8 g=255, sf::Uint8 b=255);

	///  Gui utils
	//--------------------------
	void TextG(const char* s);
	void TextG(std::string s);
	void Sep(int h), Line();  // separators

	void SetupGuiStyle();  // ImGui theme
};
