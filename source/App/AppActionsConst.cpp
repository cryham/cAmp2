#include "AppActions.h"
#include "App.h"

	
///  Names
//...........................................................................................................
void ActionsMap::FillNames()
{
	///  play
	names[Act_PlaySetCur      ] = "PlaySetCur";
	names[Act_Play			  ] = "Play";
	names[Act_Pause			  ] = "Pause";
	names[Act_Stop			  ] = "Stop";
	names[Act_Prev			  ] = "Prev";
	names[Act_Next			  ] = "Next";

	//  seek
	names[Act_Backward		  ] = "Backward";
	names[Act_BackwardSlow	  ] = "BackwardSlow";
	names[Act_BackwardFast	  ] = "BackwardFast";
								                  
	names[Act_Forward		  ] = "Forward";
	names[Act_ForwardSlow	  ] = "ForwardSlow";
	names[Act_ForwardFast	  ] = "ForwardFast";
								                  
	//  vol
	names[Act_VolumeUp		  ] = "VolumeUp";
	names[Act_VolumeUpSlow	  ] = "VolumeUpSlow";
	names[Act_VolumeUpFast	  ] = "VolumeUpFast";
								                  
	names[Act_VolumeDown	  ] = "VolumeDown";
	names[Act_VolumeDownSlow  ] = "VolumeDownSlow";
	names[Act_VolumeDownFast  ] = "VolumeDownFast";
								                  
	//  go
	names[Act_GotoPlay		  ] = "GotoPlay";
	names[Act_GotoPlaySetPls  ] = "GotoPlaySetPls";


	///  move cur
	names[Act_PlsUp			  ] = "PlsUp";
	names[Act_PlsUpBy8		  ] = "PlsUpBy8";
	names[Act_PlsDown		  ] = "PlsDown";
	names[Act_PlsDownBy8	  ] = "PlsDownBy8";
								                  
	names[Act_PlsPageUp  	  ] = "PlsPageUp";
	names[Act_PlsPageUp4  	  ] = "PlsPageUp4";
	names[Act_PlsPageDown	  ] = "PlsPageDown";
	names[Act_PlsPageDown4	  ] = "PlsPageDown4";
								                  
	names[Act_PlsPageOfsUp    ] = "PlsPageOfsUp";
	names[Act_PlsPageOfsUp4   ] = "PlsPageOfsUp4";
	names[Act_PlsPageOfsDown  ] = "PlsPageOfsDown";
	names[Act_PlsPageOfsDown4 ] = "PlsPageOfsDown4";

	names[Act_PlsListHome	  ] = "PlsListHome";
	names[Act_PlsDirUp  	  ] = "PlsDirUp";
	names[Act_PlsHome		  ] = "PlsHome";
	names[Act_PlsHome4		  ] = "PlsHome4";
								                 
	names[Act_PlsListEnd 	  ] = "PlsListEnd";
	names[Act_PlsDirDown	  ] = "PlsDirDown";
	names[Act_PlsEnd 		  ] = "PlsEnd";
	names[Act_PlsEnd4 		  ] = "PlsEnd4";
								                 
								                 
	//  rate
	names[Act_RateInc		  ] = "RateInc";
	names[Act_RateIncCur	  ] = "RateIncCur";
	names[Act_RateDec		  ] = "RateDec";
	names[Act_RateDecCur	  ] = "RateDecCur";
								                 
	names[Act_FilterLowUp	  ] = "FilterLowUp";
	names[Act_FilterHighUp	  ] = "FilterHighUp";
	names[Act_FilterLowDown	  ] = "FilterLowDown";
	names[Act_FilterHighDown  ] = "FilterHighDown";
	
	//  bookm
	names[Act_BookmInc		  ] = "BookmInc";
	names[Act_BookmDec		  ] = "BookmDec";
								                 
	names[Act_BookmIncTab	  ] = "BookmIncTab";
	names[Act_BookmDecTab	  ] = "BookmDecTab";


	///  tab cur
	names[Act_TabNext		  ] = "TabNext";
	names[Act_TabPrev		  ] = "TabPrev";
	names[Act_TabNextRow	  ] = "TabNextRow";
	names[Act_TabPrevRow	  ] = "TabPrevRow";
								                 
	names[Act_TabOfsNext	  ] = "TabOfsNext";
	names[Act_TabOfsPrev	  ] = "TabOfsPrev";
	names[Act_TabOfsNextRow	  ] = "TabOfsNextRow";
	names[Act_TabOfsPrevRow	  ] = "TabOfsPrevRow";
								                 
	//  tab new
	names[Act_TabNew		  ] = "TabNew";
	names[Act_TabNewBegin	  ] = "TabNewBegin";
	names[Act_TabNewEnd		  ] = "TabNewEnd";
	names[Act_TabNewBefore	  ] = "TabNewBefore";
								                 
	names[Act_TabClose		  ] = "TabClose";
								                 

	///  repeat
	names[Act_RepeatTrack	  ] = "RepeatTrack";
	names[Act_RepeatPls		  ] = "RepeatPls";
								                 
	//  debug
	names[Act_Fps			  ] = "Fps";
	names[Act_Debug			  ] = "Debug";
	names[Act_Times			  ] = "Times";
								                 
	//  view
	names[Act_AllStats		  ] = "AllStats";
	names[Act_FullStats		  ] = "FullStats";
	names[Act_NextVis		  ] = "NextVis";
	names[Act_DirViewNext	  ] = "DirViewNext";
	names[Act_FileInfo		  ] = "FileInfo";
								                 
								                 
	///  find, hide
	names[Act_FindHide		  ] = "FindHide";
	names[Act_FindOpen		  ] = "FindOpen";
								                 
	names[Act_DuplicateCur	  ] = "DuplicateCur";
								                 
	names[Act_HideCur		  ] = "HideCur";
	names[Act_ShowCur		  ] = "ShowCur";
								                 
								                 
	///  load, save
	names[Act_SaveAll		  ] = "SaveAll";
	names[Act_ReloadPls		  ] = "ReloadPls";
								                 
	//  insert, delete
	names[Act_InsertDir		  ] = "InsertDir";
	names[Act_InsertFiles	  ] = "InsertFiles";
								                 
	names[Act_ClearPls		  ] = "ClearPls";
	names[Act_DeleteCurFile	  ] = "DeleteCurFile";
	names[Act_DeleteCur		  ] = "DeleteCur";
								                 
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
}
