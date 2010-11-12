#ifndef __Object_h
#define __Object_h

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
//  OVERVIEW: Object.h
//  ========
//  Class definition for generic object.

#ifndef __ObjectStream_h
#include "ObjectStream.h"
#endif

namespace System
{ // begin namespace System


//////////////////////////////////////////////////////////
//
// Object: generic object class
// ======
class Object: public virtual Serializable
{
public:
	// Destructor
	virtual ~Object();

	// Make a deep-copy of this object
	virtual Object* makeCopy() const;

	// Make a shallow-copy of this object
	Object* makeUse()
	{
		++count;
		return this;
	}

	// Release this object
	virtual void release()
	{
		if (--count <= 0)
			delete this;
	}

	// Get number of uses of this object
	int getNumberOfUses() const
	{
		return count;
	}

	DECLARE_SERIALIZABLE(Object);

protected:
	// Protected default constructor
	Object():
		count(0)
	{
		// do nothing
	}

private:
	int count; // reference counter

}; // Object

//
// Auxiliary functions
//
template <typename T>
inline T*
makeUse(T* object)
{
	return object == 0 ? 0 : dynamic_cast<T*>(object->makeUse());
}

inline void
release(Object* object)
{
	if (object != 0)
		object->release();
}


//////////////////////////////////////////////////////////
//
// ObjectPtr: object pointer class
// =========
template <typename T>
class ObjectPtr
{
public:
	// Constructors
	ObjectPtr():
		object(0)
	{
		// do nothing
	}
	ObjectPtr(const ObjectPtr<T>& ptr)
	{
		this->object = makeUse(ptr.object);
	}
	ObjectPtr(T* object)
	{
		this->object = makeUse(object);
	}

	// Destructor
	~ObjectPtr()
	{
		release(this->object);
	}

	ObjectPtr<T>& operator =(T* object)
	{
		release(this->object);
		this->object = makeUse(object);
		return *this;
	}
	ObjectPtr<T>& operator =(const ObjectPtr<T>& ptr)
	{
		release(this->object);
		this->object = makeUse(ptr.object);
		return *this;
	}

	bool operator ==(T* object) const
	{
		return this->object == object;
	}
	bool operator ==(const ObjectPtr<T>& ptr) const
	{
		return this->object == ptr.object;
	}

	bool operator !=(T* object) const
	{
		return !operator ==(object);
	}
	bool operator !=(const ObjectPtr<T>& ptr) const
	{
		return !operator ==(ptr);
	}

	operator T*() const
	{
		return this->object;
	}

private:
	T* object; // this is the object

}; // ObjectPtr

//
// Serialization
//
template <typename T>
inline ObjectOutputStream&
operator <<(ObjectOutputStream& oos, const ObjectPtr<T>& ptr)
{
	oos << (T*)ptr;
	return oos;
}

template <typename T>
inline ObjectInputStream&
operator >>(ObjectInputStream& ois, ObjectPtr<T>& ptr)
{
	T* temp;

	ois >> temp; ptr = temp;
	return ois;
}

} // end namespace System

#endif // __Object_h
