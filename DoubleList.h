#ifndef __DoubleList_h
#define __DoubleList_h

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
// OVERVIEW: DoubleList.h
// ========
// Class definition for generic double linked lists.

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
// Double list procedures
//
template <typename T>
void
insertNode(T* node, T*& head)
{
	node->next = head;
	node->prev = 0;
	if (head != 0)
		head->prev = node;
	head = node;
}

template <typename T>
void
removeNode(T* node, T*& head)
{
	if (node->next != 0)
		node->next->prev = node->prev;
	if (node->prev != 0)
		node->prev->next = node->next;
	if (node == head)
		head = node->next;
}

//
// Forward definitions
//
template <typename T> class DoubleListImp;
template <typename T> class DoubleListIteratorImp;
template <typename T> class DoubleList;
template <typename T> class DoubleListIterator;


//////////////////////////////////////////////////////////
//
// DoubleListImp: generic double list class (imp)
// =============
template <typename T>
class DoubleListImp
{
public:
	// Constructor
	DoubleListImp():
		head(0),
		tail(0),
		numberOfElements(0)
	{
		// do nothing
	}

	// Destructor
	~DoubleListImp()
	{
		clear();
	}

	void addAtHead(T*);
	void addAtTail(T*);

	void add(T* e)
	{
		addAtTail(e);
	}

	void remove(T&);
	void clear();

	T* peekHead() const
	{
		return head;
	}

	T* peekTail() const
	{
		return tail;
	}

	int size() const
	{
		return numberOfElements;
	}

	bool isEmpty() const
	{
		return head == 0;
	}

	DoubleListIteratorImp<T> iterator() const;

protected:
	T* head;
	T* tail;
	int numberOfElements;

private:
	DoubleListImp(const DoubleListImp<T>&);
	DoubleListImp<T>& operator =(const DoubleListImp<T>&);

	friend class DoubleListIteratorImp<T>;
	friend class DoubleList<T>;

}; // DoubleListImp


//////////////////////////////////////////////////////////
//
// DoubleListIteratorImp: double list iterator class (imp)
// =====================
template <typename T>
class DoubleListIteratorImp
{
public:
	// Constructor
	DoubleListIteratorImp(const DoubleListImp<T>& list)
	{
		this->list = &list;
		cur = list.head;
	}

	// Testing if objects remain in the iterator
	operator bool() const
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

	// Remove the current element
	T* remove()
	{
		T* temp = cur;

		if (temp != 0)
		{
			cur = temp->next;
			list->remove(temp);
		}
		return temp;
	}

	// Next/previous object
	T* operator ++(int)
	{
		T*	temp = cur;

		cur = cur->next;
		return temp;
	}
	T* operator ++()
	{
		return cur = cur->next;
	}

	T* operator --(int)
	{
		T*	temp = cur;

		cur = cur->prev;
		return temp;
	}
	T* operator --()
	{
		return cur = cur->prev;
	}

private:
	const DoubleListImp<T>* list;
	T* cur;

}; // DoubleListIteratorImp


//////////////////////////////////////////////////////////
//
// DoubleListImp implementation
// =============
template <typename T>
void
DoubleListImp<T>::addAtHead(T* e)
{
	e->next = head;
	e->prev = 0;
	if (head != 0)
		head->prev = e;
	else
		tail = e;
	head = e;
	numberOfElements++;
}

template <typename T>
void
DoubleListImp<T>::addAtTail(T* e)
{
	e->next = 0;
	e->prev = tail;
	if (tail != 0)
		tail->next = e;
	else
		head = e;
	tail = e;
	numberOfElements++;
}

template <typename T>
void
DoubleListImp<T>::remove(T& e)
{
	if (e.next != 0)
		e.next->prev = e.prev;
	if (e.prev != 0)
		e.prev->next = e.next;
	if (&e == head)
		head = e.next;
	if (&e == tail)
		tail = e.prev;
	numberOfElements--;
}

template <typename T>
void
DoubleListImp<T>::clear()
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
inline DoubleListIteratorImp<T>
DoubleListImp<T>::iterator() const
{
	return DoubleListIteratorImp<T>(*this);
}

//
// Macro to declare a double list element
//
#define DECLARE_DOUBLE_LIST_ELEMENT(cls) \
	friend void System::Collections::insertNode<>(cls*, cls*&); \
	friend void System::Collections::removeNode<>(cls*, cls*&); \
public: \
	cls* getNext() \
	{ \
		return next; \
	} \
	cls* getPrev() \
	{ \
		return prev; \
	} \
	friend class System::Collections::DoubleListImp< cls >; \
	friend class System::Collections::DoubleListIteratorImp< cls >; \
private: \
	cls* next; \
	cls* prev

//
// Double list serialization ops (imp)
//
template <typename T>
ObjectOutputStream&
operator <<(ObjectOutputStream& os, const DoubleListImp<T>& list)
{
	int n = list.size();

	os << n;
	if (n > 0)
		for (DoubleListIteratorImp<T> it(list); it;)
			os << it++;
	return os;
}

template <class T>
ObjectInputStream&
operator >>(ObjectInputStream& is, DoubleListImp<T>& list)
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
// DoubleListElement: list element class
// =================
template <typename T>
class DoubleListElement
{
public:
	// Constructor
	DoubleListElement(const T& v):
		value(v)
	{
		// do nothing
	}

	bool operator ==(const DoubleListElement& e) const
	{
		return value == e.value;
	}

	T value;
	DoubleListElement<T>* next;
	DoubleListElement<T>* prev;

	// Get memory for a new element
	void* operator new(size_t);
	// Release element memory
	void operator delete(void*);

private:
	static BlockAllocator* blockAllocator;
	static const int elementsByBlock;

	friend class BlockInitializer<DoubleListElement<T> >;

}; // DoubleListElement


//////////////////////////////////////////////////////////
//
// DoubleListElement implementation
// =================
template <typename T> BlockAllocator* DoubleListElement<T>::blockAllocator;
template <typename T> const int DoubleListElement<T>::elementsByBlock = 40;

template <typename T>
inline void*
DoubleListElement<T>::operator new(size_t size)
{
	return blockAllocator->allocate(size);
}

template <typename T>
inline void
DoubleListElement<T>::operator delete(void* ptr)
{
	blockAllocator->free(ptr);
}


//////////////////////////////////////////////////////////
//
// DoubleList: generic list class
// ==========
template <typename T>
class DoubleList: private BlockInitializer<DoubleListElement<T> >
{
public:
	// Constructor
	DoubleList()
	{
		// do nothing
	}

	void addAtHead(const T& t)
	{
		imp.addAtHead(new DoubleListElement<T>(t));
	}

	void addAtTail(const T& t)
	{
		imp.addAtTail(new DoubleListElement<T>(t));
	}

	void add(const T& t)
	{
		addAtTail(t);
	}

	bool remove(const T&);
	bool removeHead();

	void clear()
	{
		imp.clear();
	}

	bool contains(const T& t) const
	{
		return find(t) != 0;
	}

	int size() const
	{
		return imp.size();
	}

	bool isEmpty() const
	{
		return imp.isEmpty();
	}

	DoubleListIterator<T> iterator() const;

protected:
	DoubleListImp<DoubleListElement<T> > imp;

private:
	DoubleList(const DoubleList<T>&);
	DoubleList<T>& operator =(const DoubleList<T>&);

	DoubleListElement<T>* find(const T&) const;

	void remove(DoubleListElement<T>* e)
	{
		imp.remove(*e);
		delete e;
	}

	friend class DoubleListIterator<T>;

}; // DoubleList


//////////////////////////////////////////////////////////
//
// DoubleListIterator: generic list iterator class
// ==================
template <typename T>
class DoubleListIterator
{
public:
	// Constructor
	DoubleListIterator(const DoubleList<T>& list):
		imp(list.imp)
	{
		// do nothing
	}

	// Testing if objects remain in the iterator
	operator bool() const
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

	T& operator --(int)
	{
		return (imp--)->value;
	}
	T& operator --()
	{
		return (--imp)->value;
	}

private:
	DoubleListIteratorImp<DoubleListElement<T> > imp;

}; // DoubleListIterator


//////////////////////////////////////////////////////////
//
// DoubleList implementation
// ==========
template <typename T>
bool
DoubleList<T>::remove(const T& t)
{
	DoubleListElement<T>* temp = find(t);

	if (temp == 0)
		return false;
	remove(temp);
	return true;
}

template <typename T>
bool
DoubleList<T>::removeHead()
{
	if (imp.head == 0)
		return false;
	remove(imp.head);
	return true;
}

template <typename T>
DoubleListElement<T>*
DoubleList<T>::find(const T& t) const
{
	for (DoubleListElement<T>* e = imp.head; e; e = e->next)
		if (e->value == t)
			return e;
	return 0;
}

template <typename T>
inline DoubleListIterator<T>
DoubleList<T>::iterator() const
{
	return DoubleListIterator<T>(*this);
}

//
// Double list serialization ops
//
template <class T>
ObjectOutputStream&
operator <<(ObjectOutputStream& os, const DoubleList<T>& list)
{
	int n = list.size();

	os << n;
	if (n > 0)
		for (DoubleListIterator<T> it(list); it;)
			os << it++;
	return os;
}

template <class T>
ObjectInputStream&
operator >>(ObjectInputStream& is, DoubleList<T>& list)
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

#endif // __DoubleList_h
