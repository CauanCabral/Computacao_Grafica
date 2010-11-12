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
//  OVERVIEW: Actor.cpp
//  ========
//  Source file for actor.

#ifndef __Actor_h
#include "Actor.h"
#endif

using namespace Graphics;


//////////////////////////////////////////////////////////
//
// Actor implementation
// =====
IMPLEMENT_SERIALIZABLE1(Actor, SceneComponent);

void
Actor::Streamer::write(ObjectOutputStream& oos) const
//[]---------------------------------------------------[]
//|  Write                                              |
//[]---------------------------------------------------[]
{
	writeBaseObject((SceneComponent*)getObject(), oos);
	oos << getObject()->isVisible;
	oos << getObject()->model;
}

Serializable*
Actor::Streamer::read(ObjectInputStream& ois) const
//[]---------------------------------------------------[]
//|  Read                                               |
//[]---------------------------------------------------[]
{
	readBaseObject((SceneComponent*)getObject(), ois);
	ois >> getObject()->isVisible;

	Model* model;

	ois >> model;
	getObject()->model = model->makeUse();
	return getObject();
}

Actor::~Actor()
//[]----------------------------------------------------[]
//|  Destructor                                          |
//[]----------------------------------------------------[]
{
	model->release();
}
