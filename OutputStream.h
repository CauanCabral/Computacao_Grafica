#ifndef __OutputStream_h
#define __OutputStream_h

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
//  OVERVIEW: OutputStream.h
//  ========
//  Class definition for output streams.

#ifndef __BufferedStream_h
#include "BufferedStream.h"
#endif

namespace System
{ // begin namespace System


//////////////////////////////////////////////////////////
//
// OutputStream: generic output stream
// ============
class OutputStream
{
public:
	// Destructor
	virtual ~OutputStream();

	virtual void close() = 0;
	virtual void flush() = 0;
	virtual void write(const void*, long) = 0;

}; // OutputStream


//////////////////////////////////////////////////////////
//
// FilterOutputStream: generic filter output stream
// ==================
class FilterOutputStream: public OutputStream
{
public:
	// Constructor
	FilterOutputStream(OutputStream*);

	// Destructor
	~FilterOutputStream();

	void close();
	void flush();
	void write(const void*, long);

protected:
	OutputStream* out;

	void ensureOpen() const;

}; // FilterOutputStream


//////////////////////////////////////////////////////////
//
// BufferedOutputStream: generic buffered output stream
// ====================
class BufferedOutputStream: public FilterOutputStream, public BufferedStream
{
public:
	// Constructor
	BufferedOutputStream(OutputStream*, uint = STR_BUFFER_SIZE);

	void close();
	void flush();
	void write(const void*, long);

protected:
	void flushBuffer();		

}; // BufferedOutputStream


//////////////////////////////////////////////////////////
//
// DataOutputStream: data output stream
// ================
class DataOutputStream: public FilterOutputStream
{
public:
	// Constructor
	DataOutputStream(OutputStream*);

	void writeUInt8(uint8);
	void writeInt8(int8);
	void writeUInt16(uint16);
	void writeInt16(int16);
	void writeUInt32(uint32);
	void writeInt32(int32);
	void writeWChar(wchar_t);
	void writeChar(char);
	void writeShort(short);
	void writeInt(int);
	void writeLong(long);
	void writeFloat(float);
	void writeDouble(double);
	void writeBool(bool);
	void writeWString(const wchar_t*);
	void writeString(const char*);

	DataOutputStream& operator <<(wchar_t);
	DataOutputStream& operator <<(char);
	DataOutputStream& operator <<(unsigned char);
	DataOutputStream& operator <<(short);
	DataOutputStream& operator <<(unsigned short);
	DataOutputStream& operator <<(int);
	DataOutputStream& operator <<(unsigned int);
	DataOutputStream& operator <<(long);
	DataOutputStream& operator <<(unsigned long);
	DataOutputStream& operator <<(float);
	DataOutputStream& operator <<(double);
	DataOutputStream& operator <<(bool);
	DataOutputStream& operator <<(const wchar_t*);
	DataOutputStream& operator <<(const char*);

}; // DataOutputStream


//////////////////////////////////////////////////////////
//
// FilterOutputStream inline implementation
// ==================
inline
FilterOutputStream::FilterOutputStream(OutputStream* out)
{
	this->out = out;
}

inline void
FilterOutputStream::ensureOpen() const
{
	if (out == 0)
		throw IOException(L"Output stream closed");
}


//////////////////////////////////////////////////////////
//
// BufferedOutputStream inline implementation
// ====================
inline
BufferedOutputStream::BufferedOutputStream(OutputStream* out, uint size):
	FilterOutputStream(out),
	BufferedStream(size)
{
	// do nothing
}


//////////////////////////////////////////////////////////
//
// DataOutputStream inline implementation
// ================
inline
DataOutputStream::DataOutputStream(OutputStream* out):
	FilterOutputStream(out)
{
	// do nothing
}

inline void
DataOutputStream::writeUInt8(uint8 i)
{
	write(&i, sizeof(uint8));
}

inline void
DataOutputStream::writeInt8(int8 i)
{
	write(&i, sizeof(int8));
}

inline void
DataOutputStream::writeUInt16(uint16 i)
{
	write(&i, sizeof(uint16));
}

inline void
DataOutputStream::writeInt16(int16 i)
{
	write(&i, sizeof(int16));
}

inline void
DataOutputStream::writeUInt32(uint32 i)
{
	write(&i, sizeof(uint32));
}

inline void
DataOutputStream::writeInt32(int32 i)
{
	write(&i, sizeof(int32));
}

inline void
DataOutputStream::writeWChar(wchar_t c)
{
	write(&c, sizeof(wchar_t));
}

inline void
DataOutputStream::writeChar(char c)
{
	write(&c, sizeof(char));
}

inline void
DataOutputStream::writeShort(short s)
{
	write(&s, sizeof(short));
}

inline void
DataOutputStream::writeInt(int i)
{
	write(&i, sizeof(int));
}

inline void
DataOutputStream::writeLong(long l)
{
	write(&l, sizeof(long));
}

inline void
DataOutputStream::writeFloat(float f)
{
	write(&f, sizeof(float));
}

inline void
DataOutputStream::writeDouble(double d)
{
	write(&d, sizeof(double));
}

inline void
DataOutputStream::writeBool(bool b)
{
	write(&b, sizeof(bool));
}

inline DataOutputStream&
DataOutputStream::operator <<(wchar_t c)
{
	write(&c, sizeof(wchar_t));
	return *this;
}

inline DataOutputStream&
DataOutputStream::operator <<(char c)
{
	write(&c, sizeof(char));
	return *this;
}

inline DataOutputStream&
DataOutputStream::operator <<(unsigned char c)
{
	write(&c, sizeof(unsigned char));
	return *this;
}

inline DataOutputStream&
DataOutputStream::operator <<(short s)
{
	write(&s, sizeof(short));
	return *this;
}

inline DataOutputStream&
DataOutputStream::operator <<(unsigned short s)
{
	write(&s, sizeof(unsigned short));
	return *this;
}

inline DataOutputStream&
DataOutputStream::operator <<(int i)
{
	write(&i, sizeof(int));
	return *this;
}

inline DataOutputStream&
DataOutputStream::operator <<(unsigned int i)
{
	write(&i, sizeof(unsigned int));
	return *this;
}

inline DataOutputStream&
DataOutputStream::operator <<(long l)
{
	write(&l, sizeof(long));
	return *this;
}

inline DataOutputStream&
DataOutputStream::operator <<(unsigned long l)
{
	write(&l, sizeof(unsigned long));
	return *this;
}

inline DataOutputStream&
DataOutputStream::operator <<(float f)
{
	write(&f, sizeof(float));
	return *this;
}

inline DataOutputStream&
DataOutputStream::operator <<(double d)
{
	write(&d, sizeof(double));
	return *this;
}

inline DataOutputStream&
DataOutputStream::operator <<(bool b)
{
	write(&b, sizeof(bool));
	return *this;
}

inline DataOutputStream&
DataOutputStream::operator <<(const wchar_t* s)
{
	writeWString(s);
	return *this;
}

inline DataOutputStream&
DataOutputStream::operator <<(const char* s)
{
	writeString(s);
	return *this;
}

} // end namespace System

#endif // __OutputStream_h
