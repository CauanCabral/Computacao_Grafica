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
//  OVERVIEW: PolyRenderer.cpp
//  ========
//  Source file for poly renderer.

#ifndef __PolyRenderer_h
#include "PolyRenderer.h"
#endif

using namespace Graphics;


//////////////////////////////////////////////////////////
//
// PolyRenderer implementation
// ============
Light*
PolyRenderer::makeDefaultLight() const
{
	Vec3 p = viewToWorld((Vec3(1, 1, 1))).versor() * camera->getDistance();
	return new Light(p, Color::gray);
}

void
PolyRenderer::startRender()
{
	// do nothing
}

void
PolyRenderer::endRender()
{
	// do nothing
}

void
PolyRenderer::render()
{
	startRender();
	if (renderMode == Wireframe)
		renderWireframe();
	else if (scene->getNumberOfLights() != 0)
		renderPoly();
	else
	{
		Light* light = makeDefaultLight();

		scene->addLight(light);
		renderPoly();
		scene->deleteLight(light);
	}
	endRender();
}

void
PolyRenderer::drawAABB(const BoundingBox& box) const
{
	Vec3 p1 = box.getP1();
	Vec3 p7 = box.getP2();
	Vec3 p2(p7.x, p1.y, p1.z);
	Vec3 p3(p7.x, p7.y, p1.z);
	Vec3 p4(p1.x, p7.y, p1.z);
	Vec3 p5(p1.x, p1.y, p7.z);
	Vec3 p6(p7.x, p1.y, p7.z);
	Vec3 p8(p1.x, p7.y, p7.z);

	drawLine(p1, p2);
	drawLine(p2, p3);
	drawLine(p3, p4);
	drawLine(p1, p4);
	drawLine(p5, p6);
	drawLine(p6, p7);
	drawLine(p7, p8);
	drawLine(p5, p8);
	drawLine(p3, p7);
	drawLine(p2, p6);
	drawLine(p4, p8);
	drawLine(p1, p5);
}
