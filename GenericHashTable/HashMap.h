#pragma once
#include "HashNode.h"
#include <iostream>
#include <fstream>

// Uses std::hash as default hash function
template <typename T, size_t tableSize>
struct HashFunc
{
	size_t operator()(const T& key) const
	{
		//return std::hash<T>() (key) % tableSize;
		return key % tableSize;
	}
};

// Hash map class template
template <typename K, typename V, size_t tableSize, typename F = HashFunc<K, tableSize>>
class HashMap
{
private:
	F hashFunc;
	HashNode<K, V>** hashtable;

public:

	HashMap()
	{
		hashtable = new HashNode<K, V> * [tableSize];

		for (int i = 0; i < tableSize; i++)
			hashtable[i] = nullptr;
	}

	//~HashMap()
	//{

	//}

	V get(const K& key)
	{
		return hashtable[key];
	}

	void put(const K& key, const V& value)
	{
		HashNode<K, V>* temp = new HashNode<K, V>(key, value);

		unsigned long index = hashFunc(key);

		// Linear search for free space.
		while (hashtable[index] != nullptr
			&& hashtable[index]->key != key)
		{
			index++;
			index = index % tableSize;
		}
		
		hashtable[index] = temp;
	}

	V remove(const K& key)
	{
		unsigned long index = hashFunc(key);

		// Searching for a node.
		while (hashtable[index] != nullptr)
		{
			if (hashtable[index]->key == key)
			{
				HashNode<K, V>* temp = hashtable[index];
				hashtable[index] = nullptr;

				return temp->value;
			}
			index++;
			index = index % tableSize;
		}

		return nullptr;
	}

	static HashNode<K, V>* getHashTable(size_t i)
	{
		return hashtable[i];

	}

	static K getHashTableKey(size_t i)
	{
		return hashtable[i]->key;
	}	

	static V getHashTableValue(size_t i)
	{
		return hashtable[i]->value;
	}

	friend std::ostream& operator<<(std::ostream& out, HashMap<K, V, tableSize, HashFunc<K, tableSize>>& hmap)
	{
		
		for (int i = 0; i < tableSize; i++)
		{
			if (hmap.getHashTable(i) != nullptr)
				out << "key = " << hmap.getHashTableKey(i)
				<< "  value = " << hmap.getHashTableValue(i) << '\n';
		}
		return out;
	}
};