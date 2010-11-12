#ifndef __Color_h
#define __Color_h

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
//  OVERVIEW: Color.h
//  ========
//  Class definition RGB color.

#ifndef __Vector3_h
#include "Vector3.h"
#endif


//////////////////////////////////////////////////////////
//
// Color: RGB color class
// =====
class Color
{
public:
	REAL r;
	REAL g;
	REAL b;
	REAL a;

	// Constructors
	Color()
	{
		// do nothing
	}
	Color(REAL r, REAL g, REAL b)
	{
		setRGB(r, g, b);
	}
	Color(REAL* c)
	{
		setRGB(c);
	}
	Color(int r, int g, int b)
	{
		setRGB(r, g, b);
	}

	// Setters
	void setRGB(REAL r, REAL g, REAL b)
	{
		this->r = r;
		this->g = g;
		this->b = b;
	}
	void setRGB(REAL* c)
	{
		r = c[0];
		g = c[1];
		b = c[2];
	}
	void setRGB(int r, int g, int b)
	{
		this->r = r * Math::inverse<REAL>(255);
		this->g = g * Math::inverse<REAL>(255);
		this->b = b * Math::inverse<REAL>(255);
	}

	Color operator +(const Color& c) const
	{
		return Color(r + c.r, g + c.g, b + c.b);
	}

	Color operator -(const Color& c) const
	{
		return Color(r - c.r, g - c.g, b - c.b);
	}

	Color operator *(const Color& c) const
	{
		return Color(r * c.r, g * c.g, b * c.b);
	}
	Color operator *(REAL s) const
	{
		return Color(r * s, g * s, b * s);
	}

	const REAL& operator [](int i) const
	{
		return (&r)[i];
	}
	REAL& operator [](int i)
	{
		return (&r)[i];
	}

	Color& operator +=(const Color& c)
	{
		r += c.r;
		g += c.g;
		b += c.b;
		return *this;
	}

	Color& operator -=(const Color& c)
	{
		r -= c.r;
		g -= c.g;
		b -= c.b;
		return *this;
	}

	Color& operator *=(const Color& c)
	{
		r *= c.r;
		g *= c.g;
		b *= c.b;
		return *this;
	}
	Color& operator *=(REAL s)
	{
		r *= s;
		g *= s;
		b *= s;
		return *this;
	}

	bool operator ==(const Color& c) const
	{
		return Math::isNull<REAL>(r - c.r, g - c.g, b - c.b);
	}

	bool operator !=(const Color& c) const
	{
		return !operator ==(c);
	}

	Color operator +(const Vec3& v) const
	{
		return Color(r + v.x, g + v.y, b + v.z);
	}

	Color operator -(const Vec3& v) const
	{
		return Color(r - v.x, g - v.y, b - v.z);
	}

	Color operator *(const Vec3& v) const
	{
		return Color(r * v.x, g * v.y, b * v.z);
	}

	Color& operator =(const Vec3& v)
	{
		r = v.x;
		g = v.y;
		b = v.z;
		return *this;
	}

	Color& operator +=(const Vec3& v)
	{
		r += v.x;
		g += v.y;
		b += v.z;
		return *this;
	}

	Color& operator -=(const Vec3& v)
	{
		r *= v.x;
		g -= v.y;
		b -= v.z;
		return *this;
	}

	Color& operator *=(const Vec3& v)
	{
		r *= v.x;
		g *= v.y;
		b *= v.z;
		return *this;
	}

	void print(const char* s) const
	{
		printf("%srgb(%f,%f,%f)\n", s, r, g, b);
	}

	static Color black;
	static Color red;
	static Color green;
	static Color blue;
	static Color cyan;
	static Color magenta;
	static Color yellow;
	static Color white;
	static Color darkGray;
	static Color gray;

	static Color HSV2RGB(REAL, REAL, REAL);

}; // Color

//
// Auxiliary function
//
inline Color
operator *(double s, const Color& c)
{
	return c * (REAL)s;
}

#endif // __Color_h
