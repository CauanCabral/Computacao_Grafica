#ifndef __File_h
#define __File_h

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
//  OVERVIEW: File.h
//  ========
//  Class definition for file.

#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#ifndef __LINUX
#include <io.h>
#define ftruncate _chsize
#else
#include <wchar.h>
#include <sys/types.h>
#include <unistd.h>
#define O_BINARY 0
#define _access  access
#define _open    open
#define _lseek   lseek
#define _read    read
#define _write   write
#define _close   close
long _filelength(int);
#endif

#ifndef __Typedefs_h
#include "Typedefs.h"
#endif


//////////////////////////////////////////////////////////
//
// File: file class
// ====
class File
{
public:
	enum
	{
		fileNull	= -1
	};
	enum
	{
		// Open mode
		readOnly = O_RDONLY,
		readWrite = O_RDWR,
		writeOnly = O_WRONLY,
		create = O_CREAT | O_TRUNC,
		createExcl = O_CREAT | O_EXCL,
		append = O_APPEND,
		binary = O_BINARY
	};
	enum
	{
		// Permission flags for create
		permread = S_IREAD, permwrite = S_IWRITE,
		permRdWr = S_IREAD | S_IWRITE
	};
	enum seek_dir
	{
		beg = SEEK_SET,
		cur = SEEK_CUR,
		end = SEEK_END
	};

	// Constructors
	File();
	File(const char*, uint = readWrite, uint = permRdWr);

	// Destructor
	~File();

	bool open(const char*, uint = readWrite, uint = permRdWr);
	bool close();

	const char* getName() const;

	long position() const;
	long length() const;
	void setLength(long);
	long seek(long, int = beg);
	long seekToBegin();
	long seekToEnd();
	bool isOpen() const;

	int read(void*, int);

	int read(uint32&);
	int read(int32&);
	int read(uint16&);
	int read(int&);
	int read(char&);
	int read(float&);
	int read(double&);
	int read(char*&);
	int read(wchar_t*&);
	int read(bool&);

	int write(const void*, int);

	int write(uint32);
	int write(int32);
	int write(uint16);
	int write(int);
	int write(char);
	int write(float);
	int write(double);
	int write(const char*);
	int write(const wchar_t*);
	int write(bool);

	static bool	exists(const char*);
	static void	remove(const char*);
	static void	rename(const char*, const char*);

private:
	char* name;
	int handle;

	int readArray(void*&, int);
	int writeArray(const void*, int, int);

}; // File


//////////////////////////////////////////////////////////
//
// File inline implementation
// ====
inline
File::File():
	name(0),
	handle(fileNull)
{
	// do nothing
}

inline
File::File(const char* name, uint access, uint permission):
	name(0),
	handle(fileNull)
{
	open(name, access, permission);
}

inline
File::~File()
{
	close();
}

inline const char*
File::getName() const
{
	return const_cast<char*>(name);
}

inline long
File::position() const
{
	return ::_lseek(handle, 0, cur);
}

inline bool
File::isOpen() const
{
	return handle > fileNull;
}

inline long
File::length() const
{
	return ::_filelength(handle);
}

inline void
File::setLength(long length)
{
	::ftruncate(handle, length);
}

inline long
File::seek(long offset, int origin)
{
	return ::_lseek(handle, offset, origin);
}

inline long
File::seekToBegin()
{
	return ::_lseek(handle, 0, beg);
}

inline long
File::seekToEnd()
{
	return ::_lseek(handle, 0, end);
}

inline int
File::read(void* buffer, int size)
{
	return ::_read(handle, buffer, size);
}

inline int
File::read(uint32& value)
{
	return read(&value, sizeof(uint32));
}

inline int
File::read(uint16& value)
{
	return read(&value, sizeof(uint16));
}

inline int
File::read(int32& value)
{
	return read(&value, sizeof(int32));
}

inline int
File::read(int& value)
{
	return read(&value, sizeof(int));
}

inline int
File::read(char& value)
{
	return read(&value, sizeof(char));
}

inline int
File::read(float& value)
{
	return read(&value, sizeof(float));
}

inline int
File::read(double& value)
{
	return read(&value, sizeof(double));
}

inline int
File::read(char*& value)
{
	return readArray((void*&)value, sizeof(*value));
}

inline int
File::read(wchar_t*& value)
{
	return readArray((void*&)value, sizeof(*value));
}

inline int
File::read(bool& value)
{
	return read(&value, sizeof(bool));
}

inline int
File::write(const void* buffer, int size)
{
	return ::_write(handle, buffer, size);
}

inline int
File::write(uint32 value)
{
	return write(&value, sizeof(uint32));
}

inline int
File::write(int32 value)
{
	return write(&value, sizeof(int32));
}

inline int
File::write(uint16 value)
{
	return write(&value, sizeof(uint16));
}

inline int
File::write(int value)
{
	return write(&value, sizeof(int));
}

inline int
File::write(char value)
{
	return write(&value, sizeof(char));
}

inline int
File::write(float value)
{
	return write(&value, sizeof(float));
}

inline int
File::write(double value)
{
	return write(&value, sizeof(double));
}

inline int
File::write(const char* value)
{
	int length = value != 0 ? (int)strlen(value) + 1 : 0;
	return writeArray(value, length, sizeof(*value));
}

inline int
File::write(const wchar_t* value)
{
	int length = value != 0 ? (int)wcslen(value) + 1 : 0;
	return writeArray(value, length, sizeof(*value));
}

inline int
File::write(bool value)
{
	return write(&value, sizeof(bool));
}

inline bool
File::exists(const char* name)
{
	return ::_access(name, 0) == 0;
}

inline void
File::remove(const char* name)
{
	::remove(name);
}

inline void
File::rename(const char* oldName, const char* newName)
{
	::rename(oldName, newName);
}

#endif // __File_h
