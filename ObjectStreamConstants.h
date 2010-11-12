#ifndef __ObjectStreamConstants_h
#define __ObjectStreamConstants_h

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
//  OVERVIEW: ObjectStreamConstants.h
//  ========
//  Constants for object stream.

namespace System
{ // begin namespace System


//////////////////////////////////////////////////////////
//
// ObjectStream: object stream constants
// ============
class ObjectStream
{
public:
	enum PointerType
	{
		ptNull,
		ptIndexed,
		ptObject
	};

	enum
	{
		ntIndexed,
		ntClass
	};

}; // ObjectStream

} // end namespace System

#endif // __ObjectStreamConstants_h
