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
//  OVERVIEW: ObjectInputStream.cpp
//  ========
//  Source file for object input streams.

#ifndef __ObjectInputStream_h
#include "ObjectInputStream.h"
#endif

using namespace System;


//////////////////////////////////////////////////////////
//
// ObjectInputStream implementation
// =================
ObjectInputStream::ObjectInputStream(InputStream* in):
	DataInputStream(/*new BufferedInputStream*/(in))
//[]---------------------------------------------------[]
//|  Constructor                                        |
//[]---------------------------------------------------[]
{
	// do nothing
}

Serializable*
ObjectInputStream::readObject(Serializable& obj)
//[]---------------------------------------------------[]
//|  Read object (ref)                                  |
//[]---------------------------------------------------[]
{
	Serializable* temp = &obj;

	readFields(readPrefix(), temp);
	readSuffix();
	return &obj;
}

Serializable*
ObjectInputStream::readObject()
//[]---------------------------------------------------[]
//|  Read object (prt)                                  |
//[]---------------------------------------------------[]
{
	Serializable* obj = 0;

	switch (readInt8())
	{
		case ObjectStream::ptIndexed:
			obj = findObject(readInt32());
			break;

		case ObjectStream::ptObject:
		{
			readFields(readPrefix(), obj);
			readSuffix();
			break;
		}

		case ObjectStream::ptNull:
			break;

		default:
			throw IOException(L"Error: object pointer reading");
	}
	return obj;
}

SerializableClass*
ObjectInputStream::readPrefix()
//[]---------------------------------------------------[]
//|  Read prefix                                        |
//[]---------------------------------------------------[]
{
	if (readInt8() != '[')
		throw IOException();

	SerializableClass* cls = 0;

	switch (readInt8())
	{
		case ObjectStream::ntIndexed:
			cls = findClass(readInt32());
			break;

		case ObjectStream::ntClass:
		{
			char* clsName = readString();

			registerClass(clsName);
			if ((cls = SerializableClass::getClass(clsName)) == 0)
				throw ClassNotFoundException(clsName);
			break;
		}

		default:
			throw IOException(L"Error: object prefix reading");
	}
	return cls;
}

void
ObjectInputStream::readFields(SerializableClass* cls, Serializable*& obj)
//[]---------------------------------------------------[]
//|  Read fields                                        |
//[]---------------------------------------------------[]
{
	Streamer* strmr = cls->getStreamer(obj);

	obj = strmr->getObject();
	registerObject(obj);
	strmr->read(*this);
	delete strmr;
}

void
ObjectInputStream::readSuffix()
//[]---------------------------------------------------[]
//|  Read suffix                                        |
//[]---------------------------------------------------[]
{
	if (readInt8() != ']')
		throw IOException(L"Error: object suffix reading");
}

Serializable*
ObjectInputStream::readVirtualBaseObject(const Streamer& strmr)
//[]---------------------------------------------------[]
//|  Read virtual base object                           |
//[]---------------------------------------------------[]
{
	switch (readInt8())
	{
		case ObjectStream::ptIndexed:
			break;

		case ObjectStream::ptObject:
		{
			registerObject(strmr.getObject());
			strmr.read(*this);
			break;
		}

		default:
			throw IOException(L"Error: virtual base reading");
	}
	return strmr.getObject();
}

Serializable*
ObjectInputStream::readBaseObject(const Streamer& strmr)
//[]---------------------------------------------------[]
//|  Read base object                                   |
//[]---------------------------------------------------[]
{
	strmr.read(*this);
	return strmr.getObject();
}
