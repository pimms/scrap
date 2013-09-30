#pragma once

/***** Class Token *****
* Contains all tokens in a scrap-file.
* Tokens are not checked for validity by the Tokens-class.
*****/

#include <string>
#include <list>
using namespace std;


/***** Struct Token *****
* Holds the type and text-value of a single
* scrap token. 
*****/
struct Token {
	enum Type {
		UNDEFINED	= 0x00000100,
		OPERATOR	= 0x00000200,
		PARANTH_BEG	= 0x00000400,
		PARANTH_END	= 0x00000800,
		RESERVED	= 0x00001000,
		BRACKET_BEG	= 0x00002000,
		BRACKET_END = 0x00004000,
		SEMICOLON	= 0x00008000,
		DOT			= 0x00010000,
		COMMA		= 0x00020000,
		VALUE		= 0x00040000,
		VAL_STRING	= 0x00080000,
		VAL_INT		= 0x00100000,
		VAL_FLOAT	= 0x00200000,
		VARFUNC		= 0x00400000,

		INVALID		= 0x80000000,
	};

	static string GetStringValue(Token::Type type);

	Token() { mType = INVALID; }
	Token(string token, Type t = INVALID) {
		mToken = token;
		mType = t;
	}

	string			mToken;
	Type			mType;
};

typedef list<Token*>::iterator TokenIter;

/***** Clsas Tokens *****
* Manages all Tokens in a file. 
*****/
class Tokens {
public:
						Tokens();

	void				BuildTokens(string file);

	bool 				HasMore();

	// Popped tokens must be deleted by the popper.
	Token* 				PopIfExists(Token::Type type);
	Token*				PopExpected(Token::Type type);
	Token* 				PopNext();

	Token*				PeekNext();


	/***** Iterative Methods *****
	* SetCursor allows for modifications of what the Pop-methods
	* returns. The following will ensure that the first Token is NEVER
	* touched:
	*		TokenIter it = GetFrontIter();	it++;
	*		SetCursor(it);
	*		PopNext();  // <- returns the second Token
	*****/
	TokenIter			GetFrontIter();
	TokenIter			GetCursor();
	void				SetCursor(TokenIter front);

private:
	list<Token*>		mTokens;
	TokenIter			mCursor;

	/***** GetToken *****
	* Attempts to fetch a token from the file. 
	*****/
	bool				GetToken(ifstream &file);
	bool				GetOperator(ifstream &file);
	bool				GetSpecialChar(ifstream &file);
	bool				GetWord(ifstream &file);

	/***** PeekOperator *****
	* Is the next char an operator, and is it valid
	* in the context of the passed operator?
	*****/
	bool				PeekOperator(ifstream &file, char context=0);

	/***** Naming Methods *****
	* Check for whether a word is reserved or
	* a valid scrap variable/function name.
	*****/
	bool				ReservedWord(string word);
	bool				ValidName(string name);

	/***** NextCharBlocking *****
	* If the next character in the stream is "(", ")", "+", " " or 
	* similar, it does not count toward the current token, but is
	* included in the next token. 
	*
	* In respect of floats, the otherwise blocking char '.' does not 
	* block strings containing only numbers and ONE '.'.
	*****/
	bool				NextCharBlocking(ifstream &file, bool numericalContext);

	/***** SeekNextToken *****
	* The contents of the file is ignored until the first valid
	* character. Comments and whitespace are filtered out in this
	* method.
	*****/
	void				SeekNextToken(ifstream &file);
	void				SkipLine(ifstream &file);
};