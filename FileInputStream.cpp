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
//  OVERVIEW: FileInputStream.cpp
//  ========
//  Source code for file input stream.

#ifndef __FileInputStream_h
#include "FileInputStream.h"
#endif

using namespace System;


//////////////////////////////////////////////////////////
//
// FileInputStream implementation
// ===============
FileInputStream::FileInputStream(const char* fileName):
	file(fileName, File::readOnly | File::binary)
//[]---------------------------------------------------[]
//|  Constructor                                        |
//[]---------------------------------------------------[]
{
	// do nothing
}

FileInputStream::~FileInputStream()
//[]---------------------------------------------------[]
//|  Destructor                                         |
//[]---------------------------------------------------[]
{
	close();
}

long
FileInputStream::available() const
//[]---------------------------------------------------[]
//|  Available                                          |
//[]---------------------------------------------------[]
{
	return 0;//file.length() - file.position();
}

void
FileInputStream::close()
//[]---------------------------------------------------[]
//|  Close                                              |
//[]---------------------------------------------------[]
{
	file.close();
}

long
FileInputStream::read(void* buf, long len)
//[]---------------------------------------------------[]
//|  Read                                               |
//[]---------------------------------------------------[]
{
	return file.read(buf, len);
}

long
FileInputStream::skip(long len)
//[]---------------------------------------------------[]
//|  Skip                                               |
//[]---------------------------------------------------[]
{
	return file.seek(len, File::cur);
}
