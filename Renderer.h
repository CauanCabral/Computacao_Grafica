#ifndef __Renderer_h
#define __Renderer_h

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
//  OVERVIEW: Renderer.h
//  ========
//  Class definition for generic renderer.

#ifndef __Camera_h
#include "Camera.h"
#endif
#ifndef __Scene_h
#include "Scene.h"
#endif
#ifndef __Viewport_h
#include "Viewport.h"
#endif

namespace Graphics
{ // begin namespace Graphics


//////////////////////////////////////////////////////////
//
// Renderer: generic renderer class
// ========
class Renderer
{
	friend class RenderWindow;

public:
	// Constructors
	Renderer()
	{
		// do nothing
	}
	Renderer(Scene&, Camera* = 0);

	// Destructor
	virtual ~Renderer();

	Scene* getScene() const
	{
		return scene;
	}

	Camera* getCamera() const
	{
		return camera;
	}

	void setScene(Scene&);
	void setCamera(Camera*);
	void setImageSize(int, int);

	Vec3 worldToView(const Vec3& p) const
	{
		return VTM.transform(p);
	}

	Vec3 viewToWorld(const Vec3& p) const
	{
		return invVTM.transform(p);
	}

	Vec3 backToWC(int, int) const;

	Vec3 backToWC(const DCPoint& p) const
	{
		return backToWC(p.x, p.y);
	}

	DCPoint map(const Vec3&) const;

	DCPoint map(REAL x, REAL y, REAL z) const
	{
		return map(Vec3(x, y, z));
	}

	virtual void updateView();
	virtual void render() = 0;

protected:
	Scene* scene;
	Camera* camera;
	Transf3 VTM;
	Transf3 invVTM;
	int W;
	int H;

	Light* makeDefaultLight() const;
	Vec3 project(const Vec3&) const;

private:
	Camera* defaultCamera;
	REAL sx3;
	REAL tx3;
	REAL sy3;
	REAL ty3;

}; // Renderer

} // end namespace Graphics

#endif
