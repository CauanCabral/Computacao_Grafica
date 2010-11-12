#ifndef __NameableObject_h
#define __NameableObject_h

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
// OVERVIEW: NameableObject.h
// ========
// Class definition for nameable object.

#ifndef __Object_h
#include "Object.h"
#endif

namespace System
{ // begin namespace System


//////////////////////////////////////////////////////////
//
// NameableObject: nameable object
// ==============
class NameableObject: public virtual Object
{
public:
	// Constructors
	NameableObject()
	{
		// do nothing
	}
	NameableObject(const String& s):
		name(s)
	{
		// do nothing
	}

	// Get the object name
	String getName() const
	{
		return name;
	}

	// Set the object name
	void setName(const String& name)
	{
		this->name = name;
	}

	DECLARE_ABSTRACT_SERIALIZABLE(NameableObject);

private:
	String name;

}; // NameableObject

} // end namespace System

#endif // __NameableObject_h
