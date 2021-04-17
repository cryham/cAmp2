#pragma once
#include <stdint.h>


enum EActions  // app key actions
{
	
};

class SModifier
{
	const static uint8_t modAlt = 4, modCtrl = 2, modShift = 1;  // bits
public:	
	uint8_t mod = 0;

	void Set(bool alt, bool ctrl, bool shift)
	{
		mod = (alt ? modAlt : 0) + 
			  (ctrl ? modCtrl : 0) + 
			  (shift ? modShift : 0);
	}
	void Get(bool& alt, bool& ctrl, bool& shift)
	{
		alt = mod & modAlt > 0;
		ctrl = mod & modCtrl > 0;
		shift = mod & modShift > 0;
	}
	bool Matches(const SModifier& mod1)
	{
		return mod == mod1.mod;
	}
};
		
struct SActionKey
{
	SModifier mod;
	//sf::KeyCode key = sf::KeyCode::Unknown;
	int key = -1;
	
	bool Matches(SModifier mod1, int key1)
	{
		return key == key1 && mod.Matches(mod1);
	}
};
