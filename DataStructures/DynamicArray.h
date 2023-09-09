
#pragma once

#include <cstring>
#include <utility>

template<class T>
class DynamicArray
{
	DynamicArray() : arr(nullptr), size(0), capacity(0) {}
	DynamicArray(const DynamicArray& other)
	{
		copy(other);
	}
	DynamicArray(DynamicArray&& rvalue)
	{
		copy(rvalue);
	}
	DynamicArray& operator=(const DynamicArray& other)
	{
		if (this != &other)
		{
			DynamicArray temp{ other };
			free();
			copy(std::move(temp));
		}
		return *this;
	}
	DynamicArray& operator=(DynamicArray&& rvalue)
	{
		if (this != &rvalue)
		{
			free();
			copy(rvalue);
		}
		return *this;
	}
	~DynamicArray()
	{
		free();
	}

	size_t getSize() const     { return size; }
	size_t getCapacity() const { return capacity; }


private:
	void copy(const DynamicArray& other)
	{
		if (other.capacity != 0)
		{
			arr = new T[other.capacity];
			memcpy(arr, other.arr, other.size * sizeof(T));
		}
		else
			arr = nullptr;
		
		size     = other.size;
		capacity = other.capacity;
	}
	void copy(DynamicArray&& rvalue)
	{
		arr      = rvalue.arr;
		size     = rvalue.size;
		capacity = rvalue.capacity;

		rvalue.arr      = nullptr;
		rvalue.size     = 0;
		rvalue.capacity = 0;
	}
	void free()
	{
		delete[] arr;
	}

private:
	T* arr;
	size_t size;
	size_t capacity;

	// How much bigger the new array is after the current one gets full.
	// Must be > 1.
	static const double RESIZE_COEFFICIENT = 2.0;
};