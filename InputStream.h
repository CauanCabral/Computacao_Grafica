#ifndef __InputStream_h
#define __InputStream_h

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
//  OVERVIEW: InputStream.h
//  ========
//  Class definition for input streams.

#ifndef __BufferedStream_h
#include "BufferedStream.h"
#endif

namespace System
{ // begin namespace System


//////////////////////////////////////////////////////////
//
// InputStream: generic input stream
// ===========
class InputStream
{
public:
	// Destructor
	virtual ~InputStream();

	virtual long available() const = 0;
	virtual void close() = 0;
	virtual long read(void*, long) = 0;
	virtual long skip(long) = 0;

}; // InputStream


//////////////////////////////////////////////////////////
//
// FilterInputStream: generic filter input stream
// =================
class FilterInputStream: public InputStream
{
public:
	// Constructor
	FilterInputStream(InputStream*);

	// Destructor
	~FilterInputStream();

	long available() const;
	void close();
	long read(void*, long);
	long skip(long);

protected:
	InputStream* in;

	void ensureOpen() const;

}; // FilterInputStream


//////////////////////////////////////////////////////////
//
// BufferedInputStream: generic buffered input stream
// ===================
class BufferedInputStream: public FilterInputStream, public BufferedStream
{
public:
	// Constructor
	BufferedInputStream(InputStream*, uint = STR_BUFFER_SIZE);

	long available() const;
	void close();
	long read(void*, long);
	long skip(long);

protected:
	uint position;

	long fillBuffer();
	long readBytes(void*, long);

}; // BufferedInputStream


//////////////////////////////////////////////////////////
//
// DataInputStream: data input stream
// ===============
class DataInputStream: public FilterInputStream
{
public:
	// Constructor
	DataInputStream(InputStream*);

	uint8 readUInt8();
	int8 readInt8();
	uint16 readUInt16();
	int16 readInt16();
	uint32 readUInt32();
	int32 readInt32();
	wchar_t readWChar();
	char readChar();
	short readShort();
	int readInt();
	long readLong();
	float readFloat();
	double readDouble();
	bool readBool();
	wchar_t* readWString();
	char* readString();

	DataInputStream& operator >>(wchar_t&);
	DataInputStream& operator >>(char&);
	DataInputStream& operator >>(unsigned char&);
	DataInputStream& operator >>(short&);
	DataInputStream& operator >>(unsigned short&);
	DataInputStream& operator >>(int&);
	DataInputStream& operator >>(unsigned int&);
	DataInputStream& operator >>(long&);
	DataInputStream& operator >>(unsigned long&);
	DataInputStream& operator >>(float&);
	DataInputStream& operator >>(double&);
	DataInputStream& operator >>(bool&);
	DataInputStream& operator >>(wchar_t*&);
	DataInputStream& operator >>(char*&);

}; // DataInputStream


//////////////////////////////////////////////////////////
//
// FilterInputStream inline implementation
// =================
inline
FilterInputStream::FilterInputStream(InputStream* in)
{
	this->in = in;
}

inline void
FilterInputStream::ensureOpen() const
{
	if (in == 0)
		throw IOException(L"Input stream closed");
}


//////////////////////////////////////////////////////////
//
// BufferedInputStream inline implementation
// ===================
inline
BufferedInputStream::BufferedInputStream(InputStream* in, uint size):
	FilterInputStream(in),
	BufferedStream(size),
	position(0)
{
	// do nothing
}


//////////////////////////////////////////////////////////
//
// DataInputStream inline implementation
// ===============
inline
DataInputStream::DataInputStream(InputStream* in):
	FilterInputStream(in)
{
}

inline uint8
DataInputStream::readUInt8()
{
	uint8 i;

	read(&i, sizeof(uint8));
	return i;
}

inline int8
DataInputStream::readInt8()
{
	int8 i;

	read(&i, sizeof(int8));
	return i;
}

inline uint16
DataInputStream::readUInt16()
{
	uint16 i;

	read(&i, sizeof(uint16));
	return i;
}

inline int16
DataInputStream::readInt16()
{
	int16 i;

	read(&i, sizeof(int16));
	return i;
}

inline uint32
DataInputStream::readUInt32()
{
	uint32 i;

	read(&i, sizeof(uint32));
	return i;
}

inline int32
DataInputStream::readInt32()
{
	int32 i;

	read(&i, sizeof(int32));
	return i;
}

inline wchar_t
DataInputStream::readWChar()
{
	wchar_t c;

	read(&c, sizeof(wchar_t));
	return c;
}

inline char
DataInputStream::readChar()
{
	char c;

	read(&c, sizeof(char));
	return c;
}

inline short
DataInputStream::readShort()
{
	short s;

	read(&s, sizeof(short));
	return s;
}

inline int
DataInputStream::readInt()
{
	int i;

	read(&i, sizeof(int));
	return i;
}

inline long
DataInputStream::readLong()
{
	long l;

	read(&l, sizeof(long));
	return l;
}

inline float
DataInputStream::readFloat()
{
	float f;

	read(&f, sizeof(float));
	return f;
}

inline double
DataInputStream::readDouble()
{
	double d;

	read(&d, sizeof(double));
	return d;
}

inline bool
DataInputStream::readBool()
{
	bool b;

	read(&b, sizeof(bool));
	return b;
}

inline DataInputStream&
DataInputStream::operator >>(wchar_t& c)
{
	read(&c, sizeof(wchar_t));
	return *this;
}

inline DataInputStream&
DataInputStream::operator >>(char& c)
{
	read(&c, sizeof(char));
	return *this;
}

inline DataInputStream&
DataInputStream::operator >>(unsigned char& c)
{
	read(&c, sizeof(unsigned char));
	return *this;
}

inline DataInputStream&
DataInputStream::operator >>(short& s)
{
	read(&s, sizeof(short));
	return *this;
}

inline DataInputStream&
DataInputStream::operator >>(unsigned short& s)
{
	read(&s, sizeof(unsigned short));
	return *this;
}

inline DataInputStream&
DataInputStream::operator >>(int& i)
{
	read(&i, sizeof(int));
	return *this;
}

inline DataInputStream&
DataInputStream::operator >>(unsigned int& i)
{
	read(&i, sizeof(unsigned int));
	return *this;
}

inline DataInputStream&
DataInputStream::operator >>(long& l)
{
	read(&l, sizeof(long));
	return *this;
}

inline DataInputStream&
DataInputStream::operator >>(unsigned long& l)
{
	read(&l, sizeof(unsigned long));
	return *this;
}

inline DataInputStream&
DataInputStream::operator >>(float& f)
{
	read(&f, sizeof(float));
	return *this;
}

inline DataInputStream&
DataInputStream::operator >>(double& d)
{
	read(&d, sizeof(double));
	return *this;
}

inline DataInputStream&
DataInputStream::operator >>(bool& b)
{
	read(&b, sizeof(bool));
	return *this;
}

inline DataInputStream&
DataInputStream::operator >>(wchar_t*& s)
{
	s = readWString();
	return *this;
}

inline DataInputStream&
DataInputStream::operator >>(char*& s)
{
	s = readString();
	return *this;
}

} // end namespace System

#endif // __InputStream_h
