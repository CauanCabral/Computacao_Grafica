#ifndef __Image_h
#define __Image_h

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
//  OVERVIEW: Image.h
//  ========
//  Class definition for image.

#ifndef __Color_h
#include "Color.h"
#endif
#ifndef __Typedefs_h
#include "Typedefs.h"
#endif

#define MIN_IMAGE_WIDTH 4

//
// Auxiliary function
//
inline int
roundupImageWidth(int w)
{
	return (w + MIN_IMAGE_WIDTH - 1) & -MIN_IMAGE_WIDTH;
}

#ifndef __LINUX
#pragma pack(push, 1)
#endif


//////////////////////////////////////////////////////////
//
// Pixel: pixel class
// =====
struct Pixel
{
	uint8 r;
	uint8 g;
	uint8 b;

	// Constructors
	Pixel()
	{
		// do nothing
	}
	Pixel(uint8 r, uint8 g, uint8 b)
	{
		set(r, g, b);
	}
	Pixel(const Color& c)
	{
		set(c);
	}

	void set(uint8 r, uint8 g, uint8 b)
	{
		this->r = r;
		this->g = g;
		this->b = b;
	}
	void set(const Color& c)
	{
		this->r = (uint8)(255 * c.r);
		this->g = (uint8)(255 * c.g);
		this->b = (uint8)(255 * c.b);
	}

	Pixel& operator +=(const Pixel& p)
	{
		r += p.r;
		g += p.g;
		b += p.b;
		return *this;
	}
	Pixel& operator +=(const Color& c)
	{
		r += (uint8)(255 * c.r);
		g += (uint8)(255 * c.g);
		b += (uint8)(255 * c.b);
		return *this;
	}

}; // Pixel

#ifndef __LINUX
#pragma pack(pop)
#endif

namespace Graphics
{ // begin namespace Graphics


//////////////////////////////////////////////////////////
//
// Image: generic image class
// =====
class Image
{
public:
	// Destructor
	virtual ~Image()
	{
		// do nothing
	}

	virtual void getSize(int&, int&) const = 0;
	virtual void write(int, Pixel[]) = 0;

}; // Image


//////////////////////////////////////////////////////////
//
// ImageBuffer: generic image buffer class
// ===========
class ImageBuffer: public Image
{
public:
	enum LockMode
	{
		Read,
		Write
	};

	int getWidth() const
	{
		return W;
	}
	int getHeight() const
	{
		return H;
	}

	void getSize(int& w, int& h) const
	{
		w = W;
		h = H;
	}

	// Lock image to read/write
	Pixel* lock(LockMode mode)
	{
		if (buffer == 0)
			buffer = map(mode);
		return buffer;
	}

	void unlock()
	{
		if (buffer != 0)
		{
			unmap();
			buffer = 0;
		}
	}

	Pixel readPixel(int i, int j) const
	{
		return buffer[i * W + j];
	}

	void writePixel(int i, int j, const Pixel& pixel)
	{
		buffer[i * W + j] = pixel;
	}

	// Draw image
	virtual void draw() const = 0;

protected:
	int W;
	int H;
	Pixel* buffer;

	// Constructor
	ImageBuffer(int w, int h):
		W(w),
		H(h),
		buffer(0)
	{
		// do nothing
	}

	// Map image buffer
	virtual Pixel* map(LockMode) = 0;

	// Unmap image buffer
	virtual void unmap() = 0;

}; // Image

} // end namespace Graphics

#endif // __Image_h
