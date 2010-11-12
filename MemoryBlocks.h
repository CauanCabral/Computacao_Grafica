#ifndef __MemoryBlocks_h
#define __MemoryBlocks_h

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
//  OVERVIEW: MemoryBlocks.h
//  ========
//  Class definition for memory blocks.

#include <stddef.h>

namespace System
{ // begin namespace System

namespace Collections
{ // begin namespace Collections


//////////////////////////////////////////////////////////
//
// MemoryBlocks: memory blocks class
// ============
class MemoryBlocks
{
private:
	class Block
	{
	public:
		void* operator new(size_t size, size_t bsize)
		{
			return ::operator new(size + bsize);
		}

		void operator delete(void* ptr, size_t)
		{
			::operator delete(ptr);
		}

		// Constructor
		Block(Block* next)
		{
			this->next = next;
		}

		Block* next;

	}; // Block

	size_t blockSize;
	size_t curLoc;
	Block* curBlock;
	int numberOfBlocks;

	char* block()
	{
		return reinterpret_cast<char*>(curBlock);
	}

	bool allocateBlock();
	void freeTo(int);

public:
	// Constructor
	MemoryBlocks(size_t size):
		blockSize(size),
		curLoc(size),
		curBlock(0),
		numberOfBlocks(0)
	{
		// do nothing
	}

	// Destructor
	~MemoryBlocks()
	{
		freeTo(0);
	}

	size_t getBlockSize() const
	{
		return blockSize;
	}

	int getNumberOfBlocks() const
	{
		return numberOfBlocks;
	}

	// Allocate memory
	void* allocate(size_t);

}; // MemoryBlocks


//////////////////////////////////////////////////////////
//
// BlockAllocator: single-size block allocator class
// ==============
class BlockAllocator
{
public:
	// Constructor
	BlockAllocator(int size, int count):
		memoryBlocks(size * count),
		freeList(0)
	{
		// do nothing
	}

	void* allocate(size_t);
	void free(void*);

private:
	MemoryBlocks memoryBlocks;
	void* freeList;

}; // BlockAllocator


//////////////////////////////////////////////////////////
//
// BlockInitializer: block initializer template
// ================
template <typename T>
class BlockInitializer
{
protected:
	// Constructor
	BlockInitializer();

	// Destructor
	~BlockInitializer()
	{
		if (--count == 0)
			delete T::blockAllocator;
	}

	static int count;

}; // BlockInitializer


//////////////////////////////////////////////////////////
//
// BlockInitializer implementation
// ================
template <typename T> int BlockInitializer<T>::count;

template <typename T>
BlockInitializer<T>::BlockInitializer()
{
	if (count++ == 0)
		T::blockAllocator = new BlockAllocator(sizeof(T), T::elementsByBlock);
}

//
// Macros
//
#define DECLARE_BLOCK_ALLOCABLE(cls) \
public: \
	void* operator new(size_t); \
	void operator delete(void*); \
private: \
	static int count; \
	static System::Collections::BlockAllocator* blockAllocator; \
	static const int elementsByBlock; \
	friend class System::Collections::BlockInitializer< cls >

#define IMPLEMENT_BLOCK_ALLOCABLE(cls, n) \
void* \
cls::operator new(size_t size) \
{ \
	if (count++ == 0) \
		blockAllocator = new BlockAllocator(sizeof(cls), elementsByBlock); \
	return blockAllocator->allocate(size); \
} \
void \
cls::operator delete(void* ptr) \
{ \
	blockAllocator->free(ptr); \
	if (--count == 0) \
		delete blockAllocator; \
} \
int cls::count; \
System::Collections::BlockAllocator* cls::blockAllocator; \
const int cls::elementsByBlock = (n)

} // end namespace Collections

} // end namespace System

#endif // __MemoryBlocks_h
