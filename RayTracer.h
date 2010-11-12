#ifndef __RayTracer_h
#define __RayTracer_h

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
//  OVERVIEW: RayTracer.h
//  ========
//  Class definition for simple ray tracer.

#ifndef __Image_h
#include "Image.h"
#endif
#ifndef __Renderer_h
#include "Renderer.h"
#endif

namespace Graphics
{ // begin namespace Graphics


//////////////////////////////////////////////////////////
//
// RayTracer: simple ray tracer class
// =========
class RayTracer: public Renderer
{
public:
	// Constructor
	RayTracer(Scene&, Camera* = 0);

	int getMaxRecursionLevel() const;
	REAL getMinWeight() const;

	void setMaxRecursionLevel(int);
	void setMinWeight(REAL);

	void render();
	virtual void renderImage(Image&);

protected:
	Ray pixelRay;
	int maxRecursionLevel;
	REAL minWeight;

	virtual void scan(Image&);
	virtual void setPixelRay(REAL, REAL);
	virtual REAL trace(const Ray&, Color&, int, REAL);
	virtual bool intersect(const Ray&, IntersectInfo&, REAL);
	virtual bool notShadow(const Ray&, IntersectInfo&, REAL, Color&);

	virtual Color shoot(REAL, REAL);
	virtual Color shade(const Ray&, IntersectInfo&, int, REAL);
	virtual Color background() const;

}; // RayTracer


//////////////////////////////////////////////////////////
//
// RayTracer inline implementation
// =========
inline REAL
RayTracer::getMinWeight() const
{
	return minWeight;
}

inline void
RayTracer::setMinWeight(REAL minWeight)
{
	this->minWeight = minWeight;
}

inline int
RayTracer::getMaxRecursionLevel() const
{
	return maxRecursionLevel;
}

inline void
RayTracer::setMaxRecursionLevel(int maxRecursionLevel)
{
	this->maxRecursionLevel = maxRecursionLevel;
}

} // end namespace Graphics

#endif // __RayTracer_h
