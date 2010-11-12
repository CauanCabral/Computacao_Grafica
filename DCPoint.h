#ifndef __DCPoint_h
#define __DCPoint_h

//[]------------------------------------------------------------------------[]
//|                                                                          |
//|                          GVSG Graphics Library                           |
//|                               Version 1.0                                |
//|                                                                          |
//|                Copyright® 2007, Paulo Aristarco Pagliosa                 |
//|                All Rights Reserved.                                      |
//|                                                                          |
//[]------------------------------------------------------------------------[]
//
// OVERVIEW: DCPoint.h
// ========
// Class definition for DC point.

#include <limits.h>

#ifndef __Typedefs_h
#include "Typedefs.h"
#endif

namespace Graphics
{ // begin namespace Graphics

//
// Auxilary funtion
//
inline int16
sqrt(uint32 value)
{
	if (value <= 0)
		return 0;

	int result = 0;

	for (uint mask = 0x4000; mask; mask >>= 1)
		if (((int32)result + mask) * (result + mask) <= value)
			result |= mask;
	return int16(result);
}


////////////////////////////////////////////////////////////
//
// DCPoint: DC point class
// =======
class DCPoint
{
public:
	int x;
	int y;

	// Constructors
	DCPoint()
	{
		// do nothing
	}
	DCPoint(int x, int y)
	{
		this->x = x;
		this->y = y;
	}

	DCPoint& operator +=(const DCPoint& p)
	{
		x += p.x;
		y += p.y;
		return *this;
	}

	DCPoint& operator -=(const DCPoint& p)
	{
		x -= p.x;
		y -= p.y;
		return *this;
	}

	int& operator[] (int i)
	{
		return (&x)[i];
	}
	const int& operator[] (int i) const
	{
		return (&x)[i];
	}

	DCPoint operator +(const DCPoint& p) const
	{
		return DCPoint(x + p.x, y + p.y);
	}

	DCPoint operator -(const DCPoint& p) const
	{
		return DCPoint(x - p.x, y - p.y);
	}

	bool operator ==(const DCPoint& p) const
	{
		return x == p.x && y == p.y;
	}

	bool operator !=(const DCPoint& p) const
	{
		return !operator ==(p);
	}

	int length() const
	{
		return sqrt(int32(x) * int32(x) + int32(y) * int32(y));
	}

	static int distance(const DCPoint& p1, const DCPoint& p2)
	{
		return (p2 - p1).length();
	}

}; // DCPoint

} // end namespace Graphics

#endif
