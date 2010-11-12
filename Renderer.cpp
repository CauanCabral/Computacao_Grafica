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
//  OVERVIEW: Renderer.cpp
//  ========
//  Source file for generic renderer.

#ifndef __Renderer_h
#include "Renderer.h"
#endif

using namespace Graphics;

//
// Canonical view volume
//
#define CVVX1 (REAL)-1.0
#define CVVX2 (REAL)+1.0
#define CVVY1 (REAL)-1.0
#define CVVY2 (REAL)+1.0

#define DFL_IMAGE_W 400
#define DFL_IMAGE_H 400


//////////////////////////////////////////////////////////
//
// Renderer implementation
// ========
Renderer::Renderer(Scene& aScene, Camera* aCamera):
	scene(0),
	defaultCamera(0)
//[]---------------------------------------------------[]
//|  Constructor                                        |
//[]---------------------------------------------------[]
{
	// Note: set camera first
	camera = aCamera != 0 ? aCamera : defaultCamera = new Camera();
	setScene(aScene);
	VTM.identity(); invVTM.identity();
}

Renderer::~Renderer()
//[]---------------------------------------------------[]
//|  Destructor                                         |
//[]---------------------------------------------------[]
{
	scene->release();
	delete defaultCamera;
}

Light*
Renderer::makeDefaultLight() const
//[]---------------------------------------------------[]
//|  Make default light                                 |
//[]---------------------------------------------------[]
{
	return new Light(camera->getPosition());
}

void
Renderer::setScene(Scene& scene)
//[]---------------------------------------------------[]
//|  Set scene                                          |
//[]---------------------------------------------------[]
{
	if (this->scene != &scene)
	{
		release(this->scene);
		this->scene = &scene;
		if (scene.getNumberOfLights() == 0)
			scene.addLight(makeDefaultLight());
		scene.makeUse();
	}
}

void
Renderer::setCamera(Camera* camera)
//[]---------------------------------------------------[]
//|  Set camera                                         |
//[]---------------------------------------------------[]
{
	if (camera == this->camera)
		return;
	if (camera != 0)
		this->camera = camera;
	else
	{
		if (defaultCamera == 0)
			defaultCamera = new Camera();
		this->camera = defaultCamera;
	}
}

void
Renderer::setImageSize(int w, int h)
//[]---------------------------------------------------[]
//|  Set imagem size                                    |
//[]---------------------------------------------------[]
{
	tx3 = -(sx3 = ((W = w) - 1) / (CVVX2 - CVVX1)) * CVVX1;
	ty3 = +(sy3 = ((H = h) - 1) / (CVVY2 - CVVY1)) * CVVY2;
}

Vec3
Renderer::backToWC(int x, int y) const
//[]---------------------------------------------------[]
//|  DC to WC point                                     |
//[]---------------------------------------------------[]
{
	Vec3 p((x - tx3) / sx3, (ty3 - y) / sy3, 0);
	return viewToWorld(p);
}

inline Vec3
Renderer::project(const Vec3& p) const
//[]---------------------------------------------------[]
//|  Project point                                      |
//[]---------------------------------------------------[]
{
	// TODO
	return Vec3(p.x, p.y, 0);
}

DCPoint
Renderer::map(const Vec3& p) const
//[]---------------------------------------------------[]
//|  Map point                                          |
//[]---------------------------------------------------[]
{
	Vec3 p1(project(p));
	return DCPoint((int)(p1.x * sx3 + tx3 + .5), (int)(ty3 - p1.y * sy3 + .5));
}

void
Renderer::updateView()
//[]---------------------------------------------------[]
//|  Update view                                        |
//[]---------------------------------------------------[]
{
	if (!camera->viewModified)
		return;
	camera->timestamp++;

	REAL w =  camera->height * camera->aspectRatio;
	Vec3 O =  camera->focalPoint;
	Vec3 n = -camera->directionOfProjection;
	Vec3 u =  camera->viewUp.cross(n).versor();
	Vec3 v =  n.cross(u);

	camera->viewUp = v;
	if (camera->projectionType == Camera::Perspective)
	{
		// TODO
	}
	else
	{
		REAL sx = (CVVX2 - CVVX1) / w;
		REAL sy = (CVVY2 - CVVY1) / camera->height;

		VTM.setRow(_X, u.x * sx, u.y * sx, u.z * sx, -(O * u * sx));
		VTM.setRow(_Y, v.x * sy, v.y * sy, v.z * sy, -(O * v * sy));
		VTM.setRow(_Z, n.x     , n.y     , n.z     , -(O * n));

		REAL invSx = Math::inverse<REAL>(sx);
		REAL invSy = Math::inverse<REAL>(sy);

		invVTM.setRow(_X, u.x * invSx, v.x * invSy, n.x, O.x);
		invVTM.setRow(_Y, u.y * invSx, v.y * invSy, n.y, O.y);
		invVTM.setRow(_Z, u.z * invSx, v.z * invSy, n.z, O.z);
	}
	camera->viewModified = false;
}
