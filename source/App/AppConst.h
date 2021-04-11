#pragma once


enum ETexUV  // textures in player.png
{
	TX_Prev=0, TX_Next, TX_Up, TX_Dn,  // buttons
	TX_Checks, TX_LeRi,
	TX_PosBr, TX_PosDk,  // pos bar

	TX_TabCur, TX_TabPlay, TX_TabSel,  // Cur,Play,Sel, Find,Bookm
	TX_BackPlr, TX_BackBtm, TX_BackHelp,  // backgr
	
	TX_PlsCur, TX_PlsPlay, TX_PlsSel, //TX_PlsF,
	TX_Slider, TX_SliCur, TX_SliPlay, TX_SliF,
	TX_DShow, TX_DHide,  // hide + -

	TX_PlsB1, TX_PlsB2, TX_PlsB3, TX_PlsB4, TX_PlsB5, TX_PlsB6,  // bookmarks
	TX_SliB1, TX_SliB2, TX_SliB3, TX_SliB4, TX_SliB5, TX_SliB6,
	TX_Rate1, TX_Rate2, TX_Rate3, TX_Rate4, TX_Rate5,  // rating
	TX_TabB1, TX_TabB2, TX_TabB3, TX_TabB4, TX_TabB5, TX_TabB6,  // tab bookm
	
	TX_Black,  // clear
	TexUVNum
};

//  size x,y, pos x,y  on player.png texture, in pixels
struct STexUV
{
	int w,h, x,y;
};
extern const STexUV ciTexUV[TexUVNum+1];
