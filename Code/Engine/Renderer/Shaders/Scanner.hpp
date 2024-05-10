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
		//symboltype_map.insert({"float"});
		//symboltype_map.insert({"float2"});
		//symboltype_map.insert({"float3"});
		//symboltype_map.insert({"float4"});

		//symboltype_map.insert({"float4x4"});

		//symboltype_map.insert({"int"});
		//string_buf.reserve(1024 * 16);

	}
	virtual ~Scanner() {}
	virtual yy::parser::symbol_type ScanToken();
	void							pop_state() { yy_pop_state(); }
	void							goto_codebody();
	bool							MakeInclude(const char* file_name);
	void							eof();

	bool register_type(const string& name,const SObjectTypeInfo typedefObject)
	{
		if (symboltype_map.find(name) == symboltype_map.end())
		{
			symboltype_map.insert(name);

			if (typedefObject.Name.empty())
			{
				type_map[name] = current_type_id++;
			}
			else
			{
				type_map[name] = int(typedefObject.Type);
			}

			return true;
		}
		return false;
	}
	int get_type_id(const std::string& str)
	{
		auto it = type_map.find(str);
		if (it != type_map.end())
		{
			return it->second;
		}
		return -1;
	}
	void add_shader_fragment(const char* f)
	{
		defered_fragment = f + (" " + defered_fragment);		
	}
	void add_shader_fragment()
	{
		if (previewsCanAddFragment && canNowAddFragment)
		{
			shader += defered_fragment;
			defered_fragment = YYText();
		}
		else if (canNowAddFragment)
		{
			defered_fragment = YYText();
		}
		previewsCanAddFragment = canNowAddFragment;
	}
	yy::parser::symbol_type CurrentSymbol()
	{
		//return yy::parser::symbol_type(yy::parser::token::yytokentype(*YYText()), loc);
		return {};
	}

	yy::parser::symbol_type check_type(const std::string&				s,
									   const yy::parser::location_type& loc);
	void					print_state();
    void begin_function_body();
    void end_function_body();


	/////////////////////////
		void push_state_opt()
		{

		}
		void push_state_opt(int state)
		{
			yy_push_state(state);
		}

	Driver& driver;

	std::set<std::string> symboltype_map;
	std::map<std::string, int> type_map;
	int base_type_id = 0x100;
	int current_type_id = 0x100;
	//string				  string_buf;
	string				  shader;

	bool   canNowAddFragment	  = true;
	bool   previewsCanAddFragment = false;
	string defered_fragment;

	size_t pos = 0, len = 0;
};
