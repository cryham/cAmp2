#include "App.h"
#include "AppActions.h"
#include "../Audio/Audio.h"
#include "../System/Utilities.h"
#include <SFML/Window/Event.hpp>  // input, use own enum?
using namespace std;  using namespace sf;


//  Key press actions / commands
//------------------------------------------------------------------------------------------------------------

///  play
void App::PlaySetCur()      {  Play(true);        }
void App::Play()            {  Play(false);       }
void App::Pause()           {  audio->Pause();    }
void App::Stop()            {  audio->Stop();     }
void App::Prev()            {  PlsPl().Next(-1);  }  
void App::Next()            {  PlsPl().Next( 1);  }

void App::NextPls()			{  plsId = plsPlId;  TabNext( 1,0,0);  Play(false);  plsPlId = plsId;  }
void App::PrevPls()			{  plsId = plsPlId;  TabNext(-1,0,0);  Play(false);  plsPlId = plsId;  }

//  seek
void App::Backward()        {  audio->SetPos(1, 0, 0);  }
void App::BackwardSlow() 	{  audio->SetPos(1, 1, 0);  }
void App::BackwardFast() 	{  audio->SetPos(1, 0, 1);  }

void App::Forward()      	{  audio->SetPos(0, 0, 0);  }
void App::ForwardSlow()  	{  audio->SetPos(0, 1, 0);  }
void App::ForwardFast()  	{  audio->SetPos(0, 0, 1);  }

//  vol
void App::VolumeUp()		{  audio->SetVol(0, 0, 0);  }
void App::VolumeUpSlow()	{  audio->SetVol(0, 1, 0);  }
void App::VolumeUpFast()	{  audio->SetVol(0, 0, 1);  }

void App::VolumeDown()		{  audio->SetVol(1, 0, 0);  }
void App::VolumeDownSlow()	{  audio->SetVol(1, 1, 0);  }
void App::VolumeDownFast()	{  audio->SetVol(1, 0, 1);  }

//  go
void App::GotoPlay()		{  Pls().GotoPlay();  }
void App::GotoPlaySetPls()	{  plsId = plsPlId;  Pls().GotoPlay();  }
void App::Unselect()		{  Pls().UnSel();  }


///  move cur
void App::PlsUp()			{  Pls().Up(1);  }
void App::PlsUpBy8()		{  Pls().Up(8);  }
void App::PlsDown()			{  Pls().Dn(1);  }
void App::PlsDownBy8()		{  Pls().Dn(8);  }

void App::PlsPageOfsUp()	{  Pls().PgOfsUp(1);  }
void App::PlsPageOfsUp4()	{  Pls().PgOfsUp(8);  }
void App::PlsPageOfsDown()	{  Pls().PgOfsDn(1);  }
void App::PlsPageOfsDown4()	{  Pls().PgOfsDn(8);  }

void App::PlsPageUp()		{  Pls().PgUp(yL_pl);  }
void App::PlsPageUp4()		{  Pls().PgUp(yL_pl/4);  }
void App::PlsPageDown()		{  Pls().PgDn(yL_pl);  }
void App::PlsPageDown4()	{  Pls().PgDn(yL_pl/4);  }

void App::PlsListHome()		{  Pls().Home( 2);  }
void App::PlsDirUp()		{  Pls().Home( 0);  }
void App::PlsHome()			{  Pls().Home( 1);  }
void App::PlsHome4()		{  Pls().Home(-1);  }

void App::PlsListEnd()		{  Pls().End( 2);  }
void App::PlsDirDown()		{  Pls().End( 0);  }
void App::PlsEnd()			{  Pls().End( 1);  }
void App::PlsEnd4()			{  Pls().End(-1);  }


//  rate
void App::RateInc()			{  Pls().Rate(1, 1);  }
void App::RateIncCur()		{  Pls().Rate(0, 1);  }
void App::RateDec()			{  Pls().Rate(1,-1);  }
void App::RateDecCur()		{  Pls().Rate(0,-1);  }

void App::FilterLowUp()		{  Pls().Filter(1, 1);  }
void App::FilterHighUp()	{  Pls().Filter(0, 1);  }
void App::FilterLowDown()	{  Pls().Filter(1,-1);  }
void App::FilterHighDown()	{  Pls().Filter(0,-1);  }

//  bookm
void App::BookmInc()		{  Pls().Bookm(0, 1);  Redraw();  }
void App::BookmDec()		{  Pls().Bookm(0,-1);  Redraw();  }

void App::BookmIncTab()		{  Pls().Bookm(1, 1);  Redraw();  }
void App::BookmDecTab()		{  Pls().Bookm(1,-1);  Redraw();  }


///  tab cur
void App::TabNext()			{  TabNext( 1,0,0);  }
void App::TabPrev()			{  TabNext(-1,0,0);  }
void App::TabNextRow()		{  TabNext( 1,1,0);  }
void App::TabPrevRow()		{  TabNext(-1,1,0);  }

void App::TabOfsNext()		{  TabNext( 1,0,1);  }
void App::TabOfsPrev()		{  TabNext(-1,0,1);  }
void App::TabOfsNextRow()	{  TabNext( 1,1,1);  }
void App::TabOfsPrevRow()	{  TabNext(-1,1,1);  }

//  tab new
void App::TabNew()			{  TabNew( 1);  }
void App::TabNewBegin()		{  TabNew(-2);  }
void App::TabNewEnd()		{  TabNew( 2);  }
void App::TabNewBefore()	{  TabNew(-1);  }

void App::TabClose()		{  TabCloses();  }


///  repeat
void App::RepeatTrack()		{  audio->bRepTrk = !audio->bRepTrk;  Osd("Repeat track: " + b2on(audio->bRepTrk));  }
void App::RepeatPls()		{  audio->bRepPls = !audio->bRepPls;  Osd("Repeat playlist: " + b2on(audio->bRepPls));  }

void App::NextVis()			{  NextVisual();  }

//  debug
void App::Fps()				{  bFps = !bFps;      Osd("Fps: " + b2on(bFps));  }
void App::Debug()			{  bDebug = !bDebug;  Osd("Debug: " + b2on(bDebug));  }
void App::Times()			{  ++eTimeTest;  if (eTimeTest >= TimT_ALL)  eTimeTest = TimT_Off;
								Osd(string("Time test: ") + csTimesTest[eTimeTest]);  Redraw();  }
//  view
#define OsdStats  Osd("Stats  Full: " + b2on(bFullStats) + "  All: " + b2on(bAllStats))
void App::AllStats()		{  bAllStats = !bAllStats;  OsdStats;  Redraw();  }
void App::FullStats()		{  bFullStats = !bFullStats;  OsdStats;  Redraw();  }
void App::DirViewNext()		{  set.NextDirView(1);  Osd("Dir View: " + string(csDirView[set.eDirView]));  Redraw();  }
void App::FileInfo()		{  set.bFileInfo = !set.bFileInfo;  Osd("FileInfo: " + b2on(set.bFileInfo));  }


///  find, hide
void App::FindHide()		{  bFind = !bFind;  Redraw();  }
void App::FindOpen()		{  WndOpen(WO_PlsFind);  }

void App::DuplicateCur()	{  Pls().DuplicateCur();  }

void App::HideCur()			{  Pls().HideCur(Hid_Hide);  }
void App::ShowCur()			{  Pls().HideCur(Hid_Show);  }


///  load, save
void App::SaveAll()			{  Save();  }
void App::ReloadPls()		{  Pls().Load();  Osd("Playlist reloaded.");  }

//  insert, delete
void App::InsertDir()		{  OpenDirFile(0);  Osd("Inserted dir.");    }
void App::InsertFiles()		{  OpenDirFile(1);  Osd("Inserted files.");  }

void App::ClearPls()		{  Pls().Clear();  Osd("Playlist cleared.");  }
void App::DeleteCurFile()	{  if (Pls().DeleteCurFile(plsId == plsPlId))  Osd("File deleted.");  }
void App::DeleteCur()		{  Pls().DeleteCur();  }


///  gui wnd  -----
void App::GUIMain()         {  WndOpen((EWndOpt)set.iLastWnd);  }
void App::GUIAppAudio()     {  WndOpen(WO_AppAudio);  }
void App::GUIAppKeys()      {  WndOpen(WO_AppKeys);   }
void App::GUIAppAbout()     {  WndOpen(WO_AppAbout);  }
void App::GUIAppHelp()      {  WndOpen(WO_AppHelp);   }

void App::GUIView()         {  WndOpen(WO_View);  }
void App::GUIViewFonts()    {  WndOpen(WO_ViewFonts);  }
void App::GUIVis()          {  WndOpen(WO_Vis);  }
void App::GUIVisThemes()    {  WndOpen(WO_VisThemes);  }
void App::GUIViewTimes()    {  WndOpen(WO_ViewTimes);  }

void App::GUITab()          {  WndOpen(WO_Tab);  }
void App::GUITabsAll()      {  WndOpen(WO_TabsAll);  }

void App::GUIPlsFind()      {  WndOpen(WO_PlsFind);  }
void App::GUIPlsFilter()    {  WndOpen(WO_PlsFilter);  }
void App::GUIAppStats()     {  WndOpen(WO_AppStats);  }
void App::GUIAppTest()      {  WndOpen(WO_AppTest);  }


///  views
void App::SaveCurView()     {  UpdateView(false, set.iLastView);  }
void App::LoadCurView0()    {  UpdateView(true, 0);  }
void App::LoadCurView1()    {  UpdateView(true, 1);  }
void App::LoadCurView2()    {  UpdateView(true, 2);  }
void App::LoadCurView3()    {  UpdateView(true, 3);  }
void App::LoadCurView4()    {  UpdateView(true, 4);  }
void App::LoadCurView5()    {  UpdateView(true, 5);  }
void App::LoadCurView6()    {  UpdateView(true, 6);  }
void App::LoadCurView7()    {  UpdateView(true, 7);  }
void App::LoadCurView8()    {  UpdateView(true, 8);  }
void App::LoadCurView9()    {  UpdateView(true, 9);  }
void App::LoadCurView10()   {  UpdateView(true, 10);  }
void App::LoadCurView11()   {  UpdateView(true, 11);  }
void App::LoadCurView12()   {  UpdateView(true, 12);  }
void App::LoadCurView13()   {  UpdateView(true, 13);  }
void App::LoadCurView14()   {  UpdateView(true, 14);  }
void App::LoadCurView15()   {  UpdateView(true, 15);  }
void App::LoadCurView16()   {  UpdateView(true, 16);  }
void App::LoadCurView17()   {  UpdateView(true, 17);  }
void App::LoadCurView18()   {  UpdateView(true, 18);  }
void App::LoadCurView19()   {  UpdateView(true, 19);  }


//  Key press
//------------------------------------------------------------------------------------------------------------

void App::KeyPress(Event::KeyEvent k)
{
	auto mkey = ModsKey(shift,ctrl,alt, k.code);
	act->KeyPress(mkey);
}

void App::Save()
{
	// todo: autosave
	int r = 0;  r = RenameRate(!shift);
	Pls().Save();  SaveState();  set.Save();
	string s = "Saved all.";
	if (r)  s += "  Renamed: "+i2s(r);
	Osd(s);
}

void App::UpdModifiers()
{
	#define key(k)  Keyboard::isKeyPressed(Keyboard::k)
	alt = key(LAlt) || key(RAlt);
	ctrl = key(LControl) || key(RControl);
	shift = key(LShift) || key(RShift);
	#undef key
}
