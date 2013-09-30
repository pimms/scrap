#include "tokens.h"
#include "../common/scrapexcept.h"
#include "../run/var.h"

#include <fstream>


string Token::GetStringValue(Token::Type type) {
	switch (type) {
	case UNDEFINED:		return "UNDEFINED";
	case OPERATOR:		return "OPERATOR";
	case PARANTH_BEG:	return "PARANTH_BEG";
	case PARANTH_END:	return "PARANTH_END";
	case RESERVED:		return "RESERVED";
	case BRACKET_BEG:	return "BRACKET_BEG";
	case BRACKET_END:	return "BRACKET_END";
	case SEMICOLON:		return "SEMICOLON";
	case DOT:			return "DOT";
	case COMMA:			return "COMMA";
	case VARFUNC:		return "VARFUNC";
	case VALUE:			return "VALUE";
	case VAL_STRING:	return "VAL_STRING";
	case VAL_INT:		return "VAL_INT";
	case VAL_FLOAT:		return "VAL_FLOAT";
	case INVALID:		return "INVALID";
	}

	return "???";
}




Tokens::Tokens() {
	mCursor = mTokens.begin();
}

void Tokens::BuildTokens(string fileName) {
	ifstream file;
	file.open(fileName.c_str());
	if (!file.good()) {
		throw FileNotFoundException("File not found: " + (string)fileName);		
	}

	while (GetToken(file)) ;

	mCursor = mTokens.begin();
}

bool Tokens::HasMore() {
	return mTokens.size() > 0 
		&& mCursor != mTokens.end();
}


Token* Tokens::PopIfExists(Token::Type type) {
	Token *token = NULL;

	if ((*mCursor)->mType == type) {
		token = *mCursor;
		mCursor = mTokens.erase(mCursor);
	}

	return token;
}

Token* Tokens::PopExpected(Token::Type type) {
	Token *token = PopIfExists(type);

	if (token) { 
		return token;
	}

	// All hell is loose - RELEASE THE KRAKEN!
	token = PopNext();

	throw InvalidTokenException(
							"Expected '" 
							+ Token::GetStringValue(type) 
							+ "', got: '" 
							+ (token ?Token::GetStringValue(token->mType):"NULL") 
							+ "'.");
}

Token* Tokens::PopNext() {
	Token *token = *mCursor;
	mCursor = mTokens.erase(mCursor);

	return token;
}


Token* Tokens::PeekNext() {
	return *mCursor;
}


TokenIter Tokens::GetFrontIter() {
	return mTokens.begin(); 
}

TokenIter Tokens::GetCursor() {
	return mCursor;
}

void Tokens::SetCursor(TokenIter iter) {
	mCursor = iter;
}



bool Tokens::GetToken(ifstream &file) {
	if (!file.good()) return false;

	Token token;
	SeekNextToken(file);

	if (GetSpecialChar(file)) {
		return true;
	}

	if (GetOperator(file)) {
		return true;
	}

	if (GetWord(file)) {
		return true;
	}

	return false;
}

bool Tokens::GetOperator(ifstream &file) {
	string str;
	char lastChar = 0;

	while (PeekOperator(file, lastChar) && str.length() <= 1) {
		lastChar = file.get();
		str += lastChar;
	}

	if (str.length()) {
		Token *t = new Token(str, Token::OPERATOR);
		mTokens.push_back(t);
		return true;
	}

	return false;
}

bool Tokens::GetSpecialChar(ifstream &file) {
	char ch = file.peek();
	string str;
	str = ch;

	Token *t = new Token(str);

	switch (ch) {
		case '(':
			t->mType = Token::PARANTH_BEG;
			break;

		case ')':
			t->mType = Token::PARANTH_END;
			break;

		case '{':
			t->mType = Token::BRACKET_BEG;
			break;

		case '}':
			t->mType = Token::BRACKET_END;
			break;

		case ';':
			t->mType = Token::SEMICOLON;
			break;

		case '.':
			t->mType = Token::DOT;
			break;

		case ',':
			t->mType = Token::COMMA;
			break;

		default:
			delete t;
			t = NULL;
			return false;
	}

	mTokens.push_back(t);
	file.get();
	return true;
}

bool Tokens::GetWord(ifstream &file) {
	string str;

	// String literal
	if (file.peek() == '\"') {
		// Get the first "
		str = file.get();
		char ch = file.peek();
		
		while (ch != '\"' && !file.eof()) {
			str += file.get();
			ch = file.peek();
		}
		
		// Get the last "
		str += file.get();
	} else {
		// The word can be a VARFUNC, INT or FLOAT.
		bool numerical = (file.peek() >= '0' && file.peek() <= '9');

		while (!NextCharBlocking(file, numerical) && !file.eof()) {
			char ch = file.get();
			str += ch;
		}
	}

	if (str.length()) {
		Token *t = new Token(str);

		if (ReservedWord(str)) {
			t->mType = Token::RESERVED;
		} else if (str[0] == '\"') {
			t->mType = Token::VAL_STRING;
		} else {
			Var::Type vType = Var::GetType(str);
			if (vType == Var::Type::INT) {
				t->mType = Token::VAL_INT;
			} else if (vType == Var::Type::FLOAT) {
				t->mType = Token::VAL_FLOAT;
			} else {
				if (ValidName(str)) {
					t->mType = Token::VARFUNC;
				} else {
					t->mType = Token::INVALID;
				}
			}
		}

		mTokens.push_back(t);
		return true;
	}

	return false;
}


bool Tokens::PeekOperator(ifstream &file, char context) {
	char ch = file.peek();
	
	switch (ch) {
	case '+':
	case '-':
	case '&':
	case '|':
		{
			// ++, --, && and || are valid
			// operators.
			if (context == ch || !context) {
				return true;
			}
			return false;
		}

	case '*':
	case '/':
	case '<':
	case '>':
		{
			return context == 0;
		}

	case '=':
		{
			// = can follow nearly all operators
			if (context == '&' || context == '|') {
				return false;
			}
			return true;
		}

	case '!':
		{
			// ! must be first in any dual-operand
			if (context == 0) {
				return true;
			}
			return false;
		}
	}

	return false;
}


bool Tokens::ReservedWord(string str) {
	if (str == "class"	||
		str == "for"	||
		str == "if"		||
		str == "else"	||
		str == "while"	||
		str == "func"	||
		str == "var"	||
		str == "return" ||
		str == "include"){
		return true;
	}

	return false;
}

bool Tokens::ValidName(string name) {
	if (!name.length()) { 
		return false;
	}

	if (name[0] >= '0' && name[0] <= '9') {
		return false;
	}

	for (int i=0; i<name.length(); i++) {
		char c = name[i];

		if ((c < 'a' || c > 'z') &&
			(c < 'A' || c > 'Z') &&
			(c < '0' || c > '9') &&
			(c != '_')) {
			return false;
		}
	}

	return true;
}


bool Tokens::NextCharBlocking(ifstream &file, bool numericalContext) {
	char ch = file.peek();

	switch (ch) {
		case '(':
		case ')':
		case '{':
		case '}':
		case ';':

		case ' ':
		case '\n':
		case '\t':
		case '#':

		case '-':
		case '+':
		case '*':
		case '/':
		case '=':
		case '!':

		case '<':
		case '>':
		case '\"':
		case ',':
			return true;

		case '.':
			return !numericalContext;
	}

	return false;
}

void Tokens::SeekNextToken(ifstream &file) {
	char ch = file.peek();
	while (ch == '#') {
		SkipLine(file);
		ch = file.peek();
	}

	while (ch == ' ' || ch == '\n' || ch == '\t') {
		file.get();
		ch = file.peek();

		while (ch == '#') {
			SkipLine(file);
			ch = file.peek();
		}
	}
}

void Tokens::SkipLine(ifstream &file) {
	string waste;
	std::getline(file, waste);
}