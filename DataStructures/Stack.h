
#pragma once

#include <stdexcept>
#include "DynamicArray.h"

template<typename T>
class Stack : private DynamicArray<T>
{
public:

	void push(const T& obj)
	{
		DynamicArray<T>::push(obj);
	}
	void pop()
	{
		if (isEmpty())
			throw std::logic_error("Can't pop last element because stack is empty!");
		
		DynamicArray<T>::pop();
	}
	T& peek()
	{
		if (isEmpty())
			throw std::logic_error("Can't peek last element in stack because stack is empty!");
	
		operator[](getSize() - 1);
	}
	// Returns true if the stack is empty.
	bool isEmpty() const
	{
		return !getSize();
	}
};