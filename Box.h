#ifndef BOX_H__
#define BOX_H__

#ifndef __RAY_H//aqui esta a classe IntersectInfo
#include"Ray.h"
#endif

#ifndef __MODEL_H
#include "Model.h"
#endif

#ifndef __TriangleMeshShape_H
#include "TriangleMeshShape.h"
#endif


// class defintions
class Box : public TriangleMeshShape
{
public:
	Box(Vec3, Vec3, Vec3);
  	
}; //Box

#endif                                                                                                                                                                            
