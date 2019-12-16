#pragma once
#include <vector>
#include <string>


//  format int, float to string
std::string i2s(const int v, const char width=2, const char fill=' ');
std::string f2s(const float v, const char precision=2, const char width=4);
std::string t2s(float time, bool float10s = true);

//  split string
std::vector<std::string> split(const std::string& s, const std::string& reg);

//  string case lower, UPPER
void strlower(std::string& s);
void strupper(std::string& s);
