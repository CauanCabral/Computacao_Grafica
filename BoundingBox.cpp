//[]------------------------------------------------------------------------[]
//|                                                                          |
//|                          GVSG Graphics Classes                           |
//|                               Version 1.0                                |
//|                                                                          |
//|                Copyright® 2007, Paulo Aristarco Pagliosa                 |
//|                All Rights Reserved.                                      |
//|                                                                          |
//[]------------------------------------------------------------------------[]
//
//  OVERVIEW: BoundingBox.cpp
//  ========
//  Source file for bounding box.

#ifndef __BoundingBox_h
#include "BoundingBox.h"
#endif

using namespace System;


//////////////////////////////////////////////////////////
//
// BoundingBox implementation
// ===========
void
BoundingBox::set(const Vec3& p1, const Vec3& p2)
//[]---------------------------------------------------[]
//|  Set                                                |
//[]---------------------------------------------------[]
{
	this->p1 = p1;
	this->p2 = p2;
	if (p2.x < p1.x)
		swap(this->p1.x, this->p2.x);
	if (p2.y < p1.y)
		swap(this->p1.y, this->p2.y);
	if (p2.z < p1.z)
		swap(this->p1.z, this->p2.z);
}

void
BoundingBox::transform(const Transf3& m)
//[]---------------------------------------------------[]
//|  Transform                                          |
//[]---------------------------------------------------[]
{
	Vec3 p1 = this->p1;
	Vec3 p2 = this->p2;

	setEmpty();
	for (int i = 0; i < 8; i++)
	{
		Vec3 p = p1;

		if (i & 1)
			p[_X] = p2[_X];
		if (i & 2)
			p[_Y] = p2[_Y];
		if (i & 4)
			p[_Z] = p2[_Z];
		inflate(m.transform(p));
	}
}
