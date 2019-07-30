#include "pch.h"
#include <iostream>
#include <vector>
#include <fstream>
#include "point.h"
#include <algorithm>
#include "readfile.h"
#include "surface.h"
#include <string>
#include <utility>
#include <sstream>
#include <ctime>
int main()
{
	time_t begin, end;
	double ret;
	begin = clock();
	ReadSTLFile  RC;
	RC.ReadFile("test.stl");
	RC.printASCIIfile();
	//RC.printBinaryfile();
	cout << "面片数：" << RC.Numtri() << endl;
	cout << "节点数：" << RC.Numtri1() << endl;
	end = clock();
	ret = double(end - begin) / CLOCKS_PER_SEC;
	cout << "运行时间：" << ret << endl;

}
