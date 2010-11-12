#ifndef __ObjectInputStream_h
#define __ObjectInputStream_h

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
//  OVERVIEW: ObjectInputStream.h
//  ========
//  Class definition for object input stream.

#ifndef __Array_h
#include "Array.h"
#endif
#ifndef __InputStream_h
#include "InputStream.h"
#endif
#ifndef __ObjectStreamConstants_h
#include "ObjectStreamConstants.h"
#endif
#ifndef __Serializable_h
#include "Serializable.h"
#endif

namespace System
{ // begin namespace System


//////////////////////////////////////////////////////////
//
// ReadObjects: read objects
// ===========
class ReadObjects
{
public:
	class Id
	{
		friend uint hashValue(const Id&);

	public:
		Id(int32 i):
			value(i)
		{
			// do nothing
		}

		bool operator ==(const Id& id) const
		{
			return value == id.value;
		}

		int32 value;

	}; // Id

	// Constructor
	ReadObjects():
		nextId(1)
	{
		// do nothing
	}

private:
	HashTable<Id, Serializable*> map;
	int32 nextId;

	void registerObject(Serializable* obj)
	{
		map.put(Id(nextId++), obj);
	}

	Serializable* findObject(int32 id) const
	{
		Serializable* const* obj = map.get(Id(id));
		return obj != 0 ? *obj : 0;
	}

	friend class ObjectInputStream;

}; // ReadObjects

inline uint
hashValue(const ReadObjects::Id& id)
{
	return (uint)(id.value & 0x7FFFFFFF);
}


//////////////////////////////////////////////////////////
//
// ClassNameArray: class name array
// ==============
class ClassNameArray
{
public:
	// Constructor
	ClassNameArray():
		entries(CLASS_TABLE_SIZE)
	{
		entries.shouldDelete = true;
		entries.add(0);
	}

private:
	PointerArray<const char> entries;

	void registerClass(const char* name)
	{
		entries.add(name);
	}

	const char* findClass(int32 id) const
	{
		return entries[id];
	}

	friend class ObjectInputStream;

}; // ClassNameArray


//////////////////////////////////////////////////////////
//
// ObjectInputStream: object input stream
// =================
class ObjectInputStream: public DataInputStream
{
public:
	// Constructor
	ObjectInputStream(InputStream*);

	Serializable* readObject(Serializable&);
	Serializable* readObject();
	Serializable* readVirtualBaseObject(const Streamer&);
	Serializable* readBaseObject(const Streamer&);

	ObjectInputStream& operator >>(wchar_t&);
	ObjectInputStream& operator >>(char&);
	ObjectInputStream& operator >>(unsigned char&);
	ObjectInputStream& operator >>(short&);
	ObjectInputStream& operator >>(unsigned short&);
	ObjectInputStream& operator >>(int&);
	ObjectInputStream& operator >>(unsigned int&);
	ObjectInputStream& operator >>(long&);
	ObjectInputStream& operator >>(unsigned long&);
	ObjectInputStream& operator >>(float&);
	ObjectInputStream& operator >>(double&);
	ObjectInputStream& operator >>(bool&);
	ObjectInputStream& operator >>(wchar_t*&);
	ObjectInputStream& operator >>(char*&);

private:
	ReadObjects objects;
	ClassNameArray classes;

	Serializable* findObject(int32) const;
	SerializableClass* findClass(int32) const;
	void registerObject(Serializable*);
	void registerClass(const char*);

	SerializableClass* readPrefix();
	void readFields(SerializableClass*, Serializable*&);
	void readSuffix();

}; // ObjectInputStream


//////////////////////////////////////////////////////////
//
// ObjectInputStream inline implementation
// =================
inline ObjectInputStream&
ObjectInputStream::operator >>(wchar_t& c)
{
	read(&c, sizeof(wchar_t));
	return *this;
}

inline ObjectInputStream&
ObjectInputStream::operator >>(char& c)
{
	read(&c, sizeof(char));
	return *this;
}

inline ObjectInputStream&
ObjectInputStream::operator >>(unsigned char& c)
{
	read(&c, sizeof(unsigned char));
	return *this;
}

inline ObjectInputStream&
ObjectInputStream::operator >>(short& s)
{
	read(&s, sizeof(short));
	return *this;
}

inline ObjectInputStream&
ObjectInputStream::operator >>(unsigned short& s)
{
	read(&s, sizeof(unsigned short));
	return *this;
}

inline ObjectInputStream&
ObjectInputStream::operator >>(int& i)
{
	read(&i, sizeof(int));
	return *this;
}

inline ObjectInputStream&
ObjectInputStream::operator >>(unsigned int& i)
{
	read(&i, sizeof(unsigned int));
	return *this;
}

inline ObjectInputStream&
ObjectInputStream::operator >>(long& l)
{
	read(&l, sizeof(long));
	return *this;
}

inline ObjectInputStream&
ObjectInputStream::operator >>(unsigned long& l)
{
	read(&l, sizeof(unsigned long));
	return *this;
}

inline ObjectInputStream&
ObjectInputStream::operator >>(float& f)
{
	read(&f, sizeof(float));
	return *this;
}

inline ObjectInputStream&
ObjectInputStream::operator >>(double& d)
{
	read(&d, sizeof(double));
	return *this;
}

inline ObjectInputStream&
ObjectInputStream::operator >>(bool& b)
{
	read(&b, sizeof(bool));
	return *this;
}

inline ObjectInputStream&
ObjectInputStream::operator >>(wchar_t*& s)
{
	s = readWString();
	return *this;
}

inline ObjectInputStream&
ObjectInputStream::operator >>(char*& s)
{
	s = readString();
	return *this;
}

inline void
ObjectInputStream::registerObject(Serializable* obj)
{
	objects.registerObject(obj);
}

inline Serializable*
ObjectInputStream::findObject(int32 id) const
{
	return objects.findObject(id);
}

inline void
ObjectInputStream::registerClass(const char* clsName)
{
	classes.registerClass(clsName);
}

inline SerializableClass*
ObjectInputStream::findClass(int32 id) const
{
	return SerializableClass::getClass(classes.findClass(id));
}

//
// Auxiliary functions
//
template <typename Base>
inline Serializable*
readVirtualBaseObject(Base* base, ObjectInputStream& is)
{
	typedef class Base::Streamer Streamer;
	return is.readVirtualBaseObject(Streamer(base));
}

template <typename Base>
inline Serializable*
readBaseObject(Base* base, ObjectInputStream& is)
{
	typedef class Base::Streamer Streamer;
	return is.readBaseObject(Streamer(base));
}

template <typename T>
inline ObjectInputStream&
operator >>(ObjectInputStream& is, T*& obj)
{
	obj = dynamic_cast<T*>(is.readObject());
	return is;
}

} // end namespace System

#endif // ObjectInputStream
