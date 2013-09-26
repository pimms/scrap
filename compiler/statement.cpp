#include "statement.h"
#include "../common/stack.h"
#include "../common/scrapexcept.h"


Statement::Statement() {

}

Statement* Statement::ParseStatement(Tokens *tokens) {
	Statement *stmt = NULL;

	const Token *token = tokens->PeekNext();

	if (token->mType == Token::RESERVED) {
		string s = token->mToken;

		if (s == "var") {
			stmt = new AssignStatement();
		}
	} else if (token->mType == Token::VARFUNC) {
		stmt = new AssignStatement();
	}

	if (stmt) {
		stmt->ParseStatement(tokens);
	} else {
		throw NotImplementedException("Feature not implemented");
	}

	return stmt;
}



/***** AssignStatement *****/
void AssignStatement::ParseStatement(Tokens *tokens) {
	mAlloc = false;
	mOperator = NULL;
	mExpression = NULL;
	mAssignee = NULL;

	if (tokens->PeekNext()->mType == Token::RESERVED) {
		mAlloc = true;
		delete tokens->PopNext();
	}

	mAssignee = tokens->PopExpected(Token::VARFUNC);

	mOperator = tokens->PopIfExists(Token::OPERATOR);
	if (mOperator) {
		mExpression = new Expression;
		mExpression->ParseStatement(tokens);
	}
}

void AssignStatement::ProvideIntermediates(Opcode *opcode, Parser *parser) {

}