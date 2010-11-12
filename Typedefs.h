#ifndef __Typedefs_h
#define __Typedefs_h

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
//  OVERVIEW: Typedefs.h
//  ========
//  Commom typedefs.

typedef unsigned char uint8;
typedef signed char int8;
typedef unsigned short uint16;
typedef signed short int16;
typedef unsigned long uint32;
typedef signed long int32;
typedef unsigned int uint;

#define UINT8(buf,i) (*(uint8*)((uint8*)buf + (i)))
#define INT8(buf,i) (*(int8*)((uint8*)buf + (i)))
#define UINT16(buf,i) (*(uint16*)((uint8*)buf + (i)))
#define INT16(buf,i) (*(int16*)((uint8*)buf + (i)))
#define UINT32(buf,i) (*(uint32*)((uint8*)buf + (i)))
#define INT32(buf,i) (*(int32*)((uint8*)buf + (i)))
#define INT(buf,i) (*(int*)((uint8*)buf + (i)))

namespace System
{ // begin namespace System

//
// Auxiliary functions
//
template <typename T>
inline void
swap(T& a, T& b)
{
	T temp = a;

	a = b;
	b = temp;
}

template <typename T>
inline bool
checkRange(const T& x, const T& a, const T& b)
{
	return x >= a && x <= b;
}

} // end namespace System

#endif // __Typedefs_h
