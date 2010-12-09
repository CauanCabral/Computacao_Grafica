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
//  OVERVIEW: Sphere.cpp
//  ========

#ifndef SPHERE_H_
#include "Sphere.h"
#endif

using namespace Graphics;

bool Sphere::intersect(const Ray& r, Graphics::IntersectInfo& info) const
{
	float B = 2 * (r.direction * (r.origin - this->center));

	float C = (r.origin - this->center) * (r.origin - this->center) - (this->radius * this->radius);

	float D = B * B - 4.0f*C;

	if(D >= 0 && (info.distance = (-B - sqrt(D)) / 2.0f) > 0)
	{
		info.object = (Model*) this;
		info.p = makeRayPoint(r, info.distance);

		return true;
	}

	return false;
}

Vec3 Sphere::normal(const IntersectInfo& info) const
{
	return (info.p - this->center).versor();
}

Material* Sphere::getMaterial()
{
}

BoundingBox Sphere::getBoundingBox() const
{
}

TriangleMesh* Sphere::getMesh()
{
	Vec3 normal;
	Transf3 m;

	int nv = (this->segs - 1) * (this->segs - 1) + 2;
	int nn = nv;
	int nt = nv * 2 * (this->segs - 1);

	// mesh init
	TriangleMesh::Data data;

	data.numberOfVertices = nv;
	data.vertices = new Vec3[nv];

	data.numberOfNormals = nn;
	data.normals = new Vec3[nn];

	data.numberOfTriangles = nt;
	data.triangles = new TriangleMesh::Triangle[nt];

	// startPoint is aligned in X axis with center
	Vec3 startPoint = this->center;
	startPoint.x += this->radius;

	// second point
	Vec3 p = this->center;
	p.y += this->radius;

	// first normal is perpendicular with p and startPoint, at the same time (cross product)
	Vec3 normal = p.cross(startPoint);

	// gera um meio-círculo
	// define rotação, com ponto fixo 'center', em relação ao eixo 'normal' em 180/seg graus
	m.rotation(this->center, normal, Math::toRadians<REAL>(180.0f/this->segs));

	// polos da esfera
	data.vertices[0] = startPoint;
	data.vertices[nv-1] = this->center; data.vertices[nv-1].x -= this->radius;

	for(int i = 1; i < this->segs; i++)
		data.vertices[i] = m.transform(data.vertices[i-1]);

	// agora rotaciona o meio-círulo para gerar a circunferencia
	normal = (startPoint - this->center);

	Vec3 diff = startPoint - this->center;
	diff = diff.inverse();
	REAL invR = diff.length();

	// rotaciona o meio-círculo no eixo contrário, em 360 graus
	m.rotation(this->center, normal, Math::toRadians<REAL>(180.0f/(this->segs)));

	for(int j = 1; j < this->segs; j++)
	{
		for(int i = this->segs * j; i < (this->segs * j + 1); i++)
		{
			data.vertices[i] = m.transform(data.vertices[i-this->segs]);

			data.normals[i] = (data.vertices[i] - this->center) * invR;
		}
	}

	data.normals[0] = (data.vertices[0] - this->center) * invR;
	data.normals[nv - 1] = (data.vertices[i] - this->center) * invR;

	TriangleMesh::Triangle* t = data.triangles;

	// j são os vértices, i é a linha da circunferencia
	for(int j = 1; j < this->nv - 1; j++)
	{
		t->v[0] = j;
		t->v[1] = j - this->segs;
		t->v[1] = j+1;

		t++;

		t->v[0] = j - this->segs;
		t->v[1] = j + 1;
		t->v[2] = j+1-this->segs;

		t++;
	}

	// triangulos dos polos
	for(int j = 1; j < this->segs; j++)
	{
		// polo zero
		t->v[0] = 0;
		t->v[1] = this->segs * j;
		t->v[2] = this->segs * (j + 1);

		t++;

		// polo nv
		t->v[0] = nv - 1;
		t->v[1] = (this->segs - 1) * j;
		t->v[2] = (this->segs - 1) * (j + 1);
	}

	return new TriangleMesh(data);
}

void Sphere::transform(const Transf3& t)
{
}

void Sphere::setMaterial(Material* m)
{

}
