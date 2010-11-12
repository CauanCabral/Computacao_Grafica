#ifndef __GLImage_h
#define __GLImage_h

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
//  OVERVIEW: GLImage.h
//  ========
//  Class definition for GL image.

#ifndef __LINUX
#define NOMINMAX
#include <windows.h>
#endif
#include <GL/glut.h>

#ifndef __Image_h
#include "Image.h"
#endif

namespace Graphics
{ // begin namespace Graphics


//////////////////////////////////////////////////////////
//
// GLImage: GL image class
// =======
class GLImage: public ImageBuffer
{
public:
	// Constructor
	GLImage(int, int);

	// Destructor
	~GLImage();

	// Write pixels
	void write(int, Pixel[]);

	// Draw
	void draw() const;

protected:
	Pixel* map(LockMode);
	void unmap();

private:
	Pixel* buffer;

}; // GLImage

} // end namespace Graphics

//
// Auxiliary functions
//
inline void
drawPixels(int w, int h, Pixel* buffer)
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, w, 0, h);
	glRasterPos2i(0, 0);
	glDrawPixels(w, h, GL_RGB, GL_UNSIGNED_BYTE, buffer);
	glFlush();
}

inline GLuint
createTexture(int w, int h)
{
	GLuint id;

	glEnable(GL_TEXTURE_2D);
	// Create texture
	glGenTextures(1, &id);
	glBindTexture(GL_TEXTURE_2D, id);
// Set texture parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	// Initialize texture
	glTexImage2D(GL_TEXTURE_2D,
		0,
		GL_RGB,
		w,
		h,
		0,
		GL_RGB,
		GL_UNSIGNED_BYTE,
		0);
	return id;
}

inline void
setTextureData(int w, int h, void* data)
{
	glTexSubImage2D(GL_TEXTURE_2D,
		0,
		0,
		0,
		w,
		h,
		GL_RGB,
		GL_UNSIGNED_BYTE,
		data);
}

#endif // __GLImage_h
