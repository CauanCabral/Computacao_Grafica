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
//  OVERVIEW: ObjectOutputStream.cpp
//  ========
//  Source file for object output streams.

#ifndef __ObjectOutputStream_h
#include "ObjectOutputStream.h"
#endif

using namespace System;


//////////////////////////////////////////////////////////
//
// ObjectOutputStream implementation
// ==================
ObjectOutputStream::ObjectOutputStream(OutputStream* out):
	DataOutputStream(/*new BufferedOutputStream*/(out))
//[]---------------------------------------------------[]
//|  Constructor                                        |
//[]---------------------------------------------------[]
{
	// do nothing
}

void
ObjectOutputStream::writeObject(const Serializable& obj)
//[]---------------------------------------------------[]
//|  Write object (ref)                                 |
//[]---------------------------------------------------[]
{
	writePrefix(const_cast<Serializable*>(&obj));
	writeFields(const_cast<Serializable*>(&obj));
	writeSuffix(const_cast<Serializable*>(&obj));
}

void
ObjectOutputStream::writeObject(const Serializable* obj)
//[]---------------------------------------------------[]
//|  Write object (prt)                                 |
//[]---------------------------------------------------[]
{
	if (obj == 0)
		writeInt8(ObjectStream::ptNull);
	else
	{
		int32 index = findObject(const_cast<Serializable*>(obj));

		if (index != 0)
		{
			writeInt8(ObjectStream::ptIndexed);
			writeInt32(index);
		}
		else
		{
			writeInt8(ObjectStream::ptObject);
			writeObject(*obj);
		}
	}
}

void
ObjectOutputStream::writePrefix(Serializable* obj)
//[]---------------------------------------------------[]
//|  Write prefix                                       |
//[]---------------------------------------------------[]
{
	writeInt8('[');

	int index = findClass(obj);

	if (index == 0)
	{
		registerClass(obj);
		writeInt8(ObjectStream::ntClass);
		writeString(obj->getClassName());
	}
	else
	{
		writeInt8(ObjectStream::ntIndexed);
		writeInt32(index);
	}
}

void
ObjectOutputStream::writeFields(Serializable* obj)
//[]---------------------------------------------------[]
//|  Write fields                                       |
//[]---------------------------------------------------[]
{
	Streamer* strmr = getStreamer(*obj);

	registerObject(obj);
	strmr->write(*this);
	delete strmr;
}

void
ObjectOutputStream::writeVirtualBaseObject(const Streamer& strmr)
//[]---------------------------------------------------[]
//|  Write virtual base object                          |
//[]---------------------------------------------------[]
{
	Serializable* obj = strmr.getObject();

	if (findVB(obj) != 0)
		writeInt8(ObjectStream::ptIndexed);
	else
	{
		registerObject((Serializable*)((char*)obj + 1));
		writeInt8(ObjectStream::ptObject);
		strmr.write(*this);
	}
}

void
ObjectOutputStream::writeBaseObject(const Streamer& strmr)
//[]---------------------------------------------------[]
//|  Write base object                                  |
//[]---------------------------------------------------[]
{
	strmr.write(*this);
}
