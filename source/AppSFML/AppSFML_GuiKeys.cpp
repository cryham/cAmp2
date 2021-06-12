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
	const auto tbfl =
		ImGuiTableFlags_Resizable | ImGuiTableFlags_Reorderable | ImGuiTableFlags_Hideable | ImGuiTableFlags_Sortable
		| ImGuiTableFlags_RowBg | ImGuiTableFlags_BordersOuter | ImGuiTableFlags_BordersV | ImGuiTableFlags_NoBordersInBody
		| ImGuiTableFlags_ScrollY;
	
	Sep(5);
	SameLine(200);  if (Button("Load"))  act->Load();
	SameLine(300);  if (Button("Save"))  act->Save();
	SameLine(450);  if (Button("Reset to Defaults"))  act->DefaultBindings();
	Sep(5);

	act->UpdateGuiKeysList();

	if (BeginTable("KeyBinds", 4, tbfl))
	{
		const auto fl = ImGuiTableColumnFlags_WidthFixed;
		TableSetupColumn("N", ImGuiTableColumnFlags_DefaultSort | fl, 0.f, Col_ID);
		TableSetupColumn("Name",      fl, 0.f, Col_Name);
		TableSetupColumn("Add",       fl, 0.f, Col_Name);
		TableSetupColumn("Shortcuts", fl, 0.f, Col_Keys);
		TableSetupScrollFreeze(0, 1); // Make row always visible
		TableHeadersRow();

		//  sort if changed
		ImGuiTableSortSpecs* sorts_specs = TableGetSortSpecs();
		if (sorts_specs != NULL && sorts_specs->SpecsDirty)
		{
			KeysRow::sortSpecs = sorts_specs;
			act->SortGui();
			KeysRow::sortSpecs = NULL;
			sorts_specs->SpecsDirty = false;
		}

		if (act->RowsCount() > 0)
		for (int rid = 0; rid < act->RowsCount(); ++rid)
		{
			//  display item
			const auto& row = act->GetRow(rid);
			
			//  group row  ----  separator
			const auto& group = act->GetGroup((EAction)row.Act);
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
			PushID(row.Act);
			TableNextRow();
			TableNextColumn();  TextG(i2s(row.Act, 3,'0'));
			TableNextColumn();  TextUnformatted(row.name.c_str());
			TableNextColumn();
			
			//  add .. button
			if (act->IsGuiBind(row.Act))
			{
				if (SmallButton("Press"))
					act->GuiBindCancel();
			}
			else if (SmallButton(".."))
			{
				//  wait for key press to bind..
				act->GuiBindStart((EAction)row.Act);
			}
			TableNextColumn();
			
			//  binds
			int ik = 0;
			for (auto k : row.keys)
			{
				if (SmallButton(k.c_str()))
				{	//  unbind on press
					act->Unbind(row.modsKeys[ik]);
					act->UpdateGui();
				}	++ik;
			}
			PopID();
		}
		EndTable();
	}	
}
