#pragma once
#include <vector>
#include <string>
#include "defines.h"


//  to string
std::string i2s(const int v,   const char width=2, const char fill=' ');
std::string f2s(const float v, const char precision=2, const char width=4);
//std::string d2s(const double v, const char precision=2, const char width=4);
std::string b2s(const bool b);
std::string b2on(const bool b);

//  advanced
std::string t2s(const float time, bool float10s = false);
std::string time2s(const uint time);
std::string size2s(const uint size);


//  from string
int    s2i(const char* str);
float  s2f(const char* str);
double s2d(const char* str);
bool   s2b(const char* str);
// uintmax_t ..


//  split string
std::vector<std::string> split(const std::string& s, const std::string& reg);

//  string case lower, UPPER
void strlower(std::string& s);
void strupper(std::string& s);
