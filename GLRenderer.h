#ifndef __GLRenderer_h
#define __GLRenderer_h

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
//  OVERVIEW: GLRenderer.h
//  ========
//  Class definition for GL renderer.

#ifndef __PolyRenderer_h
#include "PolyRenderer.h"
#endif
#ifndef __TriangleMesh_h
#include "TriangleMesh.h"
#endif

namespace Graphics
{ // begin namespace Graphics


//////////////////////////////////////////////////////////
//
// GLRenderer: GL renderer class
// ==========
class GLRenderer: public PolyRenderer
{
public:
	// Constructor
	GLRenderer(Scene&, Camera*);

protected:
	void startRender();
	void endRender();
	void renderWireframe();
	void renderPoly();

	void drawLine(const Vec3&, const Vec3&) const;

private:
	void setProjectionMatrix();
	void renderLights();

}; // GLRenderer

} // end namespace Graphics

#endif
