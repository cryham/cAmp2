#include "App.h"
#include "../Audio/Audio.h"
#include "../System/defines.h"
#include <cmath>
using namespace std;


void App::WheelMove(int d)
{
	if (d == 0)  return;
	if (ym < yB_tabs)  // plr vol
	{
		audio->SetVol(d < 0, shift, ctrl);
		return;
	}
	//  pls scroll
	if (Pls().LengthVis() <= yL_pl)  return;
	int m = shift ? 1 : ctrl ? yL_pl/2 : 8;
	if (d < 0)  Pls().PgOfsDn(m);  else  Pls().PgOfsUp(m);
	Redraw();
}


/* - - - - - - - - - - - - - - - - - - - - - - - - -  Mouse  - - - - - - - - - - - - - - - - - - - - - - - - */
void App::MouseUpdate()
{
	xms = xm;  xm = xMpos;
	yms = ym;  ym = yMpos;

	//if (!act)  return;
	//btnKeysOk = btnKeysOn = false;
//	bool bMInWnd = xm >= 0 && xm < v.xSize && ym >= 0 && ym < v.ySize;
//	if (!bMInWnd)  return;

	ViewSet& v = set.view;
	const int xw = v.wnd.xSize, yw = v.wnd.ySize;
	const int Fy = v.fnt[Fnt_Tracks].Fy;

	//  cur trk  ----
	int cur = max(0, min(Pls().LengthVis()-1, (ym-yB_pl)/Fy + Pls().iOfs));
	/*pTrk dest = NULL;
	if (alt && Pls().Length() > 0)
	{
		if (shift) {  Pls().lInsM =-1;  Pls().lInsPos = Pls().lOfs;  } else
		if (ctrl)  {  Pls().lInsM = 1;  Pls().lInsPos = Pls().lOfs+yL_pl-1;  } else
		if (cr < int(Pls().vList.size()))
		{
			dest = Pls().vList[cur];
			Pls().lInsPos = cur;  //ins bar vis
			Pls().lInsM = (ym-yB_pl)%Fy >= Fy/2 ? 1 : -1;
			if (cur==Pls().Length()-1)  Pls().lInsM = 1;
			//  restrictions
			if (dest->sel > 0 || dest->isDir())  Pls().lInsPos = -1;
			if (Pls().numSel==0 && (cur==Pls().lCur ||
				(cur-Pls().lCur== 1 && Pls().lInsM==-1) ||
				(cur-Pls().lCur==-1 && Pls().lInsM== 1)))  Pls().lInsPos = -1;  // not near sel1
		}
	}*/
		

	///  player
	//------------------------------------------------
	static bool bLbt = false;
	if (bL && (xm != xms || !bLs) && ym < yB_tabs)  // Left
	{
		if (!bLs && ym < yE_plr_btn)  // prev,next  btns |< >|
		{
			PlsPl().Next(xm < xw/2 ? -1 : 1);
			bLbt = true;
			//return;
		}
		//  change pos <<| >>
		if (!bLs)  bLbt = false;
		if (!bLbt && ym > yB_tabs - 120)  // h par
			audio->SetPosAbs(mia(0.,1., (double(xm) / xw - xW_pos*0.5) / (1.0-xW_pos) ));
		return;
	}
	if (bR && !bRs && ym < yE_plr_btn)  //  Right rating
	{
		// prev,next  btns |< >|
		PlsPl().Rate(true, xm < xw/2 ? -1 : 1);
		return;
	}
	
	//  Mid
	if (bM && !bMs)
	{	xMs = xm;  yMs = ym;  /*mti = 0.f;*/  yM_visH = v.vis.yH;  }
	if (shift)
	{
		if (bM && bMs)  // chng vis size
		{
			v.vis.yH = mia(8, ym/*-Fy*4*/, yM_visH + ym-yMs);
			UpdDim();
		}
	}


	//  tabs
	//------------------------------------------------
	nTabMov = -1;
	if (yB_tabs >= ym)  return;  // not visible

	if (ym > yB_tabs && ym < yE_tabs)
	{
		int y = (ym - yB_tabs) / yH_tabs,
			x = xm / xW_tabs,
			n = y * v.tabs.xCols + x + v.tabs.ofs;
		if (y < v.tabs.yRows && x < v.tabs.xCols &&
			n < int(vPls.size()))
			nTabMov = n;
	}
	int xBtnUpDn = xw - xW_tabs_btn;
	if (bL && !bLs && ym > yB_tabs && ym < yE_tabs)
	{
		Redraw();
		if (xm > xBtnUpDn)  // ofs btns up,dn
		{	if (ym-yB_tabs < (yE_tabs-yB_tabs)/2)
			{	//  dec/inc tab x,y Num
				if (ctrl) {  if (v.tabs.yRows > 1)  v.tabs.yRows--;  } else
				if (shift){  if (v.tabs.xCols > 1)  v.tabs.xCols--;  }
				else  {  TabNext(-1,0,1);  return;  }
			}else
				if (ctrl)  v.tabs.yRows++;  else
				if (shift)  v.tabs.xCols++;
				else  {  TabNext( 1,0,1);  return;  }
			UpdDim();
		}else
		{	// change Tab
			if (nTabMov != -1)
			if (ctrl/*alt*/)
				TabMove(nTabMov);  // move
			else
			{	/*Pls().Save();*/  plsId = nTabMov;  /*plsChg();*/	}
		}
		return;
	}
	//? if (!bL && bLs)  bDrawPlst = true;

	
	///  Right  Wnd Open  areas
	///------------------------------------------------
	if (bR && !bRs)
	{
		/*if (ym < yE_plr_btn)  // bookm - +
			WndOpen(WO_AppAudio, false);
		else if (ym < yE_pos)
			WndOpen(WO_AppVis, false);*/
		if (ym > yE_plr_btn && ym < yE_pos)
			WndOpen(WO_AppAudio, false);
		
		if (ym > yB_tabs && ym < yE_tabs)
			if (xm > xBtnUpDn)
				WndOpen(WO_AppTabs, false);
			else  // set cur tab ..
				WndOpen(WO_PlsTab, false);

		if (ym > yB_pl_inf && ym < yB_pl)
			if (xm > xM_pl_filt - 21 && xm < xM_pl_filt + 21)
				WndOpen(WO_PlsFilter, false);
			else
				WndOpen(WO_AppStats, false);

		if (ym > yB_pl && xm > xw - xWex_plS)
			WndOpen(WO_AppView, false);
		// main, find, vis ..?
	}


	//  playlist
	//------------------------------------------------------------------------------------------------
	if (yB_pl >= yw)  return;  // not visible
	
	if (ym > yB_pl)
	{
		///  Right  Play
		if (bR && !bRs && !shift/*move wnd-*/ &&
			!Pls().IsEmpty() && xm < xw - xWex_plS)
		{
			auto old = Pls().iPlay;
			int id = Pls().GetTrackVisId(cur);
			if (id != -1)
			{
				Pls().iPlay = id;
				if (!Play(false))
				{	Pls().iPlay = old;
					Pls().UpdPlayVis();
				}
		}	}
		
		///  Left
		if (bL && xm < xw - xWex_plS)
		{
			if (!bLs && ym > yB_pl && ym < yE_pl)
			{
				#if 0
				if (alt)		//  Move
				{
					int m = shift? -2: ctrl? 2: Pls().lInsM;
					if (m == Pls().lInsM && Pls().lInsPos == -1)
						return;  //restr
	
					//CList* plFrom = plsSel ? plsSel : pls;
					if (/*plFrom->numSel > 0 ||/**/ plsSel && plsSel != pls)
						Pls().MoveSel(m, dest, plsSel);
					else if (Pls().numSel > 0)
						Pls().MoveSel(m, dest, 0);
					else
						Pls().Move1(m, dest);
				}
				else  if (shift) Pls().SelRange(cur, ctrl);  //  select range
				else  if (ctrl)  Pls().Select1(cur);  //  select 1
				else  Pls().Pick(cur);  //  pick
				#endif
				Pls().iCur = cur;
				
				#if 0
				if (/*plsSel &&*/ (ctrl || shift || alt))  /*plsSel when sel chg*/
					updSelId(1);
				#endif
				Redraw();
			}
		}
	
		//  slider pls |
		if (bL && Pls().LengthVis() > yL_pl)
		{
			if (xm > xw - xWex_plS)
			{	if (!bLs)
				{	xL_ofs = Pls().iOfs;  yL_sl = ym;   bL_sl = true;  }
			}else
				if (!bLs)  bL_sl = false;
			
			if (bLs && bL_sl)
			{
				float fle = float(Pls().LengthVis());
				Pls().iOfs = float(ym - yL_sl) / float(yH_pl- yL_pl/fle) *fle + xL_ofs;
				Pls().PgOfsDn(0);  Pls().PgOfsUp(0);
				Redraw();
			}
		}
	}
	
	///  Mid  move pls
	if (bM && bMs && Pls().LengthVis() > yL_pl && !shift)
	{
		int yMd = ym - yMs;
		if (abs(yMd) > 15)
		{
			mtiv = 0.6f - 0.07f* pow(float(abs(yMd)-15), 0.45f);  //par? ms
			if (mti > mtiv)
			{
				mti = 0.f;  int m = mtiv >= 0.f ? 1 : 1+ mtiv/-0.06f;
				if (yMd > 0)  Pls().PgOfsDn(m);  else  Pls().PgOfsUp(m);
				Redraw();
			}
		}
		mti += dt;
	}
}

void App::MouseUpdateEnd()
{
	bLs = bL;  bRs = bR;  bMs = bM;  // old state
}


void App::MouseDown(int b)
{
	if (b==1)  bL = true;  else
	if (b==2)  bR = true;  else
	if (b==3)  bM = true;
}
void App::MouseUp(int b)
{
	if (b==1)  bL = false;  else
	if (b==2)  bR = false;  else
	if (b==3)  bM = false;
}
