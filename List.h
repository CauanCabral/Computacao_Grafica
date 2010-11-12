#ifndef __List_h
#define __List_h

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
//  OVERVIEW: List.h
//  ========
//  Class definition for generic list.

#ifndef __MemoryBlocks_h
#include "MemoryBlocks.h"
#endif
#ifndef __ObjectStream_h
#include "ObjectStream.h"
#endif

namespace System
{ // begin namespace System

namespace Collections
{ // begin namespace Collections

//
// Forward definitions
//
template <typename T> class ListImp;
template <typename T> class ListIteratorImp;
template <typename T> class List;
template <typename T> class ListIterator;


//////////////////////////////////////////////////////////
//
// ListImp: generic list class (imp)
// =======
template <typename T>
class ListImp
{
public:
	// Constructor
	ListImp():
		head(0),
		tail(0),
		numberOfElements(0)
	{
		// do nothing
	}

	// Destructor
	~ListImp()
	{
		clear();
	}

	void addAtHead(T*);
	void addAtTail(T*);

	void add(T* e)
	{
		addAtTail(e);
	}

	void invert();
	T* removeHead();
	void clear();

	T* peekHead() const
	{
		return head;
	}

	int size() const
	{
		return numberOfElements;
	}

	bool isEmpty() const
	{
		return numberOfElements == 0;
	}

	ListIteratorImp<T> iterator() const;

protected:
	T* head;
	T* tail;
	int numberOfElements;

private:
	ListImp(const ListImp<T>&);
	ListImp<T>& operator =(const ListImp<T>&);

	friend class ListIteratorImp<T>;
	friend class List<T>;

}; // ListImp


//////////////////////////////////////////////////////////
//
// ListIteratorImp: generic list iterator class (imp)
// ===============
template <typename T>
class ListIteratorImp
{
	static T* getNext(T* e)
	{
		return e->next;
	}

public:
	// Constructor
	ListIteratorImp(const ListImp<T>& list)
	{
		this->list = &list;
		cur = list.head;
	}

	// Testing if objects remain in the iterator
	operator int() const
	{
		return cur != 0;
	}

	// Get the current object
	T* current() const
	{
		return cur;
	}

	// Restart the iterator
	void restart()
	{
		cur = list->head;
	}

	// Next/previous object
	T* operator ++(int)
	{
		T*	temp = cur;

		cur = getNext(cur);
		return temp;
	}
	T* operator ++()
	{
		return cur = getNext(cur);
	}

private:
	const ListImp<T>* list;
	T* cur;

}; // ListIteratorImp


//////////////////////////////////////////////////////////
//
// ListImp implementation
// =======
template <class T>
void
ListImp<T>::addAtHead(T* e)
{
	if (tail == 0)
		tail = e;
	e->next = head;
	head = e;
	numberOfElements++;
}

template <typename T>
void
ListImp<T>::addAtTail(T* e)
{
	if (head == 0)
		head = e;
	else
		tail->next = e;
	tail = e;
	e->next = 0;
	numberOfElements++;
}

template <typename T>
T*
ListImp<T>::removeHead()
{
	if (head == 0)
		return 0;
	
	T* temp = head;

	if (tail == temp)
		tail = 0;
	head = temp->next;
	numberOfElements--;
	return temp;
}

template <typename T>
void
ListImp<T>::clear()
{
	while (head != 0)
	{
		T* temp = head;

		head = head->next;
		delete temp;
	}
	tail = 0;
	numberOfElements = 0;
}

template <typename T>
void
ListImp<T>::invert()
{
	T* node = head;
	T* last = 0;

	while (node)
	{
		T* next = node->next;

		node->next = last;
		last = node;
		node = next;
	}
	tail = head;
	head = last;
}

template <typename T>
inline ListIteratorImp<T>
ListImp<T>::iterator() const
{
	return ListIteratorImp<T>(*this);
}

//
// Macro to declare a list element
//
#define DECLARE_LIST_ELEMENT(cls) \
public: \
	cls* getNext() \
	{ \
		return next; \
	} \
	friend class System::Collections::ListImp< cls >; \
	friend class System::Collections::ListIteratorImp< cls >; \
private: \
	cls* next

//
// List serialization ops (imp)
//
template <typename T>
ObjectOutputStream&
operator <<(ObjectOutputStream& os, const ListImp<T>& list)
{
	int n = list.size();

	os << n;
	if (n > 0)
		for (ListIteratorImp<T> it(list); it;)
			os << it++;
	return os;
}

template <typename T>
ObjectInputStream&
operator >>(ObjectInputStream& is, ListImp<T>& list)
{
	int n;

	is >> n;
	for (int i = 0; i < n; i++)
	{
		T* t;

		is >> t;
		list.add(t);
	}
	return is;
}


//////////////////////////////////////////////////////////
//
// ListElement: list element class
// ===========
template <typename T>
class ListElement
{
public:
	// Constructor
	ListElement(const T& v):
		value(v)
	{
		// do nothing
	}

	T value;
	ListElement<T>* next;

	// Get memory for a new element
	void* operator new(size_t);
	// Release element memory
	void operator delete(void*);

private:
	static BlockAllocator* blockAllocator;
	static const int elementsByBlock;

	friend class BlockInitializer<ListElement<T> >;

}; // ListElement


//////////////////////////////////////////////////////////
//
// ListElement implementation
// ===========
template <typename T> BlockAllocator* ListElement<T>::blockAllocator;
template <typename T> const int ListElement<T>::elementsByBlock = 40;

template <typename T>
inline void*
ListElement<T>::operator new(size_t size)
{
	return blockAllocator->allocate(size);
}

template <typename T>
inline void
ListElement<T>::operator delete(void* ptr)
{
	blockAllocator->free(ptr);
}


//////////////////////////////////////////////////////////
//
// List: generic list class
// ====
template <typename T>
class List: private BlockInitializer<ListElement<T> >
{
public:
	// Constructor
	List()
	{
		// do nothing
	}

	void addAtHead(const T& t)
	{
		imp.addAtHead(new ListElement<T>(t));
	}

	void addAtTail(const T& t)
	{
		imp.addAtTail(new ListElement<T>(t));
	}

	void add(const T& t)
	{
		addAtTail(t);
	}

	bool remove(const T&);
	bool removeHead();

	void invert()
	{
		imp.invert();
	}

	void clear()
	{
		imp.clear();
	}

	bool contains(const T& t) const;

	int size() const
	{
		return imp.size();
	}

	bool isEmpty() const
	{
		return imp.isEmpty();
	}

	ListIterator<T> iterator() const;

protected:
	ListImp<ListElement<T> > imp;

private:
	List(const List<T>&);
	List<T>& operator =(const List<T>&);

	friend class ListIterator<T>;

}; // List


//////////////////////////////////////////////////////////
//
// ListIterator: generic list iterator class
// ============
template <typename T>
class ListIterator
{
public:
	// Constructor
	ListIterator(const List<T>& list):
		imp(list.imp)
	{
		// do nothing
	}

	// Testing if objects remain in the iterator
	operator int() const
	{
		return imp;
	}

	// Restart the iterator
	void restart()
	{
		imp.restart();
	}

	// Get the current object
	T& current() const
	{
		return imp.current()->value;
	}

	// Next/previous object
	T& operator ++(int)
	{
		return (imp++)->value;
	}
	T& operator ++()
	{
		return (++imp)->value;
	}

private:
	ListIteratorImp<ListElement<T> > imp;

}; // ListIterator


//////////////////////////////////////////////////////////
//
// List implementation
// ====
template <typename T>
bool
List<T>::remove(const T& t)
{
	ListElement<T>** pp = imp.head;

	for (ListElement<T>* prev = 0; *pp != 0; pp = &(prev = *pp)->next)
		if ((*pp)->value == t)
		{
			ListElement<T>* temp = *pp;

			if (temp == imp.tail)
				imp.tail = prev;
			*pp = temp->next;
			delete temp;
			imp.numberOfElements--;
			return true;
		}
	return false;
}

template <typename T>
bool
List<T>::removeHead()
{
	ListElement<T>* temp = imp.removeHead();

	if (temp == 0)
		return false;
	delete temp;
	return true;
}

template <typename T>
bool
List<T>::contains(const T& t) const
{
	for (ListElement<T>* e = imp.peekHead(); e != 0; e = e->next)
		if (e->value == t)
			return true;
	return false;
}

template <typename T>
inline ListIterator<T>
List<T>::iterator() const
{
	return ListIterator<T>(*this);
}

//
// List serialization ops
//
template <typename T>
ObjectOutputStream&
operator <<(ObjectOutputStream& os, const List<T>& list)
{
	int n = list.size();

	os << n;
	if (n > 0)
		for (ListIterator<T> it(list); it;)
			os << it++;
	return os;
}

template <typename T>
ObjectInputStream&
operator >>(ObjectInputStream& is, List<T>& list)
{
	int n;

	is >> n;
	for (int i = 0; i < n; i++)
	{
		T t;

		is >> t;
		list.add(t);
	}
	return is;
}

} // end namespace Collection

} // end namespace System

#endif // __List_h
