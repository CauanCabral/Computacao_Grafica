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
//  OVERVIEW: Scene.cpp
//  ========
//  Source file for scene.

#ifndef __Scene_h
#include "Scene.h"
#endif

using namespace Graphics;


//////////////////////////////////////////////////////////
//
// Scene implementation
// =====
IMPLEMENT_SERIALIZABLE1(Scene, NameableObject);

void
Scene::Streamer::write(ObjectOutputStream& oos) const
//[]---------------------------------------------------[]
//|  Write                                              |
//[]---------------------------------------------------[]
{
	MaterialFactory::write(oos);
	writeBaseObject((NameableObject*)getObject(), oos);
	oos << getObject()->backgroundColor;
	oos << getObject()->ambientLight;
	oos << getObject()->IOR;
	oos << getObject()->actors.size();
	for (ActorIterator ait(getObject()->actors); ait;)
		oos << ait++;
	oos << getObject()->lights.size();
	for (LightIterator lit(getObject()->lights); lit;)
		oos << lit++;
}

Serializable*
Scene::Streamer::read(ObjectInputStream& ois) const
//[]---------------------------------------------------[]
//|  Read                                               |
//[]---------------------------------------------------[]
{
	MaterialFactory::read(ois);
	readBaseObject((NameableObject*)getObject(), ois);
	ois >> getObject()->backgroundColor;
	ois >> getObject()->ambientLight;
	ois >> getObject()->IOR;

	int size;

	ois >> size;
	for (int i = 0; i < size; i++)
	{
		Actor* a;

		ois >> a;
		getObject()->addActor(a);
	}
	ois >> size;
	for (int i = 0; i < size; i++)
	{
		Light* l;

		ois >> l;
		getObject()->addLight(l);
	}
	return getObject();
}

Scene::~Scene()
//[]---------------------------------------------------[]
//|  Destructor                                         |
//[]---------------------------------------------------[]
{
	deleteAll();
}

void
Scene::addActor(Actor* actor)
//[]---------------------------------------------------[]
//|  Add actor                                          |
//[]---------------------------------------------------[]
{
	if (actor != 0)
	{
		actors.add(actor);
		actor->scene = this;
		actor->makeUse();
		boundingBox.inflate(actor->model->getBoundingBox());
	}
}

void
Scene::deleteActor(Actor* actor)
//[]---------------------------------------------------[]
//|  Delete actor                                       |
//[]---------------------------------------------------[]
{
	if (actor != 0 && actor->getScene() == this)
	{
		actors.remove(*actor);
		actor->scene = 0;
		actor->release();
	}
}

void
Scene::addLight(Light* light)
//[]---------------------------------------------------[]
//|  Add light                                          |
//[]---------------------------------------------------[]
{
	if (light != 0)
	{
		lights.add(light);
		light->scene = this;
		light->makeUse();
	}
}

void
Scene::deleteLight(Light* light)
//[]---------------------------------------------------[]
//|  Delete light                                       |
//[]---------------------------------------------------[]
{
	if (light != 0 && light->getScene() == this)
	{
		lights.remove(*light);
		light->scene = 0;
		light->release();
	}
}

void
Scene::deleteActors()
//[]---------------------------------------------------[]
//|  Delete all actors                                  |
//[]---------------------------------------------------[]
{
	for (Actor* actor; (actor = actors.peekHead()) != 0;)
	{
		actors.remove(*actor);
		actor->scene = 0;
		actor->release();
	}
	boundingBox.setEmpty();
}

void
Scene::deleteLights()
//[]---------------------------------------------------[]
//|  Delete all lights                                  |
//[]---------------------------------------------------[]
{
	for (Light* light; (light = lights.peekHead()) != 0;)
	{
		lights.remove(*light);
		light->scene = 0;
		light->release();
	}
}

Actor*
Scene::findActor(Model* model) const
//[]---------------------------------------------------[]
//|  Find actor                                         |
//[]---------------------------------------------------[]
{
	for (Actor* actor = actors.peekHead(); actor != 0; actor = actor->next)
		if (actor->model == model)
			return actor;
	return 0;
}

BoundingBox
Scene::computeBoundingBox()
//[]---------------------------------------------------[]
//|  Compute bounding box                               |
//[]---------------------------------------------------[]
{
	boundingBox.setEmpty();
	for (Actor* actor = actors.peekHead(); actor != 0; actor = actor->next)
		boundingBox.inflate(actor->model->getBoundingBox());
	return boundingBox;
}
