#ifndef __BufferedStream_h
#define __BufferedStream_h

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
//  OVERVIEW: BufferedStream.h
//  ========
//  Class definition for buffered streams.

#ifndef __Exception_h
#include "Exception.h"
#endif
#ifndef __Typedefs_h
#include "Typedefs.h"
#endif

namespace System
{ // begin namespace System

#define STR_BUFFER_SIZE 1024


//////////////////////////////////////////////////////////
//
// BufferedStream: generic buffered stream
// ==============
class BufferedStream
{
public:
	// Constructor
	BufferedStream(int size = STR_BUFFER_SIZE)
	{
		this->size = size;
		count = 0;
		buffer = new char[size];
	}

	// Destructor
	virtual ~BufferedStream();

	virtual void close();

protected:
	char* buffer;
	int size;
	int count;

}; // BufferedStream

} // end namespace System

#endif // __BufferedStream_h
