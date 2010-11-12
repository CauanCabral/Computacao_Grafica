#ifndef __Exception_h
#define __Exception_h

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
//  OVERVIEW: Exception.h
//  ========
//  Class definitions for exceptions.

#ifndef __WString_h
#include "WString.h"
#endif

#include <stdio.h>
#ifdef __LINUX
#include <wchar.h>
#endif

namespace System
{ // begin namespace System


//////////////////////////////////////////////////////////
//
// Exception: generic exception class
// =========
class Exception
{
public:
	// Constructors
	Exception()
	{
		// do nothing
	}
	Exception(const Exception& e):
		message(e.message)
	{
		// do nothing
	}
	Exception(const String& msg):
		message(msg)
	{
		// do nothing
	}

	String getMessage() const
	{
		return message;
	}
	operator const wchar_t*() const
	{
		return message;
	}

protected:
	String message;

private:
	Exception& operator =(const Exception&);

}; // Exception


//////////////////////////////////////////////////////////
//
// OutOfRangeException: out of range exception class
// ===================
class OutOfRangeException: public Exception
{
public:
	// Costructors
	OutOfRangeException():
		Exception(L"Out of range exception")
	{
		// do nothing
	}
	OutOfRangeException(const String& msg):
		Exception(msg)
	{
		// do nothing
	}

}; // OutOfRangeException


//////////////////////////////////////////////////////////
//
// IndexOutOfBoundsException: index out of bounds exception class
// =========================
class IndexOutOfBoundsException: public Exception
{
public:
	// Constructors
	IndexOutOfBoundsException():
		Exception(L"Index out of bounds exception")
	{
		// do nothing
	}
	IndexOutOfBoundsException(const String& msg):
		Exception(msg)
	{
		// do nothing
	}

}; // IndexOutOfBoundsException


//////////////////////////////////////////////////////////
//
// ClassNotFoundException: class not found exception
// ======================
class ClassNotFoundException: public Exception
{
public:
	// Constructor
	ClassNotFoundException(const String& clsName):
		Exception(String(L"Class not found exception: ") + clsName)
	{
		// do nothing
	}

}; // ClassNotFoundException


//////////////////////////////////////////////////////////
//
// IOException: IO exception
// ===========
class IOException: public Exception
{
public:
	// Constructors
	IOException():
		Exception(L"IO exception")
	{
		// do nothing
	}
	IOException(const String& msg):
		Exception(msg)
	{
		// do nothing
	}

}; // IOException


//////////////////////////////////////////////////////////
//
// Precondition: precondition class
// ============
class Precondition: public Exception
{
public:
	// Constructor
	Precondition(const char* expr, const char* file, int line):
		Exception(makeMessage(expr, file, line))
	{
		// do nothing
	}

private:
	static String makeMessage(const char*, const char*, int);

}; // Precondition


//////////////////////////////////////////////////////////
//
// Precondition inline implementtaion
// ============
#define PF_MAXLEN 1023
#define PF_FORMAT L"Precondition %S %d: %S"

inline String
Precondition::makeMessage(const char* expr, const char* file, int line)
{
	wchar_t msg[PF_MAXLEN + 1];
	int len = swprintf(msg, PF_MAXLEN, PF_FORMAT, file, line, expr);

	return String(msg, len);
}

#undef PF_MAXLEN
#undef PF_FORMAT

//
// PRECONDITION macro
//
#define PRECONDITION(expr) \
if (!(expr)) throw Precondition(#expr, __FILE__ , __LINE__)

} // end namespace System

#endif // __Exception_h
