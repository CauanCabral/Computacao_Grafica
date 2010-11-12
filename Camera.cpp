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
//  OVERVIEW: Camera.cpp
//  ========
//  Source file for camera.

#ifndef __Camera_h
#include "Camera.h"
#endif

using namespace Graphics;

//
// Auxiiary function
//
inline void
error(const wchar_t* msg)
{
	throw Exception(msg);
}


//////////////////////////////////////////////////////////
//
// Camera implementation
// ======
inline void
Camera::updateFocalPoint()
{
	focalPoint = position + directionOfProjection * distance;
	viewModified = true;
}

inline void
Camera::updateDOP()
{
	directionOfProjection = (focalPoint - position) * (REAL)(1 / distance);
	viewModified = true;
}

Camera::Camera(ProjectionType projectionType,
		const Vec3& position,
		const Vec3& dop,
		const Vec3& viewUp,
		REAL angle,
		REAL aspect)
//[]---------------------------------------------------[]
//|  Constructor                                        |
//[]---------------------------------------------------[]
{
	this->projectionType = projectionType;
	this->position = position;
	distance = dop.length();
	if (distance < MIN_DISTANCE)
		distance = MIN_DISTANCE;
	directionOfProjection = dop * Math::inverse<REAL>(distance);
	focalPoint = position + dop;
	this->viewUp = viewUp;
	if (angle < MIN_ANGLE)
		angle = MIN_ANGLE;
	else if (angle > MAX_ANGLE)
		angle = MAX_ANGLE;
	viewAngle = angle;
	height = 2 * distance * (REAL)tan(Math::toRadians<REAL>(angle) * 0.5f);
	aspectRatio = aspect < MIN_ASPECT ? MIN_ASPECT : aspect;
	F = (REAL)0.1;
	B = (REAL)1000.01;
	VTM.identity();
	timestamp = 0;
	viewModified = true;
}

void
Camera::setPosition(const Vec3& value)
//[]---------------------------------------------------[]
//|  Set the camera's position.                         |
//|                                                     |
//|  Setting the camera's position will not change      |
//|  neither the direction of projection nor the        |
//|  distance between the position and the focal point. |
//|  The focal point will be moved along the direction  |
//|  of projection.                                     |
//[]---------------------------------------------------[]
{
	if (position != value)
	{
		position = value;
		updateFocalPoint();
	}
}

void
Camera::setDirectionOfProjection(const Vec3& value)
//[]---------------------------------------------------[]
//|  Set the direction of projection.                   |
//|                                                     |
//|  Setting the direction of projection will not       |
//|  change the distance between the position and the   |
//|  focal point. The focal point will be moved along   |
//|  the direction of projection.                       |
//[]---------------------------------------------------[]
{
	if (value.isZero())
		error(L"Direction of projection cannot be null");

	Vec3 dop = value.versor();

	if (directionOfProjection != dop)
	{
		directionOfProjection = dop;
		updateFocalPoint();
	}
}

void
Camera::setViewUp(const Vec3& value)
//[]---------------------------------------------------[]
//|  Set the camera's view up.                          |
//[]---------------------------------------------------[]
{
	if (value.isZero())
		error(L"View up cannot be null");
	if (directionOfProjection.cross(value).isZero())
		error(L"View up cannot be parallel to DOP");

	Vec3 vup = value.versor();

	if (viewUp != vup)
	{
		viewUp = vup;
		viewModified = true;
	}
}

void
Camera::setProjectionType(ProjectionType value)
//[]---------------------------------------------------[]
//|  Set the camera's projection type                   |
//[]---------------------------------------------------[]
{
	if (projectionType != value)
	{
		projectionType = value;
		viewModified = true;
	}
}

void
Camera::setDistance(REAL value)
//[]---------------------------------------------------[]
//|  Set the camera's distance.                         |
//|                                                     |
//|  Setting the distance between the position and      |
//|  focal point will move the focal point along the    |
//|  direction of projection.                           |
//[]---------------------------------------------------[]
{
	if (value <= 0)
		error(L"Distance must be positive");
	if (!Math::isEqual(distance, value))
	{
		distance = Math::max(value, MIN_DISTANCE);
		updateFocalPoint();
	}
}

void
Camera::setViewAngle(REAL value)
//[]---------------------------------------------------[]
//|  Set the camera's view angle.                       |
//[]---------------------------------------------------[]
{
	if (value <= 0)
		error(L"View angle must be positive");
	if (!Math::isEqual(viewAngle, value))
	{
		viewAngle = Math::min(Math::max(value, MIN_ANGLE), MAX_ANGLE);
		if (projectionType == Perspective)
			viewModified = true;
	}
}

void
Camera::setHeight(REAL value)
//[]---------------------------------------------------[]
//|  Set the camera's view height.                      |
//[]---------------------------------------------------[]
{
	if (value <= 0)
		error(L"Height of the view window must be positive");
	if (!Math::isEqual(height, value))
	{
		height = Math::max(value, MIN_HEIGHT);
		if (projectionType == Parallel)
			viewModified = true;
	}
}

void
Camera::setAspectRatio(REAL value)
//[]---------------------------------------------------[]
//|  Set the camera's aspect ratio.                     |
//[]---------------------------------------------------[]
{
	if (value <= 0)
		error(L"Aspect ratio must be positive");
	if (!Math::isEqual(aspectRatio, value))
	{
		aspectRatio = Math::max(value, MIN_ASPECT);
		viewModified = true;
	}
}

void
Camera::setClippingPlanes(REAL F, REAL B)
//[]---------------------------------------------------[]
//|  Set the distance of the clippling planes.          |
//[]---------------------------------------------------[]
{
	if (F <= 0 || B <= 0)
		error(L"Clipping plane distance must be positive");
	if (F > B)
	{
		REAL temp = F;

		F = B;
		B = temp;
	}
	if (F < MIN_FRONT_PLANE)
		F = MIN_FRONT_PLANE;
	if ((B - F) < MIN_DEPTH)
		B = F + MIN_DEPTH;
	if (!Math::isEqual(this->F, F) || !Math::isEqual(this->B, B))
	{
		this->F = F;
		this->B = B;
		viewModified = true;
	}
}

void
Camera::azimuth(REAL angle)
//[]---------------------------------------------------[]
//|  Azimuth.                                           |
//|                                                     |
//|  Rotate the camera's position about the view up     |
//|  vector centered at the focal point.                |
//[]---------------------------------------------------[]
{
	if (!Math::isZero(angle))
	{
		Transf3 r;

		r.rotation(focalPoint, viewUp, Math::toRadians<REAL>(angle));
		r.transformRef(position);
		updateDOP();
	}
}

void
Camera::elevation(REAL angle)
//[]---------------------------------------------------[]
//|  Elevation.                                         |
//|                                                     |
//|  Rotate the camera's position about the cross       |
//|  product of the view plane normal and the view up   |
//|  vector centered at the focal point.                |
//[]---------------------------------------------------[]
{
	if (!Math::isZero(angle))
	{
		Vec3 axis = directionOfProjection.cross(viewUp);
		Transf3 r;

		r.rotation(focalPoint, axis, Math::toRadians<REAL>(angle));
		r.transformRef(position);
		updateDOP();
		viewUp = axis.cross(directionOfProjection);
	}
}

void
Camera::rotateYX(REAL ay, REAL ax)
//[]---------------------------------------------------[]
//|  Rotate YX.                                         |
//|                                                     |
//|  Composition of an azimuth of ay with an elavation  |
//|  of ax (in degrees).                                |
//[]---------------------------------------------------[]
{
	if (Math::isZero(ay))
	{
		elevation(ax);
		return;
	}

	Transf3 r;
	
	r.rotation(focalPoint, viewUp, Math::toRadians<REAL>(ay));
	if (!Math::isZero(ax))
	{
		Vec3 axis = directionOfProjection.cross(viewUp);
		Transf3 q;
		
		q.rotation(focalPoint, axis, Math::toRadians<REAL>(ax));
		q.transformVectorRef(viewUp);
		r.compose(q);
	}
	r.transformRef(position);
	updateDOP();
}

void
Camera::roll(REAL angle)
//[]---------------------------------------------------[]
//|  Roll.                                              |
//|                                                     |
//|  Rotate the view up vector around the view plane    |
//|  normal                                             |
//[]---------------------------------------------------[]
{
	if (!Math::isZero(angle))
	{
		Transf3 r;

		r.rotation(directionOfProjection, -Math::toRadians<REAL>(angle));
		r.transformVectorRef(viewUp);
		viewModified = true;
	}
}

void
Camera::yaw(REAL angle)
//[]---------------------------------------------------[]
//|  Yaw.                                               |
//|                                                     |
//|  Rotate the focal point about the view up vector    |
//|  centered at the camera's position.                 |
//[]---------------------------------------------------[]
{
	if (!Math::isZero(angle))
	{
		Transf3 r;

		r.rotation(position, viewUp, Math::toRadians<REAL>(angle));
		r.transformRef(focalPoint);
		updateDOP();
	}
}

void
Camera::pitch(REAL angle)
//[]---------------------------------------------------[]
//|  Pitch.                                             |
//|                                                     |
//|  Rotate the focal point about the cross product of  |
//|  the view up vector and the view plane normal       |
//|  centered at the camera's position.                 |
//[]---------------------------------------------------[]
{
	if (!Math::isZero(angle))
	{
		Vec3 axis = directionOfProjection.cross(viewUp);
		Transf3 r;

		r.rotation(position, axis, Math::toRadians<REAL>(angle));
		r.transformRef(focalPoint);
		updateDOP();
		viewUp = axis.cross(directionOfProjection);
	}
}

void
Camera::zoom(REAL zoom)
//[]---------------------------------------------------[]
//|  Zoom.                                              |
//|                                                     |
//|  Change the view angle (or height) of the camera so |
//|  that more or less of a scene occupies the view     |
//|  window.  A value > 1 is a zoom-in. A value < 1 is  |
//|  zoom-out.                                          |
//[]---------------------------------------------------[]
{
	if (zoom > 0)
		if (projectionType == Perspective)
			setViewAngle(viewAngle / zoom);
		else
			setHeight(height / zoom);
}

void
Camera::move(REAL dx, REAL dy, REAL dz)
//[]---------------------------------------------------[]
//|  Move the camera.                                   |
//[]---------------------------------------------------[]
{
	if (!Math::isZero(dx))
		position += directionOfProjection.cross(viewUp) * dx;
	if (!Math::isZero(dy))
		position += viewUp * dy;
	if (!Math::isZero(dz))
		position -= directionOfProjection * dz;
	updateFocalPoint();
}

void
Camera::setDefaultView()
//[]---------------------------------------------------[]
//|  Set default view.                                  |
//[]---------------------------------------------------[]
{
	position.set(0, 0, 1);
	directionOfProjection.set(0, 0, -1);
	focalPoint.set(0, 0, 0);
	distance = 1;
	viewUp.set(0, 1, 0);
	projectionType = Perspective;
	viewAngle = 90;
	height = 2; // = 2 * tan(viewAngle / 2) * distance
	aspectRatio = 1;
	F = (REAL)0.1;
	B = (REAL)1000.01;
	VTM.identity();
	viewModified = true;
}

Vec3
Camera::viewToWorld(const Vec3& p) const
//[]---------------------------------------------------[]
//|  Transform view coordinates in world coordinates.   |
//[]---------------------------------------------------[]
{
	Transf3 invVTM;

	VTM.inverse(invVTM);
	return invVTM.transform(p);
}

//
// Auxiliary struct
//
struct VRC
{
	Vec3 O;
	Vec3 u;
	Vec3 v;
	Vec3 n;

	VRC(const Vec3& VRP, const Vec3& VPN, const Vec3& VUP)
	{
		O = VRP;
		n = VPN;
		u = VUP.cross(n).versor();
		v = VPN.cross(u);
	}

	void setViewTransform(Transf3& VTM)
	{
		VTM.setRow(0, u.x, u.y, u.z, -(O.inner(u)));
		VTM.setRow(1, v.x, v.y, v.z, -(O.inner(v)));
		VTM.setRow(2, n.x, n.y, n.z, -(O.inner(n)));
	}

}; // VRC

uint
Camera::updateView()
//[]---------------------------------------------------[]
//|  Update VTM.                                        |
//[]---------------------------------------------------[]
{
	if (viewModified)
	{
		VRC VRC(focalPoint, -directionOfProjection, viewUp);

		VRC.setViewTransform(VTM);
		// viewUp = VRC.v;
		// TODO
		viewModified = false;
		timestamp++;
	}
	return timestamp;
}

void
Camera::print() const
//[]---------------------------------------------------[]
//|  Print camera                                       |
//[]---------------------------------------------------[]
{
	static const char* projectionName[] = { "parallel", "perspective" };

	printf("Camera name: \"%S\"\n", (const wchar_t*)getName());
	printf("Projection type: %s\n", projectionName[projectionType]);
	position.print("Position: ");
	directionOfProjection.print("Direction of projection: ");
	printf("Distance: %f\n", distance);
	viewUp.print("View up: ");
	printf("View angle/height: %f/%f\n", viewAngle, height);
}
