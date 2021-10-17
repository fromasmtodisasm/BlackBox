#pragma once

#if !defined(yyFlexLexerOnce)
#	include <FlexLexer.h>
#endif

#undef YY_DECL
#define YY_DECL yy::parser::symbol_type Scanner::ScanToken()
#include <stack>

//#include "Parser.hpp"

class Driver;

class Scanner : public yyFlexLexer
{
public:

	Scanner(Driver& driver) : driver(driver) {}
	virtual ~Scanner() {}
	virtual yy::parser::symbol_type ScanToken();
	void pop_state() { yy_pop_state(); }
	bool MakeInclude(const char* file_name);
	void eof();
	yy::parser::symbol_type CurrentSymbol()
	{
		//return yy::parser::symbol_type(yy::parser::token::yytokentype(*YYText()), loc);
		return {};
	}


	Driver& driver;
};
