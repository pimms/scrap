#include "tokens.h"
#include "../common/scrapexcept.h"

#include <fstream>

void Tokens::BuildTokens(string fileName) {
	ifstream file;
	file.open(fileName);
	if (!file.good()) {
		throw FileNotFoundException("File not found: " + (string)fileName);		
	}

	while (GetToken(file)) ;
	
	list<Token>::iterator it;
	for (it = mTokens.begin(); it != mTokens.end(); it++) {
		printf("[%i]: %s\n", it->mType, it->mToken.c_str());
	}
}


bool Tokens::GetToken(ifstream &file) {
	if (!file.good()) return false;

	Tokens::Token token;
	char ch;

	SeekNextToken(file);
	
	if (GetOperator(file)) {
		return true;
	}

	if (GetBracket(file)) {
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

	while (PeekOperator(file, lastChar)) {
		lastChar = file.get();
		str += lastChar;
	}

	if (str.length()) {
		Token token(str, Token::OPERATOR);
		mTokens.push_back(token);
		return true;
	}

	return false;
}

bool Tokens::GetBracket(ifstream &file) {
	char ch = file.peek();
	string str;

	switch (ch) {
		case '(':
		case ')':
			{
				str = ch;
				Token token(str, Token::PARANTHESES);
				mTokens.push_back(token);
				file.get();
				return true;
			}

		case '{':
		case '}':
			{
				str = ch;
				Token token(str, Token::BRACKET);
				mTokens.push_back(token);
				file.get();
				return true;
			}

		case ';':
			{
				str = ch;
				Token token(str, Token::SEMICOLON);
				mTokens.push_back(token);
				file.get();
				return true;
			}
	}

	return false;
}

bool Tokens::GetWord(ifstream &file) {
	string str;

	while (!NextCharBlocking(file) && !file.eof()) {
		str += file.get();
	}

	if (str.length()) {
		Token token(str);

		if (ReservedWord(str)) {
			token.mType = Token::RESERVED;
		} else {
			token.mType = Token::CUSTOM;
		}

		mTokens.push_back(token);
		return true;
	}

	return false;
}


bool Tokens::PeekOperator(ifstream &file, char context) {
	char ch = file.peek();
	
	switch (ch) {
	case '+':
	case '-':
		{
			// ++ and -- are valid
			// operators.
			if (context == ch || !context) {
				return true;
			}
			return false;
		}

	case '*':
	case '/':
		{
			// * and / cannot have a preceeding
			// operator.
			if (context == 0) {
				return true;
			}
			return false;
		}

	case '=':
		{
			// = can follow all other operators.
			return true;
		}

	case '!':
		{
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
		str == "do"		||
		str == "func"	||
		str == "var") {
		return true;
	}

	return false;
}


bool Tokens::NextCharBlocking(ifstream &file) {
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
			return true;
	}

	return false;
}

void Tokens::SeekNextToken(ifstream &file) {
	bool comment = false;

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