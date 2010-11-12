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

#ifndef __Sweeper_h
#include "Sweeper.h"
#endif


Vec3*
makeCircle(const Vec3& center, const Vec3& startPoint, const Vec3& normal, int seg = 10)
{
	Vec3* points = new Vec3[seg];
	
	Transf3 m;
	
	// define rotação, com ponto fixo 'center', em relação ao eixo 'normal' em 360/seg graus
	m.rotation(center, normal, Math::toRadians<REAL>(360.0f/seg));
	
	points[0] = startPoint;
	
	for(int i = 1; i < seg; i++)
		points[i] = m.transform(points[i-1]);
	
	return points;
}

TriangleMesh*
makeCylinder(const Vec3& center, const Vec3& startPoint, const Vec3& normal, REAL height, int seg = 10)
{
	Vec3* base = makeCirle(center, startPoint, normal, seg);
	
	int nv = seg * 2;
	int nn = seg + 2;
	int nt = nv * 2 - 4;
	
	// estrutura base para definição da malha
	TriangleMesh::Data data;
	
	data.numberOfVertices = nv;
	data.vertices = new Vec3[nv];
	
	data.numberOfNormals = nn;
	data.normals = new Vec3[nn];
	
	data.numberOfTriangles = nt;
	data.triangles = new Triangle[nt];
	
	Vec3 h = normal * height;
	REAL invR = Math::inverse(startPoint - center).length());
	
	for(int i = 0; i < seg; i++)
	{
		data.vertices[i] = base[i] + h;
		data.vertices[i + seg] = base[i];
		
		data.normals[i] = (base[i] - center) * invR;
	}
	
	data.normals[seg] = -normal; // bottom
	data.normals[seg + 1] = normal; // top
	
	TriangleMesh::Triangle* t = data.triangles;
	
	for(int j = seg, i = 0; i < seg;)
	{
		t->v[0] = i;
		t->v[1] = j;
		t->n[0] = t->n[1] = i;
		
		// equivalente a (++t).t->n[0]
		t[1].n[0] = i;
		
		t->v[2] = ++i;
		t->n[2] = i;
		
		t++;
		
		t->v[0] = j;
		t->v[1] = ++j;
		t->v[2] = i;
		t->n[1] = t->n[2] = i;
		
		t++;
	}
	
	// tampas: t em diante
	
	return new TriangleMesh(data);
}


/***
**** 1) Escreva uma aplicação Glut para teste do Sweeper (makeCylinder)
**** 2) Modifique makeCylinder para se ter triângulos cujos vértices possuem ao invés da normal do cilíndro a normal do próprio triângulo.
***/

/*
int main()
{
	Vec3* p = makeCircle(Vec3(0,0,0), Vec3(1,1,0), Vec3(0,0,1), 4);
	
	for(int i = 0; i < 4; i++)
		printf("p[%d] = <%f, %f, %f>\n", i, p[i].x, p[i].y, p[i].z);
	
	delete p;
	
	return 0;
}
*/
