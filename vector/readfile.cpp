#pragma once
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

using namespace std;

bool ReadSTLFile::ReadFile(const char *cfilename)
{
	FILE *pfile;
	long  size;
	char *buffer;
	size_t result;


	//打开文件
	fopen_s(&pfile, cfilename, "rb");
	if (pfile == NULL)
	{
		fputs("file error", stderr);
		exit(1);

	}
	//获取文件大小
	fseek(pfile, 0, SEEK_END);
	size = ftell(pfile);

	rewind(pfile);
	//为文件分配内存
	buffer = (char*)malloc(sizeof(char)*size);
	if (buffer == NULL)
	{
		fputs("memory error", stderr);
		exit(2);
	}
	//将文件拷贝到buffer中
	result = fread(buffer, 1, size, pfile);
	if (result != size)
	{
		fputs("reading error", stderr);
		exit(3);
	}
	//关闭文件，释放内存
	fclose(pfile);

	ios::sync_with_stdio(false);
	ReadASCII(buffer);
	ios::sync_with_stdio(true);
	free(buffer);
	return true;
}

const bool sort_finder(const point &a, const point &b)
{
	return (a.x < b.x) || ((a.x == b.x) && (a.y < b.y)) || ((a.x == b.x) && (a.y == b.y) && (a.z < b.z));
}

bool ReadSTLFile::ReadASCII(const char *buffer)
{
	unTriangles = 0;//面片数
	pointnum = 0;//节点数
	float x, y, z;
	int i, j;
	j = 0;
	surface a;
	string name, useless;
	stringstream ss(buffer);
	ss.get();
	getline(ss, useless);
	do
	{
		ss >> useless;
		if (useless != "facet")
			break;
		getline(ss, useless);
		getline(ss, useless);
		for (i = 0; i < 3; i++)
		{
			ss >> useless >> x >> y >> z;

			//存面片数据
			if (i == 0)
			{

				a.point1.x = x;
				a.point1.y = y;
				a.point1.z = z;
				a.point1.surfacenum = unTriangles;
			}
			else 	if (i == 1)
			{

				a.point2.x = x;
				a.point2.y = y;
				a.point2.z = z;
				a.point2.surfacenum = unTriangles;
			}
			else 	if (i == 2)
			{

				a.point3.x = x;
				a.point3.y = y;
				a.point3.z = z;
				a.point3.surfacenum = unTriangles;
			}




			//存节点数据
			list1.push_back(point(x, y, z));
			//pointlist.insert(std::pair<point, int>(point(x, y, z), j));
			//cout << pointlist.begin()->first.x << endl;
			j++;


		}
		cout << list1[0].x << endl;
		listnum.push_back(a);
		unTriangles++;
		// 存一个面片的数据

		getline(ss, useless);
		getline(ss, useless);
		getline(ss, useless);
	} while (1);
	//vector 用sort和unique去重 重载<和==运算符
	sort(list1.begin(), list1.end(), sort_finder);
	list1.erase(unique(list1.begin(), list1.end()), list1.end());
	//把去重数据与surface面类对应
	j = 0;
	for (std::vector<point>::iterator itr = list1.begin(); itr != list1.end(); itr++) 
	{
		(*itr).index = j;//itr表示指向Vector容器的元素的一个指针，可以用指针的方式访问成员(itr->index)
		j++;//或者对象的方式（*itr）.index
	}
	//在list1中找出与surface类相同的坐标对应的行数
	vector<point>::iterator v;
	for (list<surface>::iterator it = listnum.begin(); it != listnum.end(); it++)
	{
		
		v = find(list1.begin(), list1.end(), point(it->point1.x, it->point1.y, it->point1.z));
		it->Point1 = v->index;
		v = find(list1.begin(), list1.end(), point(it->point2.x, it->point2.y, it->point2.z));
		it->Point2 = v->index;
		v = find(list1.begin(), list1.end(), point(it->point3.x, it->point3.y, it->point3.z));
		it->Point3 = v->index;


	
		it->num = 3;
	}
	/*j = 0;
	for (std::map<point, int>::iterator itr = pointlist.begin(); itr != pointlist.end(); itr++)
	{
		itr->second = j;
		j++;


	}
	map<point, int>::iterator v;
	//v=pointlist.find(point(x, y, z));
	//cout << v->second<< endl;
	//在pointlist中找出与surface类相同的坐标对应的行数
	for (list<surface>::iterator it = listnum.begin(); it != listnum.end(); it++)
	{
		v = pointlist.find(point(it->point1.x, it->point1.y, it->point1.z));
		it->Point1 = v->second;
		v = pointlist.find(point(it->point2.x, it->point2.y, it->point2.z));
		it->Point2 = v->second;
		v = pointlist.find(point(it->point3.x, it->point3.y, it->point3.z));
		it->Point3 = v->second;
		it->num = 3;
	}
	*/



	// surface里的数据与去重的数据比较


	pointnum = list1.size();
	return true;

}
//写ASCII码的ply文件
void ReadSTLFile::printASCIIfile()
{
	std::ofstream fileout("taskASCII.ply");

	fileout.precision(std::numeric_limits<double>::digits10);

	if (fileout.is_open())
	{

		fileout << "ply\n";
		fileout << "format ascii 1.0\n";
		fileout << "comment created by platoply\n";
		fileout << "element vertex " << list1.size() << "\n";
		fileout << "property float32 x\n";
		fileout << "property float32 y\n";
		fileout << "property float32 z\n";
		fileout << "element face " << unTriangles << "\n";
		fileout << "property list uint8 int32 vertex_indices\n";
		fileout << "end_header\n";
		for (std::vector<point>::iterator itr = list1.begin(); itr != list1.end(); itr++)
		{
			fileout << itr->x << " " << itr->y << " " << itr->z << "\n";
		}
		for (list<surface>::iterator it = listnum.begin(); it != listnum.end(); it++)
		{
			fileout << it->num << " " << it->Point1 << " " << it->Point2 << " " << it->Point3 << "\n";
		}
		fileout.close();
	}

}
//写二进制文件
void ReadSTLFile::printBinaryfile()
{
	std::ofstream fileout("taskBinary.ply", ios::out | ios::binary);

	fileout.precision(std::numeric_limits<double>::digits10);

	if (fileout.is_open())
	{

		fileout << "ply\n";
		fileout << "format ascii 1.0\n";
		fileout << "comment created by platoply\n";
		fileout << "element vertex " << pointlist.size() << "\n";
		fileout << "property float32 x\n";
		fileout << "property float32 y\n";
		fileout << "property float32 z\n";
		fileout << "element face " << unTriangles << "\n";
		fileout << "property list uint8 int32 vertex_indices\n";
		fileout << "end_header\n";
		for (std::map<point, int>::iterator itr = pointlist.begin(); itr != pointlist.end(); itr++)
		{
			fileout << itr->first.x << " " << itr->first.y << " " << itr->first.z << "\n";
		}
		for (list<surface>::iterator it = listnum.begin(); it != listnum.end(); it++)
		{
			fileout << it->num << " " << it->Point1 << " " << it->Point2 << " " << it->Point3 << "\n";
		}
		fileout.close();
	}
}


//获取面片数
int ReadSTLFile::Numtri()
{
	return unTriangles;
}
//获取节点数
int ReadSTLFile::Numtri1()
{
	return pointnum;
}