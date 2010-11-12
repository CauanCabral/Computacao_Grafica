#ifndef __FileOutputStream_h
#define __FileOutputStream_h

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
//  OVERVIEW: FileOutputStream.h
//  ========
//  Class definition for file output stream.

#ifndef __File_h
#include "File.h"
#endif
#ifndef __OutputStream_h
#include "OutputStream.h"
#endif

namespace System
{ // begin namespace System


//////////////////////////////////////////////////////////
//
// FileOutputStream: file output stream
// ================
class FileOutputStream: public OutputStream
{
public:
	// Constructor
	FileOutputStream(const char*);

	// Destructor
	~FileOutputStream();

	void close();
	void flush();
	void write(const void*, long);

private:
	File file;

}; // FileOutputStream

} // end namespace System

#endif // __FileOutputStream
