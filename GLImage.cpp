//[]------------------------------------------------------------------------[]
//|                                                                          |
//|                          GVSG Graphics Classes                           |
//|                               Version 1.0                                |
//|                                                                          |
//|                Copyright® 2007, Paulo Aristarco Pagliosa                 |
//|                All Rights Reserved.                                      |
//|                                                                          |
//[]------------------------------------------------------------------------[]
//
//  OVERVIEW: GLImage.cpp
//  ========
//  Source file for GL image.

#include <string.h>

#ifndef __GLImage_h
#include "GLImage.h"
#endif

using namespace Graphics;


//////////////////////////////////////////////////////////
//
// GLImage implementation
// =======
GLImage::GLImage(int w, int h):
	ImageBuffer(w, h)
//[]----------------------------------------------------[]
//|  Constructor                                         |
//[]----------------------------------------------------[]
{
	buffer = new Pixel[w * h];
}

GLImage::~GLImage()
//[]----------------------------------------------------[]
//|  Destructor                                          |
//[]----------------------------------------------------[]
{
	delete []buffer;
}

void
GLImage::write(int i, Pixel pixels[])
//[]----------------------------------------------------[]
//|  Write                                               |
//[]----------------------------------------------------[]
{
	memcpy(buffer + i * W, pixels, W * sizeof(Pixel));
}

void
GLImage::draw() const
//[]----------------------------------------------------[]
//|  Draw                                                |
//[]----------------------------------------------------[]
{
	drawPixels(W, H, buffer);
}

Pixel*
GLImage::map(LockMode)
//[]----------------------------------------------------[]
//|  Map                                                 |
//[]----------------------------------------------------[]
{
	return buffer;
}

void
GLImage::unmap()
//[]----------------------------------------------------[]
//|  Unmap                                               |
//[]----------------------------------------------------[]
{
	// do nothing
}
