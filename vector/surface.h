#pragma once
#include <Windows.h>


class surface 
{
public:
	struct point 
	{
		int surfacenum;
		float x, y, z;
	};
	point point1, point2, point3;
	int num,Point1,Point2,Point3;
	void inpoint1(float _x, float _y, float _z, unsigned int unTriangles);

};