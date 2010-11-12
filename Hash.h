#ifndef __Hash_h
#define __Hash_h

//[]------------------------------------------------------------------------[]
//|                                                                          |
//|                        GVSG Foundation typenamees                        |
//|                               Version 1.0                                |
//|                                                                          |
//|                Copyright® 2007, Paulo Aristarco Pagliosa                 |
//|                All Rights Reserved.                                      |
//|                                                                          |
//[]------------------------------------------------------------------------[]
//
//  OVERVIEW: Hash.h
//  ========
//  Class definition for generic hash tables.

#ifndef __MemoryBlocks_h
#include "MemoryBlocks.h"
#endif
#ifndef __Typedefs_h
#include "Typedefs.h"
#endif

//
// Auxilary function
//
template <typename K>
inline uint
hashValue(const K& key)
{
	return (uint)key;
}

namespace System
{ // begin namespace System

namespace Collections
{ // begin namespace Collections

#define DFL_HASH_TABLE_SIZE 211

//
// Forward definitions
//
template <typename K, typename V> class HashTable;
template <typename K, typename V> class HashTableIterator;


//////////////////////////////////////////////////////////
//
// HashEntry: hash entry class
// =========
template <typename K, typename V>
class HashEntry
{
public:
	// Constructor
	HashEntry(const K& aKey, const V& aValue):
		key(aKey),
		value(aValue)
	{
		// do nothing
	}

	const K& getKey() const
	{
		return key;
	}

	const V& getValue() const
	{
		return value;
	}

	void setValue(const V& value)
	{
		this->value = value;
	}

private:
	const K key;
	V value;
	HashEntry<K, V>* next;
	HashEntry<K, V>** pprev;

	// Get memory for a new element
	void* operator new(size_t);
	// Release element memory
	void operator delete(void*);

	static BlockAllocator* blockAllocator;
	static const int elementsByBlock;

	friend class BlockInitializer<HashEntry<K, V> >;
	friend class HashTable<K, V>;
	friend class HashTableIterator<K, V>;

}; // HashEntry


//////////////////////////////////////////////////////////
//
// HashEntry implementation
// =========
template <typename K, typename V>
BlockAllocator* HashEntry<K, V>::blockAllocator;
template <typename K, typename V>
const int HashEntry<K, V>::elementsByBlock = 40;

template <typename K, typename V>
inline void*
HashEntry<K, V>::operator new(size_t size)
{
	return blockAllocator->allocate(size);
}

template <typename K, typename V>
inline void
HashEntry<K, V>::operator delete(void* ptr)
{
	blockAllocator->free(ptr);
}


//////////////////////////////////////////////////////////
//
// HashTable: generic hash table class
// =========
template <typename K, typename V>
class HashTable: private BlockInitializer<HashEntry<K, V> >
{
public:
	// Constructor
	HashTable(int = DFL_HASH_TABLE_SIZE);

	// Destructor
	~HashTable()
	{
		clear();
		delete []table;
	}

	void put(const K&, const V&);
	bool remove(const K&);
	void clear();

	bool contains(const K& key) const
	{
		return find(key) != 0;
	}

	V const* get(const K& key) const
	{
		Entry* e = find(key);
		return e != 0 ? &e->value : 0;
	}

	int size() const
	{
		return numberOfEntries;
	}

	bool isEmpty() const
	{
		return numberOfEntries == 0;
	}

	HashTableIterator<K, V> iterator() const;

private:
	typedef HashEntry<K, V> Entry;

	Entry** table;
	int tableSize;
	int numberOfEntries;

	uint getHashValue(const K& key) const
	{
		return hashValue(key) % tableSize;
	}

	Entry* find(const K&) const;

	void remove(Entry*);

	friend class HashTableIterator<K, V>;

}; // HashTable


//////////////////////////////////////////////////////////
//
// HashTableIterator: generic hash table iterator class
// =================
template <typename K, typename V>
class HashTableIterator
{
	typedef HashEntry<K, V> Entry;

public:
	// Constructor
	HashTableIterator(const HashTable<K, V>& ht):
		HT(&ht)
	{
		restart();
	}

	// Testing if objects remain in the iterator
	operator int() const
	{
		return cur != 0;
	}

	// Get the current object
	Entry* current() const
	{
		return cur;
	}

	// Restart the iterator
	void restart();

	// Next/previous object
	Entry* operator ++(int)
	{
		Entry* temp = cur;

		iterate();
		return temp;
	}
	Entry* operator ++()
	{
		iterate();
		return cur;
	}

private:
	const HashTable<K, V>* HT;
	Entry* cur;
	int tableIndex;

	void iterate();

}; // HashTableIterator


//////////////////////////////////////////////////////////
//
// HashTable implementation
// =========
template <typename K, typename V>
HashTable<K, V>::HashTable(int size)
{
	table = new Entry*[tableSize = size];
	for (int i = 0; i < size; i++)
		table[i] = 0;
	numberOfEntries = 0;
}

template <typename K, typename V>
void
HashTable<K, V>::put(const K& key, const V& value)
{
	Entry* e = new Entry(key, value);
	Entry** bucket = &table[getHashValue(key)];

	e->next = *bucket;
	e->pprev = bucket;
	if (*bucket != 0)
		(*bucket)->pprev = &e->next;
	*bucket = e;
	numberOfEntries++;
}

template <typename K, typename V>
bool
HashTable<K, V>::remove(const K& key)
{
	Entry* e = find(key);

	if (e == 0)
		return false;
	remove(e);
	return true;
}

template <typename K, typename V>
void
HashTable<K, V>::clear()
{
	for (int i = 0; i < tableSize; i++)
		while (table[i] != 0)
			remove(table[i]);
}

template <typename K, typename V>
HashEntry<K, V>*
HashTable<K, V>::find(const K& key) const
{
	for (Entry* e = table[getHashValue(key)]; e != 0; e = e->next)
		if (e->key == key)
			return e;
	return 0;
}

template <typename K, typename V>
void
HashTable<K, V>::remove(Entry* e)
{
	*(e->pprev) = e->next;
	if (e->next != 0)
		e->next->pprev = e->pprev;
	delete e;
	numberOfEntries--;
}

template <typename K, typename V>
inline HashTableIterator<K, V>
HashTable<K, V>::iterator() const
{
	return HashTableIterator<K, V>(*this);
}


//////////////////////////////////////////////////////////
//
// HashTableIterator implementation
// =================
template <typename K, typename V>
void
HashTableIterator<K, V>::restart()
{
	cur = 0;
	tableIndex = 0;
	while (HT->table[tableIndex] == 0)
		if (++tableIndex >= HT->tableSize)
			return;
	cur = HT->table[tableIndex];
}

template <class K, class V>
void
HashTableIterator<K, V>::iterate()
{
	// If not at the end of list, then iterate to
	// the next entry.
	if (cur != 0)
		cur = cur->next;
	// If now at the end of list, then iterate to
	// the next list (if any).
	if (cur == 0)
		while (++tableIndex < HT->tableSize)
			if (HT->table[tableIndex] != 0)
			{
				cur = HT->table[tableIndex];
				return;
			}
}

} // end namespace Collections

} // end namespace System

#endif // __Hash_h
