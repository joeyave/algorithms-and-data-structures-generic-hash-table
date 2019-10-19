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
		return std::hash<T>() (key) % tableSize;
		//return key % tableSize;
		//return key % 3;
	}
};

template <typename T, size_t tableSize>
struct MyHashFunc
{
	size_t operator()(const T& phonenumber) const
	{
		size_t mul = 1;
		for (size_t i = 0; phonenumber[i]; i++)
		{
			if ((phonenumber[i] - 48) > 0)
				mul *= (phonenumber[i] - 48);
		}

		return mul % tableSize;
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

	~HashMap()
	{
		for (size_t i = 0; i < tableSize; i++)
		{
			delete hashtable[i];
		}
		delete[] hashtable;
	}

	V get(const K& key)
	{
		unsigned long index = hashFunc(key);

		size_t i = 0;
		while (hashtable[index] != nullptr)
		{
			int i = 0;
			if (i++ > tableSize)
				return nullptr;

			if (hashtable[index]->key == key)
				return hashtable[index]->value;

			index++;
			index = index % tableSize;
		}

		return nullptr;
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

	void remove(const K& key)
	{
		unsigned long index = hashFunc(key);

		// Searching for a node.
		while (hashtable[index] != nullptr)
		{
			if (hashtable[index]->key == key)
			{
				HashNode<K, V>* temp = hashtable[index];
				hashtable[index] = nullptr;
			}
			index++;
			index = index % tableSize;
		}
	}

	HashNode<K, V>** getHashTable()
	{
		return hashtable;
	}

	friend std::ostream& operator<<(std::ostream& out, HashMap<K, V, tableSize, HashFunc<K, tableSize>>& hmap)
	{
		HashNode<K, V>** temp = hmap.getHashTable();

		for (int i = 0; i < tableSize; i++)
		{
			if (temp[i] != nullptr)
				out << i <<". key = " << temp[i]->getKey()
				<< "  value = " << temp[i]->getValue() << '\n';
		}
		return out;
	}

	friend std::ostream& operator<<(std::ostream& out, HashMap<K, V, tableSize, MyHashFunc<K, tableSize>>& hmap)
	{
		HashNode<K, V>** temp = hmap.getHashTable();

		for (int i = 0; i < tableSize; i++)
		{
			if (temp[i] != nullptr)
				out << i << ". key = " << temp[i]->getKey()
				<< "  value = " << temp[i]->getValue() << '\n';
		}
		return out;
	}
};