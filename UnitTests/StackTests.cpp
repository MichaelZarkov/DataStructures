#include "pch.h"
#include "CppUnitTest.h"

#include "../DataStructures/Stack.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace StackTests
{
	TEST_CLASS(StackTests)
	{
		TEST_METHOD(pushTest)
		{
			// Test with simple template argument.
			Stack<int> stack1;  stack1.push(5);
			Assert::AreEqual(5, stack1.peek());
			stack1.push(3);
			Assert::AreEqual(3, stack1.peek());

			// Test with more complex template argument.
			Stack<DynamicArray<int>> stack2;   DynamicArray<int> array{ 1, 2, 3 };
			stack2.push(array);
			Assert::IsTrue(array == stack2.peek());
		}
		TEST_METHOD(popTest)
		{
			Stack<char> stack;  stack.push('a');  stack.push('b');  stack.push('c');
			stack.pop();
			Assert::AreEqual('b', stack.peek());
			stack.pop();
			Assert::AreEqual('a', stack.peek());
			stack.pop();
			Assert::IsTrue(stack.isEmpty());

			try {
				stack.pop();
				Assert::Fail({ L"'pop()' doesn't throw an exception when stack is empty!" });
			}
			catch (...) {}
		}
		TEST_METHOD(peekTest)
		{
			Stack<char> stack;
			try {
				stack.peek();
				Assert::Fail({ L"'peek()' doesn't throw and exception when stack is empty!" });
			}
			catch (...) {}

			stack.push('a');  Assert::AreEqual('a', stack.peek());
			stack.push('b');  Assert::AreEqual('b', stack.peek());
		}
		TEST_METHOD(isEmptyTest)
		{
			Stack<bool> stack;  Assert::IsTrue(stack.isEmpty());
			stack.push(true);   Assert::IsFalse(stack.isEmpty());
		}
	};
}