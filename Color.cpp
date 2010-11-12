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
//  OVERVIEW: Color.cpp
//  ========
//  Source file for RGB color.

#ifndef __Color_h
#include "Color.h"
#endif


//////////////////////////////////////////////////////////
//
// Color implementation
// =====
Color Color::black   (  0,   0,   0);
Color Color::red     (255,   0,   0);
Color Color::green   (  0, 255,   0);
Color Color::blue    (  0,   0, 255);
Color Color::cyan    (  0, 255, 255);
Color Color::magenta (255,   0, 255);
Color Color::yellow  (255, 255,   0);
Color Color::white   (255, 255, 255);
Color Color::darkGray(136, 136, 136);
Color Color::gray    (194, 194, 194);

Color
Color::HSV2RGB(REAL hue, REAL saturation, REAL value)
//[]---------------------------------------------------[]
//|  HSV to RGB                                         |
//[]---------------------------------------------------[]
{
	hue = hue - 360 * int(hue * Math::inverse<REAL>(360));

	Color color;
	int f = (int)(hue * Math::inverse<REAL>(120));
	REAL d = (hue - 120 * f) * Math::inverse<REAL>(60);

	if (f == 3)
		--f;
	color[f] = value;
	if (d > 1)
	{
		color[(f + 1) % 3] = value * (1 - saturation * (2 - d));
		color[(f + 2) % 3] = value * (1 - saturation);
	}
	else
	{
		color[(f + 1) % 3] = value * (1 - saturation);
		color[(f + 2) % 3] = value * (1 - saturation * d);
	}
	return color;
}
