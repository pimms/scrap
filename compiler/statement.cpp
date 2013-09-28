#include "statement.h"
#include "interop.h"
#include "expr.h"
#include "tokens.h"

#include "../common/stack.h"
#include "../common/scrapexcept.h"


Statement::Statement() {

}

Statement* Statement::CreateStatement(Tokens *tokens, Parser *parser) {
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
		stmt->ParseStatement(tokens, parser);
	}

	return stmt;
}



/***** AssignStatement *****/
void AssignStatement::ParseStatement(Tokens *tokens, Parser *parser) {
	mAlloc = false;
	mOperator = NULL;
	mExpression = NULL;
	mAssignee = NULL;

	if (tokens->PeekNext()->mType == Token::RESERVED) {
		mAlloc = true;
		delete tokens->PopNext();
	}

	mAssignee = tokens->PopExpected(Token::VARFUNC);

	// TODO:
	// Allow "standalone" expressions
	mOperator = tokens->PopIfExists(Token::OPERATOR);
	if (mOperator) {
		mExpression = new Expression;
		mExpression->ParseStatement(tokens, parser);
	}
}

void AssignStatement::ProvideIntermediates(Opcode *opcode, Parser *parser) {
	uint varId = 0;

	if (mAlloc) {
		varId = RegisterVariable(parser, mAssignee->mToken);
		AllocateVariable(opcode, varId);
	} else if (mAssignee) {
		varId = GetVariableId(parser, mAssignee->mToken);
	}

	if (varId != 0) {
		opcode->AddInterop(new ByteOperation(OP_PUSH));
		opcode->AddInterop(new DwordOperation(&varId));

		mExpression->ProvideIntermediates(opcode, parser);

		HandleOperator(opcode, varId);
	} else {
		mExpression->ProvideIntermediates(opcode, parser);
		opcode->AddInterop(new ByteOperation(OP_POP));
	}
}

void AssignStatement::HandleOperator(Opcode *opcode, uint varId) {
	string s = mOperator->mToken;
	byte operation = 0;

	if (s == "+=") {
		operation = OP_ADD;
	} else if (s == "-=") {
		operation = OP_SUB;
	} else if (s == "/=") {
		operation = OP_DIV;
	} else if (s == "*=") {
		operation = OP_MUL;
	} else if (s == "%=") {
		operation = OP_MOD;
	} else if (s == "=") {
		// The assignee-variable and the expression result
		// is on the stack, but we only need to pop the expresion
		// into the assignee.
		opcode->AddInterop(new ByteOperation(OP_POPMOV));
		opcode->AddInterop(new DwordOperation(&varId));
		opcode->AddInterop(new ByteOperation(OP_POP));
		return;
	} else {
		throw NotImplementedException("Operator not implemented: " + s);
	}

	opcode->AddInterop(new ByteOperation(operation));
	opcode->AddInterop(new ByteOperation(OP_POP));
}