#pragma once
#include "HashNode.h"
#include <iostream>
#include <fstream>
struct MyKeyHash
{
	unsigned long operator()(const int& k) const
	{
		return k % 10;
	}
};

// Uses std::hash as default hash function
template <typename T, size_t tableSize>
struct HashFunc 
{
	size_t operator()(const T& key) const
	{
		return std::hash<T>() (key) % tableSize;
	}
};

// Hash map class template
template <typename K, typename V, size_t tableSize, typename F = HashFunc<K, tableSize>>
class HashMap
{
private:
	F hashFunc;
	HashNode<K, V>** hashtable;
	size_t used_buckets = 0;

public:

	HashMap()
	{ 
		hashtable = new HashNode<K, V>*[tableSize]();
	}

	~HashMap()
	{
		// destroy all buckets one by one
		for (size_t i = 0; i < tableSize; ++i) 
		{
			HashNode<K, V>* trav = hashtable[i];

			while (trav != nullptr) 
			{
				HashNode<K, V>* prev = trav;
				trav = trav->getNext();
				delete prev;
			}

			hashtable[i] = nullptr;
		}
	}

	bool get(const K& key, V& value)
	{
		unsigned long hashValue = hashFunc(key);
		HashNode<K, V>* trav = hashtable[hashValue];

		while (trav != nullptr) 
		{
			if (trav->getKey() == key) 
			{
				value = trav->getValue();
				return true;
			}

			trav = trav->getNext();
		}

		return false;
	}

	void put(const K& key, const V& value)
	{
		unsigned long hashValue = hashFunc(key);
		HashNode<K, V>* prev = nullptr;
		HashNode<K, V>* trav = hashtable[hashValue];

		while (trav != nullptr) 
		{
			prev = trav;
			trav = trav->getNext();
		}

		if (trav == nullptr) 
		{
			trav = new HashNode<K, V>(key, value);

			if (prev == nullptr) 
			{
				// insert as first bucket
				hashtable[hashValue] = trav;
				used_buckets++;
			}
			else
			{
				prev->setNext(trav);
			}
		}
		else 
		{
			// just update the value
			trav->setValue(value);
		}
	}

	void remove(const K& key)
	{
		unsigned long hashValue = hashFunc(key);
		HashNode<K, V>* prev = nullptr;
		HashNode<K, V>* trav = hashtable[hashValue];

		while (trav != nullptr && trav->getKey() != key) {
			prev = trav;
			trav = trav->getNext();
		}

		if (trav == nullptr) {
			// key not found
			return;

		}
		else {
			if (prev == nullptr) {
				// remove first bucket of the list
				hashtable[hashValue] = trav->getNext();

			}
			else {
				prev->setNext(trav->getNext());
			}

			delete trav;
		}
	}

	HashNode<K, V>* getHashtable(size_t i)
	{
		return hashtable[i];
	}

	size_t getUsedBuckets()
	{
		return used_buckets;
	}

	friend std::ostream& operator<<(std::ostream& out, HashMap<K, V, tableSize, HashFunc<K, tableSize>>& hmap)
	{
		for (size_t i = 0; i < tableSize; ++i)
		{
			HashNode<K, V>* trav = hmap.getHashtable(i);

			if (trav == nullptr)
				continue;

			while (trav != nullptr)
			{
				out << trav->getValue() << " | ";
				trav = trav->getNext();
			}
			out << "\n\n--------------------- " << "BUCKET " << i << " ---------------------\n\n";
		}

		return out;
	}

	friend std::ostream& operator<<(std::ostream& out, HashMap<K, V, tableSize, MyKeyHash>& hmap)
	{
		for (size_t i = 0; i < tableSize; ++i)
		{
			HashNode<K, V>* trav = hmap.getHashtable(i);

			if (trav == nullptr)
				continue;

			while (trav != nullptr)
			{
				out << trav->getValue() << " | ";
				trav = trav->getNext();
			}
			out << "\n\n--------------------- " << "BUCKET " << i << " ---------------------\n\n";
		}

		return out;
	}
};
