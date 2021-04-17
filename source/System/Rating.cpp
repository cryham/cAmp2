#include "Rating.h"
#include "defines.h"
using namespace std;


const char* Ratings::sCharRates = "=-`^~+#";  // special -2 -- 
const char Ratings::charRates[Ratings::charsAll] = {'=','-', '`','^','~','+','#'};
const int Ratings::valRates[Ratings::charsAll] = {-3,-1, 1,2,3,4,5};

const std::string Ratings::sRateAdd[Ratings::cntAll] = {"=","--","-","","`","^","~","+","#"};  // rename add, file name
const std::string Ratings::sRateVis[Ratings::cntAll] = {"=","--","-","","`","^","~","+","*"};  // draw text, sybols

const int Ratings::valMin = -Ratings::cntMin, Ratings::valMax = Ratings::cntMax;


//  colors  // todo: in xml..
const sf::Uint8 Ratings::clrTxt[Ratings::cntAll][3] = 
{
	100,140,100, //-3
	110,160,110, //-2
	120,180,120, //-1
	90,150,190, //0
	110,150,210, // 1
	120,170,220, // 2
	120,190,230, // 3
	140,200,240, // 4
	160,210,240, // 5
};
const sf::Uint8 Ratings::clrBck[Ratings::cntAll][3] = 
{
	00, 40, 00, //-3
	10, 60, 10, //-2
	20, 80, 20, //-1
	0, 50, 90, //0
	10, 50,110, // 1
	20, 70,120, // 2
	20, 90,130, // 3
	40,100,140, // 4
	60,110,140, // 5
};


//  Bookmarks
const int Bookmarks::valMax = 6;  // %1..%6

void Bookmarks::Range(int& bookm)
{	bookm = mia(0, valMax, bookm);  }

bool Bookmarks::InRange(const char ch)
{	return ch >='1' && ch <= '0'+valMax;  }


//  Range
//-----------------------------
void Ratings::Range(int& rate)
{	rate = mia(valMin, valMax, rate);  }

ETexUV Ratings::GetTex(int rate)
{
	return (ETexUV)mia(int(TX_Rate1), int(TX_Rate5),
					   int(TX_Rate1 - 1 + abs(rate)) );
}

//  Name
//---------------------------------------------------------------------------------------
void Ratings::GetFromName(const string& name, int& pRate, int& pBookm)
{
	//  get rating  from file name, at end, no ext
	int len = name.length();
	if (len == 0)  return;

	bool search = true;
	int r = 0;
	while (search && r < charsAll)
	{
		if (name[len-1] == charRates[r])
		{
			char rate = valRates[r];
			if (rate==-1 && len >= 2)
			{	if (name[len-2] == charRates[r])
					rate = -2;  //- to --
			}
			pRate = rate;
			search = false;
		}
		++r;
	}
	Bookmarks::GetFromName(name, pBookm);
}

void Bookmarks::GetFromName(const string& name, int& pBookm)
{
	//  get bookmark  %1..%6
	string s(name);
	size_t p = s.find_last_not_of(Ratings::charRates);
	if (p != string::npos &&
		p > 1 && s[p-1]=='%' && Bookmarks::InRange(s[p]))
		pBookm = s[p]-'0';
}

void Ratings::CleanName(std::string& name)
{
	auto p = name.find_last_not_of(sCharRates);
	if (p == string::npos)  return;
	
	if (p > 1 && name[p-1]=='%')
		name.erase(p-1);  // = substr(0,p-1);
	else
		name.erase(p+1);  // = substr(0,p+1);
}
