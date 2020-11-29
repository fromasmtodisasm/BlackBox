%skeleton "lalr1.cc"
%require "3.0"

%defines
%define api.token.constructor
%define api.value.type variant
%define parse.assert

%code requires {
    #include <string>
    class Scanner;
    class Driver;
}

// %param { Driver &drv }

%define parse.trace
%define parse.error verbose

%code top{
	#pragma warning(push, 0)
    #include "Driver.hpp"
    #include "Scanner.hpp"
    #include "location.hh"

    #if 0
    #include "expressions/NumberExpression.h"
    #include "expressions/AddExpression.h"
    #include "expressions/MulExpression.h"
    #include "expressions/DivExpression.h"
    #include "expressions/SubstractExpression.h"
    #include "expressions/IdentExpression.h"
    #include "assignments/Assignment.h"
    #include "assignments/AssignmentList.h"
    #include "Program.h"
    #endif

    static yy::parser::symbol_type yylex(Scanner &scanner, Driver& driver) {
        return scanner.ScanToken();
    }
}

%lex-param { Scanner &scanner }
%lex-param { Driver &driver }
%parse-param { Scanner &scanner }
%parse-param { Driver &driver }

%locations

%define api.token.prefix {TOK_}

%token
    END 0 "end of file"
    ASSIGN ":="
    MINUS "-"
    PLUS "+"
    STAR "*"
    SLASH "/"
    LPAREN "("
    RPAREN ")"
    LEFTSCOPE "{"
    RIGHTSCOPE "}"
    SEMICOLON ";"
    COMMA ","

	GLSLSHADER
;


%token <std::string> IDENTIFIER "identifier"
%token <std::string> CODEBODY
%token <int> NUMBER "number"

// %printer { yyo << $$; } <*>;

%%
%start unit;

unit: 
	GLSLSHADER IDENTIFIER { gEnv->pLog->Log("$3 Shader with name %s", $2.data()); }
    "{" CODEBODY { 
	gEnv->pLog->Log("$3 Shader parsed"); 
}
;

%%

void
yy::parser::error(const location_type& l, const std::string& m)
{
  gEnv->pLog->LogError(m.data());    
  std::cerr << l << ": " << m << '\n';
}
#pragma warning(pop)
