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
//  OVERVIEW: Object.cpp
//  ========
//  Source file for generic object.

#ifndef __Object_h
#include "Object.h"
#endif

using namespace System;


//////////////////////////////////////////////////////////
//
// Object implementation
// ======
IMPLEMENT_SERIALIZABLE(Object);

void
Object::Streamer::write(ObjectOutputStream&) const
//[]---------------------------------------------------[]
//|  Write                                              |
//[]---------------------------------------------------[]
{
	// do nothing
}

Serializable*
Object::Streamer::read(ObjectInputStream&) const
//[]---------------------------------------------------[]
//|  Read                                               |
//[]---------------------------------------------------[]
{
	getObject()->count = 0;
	return getObject();
}

Object::~Object()
//[]---------------------------------------------------[]
//|  Destructor                                         |
//[]---------------------------------------------------[]
{
	// do nothing
}

Object*
Object::makeCopy() const
//[]---------------------------------------------------[]
//| Make a deep-copy of this object                     |
//[]---------------------------------------------------[]
{
	return 0;
}
