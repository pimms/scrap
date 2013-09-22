#pragma once

#include "var.h"
#include "../common/codes.h"
#include "../common/stack.h"

#include <map>
#include <stdlib.h>

using namespace std;


template<typename T>
class ScopeT {
public:
	~ScopeT() {
		typename map<uint,T>::iterator it;

		for (it = mVars.begin(); it != mVars.end(); it++) {
			delete it->second;
		}
	}

	bool ItemExists(uint id) {
		for (int i=mNested.Size()-1; i>=0; i--) {
			if (mNested.Peek(i)->ItemExists(id)) {
				return true;
			}
		}

		if (mVars.count(id) != 0) {
			return true;
		}

		return false;
	}

	void AddItem(uint id, T t) {
		ScopeT<T> *nested = mNested.Peek();
		if (nested) {
			nested->AddItem(id, t);
			return;
		}

		mVars[id] = t;
	}

	T GetVar(uint id) {
		for (int i=mNested.Size()-1; i>=0; i--) {
			T t = mNested.Peek(i)->GetVar(id);
			if (t) {
				return t;
			}
		}

		if (mVars.count(id) == 0) {
			return NULL;
		}

		return mVars[id];
	}

	void PushNestedScope() {
		ScopeT<T> *nested = new ScopeT<T>();
		mNested.Push(nested);
	}

	void PopNestedScope() {
		if (mNested.Size() > 0) {
			ScopeT<T> *nested = mNested.Pop();
			delete nested;
		} else {
			throw UnderflowException();
		}
	}

protected:
	map<uint,T>		mVars;
	Stack<ScopeT<T>*> 	mNested;
};

typedef ScopeT<Var*> Scope;
