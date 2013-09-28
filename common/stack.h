#pragma once

#include <stdlib.h>
#include "scrapexcept.h"

using namespace std;

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
		mFront = 0;
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
			throw StackOverflowException();
		}

		Node *node = new Node();
		node->val = val;
		node->prev = mBack;
		
		if (mBack) {
			mBack->next = node;
		}

		mBack = node;

		if (mSize == 1) {
			mFront = mBack;
		}
	}

	T Pop() {
		if (--mSize < 0) {
			throw StackUnderflowException();
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

	T Peek(int index) {
		Node *node = mFront;
		
		for (int i=0; i<index && node; i++) {
			node = node->next;
		}

		return node->val;
	}

	int Size() {
		return mSize;
	}

private:
	int 			mMaxSize;
	int 			mSize;

	Node			*mFront;
	Node 			*mBack;
};