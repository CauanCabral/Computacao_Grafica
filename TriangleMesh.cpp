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
//  OVERVIEW: TriangleMesh.cpp
//  ========
//  Source file for simple triangle mesh.

#ifndef __TriangleMesh_h
#include "TriangleMesh.h"
#endif

//
// Auxiliary function
//
inline void
printVec3(FILE*f, const char* s, const Vec3& p)
{
	fprintf(f, "%s<%g, %g, %g>\n", s, p.x, p.y, p.z);
}

using namespace Graphics;


//////////////////////////////////////////////////////////
//
// TriangleMesh implementation
// ============
TriangleMesh::Data
TriangleMesh::Data::copy(const Data& d)
//[]---------------------------------------------------[]
//|  Copy data                                          |
//[]---------------------------------------------------[]
{
	Data c;

	if (d.vertices != 0)
	{
		c.numberOfVertices = d.numberOfVertices;
		copyNewArray(c.vertices, d.vertices, d.numberOfVertices);
	}
	if (d.normals != 0)
	{
		c.numberOfNormals = d.numberOfNormals;
		copyNewArray(c.normals, d.normals, d.numberOfNormals);
	}
	if (d.triangles != 0)
	{
		c.numberOfTriangles = d.numberOfTriangles;
		copyNewArray(c.triangles, d.triangles, d.numberOfTriangles);
	}
	return c;
}

void
TriangleMesh::Data::setMaterial(const Material& material)
//[]---------------------------------------------------[]
//|  Set material                                       |
//[]---------------------------------------------------[]
{
	for (int i = 0; i < numberOfTriangles; i++)
		triangles[i].materialIndex = material.getIndex();
}

void
TriangleMesh::Data::transform(const Transf3& t)
//[]---------------------------------------------------[]
//|  Transform                                          |
//[]---------------------------------------------------[]
{
	for (int i = 0; i < numberOfVertices; i++)
		t.transformRef(vertices[i]);
	if (normals != 0)
		for (int i = 0; i < numberOfNormals; i++)
			t.transformVectorRef(normals[i]);
}

void
TriangleMesh::Data::print(FILE* f)
//[]---------------------------------------------------[]
//|  Print                                              |
//[]---------------------------------------------------[]
{
	fprintf(f, "mesh\n{\n\tvertices\n\t{\n\t\t%d\n", numberOfVertices);
	for (int i = 0; i < numberOfVertices; i++)
		printVec3(f, "\t\t", vertices[i]);
	fprintf(f, "\t}\n");
	if (normals != 0)
	{
		fprintf(f, "\tnormals\n\t{\n\t\t%d\n", numberOfNormals);
		for (int i = 0; i < numberOfNormals; i++)
			printVec3(f, "\t\t", normals[i]);
		fprintf(f, "\t}\n");
	}
	fprintf(f, "\ttriangles\n\t{\n\t\t%d\n", numberOfTriangles);

	Triangle* t = triangles;

	for (int i = 0; i < numberOfTriangles; i++, t++)
	{
		fprintf(f, "\t\t<%d, %d, %d>", t->v[0], t->v[1], t->v[2]);
		if (normals != 0)
			fprintf(f, "/<%d, %d, %d>", t->n[0], t->n[1], t->n[2]);
		fputc('\n', f);
	}
	fprintf(f, "\t}\n}\n");
}
