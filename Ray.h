#ifndef __Ray_h
#define __Ray_h

//[]------------------------------------------------------------------------[]
//|                                                                          |
//|                        GVSG Foundation Classes                           |
//|                               Version 1.0                                |
//|                                                                          |
//|                Copyright® 2007-2009, Paulo Aristarco Pagliosa            |
//|                All Rights Reserved.                                      |
//|                                                                          |
//[]------------------------------------------------------------------------[]
//
//  OVERVIEW: Ray.h
//  ========
//  Class definition for ray.

#ifndef __Transform3_h
#include "Transform3.h"
#endif
#ifndef __Typedefs_h
#include "Typedefs.h"
#endif


//////////////////////////////////////////////////////////
//
// Ray: ray class
// ===
struct Ray
{
	Vec3 origin;
	Vec3 direction;

	// Constructors
	Ray()
	{
		// do nothing
	}
	Ray(const Vec3& origin, const Vec3& direction)
	{
		set(origin, direction);
	}
	Ray(const Ray& ray, const Transf3& m)
	{
		set(m.transform(ray.origin), m.transformVector(ray.direction));
	}

	void set(const Vec3& origin, const Vec3& direction)
	{
		this->origin = origin;
		this->direction = direction;
	}

	void transform(const Transf3& m)
	{
		m.transformRef(origin);
		m.transformVectorRef(direction);
	}

}; // Ray

//
// Make ray point
//
inline Vec3
makeRayPoint(const Vec3& origin, const Vec3& direction, REAL t)
{
	return origin + direction * t;
}

inline Vec3
makeRayPoint(const Ray& ray, REAL t)
{
	return makeRayPoint(ray.origin, ray.direction, t);
}

namespace Graphics
{ // begin namespace Graphics

//
// Forward definition
//
class Model;


//////////////////////////////////////////////////////////
//
// IntersectInfo: intersection ray/object info class
// =============
struct IntersectInfo
{
	// The intersection point
	Vec3 p;
	// The distance from the ray's origin to the intersection point
	REAL distance;
	// The object intercepted by the ray
	Model* object;
	// Flags
	int flags;
	// Any user data
	void* userData;

}; // IntersectInfo

} // end namespace Graphics

#endif // __Ray_h
