//[]------------------------------------------------------------------------[]
//|                                                                          |
//|                        GVSG Foundation Classes                           |
//|                               Version 1.0                                |
//|                                                                          |
//|                Copyright® 2007, Paulo Aristarco Pagliosa                 |
//|                All Rights Reserved.                                      |
//|                                                                          |
//[]------------------------------------------------------------------------[]
//
//  OVERVIEW: NameableObject.cpp
//  ========
//  Source file for nameable object.

#ifndef __NameableObject_h
#include "NameableObject.h"
#endif
#ifndef __ObjectStream_h
#include "ObjectStream.h"
#endif

using namespace System;


//////////////////////////////////////////////////////////
//
// NameableObject implementation
// ==============
IMPLEMENT_ABSTRACT_SERIALIZABLE(NameableObject);

void
NameableObject::Streamer::write(ObjectOutputStream& oos) const
//[]---------------------------------------------------[]
//|  Write                                              |
//[]---------------------------------------------------[]
{
	oos.writeWString(getObject()->getName());
}

Serializable*
NameableObject::Streamer::read(ObjectInputStream& ois) const
//[]---------------------------------------------------[]
//|  Read                                               |
//[]---------------------------------------------------[]
{
	getObject()->setName(ois.readWString());
	return getObject();
}
