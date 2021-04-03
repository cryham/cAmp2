#pragma once
#include "App.h"
#include "AppConst.h"
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <memory>


class AppSFMLDraw : public App
{
public:
	bool Run();

protected:
	//  Run stages
	void CreateWindow();
	bool LoadResources();
	void LoopMain();
	void DestroyAll();

	//  Draw stages
	void DrawAll();
	void DrawPlayer();
	void DrawTabs();
	void DrawPlaylist();
	void DrawHeader();
	void DrawSlider();

	//----------------------------------------------------

	//  windows
	//--------------------------
	typedef std::unique_ptr<sf::RenderWindow> PWindow;
	PWindow pWindow = nullptr;
	PWindow pWindow2 = nullptr;

	//  resources
	//--------------------------
	std::unique_ptr<sf::Texture> pTexture = nullptr;
	std::unique_ptr<sf::Sprite> pBackgr = nullptr;

	enum EFont
	{	Fnt_Info=0, Fnt_Track, Fnt_Time, Fnt_TimeBig, Fnt_All  };

	std::unique_ptr<sf::Font> pFont[Fnt_All] = {nullptr};
	sf::Text text[Fnt_All];
	void SetupGuiStyle();  // ImGui theme
	
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
};
