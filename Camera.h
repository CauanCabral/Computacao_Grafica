#ifndef __Camera_h
#define __Camera_h

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
//  OVERVIEW: Camera.h
//  ========
//  Class definition for camera.

#ifndef __NameableObject_h
#include "NameableObject.h"
#endif
#ifndef __Transform3_h
#include "Transform3.h"
#endif

using namespace System;

namespace Graphics
{ // begin namespace Graphics

#define MIN_HEIGHT      (REAL)0.1
#define MIN_ASPECT      (REAL)0.1
#define MIN_DISTANCE    (REAL)0.2
#define MIN_ANGLE       (REAL)1
#define MAX_ANGLE       (REAL)179
#define MIN_DEPTH       (REAL)0.1
#define MIN_FRONT_PLANE (REAL)0.1


//////////////////////////////////////////////////////////
//
// Camera: camera class
// ======
class Camera: public NameableObject
{
public:
	enum ProjectionType
	{
		Parallel,
		Perspective
	};

	// Constructors
	Camera();
	Camera(ProjectionType,
			const Vec3&,
			const Vec3&,
			const Vec3&,
			REAL,
			REAL = 1.0f);

	Vec3 getPosition() const;
	Vec3 getDirectionOfProjection() const;
	Vec3 getFocalPoint() const;
	Vec3 getViewUp() const;
	Vec3 getViewPlaneNormal() const;
	ProjectionType getProjectionType() const;
	REAL getDistance() const;
	REAL getViewAngle() const;
	REAL getHeight() const;
	REAL getAspectRatio() const;
	void getClippingPlanes(REAL&, REAL&) const;
	REAL windowHeight() const;

	void setPosition(const Vec3&);
	void setDirectionOfProjection(const Vec3&);
	void setViewUp(const Vec3&);
	void setProjectionType(ProjectionType);
	void setDistance(REAL);
	void setViewAngle(REAL);
	void setHeight(REAL);
	void setAspectRatio(REAL);
	void setClippingPlanes(REAL, REAL);

	void setDefaultView();
	uint updateView();

	uint getTimestamp() const;

	void azimuth(REAL);
	void elevation(REAL);
	void rotateYX(REAL, REAL);
	void roll(REAL);
	void yaw(REAL);
	void pitch(REAL);
	void zoom(REAL);
	void move(REAL, REAL, REAL);
	void move(const Vec3&);

	Vec3 worldToView(const Vec3&) const;
	Vec3 viewToWorld(const Vec3&) const;

	void print() const;

protected:
	Vec3 position;
	Vec3 directionOfProjection;
	Vec3 focalPoint;
	Vec3 viewUp;
	ProjectionType projectionType;
	REAL distance;
	REAL viewAngle;
	REAL height;
	REAL aspectRatio;
	REAL F;
	REAL B;
	bool viewModified;
	uint timestamp;
	Transf3 VTM; // view transform matrix

	void updateFocalPoint();
	void updateDOP();

	friend class Renderer;

}; // Camera


//////////////////////////////////////////////////////////
//
// Camera inline implementation
// ======
inline
Camera::Camera():
	timestamp(0)
{
	setDefaultView();
}

inline Vec3
Camera::getPosition() const
{
	return position;
}

inline Vec3
Camera::getDirectionOfProjection() const
{
	return directionOfProjection;
}

inline Vec3
Camera::getFocalPoint() const
{
	return focalPoint;
}

inline Vec3
Camera::getViewUp() const
{
	return viewUp;
}

inline Vec3
Camera::getViewPlaneNormal() const
{
	return -directionOfProjection;
}

inline Camera::ProjectionType
Camera::getProjectionType() const
{
	return projectionType;
}

inline REAL
Camera::getDistance() const
{
	return distance;
}

inline REAL
Camera::getViewAngle() const
{
	return viewAngle;
}

inline REAL
Camera::getHeight() const
{
	return height;
}

inline REAL
Camera::getAspectRatio() const
{
	return aspectRatio;
}

inline void
Camera::getClippingPlanes(REAL& F, REAL& B) const
{
	F = this->F;
	B = this->B;
}

inline uint
Camera::getTimestamp() const
{
	return timestamp;
}

inline void
Camera::move(const Vec3& d)
{
	if (d.isZero())
		return;
	move(d.x, d.y, d.z);
}

inline Vec3
Camera::worldToView(const Vec3& p) const
{
	return VTM.transform(p);
}

inline REAL
Camera::windowHeight() const
{
	return projectionType == Parallel ?
		height :
		2 * distance * (REAL)tan(Math::toRadians<REAL>(viewAngle) * 0.5f);
}

} // end namespace Graphics

#endif
