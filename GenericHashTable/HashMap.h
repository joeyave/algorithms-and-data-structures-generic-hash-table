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
		else if (isdigit(word[0]))
			return (word[0] - 48) + 26;
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
				trav = trav->next;
				delete prev;
			}

			hashtable[i] = nullptr;
		}
	}

	V get(const K& key)
	{
		unsigned long index = hashFunc(key);
		HashNode<K, V>* trav = hashtable[index];

		while (trav != nullptr) 
		{
			if (trav->key == key) 
			{
				return trav->val;
			}

			trav = trav->next;
		}

		return nullptr;
	}

	void put(const K& key, const V& value)
	{
		unsigned long index = hashFunc(key);
		HashNode<K, V>* trav = hashtable[index];

		HashNode<K, V>* new_node = new HashNode<K, V>(key, value);

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
		HashNode<K, V>* trav;
		HashNode<K, V>* prev = nullptr;
		HashNode<K, V>* next;
		
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

			out << "\n\n--------------------- " << "BUCKET " << i << " ---------------------\n\n";

			while (trav != nullptr)
			{
				out << "key = " << trav->key <<
					" value = " << trav->val << '\n';
				trav = trav->next;
			}
		}

		return out;
	}

	friend std::ostream& operator<<(std::ostream& out, HashMap<K, V, tableSize, HashByFirstChar<K, tableSize>>& hmap)
	{
		for (size_t i = 0; i < tableSize; ++i)
		{
			HashNode<K, V>* trav = hmap.getHashtable(i);

			if (trav == nullptr)
				continue;

			out << "\n\n--------------------- " << "BUCKET " << i << " ---------------------\n\n";

			while (trav != nullptr)
			{
				out << "key = " << trav->key <<
					" value = " << trav->val << '\n';
				trav = trav->next;
			}
		}

		return out;
	}
};
