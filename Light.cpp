//[]------------------------------------------------------------------------[]
//|                                                                          |
//|                          GVSG Graphics Classes                           |
//|                               Version 1.0                                |
//|                                                                          |
//|                Copyright® 2007, Paulo Aristarco Pagliosa                 |
//|                All Rights Reserved.                                      |
//|                                                                          |
//[]------------------------------------------------------------------------[]
//
//  OVERVIEW: Light.cpp
//  ========
//  Source file for light.

#ifndef __Light_h
#include "Light.h"
#endif

using namespace Graphics;


//////////////////////////////////////////////////////////
//
// Light implementation
// =====
IMPLEMENT_SERIALIZABLE1(Light, SceneComponent);

void
Light::Streamer::write(ObjectOutputStream& oos) const
//[]---------------------------------------------------[]
//|  Write                                              |
//[]---------------------------------------------------[]
{
	writeBaseObject((SceneComponent*)getObject(), oos);
	oos << getObject()->isOn << getObject()->isDirectional;
	oos << getObject()->position;
	oos << getObject()->color;
	oos.writeInt16(getObject()->falloff);
}

Serializable*
Light::Streamer::read(ObjectInputStream& ois) const
//[]---------------------------------------------------[]
//|  Read                                               |
//[]---------------------------------------------------[]
{
	readBaseObject((SceneComponent*)getObject(), ois);
	ois >> getObject()->isOn >> getObject()->isDirectional;
	ois >> getObject()->position;
	ois >> getObject()->color;
	getObject()->falloff = (Falloff)ois.readInt16();
	return getObject();
}

Light::Light(const Vec3& p, const Color& c):
	isOn(true),
	isDirectional(false),
	position(p),
	color(c),
	falloff(Infinite)
//[]----------------------------------------------------[]
//|  Constructor                                         |
//[]----------------------------------------------------[]
{
	// do nothing
}

Color
Light::getScaledColor(REAL t) const
//[]----------------------------------------------------[]
//|  Get scaled color                                    |
//[]----------------------------------------------------[]
{
	if (isDirectional || falloff == 0)
		return color;

	REAL f = Math::inverse<REAL>(t);

	if (falloff == Squared)
		f *= f;
	return color * f;
}

void
Light::getVector(const Vec3& P, Vec3& L, REAL& t) const
//[]----------------------------------------------------[]
//|  Get light vector                                    |
//[]----------------------------------------------------[]
{
	if (isDirectional)
	{
		L = position.versor();
		t = Math::infinity<REAL>();
	}
	else if (!Math::isZero(t = (L = position - P).length()))
		L *= Math::inverse<REAL>(t);
}
