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
//  OVERVIEW: SceneComponent.cpp
//  ========
//  Source file for scene component.

#ifndef __SceneComponent_h
#include "SceneComponent.h"
#endif
#ifndef __ObjectStream_h
#include "ObjectStream.h"
#endif

using namespace Graphics;


//////////////////////////////////////////////////////////
//
// SceneComponent implementation
// ==============
IMPLEMENT_ABSTRACT_SERIALIZABLE1(SceneComponent, NameableObject);

void
SceneComponent::Streamer::write(ObjectOutputStream& oos) const
//[]----------------------------------------------------[]
//|  Write                                               |
//[]----------------------------------------------------[]
{
	writeBaseObject((NameableObject*)getObject(), oos);
}

Serializable*
SceneComponent::Streamer::read(ObjectInputStream& ois) const
//[]----------------------------------------------------[]
//|  Read                                                |
//[]----------------------------------------------------[]
{
	readBaseObject((NameableObject*)getObject(), ois);
	return getObject();
}

SceneComponent::~SceneComponent()
//[]----------------------------------------------------[]
//|  Destructor                                          |
//[]----------------------------------------------------[]
{
	// do nothing
}
