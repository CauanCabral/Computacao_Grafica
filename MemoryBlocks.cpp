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
//  OVERVIEW: MemoryBlocks.cpp
//  ========
//  Source file for memory blocks.

#ifndef __MemoryBlocks_h
#include "MemoryBlocks.h"
#endif

using namespace System::Collections;


//////////////////////////////////////////////////////////
//
// MemoryBlocks implementation
// ============
bool
MemoryBlocks::allocateBlock()
//[]---------------------------------------------------[]
//|  Allocate a new block                               |
//[]---------------------------------------------------[]
{
	Block* blk = new(blockSize) Block(curBlock);

	if (blk == 0)
		return false;
	curBlock = blk + 1;
	numberOfBlocks++;
	return true;
}

void
MemoryBlocks::freeTo(int blkn)
//[]---------------------------------------------------[]
//|  Free blkn blocks                                   |
//[]---------------------------------------------------[]
{
	while (numberOfBlocks > blkn)
	{
		Block* blk = curBlock - 1;

		curBlock = blk->next;
		delete blk;
		numberOfBlocks--;
	}
}

void*
MemoryBlocks::allocate(size_t size)
//[]---------------------------------------------------[]
//|  Allocate memory                                    |
//[]---------------------------------------------------[]
{
	if (size > blockSize - curLoc)
		if (!allocateBlock())
			return 0;
		else curLoc = 0;

	void* ptr = block() + curLoc;

	curLoc += size;
	return ptr;
}


//////////////////////////////////////////////////////////
//
// BlockAllocator implementation
// ==============
void*
BlockAllocator::allocate(size_t size)
//[]---------------------------------------------------[]
//|  Allocate memory for a new element                  |
//[]---------------------------------------------------[]
{
	if (freeList == 0)
		return memoryBlocks.allocate(size);

	void* ptr = freeList;

	freeList = *(void**)ptr;
	return ptr;
}

void
BlockAllocator::free(void* ptr)
//[]---------------------------------------------------[]
//|  Free element memory                                |
//[]---------------------------------------------------[]
{
	*(void**)ptr = freeList;
	freeList = ptr;
}
