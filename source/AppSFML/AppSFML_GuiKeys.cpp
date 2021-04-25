#include "AppSFML_Draw.h"
#include "../Audio/Audio.h"
#include "../System/Utilities.h"
#include "../Settings/Settings.h"
#include "../Settings/VisualColors.h"
#include "../App/AppActions.h"
#include "../../libs/imgui.h"
#include "../../libs/imgui-SFML.h"
using namespace sf;  using namespace std;  using namespace ImGui;


//  Keys
//------------------------------------------------------------------
void AppSFMLDraw::WndDraw_AppKeys()
{
	act->UpdateGuiKeysList();
	
	const auto tbfl =
		ImGuiTableFlags_Resizable | ImGuiTableFlags_Reorderable | ImGuiTableFlags_Hideable | ImGuiTableFlags_Sortable
		| ImGuiTableFlags_RowBg | ImGuiTableFlags_BordersOuter | ImGuiTableFlags_BordersV | ImGuiTableFlags_NoBordersInBody
		| ImGuiTableFlags_ScrollY;
	
	if (BeginTable("KeyBinds", 4, tbfl))
	{
		const auto fl = ImGuiTableColumnFlags_WidthFixed;
		TableSetupColumn("N", ImGuiTableColumnFlags_DefaultSort | fl, 0.f, Col_ID);
		TableSetupColumn("Name",      fl, 0.f, Col_Name);
		TableSetupColumn("Add",      fl, 0.f, Col_Name);
		TableSetupColumn("Shortcuts", fl, 0.f, Col_Keys);
		TableSetupScrollFreeze(0, 1); // Make row always visible
		TableHeadersRow();

		//  sort if changed
		ImGuiTableSortSpecs* sorts_specs = TableGetSortSpecs();
		if (sorts_specs != NULL && sorts_specs->SpecsDirty)
		{
			KeysRow::sortSpecs = sorts_specs;
			if (act->rows.size() > 1)
				stable_sort(act->rows.begin(), act->rows.end(), KeysRow::Compare);
			KeysRow::sortSpecs = NULL;
			sorts_specs->SpecsDirty = false;
		}

		for (int rid = 0; rid < act->rows.size(); ++rid)
		{
			//  display item
			KeysRow* row = &act->rows[rid];
			//  group row  ----  separator
			std::string group = act->groups[(EAction)row->Act];
			if (!group.empty())
			{
				TableNextRow();
				TableNextColumn();
				ImU32 bg = GetColorU32(ImVec4(0.10f, 0.16f, 0.22f, 0.65f));
				TableSetBgColor(ImGuiTableBgTarget_RowBg0, bg);
				TableNextColumn();  TextColored(ImVec4(0.6f, 0.8f, 1.0f, 1.f), group.c_str());
				TableNextColumn();  Separator();
			}
			//  keys row  ----
			PushID(row->Act);
			TableNextRow();
			TableNextColumn();  TextG(i2s(row->Act, 3,'0'));
			TableNextColumn();  TextUnformatted(row->Name.c_str());
			TableNextColumn();
			//  add
			if (act->binding && act->bindAct == row->Act)
			{	if (Button("Press"))
				{
					act->binding = false;
			}	}
			else if (SmallButton(".."))
			{
				//  wait for key press to bind..
				act->binding = true;
				act->bindAct = (EAction)row->Act;
			}
			TableNextColumn();
			//  binds
			int ik = 0;
			for (auto k : row->Keys)
			{
				if (SmallButton(k.c_str()))
				{	//  unbind on press
					act->Unbind(row->ModsKeys[ik]);
					act->bGuiUpdate = true;
				}	++ik;
			}
			PopID();
		}
		EndTable();
	}	
}
