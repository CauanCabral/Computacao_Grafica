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
// OVERVIEW: Model.cpp
// ========
// Source file for generic model.

#ifndef __Model_h
#include "Model.h"
#endif

using namespace Graphics;


//////////////////////////////////////////////////////////
//
// Model implementation
// =====
IMPLEMENT_ABSTRACT_SERIALIZABLE1(Model, Object);

void
Model::Streamer::write(ObjectOutputStream& oos) const
//[]----------------------------------------------------[]
//|  Write                                               |
//[]----------------------------------------------------[]
{
	writeBaseObject((Object*)getObject(), oos);
}

Serializable*
Model::Streamer::read(ObjectInputStream& ois) const
//[]----------------------------------------------------[]
//|  Read                                                |
//[]----------------------------------------------------[]
{
	readBaseObject((Object*)getObject(), ois);
	return getObject();
}

Model::~Model()
//[]----------------------------------------------------[]
//|  Destructor                                          |
//[]----------------------------------------------------[]
{
	// do nothing
}

TriangleMesh*
Model::getMesh()
//[]----------------------------------------------------[]
//|  Get mesh                                            |
//[]----------------------------------------------------[]
{
	return 0;
}


//////////////////////////////////////////////////////////
//
// Primitive implementation
// =========
IMPLEMENT_ABSTRACT_SERIALIZABLE1(Primitive, Model);

void
Primitive::Streamer::write(ObjectOutputStream& oos) const
//[]----------------------------------------------------[]
//|  Write                                               |
//[]----------------------------------------------------[]
{
	writeBaseObject((Model*)getObject(), oos);
	oos << getObject()->material;
}

Serializable*
Primitive::Streamer::read(ObjectInputStream& ois) const
//[]----------------------------------------------------[]
//|  Read                                                |
//[]----------------------------------------------------[]
{
	readBaseObject((Model*)getObject(), ois);
	ois >> getObject()->material;
	return getObject();
}

Material*
Primitive::getMaterial() const
//[]----------------------------------------------------[]
//|  Get material                                        |
//[]----------------------------------------------------[]
{
	return material;
}

void
Primitive::setMaterial(Material* material)
//[]----------------------------------------------------[]
//|  Set material                                        |
//[]----------------------------------------------------[]
{
	this->material = material;
}
