#pragma once
#include "../System/defines.h"
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


//------------------------------------------------------------------
class ActionsMap
{
public:
	App* pApp = nullptr;
	std::map<EAction, std::string> names;   // const, for Gui
	std::map<EAction, std::string> groups;  // 
	std::map<EAction, AppMethod> methods;   // 
	
	std::map<TModsKey, EAction> bindings;  // key binds
	
	//  Gui
	std::vector<KeysRow> rows;  // gui list
	bool bGuiUpdate = true;
	bool binding = false;  // press to bind
	EAction bindAct = Act_None;

	static const char* csKeyNames[sf::Keyboard::KeyCount];
	static std::string StrKey(TModsKey mkey);
	
	void FillNames(), FillGroupNames();
	void FillMethods();

public:	
	ActionsMap(App* app);
	
	void DefaultBindings();
	void Bind(TModsKey mkey, EAction act);
	void Bind(bool shift, bool ctrl, bool alt, uint key, EAction act);
	void Bind2(bool shift, bool ctrl, bool alt, uint key, uint key2, EAction act);
	void Unbind(TModsKey mkey)  {  bindings[mkey] = Act_None;  }
	
	void Check(TModsKey modsKey);

	void UpdateGuiKeysList();
};


//------------------------------------------------------------------
enum ESortCol
{
    Col_ID, Col_Name, Col_Keys
};

struct KeysRow  // for gui list
{
    int Act;  // enum Act_
    std::string Name;  // action
    std::string StrKeys;  // for sort only
	
	std::vector<std::string> Keys;  // button text
	std::vector<TModsKey> ModsKeys;  // button key to unbind

    static const ImGuiTableSortSpecs* sortSpecs;

    static bool Compare(const KeysRow& a, const KeysRow& b);
};
