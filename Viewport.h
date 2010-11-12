#ifndef __Viewport_h
#define __Viewport_h

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
// OVERVIEW: Viewport.h
// ========
// Class definition for DC viewport.

#ifndef __DCPoint_h
#include "DCPoint.h"
#endif

namespace Graphics
{ // begin namespace Graphics


//////////////////////////////////////////////////////////
//
// DCLine: DC line class
// =======
class DCLine
{
public:
	// Constructors
	DCLine()
	{
		// do nothing
	}
	DCLine(int x1, int y1, int x2, int y2)
	{
		setPoints(x1, y1, x2, y2);
	}
	DCLine(const DCPoint& p1, const DCPoint& p2)
	{
		setPoints(p1, p2);
	}

	bool operator ==(const DCLine& line) const
	{
		return p1 == line.p1 && p2 == line.p2;
	}

	bool operator !=(const DCLine& line) const
	{
		return !operator ==(line);
	}

	void setPoints(const DCLine& line)
	{
		p1 = line.p1;
		p2 = line.p2;
	}
	void setPoints(int x1, int y1, int x2, int y2)
	{
		p1.x = x1;
		p1.y = y1;
		p2.x = x2;
		p2.y = y2;
	}
	void setPoints(const DCPoint& p1, const DCPoint& p2)
	{
		this->p1 = p1;
		this->p2 = p2;
	}

	DCPoint p1;
	DCPoint p2;

}; // DCLine


//////////////////////////////////////////////////////////
//
// Viewport: viewport class
// ========
class Viewport
{
public:
	// Constructors
	Viewport()
	{
		// do nothing
	}
	Viewport(int x1, int y1, int x2, int y2)
	{
		setCorners(x1, y1, x2, y2);
	}
	Viewport(const DCPoint& p1, const DCPoint& p2)
	{
		setCorners(p1, p2);
	}

	Viewport& operator +=(const Viewport&);

	Viewport operator +(const Viewport& v) const
	{
		int x1 = p1.x < v.p1.x ? p1.x : v.p1.x;
		int y1 = p1.y < v.p1.y ? p1.y : v.p1.y;
		int x2 = p2.x > v.p2.x ? p2.x : v.p2.x;
		int y2 = p2.y > v.p2.y ? p2.y : v.p2.y;

		return Viewport(x1, y1, x2, y2);
	}

	bool operator ==(const Viewport& v) const
	{
		return p1 == v.p1 && p2 == v.p2;
	}

	bool operator !=(const Viewport& v) const
	{
		return !operator ==(v);
	}

	bool isEmpty() const
	{
		return p1.x >= p2.x || p1.y >= p2.y;
	}

	DCPoint center() const
	{
		return DCPoint((p2.x + p1.x) >> 1, (p2.y + p1.y) >> 1);
	}

	int height() const
	{
		return p2.y - p1.y;
	}

	int width() const
	{
		return p2.x - p1.x;
	}

	void setCorners(const Viewport& v)
	{
		p1 = v.p1;
		p2 = v.p2;
	}
	void setCorners(int x1, int y1, int x2, int y2)
	{
		p1.x = x1;
		p1.y = y1;
		p2.x = x2;
		p2.y = y2;
		normalize();
	}
	void setCorners(const DCPoint& p1, const DCPoint& p2)
	{
		this->p1 = p1;
		this->p2 = p2;
		normalize();
	}

	void setEmpty()
	{
		p1.x = p1.y = INT_MAX;
		p2.x = p2.y = INT_MIN;
	}

	bool contains(const DCPoint& p) const
	{
		return p.x >= p1.x && p.x <= p2.x && p.y >= p1.y && p.y <= p2.y;
	}
	bool contains(const DCPoint& p1, const DCPoint& p2) const
	{
		return contains(p1) && contains(p2);
	}
	bool contains(const DCLine& line) const
	{
		return contains(line.p1, line.p2);
	}
	bool contains(const Viewport& v) const
	{
		return contains(v.p1, v.p2);
	}

	bool intersects(const DCPoint& p) const
	{
		return contains(p);
	}
	bool intersects(const DCLine& line) const
	{
		return intersects(line.p1, line.p2);
	}
	bool intersects(const Viewport& v) const
	{
		return p1.x < v.p2.x && p2.x > v.p1.x && p1.y < v.p2.y && p2.y > v.p1.y;
	}

	bool intersects(const DCPoint&, const DCPoint&) const;
	void normalize();

	DCPoint p1; // viewport corners
	DCPoint p2;

private:
	int getOutcode(const DCPoint&) const;

}; // Viewport

} // end namespace Graphics

#endif
