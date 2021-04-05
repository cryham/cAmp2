#include "AppSFML_Draw.h"
#include "Audio.h"
#include "Util.h"
#include "def.h"
#include <cmath>
#include <cstring>
using namespace std;  using namespace sf;


//  Draw all
//----------------------------------------------------
void AppSFMLDraw::DrawAll()
{
	if (Pls().bDraw)
	{	Pls().bDraw = false;
		iDraw = 2;
	}
	if (iDraw > 0)
		pWindow->clear();
	
	DrawPlayer();
	DrawTabs();
	
	if (iDraw > 0)
	{	--iDraw;
		
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
		float* fft = audio->GetFFT();

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
			// todo: fill texture and use shader
		}
	}
	
	
	//  Position bar  --=---
	int yH = yE_pos - yB_pos;
	Rect(0.f,yB_pos, xw,yH, TX_PosDk, false);

	if (play)
	{
		audio->GetPos();
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

	
	//  Prev, Next  |< >|  hover
	int y = 2;
	if (ym > 0 && ym < yE_plr_btn)  // prev,next  btns |< >|
	{
		if (xm < v.xSize/2)  Clr(90,150,210);  else  Clr(50,90,130);
		str = String(Uint32(0x25AE)) + String(Uint32(0x25C0));
		Text(Fnt_Info, xw/2 - 30, y);

		if (xm > v.xSize/2)  Clr(90,150,210);  else  Clr(50,90,130);
		str = String(Uint32(0x25B6)) + String(Uint32(0x25AE));
		Text(Fnt_Info, xw/2 + 10, y);
	}else
	{
		//  play icons  |>
		y = 2;
		Clr(50,90,120);
		//str = audio->IsPaused() ? "||" : play ? "|>" : "[]";  // simple
		str = audio->IsPaused() ? String(Uint32(0x25AE)) + String(Uint32(0x25AE))
			: play ? String(Uint32(0x25B6)) : String(Uint32(0x25A0));  // ▮▮ ▶ ■
		Text(Fnt_Info, xw/2, y);
	}
	
	
	//  Repeat  @
	if (audio->bRepTrk || audio->bRepPls)
	{
		Clr(50,100,100);
		str = String(Uint32(0x21BB));  // ↻
		if (audio->bRepTrk)  str += "1";
		//else  str += "A";
		Text(Fnt_Info, xw-75, y+25);  // +Fy TimeBig..
	}
	
	//  Volume  %
	Clr(100,140,220);
	str = i2s(audio->iVolume/10) + "%";
	Text(Fnt_Info, xw-40, y+25);
	

	//  player  info Text  * * *
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

	//  File info  ----
	if (set.bFileInfo)
	{
		const Track& trk = Pls().GetTracks()[Pls().cur];
		string s = trk.GetPath();
		
		auto f = s.find('/', 12);  // add \n in 3rd /
		if (f != string::npos)
			s.insert(s.begin() + f, '\n');
		f = s.find_last_of('/');  // add \n in last /
		if (f != string::npos)
			s.insert(s.begin() + f, '\n');

		str = Str(s);
		Clr(120,180,240);
		Text(Fnt_Info, 10, 46);  // todo: wrap text..?
	}
	
	//  Osd  ----
	if (dtOsd > 0.f)
	{
		dtOsd -= dt;
		str = osdStr;
		float f = 0.7f * dtOsd / dtOsdShow + 0.3f;
		Clr(160*f,250*f,250*f);
		Text(Fnt_Info, 10, v.eVis == viNone ? 22 : 32);
	}
	else
	//  Find  ----
	if (bFind && iFoundAll > 0)
	{
		str = "Found: "+i2s(iFoundVis)+" of "+i2s(Pls().iFound)+"  All "+i2s(iFoundAll);
		Clr(100,220,100);  // center par
		Text(Fnt_Info, (set.view.xSize - 140) / 2, v.eVis == viNone ? 22 : 32);
	}
	
	///  Debug
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
				const auto& pls = vPls[a];
				float yt = yB_tabs + y*yH_tabs;
				float x1 = x*xW_tabs, x2 = xW_tabs;
				
				Rect(x1, yt, x2, yH_tabs, TX_Black);  // clear backgr
				
				//  bookm
				int b = pls.bookm;
				if (b > 0)
				{	int tex = TX_TabB1 + b-1;
					int d = set.dimTabBck;
					Rect(x1, yt, x2, yH_tabs, ETexUV(tex), false,
						 pls.rB*d/16, pls.gB*d/16, pls.bB*d/16);
				}
				
				//  cur, add
				//Clr(120,170,220);
				int d = set.dimTabTxt;
				Clr(pls.rT*d/16, pls.gT*d/16, pls.bT*d/16);
				if (a==plsId)   {  Clr(220,240,255);  Rect(x1, yt, x2, yH_tabs, TX_TabCur, true);  }
				if (a==plsPlId)	{  Clr(240,240,255);  Rect(x1, yt, x2, yH_tabs, TX_TabPlay, true); }  // playing
				if (a==plsSelId){  Clr(170,240,170);  Rect(x1, yt, x2, yH_tabs, TX_TabSel, true);  }  // selected
				//if (a==nTabMov)  Rect(x1, yt, x2, yH_tabs, TX_TabCur, true);  // moving
				if (bFind && pls.iFound > 0)
					Clr(70,240,70);

				//  text
				str = pls.name;
				const int w = Text(Fnt_Track, 0,0, false);
				int xc = max(0, (xW_tabs - w)/2);  //center
				//cf->xmax = (x+1)*xW_pt;
//				if (bShowSrch && vPlst[a]->iSrch > 0)
//					cf->Fclr = D3DXCOLOR(0.4,1,0.2,1);  else  cf->Fclr = D3DXCOLOR(0.7,0.8,1,1);  // search..
				Text(Fnt_Track, x*xW_tabs + xc, ytb+y*yH_tabs);
			}
			a++;
		}
	}
}


//  Pls header
//------------------------------------------------------------------------------------------------------------
void AppSFMLDraw::DrawPlsHeader()
{
	int low = Pls().GetFilter(true), high = Pls().GetFilter(false);
	Clr(130,160,195);

	//  filter  ` *
	int x = xE_pl_inf*11/20, y = yB_pl_inf;
	EFont fnt = Fnt_Info;

	str = GetRateStr(low);
	Text(fnt, x -11, y);
	str = GetRateStr(high);
	Text(fnt, x +11, y);

	//  info  Total dirs, files, bookm*, size, time
	//----------------------------------------------------------------
	bool bList = Pls().Length() > 0;
	//if (yB_pli >= view.ySize)  return;
	
	//  get
	uint di, fi, si, tm;
	const Stats& st = bAllStats ?
		bFullStats ? allFull : all :
		bFullStats ? Pls().stAll : Pls().stats;
	/*if (Pls().numSel > 0)
	{	aD = 0;  aF = Pls().numSel;
		aSi = Pls().selSize/1000000;
		aTm = Pls().selTime;  }
	else/**/
	{	di = st.GetDirs();
		fi = st.GetFiles();
		si = st.GetSize() / 1000000;
		tm = st.GetTime();
	}
	//  clr
	if (bAllStats){  if (bFullStats)  Clr(160,170,190);  else  Clr(150,150,190);  }
	else		  {  if (bFullStats)  Clr(120,150,190);  else  Clr(100,130,160);  }
	/*if (Pls().numSel > 0)  clr(0,1,0.9);  else
	if (bAllInfo)  clr(1,0.7,1);  else
	if (!bList)  clr(1,1,1);  else
	if (!CList::bFilInf)  clr(0.7,0.8,1);  else  clr(0.65,0.75,1);/**/
	
	//  size
	string s = size2s(si);
	
	//*L*/sfmt(cf->s) "L ofs %3d  cur %3d  Lin %3d  all %3d", plst->lOfs, plst->lCur, yLpl, plst->listLen);  Text(0, yBpli);
	//*M*/sfmt(cf->s) "xm %4d ym %3d %d%d%d yMd %d %6.3f", xm,ym, bL,bR,bM, yMd, mtiv);  Text(0, yBpli);
	y = yB_pl_inf;
	if (di == 0)
	{	str = i2s(fi) +"  "+ s;  Text(fnt, 20, y);  }
	else
	{	str = i2s(di) +"  "+ i2s(fi) +"  "+ s;  Text(fnt, 0, y);  }
//	if (Pls().bThrTi /*&& plst->itu < plst->listLen*/)  {
//		Format("%6.1f%%", 100.f*Pls().itu/float(Pls().listLen));  Text(fnt, xwr, y);  }//-
	
	//  total time
	str = time2s(tm);
	x = xE_pl_inf;
	int w = Text(fnt, x, y, false);
	Text(fnt, x - w, y);
	
	//  num sel
	//if (Pls().numSel > 0)
	//{	clr(0,1,0.9);  cf->Format("%d", Pls().numSel);  Text(view.xSize/2+20,yBpli);  }  //Sel:
}
