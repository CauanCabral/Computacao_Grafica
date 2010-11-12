#ifndef __Light_h
#define __Light_h

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
//  OVERVIEW: Light.h
//  ========
//  Class definition for light.

#ifndef __BaseTypes_h
#include "BaseTypes.h"
#endif
#ifndef __DoubleList_h
#include "DoubleList.h"
#endif
#ifndef __SceneComponent_h
#include "SceneComponent.h"
#endif
#ifndef __Transform3_h
#include "Transform3.h"
#endif

namespace Graphics
{ // begin namespace Graphics


//////////////////////////////////////////////////////////
//
// Light: light class
// =====
class Light: public SceneComponent
{
public:
	enum Falloff
	{
		Infinite = 0,
		Linear,
		Squared
	};

	bool isOn;
	bool isDirectional;
	Vec3 position;
	Color color;
	Falloff falloff;

	// Constructor
	Light(const Vec3&, const Color& = Color::white);

	virtual Color getScaledColor(REAL) const;
	virtual void getVector(const Vec3&, Vec3&, REAL&) const;

	DECLARE_DOUBLE_LIST_ELEMENT(Light);
	DECLARE_SERIALIZABLE(Light);

	friend class Scene;

}; // Light

typedef DoubleListImp<Light> Lights;
typedef DoubleListIteratorImp<Light> LightIterator;

} // end namespace Graphics

#endif
