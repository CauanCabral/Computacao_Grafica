#ifndef __Vector3_h
#define __Vector3_h

//[]------------------------------------------------------------------------[]
//|                                                                          |
//|                        GVSG Foundation Classes                           |
//|                               Version 1.0                                |
//|                                                                          |
//|                Copyright® 2007, Paulo Aristarco Pagliosa                 |
//|                All Rights Reserved.                                      |
//|                                                                          |
//[]------------------------------------------------------------------------[]
//
// OVERVIEW: Vector3.h
// ========
// Class definition for 3D vector.

#include <stdio.h>

#ifndef __Real_h
#include "Real.h"
#endif

using namespace Math;


//////////////////////////////////////////////////////////
//
// Vec3: 3D vector class
// ====
class Vec3
{
public:
	REAL x;
	REAL y;
	REAL z;
	REAL w;

	// Constructors
	Vec3()
	{
		// do nothing
	}
	Vec3(REAL a, REAL b, REAL c)
	{
		set(a, b, c);
	}
	Vec3(const REAL* v)
	{
		set(v);
	}

	void set(const Vec3& v)
	{
		x = v.x;
		y = v.y;
		z = v.z;
	}
	void set(REAL a, REAL b, REAL c)
	{
		x = a;
		y = b;
		z = c;
	}
	void set(const REAL* v)
	{
		x = v[_X];
		y = v[_Y];
		z = v[_Z];
	}

	Vec3& zero()
	{
		x = y = z = 0;
		return *this;
	}

	bool operator ==(const Vec3& v) const
	{
		return Math::isNull<REAL>(x - v.x, y - v.y, z - v.z);
	}

	bool operator !=(const Vec3& v) const
	{
		return !operator ==(v);
	}

	Vec3& operator +=(const Vec3& b)
	{
		x += b.x;
		y += b.y;
		z += b.z;
		return *this;
	}

	Vec3& operator -=(const Vec3& b)
	{
		x -= b.x;
		y -= b.y;
		z -= b.z;
		return *this;
	}

	Vec3& operator *=(REAL s)
	{
		x *= s;
		y *= s;
		z *= s;
		return *this;
	}

	REAL& operator [](int i)
	{
		return (&x)[i];
	}
	const REAL& operator [](int i) const
	{
		return (&x)[i];
	}

	Vec3 operator +(const Vec3& b) const
	{
		return Vec3(x + b.x, y + b.y, z + b.z);
	}

	Vec3 operator -(const Vec3& b) const
	{
		return Vec3(x - b.x, y - b.y, z - b.z);
	}
	Vec3 operator -() const
	{
		return Vec3(-x, -y, -z);
	}

	REAL operator *(const Vec3& b) const
	{
		return x * b.x + y * b.y + z * b.z;
	}
	Vec3 operator *(REAL s) const
	{
		return Vec3(x * s, y * s, z * s);
	}

	bool isZero() const
	{
		return Math::isNull<REAL>(x, y, z);
	}

	REAL norm() const
	{
		return sqr(x) + sqr(y) + sqr(z);
	}

	REAL length() const
	{
		return sqrt(norm());
	}

	REAL max() const
	{
		return Math::max<REAL>(x, Math::max<REAL>(y, z));
	}

	REAL min() const
	{
		return Math::min<REAL>(x, Math::min<REAL>(y, z));
	}

	Vec3 inverse() const
	{
		return Vec3(1 / x, 1 / y, 1 / z);
	}

	Vec3& negate()
	{
		x = -x;
		y = -y;
		z = -z;
		return *this;
	}

	Vec3& normalize()
	{
		REAL len = length();

		if (!Math::isZero<REAL>(len))
			operator *=(Math::inverse<REAL>(len));
		return *this;
	}

	Vec3 versor() const
	{
		return Vec3(*this).normalize();
	}

	REAL inner(const Vec3& v) const
	{
		return operator*(v);
	}
	REAL inner(REAL x, REAL y, REAL z) const
	{
		return inner(Vec3(x, y, z));
	}

	Vec3 cross(const Vec3& v) const
	{
		REAL a = y * v.z - z * v.y;
		REAL b = z * v.x - x * v.z;
		REAL c = x * v.y - y * v.x;

		return Vec3(a, b, c);
	}
	Vec3 cross(REAL x, REAL y, REAL z) const
	{
		return cross(Vec3(x, y, z));
	}

	void print(const char* s) const
	{
		printf("%s<%f,%f,%f>\n", s, x, y, z);
	}

}; // Vec3

//
// Auxiliary function
//
inline Vec3
operator *(double s, const Vec3& v)
{
	return v * (REAL)s;
}

#endif // __Vector3_h
