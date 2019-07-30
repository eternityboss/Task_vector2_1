#pragma once
#include <map>
#include "point.h"
#include "surface.h"
#include <algorithm>
#include <utility>
#include <list>
#include <vector>
using namespace std;

class ReadSTLFile
{
public:
	bool ReadFile(const char *cfilename);
	int Numtri();
	int Numtri1();
	unsigned int unTriangles;
	int pointnum;
	map<point, int> pointlist;
	list<surface> listnum;
	vector<point> list1;
	void printASCIIfile();
	void printBinaryfile();
	bool unique_finder(const point& first, const point& second);

private:

	bool ReadASCII(const char *buffer);
	char *memwriter;

};