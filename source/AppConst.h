#pragma once


enum ETexUV {
	TX_Prev=0, TX_Next, TX_Up, TX_Dn, TX_Checks, TX_LeRi,
	TX_PosBr, TX_PosDk,
	TX_TabC, TX_TabP, TX_TabS,  //_Cur,Play,Sel,Find,Bookm
	TX_BackPlr, TX_BackBtm, TX_BackHelp,
	TX_PlsC, TX_PlsP, TX_PlsS, //TX_PlsF,
	TX_Slid, TX_SliC, TX_SliP, TX_SliF,
	TX_DShow, TX_DHide,
	TX_PlsB1, TX_PlsB2, TX_PlsB3, TX_PlsB4, TX_PlsB5, TX_PlsB6,
	TX_SliB1, TX_SliB2, TX_SliB3, TX_SliB4, TX_SliB5, TX_SliB6,
	TX_Rate1, TX_Rate2, TX_Rate3, TX_Rate4, TX_Rate5,
	TX_TabB1, TX_TabB2, TX_TabB3, TX_TabB4, TX_TabB5, TX_TabB6,
	TX_Black,
	TexUVNum };

//  size x,y, pos x,y  on player.png texture, in pixels
struct STexUV
{
	int w,h, x,y;
};
extern const STexUV ciTexUV[TexUVNum+1];
