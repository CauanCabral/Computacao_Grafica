#ifndef __Material_h
#define __Material_h

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
// OVERVIEW: Material.h
// ========
// Class definition for material.

#ifndef __Array_h
#include "Array.h"
#endif
#ifndef __BaseTypes_h
#include "BaseTypes.h"
#endif
#ifndef __NameableObject_h
#include "NameableObject.h"
#endif

using namespace System;

namespace Graphics
{ // begin namespace Graphics


//////////////////////////////////////////////////////////
//
// Finish: finish class
// ======
class Finish
{
public:
	// Constructor
	Finish():
		ambient(0.2f),
		diffuse(0.8f),
		shine(0),
		spot(0),
		specular(Color::black),
		transparency(Color::black),
		IOR(1)
	{
		// do nothing
	}

	float ambient;
	float diffuse;
	float shine;
	float spot;
	Color specular;
	Color transparency;
	float IOR;

}; // Finish


//////////////////////////////////////////////////////////
//
// Material: material class
// ========
class Material: public NameableObject
{
public:
	class Surface
	{
	public:
		Color ambient;        // ambient color
		Color diffuse;        // diffuse color
		float shine;          // specular spot exponent
		Color spot;           // specular spot color
		Color specular;       // specular color
		Color transparency;   // transparency color
		float IOR;            // index of refraction

	}; // Surface

	Surface surface;

	Material* makeUse()
	{
		Object::makeUse();
		return this;
	}

	int getIndex() const
	{
		return index;
	}

	static Material* getDefault();

	void setSurface(const Color&, const Finish&);

protected:
	// Constructor
	Material(const String&);

private:
	int index;

	DECLARE_BLOCK_ALLOCABLE(Material);
	DECLARE_SERIALIZABLE(Material);

	friend class MaterialFactory;

}; // Material

typedef PointerArrayIterator<Material> MaterialIterator;


//////////////////////////////////////////////////////////
//
// MaterialFactory: material factory class
// ===============
class MaterialFactory
{
public:
	class Materials: public PointerArray<Material>
	{
	public:
		Material* defaultMaterial;

		// Constructor
		Materials()
		{
			defaultMaterial = MaterialFactory::New(L"default");
		}

	}; // Materials

	static Material* New();
	static Material* New(const String&);

	static Material* get(const String&);

	static Material* get(int id)
	{
		return materials[id];
	}

	static Material* getDefaultMaterial()
	{
		return materials.defaultMaterial;
	}

	static int size()
	{
		return materials.size();
	}

	static MaterialIterator iterator()
	{
		return MaterialIterator(materials);
	}

	static void write(ObjectOutputStream&);
	static void read(ObjectInputStream&);

private:
	static Materials materials;

	static void add(Material* material)
	{
		material->index = materials.size();
		materials.add(material->makeUse());
	}

}; // MaterialFactory


//////////////////////////////////////////////////////////
//
// Material inline implementtaion
// ========
inline Material*
Material::getDefault()
{
	return MaterialFactory::getDefaultMaterial();
}

//
// Serialization
//
inline ObjectOutputStream&
operator <<(ObjectOutputStream& oos, const Material::Surface& s)
{
	oos << s.ambient << s.diffuse;
	oos << s.shine << s.spot << s.specular;
	oos << s.transparency;
	return oos;
}

inline ObjectInputStream&
operator >>(ObjectInputStream& ois, Material::Surface& s)
{
	ois >> s.ambient >> s.diffuse;
	ois >> s.shine >> s.spot >> s.specular;
	ois >> s.transparency;
	return ois;
}

} // end namespace Graphics

#endif // __Material_h
