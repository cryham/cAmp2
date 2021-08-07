#include "AppActions.h"
#include "App.h"
#include <SFML/Window/Event.hpp>
using namespace std;  using namespace sf;

	
///  Binding defaults
//...........................................................................................................
void ActionsMap::DefaultBindings()
{
	bindings.clear();

	#define key(k)  Keyboard::k
	//  shift, ctrl, alt

	///  play
	Bind(0,0,0, key(Enter), Act_PlaySetCur);
	Bind(0,0,0, key(Z), Act_Play);
	Bind(0,0,0, key(X), Act_Pause);
	Bind(0,0,0, key(C), Act_Stop);
	Bind(0,0,0, key(A), Act_Prev);
	Bind(0,0,0, key(S), Act_Next);

	Bind(0,1,0, key(A), Act_PrevPls);
	Bind(0,1,0, key(S), Act_NextPls);

	//  seek
	Bind2(0,0,0, key(Q),key(Left), Act_Backward);
	Bind2(0,1,0, key(Q),key(Left), Act_BackwardFast);
	Bind2(1,0,0, key(Q),key(Left), Act_BackwardSlow);
				               
	Bind2(0,0,0, key(W),key(Right), Act_Forward);
	Bind2(0,1,0, key(W),key(Right), Act_ForwardFast);
	Bind2(1,0,0, key(W),key(Right), Act_ForwardSlow);
	
	//  vol
	Bind2(0,0,1, key(Q),key(Left), Act_VolumeDown);
	Bind2(0,1,1, key(Q),key(Left), Act_VolumeDownFast);
	Bind2(1,0,1, key(Q),key(Left), Act_VolumeDownSlow);
				               
	Bind2(0,0,1, key(W),key(Right), Act_VolumeUp);
	Bind2(0,1,1, key(W),key(Right), Act_VolumeUpFast);
	Bind2(1,0,1, key(W),key(Right), Act_VolumeUpSlow);
	
	//  go
	Bind(0,0,0, key(Backspace), Act_GotoPlay);
	Bind(0,1,0, key(Backspace), Act_GotoPlaySetPls);
	Bind(0,0,0, key(Space), Act_Unselect);


	///  move cur
	Bind(0,0,0, key(Up), Act_PlsUp);
	Bind(0,1,0, key(Up), Act_PlsUpBy8);
	Bind(0,0,0, key(Down), Act_PlsDown);
	Bind(0,1,0, key(Down), Act_PlsDownBy8);
								                  
	Bind(0,0,1, key(PageUp), Act_PlsPageOfsUp);
	Bind(0,1,1, key(PageUp), Act_PlsPageOfsUp4);
	Bind(0,0,1, key(PageDown), Act_PlsPageOfsDown);
	Bind(0,1,1, key(PageDown), Act_PlsPageOfsDown4);

	Bind(0,1,0, key(PageUp), Act_PlsPageUp);
	Bind(0,0,0, key(PageUp), Act_PlsPageUp4);
	Bind(0,1,0, key(PageDown), Act_PlsPageDown);
	Bind(0,0,0, key(PageDown), Act_PlsPageDown4);

	Bind(0,0,1, key(Home), Act_PlsListHome);
	Bind(0,0,0, key(Home), Act_PlsDirUp);
	Bind(0,1,0, key(Home), Act_PlsHome);
	Bind(1,0,0, key(Home), Act_PlsHome4);
								                 
	Bind(0,0,1, key(End), Act_PlsListEnd);
	Bind(0,0,0, key(End), Act_PlsDirDown);
	Bind(0,1,0, key(End), Act_PlsEnd);
	Bind(1,0,0, key(End), Act_PlsEnd4);
								                 
								                 
	//  rate
	Bind2(0,0,0, key(Add),      key(Equal),    Act_RateInc);
	Bind2(0,1,0, key(Add),      key(Equal),    Act_RateIncCur);
	Bind2(0,0,0, key(Subtract), key(Hyphen),   Act_RateDec);
	Bind2(0,1,0, key(Subtract), key(Hyphen),   Act_RateDecCur);

	Bind2(0,0,0, key(Multiply), key(RBracket), Act_FilterLowUp);
	Bind2(0,1,0, key(Multiply), key(RBracket), Act_FilterHighUp);
	Bind2(0,0,0, key(Divide),   key(LBracket), Act_FilterLowDown);
	Bind2(0,1,0, key(Divide),   key(LBracket), Act_FilterHighDown);
	
	//  bookm
	Bind(0,0,0, key(Backslash), Act_BookmInc);
	Bind(0,1,0, key(Backslash), Act_BookmDec);
	Bind(0,0,1, key(Backslash), Act_BookmIncTab);
	Bind(0,1,1, key(Backslash), Act_BookmDecTab);

	
	///  tab cur
	Bind(0,0,0, key(Tab),   Act_TabNext);
	Bind(0,0,0, key(Tilde), Act_TabPrev);
	Bind(0,1,0, key(Tab),   Act_TabNextRow);
	Bind(0,1,0, key(Tilde), Act_TabPrevRow);
								                 
	Bind(1,0,0, key(Tab),   Act_TabOfsNext);
	Bind(1,0,0, key(Tilde), Act_TabOfsPrev);
	Bind(1,1,0, key(Tab),   Act_TabOfsNextRow);
	Bind(1,1,0, key(Tilde), Act_TabOfsPrevRow);
								                 
	//  tab new
	Bind(0,0,0, key(N), Act_TabNew);
	Bind(1,0,0, key(N), Act_TabNewBegin);
	Bind(0,1,0, key(N), Act_TabNewEnd);
	Bind(0,0,1, key(N), Act_TabNewBefore);
								                 
	Bind(0,0,0, key(F8), Act_TabClose);
								                 

	///  repeat
	Bind(0,0,0, key(T), Act_RepeatTrack);
	Bind(0,0,0, key(R), Act_RepeatPls);
								                 
	//  debug
	Bind(0,0,0, key(I), Act_Fps);
	Bind(0,0,0, key(U), Act_Debug);//
	Bind(0,0,0, key(M), Act_Times);//
								                 
	//  view
	Bind(0,0,0, key(K), Act_AllStats);//
	Bind(0,0,0, key(L), Act_FullStats);//
	Bind(0,0,0, key(P), Act_NextVis);
	Bind(0,0,0, key(V), Act_DirViewNext);
	Bind(0,0,0, key(Num3), Act_FileInfo);
								                 
								                 
	///  find, hide
	Bind(1,0,0, key(F), Act_FindHide);
	Bind(0,1,0, key(F), Act_FindOpen);  Bind(0,0,1, key(F), Act_FindOpen);

	Bind(0,0,1, key(D), Act_DuplicateCur);

	Bind(0,0,0, key(H), Act_HideCur);
	Bind(0,0,0, key(J), Act_ShowCur);


	///  load, save
	Bind(0,0,0, key(F4), Act_SaveAll);
	Bind(0,0,0, key(F5), Act_ReloadPls);
	
	//  insert, delete
	Bind(0,0,0, key(Insert), Act_InsertDir);
	Bind(1,0,0, key(Insert), Act_InsertFiles);

	Bind(0,1,0, key(Delete), Act_ClearPls);
	Bind(1,0,0, key(Delete), Act_DeleteCurFile);
	Bind(0,0,0, key(Delete), Act_DeleteCur);


	///  gui wnd  -----
	// Act_WO_AppAbout, Act_WO_AppHelp, Act_WO_AppTest, 
	// Act_WO_ViewFonts,  Act_WO_VisThemes, Act_WO_ViewTimes
	Bind(0,0,0, key(F1), Act_WO_Main);
	Bind(0,0,1, key(A), Act_WO_AppAudio);
	Bind(0,0,1, key(K), Act_WO_AppKeys);
	Bind(0,0,1, key(F2), Act_WO_View);
	Bind(0,0,0, key(F3), Act_WO_Vis);  Bind(0,0,1, key(V), Act_WO_Vis);

	Bind(0,0,1, key(F), Act_WO_PlsFind);
	Bind(0,0,1, key(R), Act_WO_PlsFilter);
	Bind(0,0,0, key(F2), Act_WO_Tab);  Bind(0,0,1, key(T), Act_WO_Tab);
	Bind(0,0,1, key(H), Act_WO_TabsAll);
	Bind(0,0,1, key(S), Act_WO_AppStats);
	
	
	///  views
	Bind(0,1,0, key(Num1), Act_SaveCurView);
	Bind(0,0,1, key(Num0), Act_LoadView0);  Bind(1,0,1, key(Num0), Act_LoadView10);
	Bind(0,0,1, key(Num1), Act_LoadView1);  Bind(1,0,1, key(Num1), Act_LoadView11);
	Bind(0,0,1, key(Num2), Act_LoadView2);  Bind(1,0,1, key(Num2), Act_LoadView12);
	Bind(0,0,1, key(Num3), Act_LoadView3);  Bind(1,0,1, key(Num3), Act_LoadView13);
	Bind(0,0,1, key(Num4), Act_LoadView4);  Bind(1,0,1, key(Num4), Act_LoadView14);
	Bind(0,0,1, key(Num5), Act_LoadView5);  Bind(1,0,1, key(Num5), Act_LoadView15);
	Bind(0,0,1, key(Num6), Act_LoadView6);  Bind(1,0,1, key(Num6), Act_LoadView16);
	Bind(0,0,1, key(Num7), Act_LoadView7);  Bind(1,0,1, key(Num7), Act_LoadView17);
	Bind(0,0,1, key(Num8), Act_LoadView8);  Bind(1,0,1, key(Num8), Act_LoadView18);
	Bind(0,0,1, key(Num9), Act_LoadView9);  Bind(1,0,1, key(Num9), Act_LoadView19);

	bGuiUpdate = true;
}
