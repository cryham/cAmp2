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
	if (pls->Length() <= yL_pl)  return;
	int m = shift ? 1 : ctrl ? yL_pl/2 : 8;
	if (d < 0)  pls->PgOfsDn(m);  else  pls->PgOfsUp(m);
	bDraw = true;
}


/* - - - - - - - - - - - - - - - - - - - - - - - - -  Mouse  - - - - - - - - - - - - - - - - - - - - - - - - */
void App::Mouse()
{
	xms = xm;  xm = xMpos;
	yms = ym;  ym = yMpos;

	//if (!act)  return;
	//btnKeysOk = btnKeysOn = false;
//	bool bMInWnd = xm >= 0 && xm < view.xSize && ym >= 0 && ym < view.ySize;
//	if (!bMInWnd)  return;

	ViewSet& v = set.view;
	int Fy = v.Fy; //cfont[view.cfP]->Fy;

	//  cur trk  ----
	if (!pls)  return; //-
	int cr = max(0, min(pls->Length()-1, (ym-yB_pl)/Fy + pls->ofs));
	/*pTrk dest = NULL;
	if (alt && pls->Length() > 0)
	{
		if (shift) {  pls->lInsM =-1;  pls->lInsPos = pls->lOfs;  } else
		if (ctrl)  {  pls->lInsM = 1;  pls->lInsPos = pls->lOfs+yL_pl-1;  } else
		if (cr < int(pls->vList.size()))
		{
			dest = pls->vList[cr];
			pls->lInsPos = cr;  //ins bar vis
			pls->lInsM = (ym-yB_pl)%Fy >= Fy/2 ? 1 : -1;
			if (cr==pls->Length()-1)  pls->lInsM = 1;
			//  restrictions
			if (dest->sel > 0 || dest->isDir())  pls->lInsPos = -1;
			if (pls->numSel==0 && (cr==pls->lCur ||
				(cr-pls->lCur== 1 && pls->lInsM==-1) ||
				(cr-pls->lCur==-1 && pls->lInsM== 1)))  pls->lInsPos = -1;  // not near sel1
		}
	}	*/
		

	///  player
	//------------------------------------------------
	static bool bLbt = false;
	if (bL && (xm != xms || !bLs) && ym < yB_tabs)  // Left
	{
		/*if (!bLs && ym < yE_pl_btn)  // prev,next  btns |< >|
		{
			pls->Next(xm < v.xSize/2 ? -1 : 1);
			bLbt = true;
			return;
		}/*fixme..*/
		//  change pos <<| >>
		if (!bLs)  bLbt = false;
		if (!bLbt && ym > yB_tabs - 120)  // h par
			audio->chPosAbs(mia(0.,1., (double(xm) / v.xSize - xW_pos*0.5) / (1.0-xW_pos) ));
		return;
	}
	/*if (bR && !bRs && ym < yE_pl_btn && plsPl)  //  Right rating
	{	// prev,next  btns |< >|
		if (xm < view.xSize/2)  plsPl->DecRatePl();  else  plsPl->IncRatePl();
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
	/*nTabMov = -1;
	if (yB_pt >= view.ySize)  return;  // not visible

	if (ym > yB_pt && ym < yE_pt)
	{	int y = (ym-yB_pt)/yH_pt, x = xm/xW_pt, n = y*view.xNpt+x +view.ofsTab;
		if (y < view.yNpt && x < view.xNpt && n < int(vPlst.size()))
			nTabMov = n;  }
	if (bL && !bLs && ym > yB_pt && ym < yE_pt)
	{
		bDrawPlst = true;
		if (xm > view.xSize-xW_pt_btn)  // ofs btns up,dn
		{	if (ym-yB_pt < (yE_pt-yB_pt)/2)
			{	// dec/inc tab x,y Num
				if (ctrl) {  if (view.yNpt>1) view.yNpt--;  }else
				if (shift){  if (view.xNpt>1) view.xNpt--;  }
				else  {  tabPrev(0,1);  return;  }
			}else
				if (ctrl)  view.yNpt++;  else  if (shift)  view.xNpt++;
				else  {  tabNext(0,1);  return;  }
			UpdDim();
		}else
		{	// change Tab
			if (nTabMov != -1)
			if (alt)
				tabMove(nTabMov);  // move
			else
			{	pls->Save();  plsId = nTabMov;  plsChg();	}
		}
		return;
	}*/
	//? if (!bL && bLs)  bDrawPlst = true;


	//  playlist
	//------------------------------------------------------------------------------------------------
	//if (yB_pl >= view.ySize)  return;  // not visible
	
	if (ym > yB_pl)
	{
		///  Right  Play
		if (bR && !bRs && !shift/*move wnd-*/ /*&& pls->Length() > 0*/)
		{
			pls->play = cr;
			pls->Play(false);
			/*if (!t->isDir())
			{	Stop();  //pls->idPl = cr;
				plsPlChg(plsId);  //pls id*/
			bDraw = true;
		}
		
		///  Left
		if (bL && xm < v.xSize - xWex_plS)
		{
			if (!bLs && ym > yB_pl && ym < yE_pl)
			{
				#if 0
				if (alt)		//  Move
				{
					int m = shift? -2: ctrl? 2: pls->lInsM;
					if (m == pls->lInsM && pls->lInsPos == -1)
						return;  //restr
	
					//CList* plFrom = plsSel ? plsSel : pls;
					if (/*plFrom->numSel > 0 ||/**/ plsSel && plsSel != pls)
						pls->MoveSel(m, dest, plsSel);
					else if (pls->numSel > 0)
						pls->MoveSel(m, dest, 0);
					else
						pls->Move1(m, dest);
				}
				else  if (shift) pls->SelRange(cr, ctrl);  //  select range
				else  if (ctrl)  pls->Select1(cr);  //  select 1
				else  pls->Pick(cr);  //  pick
				#endif
				pls->cur = cr;
				
				#if 0
				if (/*plsSel &&*/ (ctrl || shift || alt))  /*plsSel when sel chg*/
					updSelId(1);
				#endif
				bDraw = true;
			}
		}
	
		//  slider pls |
		if (bL && pls->Length() > yL_pl)
		{
			if (xm > v.xSize - xWex_plS)
			{	if (!bLs)
				{	xL_ofs = pls->ofs;  yL_sl = ym;   bL_sl = true;  }
			}else
				if (!bLs)  bL_sl = false;
			
			if (bLs && bL_sl)
			{
				float fle = float(pls->Length());
				pls->ofs = float(ym - yL_sl) / float(yH_pl- yL_pl/fle) *fle + xL_ofs;
				pls->PgOfsDn(0);  pls->PgOfsUp(0);
				bDraw = true;
			}
		}
	}
	
	///  Mid  move pls
	if (bM && bMs && pls->Length() > yL_pl && !shift)
	{
		int yMd = ym - yMs;
		if (abs(yMd) > 15)
		{
			mtiv = 0.6f - 0.07f* pow(float(abs(yMd)-15), 0.45f);  //ms
			if (mti > mtiv)
			{
				mti = 0.f;  int m = mtiv >= 0.f ? 1 : 1+ mtiv/-0.06f;
				if (yMd > 0)  pls->PgOfsDn(m);  else  pls->PgOfsUp(m);
				bDraw = true;
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
