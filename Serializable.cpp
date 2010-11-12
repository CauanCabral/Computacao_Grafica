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
//  OVERVIEW: Serializable.cpp
//  ========
//  Source file for serializable objetcs.

#ifndef __Serializable_h
#include "Serializable.h"
#endif

using namespace System;

//
// Auxiliary functions
//
uint
System::hashValue(const char* s)
{
	uint h = 0;
	uint g;

	for (const char* p = s; *p; p++)
	{
		h = (h << 4) + (*p);
		if ((g = h & 0xf0000000) != 0)
		{
			h ^= g >> 24;
			h ^= g;
		}
	}
	return h;
}

Streamer*
System::getStreamer(const char* clsName, Serializable* obj)
{
	SerializableClass* cls = SerializableClass::getClass(clsName);

	if (cls == 0)
		throw ClassNotFoundException(clsName);
	return cls->getStreamer(obj);
}


//////////////////////////////////////////////////////////
//
// SerializableClass implementation
// =================
SerializableClass::Classes* SerializableClass::classes;

void
SerializableClass::registerClass(const char* clsName, SerializableClass* cls)
//[]---------------------------------------------------[]
//|  Register class                                     |
//[]---------------------------------------------------[]
{
	initializeClasses();
	classes->put(ClassName(clsName), cls);
}
