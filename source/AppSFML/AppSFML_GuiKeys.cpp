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

struct MyItem
{
    int ID;
    string Name;
    string Keys;

    static const ImGuiTableSortSpecs* sortSpecs;

    // Compare function to be used by qsort()
    static bool Compare(const MyItem& a, const MyItem& b)
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
const ImGuiTableSortSpecs* MyItem::sortSpecs = NULL;


//  Keys
//------------------------------------------------------------------
void AppSFMLDraw::WndDraw_AppKeys()
{
	// Create item list
	static vector<MyItem> items;
	if (items.size() == 0)
	{
		items.resize(act->names.size(), MyItem());
		int n = 0;
		for (auto& a : act->names)
		{
			MyItem& item = items[n];
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
	
	static ImGuiTableFlags flags =
		ImGuiTableFlags_Resizable | ImGuiTableFlags_Reorderable | ImGuiTableFlags_Hideable | ImGuiTableFlags_Sortable
		| ImGuiTableFlags_RowBg | ImGuiTableFlags_BordersOuter | ImGuiTableFlags_BordersV | ImGuiTableFlags_NoBordersInBody
		| ImGuiTableFlags_ScrollY;
	
	const auto fl = ImGuiTableColumnFlags_WidthFixed;
	if (BeginTable("table_sorting", 4, flags))
	{
		TableSetupColumn("N", ImGuiTableColumnFlags_DefaultSort | fl, 0.f, Col_ID);
		TableSetupColumn("Name",      fl, 0.f, Col_Name);
		TableSetupColumn("Shortcuts", fl, 0.f, Col_Keys);
		TableSetupScrollFreeze(0, 1); // Make row always visible
		TableHeadersRow();

		//  sort if changed
		if (ImGuiTableSortSpecs* sorts_specs = TableGetSortSpecs())
			if (sorts_specs->SpecsDirty)
			{
				MyItem::sortSpecs = sorts_specs;
				if (items.size() > 1)
					stable_sort(items.begin(), items.end(), MyItem::Compare);
				MyItem::sortSpecs = NULL;
				sorts_specs->SpecsDirty = false;
			}

		ImGuiListClipper clipper;
		clipper.Begin(items.size());
		while (clipper.Step())
			for (int row = clipper.DisplayStart; row < clipper.DisplayEnd; ++row)
			{
				//  display item
				MyItem* item = &items[row];
				PushID(item->ID);
				TableNextRow();
				TableNextColumn();  TextG(i2s(item->ID, 2,'0'));				
				TableNextColumn();  TextUnformatted(item->Name.c_str());
				TableNextColumn();  TextG(item->Keys);
				//TableNextColumn();  SmallButton("Add Remove");
				PopID();
			}
		EndTable();
	}	
}
