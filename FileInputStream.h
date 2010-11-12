#ifndef __FileInputStream_h
#define __FileInputStream_h

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
//  OVERVIEW: FileInputStream.h
//  ========
//  Class definition for file input stream.

#ifndef __File_h
#include "File.h"
#endif
#ifndef __InputStream_h
#include "InputStream.h"
#endif

namespace System
{ // begin namespace System


//////////////////////////////////////////////////////////
//
// FileInputStream: file input stream
// ===============
class FileInputStream: public InputStream
{
public:
	// Constructor
	FileInputStream(const char*);

	// Destructor
	~FileInputStream();

	long available() const;
	void close();
	long read(void*, long);
	long skip(long);

private:
	File file;

}; // FileInputStream

} // end namespace System

#endif // __FileInputStream_h
