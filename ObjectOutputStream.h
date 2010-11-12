#ifndef __ObjectOutputStream_h
#define __ObjectOutputStream_h

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
//  OVERVIEW: ObjectOutputStream.h
//  ========
//  Class definition for object output stream.

#ifndef __ObjectStreamConstants_h
#include "ObjectStreamConstants.h"
#endif
#ifndef __OutputStream_h
#include "OutputStream.h"
#endif
#ifndef __Serializable_h
#include "Serializable.h"
#endif

namespace System
{ // begin of namespace System


//////////////////////////////////////////////////////////
//
// WrittenObjects: written objects
// ==============
class WrittenObjects
{
public:
	typedef Pointer<void> VoidPointer;

	// Constructor
	WrittenObjects():
		curId(0)
	{
		// do nothing
	}

	void clear()
	{
		map.clear();
	}

private:
	HashTable<VoidPointer, int32> map;
	int32 curId;

	int32 registerObject(Serializable* obj)
	{
		map.put(VoidPointer((char*)obj + 1), ++curId);
		return curId;
	}

	int32 registerVB(Serializable* obj)
	{
		map.put(VoidPointer(obj), ++curId);
		return curId;
	}

	int32 findObject(Serializable* obj) const
	{
		int32 const* pid = map.get(VoidPointer((char*)obj + 1));
		return pid != 0 ? *pid : 0;
	}

	int32 findVB(Serializable* obj) const
	{
		int32 const* pid = map.get(VoidPointer(obj));
		return pid != 0 ? *pid : 0;
	}

	friend class ObjectOutputStream;

}; // WrittenObjecs


//////////////////////////////////////////////////////////
//
// ClassNameTable: class name table
// ==============
class ClassNameTable
{
public:
	typedef HashTable<ClassName, int32> Map;

	// Constructor
	ClassNameTable():
		map(CLASS_TABLE_SIZE),
		curId(0)
	{
		// do nothing
	}

	void clear()
	{
		map.clear();
		curId = 0;
	}

	int32 registerClass(const char* name)
	{
		map.put(ClassName(name), ++curId);
		return curId;
	}

	int32 findClass(const char* name) const
	{
		int32 const* pid = map.get(ClassName(name));
		return pid != 0 ? *pid : 0;
	}

private:
	Map map;
	int32 curId;

}; // ClassNameTable


//////////////////////////////////////////////////////////
//
// ObjectOutputStream: object output stream
// ==================
class ObjectOutputStream: public DataOutputStream
{
public:
	// Constructor
	ObjectOutputStream(OutputStream*);

	void writeObject(const Serializable&);
	void writeObject(const Serializable*);
	void writeVirtualBaseObject(const Streamer&);
	void writeBaseObject(const Streamer&);

	ObjectOutputStream& operator <<(wchar_t);
	ObjectOutputStream& operator <<(char);
	ObjectOutputStream& operator <<(unsigned char);
	ObjectOutputStream& operator <<(short);
	ObjectOutputStream& operator <<(unsigned short);
	ObjectOutputStream& operator <<(int);
	ObjectOutputStream& operator <<(unsigned int);
	ObjectOutputStream& operator <<(long);
	ObjectOutputStream& operator <<(unsigned long);
	ObjectOutputStream& operator <<(float);
	ObjectOutputStream& operator <<(double);
	ObjectOutputStream& operator <<(bool);
	ObjectOutputStream& operator <<(const wchar_t*);
	ObjectOutputStream& operator <<(const char*);
	ObjectOutputStream& operator <<(const Serializable&);
	ObjectOutputStream& operator <<(const Serializable*);

private:
	WrittenObjects objects;
	ClassNameTable classes;

	int32 findObject(Serializable*) const;
	int32 findVB(Serializable*) const;
	int32 findClass(Serializable*) const;
	int32 registerObject(Serializable*);
	int32 registerVB(Serializable*);
	int32 registerClass(Serializable*);

	void writePrefix(Serializable*);
	void writeFields(Serializable*);
	void writeSuffix(Serializable*);

}; // ObjectOutputStream


//////////////////////////////////////////////////////////
//
// ObjectOutputStream inline implementation
// ===================
inline ObjectOutputStream&
ObjectOutputStream::operator <<(wchar_t c)
{
	write(&c, sizeof(wchar_t));
	return *this;
}

inline ObjectOutputStream&
ObjectOutputStream::operator <<(char c)
{
	write(&c, sizeof(char));
	return *this;
}

inline ObjectOutputStream&
ObjectOutputStream::operator <<(unsigned char c)
{
	write(&c, sizeof(unsigned char));
	return *this;
}

inline ObjectOutputStream&
ObjectOutputStream::operator <<(short s)
{
	write(&s, sizeof(short));
	return *this;
}

inline ObjectOutputStream&
ObjectOutputStream::operator <<(unsigned short s)
{
	write(&s, sizeof(unsigned short));
	return *this;
}

inline ObjectOutputStream&
ObjectOutputStream::operator <<(int i)
{
	write(&i, sizeof(int));
	return *this;
}

inline ObjectOutputStream&
ObjectOutputStream::operator <<(unsigned int i)
{
	write(&i, sizeof(unsigned int));
	return *this;
}

inline ObjectOutputStream&
ObjectOutputStream::operator <<(long l)
{
	write(&l, sizeof(long));
	return *this;
}

inline ObjectOutputStream&
ObjectOutputStream::operator <<(unsigned long l)
{
	write(&l, sizeof(unsigned long));
	return *this;
}

inline ObjectOutputStream&
ObjectOutputStream::operator <<(float f)
{
	write(&f, sizeof(float));
	return *this;
}

inline ObjectOutputStream&
ObjectOutputStream::operator <<(double d)
{
	write(&d, sizeof(double));
	return *this;
}

inline ObjectOutputStream&
ObjectOutputStream::operator <<(bool b)
{
	write(&b, sizeof(bool));
	return *this;
}

inline ObjectOutputStream&
ObjectOutputStream::operator <<(const wchar_t* s)
{
	writeWString(s);
	return *this;
}

inline ObjectOutputStream&
ObjectOutputStream::operator <<(const char* s)
{
	writeString(s);
	return *this;
}

inline ObjectOutputStream&
ObjectOutputStream::operator <<(const Serializable& obj)
{
	writeObject(obj);
	return *this;
}

inline ObjectOutputStream&
ObjectOutputStream::operator <<(const Serializable* obj)
{
	writeObject(obj);
	return *this;
}

inline int32
ObjectOutputStream::findObject(Serializable* obj) const
{
	return objects.findObject(obj);
}

inline int32
ObjectOutputStream::findVB(Serializable* obj) const
{
	return objects.findVB(obj);
}

inline int32
ObjectOutputStream::findClass(Serializable* obj) const
{
	return classes.findClass(obj->getClassName());
}

inline int32
ObjectOutputStream::registerObject(Serializable* obj)
{
	return objects.registerObject(obj);
}

inline int32
ObjectOutputStream::registerVB(Serializable* obj)
{
	return objects.registerVB(obj);
}

inline int32
ObjectOutputStream::registerClass(Serializable* obj)
{
	return classes.registerClass(obj->getClassName());
}

inline void
ObjectOutputStream::writeSuffix(Serializable*)
{
	writeInt8(']');
}

//
// Auxiliary functions
//
template <class Base>
inline void
writeVirtualBaseObject(Base* base, ObjectOutputStream& os)
{
	typedef typename Base::Streamer Streamer;
	os.writeVirtualBaseObject(Streamer(base));
}

template <class Base>
inline void
writeBaseObject(Base* base, ObjectOutputStream& os)
{
	typedef typename Base::Streamer Streamer;
	os.writeBaseObject(Streamer(base));
}

} // end of namespace System

#endif // __ObjectOutputStream_h
