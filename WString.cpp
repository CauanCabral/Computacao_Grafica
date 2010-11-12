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
//  OVERVIEW: WString.cpp
//  ========
//  Source file for string.

#include <stdio.h>
#ifdef __LINUX
#include <wchar.h>
#include <wctype.h>
#else
#include <ctype.h>
#endif

#ifndef __Exception_h
#include "Exception.h"
#endif

using namespace System;

#define MIN_STRBUF_SIZE 4

//
// Roundup string body size
//
inline int
roundupSize(size_t size)
{
	return (size + MIN_STRBUF_SIZE - 1) & -MIN_STRBUF_SIZE;
}


//////////////////////////////////////////////////////////
//
// StringBody implementation
// ==========
StringBody* StringBody::emptyString;

void*
StringBody::operator new(size_t size, size_t length)
{
	return ::operator new(roundupSize(size + (length + 1) * sizeof(wchar_t)));
}

void
StringBody::operator delete(void* ptr, size_t)
{
	return ::operator delete(ptr);
}

StringBody*
StringBody::New(const wchar_t* s)
{
	return s == 0 ? New() : New(s, (int)wcslen(s));
}

StringBody*
StringBody::New(const char* s)
{
	return s == 0 ? New() : New(s, (int)strlen(s));
}

StringBody*
StringBody::New(const wchar_t* buffer, int length)
{
	StringBody* body = New(length);

#ifndef __LINUX
	swprintf(body->stringBuffer(), length + 1, L"%.*s", length, buffer);
#else
	wcsncpy(body->stringBuffer(), buffer, length);
	body->stringBuffer()[length] = 0;
#endif
	return body;
}

StringBody*
StringBody::New(const char* buffer, int length)
{
	StringBody* body = New(length);

	swprintf(body->stringBuffer(), length + 1, L"%.*S", length, buffer);
	return body;
}

StringBody*
StringBody::append(StringBody* s)
{
	if (s->isEmpty())
		return this->makeUse();
	if (this->isEmpty())
		return s->makeUse();

	StringBody* body = New(this->stringBuffer(), this->length + s->length);

	wcscpy(body->stringBuffer() + this->length, s->stringBuffer());
	return body;
}

int
StringBody::compare(StringBody* s)
{
	return this == s ? 0 : wcscmp(stringBuffer(), s->stringBuffer());
}

const wchar_t&
StringBody::operator [](int i)
{
	if (i < 0 || i >= length)
		throw OutOfRangeException();
	return stringBuffer()[i];
}

int
StringBody::indexOf(wchar_t c) const
{
	const wchar_t* stringBuffer = (const wchar_t*)(*this);
	const wchar_t* indexPointer = wcschr(stringBuffer, c);

	return (int)(indexPointer ? indexPointer - stringBuffer : - 1);
}

int
StringBody::lastIndexOf(wchar_t c) const
{
	const wchar_t* stringBuffer = (const wchar_t*)(*this);
	const wchar_t* indexPointer = wcsrchr(stringBuffer, c);

	return (int)(indexPointer ? indexPointer - stringBuffer : - 1);
}

StringBody*
StringBody::reverse()
{
	if (this->isEmpty())
		return New();

	wchar_t* thisBuffer = stringBuffer();
	StringBody* body = New(length);
	wchar_t* bodyBuffer = body->stringBuffer();

	for (int n = length; n;)
		*bodyBuffer++ = thisBuffer[--n];
	*bodyBuffer = 0;
	return body;
}

StringBody*
StringBody::toLower()
{
	if (this->isEmpty())
		return New();

	wchar_t* thisBuffer = stringBuffer();
	StringBody* body = New(length);
	wchar_t* bodyBuffer = body->stringBuffer();

	for (int i = 0; i < length; ++i)
		bodyBuffer[i] = towlower(thisBuffer[i]);
	return body;
}

StringBody*
StringBody::toUpper()
{
	if (this->isEmpty())
		return New();

	wchar_t* thisBuffer = stringBuffer();
	StringBody* body = New(length);
	wchar_t* bodyBuffer = body->stringBuffer();

	for (int i = 0; i < this->length; ++i)
		bodyBuffer[i] = towupper(thisBuffer[i]);
	return body;
}


//////////////////////////////////////////////////////////
//
// String implementation
// ======
String&
String::copy(const String& s, int type)
{
	body->release();
	body = type == DeepCopy ? StringBody::New(s.body) : s.body->makeUse();
	return *this;
}

String
String::toString(wchar_t c)
{
	wchar_t s[2];

	s[0] = c;
	s[1] = 0;
	return String(s);
}

String
String::toString(char c)
{
	char s[2];

	s[0] = c;
	s[1] = 0;
	return String(s);
}

String
String::toString(int i)
{
	wchar_t s[16];

	swprintf(s, sizeof(s) / sizeof(wchar_t) - 1, L"%d", i);
	return String(s);
}

String
String::toString(double d)
{
	wchar_t s[20];

	swprintf(s, sizeof(s) / sizeof(wchar_t) - 1, L"%g", d);
	return String(s);
}

String
String::toString(bool b)
{
	return String(b ? L"true" : L"false");
}
