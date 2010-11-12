#ifndef __WString_h
#define __WString_h

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
//  OVERVIEW: WString.h
//  ========
//  Class definition for string.

#include <string.h>

namespace System
{ // begin namespace System

//
// Constants
//
enum
{
	ShallowCopy,
	DeepCopy
};

//
// Forward definitions
//
class StringBody;
class OutOfRangeException;


//////////////////////////////////////////////////////////
//
// String: string class
// ======
class String
{
public:
	// Constructors
	String();
	String(const String&);
	String(const wchar_t*);
	String(const char*);
	String(const wchar_t*, int);
	String(const char*, int);

	// Destructor
	~String();

	// Assignment
	String& copy(const String&, int = ShallowCopy);
	String& operator =(const String&);
	String& operator =(wchar_t);
	String& operator =(char);
	String& operator =(int);
	String& operator =(double);
	String& operator =(bool);
	String& operator =(const wchar_t*);
	String& operator =(const char*);

	// Concatenation
	String operator +(const String&) const;
	String operator +(wchar_t) const;
	String operator +(char) const;
	String operator +(int) const;
	String operator +(double) const;
	String operator +(bool) const;
	String operator +(const wchar_t*) const;
	String operator +(const char*) const;

	// Concatenation & assignment
	String& append(const String&);
	String& operator +=(const String&);
	String& operator +=(wchar_t);
	String& operator +=(char);
	String& operator +=(int);
	String& operator +=(double);
	String& operator +=(bool);
	String& operator +=(const wchar_t*);
	String& operator +=(const char*);

	// Comparison
	int compare(const String&) const;
	bool operator ==(const String&) const;
	bool operator !=(const String&) const;

	// Subscripting
	wchar_t  operator [](int) const;
	wchar_t& operator [](int);

	// Character set searching
	int indexOf(wchar_t) const;
	int lastIndexOf(wchar_t) const;

	// Miscellaneous
	operator const wchar_t*() const;
	int length() const;
	String& reverse();
	String& toLower();
	String& toUpper();

	// Static casting
	static String toString(wchar_t);
	static String toString(char);
	static String toString(int);
	static String toString(double);
	static String toString(bool);

private:
	StringBody* body; // this is the body

	// Private constructor
	String(StringBody*);

}; // String


//////////////////////////////////////////////////////////
//
// StringBody: string body class
// ==========
class StringBody
{
private:
	int refCount;
	int length; // string buffer length

	static StringBody* emptyString;
	static StringBody* getEmptyString();

	// Constructor
	StringBody(int = 0);

	// Allocate memory to store a string body
	void* operator new(size_t, size_t);
	// Free string body memory
	void operator delete(void*, size_t);

	// Is this string body the empty string?
	bool isEmpty() const;

	// Static "constructors"
	static StringBody* New();
	static StringBody* New(StringBody*);
	static StringBody* New(int);
	static StringBody* New(const wchar_t*);
	static StringBody* New(const char*);
	static StringBody* New(const wchar_t*, int);
	static StringBody* New(const char*, int);

	// Make a shallow-copy of this string body
	StringBody* makeUse();
	// Release this string body
	void release();

	// Get the string buffer of this string body
	operator const wchar_t*() const;
	wchar_t* stringBuffer();

	// Get the character of this string body at the specified index
	const wchar_t& operator [](int);

	// Get the index within this string body of the specified character
	int indexOf(wchar_t) const;
	int lastIndexOf(wchar_t) const;

	// Append another string body to this string body
	StringBody* append(StringBody*);

	// Compare this string body to another
	int compare(StringBody*);

	// Reverse the string buffer of this this string buffer
	StringBody* reverse();

	// Convert this string body to lowercase
	StringBody* toLower();

	// Convert this string body to uppercase
	StringBody* toUpper();

	friend class String;

}; // StringBody


//////////////////////////////////////////////////////////
//
// StringBody inline implementation
// ==========
inline StringBody*
StringBody::getEmptyString()
{
	if (emptyString == 0)
		emptyString = StringBody::New(L"");
	return emptyString;
}

inline
StringBody::StringBody(int length)
{
	refCount = 1;
	this->length = length;
}

inline bool
StringBody::isEmpty() const
{
	return this == emptyString;
}

inline StringBody*
StringBody::New()
{
	return getEmptyString()->makeUse();
}

inline StringBody*
StringBody::New(StringBody* s)
{
	return s->isEmpty() ? New() : New(s->stringBuffer(), s->length);
}

inline StringBody*
StringBody::New(int length)
{
	return new(length) StringBody(length);
}

inline StringBody*
StringBody::makeUse()
{
	++refCount;
	return this;
}

inline void
StringBody::release()
{
	if (!--refCount)
		delete this;
}

inline
StringBody::operator const wchar_t*() const
{
	return reinterpret_cast<const wchar_t*>(this + 1);
}

inline wchar_t*
StringBody::stringBuffer()
{
	return const_cast<wchar_t*>((const wchar_t*)*this);
}


//////////////////////////////////////////////////////////
//
// String inline implementation
// ======
inline
String::String()
{
	body = StringBody::New();
}

inline
String::String(const String& s)
{
	body = s.body->makeUse();
}

inline
String::String(const wchar_t* s)
{
	body = StringBody::New(s);
}

inline
String::String(const char* s)
{
	body = StringBody::New(s);
}

inline
String::String(const wchar_t* s, int length)
{
	body = StringBody::New(s, length);
}

inline
String::String(const char* s, int length)
{
	body = StringBody::New(s, length);
}

inline
String::String(StringBody* body)
{
	this->body = body;
}

inline
String::~String()
{
	body->release();
}

inline String&
String::operator =(const String& s)
{
	return copy(s);
}

inline String&
String::operator =(wchar_t c)
{
	return operator =(toString(c));
}

inline String&
String::operator =(char c)
{
	return operator =(toString(c));
}

inline String&
String::operator =(int i)
{
	return operator =(toString(i));
}

inline String&
String::operator =(double d)
{
	return operator =(toString(d));
}

inline String&
String::operator =(bool b)
{
	return operator =(toString(b));
}

inline String&
String::operator =(const wchar_t* s)
{
	return operator =(String(s));
}

inline String&
String::operator =(const char* s)
{
	return operator =(String(s));
}

inline String
String::operator +(const String& s) const
{
	return String(body->append(s.body));
}

inline String
String::operator +(wchar_t c) const
{
	return operator +(toString(c));
}

inline String
String::operator +(char c) const
{
	return operator +(toString(c));
}

inline String
String::operator +(int i) const
{
	return operator +(toString(i));
}

inline String
String::operator +(double d) const
{
	return operator +(toString(d));
}

inline String
String::operator +(bool b) const
{
	return operator +(toString(b));
}

inline String
String::operator +(const wchar_t* s) const
{
	return operator +(String(s));
}

inline String
String::operator +(const char* s) const
{
	return operator +(String(s));
}

inline String&
String::append(const String& s)
{
	return operator =(operator +(s));
}

inline String&
String::operator +=(const String& s)
{
	return append(s);
}

inline String&
String::operator +=(wchar_t c)
{
	return append(toString(c));
}

inline String&
String::operator +=(char c)
{
	return append(toString(c));
}

inline String&
String::operator +=(int i)
{
	return append(toString(i));
}

inline String&
String::operator +=(double d)
{
	return append(toString(d));
}

inline String&
String::operator +=(bool b)
{
	return append(toString(b));
}

inline String&
String::operator +=(const wchar_t* s)
{
	return append(String(s));
}

inline String&
String::operator +=(const char* s)
{
	return append(String(s));
}

inline int
String::compare(const String& s) const
{
	return body->compare(s.body);
}

inline bool
String::operator ==(const String& s) const
{
	return compare(s) == 0;
}

inline bool
String::operator !=(const String& s) const
{
	return !operator ==(s);
}

inline wchar_t
String::operator [](int i) const
{
	return (*body)[i];
}

inline wchar_t&
String::operator [](int i)
{
	return const_cast<wchar_t&>((*body)[i]);
}

inline int
String::indexOf(wchar_t c) const
{
	return body->indexOf(c);
}

inline int
String::lastIndexOf(wchar_t c) const
{
	return body->lastIndexOf(c);
}

inline int
String::length() const
{
	return body->length;
}

inline
String::operator const wchar_t*() const
{
	return (const wchar_t*)*body;
}

inline String&
String::reverse()
{
	return operator =(String(body->reverse()));
}

inline String&
String::toLower()
{
	return operator =(String(body->toLower()));
}

inline String&
String::toUpper()
{
	return operator =(String(body->toUpper()));
}

} // end namespace System

#endif // __WString_h
