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
//  OVERVIEW: File.cpp
//  ========
//  Source code for file.

#include <string.h>

#ifndef __File_h
#include "File.h"
#endif

#ifdef __LINUX
long
_filelength(int filedes)
{
	struct stat statbuffer;

	fstat(filedes, &statbuffer);
	return statbuffer.st_size;
}
#endif

//
// Auxiliary function
//
char*
strnewdup(const char* s)
{
	return s ? strcpy(new char[strlen(s) + 1], s) : 0;
}


//////////////////////////////////////////////////////////
//
// File implementation
// ====
bool
File::open(const char* name, uint access, uint permission)
//[]----------------------------------------------------[]
//|  Open file                                           |
//[]----------------------------------------------------[]
{
	if (isOpen() || name == 0)
		return false;
	this->name = strnewdup(name);
	handle = ::_open(name, access, permission);
	return isOpen();
}

bool
File::close()
//[]----------------------------------------------------[]
//|  Close file                                          |
//[]----------------------------------------------------[]
{
	if (isOpen())
	{
		if (::_close(handle) != 0)
			return false;
		delete []name;
		name = 0;
		handle = fileNull;
	}
	return true;
}

int
File::readArray(void*& buffer, int elementSize)
//[]----------------------------------------------------[]
//|  Read array                                          |
//[]----------------------------------------------------[]
{
	int32 length = 0;
	int size;

	size = read(length);
	if (length == 0)
		buffer = 0;
	else
	{
		buffer = new char[length *= elementSize];
		size += read(buffer, length);
	}
	return size;
}

int
File::writeArray(const void* value, int length, int elementSize)
//[]----------------------------------------------------[]
//|  Write array                                         |
//[]----------------------------------------------------[]
{
	int size = write(length);

	if (length != 0)
		size += write(value, length * elementSize);
	return size;
}
