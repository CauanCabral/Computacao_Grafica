#ifndef __Scene_h
#define __Scene_h

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
//  OVERVIEW: Scene.h
//  ========
//  Class definition for scene.

#ifndef __Actor_h
#include "Actor.h"
#endif
#ifndef __Light_h
#include "Light.h"
#endif

namespace Graphics
{ // begin namespace Graphics


//////////////////////////////////////////////////////////
//
// Scene: scene class
// =====
class Scene: public NameableObject
{
public:
	Color backgroundColor;
	Color ambientLight;

	// Constructor
	Scene(const wchar_t* name = 0):
		NameableObject(name),
		backgroundColor(Color::black),
		ambientLight(Color::gray),
		IOR(1)
	{
		// do nothing
	}

	// Destructor
	virtual ~Scene();

	REAL getIOR() const
	{
		return IOR;
	}

	void setIOR(REAL ior)
	{
		IOR = ior > 0 ? ior : 0;
	}

	int getNumberOfActors() const
	{
		return actors.size();
	}

	ActorIterator getActorIterator() const
	{
		return ActorIterator(actors);
	}

	int getNumberOfLights() const
	{
		return lights.size();
	}

	LightIterator getLightIterator() const
	{
		return LightIterator(lights);
	}

	void addActor(Actor*);
	void deleteActor(Actor*);
	void deleteActors();
	void addLight(Light*);
	void deleteLight(Light*);
	void deleteLights();

	void deleteAll()
	{
		deleteActors();
		deleteLights();
	}

	Actor* findActor(Model*) const;

	BoundingBox getBoundingBox() const
	{
		return boundingBox;
	}

	BoundingBox computeBoundingBox();

protected:
	BoundingBox boundingBox;
	REAL IOR;
	// Scene components
	Actors actors;
	Lights lights;

	DECLARE_SERIALIZABLE(Scene);

}; // Scene

} // end namespace Graphics

#endif // __Scene_h
