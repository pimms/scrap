#include "statement.h"
#include "interop.h"
#include "expr.h"
#include "tokens.h"
#include "parser.h"

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
		} else if (s == "return") {
			stmt = new ReturnStatement();
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

	// Check for alloocatoin
	if (tokens->PeekNext()->mToken == "var") {
		mAlloc = true;
		delete tokens->PopExpected(Token::RESERVED);
	}

	// Check for assignment
	TokenIter iter = tokens->GetCursor();

	// Check for raw expression (probably a function call)
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


/***** ReturnStatement *****/
void ReturnStatement::ParseStatement(Tokens *tokens, Parser *parser) {
	delete tokens->PopExpected(Token::RESERVED);

	if (tokens->PeekNext()->mType == Token::SEMICOLON) {
		mExpression = NULL;
		delete tokens->PopNext();
	} else {
		mExpression = new Expression;
		mExpression->ParseStatement(tokens, parser);
	}
}

void ReturnStatement::ProvideIntermediates(Opcode *opcode, Parser *parser) {
	if (!parser->IsInLocalScope()) {
		throw InvalidTokenException("Unexpected return");
	}


	if (mExpression) {
		uint varId = parser->RegisterVariable("");
		AllocateVariable(opcode, varId);

		mExpression->ProvideIntermediates(opcode, parser);

		opcode->AddInterop(new ByteOperation(OP_POPMOV));
		opcode->AddInterop(new DwordOperation(&varId));
		
		opcode->AddInterop(new ByteOperation(OP_RET));
		opcode->AddInterop(new DwordOperation(&varId));
	} else {
		uint zero = 0;
		opcode->AddInterop(new ByteOperation(OP_RET));
		opcode->AddInterop(new DwordOperation(&zero));
	}
}