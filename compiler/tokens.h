#pragma once

/***** Class Token *****
* Contains all tokens in a scrap-file.
* Tokens are not checked for validity by the Tokens-class.
*****/

#include <string>
#include <list>
using namespace std;

class Tokens {
private:
	struct Token {
		enum Type {
			UNDEFINED,
			OPERATOR,
			CUSTOM,
			RESERVED,
			BRACKET,
			SEMICOLON,
			PARANTHESES,
		};

		Token() { mType = UNDEFINED; }
		Token(string token, Type t = UNDEFINED) {
			mToken = token;
			mType = t;
		}

		string			mToken;
		Type			mType;
	};

public:
	void				BuildTokens(string file);


private:
	list<Token>			mTokens;

	/***** GetToken *****
	* Attempts to fetch a token from the file. 
	*****/
	bool				GetToken(ifstream &file);
	bool				GetOperator(ifstream &file);
	bool				GetBracket(ifstream &file);
	bool				GetWord(ifstream &file);

	/***** PeekOperator *****
	* Is the next char an operator, and is it valid
	* in the context of the passed operator?
	*****/
	bool				PeekOperator(ifstream &file, char context=0);

	bool				ReservedWord(string word);

	/***** NextCharBlocking *****
	* If the next character in the stream is "(", ")", "+", " " or 
	* similar, it does not count toward the current token, but is
	* included in the next token. 
	*****/
	bool				NextCharBlocking(ifstream &file);

	/***** SeekNextToken *****
	* The contents of the file is ignored until the first valid
	* character. Comments and whitespace are filtered out in this
	* method.
	*****/
	void				SeekNextToken(ifstream &file);
	void				SkipLine(ifstream &file);
};