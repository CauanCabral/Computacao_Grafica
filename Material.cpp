//[]------------------------------------------------------------------------[]
//|                                                                          |
//|                          GVSG Graphics Library                           |
//|                               Version 1.0                                |
//|                                                                          |
//|                Copyright® 2007, Paulo Aristarco Pagliosa                 |
//|                All Rights Reserved.                                      |
//|                                                                          |
//[]------------------------------------------------------------------------[]
//
// OVERVIEW: Material.cpp
// ========
// Source file for material.

#ifndef __Material_h
#include "Material.h"
#endif

using namespace Graphics;


//////////////////////////////////////////////////////////
//
// Material implementation
// ========
IMPLEMENT_BLOCK_ALLOCABLE(Material, 10);
IMPLEMENT_SERIALIZABLE1(Material, NameableObject);

void
Material::Streamer::write(ObjectOutputStream& oos) const
//[]---------------------------------------------------[]
//|  Write                                              |
//[]---------------------------------------------------[]
{
	writeBaseObject((NameableObject*)getObject(), oos);
	oos << getObject()->surface;
}

Serializable*
Material::Streamer::read(ObjectInputStream& ois) const
//[]---------------------------------------------------[]
//|  Read                                               |
//[]---------------------------------------------------[]
{
	readBaseObject((NameableObject*)getObject(), ois);
	ois >> getObject()->surface;
	return getObject();
}

Material::Material(const String& name):
	NameableObject(name)
//[]---------------------------------------------------[]
//|  Constructor                                        |
//[]---------------------------------------------------[]
{
	setSurface(Color(255, 255, 255), Finish());
}

void
Material::setSurface(const Color& color, const Finish& finish)
//[]---------------------------------------------------[]
//|  Set surface                                        |
//[]---------------------------------------------------[]
{
	surface.ambient = color * finish.ambient;
	surface.diffuse = color * finish.diffuse;
	surface.shine = finish.shine;
	surface.spot = color * finish.spot;
	surface.specular = finish.specular;
	surface.transparency = finish.transparency;
	surface.IOR = finish.IOR;
}


//////////////////////////////////////////////////////////
//
// MaterialFactory implementation
// ===============
MaterialFactory::Materials MaterialFactory::materials;

Material*
MaterialFactory::New()
//[]---------------------------------------------------[]
//|  Create material                                    |
//[]---------------------------------------------------[]
{
	int id = materials.size();
	wchar_t name[16];

	swprintf(name, sizeof(name) / sizeof(wchar_t) - 1, L"mat%d", id);

	Material* material = new Material(name);

	add(material);
	return material;
}

Material*
MaterialFactory::New(const String& name)
//[]---------------------------------------------------[]
//|  Create material                                    |
//[]---------------------------------------------------[]
{
	Material* material = get(name);
	
	if (material == 0)
		add(material = new Material(name));
	return material;
}

Material*
MaterialFactory::get(const String& name)
//[]---------------------------------------------------[]
//|  Get material                                       |
//[]---------------------------------------------------[]
{
	for (MaterialIterator mit(materials); mit; ++mit)
		if (name == mit.current()->getName())
			return mit.current();
	return 0;
}

void
MaterialFactory::write(ObjectOutputStream& oos)
//[]---------------------------------------------------[]
//|  Write                                              |
//[]---------------------------------------------------[]
{
	int n = materials.size();

	oos << n;
	for (MaterialIterator mit(materials); mit;)
		oos << mit++;
}

void
MaterialFactory::read(ObjectInputStream& ois)
//[]---------------------------------------------------[]
//|  Read                                               |
//[]---------------------------------------------------[]
{
	int n;

	ois >> n;
	materials.clear(true);
	for (int i = 0; i < n; i++)
	{
		Material* temp;

		ois >> temp;
		add(temp);
	}
}
