#pragma once
// Hash node class template
template <typename K>
struct HashNode
{
	K key;
	HashNode* next;

	HashNode(const K& _key)
	{
		key = _key;
		next = nullptr;
	}
};