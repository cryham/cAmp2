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

	string find = sFind;  strlower(find);

	for (auto& a : names)
	{
		bool found = find.empty();
		if (!found)
		{
			string name = a.second;  strlower(name);
			found = name.find(find) != string::npos;
		}
		if (!found)
			continue;

		KeysRow row;
		auto actId = a.first;  // enum Act_
		row.Act = actId;
		row.name = a.second;

		string ss;  // todo: use multi_map?
		for (auto& b : bindings)
		{
			if (b.second == actId)
			{
				if (!ss.empty())
					ss += "\n";
				string s = ActionsMap::StrKey(b.first);
				ss += s;
				// todo: search keys too?
				
				row.keys.emplace_back(s);
				row.modsKeys.emplace_back(b.first);
			}
		}
		row.strKeys = ss;
		rows.emplace_back(row);
	}
}

//  map util
EAction ActionsMap::GetActFromName(string name)
{
	for (const auto& n : names)
		if (n.second == name)
			return n.first;
	return Act_None;
}

std::string ActionsMap::GetKeyForAct(EAction a)
{
	for (const auto& b : bindings)
	if (b.second == a)
		return StrKey(b.first);
	return "";
}


///  Load
//------------------------------------------------------------------------------------------------
bool ActionsMap::Load()
{
	bindings.clear();

	string file = FileSystem::Keys();
	if (!FileSystem::Exists(file))
		return false;

	const string sErr = string("Load Error keys.xml: ") + file + "\n";
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
	int cnt = 0;
	e = root->FirstChildElement("keys");
	if (!e)  Log(sErr + "No <keys>");
	if (e)
	{
		m = e->FirstChildElement("k");
		if (!m)  Log(sErr + "No <k> in <keys>");
		while (m)
		{
			EAction act = Act_None;
			a = m->Attribute("a");
			if (a)
				act = GetActFromName(a);

			a = m->Attribute("key");
			TModsKey mkey = s2i(a);
			
			bindings[mkey] = act;  // bind add
			++cnt;

			m = m->NextSiblingElement("k");
	}	}
	
	Log("Keys Loaded, from: " + file);
	bGuiUpdate = true;
	return cnt > 1;
}


///  Save
//------------------------------------------------------------------------------------------------
bool ActionsMap::Save() const
{
	using tinyxml2::XMLElement;
	XMLDocument xml;
	const string sErr = string("Error saving keys.xml file: ");

	XMLElement* root = xml.NewElement("cAmp2");
	root->SetAttribute("ver", Settings::version);
	XMLElement* e, *p;

	//  playlists
	e = xml.NewElement("keys");
	
	for (const auto& b : bindings)
	{
		EAction a = b.second;
		p = xml.NewElement("k");
		p->SetAttribute("key", b.first);

		auto fn = names.find(a);
		if (fn != names.end())
			p->SetAttribute("a", fn->second.c_str());
		else  // forgot to add in FillNames
			Error(sErr + "Action name not found for id: " + i2s(a));

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
		Error(sErr + file);
	return ok;
}
