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


	//���ļ�
	fopen_s(&pfile, cfilename, "rb");
	if (pfile == NULL)
	{
		fputs("file error", stderr);
		exit(1);

	}
	//��ȡ�ļ���С
	fseek(pfile, 0, SEEK_END);
	size = ftell(pfile);

	rewind(pfile);
	//Ϊ�ļ������ڴ�
	buffer = (char*)malloc(sizeof(char)*size);
	if (buffer == NULL)
	{
		fputs("memory error", stderr);
		exit(2);
	}
	//���ļ�������buffer��
	result = fread(buffer, 1, size, pfile);
	if (result != size)
	{
		fputs("reading error", stderr);
		exit(3);
	}
	//�ر��ļ����ͷ��ڴ�
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
	unTriangles = 0;//��Ƭ��
	pointnum = 0;//�ڵ���
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

			//����Ƭ����
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




			//��ڵ�����
			list1.push_back(point(x, y, z));
			//pointlist.insert(std::pair<point, int>(point(x, y, z), j));
			//cout << pointlist.begin()->first.x << endl;
			j++;


		}
		cout << list1[0].x << endl;
		listnum.push_back(a);
		unTriangles++;
		// ��һ����Ƭ������

		getline(ss, useless);
		getline(ss, useless);
		getline(ss, useless);
	} while (1);
	//vector ��sort��uniqueȥ�� ����<��==�����
	sort(list1.begin(), list1.end(), sort_finder);
	list1.erase(unique(list1.begin(), list1.end()), list1.end());
	//��ȥ��������surface�����Ӧ
	j = 0;
	for (std::vector<point>::iterator itr = list1.begin(); itr != list1.end(); itr++) 
	{
		(*itr).index = j;//itr��ʾָ��Vector������Ԫ�ص�һ��ָ�룬������ָ��ķ�ʽ���ʳ�Ա(itr->index)
		j++;//���߶���ķ�ʽ��*itr��.index
	}
	//��list1���ҳ���surface����ͬ�������Ӧ������
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
	//��pointlist���ҳ���surface����ͬ�������Ӧ������
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



	// surface���������ȥ�ص����ݱȽ�


	pointnum = list1.size();
	return true;

}
//дASCII���ply�ļ�
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
//д�������ļ�
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


//��ȡ��Ƭ��
int ReadSTLFile::Numtri()
{
	return unTriangles;
}
//��ȡ�ڵ���
int ReadSTLFile::Numtri1()
{
	return pointnum;
}