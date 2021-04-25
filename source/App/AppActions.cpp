#include "AppActions.h"
#include "App.h"
#include <SFML/Window/Event.hpp>
#include "../../libs/imgui.h"

#include "../../libs/tinyxml2.h"
#include "../System/FileSystem.h"
#include "../System/Utilities.h"
using namespace std;  using namespace sf;  using namespace tinyxml2;


//  ctor  Init const
ActionsMap::ActionsMap(App* app)
	:pApp(app)
{
	FillNames();
	FillGroupNames();
	FillMethods();
	DefaultBindings();
}

//  bind
void ActionsMap::Bind(TModsKey mkey, EAction act)
{
	bindings[mkey] = act;
}
void ActionsMap::Bind(bool shift, bool ctrl, bool alt, uint key, EAction act)
{
	Bind(ModsKey(shift, ctrl, alt, key), act);
}
void ActionsMap::Bind2(bool shift, bool ctrl, bool alt, uint key, uint key2, EAction act)
{
	Bind(ModsKey(shift, ctrl, alt, key), act);
	Bind(ModsKey(shift, ctrl, alt, key2), act);
}

//  gui bind
void ActionsMap::GuiBindEnd(TModsKey mkey)
{
	binding = false;
	Bind(mkey, bindAct);
	UpdateGui();
}
void ActionsMap::GuiBindStart(EAction act)
{
	binding = true;
	bindAct = act;
}

//  check if key bound to action and if so do its method
void ActionsMap::KeyPress(TModsKey modsKey)
{
	auto act = bindings[modsKey];
	if (act == Act_None)
		return;
	
	AppMethod met = methods[act];
	if (met != nullptr)
		(pApp->*met)();
}

string ActionsMap::StrKey(TModsKey k)  // for gui
{
	string s;
	if ((k & 0x1000) >= 0x1000)  s += "Shift + ";
	if ((k & 0x2000) >= 0x2000)  s += "Ctrl + ";
	if ((k & 0x4000) >= 0x4000)  s += "Alt + ";
	s += csKeyNames[k & 0xFFF];
	return s;
}


//  gui sort list
bool KeysRow::Compare(const KeysRow& a, const KeysRow& b)
{
	const auto* spec = &sortSpecs->Specs[0];
	if (spec->SortDirection == ImGuiSortDirection_Ascending)
	switch (spec->ColumnUserID)
	{
	case Col_ID:    return a.Act < b.Act;
	case Col_Name:  return a.name < b.name;
	case Col_Keys:  return a.strKeys < b.strKeys;
	}else
	switch (spec->ColumnUserID)
	{
	case Col_ID:    return a.Act > b.Act;
	case Col_Name:  return a.name > b.name;
	case Col_Keys:  return a.strKeys > b.strKeys;
	}
	return a.Act < b.Act;
}
const ImGuiTableSortSpecs* KeysRow::sortSpecs = NULL;

void ActionsMap::SortGui()
{
	if (rows.size() > 1)
		stable_sort(rows.begin(), rows.end(), KeysRow::Compare);
}


//  gui keys list
void ActionsMap::UpdateGuiKeysList()
{
	if (!bGuiUpdate)  return;
	bGuiUpdate = false;

	rows.clear();
	rows.resize(names.size(), KeysRow());

	int n = 0;
	for (auto& a : names)
	{
		KeysRow& row = rows[n];
		auto aid = a.first;  // enum Act_
		row.Act = aid;
		row.name = a.second;
		
		string ss;  // todo: use multi_map?
		for (auto& b : bindings)
		{
			if (b.second == aid)
			{
				if (!ss.empty())
					ss += "\n";
				string s = ActionsMap::StrKey(b.first);
				ss += s;
				
				row.keys.emplace_back(s);
				row.modsKeys.emplace_back(b.first);
			}
		}
		row.strKeys = ss;
		++n;
	}
}


///  Load
//------------------------------------------------------------------------------------------------
bool ActionsMap::Load()
{
	//bindings.clear(); //?
	DefaultBindings();

	string file = FileSystem::Keys();
	if (!FileSystem::Exists(file))
		return false;

	const string sErr = string("Keys Load Error: ") + file + "\n";
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

	//  playlists
	e = root->FirstChildElement("keys");
	if (!e)  Log(sErr + "No <keys>");
	if (e)
	{
		m = e->FirstChildElement("k");
		if (!m)  Log(sErr + "No <k> in <keys>");
		while (m)
		{
			a = m->Attribute("act");
			if (!a)  {  Log(sErr + "No act in <k>");  }
			EAction act = (EAction)s2i(a);
			
			a = m->Attribute("key");
			TModsKey mkey = s2i(a);
			
			bindings[mkey] = act;  // bind add

			m = m->NextSiblingElement("k");
	}	}
	
	Log("Keys Loaded, from: " + file);
	bGuiUpdate = true;
	return true;
}


///  Save
//------------------------------------------------------------------------------------------------
bool ActionsMap::Save() const
{
	using tinyxml2::XMLElement;
	XMLDocument xml;
	XMLElement* root = xml.NewElement("cAmp2");
	root->SetAttribute("ver", Settings::ver);
	XMLElement* e, *p;

	//  playlists
	e = xml.NewElement("keys");
	
	for (const auto& b : bindings)
	{
		p = xml.NewElement("k");
		p->SetAttribute("key", b.first);
		p->SetAttribute("act", b.second);
		e->InsertEndChild(p);
	}
	root->InsertEndChild(e);
	
	
	//  save  .......
	xml.InsertEndChild(root);
	
	string file = FileSystem::Keys();
	bool ok = xml.SaveFile(file.c_str()) == XML_SUCCESS;
	if (ok)
		Log("Keys Saved.");
	else
		Error("Error saving Keys file: " + file);
	return ok;
}
