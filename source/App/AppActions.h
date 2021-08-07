#pragma once
#include "../System/defines.h"
#include "../System/LogFile.h"
#include "AppActionsEnum.h"
#include <SFML/Window/Event.hpp>
#include <map>
#include <vector>

class App;
typedef void(App::*AppMethod)(void);
struct ImGuiTableSortSpecs;
struct KeysRow;


//------------------------------------------------------------------
typedef int TModsKey;

//  key bind  modifiers + key
static TModsKey ModsKey(bool shift, bool ctrl, bool alt, uint key)
{
	return	(shift ? 0x1000 : 0) + 
			(ctrl ? 0x2000 : 0) + 
			(alt ? 0x4000 : 0) + key;
}


//  Key binds to app actions
//------------------------------------------------------------------
class ActionsMap : public Logger
{
	App* pApp = nullptr;
	std::map<EAction, std::string> names;   // const, for Gui and keys.xml
	std::map<EAction, std::string> groups;  // actions which start a group
	std::map<EAction, AppMethod> methods;   // App method for action
	
	std::map<TModsKey, EAction> bindings;  // var, key binds, in keys.xml
	
	//  Gui
	std::vector<KeysRow> rows;  // gui list
	bool bGuiUpdate = true;
	bool binding = false;  // press to bind
	EAction bindAct = Act_None;

	static const char* csKeyNames[sf::Keyboard::KeyCount];
	static std::string StrKey(TModsKey mkey);
	
	//  Init const
	void FillNames(), FillGroupNames();
	void FillMethods();

public:	
	ActionsMap(App* app);
	bool Load();
	bool Save() const;
	
	void DefaultBindings();
	
	void Bind(TModsKey mkey, EAction act);
	void Bind(bool shift, bool ctrl, bool alt, uint key, EAction act);
	void Bind2(bool shift, bool ctrl, bool alt, uint key, uint key2, EAction act);
	void Unbind(TModsKey mkey)  {  bindings.erase(mkey);  }
	
	void KeyPress(TModsKey modsKey);

	std::string sFind;
	void UpdateGui()  {  bGuiUpdate = true;  }
	void UpdateGuiKeysList();
	void SortGui();

	void GuiBindStart(EAction act);
	void GuiBindEnd(TModsKey mkey);
	void GuiBindCancel()    {  binding = false;  }
	bool IsGuiBind()        {  return binding;   }
	bool IsGuiBind(int act) {  return binding && bindAct == act;  }
	
	int RowsCount()  {  return (int)rows.size();  }
	const KeysRow& GetRow(int id)   {  return rows[id];   }
	std::string GetGroup(EAction a) {  return groups[a];  }
	EAction GetActFromName(std::string name);
};


//------------------------------------------------------------------
enum ESortCol
{
    Col_ID, Col_Name, Col_Keys
};

struct KeysRow  // for gui list
{
    int Act;  // enum Act_
    std::string name;  // action
    std::string strKeys;  // for sort only
	
	std::vector<std::string> keys;  // button's texts
	std::vector<TModsKey> modsKeys;  // button's keys to unbind

    static const ImGuiTableSortSpecs* sortSpecs;
    static bool Compare(const KeysRow& a, const KeysRow& b);
};
