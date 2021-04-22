#include "../../libs/imgui.h"
#include "AppSFML_Draw.h"
using namespace ImGui;


//  Gui look and colors
//------------------------------------------------------------------
void AppSFMLDraw::SetupGuiStyle()
{
	ImGuiStyle& st = ImGui::GetStyle();  st.Alpha = 1.f;
	st.WindowPadding = ImVec2(20,6);  st.WindowRounding = 1.f;  st.WindowBorderSize = 1.f;
	st.WindowMinSize = ImVec2(100,20);  st.ChildRounding = 1.f;
	st.FramePadding = ImVec2(12,3);  st.FrameRounding = 0;
	st.ItemSpacing  = ImVec2(11,5);  st.ItemInnerSpacing = ImVec2(11,4);  st.IndentSpacing = 14.f;
	st.ColumnsMinSpacing = 21.f;
	st.ScrollbarSize = 18.f;  st.ScrollbarRounding = 0.f;
	st.GrabMinSize = 20.f;  st.GrabRounding = 0.f;
	st.TabRounding = 0.f;  st.TabBorderSize = 1.f;
	st.AntiAliasedLines = 1;  //st.AntiAliasedShapes = 1;  //style.CurveTessellationTol;

	ImVec4* colors = ImGui::GetStyle().Colors;
	colors[ImGuiCol_Text]                   = ImVec4(0.79f, 0.83f, 1.00f, 0.98f);
	colors[ImGuiCol_TextDisabled]           = ImVec4(0.59f, 0.62f, 0.75f, 0.91f);
	colors[ImGuiCol_WindowBg]               = ImVec4(0.03f, 0.03f, 0.08f, 0.97f);
	colors[ImGuiCol_ChildBg]                = ImVec4(0.00f, 0.00f, 0.00f, 0.65f);
	colors[ImGuiCol_PopupBg]                = ImVec4(0.05f, 0.05f, 0.10f, 0.82f);
	colors[ImGuiCol_Border]                 = ImVec4(0.23f, 0.23f, 0.35f, 0.51f);
	colors[ImGuiCol_BorderShadow]           = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
	colors[ImGuiCol_FrameBg]                = ImVec4(0.80f, 0.83f, 1.00f, 0.06f);//
	colors[ImGuiCol_FrameBgHovered]         = ImVec4(0.80f, 0.83f, 1.00f, 0.09f);
	colors[ImGuiCol_FrameBgActive]          = ImVec4(0.80f, 0.83f, 1.00f, 0.09f);
	colors[ImGuiCol_TitleBg]                = ImVec4(0.27f, 0.27f, 0.54f, 0.61f);
	colors[ImGuiCol_TitleBgActive]          = ImVec4(0.32f, 0.32f, 0.63f, 0.89f);
	colors[ImGuiCol_TitleBgCollapsed]       = ImVec4(0.40f, 0.40f, 0.80f, 0.45f);
	colors[ImGuiCol_MenuBarBg]              = ImVec4(0.45f, 0.45f, 0.55f, 0.19f);
	colors[ImGuiCol_ScrollbarBg]            = ImVec4(0.14f, 0.13f, 0.22f, 0.60f);
	colors[ImGuiCol_ScrollbarGrab]          = ImVec4(0.42f, 0.42f, 0.85f, 0.30f);
	colors[ImGuiCol_ScrollbarGrabHovered]   = ImVec4(0.50f, 0.50f, 1.00f, 0.40f);
	colors[ImGuiCol_ScrollbarGrabActive]    = ImVec4(0.49f, 0.50f, 1.00f, 0.46f);
	colors[ImGuiCol_CheckMark]              = ImVec4(0.30f, 0.60f, 0.90f, 1.00f);
	colors[ImGuiCol_SliderGrab]             = ImVec4(0.36f, 0.40f, 0.54f, 0.79f);
	colors[ImGuiCol_SliderGrabActive]       = ImVec4(0.36f, 0.40f, 0.64f, 0.79f);//
	colors[ImGuiCol_Button]                 = ImVec4(0.47f, 0.43f, 0.85f, 0.30f);
	colors[ImGuiCol_ButtonHovered]          = ImVec4(0.24f, 0.22f, 0.47f, 1.00f);
	colors[ImGuiCol_ButtonActive]           = ImVec4(0.27f, 0.27f, 0.50f, 1.00f);
	colors[ImGuiCol_Header]                 = ImVec4(0.58f, 0.58f, 0.90f, 0.12f);//
	colors[ImGuiCol_HeaderHovered]          = ImVec4(0.45f, 0.45f, 0.90f, 0.17f);
	colors[ImGuiCol_HeaderActive]           = ImVec4(0.53f, 0.53f, 0.87f, 0.24f);
	colors[ImGuiCol_Separator]              = ImVec4(0.22f, 0.22f, 0.40f, 0.80f);
	colors[ImGuiCol_SeparatorHovered]       = ImVec4(0.35f, 0.30f, 0.60f, 0.90f);
	colors[ImGuiCol_SeparatorActive]        = ImVec4(0.45f, 0.40f, 0.60f, 1.00f);
	colors[ImGuiCol_ResizeGrip]             = ImVec4(0.75f, 0.89f, 1.00f, 0.30f);
	colors[ImGuiCol_ResizeGripHovered]      = ImVec4(1.00f, 1.00f, 1.00f, 0.60f);
	colors[ImGuiCol_ResizeGripActive]       = ImVec4(1.00f, 1.00f, 1.00f, 0.90f);
	colors[ImGuiCol_Tab]                    = ImVec4(0.12f, 0.20f, 0.40f, 0.86f);//
	colors[ImGuiCol_TabHovered]             = ImVec4(0.16f, 0.36f, 0.60f, 0.86f);
	colors[ImGuiCol_TabActive]              = ImVec4(0.16f, 0.36f, 0.60f, 0.86f);
	colors[ImGuiCol_TabUnfocused]           = ImVec4(0.07f, 0.10f, 0.15f, 0.97f);
	colors[ImGuiCol_TabUnfocusedActive]     = ImVec4(0.13f, 0.26f, 0.42f, 0.97f);
	colors[ImGuiCol_PlotLines]              = ImVec4(0.82f, 0.82f, 0.82f, 1.00f);
	colors[ImGuiCol_PlotLinesHovered]       = ImVec4(0.76f, 0.81f, 0.90f, 1.00f);
	colors[ImGuiCol_PlotHistogram]          = ImVec4(0.61f, 0.69f, 0.90f, 1.00f);
	colors[ImGuiCol_PlotHistogramHovered]   = ImVec4(0.55f, 0.76f, 1.00f, 1.00f);
	colors[ImGuiCol_TableHeaderBg]          = ImVec4(0.15f, 0.14f, 0.25f, 0.88f);//
	colors[ImGuiCol_TableBorderStrong]      = ImVec4(0.31f, 0.31f, 0.45f, 0.60f);
	colors[ImGuiCol_TableBorderLight]       = ImVec4(0.31f, 0.31f, 0.45f, 0.50f);
	colors[ImGuiCol_TableRowBg]             = ImVec4(0.80f, 0.80f, 1.00f, 0.01f);
	colors[ImGuiCol_TableRowBgAlt]          = ImVec4(0.90f, 0.90f, 1.00f, 0.09f);
	colors[ImGuiCol_TextSelectedBg]         = ImVec4(0.13f, 0.20f, 0.76f, 0.47f);
	colors[ImGuiCol_DragDropTarget]         = ImVec4(0.26f, 0.60f, 1.00f, 1.00f);
	//colors[ImGuiCol_NavHighlight]           = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
	//colors[ImGuiCol_NavWindowingHighlight]  = ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
	//colors[ImGuiCol_NavWindowingDimBg]      = ImVec4(0.80f, 0.80f, 0.80f, 0.20f);
	//colors[ImGuiCol_ModalWindowDimBg]       = ImVec4(0.80f, 0.80f, 0.80f, 0.35f);
}
