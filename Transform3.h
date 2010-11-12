#ifndef __Transform3_h
#define __Transform3_h

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
//  OVERVIEW: Transform3.h
//  ========
//  Class definition for 3D transformation.

#ifndef __Vector3_h
#include "Vector3.h"
#endif

#define MUL4(a, b, i, j) ( \
	a[i][0] * b[0][j] + \
	a[i][1] * b[1][j] + \
	a[i][2] * b[2][j] + \
	a[i][3] * b[3][j])

//
// Auxiliary functions
//
inline void
multiply(const REAL a[4][4], const REAL b[4][4], REAL c[4][4])
{
	REAL c00 = MUL4(a, b, 0, 0);
	REAL c01 = MUL4(a, b, 0, 1);
	REAL c02 = MUL4(a, b, 0, 2);
	REAL c03 = MUL4(a, b, 0, 3);
	REAL c10 = MUL4(a, b, 1, 0);
	REAL c11 = MUL4(a, b, 1, 1);
	REAL c12 = MUL4(a, b, 1, 2);
	REAL c13 = MUL4(a, b, 1, 3);
	REAL c20 = MUL4(a, b, 2, 0);
	REAL c21 = MUL4(a, b, 2, 1);
	REAL c22 = MUL4(a, b, 2, 2);
	REAL c23 = MUL4(a, b, 2, 3);
	REAL c30 = MUL4(a, b, 3, 0);
	REAL c31 = MUL4(a, b, 3, 1);
	REAL c32 = MUL4(a, b, 3, 2);
	REAL c33 = MUL4(a, b, 3, 3);

	c[0][0] = c00; c[0][1] = c01; c[0][2] = c02; c[0][3] = c03;
	c[1][0] = c10; c[1][1] = c11; c[1][2] = c12; c[1][3] = c13;
	c[2][0] = c20; c[2][1] = c21; c[2][2] = c22; c[2][3] = c23;
	c[3][0] = c30; c[3][1] = c31; c[3][2] = c32; c[3][3] = c33;
}

#define DET3(a1, a2, a3, b1, b2, b3, c1, c2, c3) ( \
	a1 * b2 * c3 + \
	a2 * b3 * c1 + \
	a3 * b1 * c2 - \
	a3 * b2 * c1 - \
	a1 * b3 * c2 - \
	a2 * b1 * c3)

inline bool
invert(const REAL m[4][4], REAL c[4][4])
{
	REAL a1 = m[0][0], b1 = m[0][1], c1 = m[0][2], d1 = m[0][3];
	REAL a2 = m[1][0], b2 = m[1][1], c2 = m[1][2], d2 = m[1][3];
	REAL a3 = m[2][0], b3 = m[2][1], c3 = m[2][2], d3 = m[2][3];
	REAL a4 = m[3][0], b4 = m[3][1], c4 = m[3][2], d4 = m[3][3];
	REAL m1 = DET3(b2, b3, b4, c2, c3, c4, d2, d3, d4);
	REAL m2 = DET3(a2, a3, a4, c2, c3, c4, d2, d3, d4);
	REAL m3 = DET3(a2, a3, a4, b2, b3, b4, d2, d3, d4);
	REAL m4 = DET3(a2, a3, a4, b2, b3, b4, c2, c3, c4);
	REAL d;

	if (Math::isZero(d = a1 * m1 + b1 * m2 + c1 * m3 + d1 * m4))
		return false;
	d = Math::inverse<REAL>(d);
	c[0][0] =  m1 * d;
	c[1][0] = -m2 * d;
	c[2][0] =  m3 * d;
	c[3][0] = -m4 * d;
	c[0][1] = -DET3(b1, b3, b4, c1, c3, c4, d1, d3, d4) * d;
	c[1][1] =  DET3(a1, a3, a4, c1, c3, c4, d1, d3, d4) * d;
	c[2][1] = -DET3(a1, a3, a4, b1, b3, b4, d1, d3, d4) * d;
	c[3][1] =  DET3(a1, a3, a4, b1, b3, b4, c1, c3, c4) * d;
	c[0][2] =  DET3(b1, b2, b4, c1, c2, c4, d1, d2, d4) * d;
	c[1][2] = -DET3(a1, a2, a4, c1, c2, c4, d1, d2, d4) * d;
	c[2][2] =  DET3(a1, a2, a4, b1, b2, b4, d2, d2, d4) * d;
	c[3][2] = -DET3(a1, a2, a4, b1, b2, b4, c1, c2, c4) * d;
	c[0][3] = -DET3(b1, b2, b3, c1, c2, c3, d1, d2, d3) * d;
	c[1][3] =  DET3(a1, a2, a3, c1, c2, c3, d1, d2, d3) * d;
	c[2][3] = -DET3(a1, a2, a3, b1, b2, b3, d1, d2, d3) * d;
	c[3][3] =  DET3(a1, a2, a3, b1, b2, b3, c1, c2, c3) * d;
	return true;
}


//////////////////////////////////////////////////////////
//
// Transf3: 3D transformation class
// =======
class Transf3
{
public:
	Transf3& identity()
	{
		M[_X][_X] = 1; M[_X][_Y] = 0; M[_X][_Z] = 0; M[_X][_W] = 0;
		M[_Y][_X] = 0; M[_Y][_Y] = 1; M[_Y][_Z] = 0; M[_Y][_W] = 0;
		M[_Z][_X] = 0; M[_Z][_Y] = 0; M[_Z][_Z] = 1; M[_Z][_W] = 0;
		M[_W][_X] = 0; M[_W][_Y] = 0; M[_W][_Z] = 0; M[_W][_W] = 1;
		return *this;
	}

	Transf3& translation(const Vec3& d)
	{
		M[_X][_X] = 1; M[_X][_Y] = 0; M[_X][_Z] = 0; M[_X][_W] = d.x;
		M[_Y][_X] = 0; M[_Y][_Y] = 1; M[_Y][_Z] = 0; M[_Y][_W] = d.y;
		M[_Z][_X] = 0; M[_Z][_Y] = 0; M[_Z][_Z] = 1; M[_Z][_W] = d.z;
		M[_W][_X] = 0; M[_W][_Y] = 0; M[_W][_Z] = 0; M[_W][_W] = 1;
		return *this;
	}

	Transf3& scale(const Vec3& s)
	{
		M[_X][_X] = s.x; M[_X][_Y] = 0;   M[_X][_Z] = 0;   M[_X][_W] = 0;
		M[_Y][_X] = 0;   M[_Y][_Y] = s.y; M[_Y][_Z] = 0;   M[_Y][_W] = 0;
		M[_Z][_X] = 0;   M[_Z][_Y] = 0;   M[_Z][_Z] = s.z; M[_Z][_W] = 0;
		M[_W][_X] = 0;   M[_W][_Y] = 0;   M[_W][_Z] = 0;   M[_W][_W] = 1;
		return *this;
	}
	Transf3& scale(REAL s)
	{
		return scale(Vec3(s, s, s));
	}
	Transf3& scale(const Vec3& p, const Vec3& s)
	{
		// _X row
		M[_X][_X] = s.x;
		M[_X][_Y] = 0;
		M[_X][_Z] = 0;
		M[_X][_W] = p.x * (1 - s.x);
		// _Y row
		M[_Y][_X] = 0;
		M[_Y][_Y] = s.y;
		M[_Y][_Z] = 0;
		M[_Y][_W] = p.y * (1 - s.y);
		// _Z row
		M[_Z][_X] = 0;
		M[_Z][_Y] = 0;
		M[_Z][_Z] = s.z;
		M[_Z][_W] = p.z * (1 - s.z);
		// _W row
		M[_W][_X] = 0;
		M[_W][_Y] = 0;
		M[_W][_Z] = 0;
		M[_W][_W] = 1;
		return *this;
	}
	Transf3& scale(const Vec3& p, REAL s)
	{
		return scale(p, Vec3(s, s, s));
	}

	Transf3& rotationX(REAL a)
	{
		REAL cosA = cos(a);
		REAL sinA = sin(a);

		M[_X][_X] = 1; M[_X][_Y] = 0;    M[_X][_Z] =  0;    M[_X][_W] = 0;
		M[_Y][_X] = 0; M[_Y][_Y] = cosA; M[_Y][_Z] = -sinA; M[_Y][_W] = 0;
		M[_Z][_X] = 0; M[_Z][_Y] = sinA; M[_Z][_Z] =  cosA; M[_Z][_W] = 0;
		M[_W][_X] = 0; M[_W][_Y] = 0;    M[_W][_Z] =  0;    M[_W][_W] = 1;
		return *this;
	}

	Transf3& rotationY(REAL a)
	{
		REAL cosA = cos(a);
		REAL sinA = sin(a);

		M[_X][_X] =  cosA; M[_X][_Y] = 0; M[_X][_Z] = sinA; M[_X][_W] = 0;
		M[_Y][_X] =  0;    M[_Y][_Y] = 1; M[_Y][_Z] = 0;    M[_Y][_W] = 0;
		M[_Z][_X] = -sinA; M[_Z][_Y] = 0; M[_Z][_Z] = cosA; M[_Z][_W] = 0;
		M[_W][_X] =  0;    M[_W][_Y] = 0; M[_W][_Z] = 0;    M[_W][_W] = 1;
		return *this;
	}

	Transf3& rotationZ(REAL a)
	{
		REAL cosA = cos(a);
		REAL sinA = sin(a);

		M[_X][_X] = cosA; M[_X][_Y] = -sinA; M[_X][_Z] = 0; M[_X][_W] = 0;
		M[_Y][_X] = sinA; M[_Y][_Y] =  cosA; M[_Y][_Z] = 0; M[_Y][_W] = 0;
		M[_Z][_X] = 0;    M[_Z][_Y] =  0;    M[_Z][_Z] = 1; M[_Z][_W] = 0;
		M[_W][_X] = 0;    M[_W][_Y] =  0;    M[_W][_Z] = 0; M[_W][_W] = 1;
		return *this;
	}

	Transf3& rotation(const Vec3& r, REAL a)
	{
		REAL w = cos(a *= (REAL)0.5);
		Vec3 u = r.versor() * sin(a);

		// _X row
		M[_X][_X] = 1 - 2 * (u.y * u.y + u.z * u.z);
		M[_X][_Y] = 2 * (u.x * u.y - w * u.z);
		M[_X][_Z] = 2 * (u.x * u.z + w * u.y);
		M[_X][_W] = 0;
		// _Y row
		M[_Y][_X] = 2 * (u.x * u.y + w * u.z);
		M[_Y][_Y] = 1 - 2 * (u.x * u.x + u.z * u.z);
		M[_Y][_Z] = 2 * (u.y * u.z - w * u.x);
		M[_Y][_W] = 0;
		// _Z row
		M[_Z][_X] = 2 * (u.x * u.z - w * u.y);
		M[_Z][_Y] = 2 * (u.y * u.z + w * u.x);
		M[_Z][_Z] = 1 - 2 * (u.x * u.x + u.y * u.y);
		M[_Z][_W] = 0;
		// _W row
		M[_W][_X] = 0;
		M[_W][_Y] = 0;
		M[_W][_Z] = 0;
		M[_W][_W] = 1;
		return *this;
	}
	Transf3& rotation(const Vec3& p, const Vec3& r, REAL a)
	{
		REAL w = cos(a *= (REAL)0.5);
		Vec3 u = r.versor() * sin(a);

		// _X row
		M[_X][_X] = 1 - 2 * (u.y * u.y + u.z * u.z);
		M[_X][_Y] = 2 * (u.x * u.y - w * u.z);
		M[_X][_Z] = 2 * (u.x * u.z + w * u.y);
		M[_X][_W] = p.x - (M[_X][_X] * p.x + M[_X][_Y] * p.y + M[_X][_Z] * p.z);
		// _Y row
		M[_Y][_X] = 2 * (u.x * u.y + w * u.z);
		M[_Y][_Y] = 1 - 2 * (u.x * u.x + u.z * u.z);
		M[_Y][_Z] = 2 * (u.y * u.z - w * u.x);
		M[_Y][_W] = p.y - (M[_Y][_X] * p.x + M[_Y][_Y] * p.y + M[_Y][_Z] * p.z);
		// _Z row
		M[_Z][_X] = 2 * (u.x * u.z - w * u.y);
		M[_Z][_Y] = 2 * (u.y * u.z + w * u.x);
		M[_Z][_Z] = 1 - 2 * (u.x * u.x + u.y * u.y);
		M[_Z][_W] = p.z - (M[_Z][_X] * p.x + M[_Z][_Y] * p.y + M[_Z][_Z] * p.z);
		// _W row
		M[_W][_X] = 0;
		M[_W][_Y] = 0;
		M[_W][_Z] = 0;
		M[_W][_W] = 1;
		return *this;
	}

	Transf3& mirror(const Vec3& p, const Vec3& n)
	{
		Vec3 u = n.versor();
		Vec3 d = u * (2 * p.inner(u));

		// _X row
		M[_X][_X] = 1 - 2 * u.x * u.x;
		M[_X][_Y] = - 2 * u.x * u.y;
		M[_X][_Z] = - 2 * u.x * u.z;
		M[_X][_W] = d.x;
		// _Y row
		M[_Y][_X] = M[_X][_Y];
		M[_Y][_Y] = 1 - 2 * u.y * u.y;
		M[_Y][_Z] = - 2 * u.y * u.z;
		M[_Y][_W] = d.y;
		// _Z row
		M[_Z][_X] = M[_X][_Z];
		M[_Z][_Y] = M[_Y][_Z];
		M[_Z][_Z] = 1 - 2 * u.z * u.z;
		M[_Z][_W] = d.z;
		// _W row
		M[_W][_X] = 0;
		M[_W][_Y] = 0;
		M[_W][_Z] = 0;
		M[_W][_W] = 1;
		return *this;
	}

	void setColumn(int j, REAL x, REAL y, REAL z, REAL w)
	{
		M[_X][j] = x;
		M[_Y][j] = y;
		M[_Z][j] = z;
		M[_W][j] = w;
	}

	void setRow(int i, REAL x, REAL y, REAL z, REAL w)
	{
		M[i][_X] = x; M[i][_Y] = y; M[i][_Z] = z; M[i][_W] = w;
	}

	Transf3& compose(const Transf3& m)
	{
		::multiply(m.M, M, M);
		return *this;
	}

	bool invert()
	{
		return ::invert(M, M);
	}

	bool inverse(Transf3& m) const
	{
		return (m = *this).invert();
	}

	Vec3 transform(const Vec3& p) const
	{
		REAL x = M[_X][_X] * p.x + M[_X][_Y] * p.y + M[_X][_Z] * p.z + M[_X][_W];
		REAL y = M[_Y][_X] * p.x + M[_Y][_Y] * p.y + M[_Y][_Z] * p.z + M[_Y][_W];
		REAL z = M[_Z][_X] * p.x + M[_Z][_Y] * p.y + M[_Z][_Z] * p.z + M[_Z][_W];

		return Vec3(x, y, z);
	}

	Vec3 transformVector(const Vec3& v) const
	{
		REAL x = M[_X][_X] * v.x + M[_X][_Y] * v.y + M[_X][_Z] * v.z;
		REAL y = M[_Y][_X] * v.x + M[_Y][_Y] * v.y + M[_Y][_Z] * v.z;
		REAL z = M[_Z][_X] * v.x + M[_Z][_Y] * v.y + M[_Z][_Z] * v.z;

		return Vec3(x, y, z);
	}

	Vec3& transformRef(Vec3& p) const
	{
		return p = transform(p);
	}

	Vec3& transformVectorRef(Vec3& v) const
	{
		return v = transformVector(v);
	}

	Transf3& operator *=(const Transf3& m)
	{
		::multiply(M, m.M, M);
		return *this;
	}

	REAL& operator ()(int i, int j)
	{
		return M[i][j];
	}
	const REAL& operator ()(int i, int j) const
	{
		return M[i][j];
	}

	Vec3 operator *(const Vec3& p) const
	{
		return transform(p);
	}

private:
	REAL M[4][4];

}; // Transf3

#endif // __Transform3_h
