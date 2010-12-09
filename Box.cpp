#include"Box.h"

Box::Box(Vec3 c, Vec3 o, Vec3 s)
{
	Primitive* p;
	TriangleMesh::Data data;
	
	Vec3* vertices = new Vec3[8]();
	data.numberOfVertices = 8;
	data.vertices = vertices;

	vertices[0].set(-0.5, -0.5, 0.5);
	vertices[1].set(0.5, -0.5, 0.5);
	vertices[2].set(0.5, 0.5, 0.5);
	vertices[3].set(-0.5, 0.5, 0.5);
	vertices[4].set(-0.5, -0.5, -0.5);
	vertices[5].set(0.5, -0.5, -0.5);
	vertices[6].set(0.5, 0.5, -0.5);
	vertices[7].set(-0.5, 0.5, -0.5);

 	//normais dos vertices
	Vec3* normals = new Vec3[6]();
	data.numberOfNormals = 6;
	data.normals = normals;

	normals[0].set(0, -1, 0);
	normals[1].set(1, -0, 0);
	normals[2].set(0, 1, 0);
	normals[3].set(-1, -0, -0);
	normals[4].set(0, 0, 1);
	normals[5].set(-0, -0, -1);

	int n3 = 12;

	TriangleMesh::Triangle* triangles = new TriangleMesh::Triangle[12]();
	data.numberOfTriangles = 12;
	data.triangles = triangles;

		triangles[0].setVertices((int)0, (int)4, (int)1);
		triangles[0].setNormals((int)0, (int)0, (int)0);

		triangles[1].setVertices((int)4, (int)5, (int)1);
		triangles[1].setNormals((int)0, (int)0, (int)0);

		triangles[2].setVertices((int)1, (int)5, (int)2);
		triangles[2].setNormals((int)1, (int)1, (int)1);

		triangles[3].setVertices((int)5, (int)6, (int)2);
		triangles[3].setNormals((int)1, (int)1, (int)1);

		triangles[4].setVertices((int)2, (int)6, (int)3);
		triangles[4].setNormals((int)2, (int)2, (int)2);

		triangles[5].setVertices((int)6, (int)7, (int)3);
		triangles[5].setNormals((int)2, (int)2, (int)2);

		triangles[6].setVertices((int)3, (int)7, (int)0);
		triangles[6].setNormals((int)3, (int)3, (int)3);

		triangles[7].setVertices((int)7, (int)4, (int)0);
		triangles[7].setNormals((int)3, (int)3, (int)3);

		triangles[8].setVertices((int)0, (int)1, (int)2);
		triangles[8].setNormals((int)4, (int)4, (int)4);

		triangles[9].setVertices((int)2, (int)3, (int)0);
		triangles[9].setNormals((int)4, (int)4, (int)4);

		triangles[10].setVertices((int)4, (int)6, (int)5);
		triangles[10].setNormals((int)5, (int)5, (int)5);

		triangles[11].setVertices((int)6, (int)4, (int)7);
		triangles[11].setNormals((int)5, (int)5, (int)5);

	p = new TriangleMeshShape(data);

	Transf3 t;
	p->transform( t.translation(c) );
	p->transform( t.rotationX(o.x) );
	p->transform( t.rotationX(o.y) );
	p->transform( t.rotationX(o.z) );
	p->transform( t.scale(s) );
}
