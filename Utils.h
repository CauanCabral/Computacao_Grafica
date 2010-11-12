#ifndef __Utils_h
#define __Utils_h

//[]------------------------------------------------------------------------[]
//|                                                                          |
//|                  GVSG Foundation Classes (CUDA Library)                  |
//|                               Version 1.0                                |
//|                                                                          |
//|                Copyright® 2009, Paulo Pagliosa                           |
//|                All Rights Reserved.                                      |
//|                                                                          |
//[]------------------------------------------------------------------------[]
//
// OVERVIEW: Util.h
// ========
// Definitions for timer, etc.

#ifndef __Typedefs_h
#include "Typedefs.h"
#endif

namespace Utils
{ // begin namespace Utils


//////////////////////////////////////////////////////////
//
// Utils:Flags: flags class
// ===========
class Flags
{
public:
	// Constructors
	Flags()
	{
		// do nothing
	}
	Flags(int mask)
	{
		bits = mask;
	}

	Flags& operator =(int mask)
	{
		bits = mask;
		return *this;
	}

	void set(int mask)
	{
		bits |= mask;
	}

	void reset(int mask)
	{
		bits &= ~mask;
	}

	void clear()
	{
		bits = 0;
	}

	void enable(int mask, bool state)
	{
		state ? set(mask) : reset(mask);
	}

	operator int() const
	{
		return bits;
	}

	bool isSet(int mask) const
	{
		return (bits & mask) == mask;
	}

	bool test(int mask) const
	{
		return (bits & mask) != 0;
	}

private:
	int bits;

}; // Flags

} // end namespace Utils

#endif // __Utils_h
