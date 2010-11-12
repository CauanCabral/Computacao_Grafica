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
//  OVERVIEW: OutputStream.cpp
//  ========
//  Source file for output streams.

#include <string.h>

#ifndef __OutputStream_h
#include "OutputStream.h"
#endif

using namespace System;


//////////////////////////////////////////////////////////
//
// OutputStream implementation
// ============
OutputStream::~OutputStream()
//[]---------------------------------------------------[]
//|  Destructor                                         |
//[]---------------------------------------------------[]
{
	// do nothing
}


//////////////////////////////////////////////////////////
//
// FilterOutputStream implementation
// ==================
FilterOutputStream::~FilterOutputStream()
//[]---------------------------------------------------[]
//|  Destructor                                         |
//[]---------------------------------------------------[]
{
	close();
}

void
FilterOutputStream::close()
//[]---------------------------------------------------[]
//|  Close                                              |
//[]---------------------------------------------------[]
{
	if (out == 0)
		return;
	flush();
	out->close();
	delete out;
	out = 0;
}

void
FilterOutputStream::flush()
//[]---------------------------------------------------[]
//|  Flush                                              |
//[]---------------------------------------------------[]
{
	ensureOpen();
	out->flush();
}

void
FilterOutputStream::write(const void* buf, long len)
//[]---------------------------------------------------[]
//|  Write                                              |
//[]---------------------------------------------------[]
{
	ensureOpen();
	if (len <= 0)
		return;
	out->write(buf, len);
}


//////////////////////////////////////////////////////////
//
// BufferedOutputStream implementation
// ====================
void
BufferedOutputStream::close()
//[]---------------------------------------------------[]
//|  Close                                              |
//[]---------------------------------------------------[]
{
	FilterOutputStream::close();
	BufferedStream::close();
}

void
BufferedOutputStream::flush()
//[]---------------------------------------------------[]
//|  Flush                                              |
//[]---------------------------------------------------[]
{
	ensureOpen();
	flushBuffer();
	out->flush();
}

void
BufferedOutputStream::write(const void* buf, long len)
//[]---------------------------------------------------[]
//|  Write                                              |
//[]---------------------------------------------------[]
{
	ensureOpen();
	if (len <= 0)
		return;
	if (len >= long(size))
	{
		flushBuffer();
		out->write(buf, len);
		return;
	}
	if (len > long(size - count))
		flushBuffer();
	memmove(buffer + count, buf, len);
	count += len;
}

void
BufferedOutputStream::flushBuffer()
//[]---------------------------------------------------[]
//|  Flush buffer                                       |
//[]---------------------------------------------------[]
{
	if (count > 0)
	{
		out->write(buffer, count);
		count = 0;
	}
}


//////////////////////////////////////////////////////////
//
// DataOutputStream implementation
// ================
void
DataOutputStream::writeWString(const wchar_t* s)
//[]---------------------------------------------------[]
//|  Write wide string                                  |
//[]---------------------------------------------------[]
{
	if (s == 0)
		writeInt32(-1);
	else
	{
		int32 len = (int)wcslen(s);

		writeInt32(len);
		if (len > 0)
			write(s, len * sizeof(wchar_t));
	}
}

void
DataOutputStream::writeString(const char* s)
//[]---------------------------------------------------[]
//|  Write string                                       |
//[]---------------------------------------------------[]
{
	if (s == 0)
		writeInt32(-1);
	else
	{
		int32 len = (int)strlen(s);

		writeInt32(len);
		if (len > 0)
			write(s, len);
	}
}
