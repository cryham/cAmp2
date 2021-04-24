#pragma once
#include "../System/defines.h"
#include "AppActionsEnum.h"
#include <SFML/Window/Event.hpp>
#include <map>

class App;
typedef void(App::*AppMethod)(void);


typedef int TModsKey;

//  key bind  modifiers + key
static TModsKey ModsKey(bool shift, bool ctrl, bool alt, uint key)
{
	return	(shift ? 0x1000 : 0) + 
			(ctrl ? 0x2000 : 0) + 
			(alt ? 0x4000 : 0) + key;
}

class ActionsMap
{
public:
	App* pApp = nullptr;
	std::map<EAction, std::string> names;   // const, for Gui
	std::map<EAction, std::string> groups;  // 
	std::map<EAction, AppMethod> methods;   // 
	
	std::map<TModsKey, EAction> bindings;  // var key binds

	static const char* csKeyNames[sf::Keyboard::KeyCount];
	static std::string StrKey(TModsKey mkey);
	
	void FillNames(), FillGroupNames();
	void FillMethods();

public:	
	ActionsMap(App* app);
	
	void DefaultBindings();
	void Bind(bool shift, bool ctrl, bool alt, uint key, EAction act);
	void Bind2(bool shift, bool ctrl, bool alt, uint key, uint key2, EAction act);
	//  Unbinds with Act_None
	
	void Check(TModsKey modsKey);
};
