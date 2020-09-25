
#ifndef _LOGGER_H
#define _LOGGER_H 

#include <vector>
#include <cstdio>
#include <cstdarg>
#include <cstring>
// #include "system.hpp"
using namespace std;

class DataLogger{
// friend class System;
public:
	DataLogger(void){}
	static vector<char*> dataBuffer;
	static int append(const char* formatStr,...);
	static void printData(void);
private:
	static char tempBuffer[256];
};




#endif

