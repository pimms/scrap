#pragma once

#include "var.h"
#include "../common/codes.h"
#include "../common/stack.h"

#include <map>
#include <stdlib.h>
#include <string>

using namespace std;


template<typename T>
class ScopeT {
public:
	~ScopeT() {
		typename map<uint,T>::iterator it;

		for (it = mVars.begin(); it != mVars.end(); it++) {
			delete it->second;
		}

		while (mNested.Size()) {
			delete mNested.Pop();
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
		delete mNested.Pop();
	}

protected:
	map<uint,T>			mVars;
	Stack<ScopeT<T>*> 	mNested;
};


// The default scope, used at runtime. IDs
// are mapped directly to the Var-objects.
typedef ScopeT<Var*> Scope;

// The scope used for scope-checking during
// compilation. IDs are mapped to variable
// names rather than actual variables.
typedef ScopeT<string*> CompileScope;