#ifndef __BoundingBox_h
#define __BoundingBox_h

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
//  OVERVIEW: BoundingBox.h
//  ========
//  Class definition for bounding (&p1).

#include <stdlib.h>

#ifndef __Ray_h
#include "Ray.h"
#endif

//
// Auxiliary function
//
inline void
inflateAABB(Vec3& p1, Vec3& p2, const Vec3& p)
{
	if (p.x < p1.x)
		p1.x = p.x;
	if (p.x > p2.x)
		p2.x = p.x;
	if (p.y < p1.y)
		p1.y = p.y;
	if (p.y > p2.y)
		p2.y = p.y;
	if (p.z < p1.z)
		p1.z = p.z;
	if (p.z > p2.z)
		p2.z = p.z;
}


//////////////////////////////////////////////////////////
//
// BoundingBox: axis-aligned bounding box class
// ===========
class BoundingBox
{
public:
	// Constructors
	BoundingBox()
	{
		setEmpty();
	}
	BoundingBox(const Vec3& p1, const Vec3& p2)
	{
		set(p1, p2);
	}
	BoundingBox(const BoundingBox& b, const Transf3& m):
		p1(b.p1),
		p2(b.p2)
	{
		transform(m);
	}

	Vec3 getCenter() const
	{
		return (p1 + p2) * 0.5;
	}

	REAL getDiagonalLength() const
	{
		return (p2 - p1).length();
	}

	Vec3 getSize() const
	{
		return p2 - p1;
	}

	REAL getMaxSize() const
	{
		return getSize().max();
	}

	REAL getArea() const
	{
		Vec3 s = getSize();
		REAL a = s.x * s.y + s.y * s.z + s.z * s.x;

		return a + a;
	}

	bool isEmpty() const
	{
		return p1.x >= p2.x || p1.y >= p2.y || p1.z >= p2.z;
	}

	const Vec3& getP1() const
	{
		return p1;
	}

	const Vec3& getP2() const
	{
		return p2;
	}

	void setEmpty()
	{
		p1.x = p1.y = p1.z = +Math::infinity<REAL>();
		p2.x = p2.y = p2.z = -Math::infinity<REAL>();
	}

	void set(const Vec3&, const Vec3&);

	void inflate(const Vec3& p)
	{
		inflateAABB(p1, p2, p);
	}

	void inflate(REAL s)
	{
		if (Math::isPositive(s))
		{
			Vec3 c = getCenter() * (1 - s);

			p1 = p1 * s + c;
			p2 = p2 * s + c;
		}
	}
	void inflate(const BoundingBox& b)
	{
		inflate(b.p1);
		inflate(b.p2);
	}

	void transform(const Transf3&);

protected:
	Vec3 p1;
	Vec3 p2;

}; // BoundingBox

#endif // __BoundingBox_h
