#include "AppActions.h"
#include "App.h"
#include "../System/Utilities.h"

	
///  Groups
//...........................................................................................................
void ActionsMap::FillGroupNames()
{
	groups.clear();
	groups[Act_PlaySetCur  ] = "Playing";
	groups[Act_Backward    ] = "Seeking";
	groups[Act_VolumeUp    ] = "Volume";
	groups[Act_GotoPlay    ] = "Go to";
	groups[Act_PlsUp       ] = "Move Playlist cursor";
	groups[Act_RateInc     ] = "Rating";
	groups[Act_FilterLowUp ] = "Filtering";
	groups[Act_BookmInc    ] = "Bookmarks";
	groups[Act_TabNext     ] = "Tab current";
	groups[Act_TabNew      ] = "New Tab";
	groups[Act_RepeatTrack ] = "Repeat";
	groups[Act_Fps         ] = "Test";
	groups[Act_AllStats    ] = "View toggle";
	groups[Act_FindHide    ] = "Find";
	groups[Act_DuplicateCur] = "Advanced";
	groups[Act_HideCur     ] = "Hide/Show";
	groups[Act_SaveAll     ] = "Save/Load";
	groups[Act_InsertDir   ] = "Insert";
	groups[Act_ClearPls    ] = "Delete";
	groups[Act_WO_Main     ] = "GUI Windows";
	groups[Act_SaveCurView ] = "Views";
}


///  Names
//...........................................................................................................
void ActionsMap::FillNames()
{
	///  play
	names[Act_PlaySetCur      ] = "Play and set playlist";
	names[Act_Play			  ] = "Play";
	names[Act_Pause			  ] = "Pause";
	names[Act_Stop			  ] = "Stop";
	names[Act_Prev			  ] = "Previous";
	names[Act_Next			  ] = "Next";

	//  seek
	names[Act_Backward		  ] = "Backward";
	names[Act_BackwardSlow	  ] = "Backward Slow";
	names[Act_BackwardFast	  ] = "Backward Fast";

	names[Act_Forward		  ] = "Forward";
	names[Act_ForwardSlow	  ] = "Forward Slow";
	names[Act_ForwardFast	  ] = "Forward Fast";

	//  vol
	names[Act_VolumeUp		  ] = "Volume Up";
	names[Act_VolumeUpSlow	  ] = "Volume Up Slow";
	names[Act_VolumeUpFast	  ] = "Volume Up Fast";

	names[Act_VolumeDown	  ] = "Volume Down";
	names[Act_VolumeDownSlow  ] = "Volume Down Slow";
	names[Act_VolumeDownFast  ] = "Volume Down Fast";

	//  go
	names[Act_GotoPlay		  ] = "Go to Playing in playlist";
	names[Act_GotoPlaySetPls  ] = "Go to Playing and playlist";


	///  move cur
	names[Act_PlsUp			  ] = "Move Up";
	names[Act_PlsUpBy8		  ] = "Move Up by 8";
	names[Act_PlsDown		  ] = "Move Down";
	names[Act_PlsDownBy8	  ] = "Move Down by 8";

	names[Act_PlsPageUp  	  ] = "Move Page Up";
	names[Act_PlsPageUp4  	  ] = "Move Page Up by 4";
	names[Act_PlsPageDown	  ] = "Move Page Down";
	names[Act_PlsPageDown4	  ] = "Move Page Down by 4";

	names[Act_PlsPageOfsUp    ] = "Move Page Offset Up";
	names[Act_PlsPageOfsUp4   ] = "Move Page Offset Up by 4";
	names[Act_PlsPageOfsDown  ] = "Move Page Offset Down";
	names[Act_PlsPageOfsDown4 ] = "Move Page Offset Down by 4";

	names[Act_PlsListHome	  ] = "Move to Begin";
	names[Act_PlsDirUp  	  ] = "Move Dir Up";
	names[Act_PlsHome		  ] = "Move Home";
	names[Act_PlsHome4		  ] = "Move Home by 4";

	names[Act_PlsListEnd 	  ] = "Move to End";
	names[Act_PlsDirDown	  ] = "Move Dir Down";
	names[Act_PlsEnd 		  ] = "Move End";
	names[Act_PlsEnd4 		  ] = "Move End by 4";

	
	//  rate
	names[Act_RateInc		  ] = "Rate Increase playing";
	names[Act_RateIncCur	  ] = "Rate Increase at cursor";
	names[Act_RateDec		  ] = "Rate Decrease playing";
	names[Act_RateDecCur	  ] = "Rate Decrease at cursor";

	names[Act_FilterLowUp	  ] = "Filter Lower Up";
	names[Act_FilterHighUp	  ] = "Filter Higher Up";
	names[Act_FilterLowDown	  ] = "Filter Lower Down";
	names[Act_FilterHighDown  ] = "Filter Higher Down";
	
	//  bookm
	names[Act_BookmInc		  ] = "Bookmark Increase";
	names[Act_BookmDec		  ] = "Bookmark Decrease";

	names[Act_BookmIncTab	  ] = "Tab Bookmark Increase";
	names[Act_BookmDecTab	  ] = "Tab Bookmark Decrease";


	///  tab cur
	names[Act_TabNext		  ] = "Tab Next";
	names[Act_TabPrev		  ] = "Tab Prev";
	names[Act_TabNextRow	  ] = "Tab Next Row";
	names[Act_TabPrevRow	  ] = "Tab Prev Row";

	names[Act_TabOfsNext	  ] = "Tab Offset Next";
	names[Act_TabOfsPrev	  ] = "Tab Offset Prev";
	names[Act_TabOfsNextRow	  ] = "Tab Offset Next Row";
	names[Act_TabOfsPrevRow	  ] = "Tab Offset Prev Row";

	//  tab new
	names[Act_TabNew		  ] = "New Tab after current";
	names[Act_TabNewBegin	  ] = "New Tab at begin";
	names[Act_TabNewEnd		  ] = "New Tab at end";
	names[Act_TabNewBefore	  ] = "New Tab before current";

	names[Act_TabClose		  ] = "Tab Close";

	
	///  repeat
	names[Act_RepeatTrack	  ] = "Toggle Repeat Track";
	names[Act_RepeatPls		  ] = "Toggle Repeat Playlist";

	//  debug
	names[Act_Fps			  ] = "Toggle Fps";
	names[Act_Debug			  ] = "Toggle Debug";
	names[Act_Times			  ] = "Next Times test mode";

	//  view
	names[Act_AllStats		  ] = "Toggle All Stats";
	names[Act_FullStats		  ] = "Toggle Full Stats";
	names[Act_NextVis		  ] = "Next Visual";
	names[Act_DirViewNext	  ] = "Next Dir View mode";
	names[Act_FileInfo		  ] = "Toggle File Info";

	
	///  find, hide
	names[Act_FindHide		  ] = "Find Hide results";
	names[Act_FindOpen		  ] = "Find Open";

	names[Act_DuplicateCur	  ] = "Duplicate file at cursor";

	names[Act_HideCur		  ] = "Toggle Hide at cursor";
	names[Act_ShowCur		  ] = "Toggle Show at cursor";

	
	///  load, save
	names[Act_SaveAll		  ] = "Save All";
	names[Act_ReloadPls		  ] = "Reload Playlist";

	//  insert, delete
	names[Act_InsertDir		  ] = "Insert Dir";
	names[Act_InsertFiles	  ] = "Insert Files";

	names[Act_ClearPls		  ] = "Clear Whole Playlist";
	names[Act_DeleteCurFile	  ] = "Delete File at cursor";
	names[Act_DeleteCur		  ] = "Delete at cursor";

	
	///  gui wnd
	names[Act_WO_Main         ] = "GUI Main";
	names[Act_WO_AppAudio	  ] = "GUI AppAudio";
	names[Act_WO_AppKeys	  ] = "GUI AppKeys";
	names[Act_WO_AppAbout	  ] = "GUI AppAbout";
	names[Act_WO_AppHelp	  ] = "GUI AppHelp";

	names[Act_WO_View		  ] = "GUI View";
	names[Act_WO_ViewFonts	  ] = "GUI ViewFonts";
	names[Act_WO_Vis		  ] = "GUI Vis";
	names[Act_WO_VisThemes	  ] = "GUI VisThemes";
	names[Act_WO_ViewTimes	  ] = "GUI ViewTimes";

	names[Act_WO_Tab		  ] = "GUI Tab";
	names[Act_WO_TabsAll	  ] = "GUI TabsAll";
	names[Act_WO_PlsFind	  ] = "GUI PlsFind";
	names[Act_WO_PlsFilter	  ] = "GUI PlsFilter";
	names[Act_WO_AppStats	  ] = "GUI AppStats";
	names[Act_WO_AppTest	  ] = "GUI AppTest";

	///  views
	names[Act_SaveCurView     ] = "Save Current View";
	for (int i = Act_LoadView0; i <= Act_LoadView19; ++i)
		names[(EAction)i      ] = "Load View " + i2s(i - Act_LoadView0);
}


///  Methods
//...........................................................................................................
void ActionsMap::FillMethods()
{
	///  play
	methods[Act_PlaySetCur	] = &App::PlaySetCur;
	methods[Act_Play		] = &App::Play;
	methods[Act_Pause		] = &App::Pause;
	methods[Act_Stop		] = &App::Stop;
	methods[Act_Prev		] = &App::Prev;
	methods[Act_Next		] = &App::Next;

	//  seek
	methods[Act_Backward		] = &App::Backward;
	methods[Act_BackwardSlow	] = &App::BackwardSlow;
	methods[Act_BackwardFast	] = &App::BackwardFast;

	methods[Act_Forward		    ] = &App::Forward;
	methods[Act_ForwardSlow	    ] = &App::ForwardSlow;
	methods[Act_ForwardFast	    ] = &App::ForwardFast;

	//  vol
	methods[Act_VolumeUp		] = &App::VolumeUp;
	methods[Act_VolumeUpSlow	] = &App::VolumeUpSlow;
	methods[Act_VolumeUpFast	] = &App::VolumeUpFast;

	methods[Act_VolumeDown	    ] = &App::VolumeDown;
	methods[Act_VolumeDownSlow  ] = &App::VolumeDownSlow;
	methods[Act_VolumeDownFast  ] = &App::VolumeDownFast;

	//  go
	methods[Act_GotoPlay		] = &App::GotoPlay;
	methods[Act_GotoPlaySetPls  ] = &App::GotoPlaySetPls;


	///  move cur
	methods[Act_PlsUp		  ] = &App::PlsUp;
	methods[Act_PlsUpBy8	  ] = &App::PlsUpBy8;

	methods[Act_PlsDown		  ] = &App::PlsDown;
	methods[Act_PlsDownBy8	  ] = &App::PlsDownBy8;

	methods[Act_PlsPageOfsUp  ] = &App::PlsPageOfsUp;
	methods[Act_PlsPageOfsUp4 ] = &App::PlsPageOfsUp4;
	methods[Act_PlsPageUp4    ] = &App::PlsPageUp4;
	methods[Act_PlsPageUp  	  ] = &App::PlsPageUp;

	methods[Act_PlsPageOfsDown  ] = &App::PlsPageOfsDown;
	methods[Act_PlsPageOfsDown4 ] = &App::PlsPageOfsDown4;
	methods[Act_PlsPageDown4  ] = &App::PlsPageDown4;
	methods[Act_PlsPageDown	  ] = &App::PlsPageDown;

	methods[Act_PlsListHome	  ] = &App::PlsListHome;
	methods[Act_PlsDirUp  	  ] = &App::PlsDirUp;
	methods[Act_PlsHome		  ] = &App::PlsHome;
	methods[Act_PlsHome4	  ] = &App::PlsHome4;

	methods[Act_PlsListEnd 	  ] = &App::PlsListEnd;
	methods[Act_PlsDirDown	  ] = &App::PlsDirDown;
	methods[Act_PlsEnd 		  ] = &App::PlsEnd;
	methods[Act_PlsEnd4 	  ] = &App::PlsEnd4;

	
	//  rate
	methods[Act_RateInc		  ] = &App::RateInc;
	methods[Act_RateIncCur	  ] = &App::RateIncCur;
	methods[Act_RateDec		  ] = &App::RateDec;
	methods[Act_RateDecCur	  ] = &App::RateDecCur;

	methods[Act_FilterLowUp	  ] = &App::FilterLowUp;
	methods[Act_FilterHighUp  ] = &App::FilterHighUp;
	methods[Act_FilterLowDown ] = &App::FilterLowDown;
	methods[Act_FilterHighDown] = &App::FilterHighDown;
	
	//  bookm
	methods[Act_BookmInc	  ] = &App::BookmInc;
	methods[Act_BookmDec	  ] = &App::BookmDec;
	methods[Act_BookmIncTab	  ] = &App::BookmIncTab;
	methods[Act_BookmDecTab	  ] = &App::BookmDecTab;


	///  tab cur
	methods[Act_TabNext		  ] = &App::TabNext;
	methods[Act_TabPrev		  ] = &App::TabPrev;
	methods[Act_TabNextRow	  ] = &App::TabNextRow;
	methods[Act_TabPrevRow	  ] = &App::TabPrevRow;

	methods[Act_TabOfsNext	  ] = &App::TabOfsNext;
	methods[Act_TabOfsPrev	  ] = &App::TabOfsPrev;
	methods[Act_TabOfsNextRow ] = &App::TabOfsNextRow;
	methods[Act_TabOfsPrevRow ] = &App::TabOfsPrevRow;

	//  tab new
	methods[Act_TabNew		  ] = &App::TabNew;
	methods[Act_TabNewBegin	  ] = &App::TabNewBegin;
	methods[Act_TabNewEnd	  ] = &App::TabNewEnd;
	methods[Act_TabNewBefore  ] = &App::TabNewBefore;

	methods[Act_TabClose	  ] = &App::TabClose;


	///  repeat
	methods[Act_RepeatTrack	  ] = &App::RepeatTrack;
	methods[Act_RepeatPls	  ] = &App::RepeatPls;

	//  debug
	methods[Act_Fps			  ] = &App::Fps;
	methods[Act_Debug		  ] = &App::Debug;
	methods[Act_Times		  ] = &App::Times;

	//  view
	methods[Act_AllStats	  ] = &App::AllStats;
	methods[Act_FullStats	  ] = &App::FullStats;
	methods[Act_NextVis		  ] = &App::NextVis;
	methods[Act_DirViewNext	  ] = &App::DirViewNext;
	methods[Act_FileInfo	  ] = &App::FileInfo;

	
	///  find, hide
	methods[Act_FindHide	  ] = &App::FindHide;
	methods[Act_FindOpen	  ] = &App::FindOpen;

	methods[Act_DuplicateCur  ] = &App::DuplicateCur;

	methods[Act_HideCur		  ] = &App::HideCur;
	methods[Act_ShowCur		  ] = &App::ShowCur;

	
	///  load, save
	methods[Act_SaveAll			] = &App::SaveAll;
	methods[Act_ReloadPls		] = &App::ReloadPls;

	//  insert, delete
	methods[Act_InsertDir		] = &App::InsertDir;
	methods[Act_InsertFiles		] = &App::InsertFiles;

	methods[Act_ClearPls		] = &App::ClearPls;
	methods[Act_DeleteCurFile	] = &App::DeleteCurFile;
	methods[Act_DeleteCur		] = &App::DeleteCur;

	
	///  gui wnd
	methods[Act_WO_Main       ] = &App::GUIMain;
	methods[Act_WO_AppAudio	  ] = &App::GUIAppAudio;
	methods[Act_WO_AppKeys	  ] = &App::GUIAppKeys;
	methods[Act_WO_AppAbout	  ] = &App::GUIAppAbout;
	methods[Act_WO_AppHelp	  ] = &App::GUIAppHelp;

	methods[Act_WO_View		  ] = &App::GUIView;
	methods[Act_WO_ViewFonts  ] = &App::GUIViewFonts;
	methods[Act_WO_Vis		  ] = &App::GUIVis;
	methods[Act_WO_VisThemes  ] = &App::GUIVisThemes;
	methods[Act_WO_ViewTimes  ] = &App::GUIViewTimes;
	
	methods[Act_WO_Tab		  ] = &App::GUITab;
	methods[Act_WO_TabsAll	  ] = &App::GUITabsAll;
	methods[Act_WO_PlsFind	  ] = &App::GUIPlsFind;
	methods[Act_WO_PlsFilter  ] = &App::GUIPlsFilter;
	methods[Act_WO_AppStats	  ] = &App::GUIAppStats;
	methods[Act_WO_AppTest	  ] = &App::GUIAppTest;

	///  views
	methods[Act_SaveCurView	  ] = &App::SaveCurView;
	methods[Act_LoadView0	  ] = &App::LoadCurView0;    methods[Act_LoadView10	  ] = &App::LoadCurView10;
	methods[Act_LoadView1	  ] = &App::LoadCurView1;    methods[Act_LoadView11	  ] = &App::LoadCurView11;
	methods[Act_LoadView2	  ] = &App::LoadCurView2;    methods[Act_LoadView12	  ] = &App::LoadCurView12;
	methods[Act_LoadView3	  ] = &App::LoadCurView3;    methods[Act_LoadView13	  ] = &App::LoadCurView13;
	methods[Act_LoadView4	  ] = &App::LoadCurView4;    methods[Act_LoadView14	  ] = &App::LoadCurView14;
	methods[Act_LoadView5	  ] = &App::LoadCurView5;    methods[Act_LoadView15	  ] = &App::LoadCurView15;
	methods[Act_LoadView6	  ] = &App::LoadCurView6;    methods[Act_LoadView16	  ] = &App::LoadCurView16;
	methods[Act_LoadView7	  ] = &App::LoadCurView7;    methods[Act_LoadView17	  ] = &App::LoadCurView17;
	methods[Act_LoadView8	  ] = &App::LoadCurView8;    methods[Act_LoadView18	  ] = &App::LoadCurView18;
	methods[Act_LoadView9	  ] = &App::LoadCurView9;    methods[Act_LoadView19	  ] = &App::LoadCurView19;
}


//  Key names
//...........................................................................................................
const char* ActionsMap::csKeyNames[sf::Keyboard::KeyCount] =
{
	"A","B","C","D","E","F","G","H","I","J","K","L","M","N","O","P","Q","R","S","T","U","V","W","X","Y","Z",
	"0","1","2","3","4","5","6","7","8","9",
	"Escape",
	"LControl","LShift","LAlt","LSystem",
	"RControl","RShift","RAlt","RSystem",
	"Menu",
	"LBracket","RBracket",
	"Semicolon","Comma","Period","Quote",
	"Slash","Backslash",
	"Tilde","Equal","Hyphen",
	"Space","Enter","Backspace","Tab",
	"PageUp","PageDown","End","Home",
	"Insert","Delete",
	"Add","Subtract","Multiply","Divide",
	"Left","Right","Up","Down",
	"Numpad0","Numpad1","Numpad2","Numpad3","Numpad4","Numpad5","Numpad6","Numpad7","Numpad8","Numpad9",
	"F1","F2","F3","F4","F5","F6","F7","F8","F9","F10","F11","F12","F13","F14","F15",
	"Pause",
};
