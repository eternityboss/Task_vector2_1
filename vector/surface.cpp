#include "pch.h"
#include <iostream>
#include "surface.h"


void surface::inpoint1(float _x, float _y, float _z, unsigned int Triangles)
{
	point1.x = _x;
	point1.y = _y;
	point1.z = _z;
	point1.surfacenum = Triangles;
}
