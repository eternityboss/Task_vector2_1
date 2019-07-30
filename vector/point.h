#pragma once
#include <Windows.h>
#include"surface.h"
#include <iostream>
#include <sstream>
#include <utility>
using namespace std;
class point
{
public:
	float x, y, z;
	int index;

	point(float _x, float _y, float _z);
	bool operator== (const point &a)const
	{


		return (x == a.x&& y == a.y && z == a.z);
	}

};
