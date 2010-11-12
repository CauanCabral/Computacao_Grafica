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
//  OVERVIEW: FileOutputStream.cpp
//  ========
//  Source code for file output stream.

#ifndef __FileOutputStream_h
#include "FileOutputStream.h"
#endif

using namespace System;


//////////////////////////////////////////////////////////
//
// FileOutputStream implementation
// ================
FileOutputStream::FileOutputStream(const char* fileName):
	file(fileName, File::readWrite | File::create | File::binary)
//[]---------------------------------------------------[]
//|  Constructor                                        |
//[]---------------------------------------------------[]
{
	// do nothing
}

FileOutputStream::~FileOutputStream()
//[]---------------------------------------------------[]
//|  Destructor                                         |
//[]---------------------------------------------------[]
{
	close();
}

void
FileOutputStream::close()
//[]---------------------------------------------------[]
//|  Close                                              |
//[]---------------------------------------------------[]
{
	file.close();
}

void
FileOutputStream::flush()
//[]---------------------------------------------------[]
//|  Flush                                              |
//[]---------------------------------------------------[]
{
	// do nothing
}

void
FileOutputStream::write(const void* buf, long len)
//[]---------------------------------------------------[]
//|  Write                                              |
//[]---------------------------------------------------[]
{
	file.write(buf, len);
}
