%{
    #include <cerrno>
    #include <climits>
    #include <cstdlib>
    #include <cstring> // strerror
    #include <string>
    #include <iostream>
    #include "driver.hpp"
    #include "Scanner.hpp"
	#pragma warning(push, 0)
	#include "Parser.hpp"
    #define MAX_STR_CONST 16382 // TODO: Change this and make it dynamic... shaders could be big

    int  bracket_level = 0;
    char  *string_buf_ptr;

%}

%option noyywrap nounput noinput batch debug

%option c++
%option yyclass="Scanner"

%{
  // A number symbol corresponding to the value in S.
  yy::parser::symbol_type make_NUMBER(
    const std::string &s,
    const yy::parser::location_type& loc
  );
%}

id    [a-zA-Z][a-zA-Z_0-9]*
int   [0-9]+
blank [ \t\r]

%option stack
%x fbo fbo1 clearmode rendermode incl comment comment2 str shader shaderbody cstbuffer technique pass sampler_state dst_state pr_state color_sample_state rasterization_state resource resource1

%{
  // Code run each time a pattern is matched.
  # define YY_USER_ACTION  loc.columns (yyleng);
%}
%%
%{
  // A handy shortcut to the location held by the driver.
  yy::location& loc = driver.location;
  // Code run each time yylex is called.
  loc.step();
  int  comment_caller;
  char  string_buf[MAX_STR_CONST];

%}

GLSLShader {
    //comment_caller  =  INITIAL;
    bracket_level = 0;
    yy_push_state(shader);
	return yy::parser::make_GLSLSHADER(loc);
}

    /*==================================================================
      rules for inside a GLSL section
    */
<shader>{
    \{  {
        bracket_level = 1; // must be one...
        string_buf_ptr  =  string_buf;
        *string_buf_ptr = '\0';
        yy_push_state(shaderbody);
		return yy::parser::make_LEFTSCOPE(loc);
    }
    {id} {
        // TODO create and REGISTER the variable in a table
		return yy::parser::make_IDENTIFIER(yytext, loc);
    }
}
<shaderbody>{
    \{  {
        bracket_level++;
        *string_buf_ptr++  =  yytext[0];
    }
    \} {
        bracket_level--;
        if((bracket_level) == 0)
        {
            *string_buf_ptr  =  '\0';
            // shall I just do BEGIN(INITIAL) ?
            yy_pop_state(); // back to shader
            yy_pop_state();// back to INITIAL
			return yy::parser::make_CODEBODY(string_buf, loc);
        } else {
            *string_buf_ptr++  =  yytext[0];
        }
    }
    \n {  /*copy the GLSL data*/
        char  *yptr  =  yytext;
        while  (  *yptr  )
        *string_buf_ptr++  =  *yptr++;
        //TODO:
        #if 0
        ++line_num; 
        #endif
    }
    [^\n^\{^\}]+ {  /*copy the GLSL data*/
        char  *yptr  =  yytext;
        while  (  *yptr  )
        *string_buf_ptr++  =  *yptr++;
    }
}

{blank}+   loc.step ();
\n+        loc.lines (yyleng); loc.step ();

"-"        {
              std::cout << loc.begin.line << " " << loc.end.line << std::endl;
              return yy::parser::make_MINUS  (loc);
           }
"+"        return yy::parser::make_PLUS   (loc);
"*"        return yy::parser::make_STAR   (loc);
"/"        return yy::parser::make_SLASH  (loc);
"("        return yy::parser::make_LPAREN (loc);
")"        return yy::parser::make_RPAREN (loc);
":="       return yy::parser::make_ASSIGN (loc);
";"        return yy::parser::make_SEMICOLON(loc);
"{"        return yy::parser::make_LEFTSCOPE(loc);
"}"        return yy::parser::make_RIGHTSCOPE(loc);
","        return yy::parser::make_COMMA(loc);

{int}      return make_NUMBER(yytext, loc);
{id}       return yy::parser::make_IDENTIFIER(yytext, loc);
.          {
                throw yy::parser::syntax_error(loc, "invalid character: " + std::string(yytext));
           }
<<EOF>>    return yy::parser::make_END (loc);
%%

yy::parser::symbol_type make_NUMBER(
  const std::string &s,
  const yy::parser::location_type& loc
) {
  errno = 0;
  long n = strtol(s.c_str(), NULL, 10);
  if (! (INT_MIN <= n && n <= INT_MAX && errno != ERANGE))
    throw yy::parser::syntax_error(loc, "integer is out of range: " + s);
  return yy::parser::make_NUMBER((int) n, loc);
}

#pragma warning(pop)
