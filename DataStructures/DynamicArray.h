
#pragma once

#include <algorithm>
#include <exception>
#include <stdexcept>
#include <utility>

template<class T>
class DynamicArray
{
public:
	DynamicArray() : arr(nullptr), size(0), capacity(0) {}
	// Initializer list constructor.
	// 'std::initializer_list<>' is a reference type so passing by value is OK.
	DynamicArray(std::initializer_list<T> list)
		: arr(new T[list.size()]), size(list.size()), capacity(list.size())
	{
		try {
			std::copy(list.begin(), list.end(), arr);
		}
		catch (...) {
			free();
			throw "Failure constructing object of class DynamicArray from initializer list!";
		}
	}
	DynamicArray(const DynamicArray& other)
	{
		try {
			copy(other);
		}
		catch (...) {
			free();
			throw "Failure constructing object of class DynamicArray from another object!";
		}
	}
	DynamicArray(DynamicArray&& rvalue) noexcept
	{
		copy(std::move(rvalue));
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
	DynamicArray& operator=(DynamicArray&& rvalue) noexcept
	{
		if (this != &rvalue)
		{
			free();
			copy(std::move(rvalue));
		}
		return *this;
	}
	~DynamicArray()
	{
		free();
	}

	size_t getSize() const     { return size; }
	size_t getCapacity() const { return capacity; }

	T& at(size_t index)
	{
		if (index < size)
			return arr[index];
		else
			throw std::out_of_range("Function 'at()' called with index >= size.");
	}
	const T& at(size_t index) const
	{
		if (index < size)
			return arr[index];
		else
			throw std::out_of_range("Function 'at() const' called with index >= size.");
	}
	// Doesn't check if index is valid.
	T& operator[](size_t index) { return arr[index]; } 
	// Doesn't check if index is valid.
	const T& operator[](size_t index) const { return arr[index]; }

	// Makes the size and capacity of the array exactly 'newSize'.
	void resize(size_t newSize)
	{
		if (newSize <= capacity)
		{
			size = newSize;
			return;
		}
			
		T* newArr = allocAndCopy(newSize);
		free();
		arr      = newArr;
		size     = newSize;
		capacity = newSize;
	}
	// Reduces 'capacity' to 'size'.
	void shrinkToFit()
	{
		if (size == capacity)
			return;

		T* newArr = allocAndCopy(size);
		free();
		arr      = newArr;
		capacity = size;
	}
	// Makes space for at least the given number of elements.
	void reserve(size_t newCapacity)
	{
		if (newCapacity <= capacity)    // Already have enough space. 
			return;

		// Increase capacity of buffer by a factor of at least 'RESIZE_COEFFICIENT'.
		T* newArr = allocAndCopy(std::max(newCapacity, static_cast<size_t>(capacity * RESIZE_COEFFICIENT)));
		free();
		arr      = newArr;
		capacity = newCapacity;
	}

	// Adds the given element at the end.
	void push(const T& obj)
	{
		if (size == capacity)
			reserve(capacity + 1);

		arr[size++] = obj;
	}
	void push(T&& rvalue)
	{
		if (size == capacity)
			reserve(capacity + 1);
		
		arr[size++] = std::move(rvalue);
	}
	// Removes last element.
	// Throws std::logic_error if array is empty.
	void pop()
	{
		size != 0 ? --size : throw std::logic_error("Can't pop last element from empty dynamic array!");
	}
	// Inserts element at given position.
	// Throws std::out_of_range if index >= size.
	void insertAt(const T& obj, size_t index)
	{
		if (index > size)
			throw std::out_of_range("DynamicArray: function 'insertAt()' called with index >= size!");

		if (size == capacity)
			reserve(capacity + 1);

		/*
			Note: this can be made fasted because at the moment the elements in the array are
			copied 2 times: one time 'reserve()' and one more time in the below for loop.
		*/
		// Make space for the new element.
		for (size_t i = size; i > index; --i)   // Maybe should use std::copy_backward.
			arr[i] = std::move(arr[i - 1]);

		arr[index] = obj;
		++size;
	}

	void sort()
	{
		if (size == 0)
			return;
		else
			quickSort(arr, arr + size - 1);
	}

private:
	// Throws std::bad_alloc if memory allocation fails.
	// Must be called with 'newCapacity >= this->size'.
	T* allocAndCopy(size_t newCapacity)
	{
		T* newArr = new T[newCapacity];
		std::copy(arr, arr + size, newArr);
		return newArr;
	}
	void copy(const DynamicArray& other)
	{
		if (other.capacity != 0)
		{
			arr = new T[other.capacity];
			std::copy(other.arr, other.arr + other.size, arr);
		}
		else
			arr = nullptr;
		
		size     = other.size;
		capacity = other.capacity;
	}
	void copy(DynamicArray&& rvalue) noexcept
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

	/*
		- Chooses and element called pivot and rearranges the elements in 'array' so that
		  every element smaller than pivot is before pivot, then pivot, then every element
		  greater or equal to pivot.
		- Returns position of pivot.
		- len must be > 0.
	*/ 
	T* partition(T* arrBeg, T* arrEnd)
	{
		T* pivot = arrEnd;   // Choose pivot to be the last element.
		T* pp    = arrBeg;   // Pivot position in the final array.
		for (T* i = arrBeg; i < arrEnd; ++i)
		{
			if (*i < *pivot)
			{
				if (i != pp)
					std::swap(*i, *pp);
				++pp;
			}
		}
		if (pp != pivot)
			std::swap(*pp, *pivot);    // Place pivot in the correct position in the final array.
		
		return pp;    // Return the pivot position.
	}
	void quickSort(T* arrBeg, T* arrEnd)
	{
		if (arrBeg >= arrEnd)  // Arrays with 0 or 1 elements are sorted.
			return;

		T* pivotPosition = partition(arrBeg, arrEnd);
		quickSort(arrBeg, pivotPosition - 1);
		quickSort(pivotPosition + 1, arrEnd);
	}

private:
	T* arr{ nullptr };
	size_t size{ 0 };       // Number of elements stored.
	size_t capacity{ 0 };   // Size of the buffer.

	// How much bigger the new array is after the current one gets full.
	// MUST BE > 1.
	static const double RESIZE_COEFFICIENT;
};

template<typename T>
const double DynamicArray<T>::RESIZE_COEFFICIENT = 2.0;

template<typename T>
bool operator==(const DynamicArray<T>& left, const DynamicArray<T>& right)
{
	if (left.getSize() != right.getSize())
		return false;

	for (size_t i = 0; i < left.getSize(); ++i)
		if (left[i] != right[i])
			return false;

	return true;
}
template<typename T>
bool operator!=(const DynamicArray<T>& left, const DynamicArray<T>& right)
{
	return !operator==(left, right);
}