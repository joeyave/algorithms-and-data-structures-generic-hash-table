#pragma once
// Hash node class template
template <typename K, typename V>
struct HashNode
{
	K key;
	V value;
	HashNode* next;

	HashNode(const K& _key, const V& _value)
	{
		key = _key;
		value = _value;
		next = nullptr;
	}

	K getKey()
	{
		return key;
	}

	V getValue()
	{
		return value;
	}
};