#include "expr.h"
#include "../common/stack.h"
#include "../common/scrapexcept.h"


Expression::Expression(bool isFunctionParam) {
	mIsParam = isFunctionParam;
}

Expression::~Expression() {
	while (mPostfix.size()) {
		delete mPostfix.front();
		mPostfix.pop_front();
	}
}


void Expression::ParseExpression(Tokens *tokens) {
	BuildPostfix(tokens);

	list<ExprTerm*>::iterator it;
	for (it = mPostfix.begin(); it != mPostfix.end(); it++) {
		if ((*it)->mToken != NULL) {
			printf("%s ", (*it)->mToken->mToken.c_str());
		} else {
			printf("func ");
		}
	}
	printf("\n");
}


void Expression::BuildPostfix(Tokens *tokens) {
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
				func->ParseExpression(tokens);
				mPostfix.push_back(new ExprTerm(func));
			} else {
				mPostfix.push_back(new ExprTerm(token));
			}
		} else if (next->mType == Token::OPERATOR) {
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
	if (token->mType != Token::OPERATOR) {
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