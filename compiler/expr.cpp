#include "expr.h"
#include "../common/stack.h"
#include "../common/scrapexcept.h"
#include "interop.h"
#include "func.h"

Expression::Expression(bool isFunctionParam, bool canBeNull) {
	mIsParam = isFunctionParam;
	mCanBeNull = canBeNull;
}

Expression::~Expression() {
	while (mPostfix.size()) {
		delete mPostfix.front();
		mPostfix.pop_front();
	}
}

string Expression::DbgGetString() {
	string ret;

	list<ExprTerm*>::iterator it;
	for (it = mPostfix.begin(); it != mPostfix.end(); it++) {
		if ((*it)->mToken != NULL) {
			ret += (*it)->mToken->mToken + " ";
		} else {
			ret += (*it)->mFunc->DbgGetString();
		}
	}
	
	return ret;
}


void Expression::ParseFragment(Tokens *tokens, Parser *parser) {
	mPostfix.clear();

	BuildPostfix(tokens, parser);

	if (!mCanBeNull && !mPostfix.size()) {
		throw InvalidTokenException("Expected expression");
	}
}

void Expression::BuildPostfix(Tokens *tokens, Parser *parser) {
	Stack<Token*> stack;

	// Keeps the loop going when parsing a function-parameter expr
	int paranthCnt = 0;

	const Token *next = tokens->PeekNext();
	while (tokens->HasMore() && next->mType != Token::SEMICOLON 
							 && next->mType != Token::COMMA) {

		if (next->mType == Token::VAL_INT || next->mType == Token::VAL_FLOAT) {
			mPostfix.push_back(new ExprTerm(tokens->PopNext()));
		} else if (next->mType == Token::VARFUNC) {
			Token *token = tokens->PopNext();

			// Function call
			if (tokens->PeekNext()->mType == Token::PARANTH_BEG) {
				FunctionCall *func = new FunctionCall(token);
				func->ParseFragment(tokens, parser);
				mPostfix.push_back(new ExprTerm(func));
			} else {
				mPostfix.push_back(new ExprTerm(token));
			}
		} else if (next->mType & Token::OPERATOR) {
			Token *token = tokens->PopNext();

			while (stack.Size() && stack.Peek()->mType != Token::PARANTH_BEG) {
				if (OperatorPrecedence(stack.Peek()) > OperatorPrecedence(token)) {
					mPostfix.push_back(new ExprTerm(stack.Pop()));
				} else {
					break;
				}
			}

			stack.Push(token);
		} else if (next->mType == Token::PARANTH_BEG) {
			paranthCnt++;
			stack.Push(tokens->PopNext());
		} else if (next->mType == Token::PARANTH_END) {
			paranthCnt--;
			if (paranthCnt < 0) break;

			while (stack.Size() && stack.Peek()->mType != Token::PARANTH_BEG) {
				mPostfix.push_back(new ExprTerm(stack.Pop()));
			}
			
			// Pop the "("
			stack.Pop();

			// Pop the ")"
			tokens->PopNext();
		} else {
			throw InvalidTokenException("Unexpected token: " + next->mToken);
		}
		
		next = tokens->PeekNext();
	}

	while (stack.Size()) {
		mPostfix.push_back(new ExprTerm(stack.Pop()));
	}

	if (!mIsParam) {
		delete tokens->PopExpected(Token::SEMICOLON);
	}
}

int Expression::OperatorPrecedence(Token *token) {
	if ((token->mType & Token::OPERATOR) == 0) {
		return -1;
	}

	string s = token->mToken;

	if (s == "=") {
		return 11;
	}

	if (s == "*" ||
		s == "/") {
		return 10;
	} 

	if (s == "+" ||
		s == "-") {
		return 9;
	}

	return 0;
}


void Expression::ProvideIntermediates(Opcode *opcode, Parser *parser) {
	AllocateVariables(opcode, parser);
	HandleFunctionCalls(opcode, parser);

	if (mExprVars.size()) {
		BuildBytecodePostfix(opcode, parser);
	}
}


void Expression::AllocateVariables(Opcode *opcode, Parser* parser) {
	// Register all required variables under anonymous names
	for (list<ExprTerm*>::iterator it=mPostfix.begin(); 
			it!=mPostfix.end(); it++) {
		Token *token = (*it)->mToken;
		if (token && (token->mType & Token::OPERATOR) == 0) {
			mExprVars[*it] = RegisterVariable(parser, "");
		}
	}

	// Intermediate allocation objects
	for (map<ExprTerm*,uint>::iterator it=mExprVars.begin(); 
			it!=mExprVars.end(); it++) {
		if (!it->first->mToken) continue;

		Token *token = it->first->mToken;

		AllocateVariable(opcode, it->second);

		if (token->mType == Token::VARFUNC) {
			// Copy the original variable into our new one
			opcode->AddInterop(new ByteOperation(OP_MOV));

			uint src = GetVariableId(parser, token->mToken);

			opcode->AddInterop(new DwordOperation(&it->second));
			opcode->AddInterop(new DwordOperation(&src));
		} else {
			// The value to be copied is a numerical value.
			byte operation = 0;
			void *dword = malloc(4);

			if (it->first->mToken->mType == Token::VAL_INT) {
				operation = OP_MOVI;
				*(int*)dword = atoi(token->mToken.c_str());
			} else if (token->mType == Token::VAL_FLOAT) {
				operation = OP_MOVF;
				*(float*)dword = (float)atof(token->mToken.c_str());
			} else {
				throw InvalidTokenException("Expression token is of invalid type");
			}

			opcode->AddInterop(new ByteOperation(operation));
			opcode->AddInterop(new DwordOperation(&it->second));
			opcode->AddInterop(new DwordOperation(dword));

			free(dword);
		}
	}
}

void Expression::HandleFunctionCalls(Opcode *opcode, Parser *parser) {
	for (list<ExprTerm*>::iterator it=mPostfix.begin();
			it!=mPostfix.end(); it++) {
		FunctionCall *fcall = (*it)->mFunc;
		if (fcall) {
			fcall->ProvideIntermediates(opcode, parser);

			if (mPostfix.size() > 1) {
				// The return value of the function will be
				// stored in VAR_RETURN upon return. Allocate
				// a new variable and pop the value into it.
				uint id = RegisterVariable(parser, "");

				AllocateVariable(opcode, id);
				opcode->AddInterop(new ByteOperation(OP_POPMOV));
				opcode->AddInterop(new DwordOperation(&id));

				mExprVars[*it] = id;
			}
		}
	}
}

void Expression::BuildBytecodePostfix(Opcode *opcode, Parser *parser) {
	for (list<ExprTerm*>::iterator it = mPostfix.begin();
			it != mPostfix.end(); it++) {
		ExprTerm *term = *it;

		if (mExprVars.count(term) != 0) {
			opcode->AddInterop(new ByteOperation(OP_PUSH));
			opcode->AddInterop(new DwordOperation(&mExprVars[term]));
		} else {
			AddOperator(opcode, term->mToken);
		}
	}
}


void Expression::AddOperator(Opcode *opcode, Token *token) {
	if (!token) {
		throw NullPointerException("Token cannot be nil");
	}

	if ((token->mType & Token::OPERATOR) == 0) {
		throw InvalidTokenException("Expected an operator, got: " + token->mToken);
	}

	byte oper = 0;
	string s = token->mToken;

	if (s == "+") {
		oper = OP_ADD;
	} else if (s == "-") {
		oper = OP_SUB;
	} else if (s == "*") {
		oper = OP_MUL;
	} else if (s == "/") {
		oper = OP_DIV;
	} else if (s == "%") {
		oper = OP_MOD;
	} else {
		throw NotImplementedException("Operator not implemented: " + s);
	}

	opcode->AddInterop(new ByteOperation(oper));
}