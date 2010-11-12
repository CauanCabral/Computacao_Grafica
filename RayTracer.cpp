//[]------------------------------------------------------------------------[]
//|                                                                          |
//|                        GVSG Foundation Classes                           |
//|                               Version 1.0                                |
//|                                                                          |
//|                Copyright® 2007-2009, Paulo Aristarco Pagliosa            |
//|                All Rights Reserved.                                      |
//|                                                                          |
//[]------------------------------------------------------------------------[]
//
//  OVERVIEW: RayTracer.cpp
//  ========
//  Source code for simple ray tracer.

#include <stdlib.h>

#ifndef __RayTracer_h
#include "RayTracer.h"
#endif

using namespace Graphics;


//////////////////////////////////////////////////////////
//
// RayTracer implementation
// =========
RayTracer::RayTracer(Scene& scene, Camera* camera):
//[]---------------------------------------------------[]
//|  Constructor                                        |
//[]---------------------------------------------------[]
	Renderer(scene, camera)
{
	maxRecursionLevel = 10;
	minWeight = 0.001f;

}

//
// Auxiliary VRC
//
static Vec3 VRC_u;
static Vec3 VRC_v;
static Vec3 VRC_n;

//
// Auxiliary mapping variables
//
static REAL VW_h;
static REAL VW_w;
static REAL II_h;
static REAL II_w;

void
RayTracer::render()
//[]---------------------------------------------------[]
//|  Render                                             |
//[]---------------------------------------------------[]
{
	printf("Invoke renderImage(image) to run the ray tracer\n");
}

void
RayTracer::renderImage(Image& image)
//[]---------------------------------------------------[]
//|  Run the ray tracer                                 |
//[]---------------------------------------------------[]
{
	image.getSize(W, H);
	// init auxiliary VRC
	VRC_n = camera->getViewPlaneNormal();
	VRC_v = camera->getViewUp();
	VRC_u = VRC_v.cross(VRC_n);
	// init auxiliary mapping variables
	II_w = Math::inverse((REAL)W);
	II_h = Math::inverse((REAL)H);

	REAL height = camera->windowHeight();

	if (W >= H)
		VW_w = (VW_h = height) * W * II_h;
	else
		VW_h = (VW_w = height) * H * II_w;
	// init pixel ray
	pixelRay.origin = camera->getPosition();
	pixelRay.direction = -VRC_n;
	scan(image);
}

void
RayTracer::scan(Image& image)
//[]---------------------------------------------------[]
//|  Basic scan with optional jitter                    |
//[]---------------------------------------------------[]
{
	Pixel* pixels = new Pixel[W];

	for (int j = 0; j < H; j++)
	{
		REAL y = j + 0.5f;

		printf("Scanning line %d of %d\r", j + 1, H);
		for (int i = 0; i < W; i++)
			pixels[i] = shoot(i + 0.5f, y);
		image.write(j, pixels);
	}
	delete []pixels;
}

Color
RayTracer::shoot(REAL x, REAL y)
//[]---------------------------------------------------[]
//|  Shoot a pixel ray                                  |
//|  @param x coordinate of the pixel                   |
//|  @param y cordinates of the pixel                   |
//|  @return RGB color of the pixel                     |
//[]---------------------------------------------------[]
{
	Color color;

	// set pixel ray
	setPixelRay(x, y);
	// trace pixel ray
	trace(pixelRay, color, 0, 1.0f);
	// adjust RGB color
	if (color.r > 1.0f)
		color.r = 1.0f;
	if (color.g > 1.0f)
		color.g = 1.0f;
	if (color.b > 1.0f)
		color.b = 1.0f;
	// return pixel color
	return color;
}

void
RayTracer::setPixelRay(REAL x, REAL y)
//[]---------------------------------------------------[]
//|  Set pixel ray                                      |
//|  @param x coordinate of the pixel                   |
//|  @param y cordinates of the pixel                   |
//[]---------------------------------------------------[]
{
	Vec3 p;

	p = VW_w * (x * II_w - 0.5) * VRC_u + VW_h * (y * II_h - 0.5) * VRC_v;
	switch (camera->getProjectionType())
	{
		case Camera::Perspective:
			pixelRay.direction = (p - camera->getDistance() * VRC_n).versor();
			break;

		case Camera::Parallel:
			pixelRay.origin = camera->getPosition() + p;
			break;
	}
}

REAL
RayTracer::trace(const Ray& ray, Color& color, int level, REAL weight)
//[]---------------------------------------------------[]
//|  Trace a ray                                        |
//|  @param the ray (input)                             |
//|  @param color of the ray (output)                   |
//|  @param recursion level                             |
//|  @param ray weight                                  |
//|  @return distance between ray.Origin and P          |
//[]---------------------------------------------------[]
{
	if (level > maxRecursionLevel)
	{
		color = Color::black;
		return 0;
	}

	IntersectInfo hit;

	if (intersect(ray, hit, Math::infinity<REAL>()))
	{
		color = shade(ray, hit, level, weight);
		return hit.distance;
	}
	color = background();
	return Math::infinity<REAL>();
}

#define EPS 1e-4f // TODO

bool
RayTracer::intersect(const Ray& ray, IntersectInfo& hit, REAL maxDist)
//[]---------------------------------------------------[]
//|  Ray/object intersection                            |
//|  @param the ray (input)                             |
//|  @param information on intersection (output)        |
//|  @param background distance                         |
//|  @return true if the ray intersects an object       |
//[]---------------------------------------------------[]
{
	hit.distance = maxDist;
	hit.object = 0;

	IntersectInfo temp;

	for (ActorIterator ait(scene->getActorIterator()); ait; ++ait)
		if (ait.current()->isVisible)
		{
			Model* object = ait.current()->getModel();

			if (object->intersect(ray, temp))
				if (temp.distance < hit.distance)
					hit = temp;
		}
	return hit.object != 0;
}

Color
RayTracer::background() const
//[]---------------------------------------------------[]
//|  Background                                         |
//|  @return background color                           |
//[]---------------------------------------------------[]
{
	return scene->backgroundColor;
}

inline Vec3
getReflectDir(const Vec3& I, const Vec3& N, REAL dot)
{
	return I - 2 * dot * N;
}

inline REAL
maxRGB(Color& color)
{
	return max(max(color.r, color.g), color.b);
}

Color
RayTracer::shade(const Ray& ray, IntersectInfo& hit, int level, REAL weight)
//[]---------------------------------------------------[]
//|  Shade a point P                                    |
//|  @param the ray (input)                             |
//|  @param information on intersection (input)         |
//|  @param recursion level                             |
//|  @param shade weight                                |
//|  @return color at point P                           |
//[]---------------------------------------------------[]
{
	Vec3 R; // reflection vector
	bool calc_R = false; // we haven't calculated the reflection vector yet
	Vec3 P = makeRayPoint(ray, hit.distance);
	Vec3 N = hit.object->normal(hit);
	Vec3 V = ray.direction;
	Material::Surface& surf = hit.object->getMaterial()->surface;
	REAL dot_NV = N.inner(V);

	// make sure "real" normal is on right side
	if (Math::isPositive(dot_NV))
	{
		// if normal on wrong side, flip it
		N.negate();
		dot_NV = -dot_NV;
	}

	// start with global ambient
	Color color = surf.ambient * scene->ambientLight;

	// compute direct lighting
	for (LightIterator lit(scene->getLightIterator()); lit;)
	{
		Light* light = lit++; // light source
		Vec3 L; // light vector
		REAL t; // light distance

		light->getVector(P, L, t);

		REAL dot_NL = N.inner(L);

		// if not backfaced
		if (Math::isPositive(dot_NL))
		{
			Ray lightRay(P, L); // light ray
			IntersectInfo temp = hit;
			Color shadowColor; // shadow color

			// if not shadowed
			if (notShadow(lightRay, hit, t, shadowColor))
			{
				Color I = light->getScaledColor(t); // light color

				// add diffuse reflection
				color += surf.diffuse * I * dot_NL * shadowColor;
				// add specular spot
				if (Math::isPositive(surf.shine))
				{
					if (!calc_R)
					{
						R = getReflectDir(V, N, dot_NV);
						calc_R = true;
					}
					if (Math::isPositive(t = R.inner(L)))
					{
						shadowColor *= surf.spot * pow(t, surf.shine);
						color += shadowColor;
					}
				}
			} // if not shadowed
		} // if not backfaced
	}
	// compute specular reflection
	if (surf.specular != Color::black)
	{
		weight *= maxRGB(surf.specular);
		if (weight > minWeight && (level < maxRecursionLevel))
		{
			if (!calc_R)
				R = getReflectDir(V, N, dot_NV);

			Ray reflectedRay(P + R * EPS, R); // reflection ray
			Color reflectedColor; // reflection color

			// trace reflection color
			trace(reflectedRay, reflectedColor, level + 1, weight);
			color += surf.specular * reflectedColor;
		}
	}
	return color;
}

bool
RayTracer::notShadow(const Ray& ray,
	IntersectInfo& hit,
	REAL maxDistance,
	Color& color)
//[]---------------------------------------------------[]
//|  Verifiy if ray is not a shadow ray                 |
//|  @param the ray (input)                             |
//|  @param information on intersection (input)         |
//|  @param background distance                         |
//|  @return false if the ray intersects an object      |
//[]---------------------------------------------------[]
{
	color = Color::black;

	Ray shadowRay(ray.origin + ray.direction * EPS, ray.direction);
	IntersectInfo temp;

	if (intersect(shadowRay, temp, maxDistance))
		return false;
	color = Color::white; return true;
}
