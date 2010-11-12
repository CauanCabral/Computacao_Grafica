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
//  OVERVIEW: InputStream.cpp
//  ========
//  Source file for input streams.

#include <memory.h>

#ifndef __InputStream_h
#include "InputStream.h"
#endif

using namespace System;


//////////////////////////////////////////////////////////
//
// InputStream implementation
// ============
InputStream::~InputStream()
//[]---------------------------------------------------[]
//|  Destructor                                         |
//[]---------------------------------------------------[]
{
	// do nothing
}


//////////////////////////////////////////////////////////
//
// FilterInputStream implementation
// =================
FilterInputStream::~FilterInputStream()
//[]---------------------------------------------------[]
//|  Destructor                                         |
//[]---------------------------------------------------[]
{
	close();
}

long
FilterInputStream::available() const
//[]---------------------------------------------------[]
//|  Available                                          |
//[]---------------------------------------------------[]
{
	ensureOpen();
	return in->available();
}

void
FilterInputStream::close()
//[]---------------------------------------------------[]
//|  Close                                              |
//[]---------------------------------------------------[]
{
	if (in == 0)
		return;
	in->close();
	delete in;
	in = 0;
}

long
FilterInputStream::read(void* buf, long len)
//[]---------------------------------------------------[]
//|  Read                                               |
//[]---------------------------------------------------[]
{
	ensureOpen();
	return in->read(buf, len);
}

long
FilterInputStream::skip(long len)
//[]---------------------------------------------------[]
//|  Skip                                               |
//[]---------------------------------------------------[]
{
	ensureOpen();
	return in->skip(len);
}


//////////////////////////////////////////////////////////
//
// BufferedInputStream implementation
// ===================
long
BufferedInputStream::available() const
//[]---------------------------------------------------[]
//|  Available                                          |
//[]---------------------------------------------------[]
{
	ensureOpen();
	return count - position + in->available();
}

void
BufferedInputStream::close()
//[]---------------------------------------------------[]
//|  Close                                              |
//[]---------------------------------------------------[]
{
	FilterInputStream::close();
	BufferedStream::close();
	position = 0;
}

long
BufferedInputStream::read(void* buf, long len)
//[]---------------------------------------------------[]
//|  Read                                               |
//[]---------------------------------------------------[]
{
	ensureOpen();
	if (len <= 0)
		return 0;

	long count = readBytes(buf, len);

	if (count > 0)
		while (count < len)// && in->available() > 0)
		{
			long n = readBytes((char*)buf + count, len - count);

			if (n <= 0)
				break;
			count += n;
		}
	return count;
}

long
BufferedInputStream::skip(long len)
//[]---------------------------------------------------[]
//|  Skip                                               |
//[]---------------------------------------------------[]
{
	ensureOpen();
	if (len <= 0)
		return 0;

	long avail = count - position;

	if (avail <= 0)
		return in->skip(len);
	if (avail < len)
		len = avail;
	position += len;
	return len;
}

long
BufferedInputStream::fillBuffer()
//[]---------------------------------------------------[]
//|  Fill buffer                                        |
//[]---------------------------------------------------[]
{
	position = 0;
	return count = in->read(buffer, size);
}


long
BufferedInputStream::readBytes(void* buf, long len)
//[]---------------------------------------------------[]
//|  Read bytes                                         |
//[]---------------------------------------------------[]
{
	long avail = count - position;

	if (avail <= 0)
	{
		if (len >= long(size))
			return in->read(buf, len);
		if ((avail = fillBuffer()) <= 0)
			return -1;
	}
	if (avail < len)
		len = avail;
	memmove(buf, buffer + position, len);
	position += len;
	return len;
}


//////////////////////////////////////////////////////////
//
// DataInputStream implementation
// ===============
wchar_t*
DataInputStream::readWString()
//[]---------------------------------------------------[]
//|  Read wide string                                   |
//[]---------------------------------------------------[]
{
	int32 len = readInt32();

	if (len == -1)
		return 0;

	wchar_t* s = new wchar_t[len + 1];

	read(s, len * sizeof(wchar_t));
	s[len] = 0;
	return s;
}

char*
DataInputStream::readString()
//[]---------------------------------------------------[]
//|  Read string                                        |
//[]---------------------------------------------------[]
{
	int32 len = readInt32();

	if (len == -1)
		return 0;

	char* s = new char[len + 1];

	read(s, len);
	s[len] = 0;
	return s;
}
