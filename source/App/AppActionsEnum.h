#pragma once

//  all App key actions / commands
enum EAction
{
	Act_None,
	///  play
	Act_PlaySetCur,	Act_Play, Act_Pause, Act_Stop, Act_Prev, Act_Next,
	//  seek
	Act_Backward, Act_BackwardSlow, Act_BackwardFast,
	Act_Forward, Act_ForwardSlow, Act_ForwardFast,
	//  vol
	Act_VolumeUp, Act_VolumeUpSlow, Act_VolumeUpFast,
	Act_VolumeDown, Act_VolumeDownSlow, Act_VolumeDownFast,
	//  go	                    
	Act_GotoPlay, Act_GotoPlaySetPls,

	///  move cur
	Act_PlsUp, Act_PlsUpBy8, Act_PlsDown, Act_PlsDownBy8,
	Act_PlsPageOfsUp  ,Act_PlsPageOfsUp4  ,Act_PlsPageUp4  ,Act_PlsPageUp  ,
	Act_PlsPageOfsDown,Act_PlsPageOfsDown4,Act_PlsPageDown4,Act_PlsPageDown,
	Act_PlsListHome,Act_PlsDirUp  ,Act_PlsHome,Act_PlsHome4,
	Act_PlsListEnd ,Act_PlsDirDown,Act_PlsEnd ,Act_PlsEnd4 ,

	//  rate
	Act_RateInc, Act_RateIncCur, Act_RateDec, Act_RateDecCur,
	Act_FilterLowUp, Act_FilterHighUp, Act_FilterLowDown, Act_FilterHighDown,
	//  bookm
	Act_BookmInc, Act_BookmDec, Act_BookmIncTab, Act_BookmDecTab,

	///  tab cur
	Act_TabNext, Act_TabPrev, Act_TabNextRow, Act_TabPrevRow, 
	Act_TabOfsNext, Act_TabOfsPrev, Act_TabOfsNextRow, Act_TabOfsPrevRow, 
	//  tab new
	Act_TabNew, Act_TabNewBegin, Act_TabNewEnd, Act_TabNewBefore, Act_TabClose,
	
	///  repeat
	Act_RepeatTrack, Act_RepeatPls,
	//  debug
	Act_Fps, Act_Debug, Act_Times,
	//  view
	Act_AllStats, Act_FullStats,
	Act_NextVis,
	Act_DirViewNext, Act_FileInfo,

	///  find, hide
	Act_FindHide, Act_FindOpen,
	Act_DuplicateCur,
	Act_HideCur, Act_ShowCur,
		
	///  load, save
	Act_SaveAll, Act_ReloadPls,
	
	//  insert, delete
	Act_InsertDir, Act_InsertFiles,
	Act_ClearPls, Act_DeleteCurFile, Act_DeleteCur,

	// note: add new here for bindings in xml to work ..
	
	///  gui wnd   same as in EWndOpt  . . . . . . 
	Act_WO_Main=500,
	Act_WO_AppAudio, Act_WO_AppKeys, Act_WO_AppAbout, Act_WO_AppHelp,
	Act_WO_View, Act_WO_ViewFonts, Act_WO_Vis, Act_WO_VisThemes, Act_WO_ViewTimes, 
	Act_WO_Tab, Act_WO_TabsAll,
	Act_WO_PlsFind, Act_WO_PlsFilter, Act_WO_AppStats, Act_WO_AppTest, 

	///  views
	Act_SaveCurView=699,
	Act_LoadView0=700, Act_LoadView1, Act_LoadView2, Act_LoadView3, Act_LoadView4,
	Act_LoadView5, Act_LoadView6, Act_LoadView7, Act_LoadView8, Act_LoadView9,
	Act_LoadView10, Act_LoadView11, Act_LoadView12, Act_LoadView13, Act_LoadView14,
	Act_LoadView15, Act_LoadView16, Act_LoadView17, Act_LoadView18,	Act_LoadView19,

	Act_All
};
