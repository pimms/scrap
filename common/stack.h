#pragma once

#include <exception>
#include <stdlib.h>

using namespace std;

class OverflowException : public exception {
public:
	using exception::what;

	const char* what() {
		return "Stack overflow!\n";
	}
};

class UnderflowException : public exception {
public:
	using exception::what;

	const char* what() {
		return "Stack underflow!\n";
	}
};


template<typename T>
class Stack {
private:
	struct Node {
		Node() {
			prev = next = 0;
		}

		T 			val;
		Node 		*prev;
		Node 		*next;
	};


public:
	Stack(int maxSize = 2048) {
		mMaxSize = maxSize;
		mSize = 0;
		mBack = 0;
	}

	~Stack() {
		while (mBack) {
			Node *prev = mBack->prev;
			delete mBack;

			mBack = prev;
		}
	}

	void Push(T val) {
		if (++mSize >= mMaxSize) {
			throw OverflowException();
		}

		Node *node = new Node();
		node->val = val;
		node->prev = mBack;
		
		if (mBack) {
			mBack->next = node;
		}

		mBack = node;
	}

	T Pop() {
		if (--mSize < 0) {
			throw UnderflowException();
		}

		Node *node = mBack;
		mBack = mBack->prev;

		T ret = node->val;
		delete node;

		return ret;
	}

	T Peek() {
		if (mBack != NULL) {
			return mBack->val;
		}

		return NULL;
	}

private:
	int 			mMaxSize;
	int 			mSize;

	Node 			*mBack;
};