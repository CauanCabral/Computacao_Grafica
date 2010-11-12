#ifndef __PolyRenderer_h
#define __PolyRenderer_h

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
//  OVERVIEW: PolyRenderer.h
//  ========
//  Class definition for poly renderer.

#ifndef __Renderer_h
#include "Renderer.h"
#endif
#ifndef __Utils_h
#include "Utils.h"
#endif

namespace Graphics
{ // begin namespace Graphics


//////////////////////////////////////////////////////////
//
// PolyRenderer: poly renderer class
// =============
class PolyRenderer: public Renderer
{
public:
	enum RenderMode
	{
		Wireframe = 1,
		HiddenLines = 2,
		Flat = 4,
		Gouraud = 8,
		Phong = 16
	};

	// Flags
	enum
	{
		useLights = 1,
		drawSceneBoundingBox = 2
	};

	RenderMode renderMode;
	Utils::Flags flags;

	// Constructor
	PolyRenderer(Scene& scene, Camera* camera):
		Renderer(scene, camera),
		renderMode(Gouraud)
	{
		flags.set(useLights | drawSceneBoundingBox);
	}

	void render();

protected:
	virtual void startRender();
	virtual void endRender();
	virtual void renderWireframe() = 0;
	virtual void renderPoly() = 0;

	virtual void drawLine(const Vec3&, const Vec3&) const = 0;
	virtual void drawAABB(const BoundingBox&) const;

	virtual Light* makeDefaultLight() const;

}; // PolyRenderer

} //end namespace Graphics

#endif
