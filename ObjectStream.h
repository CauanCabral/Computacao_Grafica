#ifndef __ObjectStream_h
#define __ObjectStream_h

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
//  OVERVIEW: ObjectStream.h
//  ========
//  Class definition for object streams.

#ifndef __ObjectInputStream_h
#include "ObjectInputStream.h"
#endif
#ifndef __ObjectOutputStream_h
#include "ObjectOutputStream.h"
#endif

namespace System
{ // begin namespace System


//////////////////////////////////////////////////////////
//
// ObjectIOStream: object IO stream class
// ==============
class ObjectIOStream: public ObjectInputStream, public ObjectOutputStream
{
public:
	// Constructor
	ObjectIOStream(InputStream* is, OutputStream* os):
		ObjectInputStream(is), ObjectOutputStream(os)
	{
		// do nothing
	}

}; // ObjectIOStream

} // end namespace System

#endif // __ObjectStream_h
