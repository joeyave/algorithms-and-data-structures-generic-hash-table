#pragma once
// Hash node class template
template <typename K, typename V>
struct HashNode
{
	K key;
	V val;
	HashNode* next;

	HashNode(const K& _key, const K& _val)
	{
		key = _key;
		val = _val;
		next = nullptr;
	}
};