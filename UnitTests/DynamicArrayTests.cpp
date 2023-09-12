#include "pch.h"
#include "CppUnitTest.h"

#include <array>      // For 'std::size()'
#include <exception>
#include <utility>    // 'std::move'
#include "../DataStructures/DynamicArray.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace DynArrTests
{
	TEST_CLASS(DynamicArrayTests)
	{
	private:
		// Arrays used in the tests.
		// You should be able to change the values in the initializer lists without breaking any of the tests. 
		const DynamicArray<int> array1;
		const DynamicArray<int> array2{ -2, 5, 7, -4, 0 };
		const DynamicArray<DynamicArray<int>> array3{ {}, { 1, -19, 7 }, { 3 , 4, 5 }, { 42 } };

		// Returns true if size and capacity of 'arr1' and 'arr2' are equal
		// and if all their elements are equal.
		template<typename T> 
		static bool areEqual(const DynamicArray<T>& arr1, const DynamicArray<T>& arr2)
		{
			return arr1 == arr2 && arr1.getCapacity() == arr2.getCapacity();
		}

		TEST_METHOD(areEqualTest)
		{
			DynamicArray<int> obj1, obj2;
			Assert::IsTrue(areEqual(obj1, obj2), { L"Function 'areEqual()' thinks two empty arrays are not equal!"});

			obj1.reserve(3); obj1.push(-6); obj1.push(8);    // Make the arrays different.
			Assert::IsFalse(areEqual(obj1, obj2), { L"Function 'areEqual()' doesn't recognize different arrays!" });

			obj2 = obj1;    // Make arrays the same.
			Assert::IsTrue(areEqual(obj1, obj2), { L"Function 'areEqual()' doesn't recognize equal arrays!" });

			// Is an array equal to itself.
			Assert::IsTrue(areEqual(obj1, obj1), { L"Function 'areEqual()' doesn't think an array is equal to itself!" });



			// Test with more complex template parameter.
			DynamicArray<DynamicArray<int>> arr3Copy(array3);
			Assert::IsTrue(areEqual(arr3Copy, array3));
			
			arr3Copy[1].push(-6);  // Make a change.
			Assert::IsFalse(areEqual(arr3Copy, array3));
		}

	public:

		TEST_METHOD(defaultConstructorTest)
		{
			DynamicArray<bool> obj;
			Assert::AreEqual((size_t)0, obj.getSize());
			Assert::AreEqual((size_t)0, obj.getCapacity());
		}
		TEST_METHOD(initializerListConstructorTest)
		{
			DynamicArray<int> obj{ -2, 18, 7, -4, 0 };
			int initArr[]      = { -2, 18, 7, -4, 0 };

			Assert::AreEqual(obj.getSize(), std::size(initArr));
			Assert::AreEqual(obj.getSize(), obj.getCapacity());

			for (size_t i = 0; i < obj.getSize(); ++i)
				Assert::AreEqual(obj[i], initArr[i]);
		}
		TEST_METHOD(copyConstructorTest)
		{
			DynamicArray<int> arr1Copy(array1);
			DynamicArray<int> arr2Copy(array2);
			DynamicArray<DynamicArray<int>> arr3Copy(array3);

			// Check if arrays are copied correctly.
			Assert::IsTrue(areEqual(arr1Copy, array1));
			Assert::IsTrue(areEqual(arr2Copy, array2));
			Assert::IsTrue(areEqual(arr3Copy, array3));

			Assert::IsFalse(areEqual(arr1Copy, arr2Copy));   // Copeies of different arrays must be different.
		}
		TEST_METHOD(moveConstructorTest)
		{
			DynamicArray<int> arr1Copy(array1);
			DynamicArray<int> arr2Copy(array2);
			DynamicArray<DynamicArray<int>> arr3Copy(array3);

			// Move.
			DynamicArray<int> arr1MoveCopy(std::move(arr1Copy));
			DynamicArray<int> arr2MoveCopy(std::move(arr2Copy));
			DynamicArray<DynamicArray<int>> arr3MoveCopy(std::move(arr3Copy));

			// Check if arrays are copied correctly.
			Assert::IsTrue(areEqual(arr1MoveCopy, array1));
			Assert::IsTrue(areEqual(arr2MoveCopy, array2));
			Assert::IsTrue(areEqual(arr3MoveCopy, array3));

			Assert::IsFalse(areEqual(arr1MoveCopy, arr2MoveCopy));   // Copies of different arrays must be different.

			// Check if move constructor nullifies the rvalue.
			size_t zero = 0;
			Assert::AreEqual(zero, arr1Copy.getSize());    Assert::AreEqual(zero, arr1Copy.getCapacity());
			Assert::AreEqual(zero, arr2Copy.getSize());    Assert::AreEqual(zero, arr2Copy.getCapacity());
			Assert::AreEqual(zero, arr3Copy.getSize());    Assert::AreEqual(zero, arr3Copy.getCapacity());
		}
		TEST_METHOD(assignmentOperatorTest)
		{
			DynamicArray<int> arr1Copy;                   arr1Copy = array1;
			DynamicArray<int> arr2Copy;                   arr2Copy = array2;
			DynamicArray<DynamicArray<int>> arr3Copy;     arr3Copy = array3;

			// Check if arrays are copied correctly.
			Assert::IsTrue(areEqual(arr1Copy, array1));
			Assert::IsTrue(areEqual(arr2Copy, array2));
			Assert::IsTrue(areEqual(arr3Copy, array3));
			
			Assert::IsFalse(areEqual(arr1Copy, arr2Copy));   // Copies of different arrays must be different.
		}
		TEST_METHOD(moveAssignmentOperatorTest)
		{
			DynamicArray<int> arr1Copy(array1);
			DynamicArray<int> arr2Copy(array2);
			DynamicArray<DynamicArray<int>> arr3Copy(array3);

			DynamicArray<int> arr1MoveAssing;                 arr1MoveAssing = std::move(arr1Copy);
			DynamicArray<int> arr2MoveAssing;                 arr2MoveAssing = std::move(arr2Copy);
			DynamicArray<DynamicArray<int>> arr3MoveAssing;   arr3MoveAssing = std::move(arr3Copy);

			// Check if arrays are copied correctly.
			Assert::IsTrue(areEqual(arr1MoveAssing, array1));
			Assert::IsTrue(areEqual(arr2MoveAssing, array2));
			Assert::IsTrue(areEqual(arr3MoveAssing, array3));

			Assert::IsFalse(areEqual(arr1MoveAssing, arr2MoveAssing));   // Copies of different arrays must be different.

			// Check if move constructor nullifies the rvalue.
			const size_t zero = 0;
			Assert::AreEqual(zero, arr1Copy.getSize());    Assert::AreEqual(zero, arr1Copy.getCapacity());
			Assert::AreEqual(zero, arr2Copy.getSize());    Assert::AreEqual(zero, arr2Copy.getCapacity());
			Assert::AreEqual(zero, arr3Copy.getSize());    Assert::AreEqual(zero, arr3Copy.getCapacity());
		}

		TEST_METHOD(getSizeTest)
		{
			const DynamicArray<char> obj1, obj2{ 'a', 'b', 'c', 'd' };
			Assert::AreEqual(static_cast<size_t>(0), obj1.getSize());
			Assert::AreEqual(static_cast<size_t>(4), obj2.getSize());
		}
		TEST_METHOD(getCapacityTest)
		{
			const DynamicArray<double> obj1, obj2{ 2.7, 3.14, 1.68 };
			Assert::AreEqual(static_cast<size_t>(0), obj1.getCapacity());
			Assert::AreEqual(static_cast<size_t>(3), obj2.getCapacity());
		}
		TEST_METHOD(atTest)
		{
			DynamicArray<char> obj{ 'a', 'b', 'c', 'd', 'e' };
			char arrObj[]       = { 'a', 'b', 'c', 'd', 'e' };
			
			// Test 'at()'.
			for (size_t i = 0; i < obj.getSize(); ++i)
				Assert::AreEqual(obj.at(i), arrObj[i]);
			try {
				obj.at(obj.getSize());
				Assert::Fail({ L"Buffer overrun by function 'at()' in class 'DynamicArray!" });
			}
			catch (...) {}

			// Test 'at() const'.
			DynamicArray<char>& rObj = obj;
			for (size_t i = 0; i < rObj.getSize(); ++i)
				Assert::AreEqual(rObj.at(i), arrObj[i]);
			try {
				rObj.at(rObj.getSize());
				Assert::Fail({ L"Buffer overrun by function 'at() const' in class 'DynamicArray!" });
			}
			catch (...) {}
		}
		TEST_METHOD(arrayIndexOperatorTest)
		{
			DynamicArray<char> obj{ 'a', 'b', 'c', 'd', 'e' };
			char arrObj[]       = { 'a', 'b', 'c', 'd', 'e' };

			// Test non const 'operator[]'.
			for (size_t i = 0; i < obj.getSize(); ++i)
				Assert::AreEqual(obj[i], arrObj[i]);

			// Test const 'operator[]'.
			DynamicArray<char>& rObj = obj;
			for (size_t i = 0; i < rObj.getSize(); ++i)
				Assert::AreEqual(rObj[i], arrObj[i]);
		}

		TEST_METHOD(resizeTest)
		{
			DynamicArray<int> obj1;
			obj1.resize(0);
			Assert::IsTrue(0 == obj1.getSize(), { L"'resize()' doesn't keep array the same size!" });

			obj1.resize(1);
			Assert::IsTrue(0 < obj1.getSize(), { L"'resize()' doesn't resize correctly and empty array!" });

			DynamicArray<int> obj2(array2);
			obj2.resize(obj2.getSize() + 1);
			Assert::IsTrue(array2.getSize() < obj2.getSize());

			for (size_t i = 0; i < array2.getSize(); ++i)
				Assert::AreEqual(obj2[i], array2[i], { L"'resize()' changes the elements!" });
		}
		TEST_METHOD(shrinkToFitTest)
		{
			DynamicArray<DynamicArray<int>> obj(array3);
			obj.shrinkToFit();
			Assert::IsTrue(obj.getCapacity() == array3.getCapacity(), { L"'shrinkToFit()' changes capacity when it shouldn't!" });

			obj.reserve(100);
			obj.shrinkToFit();
			Assert::AreEqual(obj.getCapacity(), array3.getCapacity());
		}
		TEST_METHOD(reserveTest)
		{
			DynamicArray<int> obj(array2);
			obj.reserve(obj.getCapacity());
			Assert::IsTrue(obj.getCapacity() == array2.getCapacity(), { L"'reserve()' changes capacity when it shouldn't!" });

			obj.reserve(100);
			Assert::IsTrue(100 <= obj.getCapacity(), { L"'reserve()' doesn't reserve the given capacity!" });
		}

		TEST_METHOD(pushTest)
		{
			DynamicArray<DynamicArray<int>> array;   DynamicArray<int> subArray{ 'a', 'b', 'c' };
			array.push(subArray);
			Assert::IsTrue(areEqual(array[0], subArray));    // Test 'push()'.
			array.push(std::move(subArray));
			Assert::IsTrue(areEqual(array[0], array[1]));    // Test move 'push()'.

			// Check if move 'push()' nullifies the rvalue.
			Assert::IsTrue(0 == subArray.getSize());    Assert::IsTrue(0 == subArray.getCapacity());
		}
		TEST_METHOD(popTest)
		{
			DynamicArray<int> obj1;
			try {
				obj1.pop();
				Assert::Fail({ L"'pop()' doesn't throw exception for empty array!" });
			} 
			catch (std::logic_error& err) {}

			DynamicArray<int> obj2(array2);   obj2.pop();
			Assert::AreEqual(obj2.getSize(), array2.getSize() - 1, { L"'pop()' doesn't reduce size!" });
		}
		TEST_METHOD(insertAtTest)
		{
			DynamicArray<int> obj{ 1, 2, 3 };
			try {
				obj.insertAt(2, obj.getSize() + 1);
				Assert::Fail({ L"'insertAt()' doesn't throw when out of range!" });
			}
			catch (std::out_of_range& err) {}

			obj.insertAt(34789, obj.getSize());
			Assert::AreEqual(34789, obj[obj.getSize() - 1], { L"'insertAt()' fails to insert at the back!"});

			obj.insertAt(564, 0);
			Assert::AreEqual(564, obj[0], { L"'insertAt()' fails to insert at the front!" });
		}

		TEST_METHOD(sortTest)
		{
			DynamicArray<char> unsorted{ 'c', 'b', 'a' };
			DynamicArray<char> sorted  { 'a', 'b', 'c' };
			unsorted.sort();
			Assert::IsTrue(areEqual(sorted, unsorted));

			DynamicArray<int> unsorted1{ -1, 0, 18, -4, 7, 6, 1 };
			DynamicArray<int> sorted1  { -4, -1, 0, 1, 6, 7, 18 };
			unsorted1.sort();
			Assert::IsTrue(areEqual(sorted1, unsorted1));
		}

		TEST_METHOD(equalityOperatorsTest)
		{
			DynamicArray<int> obj1(array2), obj2(array2);
			Assert::IsTrue (obj1 == obj1);
			Assert::IsFalse(obj1 != obj1);

			obj1.reserve(obj1.getCapacity() * 10 + 10);   // Changing only the capacity should keep the arrays equal.
			Assert::IsTrue (obj1 == obj2, { L"Equality operator differentiates between arrays with different capacity!"});
			Assert::IsFalse(obj1 != obj2);
		}
	};
}