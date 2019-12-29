#include "Util.h"
#include <sstream>
#include <iomanip>
#include <regex>
using namespace std;


//  to string
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

string b2s(const bool b)
{
	return b ? "1" : "0";
}

std::string t2s(float time, bool float10s)
{
	if (/*time < 10.f &&*/ float10s)
		return f2s(time,1,3);
	int t = time,
		s = t%60, m = t/60%60, h = t/3600;
	if (h > 0)
		return i2s(h,1)+":"+i2s(m,2,'0')+":"+i2s(s,2,'0');
	
	return i2s(m,1)+":"+i2s(s,2,'0');
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
        res.push_back (token);
    }
    res.push_back(s.substr(pos_start));
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
