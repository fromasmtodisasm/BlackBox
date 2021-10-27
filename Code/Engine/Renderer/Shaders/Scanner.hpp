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
	Scanner(Driver& driver)
		: driver(driver)
	{
		symboltype_map.insert({"float"});
		symboltype_map.insert({"float2"});
		symboltype_map.insert({"float3"});
		symboltype_map.insert({"float4"});

		symboltype_map.insert({"float4x4"});

		symboltype_map.insert({"int"});
		string_buf.reserve(1024 * 16);
	}
	virtual ~Scanner() {}
	virtual yy::parser::symbol_type ScanToken();
	void							pop_state() { yy_pop_state(); }
	void							goto_codebody();
	bool							MakeInclude(const char* file_name);
	void							eof();

	bool register_type(const string& str)
	{
		symboltype_map.insert(str);	
		return false;
	}
	yy::parser::symbol_type			CurrentSymbol()
	{
		//return yy::parser::symbol_type(yy::parser::token::yytokentype(*YYText()), loc);
		return {};
	}

	yy::parser::symbol_type check_type(const std::string&				s,
									   const yy::parser::location_type& loc);
	void					print_state();

	Driver& driver;

	std::set<std::string> symboltype_map;
	string				  string_buf;
};
