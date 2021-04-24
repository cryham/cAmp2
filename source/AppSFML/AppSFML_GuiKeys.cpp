#include "AppSFML_Draw.h"
#include "../Audio/Audio.h"
#include "../System/Utilities.h"
#include "../Settings/Settings.h"
#include "../Settings/VisualColors.h"
#include "../App/AppActions.h"
#include "../../libs/imgui.h"
#include "../../libs/imgui-SFML.h"
using namespace sf;  using namespace std;  using namespace ImGui;


//------------------------------------------------------------------
enum EColID
{
    Col_ID, Col_Name, Col_Keys
};

struct KeysItem  // for gui list
{
    int ID;
    string Name;
    string Keys;

    static const ImGuiTableSortSpecs* sortSpecs;

    static bool Compare(const KeysItem& a, const KeysItem& b)
    {
		const ImGuiTableColumnSortSpecs* sort_spec = &sortSpecs->Specs[0];
		if (sort_spec->SortDirection == ImGuiSortDirection_Ascending)
		switch (sort_spec->ColumnUserID)
		{
		case Col_ID:    return a.ID < b.ID;
		case Col_Name:  return a.Name < b.Name;
		case Col_Keys:  return a.Keys < b.Keys;
		}else
		switch (sort_spec->ColumnUserID)
		{
		case Col_ID:    return a.ID > b.ID;
		case Col_Name:  return a.Name > b.Name;
		case Col_Keys:  return a.Keys > b.Keys;
		}
		return a.ID < b.ID;
    }
};
const ImGuiTableSortSpecs* KeysItem::sortSpecs = NULL;


//  Keys
//------------------------------------------------------------------
void AppSFMLDraw::WndDraw_AppKeys()
{
	//  Create item list
	static vector<KeysItem> items;
	if (items.size() == 0)
	{
		items.resize(act->names.size(), KeysItem());
		int n = 0;
		for (auto& a : act->names)
		{
			KeysItem& item = items[n];
			auto aid = a.first;  // enum Act_
			item.ID = aid;
			item.Name = a.second;
			
			string ss;
			for (auto& b : act->bindings)
			{
				if (b.second == aid)
				{
					if (!ss.empty())
						ss += "\n";
					ss += ActionsMap::StrKey(b.first);
				}
			}
			item.Keys = ss;
			++n;
		}
	}
	
	const auto tbfl =
		ImGuiTableFlags_Resizable | ImGuiTableFlags_Reorderable | ImGuiTableFlags_Hideable | ImGuiTableFlags_Sortable
		| ImGuiTableFlags_RowBg | ImGuiTableFlags_BordersOuter | ImGuiTableFlags_BordersV | ImGuiTableFlags_NoBordersInBody
		| ImGuiTableFlags_ScrollY;
	
	if (BeginTable("KeyBinds", 4, tbfl))
	{
		const auto fl = ImGuiTableColumnFlags_WidthFixed;
		TableSetupColumn("N", ImGuiTableColumnFlags_DefaultSort | fl, 0.f, Col_ID);
		TableSetupColumn("Name",      fl, 0.f, Col_Name);
		TableSetupColumn("Shortcuts", fl, 0.f, Col_Keys);
		TableSetupScrollFreeze(0, 1); // Make row always visible
		TableHeadersRow();

		//  sort if changed
		if (ImGuiTableSortSpecs* sorts_specs = TableGetSortSpecs())
			if (sorts_specs->SpecsDirty)
			{
				KeysItem::sortSpecs = sorts_specs;
				if (items.size() > 1)
					stable_sort(items.begin(), items.end(), KeysItem::Compare);
				KeysItem::sortSpecs = NULL;
				sorts_specs->SpecsDirty = false;
			}

		/*ImGuiListClipper clipper;
		clipper.Begin(items.size());
		while (clipper.Step())
			for (int row = clipper.DisplayStart; row < clipper.DisplayEnd; ++row)*/
		for (int row = 0; row < items.size(); ++row)
			{
				//  display item
				KeysItem* item = &items[row];
				std::string grp = act->groups[(EAction)item->ID];
				if (!grp.empty())
				{
					TableNextRow();
					TableNextColumn();
					ImU32 bg = GetColorU32(ImVec4(0.10f, 0.16f, 0.22f, 0.65f));
                    TableSetBgColor(ImGuiTableBgTarget_RowBg0, bg);
					TableNextColumn();  TextColored(ImVec4(0.6f, 0.8f, 1.0f, 1.f), grp.c_str());
					TableNextColumn();  Separator();
				}
				PushID(item->ID);
				TableNextRow();
				TableNextColumn();  TextG(i2s(item->ID, 3,'0'));
				TableNextColumn();  TextUnformatted(item->Name.c_str());
				TableNextColumn();  TextG(item->Keys);
				//TableNextColumn();  SmallButton("Add Remove");
				PopID();
			}
		EndTable();
	}	
}
