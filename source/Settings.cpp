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

void Settings::SetDimFromWnd(sf::Window* wnd)
{
	view.xPos = wnd->getPosition().x;
	view.yPos = wnd->getPosition().y;
	view.xSize = wnd->getSize().x;
	view.ySize = wnd->getSize().y;
}


//  Defaults
//------------------------------------------------------------------------------------------------
void SetState::Default()
{
	bPlay = false;
	iVolume = 500;
	idPls = 0;  idPlay = 0;
}

void Settings::Default()
{
	view.Defaults();
	for (auto& v : views)
		v.Defaults();
	
	escQuit = false;
	bFileInfo = false;
	eDirView = DV_Path;

	cntrPls = 1;
	vSetPls.clear();
	
	state.Default();
}


//  cycle dir view modes  /
void Settings::NextDirView(int i)
{
	int dv = ((int)eDirView + i + DV_All) % DV_All;
	eDirView = (EDirView)dv;
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

	XMLElement* e,*m;  const char* a;

	//  load  ..
	e = root->FirstChildElement("debug");
	if (e)
	{
		a = e->Attribute("escQuit");  if (a)  escQuit = s2b(a);
		a = e->Attribute("dirView");  if (a)  eDirView = (EDirView)s2i(a);
	}
	//  state
	e = root->FirstChildElement("state");
	if (e)
	{
		a = e->Attribute("bPlay");    if (a)  state.bPlay = s2b(a);
		a = e->Attribute("iVolume");  if (a)  state.iVolume = s2i(a);
		a = e->Attribute("idPls");    if (a)  state.idPls = s2i(a);
		a = e->Attribute("idPlay");   if (a)  state.idPlay = s2i(a);
	}

	e = root->FirstChildElement("view");
	if (e)
		view.Load(e);

	//  playlists
	e = root->FirstChildElement("Playlists");
	if (!e)  Log(sErr + "No <Playlists>");
	if (e)
	{	a = e->Attribute("counter");	if (a)  cntrPls = s2i(a);
		
		m = e->FirstChildElement("Pls");
		if (!m)  Log(sErr + "No <Pls>");
		while (m)
		{
			SetPls sp;
			a = m->Attribute("name");	sp.name = a;
			a = m->Attribute("bookm");	if (a)  sp.bookm = s2i(a);

			vSetPls.push_back(sp);
			m = m->NextSiblingElement("Pls");
	}	}
	
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
	XMLElement* e, *p;

	//  save  ..
	e = xml.NewElement("debug");
		e->SetAttribute("escQuit", escQuit ? 1 : 0);
		e->SetAttribute("dirView", eDirView);
	root->InsertEndChild(e);

	//  state
	e = xml.NewElement("state");
		e->SetAttribute("bPlay",   state.bPlay ? 1 : 0);
		e->SetAttribute("iVolume", state.iVolume);
		e->SetAttribute("idPls",   state.idPls);
		e->SetAttribute("idPlay",  state.idPlay);
	root->InsertEndChild(e);

	e = xml.NewElement("view");
		view.Save(e);
	root->InsertEndChild(e);

	//  playlists
	e = xml.NewElement("Playlists");
	e->SetAttribute("counter", cntrPls);
	
	for (const auto& s: vSetPls)
	{
		p = xml.NewElement("Pls");
		p->SetAttribute("name", s.name.c_str());
		p->SetAttribute("bookm", s.bookm);
		// idx,cur,ofs on each playlist here?
		e->InsertEndChild(p);
	}
	root->InsertEndChild(e);
	
	
	xml.InsertEndChild(root);
	
	string file = FileSystem::Settings();
	bool ok = xml.SaveFile(file.c_str()) == XML_SUCCESS;
	if (ok)
		Log("Settings Saved.");
	else
		Error("Error saving Settings file: " + file);
	return ok;
}
