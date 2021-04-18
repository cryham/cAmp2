#include "Utilities.h"
#include <sstream>
#include <iomanip>
#include <regex>
using namespace std;


//  to string
string i2s(const int v, const char width, const char fill)
{
	ostringstream s;
	if (width != 0)  s.width(width);
	s.fill(fill);
	s << fixed << v;
	return s.str();
}

string f2s(const float v, const char precision, const char width)
{
	ostringstream s;
	if (width != 0)  s.width(width);
	s << fixed << setprecision(precision) << v;
	return s.str();
}

string b2s(const bool b)
{
	return b ? "1" : "0";
}
string b2on(const bool b)
{
	return b ? "on" : "off";
}


//  advanced
string t2s(const float time, bool float10s, bool addMin0)
{
	if (/*time < 10.f &&*/ float10s)
		return f2s(time,1,3);
	int t = time,
		s = t%60, m = t/60%60, h = t/3600;
	if (h > 0)  // h:mm:ss
		return i2s(h,1)+":"+i2s(m,2,'0')+":"+i2s(s,2,'0');
	if (m > 0 || addMin0)  // m:ss
		return i2s(m,1)+":"+i2s(s,2,'0');
	//  :ss
	return ":"+i2s(s,2,'0');
}

string time2s(const uint time)
{
	uint t = time, ts,tm,th,td;
	ts = t%60;  t /= 60;  tm = t%60;  t /= 60;  th = t%24;  td = t/24;

	string s;
	s = tm%10+'0';  s += tm/10+'0';
	if (th > 0 || td > 0)
	{	s += " h";  s += th%10+'0';  s += th>9? th/10+'0': td>0?'0':' ';
		if (td > 0)
		{	s += " d";  s += td%10+'0';  s += td>9? td/10%10+'0':' ';  s += td>99? td/100%10+'0':' ';
	}	}
	reverse(s.begin(), s.end());
	return s;
}

string size2s(const uint si)
{
	int sg = si/1000;  float fsm = si/1000.f;
	string s;
	if (sg < 1)		s = i2s(si, 3) + " MB";  else
	if (sg < 10)	s = f2s(fsm,3,4) + " GB"; else
	if (sg < 100)	s = f2s(fsm,2,4) + " GB";  else
					s = f2s(fsm,1) + " GB";
	return s;
}


//  from string
int s2i(const char* str)
{
	return (int)strtol(str, NULL, 0);
}

float s2f(const char* str)
{
	return (float)atof(str);
}

double s2d(const char* str)
{
	return atof(str);
}

bool s2b(const char* str)
{
	return ((int)strtol(str, NULL, 0) > 0) ? true : false;
}


//  split
/*vector<string> split(const string& s, const string& reg)
{
	regex re(reg);
	sregex_token_iterator
		first{s.begin(), s.end(), re, -1},  // -1 split
		last;
	return {first, last};
}*/

// for string delimiter
vector<string> split(const string& s, const string& delimiter)
{
	size_t pos_start = 0, pos_end, delim_len = delimiter.length();
	string token;
	vector<string> res;

	while ((pos_end = s.find(delimiter, pos_start)) != string::npos)
	{
		token = s.substr(pos_start, pos_end - pos_start);
		pos_start = pos_end + delim_len;
		res.emplace_back(token);
	}
	res.emplace_back(s.substr(pos_start));
	return res;
}


void strlower(string& s)
{
	transform(s.begin(), s.end(), s.begin(), ::tolower);
}
void strupper(string& s)
{
	transform(s.begin(), s.end(), s.begin(), ::toupper);
}
