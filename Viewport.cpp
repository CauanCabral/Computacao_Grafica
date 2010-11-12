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
// OVERVIEW: Viewport.cpp
// ========
// Source file for DC viewport.

#ifndef __Viewport_h
#include "Viewport.h"
#endif

#define BIT0  1
#define BIT1  2
#define BIT2  4
#define BIT3  8

using namespace Graphics;


//////////////////////////////////////////////////////////
//
// Viewport implementation
// ========
Viewport&
Viewport::operator +=(const Viewport& v)
//[]---------------------------------------------------[]
//|  Add viewport                                       |
//[]---------------------------------------------------[]
{
	if (p1.x > v.p1.x)
		p1.x = v.p1.x;
	if (p1.y > v.p1.y)
		p1.y = v.p1.y;
	if (p2.x < v.p2.x)
		p2.x = v.p2.x;
	if (p2.y < v.p2.y)
		p2.y = v.p2.y;
	return *this;
}

inline int
Viewport::getOutcode(const DCPoint& p) const
//[]---------------------------------------------------[]
//|  Get point outcode                                  |
//[]---------------------------------------------------[]
{
	return ((p.x < p1.x)     ) |
				 ((p.x > p2.x) << 1) |
				 ((p.y < p1.y) << 2) |
				 ((p.y > p2.y) << 3);
}

bool
Viewport::intersects(const DCPoint& pa, const DCPoint& pb) const
//[]---------------------------------------------------[]
//|  Does viewport intersect the line?                  |
//|  Cohen-Sutherland algorithm.                        |
//[]---------------------------------------------------[]
{
	DCPoint p1 = pa;
	DCPoint p2 = pb;

	for (;;)
	{
		int out1 = getOutcode(p1);
		int out2 = getOutcode(p2);
		int code;
		DCPoint* p;

		if ((out1 | out2) == 0)
			return true;
		if ((out1 & out2) != 0)
			return false;
		if (!out1)
		{
			code = out2;
			p = &p2;
		}
		else
		{
			code = out1;
			p = &p1;
		}
		if (code & BIT3)
		{
			(*p).x = int(p1.x + (p2.x - p1.x) * ((this->p2.y - p1.y) /
							 (double)(p2.y - p1.y)));
			(*p).y = this->p2.y;
		}
		else if (code & BIT2)
		{
			(*p).x = int(p1.x + (p2.x - p1.x) * ((this->p1.y - p1.y) /
							 (double)(p2.y - p1.y)));
			(*p).y = this->p1.y;
		}
		else if (code & BIT1)
		{
			(*p).y = int(p1.y + (p2.y - p1.y) * ((this->p2.x - p1.x) /
							 (double)(p2.x - p1.x)));
			(*p).x = this->p2.x;
		}
		else if (code & BIT0)
		{
			(*p).y = int(p1.y + (p2.y - p1.y) * ((this->p1.x - p1.x) /
							 (double)(p2.x - p1.x)));
			(*p).x = this->p1.x;
		}
	}
}

void
Viewport::normalize()
//[]---------------------------------------------------[]
//|  Interchanges the corner points so that P1 < P2     |
//[]---------------------------------------------------[]
{
	if (p1.x > p2.x)
		System::swap(p1.x, p2.x);
	if (p1.y > p2.y)
		System::swap(p1.y, p2.y);
}
