#pragma once
#include <stdint.h>


class Stats
{
	uintmax_t size  = 0;    // bytes
	double    time  = 0.0;  // sec
	int       dirs  = 0;    // counts
	int       files = 0;

public:
	Stats();
	void Clear();

	uintmax_t GetSize()  const {  return size;   }
	double    GetTime()  const {  return time;   }
	int       GetDirs()  const {  return dirs;   }
	int       GetFiles() const {  return files;  }
	
	void Add(const class Track* t);
	void Sub(const class Track* t);
	void AddDir();
	void operator += (const Stats& add);
};
