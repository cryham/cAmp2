#include "Util.h"
#include <sstream>
#include <iomanip>
#include <regex>
using namespace std;


//  format
string i2s(const int v, const char width, const char fill)
{
	ostringstream s;
	if (width != 0)  s.width(width);  s.fill(fill);
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

//  time
std::string t2s(float time)
{
	if (time < 10.f)
		return f2s(time,1,3);
	int t = time,
		s = t%60, m = t/60%60, h = t/3600;
	if (h > 0)
		return i2s(h,1)+":"+i2s(m,2,'0')+":"+i2s(s,2,'0');
	
	return i2s(m,1)+":"+i2s(s,2,'0');
}


//  string
vector<string> split(const string& s, const string& reg)
{
	regex re(reg);
	sregex_token_iterator
		first{s.begin(), s.end(), re, -1},  // -1 split
		last;
	return {first, last};
}


string strlower(const string& s)
{
	string ss = s;
	transform(ss.begin(), ss.end(), ss.begin(), ::tolower);
	return ss;
}
