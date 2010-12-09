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
//  OVERVIEW: TriangleMeshShape.cpp
//  ========
//  Source file for triangle mesh shape.

#ifndef __TriangleMeshShape_h
#include "TriangleMeshShape.h"
#endif


//////////////////////////////////////////////////////////
//
// TriangleMeshShape implementation
// =================
Object*
TriangleMeshShape::makeCopy() const
//[]---------------------------------------------------[]
//|  Make copy                                          |
//[]---------------------------------------------------[]
{
	return new TriangleMeshShape(Data::copy(this->data));
}

bool
TriangleMeshShape::intersect(const Ray& ray, IntersectInfo& info) const
//[]---------------------------------------------------[]
//|  Intersect                                          |
//[]---------------------------------------------------[]
{/*só implementar o metodo intersect
cada triangulo sabe ver se um raio intercepta
entao vc só tem q ir de triangulo em triangulo
e ver qual intercepta mais perto
se nao interceptar retorna false*/
	//TODO
	
	int n = getNumberOfTriangles();
	float d = 0.f;
	Vec3 p, closerPoint;
	float closer = M_INFINITY;
	int mat, index = -1;
	
	for(int i = 0 ; i < n ; i++)
		if( data.intersect((int)i, ray, p, d) )
			if(d < closer)
			{
				index = i;
				closer = d;
				closerPoint = p;
				mat = data.triangles[i].materialIndex;
			}
	if(index == -1)
		return false;
	
	info.distance = closer; 
	info.p = closerPoint;
	info.materialIndex = mat;
	return true;

}

Vec3
TriangleMeshShape::normal(const IntersectInfo& info) const
//[]---------------------------------------------------[]
//|  Normal                                             |
//[]---------------------------------------------------[]
{
	return data.normalAt((Triangle*)info.userData, info.p);
}

BoundingBox
TriangleMeshShape::getBoundingBox() const
//[]---------------------------------------------------[]
//|  Bounding box                                       |
//[]---------------------------------------------------[]
{
	BoundingBox box;

	for (int i = 0; i < data.numberOfVertices; i++)
		box.inflate(data.vertices[i]);
	return box;
}

void
TriangleMeshShape::transform(const Transf3& t)
//[]---------------------------------------------------[]
//|  Transform                                          |
//[]---------------------------------------------------[]
{
	for (int i = 0; i < data.numberOfVertices; i++)
		t.transformRef(data.vertices[i]);
	if (data.normals != 0)
		for (int i = 0; i < data.numberOfNormals; i++)
			t.transformVectorRef(data.normals[i]);
}

void
TriangleMeshShape::setMaterial(Material* m)
//[]---------------------------------------------------[]
//|  Set material                                       |
//[]---------------------------------------------------[]
{
	Primitive::setMaterial(m);
	for (int i = 0; i < data.numberOfTriangles; i++)
		data.triangles[i].materialIndex = m->getIndex();
}
ef(data.vertices[i]);
	if (data.normals != 0)
		for (int i = 0; i < data.numberOfNormals; i++)
			t.transformVectorRef(data.normals[i]);
}

void
TriangleMeshShape::setMaterial(Material* m)
//[]---------------------------------------------------[]
//|  Set material                                       |
//[]---------------------------------------------------[]
{
	Primitive::setMate