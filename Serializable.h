#ifndef __Serializable_h
#define __Serializable_h

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
//  OVERVIEW: Serializable.h
//  ========
//  Class definition for serializable objects.

#include <typeinfo>

#ifndef __Exception_h
#include "Exception.h"
#endif
#ifndef __Hash_h
#include "Hash.h"
#endif

#define CLASS_TABLE_SIZE 11

namespace System
{ // begin namespace System

using namespace Collections;

//
// Forward definitions
//
class ObjectInputStream;
class ObjectOutputStream;
class Streamer;
class Serializable;


//////////////////////////////////////////////////////////
//
// Pointer: object pointer
// =======
template <typename T>
class Pointer
{
public:
	// Constructor
	Pointer(const T* address)
	{
		this->address = address;
	}

	bool operator ==(const Pointer<T>& p) const
	{
		return address == p.address;
	}

	const T* address;

}; // Pointer

template <typename T>
inline uint
hashValue(const Pointer<T>& p)
{
	return (uint)(long long)(p.address) & 0x7FFFFFFF;
}

//
// SerializableInit enum
//
enum SerializableInit
{
	serializableInit
};


//////////////////////////////////////////////////////////
//
// Serializable: serializable object
// ============
class Serializable
{
public:
	// Destructor
	virtual ~Serializable()
	{
		// do nothing
	}

	const char* getClassName() const
	{
		return typeid(*this).name();
	}

}; // Serializable


//
// Auxiliary functions
//
extern Streamer* getStreamer(const char*, Serializable*);

inline Streamer*
getStreamer(Serializable& obj)
{
	return System::getStreamer(obj.getClassName(), &obj);
}


//////////////////////////////////////////////////////////
//
// ClassName: class name
// =========
class ClassName
{
public:
	// Constructor
	ClassName(const char* name)
	{
		this->name = name;
	}

	bool operator ==(const ClassName& className) const
	{
		return strcmp(name, className.name)== 0;
	}

	const char* name;

}; // ClassName

extern uint hashValue(const char*);

inline uint
hashValue(const ClassName& className)
{
	return System::hashValue(className.name);
}

//
// BUILDER typedef
//
typedef Streamer* (*S_BUILDER)(Serializable*);


//////////////////////////////////////////////////////////
//
// SerializableClass: serializable class
// =================
class SerializableClass
{
public:
	typedef HashTable<ClassName, SerializableClass*> Classes;

	// Constructor
	SerializableClass(const char* name, S_BUILDER builder)
	{
		this->builder = builder;
		registerClass(name, this);
	}

	Streamer* getStreamer(Serializable* obj) const
	{
		return (*builder)(obj);
	}

	static SerializableClass* getClass(const char* name)
	{
		SerializableClass* const* cls = classes->get(name);
		return cls != 0 ? *cls : 0;
	}

private:
	S_BUILDER builder;
	static Classes* classes;

	static void initializeClasses()
	{
		if (classes == 0)
			classes = new Classes();
	}
	static void registerClass(const char*, SerializableClass*);

}; // SerializableClass


//////////////////////////////////////////////////////////
//
// Streamer: generic serializable object streamer
// ========
class Streamer
{
public:
	Serializable* getObject() const
	{
		return object;
	}

protected:
	// Constructor
	Streamer(Serializable *obj):
		object(obj)
	{
		// do nothing
	}

	virtual Serializable* read(ObjectInputStream&) const = 0;
	virtual void write(ObjectOutputStream&) const = 0;

private:
	Serializable* object;

	friend class ObjectInputStream;
	friend class ObjectOutputStream;

}; // Streamer


//
// Macros to define serializable objects
//
#define DECLARE_SERIALIZABLE_STREAMER(cls) \
public: \
	class Streamer: public System::Streamer \
	{ \
	public: \
		Streamer(System::Serializable* obj): \
			System::Streamer(obj), object(dynamic_cast<cls*>(obj)) \
		{ \
		} \
		void write(ObjectOutputStream&) const; \
		System::Serializable* read(ObjectInputStream&) const; \
		cls* getObject() const \
		{ \
			return object; \
		} \
		static System::Streamer* build(System::Serializable* obj) \
		{ \
			return new Streamer(obj ? obj : new cls(System::serializableInit)); \
		} \
	private: \
		cls* object; \
	}; \
	friend class Streamer

#define DECLARE_SERIALIZABLE_STREAMER_FROM_BASE(cls, base) \
public: \
	class Streamer: public base::Streamer \
	{ \
	public: \
		Streamer(System::Serializable* obj): \
			base::Streamer(obj) \
		{ \
		} \
		static System::Streamer* build(System::Serializable* obj) \
		{ \
			return new Streamer(obj ? obj : new cls(System::serializableInit)); \
		} \
	}; \
	friend class Streamer

#define DECLARE_ABSTRACT_SERIALIZABLE_STREAMER(cls) \
public: \
	class Streamer: public System::Streamer \
	{ \
	public: \
		Streamer(System::Serializable* obj): \
			System::Streamer(obj), object(dynamic_cast<cls*>(obj)) \
		{ \
		} \
		void write(ObjectOutputStream&) const; \
		System::Serializable* read(ObjectInputStream&) const; \
		cls* getObject() const \
		{ \
			return object; \
		} \
	private: \
		cls* object; \
	}; \
	friend class Streamer

#define DECLARE_SERIALIZABLE_CTOR(cls) \
public: \
	cls(System::SerializableInit)

#define DECLARE_SERIALIZABLE(cls) \
DECLARE_SERIALIZABLE_STREAMER(cls); \
DECLARE_SERIALIZABLE_CTOR(cls)

#define DECLARE_SERIALIZABLE_FROM_BASE(cls, base) \
DECLARE_SERIALIZABLE_STREAMER_FROM_BASE(cls, base); \
DECLARE_SERIALIZABLE_CTOR(cls)

#define DECLARE_ABSTRACT_SERIALIZABLE(cls) \
DECLARE_ABSTRACT_SERIALIZABLE_STREAMER(cls); \
DECLARE_SERIALIZABLE_CTOR(cls)

#define IMPLEMENT_SERIALIZABLE_CLASS(cls) \
System::SerializableClass s##cls(typeid(cls).name(), &cls::Streamer::build)

#define IMPLEMENT_SERIALIZABLE_CTOR(cls) \
cls::cls(System::SerializableInit) \
{ \
}

#define IMPLEMENT_SERIALIZABLE_CTOR1(cls, base) \
cls::cls(System::SerializableInit): \
	base(serializableInit) \
{ \
}

#define IMPLEMENT_SERIALIZABLE_CTOR2(cls, base1, base2) \
cls::cls(System::SerializableInit): \
	base1(serializableInit), \
	base2(serializableInit) \
{ \
}

#define IMPLEMENT_SERIALIZABLE_CTOR3(cls, base1, base2, base3) \
cls::cls(System::SerializableInit): \
	base1(serializableInit), \
	base2(serializableInit) \
	base3(serializableInit) \
{ \
}

#define IMPLEMENT_ABSTRACT_SERIALIZABLE(cls) \
IMPLEMENT_SERIALIZABLE_CTOR(cls)

#define IMPLEMENT_ABSTRACT_SERIALIZABLE1(cls, base) \
IMPLEMENT_SERIALIZABLE_CTOR1(cls, base)

#define IMPLEMENT_ABSTRACT_SERIALIZABLE2(cls, base1, base2) \
IMPLEMENT_SERIALIZABLE_CTOR2(cls, base1, base2)

#define IMPLEMENT_ABSTRACT_SERIALIZABLE3(cls, base1, base2, base3) \
IMPLEMENT_SERIALIZABLE_CTOR3(cls, base1, base2, base3)

#define IMPLEMENT_SERIALIZABLE(cls) \
IMPLEMENT_SERIALIZABLE_CLASS(cls); \
IMPLEMENT_ABSTRACT_SERIALIZABLE(cls)

#define IMPLEMENT_SERIALIZABLE1(cls, base) \
IMPLEMENT_SERIALIZABLE_CLASS(cls); \
IMPLEMENT_ABSTRACT_SERIALIZABLE1(cls, base)

#define IMPLEMENT_SERIALIZABLE2(cls, base1, base2) \
IMPLEMENT_SERIALIZABLE_CLASS(cls); \
IMPLEMENT_ABSTRACT_SERIALIZABLE2(cls, base1, base2)

#define IMPLEMENT_SERIALIZABLE3(cls, base1, base2, base3) \
IMPLEMENT_SERIALIZABLE_CLASS(cls); \
IMPLEMENT_ABSTRACT_SERIALIZABLE3(cls, base1, base2, base3)

#define IMPLEMENT_SERIALIZABLE_FROM_BASE(cls, base) \
IMPLEMENT_SERIALIZABLE_CLASS(cls); \
IMPLEMENT_SERIALIZABLE_CTOR1(cls, base)

} // end namespace System

#endif // __Serializable_h
