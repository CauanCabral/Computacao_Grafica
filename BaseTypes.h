#ifndef __BaseTypes_h
#define __BaseTypes_h

//[]------------------------------------------------------------------------[]
//|                                                                          |
//|                        GVSG Foundation Classes                           |
//|                               Version 1.0                                |
//|                                                                          |
//|                Copyright® 2010, Paulo Aristarco Pagliosa                 |
//|                All Rights Reserved.                                      |
//|                                                                          |
//[]------------------------------------------------------------------------[]
//
// OVERVIEW: BaseTypes.h
// ========
// Header file for commom base types.

#ifndef __ObjectStream_h
#include "ObjectStream.h"
#endif
#ifndef __Color_h
#include "Color.h"
#endif

using namespace System;

//
// 3D vector serialization ops
//
inline ObjectOutputStream&
operator <<(ObjectOutputStream& oos, const Vec3& v)
{
	oos << v.x << v.y << v.z;
	return oos;
}

inline ObjectInputStream&
operator >>(ObjectInputStream& ois, Vec3& v)
{
	ois >> v.x >> v.y >> v.z;
	return ois;
}

//
// Color serialization ops
//
inline ObjectOutputStream&
operator <<(ObjectOutputStream& oos, const Color& c)
{
	oos << c.r << c.g << c.b;
	return oos;
}

inline ObjectInputStream&
operator >>(ObjectInputStream& ois, Color& c)
{
	ois >> c.r >> c.g >> c.b;
	return ois;
}

#endif // __BaseTypes_h
