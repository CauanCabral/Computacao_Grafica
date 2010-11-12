#ifndef __SceneComponent_h
#define __SceneComponent_h

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
//  OVERVIEW: SceneComponent.h
//  ========
//  Class definition for scene component.

#ifndef __NameableObject_h
#include "NameableObject.h"
#endif

using namespace System;

namespace Graphics
{ // begin namespace Graphics

//
// Forward definition
//
class Scene;


//////////////////////////////////////////////////////////
//
// SceneComponent: scene component class
// ==============
class SceneComponent: public NameableObject
{
public:
	Scene* getScene() const
	{
		return scene;
	}

	// Destructor
	virtual ~SceneComponent();

protected:
	Scene* scene;

	// Protected constructor
	SceneComponent():
		scene(0)
	{
		// do nothing
	}

	DECLARE_ABSTRACT_SERIALIZABLE(SceneComponent);

	friend class Scene;

}; // SceneComponent

} // end namespace Graphics

#endif
