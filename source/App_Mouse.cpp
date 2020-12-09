#include "App.h"
#include "Audio.h"
#include "def.h"
#include <cmath>
using namespace std;  using namespace sf;


void App::Wheel(int d)
{
	if (d == 0)  return;
	if (ym < yB_tabs)  // plr vol
	{
		audio->chVol(d < 0, shift, ctrl);
		return;
	}
	//  pls scroll
	if (Pls().Length() <= yL_pl)  return;
	int m = shift ? 1 : ctrl ? yL_pl/2 : 8;
	if (d < 0)  Pls().PgOfsDn(m);  else  Pls().PgOfsUp(m);
	Redraw();
}


/* - - - - - - - - - - - - - - - - - - - - - - - - -  Mouse  - - - - - - - - - - - - - - - - - - - - - - - - */
void App::Mouse()
{
	xms = xm;  xm = xMpos;
	yms = ym;  ym = yMpos;

	//if (!act)  return;
	//btnKeysOk = btnKeysOn = false;
//	bool bMInWnd = xm >= 0 && xm < v.xSize && ym >= 0 && ym < v.ySize;
//	if (!bMInWnd)  return;

	ViewSet& v = set.view;
	int Fy = v.Fy; //cfont[v.cfP]->Fy;

	//  cur trk  ----
	//if (!pls)  return; //-
	int cr = max(0, min(Pls().Length()-1, (ym-yB_pl)/Fy + Pls().ofs));
	/*pTrk dest = NULL;
	if (alt && Pls().Length() > 0)
	{
		if (shift) {  Pls().lInsM =-1;  Pls().lInsPos = Pls().lOfs;  } else
		if (ctrl)  {  Pls().lInsM = 1;  Pls().lInsPos = Pls().lOfs+yL_pl-1;  } else
		if (cr < int(Pls().vList.size()))
		{
			dest = Pls().vList[cr];
			Pls().lInsPos = cr;  //ins bar vis
			Pls().lInsM = (ym-yB_pl)%Fy >= Fy/2 ? 1 : -1;
			if (cr==Pls().Length()-1)  Pls().lInsM = 1;
			//  restrictions
			if (dest->sel > 0 || dest->isDir())  Pls().lInsPos = -1;
			if (Pls().numSel==0 && (cr==Pls().lCur ||
				(cr-Pls().lCur== 1 && Pls().lInsM==-1) ||
				(cr-Pls().lCur==-1 && Pls().lInsM== 1)))  Pls().lInsPos = -1;  // not near sel1
		}
	}	*/
		

	///  player
	//------------------------------------------------
	static bool bLbt = false;
	if (bL && (xm != xms || !bLs) && ym < yB_tabs)  // Left
	{
		/*if (!bLs && ym < yE_pl_btn)  // prev,next  btns |< >|
		{
			Pls().Next(xm < v.xSize/2 ? -1 : 1);
			bLbt = true;
			return;
		}/*fixme..*/
		//  change pos <<| >>
		if (!bLs)  bLbt = false;
		if (!bLbt && ym > yB_tabs - 120)  // h par
			audio->chPosAbs(mia(0.,1., (double(xm) / v.xSize - xW_pos*0.5) / (1.0-xW_pos) ));
		//return;
	}
	/*if (bR && !bRs && ym < yE_pl_btn && plsPl)  //  Right rating
	{	// prev,next  btns |< >|
		if (xm < v.xSize/2)  plsPl->DecRatePl();  else  plsPl->IncRatePl();
		bDrawPlst = true;  return;
	}*/
	
	//  Mid
	if (bM && !bMs)
	{	xMs = xm;  yMs = ym;  /*mti = 0.f;*/  yM_visH = v.iVisH;  }
	if (shift)
	{
		if (bM && bMs)  // chng vis size
		{
			v.iVisH = mia(8, v.ySize/*-Fy*4*/, yM_visH + ym-yMs);
			UpdDim();
		}
	}


	//  tabs  ------------------------------------------------
	nTabMov = -1;
	if (yB_tabs >= v.ySize)  return;  // not visible

	if (ym > yB_tabs && ym < yE_tabs)
	{	int y = (ym-yB_tabs)/yH_tabs, x = xm/xW_tabs, n = y*v.xNpt+x +v.ofsTab;
		if (y < v.yNpt && x < v.xNpt && n < int(vPls.size()))
			nTabMov = n;
	}
	if (bL && !bLs && ym > yB_tabs && ym < yE_tabs)
	{
		Redraw();
		if (xm > v.xSize-xW_tabs_btn)  // ofs btns up,dn
		{	if (ym-yB_tabs < (yE_tabs-yB_tabs)/2)
			{	//  dec/inc tab x,y Num
				if (ctrl) {  if (v.yNpt > 1)  v.yNpt--;  } else
				if (shift){  if (v.xNpt > 1)  v.xNpt--;  }
				else  {  TabNext(-1,0,1);  return;  }
			}else
				if (ctrl)  v.yNpt++;  else
				if (shift)  v.xNpt++;
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
		//return;
	}
	//? if (!bL && bLs)  bDrawPlst = true;


	//  playlist
	//------------------------------------------------------------------------------------------------
	//if (yB_pl >= v.ySize)  return;  // not visible
	
	if (ym > yB_pl)
	{
		///  Right  Play
		if (bR && !bRs && !shift/*move wnd-*/ /*&& Pls().Length() > 0*/)
		{
			Pls().play = cr;
			if (Pls().Play(false))  plsPlId = plsId;
			/*if (!t->isDir())
			{	Stop();  //Pls().idPl = cr;
				plsPlChg(plsId);  //pls id*/
			Redraw();
		}
		
		///  Left
		if (bL && xm < v.xSize - xWex_plS)
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
				else  if (shift) Pls().SelRange(cr, ctrl);  //  select range
				else  if (ctrl)  Pls().Select1(cr);  //  select 1
				else  Pls().Pick(cr);  //  pick
				#endif
				Pls().cur = cr;
				
				#if 0
				if (/*plsSel &&*/ (ctrl || shift || alt))  /*plsSel when sel chg*/
					updSelId(1);
				#endif
				Redraw();
			}
		}
	
		//  slider pls |
		if (bL && Pls().Length() > yL_pl)
		{
			if (xm > v.xSize - xWex_plS)
			{	if (!bLs)
				{	xL_ofs = Pls().ofs;  yL_sl = ym;   bL_sl = true;  }
			}else
				if (!bLs)  bL_sl = false;
			
			if (bLs && bL_sl)
			{
				float fle = float(Pls().Length());
				Pls().ofs = float(ym - yL_sl) / float(yH_pl- yL_pl/fle) *fle + xL_ofs;
				Pls().PgOfsDn(0);  Pls().PgOfsUp(0);
				Redraw();
			}
		}
	}
	
	///  Mid  move pls
	if (bM && bMs && Pls().Length() > yL_pl && !shift)
	{
		int yMd = ym - yMs;
		if (abs(yMd) > 15)
		{
			mtiv = 0.6f - 0.07f* pow(float(abs(yMd)-15), 0.45f);  //ms
			if (mti > mtiv)
			{
				mti = 0.f;  int m = mtiv >= 0.f ? 1 : 1+ mtiv/-0.06f;
				if (yMd > 0)  Pls().PgOfsDn(m);  else  Pls().PgOfsUp(m);
				Redraw();
			}
		}
		mti += dt;
	}
	
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
