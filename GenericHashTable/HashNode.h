#pragma once
// Hash node class template
template <typename K, typename V>
class HashNode
{
private:
	K key;
	V value;
	HashNode* next;

public:
	HashNode(const K& _key, const V& _value)
	{
		key = _key;
		value = _value;
		next = nullptr;
	}

	K getKey() const
	{
		return key;
	}

	V getValue() const
	{
		return value;
	}

	void setValue(V _value)
	{
		value = _value;
	}

	HashNode* getNext() const
	{
		return next;
	}

	void setNext(HashNode* _next)
	{
		next = _next;
	}
};