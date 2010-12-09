#ifndef __TriangleMeshShape_h
#define __TriangleMeshShape_h

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
//  OVERVIEW: TriangleMeshShape.h
//  ========
//  Class definition for triangle mesh shape.

#ifndef __Model_h
#include "Model.h"
#endif
#ifndef __TriangleMesh_h
#include "TriangleMesh.h"
#endif


//////////////////////////////////////////////////////////
//
// TriangleMeshShape: simple triangle mesh class
// =================
class TriangleMeshShape: public Primitive, public TriangleMesh
{
public:
	// Constructor
	TriangleMeshShape(const Data& data):
		TriangleMesh(data)
	{
		// do nothing
	}

	Object* makeCopy() const;
	BoundingBox getBoundingBox() const;
	bool intersect(const Ray&, IntersectInfo&) const;
	Vec3 normal(const IntersectInfo&) const;

	void transform(const Transf3&);
	void setMaterial(Material*);

protected:
	 TriangleMeshShape()
	 {}
}; // TriangleMeshShape

//
// Auxiliary function
//
inline TriangleMeshShape*
toTriangleMesh(Model* model)
{
	return dynamic_cast<TriangleMeshShape*>(model);
}

#endif // __TriangleMeshShape_h
(data)
	{
		// do nothing
	}

	Object* makeCopy() const;
	BoundingBox getBoundingBox() const;
	bool intersect(const Ray&, IntersectInfo&) const;
	Vec3 normal(const IntersectInfo&) const;

	void transform(const Transf3&);
	void setMaterial(Materi