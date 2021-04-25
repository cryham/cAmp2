#include "AppActions.h"
#include "App.h"
#include <SFML/Window/Event.hpp>
#include "../../libs/imgui.h"
using namespace std;  using namespace sf;

	
ActionsMap::ActionsMap(App* app)
	:pApp(app)
{
	FillNames();
	FillGroupNames();
	FillMethods();
	DefaultBindings();
}

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

void ActionsMap::Check(TModsKey modsKey)
{
	auto act = bindings[modsKey];
	if (act == Act_None)
		return;
	
	AppMethod met = methods[act];
	if (met != nullptr)
		(pApp->*met)();
}

string ActionsMap::StrKey(TModsKey k)
{
	if (k == 0)
		return "";
	string s;
	if ((k & 0x1000) >= 0x1000)  s += "Shift + ";
	if ((k & 0x2000) >= 0x2000)  s += "Ctrl + ";
	if ((k & 0x4000) >= 0x4000)  s += "Alt + ";
	s += csKeyNames[k & 0xFFF];
	return s;
}


bool KeysRow::Compare(const KeysRow& a, const KeysRow& b)
{
	const auto* spec = &sortSpecs->Specs[0];
	if (spec->SortDirection == ImGuiSortDirection_Ascending)
	switch (spec->ColumnUserID)
	{
	case Col_ID:    return a.Act < b.Act;
	case Col_Name:  return a.Name < b.Name;
	case Col_Keys:  return a.StrKeys < b.StrKeys;
	}else
	switch (spec->ColumnUserID)
	{
	case Col_ID:    return a.Act > b.Act;
	case Col_Name:  return a.Name > b.Name;
	case Col_Keys:  return a.StrKeys > b.StrKeys;
	}
	return a.Act < b.Act;
}
const ImGuiTableSortSpecs* KeysRow::sortSpecs = NULL;


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
		row.Name = a.second;
		
		string ss;  // todo: use multi_map?
		for (auto& b : bindings)
		{
			if (b.second == aid)
			{
				if (!ss.empty())
					ss += "\n";
				string s = ActionsMap::StrKey(b.first);
				ss += s;
				row.Keys.emplace_back(s);
				row.ModsKeys.emplace_back(b.first);
			}
		}
		row.StrKeys = ss;
		++n;
	}
}

// todo: from str, save load xml
