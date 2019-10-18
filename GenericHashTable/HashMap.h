#pragma once
#include "HashNode.h"
#include <iostream>
#include <fstream>

template <typename T, size_t tableSize>
struct HashByFirstChar
{
	unsigned long operator()(const T& word) const
	{
		if (isalpha(word[0]))
			return (tolower(word[0]) - 'a') % tableSize;
		else
			return tableSize - 1;
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
template <typename K, size_t tableSize, typename F = HashFunc<K, tableSize>>
class HashMap
{
private:
	F hashFunc;
	HashNode<K>** hashtable;
	size_t used_buckets = 0;

public:
	HashMap()
	{ 
		hashtable = new HashNode<K>*[tableSize]();
	}

	~HashMap()
	{
		// destroy all buckets one by one
		for (size_t i = 0; i < tableSize; ++i) 
		{
			HashNode<K>* trav = hashtable[i];

			while (trav != nullptr) 
			{
				HashNode<K>* prev = trav;
				trav = trav->next;
				delete prev;
			}

			hashtable[i] = nullptr;
		}
	}

	bool get(const K& key, K& value)
	{
		unsigned long index = hashFunc(key);
		HashNode<K>* trav = hashtable[index];

		while (trav != nullptr) 
		{
			if (trav->key == key) 
			{
				value = trav->val;
				return true;
			}

			trav = trav->next;
		}

		return false;
	}

	void put(const K& key)
	{
		unsigned long index = hashFunc(key);
		HashNode<K>* trav = hashtable[index];

		HashNode<K>* new_node = new HashNode<K>(key);

		if (hashtable[index] == nullptr)
			hashtable[index] = new_node;
		else
		{
			// Make next of new_node as head and prev as nullptr.
			new_node->next = hashtable[index];
			hashtable[index] = new_node;
		}
	}
	
	void remove(const K& key)
	{
		unsigned long index = hashFunc(key);
		HashNode<K>* trav;
		HashNode<K>* prev = nullptr;
		HashNode<K>* next;
		
		for (trav = hashtable[index]; trav != nullptr; trav = next)
		{
			next = trav->next;

			if (trav->key != key)
			{
				prev = trav;
				continue;
			}

			if (prev != nullptr)
				prev->next = next;
			else
				hashtable[index] = next;

			delete trav;
		}
	}

	HashNode<K>* getHashtable(size_t i)
	{
		return hashtable[i];
	}

	size_t getUsedBuckets()
	{
		return used_buckets;
	}

	friend std::ostream& operator<<(std::ostream& out, HashMap<K, tableSize, HashFunc<K, tableSize>>& hmap)
	{
		for (size_t i = 0; i < tableSize; ++i)
		{
			HashNode<K>* trav = hmap.getHashtable(i);

			if (trav == nullptr)
				continue;

			out << "\n\n--------------------- " << "BUCKET " << i << " ---------------------\n\n";

			while (trav != nullptr)
			{
				out << trav->key << " | ";
				trav = trav->next;
			}
		}

		return out;
	}

	friend std::ostream& operator<<(std::ostream& out, HashMap<K, tableSize, HashByFirstChar<K, tableSize>>& hmap)
	{
		for (size_t i = 0; i < tableSize; ++i)
		{
			HashNode<K>* trav = hmap.getHashtable(i);

			if (trav == nullptr)
				continue;

			out << "\n\n--------------------- " << "BUCKET " << i << " ---------------------\n\n";

			while (trav != nullptr)
			{
				out << trav->key << " | ";
				trav = trav->next;
			}
		}

		return out;
	}
};
