#include "AppSFML_Draw.h"
#include "Audio.h"
#include "Util.h"
#include "def.h"
#include <cmath>
using namespace std;  using namespace sf;


//  Draw all
void AppSFMLDraw::Draw()
{
	if (Pls().bDraw)
	{	Pls().bDraw = false;
		bDraw = true;
	}
	if (bDraw)
		pWindow->clear();
	
	DrawPlayer();
	DrawTabs();
	
	if (bDraw)
	{	bDraw = false;
		
		DrawPlaylist();
	}
}


//  Player  --
//------------------------------------------------------------------------------------------------------------
void AppSFMLDraw::DrawPlayer()
{
	//for (const auto& e:skin)
	//	e.Draw();
	
	const ViewSet& v = set.view;
	int xw = v.xSize, yw = v.ySize;
	bool play = audio->IsPlaying();

	
	//  backgr  []
	Rect(0,0, xw,yE_vis, TX_BackPlr, false);
	
	
	//  Vis FFT  ~~~~~
	//  params
	int h = yE_vis - yB_vis;
	const int uy=40 /*>0 darker*/, uh=52 /*0 line..fire 512*/;

	if (v.eVis == viFFT && play)
	{
		audio->GetVisData(xw, v);
		float* fft = audio->getFFT();

		for (int i=0; i < xw; ++i)
		{
			float f = fft[i];
			int y = h - f*h;
			f = mia(0.f,1.f, 1.f-f*1.5f);  // mul
			
			Uint8 r,g,b;  // clr
			r = 40+f*f*(255-60);
			g = 100+f*(255-120);
			b = 180+f*(255-200);
			RectUV(i, yB_vis + h-y, 1,y,  475,uy,1,uh, true, r,g,b);
		}
	}
	
	
	//  Position bar  --=---
	int yH = yE_pos - yB_pos;
	Rect(0.f,yB_pos, xw,yH, TX_PosDk, false);

	if (play)
	{
		audio->getPos();
		float xp = (audio->timePlay / audio->timeTrack) * (1.f-xW_pos), //marg,bar dim
			x1 = xp*xw;  int xb = xW_pos*xw;
		
		const auto& t = ciTexUV[TX_PosBr];
		RectUV(x1, yB_pos,  xb, yH, 
			 t.x+t.w*0.48f, t.y,  t.w*0.04f, t.h);
	}

	
	if (bFps)
	{
		Clr(120,160,240);
		str = f2s(1.f / dt);
		Text(Fnt_Info, 60, 16);
	}

	
	//  play icons  >
	int y = 2;
	Clr(50,90,120);
	//str = audio->IsPaused() ? "||" : play ? "|>" : "[]";  // simple
	str = audio->IsPaused() ? String(Uint32(0x25AE)) + String(Uint32(0x25AE))
		: play ? String(Uint32(0x25B6)) : String(Uint32(0x25A0));  // ▮▮ ▶ ■
	Text(Fnt_Info, xw/2, y);
	
	//  repeat  @
	if (audio->bRepTrk || audio->bRepPls)
	{
		Clr(50,100,100);
		str = String(Uint32(0x21BB));  // ↻
		if (audio->bRepTrk)  str += "1";
		//else  str += "A";
		Text(Fnt_Info, xw-75, y+25);  // +Fy TimeBig..
	}
	

	//  player  info text  * * *
	if (play)
	{
		//  ext bitrate freq size
		Clr(110,140,190);
		str = audio->sInfo;
		Text(Fnt_Info, 10, y);
				
		//  time right align..
		str = "0";
		const int w0 = Text(Fnt_TimeBig, 0,0, false);
		string s;  int l, w;

		//  Time cur
		Clr(120,160,220);
		bool below10 = audio->timeTrack < 10.0;
		s = t2s(audio->timePlay, below10);  str = s;
		l = s.length();  w = l * w0;
		Text(Fnt_TimeBig, xw - w - 110, y-2);

		//  Time track
		Clr(100,140,210);
		s = t2s(audio->timeTrack);  str = s;
		l = s.length();  w = l * w0;
		Text(Fnt_TimeBig, xw - w - 20, y-2);
		
		//  track rating
		int rr = audio->rate, r = rr+3;
		str = chFRateVis[r];
		Text(Fnt_TimeBig, xw - 84, y-2);
	}
	
	//  volume  %
	Clr(100,140,220);
	str = i2s(audio->iVolume/10) + "%";
	Text(Fnt_Info, xw-40, y+25);
	
	
	///  debug
	///==================
	if (bDebug)
	{
		Clr(120,180,240);
		/*str = "s "+b2s(shift)+" c "+b2s(ctrl)+" a "+b2s(alt);
		Text(Fnt_Info, 50, 10);/**/
		str = "b "+i2s(yB_pl)+" e "+i2s(yE_pl)+" m "+i2s(ym)+" be "+i2s(yE_pl-yB_pl)+
			" yL " + i2s(yL_pl);/**/
		Text(Fnt_Info, 50, 30);
		str = "t "+i2s(Pls().GetTracks().size())+" c "+i2s(Pls().cur)+
			" o "+i2s(Pls().ofs)+" oe "+i2s(Pls().ofs+yL_pl);
		Text(Fnt_Info, 50, 46);/**/
	#if 0
		float xk1 = (view.xSize-view.xW_plS+1),	 xk2 = view.xSize - xk1;
		//float xp1 = (view.xSize-2*view.xW_plS/3+1),  xp2 = (view.xSize-view.xW_plS/2+1);
		str = f2s(xk1)+" "+i2s(yB_pl)+" "+f2s(xk2)+" "+i2s(yE_pl);
		Text(50,30);

		//str = "l " + i2s(bL?1:0)+" m "+i2s(bM?1:0)+" y "+i2s(ym)+" iv "+f2s(mtiv);
		//Text(50,50);
	#endif
	}
}


//  playlist Tabs
//------------------------------------------------------------------------------------------------------------
void AppSFMLDraw::DrawTabs()
{
	/* multi line  up,dn */
	const ViewSet& view = set.view;
	int ytb = yB_tabs+1;
	if (ytb >= view.ySize)  return;  // fullscr vis
	
	int ntab = vPls.size();
	if (ntab > 0)
	{
		int x,y, a = view.ofsTab;
		for (y=0; y < view.yNpt; y++)
		for (x=0; x < view.xNpt; x++)
		{
			if (a < ntab)
			{
				float yt = yB_tabs + y*yH_tabs;
				float x1 = x*xW_tabs, x2 = xW_tabs;
				
				Rect(x1, yt, x2, yH_tabs, TX_Black);  // clear backgr
				
				//  bookm
				int b = vPls[a].bookm;
				if (b > 0)
				{	int tex = TX_TabB1 + b-1;
					Rect(x1, yt, x2, yH_tabs, ETexUV(tex));
				}
				//  cur, add
				if (a==plsId)    Rect(x1, yt, x2, yH_tabs, TX_TabCur, true);
				if (a==plsPlId)	 Rect(x1, yt, x2, yH_tabs, TX_TabPlay, true);  // playing
				if (a==plsSelId) Rect(x1, yt, x2, yH_tabs, TX_TabSel, true);  // selected
				//if (a==nTabMov)  Rect(x1, yt, x2, yH_tabs, TX_TabCur, true);  // moving

				//  text
				str = vPls[a].name;
				//int w = cf->GetWidth(), o = max(0, (xW_pt-w)/2);  //center
				//cf->xmax = (x+1)*xW_pt;
//				if (bShowSrch && vPlst[a]->iSrch > 0)
//					cf->Fclr = D3DXCOLOR(0.4,1,0.2,1);  else  cf->Fclr = D3DXCOLOR(0.7,0.8,1,1);  // search..
				Text(Fnt_Track, x*xW_tabs/*+o*/, ytb+y*yH_tabs);
			}
			a++;
		}
	}
}


#if 0
void AppSFMLDraw::DrawHeader()
{

	//  info  Total dirs, files, bookm*, size, time
	//----------------------------------------------------------------
	bool bList = Pls().listLen > 0;
	if (yB_pli >= view.ySize)  return;
	
	if (ed!=ED_nFind && ed!=ED_nTab)
	{
	//  get
	DWORD  aD,aF, aSi, aTm;
	if (Pls().numSel > 0)
	{	aD = 0;  aF = Pls().numSel;
		aSi = Pls().selSize/1000000;
		aTm = Pls().selTime;  }
	else if (bAllInfo)
	{	aD= aaD;  aF= aaF;  aSi= aaSi;  aTm= aaTm;  }
	else
	{	aD = Pls().allDirs;  aF = Pls().allFiles;
		aSi = Pls().allSize/1000000;
		aTm = Pls().allTime;
	}
	//  clr
	if (Pls().numSel > 0)  clr(0,1,0.9);  else
	if (bAllInfo)  clr(1,0.7,1);  else
	if (!bList)  clr(1,1,1);  else
	if (!CList::bFilInf)  clr(0.7,0.8,1);  else  clr(0.65,0.75,1);
	
	//  size
	char st[60];
	#define sfmt(s)  sprintf_s(st,sizeof(st)-1, 
	int sg = aSi/1000;  float fsm = aSi/1000.f;
	if (sg < 1)		sfmt(st) "%3d MB", aSi);  else
	if (sg < 10)	sfmt(st) "%4.3f GB", fsm); else
	if (sg < 100)	sfmt(st) "%4.2f GB", fsm);  else
					sfmt(st) "%.1f GB", fsm);
	
	//*L*/sfmt(cf->s) "L ofs %3d  cur %3d  Lin %3d  all %3d", plst->lOfs, plst->lCur, yLpl, plst->listLen);  cf->Write(0, yBpli);
	//*M*/sfmt(cf->s) "xm %4d ym %3d %d%d%d yMd %d %6.3f", xm,ym, bL,bR,bM, yMd, mtiv);  cf->Write(0, yBpli);
	if (aD == 0)
	{	cf->Format("%d  %s", aF, st);  cf->Write(20, yB_pli);  }
	else
	{	cf->Format("%d  %d  %s", aD, aF, st);  cf->Write(0, yB_pli);  }
	if (Pls().bThrTi /*&& plst->itu < plst->listLen*/)  {
		cf->Format("%6.1f%%", 100.f*Pls().itu/float(Pls().listLen));  cf->Write(cf->xwr, yB_pli);  }//-
	
	//  total time  once, str fun..
	DWORD t = aTm, ts,tm,th,td;
	ts= t%60;  t/=60;  tm= t%60;  t/=60;  th= t%24;  td= t/24;
	
		sfmt(st) "%c%c", tm%10+'0', tm/10+'0');  cf->StrCopy(st);	if (th > 0 || td > 0) {  
	  sfmt(st) " h%c%c", th%10+'0', th>9? th/10+'0': td>0?'0':' ');  cf->StrAdd(st);	if (td > 0) {  
	sfmt(st) " d%c%c%c", td%10+'0', td>9? td/10%10+'0':' ', td>99? td/100%10+'0':' ');  cf->StrAdd(st);	 }  }
	cf->dir = -1;  cf->Write(xTm, yB_pli);  cf->dir = 1;
	#undef sfmt
	
	//  num sel
	//if (Pls().numSel > 0)
	//{	clr(0,1,0.9);  cf->Format("%d", Pls().numSel);  cf->Write(view.xSize/2+20,yBpli);  }  //Sel:
	
	clr(1,1,1);  cf->dir = 1;
	cf->Fs[' '] = cf->Fs['0']/2;
	}
}
#endif
