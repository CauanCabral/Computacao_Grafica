#ifndef __Array_h
#define __Array_h

//[]------------------------------------------------------------------------[]
//|                                                                          |
//|                        GVSG Foundation Classes                           |
//|                               Version 1.0                                |
//|                                                                          |
//|                Copyright® 2007, Paulo Aristarco Pagliosa                 |
//|                All Rights Reserved.                                      |
//|                                                                          |
//[]------------------------------------------------------------------------[]
//
// OVERVIEW: Array.h
// ========
// Class definition for generic arrays.

#ifndef __Exception_h
#include "Exception.h"
#endif

#include <memory.h>

namespace System
{ // begin namespace System

namespace Collections
{ // begin namespace Collections

#define DFL_ARRAY_SIZE 10

//
// Forward definitions
//
template <typename T> class Array;
template <typename T> class ArrayIterator;
template <typename T> class PointerArray;
template <typename T> class PointerArrayIterator;


//////////////////////////////////////////////////////////
//
// Array: generic array class
// =====
template <typename T>
class Array
{
public:
	// Constructor
	Array(int = DFL_ARRAY_SIZE, int = 0);

	// Destructor
	~Array()
	{
		delete []data;
	}

	void add(const T&);
	void addAt(const T&, int);
	bool removeAt(int);

	bool remove(const T& t)
	{
		int i = findIndex(t);
		return i >= 0 ? removeAt(i) : false;
	}

	T& operator [](int i)
	{
		PRECONDITION(i >= 0 && i < numberOfElements);
		return this->data[i];
	}
	const T& operator [](int i) const
	{
		PRECONDITION(i >= 0 && i < numberOfElements);
		return this->data[i];
	}

	int findIndex(const T&) const;

	int size() const
	{
		return numberOfElements;
	}

	bool isEmpty() const
	{
		return numberOfElements == 0;
	}

protected:
	int capacity;
	int delta;
	int numberOfElements;
	T* data;

	void resize();

private:
	Array(const Array<T>&);
	Array<T>& operator =(const Array<T>&);

	friend class ArrayIterator<T>;

}; // Array


//////////////////////////////////////////////////////////
//
// Array implementation
// =====
template <typename T>
Array<T>::Array(int initSize, int delta)
{
	PRECONDITION(initSize > 0 && delta >= 0);
	data = new T[capacity = initSize];
	this->delta = delta > 0 ? delta : DFL_ARRAY_SIZE;
	numberOfElements = 0;
}

template <typename T>
void
Array<T>::resize()
{
	int newSize = this->capacity + delta;
	T* temp = new T[newSize];

	memcpy(temp, this->data, this->capacity * sizeof(T));
	delete []this->data;
	this->capacity = newSize;
	this->data = temp;
}

template <typename T>
void
Array<T>::add(const T& t)
{
	if (numberOfElements >= this->capacity)
		resize();
	this->data[numberOfElements++] = t;
}

template <typename T>
void
Array<T>::addAt(const T& t, int i)
{
	PRECONDITION(i >= 0 && i < numberOfElements);
	if (numberOfElements >= this->capacity)
		resize();

	T* src = this->data + i;

	memmove(src + 1, src, (numberOfElements - i) * sizeof(T));
	this->data[i] = t;
	numberOfElements++;
}

template <typename T>
bool
Array<T>::removeAt(int i)
{
	PRECONDITION(i >= 0);
	if (i >= numberOfElements)
		return false;
	numberOfElements--;

	T* dst = this->data + i;

	memmove(dst, dst + 1, (numberOfElements - i) * sizeof(T));
	return true;
}

template <typename T>
int
Array<T>::findIndex(const T& t) const
{
	for (int cur = 0; cur < numberOfElements; cur++)
		if (this->data[cur] == t)
			return cur;
	 return -1;
}


//////////////////////////////////////////////////////////
//
// ArrayIterator: generic array iterator class
// =============
template <typename T>
class ArrayIterator
{
public:
	// Constructor
	ArrayIterator(const Array<T>& array)
	{
		this->array = &array;
		cur = 0;
	}

	// Testing if objects remain in the iterator
	operator int() const
	{
		return cur < array->numberOfElements;
	}

	// Get the current object
	T& current() const
	{
		return array->data[cur];
	}

	// Restart the iterator
	void restart()
	{
		cur = 0;
	}

	// Next/previous object
	T& operator ++(int)
	{
		return array->data[cur++];
	}
	T& operator ++()
	{
		return array->data[++cur];
	}

	T& operator --(int)
	{
		return array->data[cur--];
	}
	T& operator --()
	{
		return array->data[--cur];
	}

protected:
	const Array<T>* array;
	int cur;

}; // ArrayIterator


//////////////////////////////////////////////////////////
//
// PointerArray: generic pointer array class
// ============
template <typename T>
class PointerArray
{
public:
	bool shouldDelete;

	// Constructor
	PointerArray(int = DFL_ARRAY_SIZE, int = 0);

	// Destructor
	~PointerArray()
	{
		if (shouldDelete)
			clear();
		delete []data;
	}

	void add(T*);
	void addAt(T*, int);
	bool removeAt(int, bool = false);

	bool remove(T* t, bool forceDelete = false)
	{
		int i = findIndex(t);
		return i >= 0 ? removeAt(i, forceDelete) : false;
	}

	void clear(bool = false);

	T*& operator [](int i)
	{
		PRECONDITION(i >= 0 && i < numberOfElements);
		return data[i];
	}
	const T* operator [](int i) const
	{
		PRECONDITION(i >= 0 && i < numberOfElements);
		return data[i];
	}

	int findIndex(T*) const;

	int size() const
	{
		return numberOfElements;
	}

	bool isEmpty() const
	{
		return numberOfElements == 0;
	}

protected:
	int capacity;
	int delta;
	int numberOfElements;
	T** data;

	void resize();

private:
	PointerArray(const PointerArray<T>&);
	PointerArray<T>& operator =(const PointerArray<T>&);

	friend class PointerArrayIterator<T>;

}; // PointerArray


//////////////////////////////////////////////////////////
//
// PointerArray implementation
// ============
template <typename T>
PointerArray<T>::PointerArray(int initSize, int delta):
	shouldDelete(false)
{
	PRECONDITION(initSize >= 0 && delta >= 0);
	data = new T*[capacity = initSize];
	this->delta = delta > 0 ? delta : DFL_ARRAY_SIZE;
	numberOfElements = 0;
}

template <typename T>
void
PointerArray<T>::resize()
{
	int newSize = this->capacity + delta;
	T** temp = new T*[newSize];

	memcpy(temp, this->data, this->capacity * sizeof(T*));
	delete []this->data;
	this->capacity = newSize;
	this->data = temp;
}

template <typename T>
void
PointerArray<T>::add(T* t)
{
	if (numberOfElements >= this->capacity)
		resize();
	this->data[numberOfElements++] = t;
}

template <typename T>
void
PointerArray<T>::addAt(T* t, int i)
{
	PRECONDITION(i >= 0 && i < numberOfElements);
	if (numberOfElements >= this->capacity)
		resize();

	T** src = this->data + i;

	memmove(src + 1, src, (numberOfElements - i) * sizeof(T*));
	this->data[i] = t;
	numberOfElements++;
}

template <typename T>
bool
PointerArray<T>::removeAt(int i, bool forceDelete)
{
	PRECONDITION(i >= 0);
	if (i >= numberOfElements)
		return false;
	if (forceDelete || this->shouldDelete)
		delete this->data[i];
	numberOfElements--;

	T** dst = this->data + i;

	memmove(dst, dst + 1, (numberOfElements - i) * sizeof(T*));
	return true;
}

template <typename T>
void
PointerArray<T>::clear(bool forceDelete)
{
	forceDelete |= shouldDelete;
	for (int i = 0, n = size(); i < n; i++)
		if (data[i] != 0)
		{
			if (forceDelete)
				delete data[i];
			data[i] = 0;
		}
	numberOfElements = 0;
}

template <typename T>
int
PointerArray<T>::findIndex(T* t) const
{
	for (int cur = 0; cur < numberOfElements; cur++)
		if (this->data[cur] == t)
			return cur;
	 return -1;
}


//////////////////////////////////////////////////////////
//
// PointerArrayIterator: generic pointer array iterator class
// ====================
template <typename T>
class PointerArrayIterator
{
public:
	// Constructor
	PointerArrayIterator(const PointerArray<T>& array)
	{
		this->array = &array;
		cur = 0;
	}

	// Testing if objects remain in the iterator
	operator int() const
	{
		return cur < array->numberOfElements;
	}

	// Get the current object
	T* current() const
	{
		return array->data[cur];
	}

	// Restart the iterator
	void restart()
	{
		cur = 0;
	}

	// Next/previous object
	T* operator ++(int)
	{
		return array->data[cur++];
	}
	T* operator ++()
	{
		return array->data[++cur];
	}

	T* operator --(int)
	{
		return array->data[cur--];
	}
	T* operator --()
	{
		return array->data[--cur];
	}

protected:
	const PointerArray<T>* array;
	int cur;

}; // PointerArrayIterator

} // end namespace Collections

} // end namespace System

#endif // __Array_h
