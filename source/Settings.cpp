#include <iostream>
#include <fstream>
#include <SFML/Window.hpp>
#include "../libs/tinyxml2.h"
#include "FileSystem.h"
#include "Settings.h"
#include "Util.h"
using namespace std;  using namespace tinyxml2;


//  ctor
Settings::Settings()
{
	Default();
}

void Settings::GetWndDim(sf::Window* wnd)
{
	view.xPos = wnd->getPosition().x;
	view.yPos = wnd->getPosition().y;
	view.xSize = wnd->getSize().x;
	view.ySize = wnd->getSize().y;
}


//  Defaults
//------------------------------------------------------------------------------------------------
void Settings::Default()
{
//	fSplit = 0.22f;
//	iFontH = 18;
//	iFontGui = 17;
//	iLineH = 2;

//	xPos = 0;  ywPos = 0;
//	xSize = 350;  ywSize = 768;

	escQuit = false;
}


///  Load
//------------------------------------------------------------------------------------------------
bool Settings::Load()
{
	Default();

	string file = FileSystem::Settings();
	if (!FileSystem::Exists(file))
		return false;

	const string sErr = string("Settings Load Error: ") + file + "\n";
	ifstream fi;
	fi.open(file);
	if (!fi.good())
	{	Log(sErr + "Can't open file.");  return false;  }
	
	XMLDocument doc;
	XMLError er = doc.LoadFile(file.c_str());
	if (er != XML_SUCCESS)
	{	Log(sErr + "Can't load file.");  return false;  }

	using tinyxml2::XMLElement;
	XMLElement* root = doc.RootElement();
	if (!root)
	{	Log(sErr + "No root.");  return false;  }
	string rn = root->Name();
	if (rn != "cAmp2")
	{	Log(sErr + "Root not cAmp2.");  return false;  }

	XMLElement* e;  const char* a;

	//  load
	e = root->FirstChildElement("window");
	if (e)
	{	a = e->Attribute("x");  if (a)  view.xPos = atoi(a);
		a = e->Attribute("y");  if (a)  view.yPos = atoi(a);
		a = e->Attribute("sx");  if (a)  view.xSize = atoi(a);
		a = e->Attribute("sy");  if (a)  view.ySize = atoi(a);
		a = e->Attribute("escQuit");  if (a)  escQuit = atoi(a) >0? true: false;
	}

//	e = root->FirstChildElement("dim");
//	if (e)
//	{	a = e->Attribute("fSplit");  if (a)  fSplit = atof(a);
//		a = e->Attribute("iFontH");  if (a)  iFontH = atoi(a);
//		a = e->Attribute("iLineH");  if (a)  iLineH = atoi(a);
//		a = e->Attribute("iFontGui");  if (a)  iFontGui = atoi(a);
//		//a = e->Attribute("merge");  if (a)  merge = atoi(a) > 0? true: false;
//	}

	//  paths
	//e = root->FirstChildElement("path");   if (e){  a = e->Attribute("p");  if (a)  path = a;  }

	Log("Settings Loaded.");
	return true;
}

///  Save
//------------------------------------------------------------------------------------------------
bool Settings::Save()
{
	using tinyxml2::XMLElement;
	XMLDocument xml;
	XMLElement* root = xml.NewElement("cAmp2");
	root->SetAttribute("ver", ver);
	XMLElement* e;

	e = xml.NewElement("window");
		e->SetAttribute("x", view.xPos);
		e->SetAttribute("y", view.yPos);
		e->SetAttribute("sx", view.xSize);
		e->SetAttribute("sy", view.ySize);
		e->SetAttribute("escQuit", escQuit ? 1 : 0);
	root->InsertEndChild(e);

//	e = xml.NewElement("dim");
//		e->SetAttribute("fSplit", f2s(fSplit,3).c_str());
//		e->SetAttribute("iFontH", iFontH);
//		e->SetAttribute("iLineH", iLineH);
//		e->SetAttribute("iFontGui", iFontGui);
//	root->InsertEndChild(e);

	//  paths
	//e = xml.NewElement("path");   e->SetAttribute("p", path);  root->InsertEndChild(e);
	
	xml.InsertEndChild(root);
	
	string file = FileSystem::Settings();
	bool ok = xml.SaveFile(file.c_str()) == XML_SUCCESS;
	if (ok)
		Log("Settings Saved.");
	else
		Error("Error saving Settings file: " + file);
	return ok;
}
