#ifndef __TriangleMesh_h
#define __TriangleMesh_h

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
//  OVERVIEW: TriangleMesh.h
//  ========
//  Class definition for simple triangle mesh.

#include <memory.h>

#ifndef __Material_h
#include "Material.h"
#endif
#ifndef __Ray_h
#include "Ray.h"
#endif

//
// Auxiliary functions
//
template <typename T>
inline void
copyArray(T* dst, const T* src, int n)
{
	memcpy(dst, src, n * sizeof(T));
}

template <typename T>
inline void
copyNewArray(T*& dst, const T* src, int n)
{
	copyArray<T>(dst = new T[n], src, n);
}

namespace Graphics
{ // begin namespace Graphics

//
// Auxliary functions
//
inline void
eliminateDominant(Vec3 &v, int d)
{
	if (d == 0)
		v.x = v.z;
	else if (d == 1)
		v.y = v.z;
}

inline Vec3
triangleNormal(const Vec3& v0, const Vec3& v1, const Vec3& v2)
{
	return ((v1 - v0).cross(v2 - v0)).versor();
}

inline Vec3
triangleNormal(Vec3* v)
{
	return triangleNormal(v[0], v[1], v[2]);
}

inline Vec3
triangleNormal(Vec3* v, int i, int j, int k)
{
	return triangleNormal(v[i], v[j], v[k]);
}

inline Vec3
triangleNormal(Vec3* v, int i[3])
{
	return triangleNormal(v[i[0]], v[i[1]], v[i[2]]);
}

inline Vec3
triangleCenter(const Vec3& v0, const Vec3& v1, const Vec3& v2)
{
	return (v0 + v1 + v2) * Math::inverse<REAL>(3);
}

inline Vec3
triangleCenter(Vec3* v)
{
	return triangleCenter(v[0], v[1], v[2]);
}

inline Vec3
triangleCenter(Vec3* v, int i, int j, int k)
{
	return triangleCenter(v[i], v[j], v[k]);
}

inline Vec3
triangleCenter(Vec3* v, int i[3])
{
	return triangleCenter(v[i[0]], v[i[1]], v[i[2]]);
}


//////////////////////////////////////////////////////////
//
// Triangle: simple triangle mesh class
// ========
struct Triangle
{
	Vec3 v0, v1, v2;
	Vec3 N;

	// Constructors
	Triangle()
	{
		// do nothing
	}
	Triangle(const Vec3& p0, const Vec3& p1, const Vec3& p2)
	{
		v0 = p0;
		v1 = p1;
		v2 = p2;
		computeNormal();
	}
	Triangle(Vec3* p)
	{
		v0 = p[0];
		v1 = p[1];
		v2 = p[2];
		computeNormal();
	}
	Triangle(Vec3* p, int i[3])
	{
		v0 = p[i[0]];
		v1 = p[i[1]];
		v2 = p[i[2]];
		computeNormal();
	}

	Vec3 center() const
	{
		return triangleCenter(v0, v1, v2);
	}

	template <typename T>
	static T interpolate(const Vec3& p, const T& v0, const T& v1, const T& v2)
	{
		return v0 * p.x + v1 * p.y + v2 * p.z;
	}
	template <typename T>
	static T interpolate(const Vec3& p, T v[3])
	{
		return interpolate<T>(p, v[0], v[1], v[2]);
	}

	bool intersect(const Ray& ray, Vec3& p, REAL& t)
	{
		REAL d = ray.direction * N;

		if (Math::isZero(d))
			return false;
		t = -((ray.origin - v0) * N) / d;
		if (t <= Math::zero<REAL>())
			return false;
		p = makeRayPoint(ray, t);

		// find the normal's dominant axis
		int dominantAxis = 0;

		if (fabs(N[1]) > fabs(N[0]))
			dominantAxis = 1;
		if (fabs(N[2]) > fabs(N[dominantAxis]))
			dominantAxis = 2;

		Vec3 b = v1 - v0;
		Vec3 c = v2 - v0;

		p -= v0;
		eliminateDominant(p, dominantAxis);
		eliminateDominant(b, dominantAxis);
		eliminateDominant(c, dominantAxis);
		d = Math::inverse(b.y * c.x - b.x * c.y);

		REAL uf = (p.y * c.x - p.x * c.y) * d;

		if (uf < 0 || 1 < uf)
			return false;

		REAL vf = (p.x * b.y - p.y * b.x) * d;

		if (vf < 0 || 1 < vf || (uf + vf) > 1)
			return false;
		p[1] = uf;
		p[2] = vf;
		p[0] = 1 - (uf + vf);
		return true;
	}

private:
	void computeNormal()
	{
		N = triangleNormal(v0, v1, v2);
	}

}; // Triangle


//////////////////////////////////////////////////////////
//
// TriangleMesh: simple triangle mesh class
// ============
class TriangleMesh
{
public:
	struct Triangle
	{
		int v[3];
		int n[3];
		int materialIndex;

		// Constructor
		Triangle():
			materialIndex(0)
		{
			// do nothing
		}

		void setVertices(int v0, int v1, int v2)
		{
			v[0] = v0;
			v[1] = v1;
			v[2] = v2;
		}

		void setNormal(int i)
		{
			n[0] = n[1] = n[2] = i;
		}
		void setNormals(int n0, int n1, int n2)
		{
			n[0] = n0;
			n[1] = n1;
			n[2] = n2;
		}

	}; // Triangle

	struct Data
	{
		int numberOfVertices;
		Vec3* vertices;
		int numberOfNormals;
		Vec3* normals;
		int numberOfTriangles;
		Triangle* triangles;

		// Constructor
		Data():
			numberOfVertices(0),
			vertices(0),
			numberOfNormals(0),
			normals(0),
			numberOfTriangles(0),
			triangles(0)
		{
			// do nothing
		}

		bool intersect(int i, const Ray& ray, Vec3& p, REAL& d) const
		{
			Graphics::Triangle t(vertices, triangles[i].v);
			return t.intersect(ray, p, d);
		}

		void setMaterial(const Material&);
		void transform(const Transf3&);

		void print(FILE*);

		static Data copy(const Data&);

	}; // Data

	// Constructor
	TriangleMesh(const Data& aData):
		data(aData)
	{
		// do nothing
	}

	// Destructor
	~TriangleMesh()
	{
		delete data.vertices;
		delete data.normals;
		delete data.triangles;
	}

	const Data& getData() const
	{
		return data;
	}

	void setMaterial(const Material& material)
	{
		data.setMaterial(material);
	}
	void transform(const Transf3& t)
	{
		data.transform(t);
	}

protected:
	Data data;

}; // TriangleMesh

} // end namespace Graphics

#endif // __TriangleMesh_h
