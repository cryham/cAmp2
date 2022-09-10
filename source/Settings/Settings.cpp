#include <fstream>
#include <SFML/Window.hpp>
#include "../../libs/tinyxml2.h"
#include "Settings.h"
#include "../System/FileSystem.h"
#include "../System/Utilities.h"
using namespace std;  using namespace tinyxml2;


//  ctor
Settings::Settings()
{
	Default();
}


//  Defaults
//------------------------------------------------------------------------------------------------
void SetState::Default()
{
	bPlay = false;
	iVolume = 500;

	idPls = 0;  idPlay = 0;  idQue = -1;

	bRepPls = true;
	bDirNext = true;
}

void SetFind::Default()
{
	bCaseSens = false;
	bFullPath = false;
	bUnfiltered = true;
	bAllPls = true;
}

void Settings::Default()
{
	view.Defaults();
	for (auto& v : views)
		v.Defaults();
	
	escQuit = false;
	bFileInfo = false;
	eDirView = DirV_Path;
	iLastView = 1;

	cntrPls = 1;
	vSetPls.clear();
	find.Default();
	
	state.Default();
	dimTabTxt = 16;  dimTabBck = 16;
	iLastWnd = WO_AppAbout;
}


//  cycle dir view modes  /
void Settings::NextDirView(int i)
{
	int dv = ((int)eDirView + i + DirV_ALL) % DirV_ALL;
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

	//  load  .......
	//  debug
	e = root->FirstChildElement("debug");
	if (e)
	{
		a = e->Attribute("escQuit");  if (a)  escQuit = s2b(a);
	}
	//  app view
	e = root->FirstChildElement("appView");
	if (e)
	{
		a = e->Attribute("last");     if (a)  iLastView = s2i(a);
		a = e->Attribute("lastWnd");  if (a)  iLastWnd = s2i(a);
		a = e->Attribute("dirView");  if (a)  eDirView = (EDirView)s2i(a);
		a = e->Attribute("tbBck");    if (a)  dimTabBck = s2i(a);
		a = e->Attribute("tbTxt");    if (a)  dimTabTxt = s2i(a);
	}
	//  state
	e = root->FirstChildElement("state");
	if (e)
	{
		a = e->Attribute("bPlay");    if (a)  state.bPlay = s2b(a);
		a = e->Attribute("iVolume");  if (a)  state.iVolume = s2i(a);
		
		a = e->Attribute("idPls");    if (a)  state.idPls = s2i(a);
		a = e->Attribute("idPlay");   if (a)  state.idPlay = s2i(a);
		a = e->Attribute("idQue");    if (a)  state.idQue = s2i(a);

		a = e->Attribute("bRepPls");  if (a)  state.bRepPls = s2b(a);
		a = e->Attribute("bDirNext"); if (a)  state.bDirNext = s2b(a);
	}
	//  find
	e = root->FirstChildElement("find");
	if (e)
	{
		a = e->Attribute("case");    if (a)  find.bCaseSens = s2b(a);  
		a = e->Attribute("path");    if (a)  find.bFullPath = s2b(a);  
		a = e->Attribute("unfilt");  if (a)  find.bUnfiltered = s2b(a);
		a = e->Attribute("all");     if (a)  find.bAllPls = s2b(a);    
	}

	//  view
	e = root->FirstChildElement("view");
	if (e)
		view.Load(e);

	//  playlists
	e = root->FirstChildElement("Playlists");
	if (!e)  Log(sErr + "No <Playlists>");
	if (e)
	{	a = e->Attribute("counter");	if (a)  cntrPls = s2i(a);
		
		m = e->FirstChildElement("Pls");
		if (!m)  Log(sErr + "No <Pls> in <Playlists>");
		while (m)
		{
			a = m->Attribute("name");
			if (a)  vSetPls.emplace_back(a);
			m = m->NextSiblingElement("Pls");
	}	}
	
	//  views all
	e = root->FirstChildElement("Views");
	if (!e)  Log(sErr + "No <Views>");
	if (e)
	{
		m = e->FirstChildElement("View");
		if (!m)  Log(sErr + "No <View> in <Views>");
		
		int i = 0;
		while (m && i < cMaxViews)
		{
			views[i].Load(m);
			m = m->NextSiblingElement("View");  ++i;
	}	}
	
	Log("Settings Loaded, from: " + file);
	return true;
}


///  Save
//------------------------------------------------------------------------------------------------
bool Settings::Save() const
{
	using tinyxml2::XMLElement;
	XMLDocument xml;
	XMLElement* root = xml.NewElement("cAmp2");
	root->SetAttribute("ver", version);
	XMLElement* e, *p;

	//  debug
	e = xml.NewElement("debug");
		e->SetAttribute("escQuit", escQuit ? 1 : 0);
	root->InsertEndChild(e);

	//  app view
	e = xml.NewElement("appView");
		e->SetAttribute("last",    iLastView);
		e->SetAttribute("lastWnd", iLastWnd);
		e->SetAttribute("dirView", eDirView);
		e->SetAttribute("tbBck",   dimTabBck);
		e->SetAttribute("tbTxt",   dimTabTxt);
	root->InsertEndChild(e);
	
	//  state
	e = xml.NewElement("state");
		e->SetAttribute("bPlay",    state.bPlay ? 1 : 0);
		e->SetAttribute("iVolume",  state.iVolume);
 
		e->SetAttribute("idPls",    state.idPls);
		e->SetAttribute("idPlay",   state.idPlay);
		e->SetAttribute("idQue",    state.idQue);
 
		e->SetAttribute("bRepPls",  state.bRepPls);
		e->SetAttribute("bDirNext", state.bDirNext);
	root->InsertEndChild(e);
	
	//  find
	e = xml.NewElement("find");
		e->SetAttribute("case",   find.bCaseSens ? 1 : 0);
		e->SetAttribute("path",   find.bFullPath ? 1 : 0);
		e->SetAttribute("unfilt", find.bUnfiltered ? 1 : 0);
		e->SetAttribute("all",    find.bAllPls ? 1 : 0);
	root->InsertEndChild(e);

	//  view
	e = xml.NewElement("view");
		view.Save(e, &xml);
	root->InsertEndChild(e);
	

	//  playlists
	e = xml.NewElement("Playlists");
	e->SetAttribute("counter", cntrPls);
	
	for (const auto& s : vSetPls)
	{
		p = xml.NewElement("Pls");
		p->SetAttribute("name", s.c_str());
		e->InsertEndChild(p);
	}
	root->InsertEndChild(e);
	
	
	//  views all
	e = xml.NewElement("Views");
	for (const auto& v : views)
	{
		p = xml.NewElement("View");
		v.Save(p, &xml);
		e->InsertEndChild(p);
	}
	root->InsertEndChild(e);
	
	
	//  save  .......
	xml.InsertEndChild(root);
	
	string file = FileSystem::Settings();
	bool ok = xml.SaveFile(file.c_str()) == XML_SUCCESS;
	if (ok)
		Log("Settings Saved.");
	else
		Error("Error saving Settings file: " + file);
	return ok;
}
