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
//  OVERVIEW: BufferedStream.cpp
//  ========
//  Source file for buffered streams.

#ifndef __BufferedStream_h
#include "BufferedStream.h"
#endif

using namespace System;


//////////////////////////////////////////////////////////
//
// BufferedStream implementation
// ==============
BufferedStream::~BufferedStream()
//[]---------------------------------------------------[]
//|  Destructor                                         |
//[]---------------------------------------------------[]
{
	close();
}

void
BufferedStream::close()
//[]---------------------------------------------------[]
//|  Close                                              |
//[]---------------------------------------------------[]
{
	if (buffer != 0)
	{
		delete []buffer;
		buffer = 0;
		size = count = 0;
	}
}
