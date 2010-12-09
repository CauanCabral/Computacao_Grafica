//[]------------------------------------------------------------------------[]
//|                                                                          |
//|                          GVSG Graphics Library                           |
//|                               Version 1.0                                |
//|                                                                          |
//|                Copyright® 2007, Paulo Aristarco Pagliosa                 |
//|                Copyright® 2010, Cauan Gama Cabral                        |
//|                All Rights Reserved.                                      |
//|                                                                          |
//[]------------------------------------------------------------------------[]
//
//  OVERVIEW: Sphere.h
//  ========

#ifndef SPHERE_H_
#define SPHERE_H_

#ifndef __Model_h
#include "Model.h"
#endif

namespace Graphics
{ // begin namespace Graphics

class Sphere : public Primitive
{
	protected:
		Vec3 center;
		float radius;
		int segs; // number of segments in 180 degrees

	public:
		Sphere(Vec3 center, float r, int s = 20)
		{
			this->center = center;
			this->radius = r;
			this->segs = s;
		}

		bool intersect(const Ray&, Graphics::IntersectInfo&) const;
		Vec3 normal(const IntersectInfo&) const;
		Material* getMaterial();
		BoundingBox getBoundingBox() const;

		TriangleMesh* getMesh();
		void transform(const Transf3&);
		void setMaterial(Material*);

	};

}

#endif /* SPHERE_H_ */
