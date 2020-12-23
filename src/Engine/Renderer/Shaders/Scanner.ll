%{
    #include <cerrno>
    #include <climits>
    #include <cstdlib>
    #include <cstring> // strerror
    #include <string>
    #include <iostream>
    #include "Driver.hpp"
    #include "Scanner.hpp"
	#pragma warning(push, 0)
	#include "Parser.hpp"
    #define MAX_STR_CONST 16382 // TODO: Change this and make it dynamic... shaders could be big

    int  bracket_level = 0;
    char  *string_buf_ptr;
    #define CURRENT_SYMBOL yy::parser::symbol_type(yy::parser::token::yytokentype(yytext\[0\]), loc)

%}

%option noyywrap nounput noinput batch debug
%option never-interactive

%option c++
%option yyclass="Scanner"

%{
  // A number symbol corresponding to the value in S.
  yy::parser::symbol_type make_INT(
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
Technique {
    yy_push_state(technique);
	return yy::parser::make_TECHNIQUE(loc);
}

VertexFormat return yy::parser::make_VERTEXFORMAT(loc);

    /*==================================================================
      Comment starting points
    */
<INITIAL,str,shader,cstbuffer,technique,pass,sampler_state,dst_state,pr_state,color_sample_state,rasterization_state,resource,resource1,fbo,fbo1>"/*" {
    comment_caller  =  INITIAL;
    yy_push_state(comment);
}

<INITIAL,str,shader,cstbuffer,technique,pass,sampler_state,dst_state,pr_state,color_sample_state,rasterization_state,resource,resource1,fbo,fbo1>"//" {
    comment_caller  =  INITIAL;
    yy_push_state(comment2);
}

<comment>{
    [^*\n]* /*  eat  anything  that's  not  a  '*'  */
    "*"+[^*/\n]*     /*  eat  up  '*'s  not  followed  by  '/'s  */
    \n loc.lines (yyleng); loc.step ();

    "*"+"/" yy_pop_state();//BEGIN(comment_caller);
}

<comment2>{
    .*
    \n loc.lines (yyleng); loc.step (); yy_pop_state();//BEGIN(comment_caller);
}

    /*==================================================================
      rules for inside a string
    */
<str>{
    \"    {  /*  saw  closing  quote  -  all  done  */
        yy_pop_state();//BEGIN(INITIAL);
        *string_buf_ptr  =  '\0';
        return yy::parser::make_STR(string_buf, loc);
    }
    \n    {
        //FIXME:
        //fprintf(stderr,"\nunterminated  string  constant at line %d\n", line_num);
        //yyterminate();
        gEnv->pLog->LogError("\nunterminated  string  constant at unknown line, FIXME!!!");
		return yy::parser::make_END (loc);
    }
    \\[0-7]{1,3}    {
        /*  octal  escape  sequence  */
        int  result;
        (void)  sscanf(  yytext  +  1,  "%o",  &result  );
        if  (  result  >  0xff  )
            /*  error,  constant  is  out-of-bounds  */
        *string_buf_ptr++  =  result;
    }
    \\[0-9]+    {
        /*  generate  error  -  bad  escape  sequence;  something
        *  like  '\48'  or  '\0777777'
        */
    }
    \\n    *string_buf_ptr++  =  '\n';
    \\t    *string_buf_ptr++  =  '\t';
    \\r    *string_buf_ptr++  =  '\r';
    \\b    *string_buf_ptr++  =  '\b';
    \\f    *string_buf_ptr++  =  '\f';
    \\(.|\n)    *string_buf_ptr++  =  yytext[1];
    [^\\\n\"]+ {
        char  *yptr  =  yytext;
        while  (  *yptr  )
        *string_buf_ptr++  =  *yptr++;
    }
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
		loc.lines (yyleng); loc.step ();
    }
    [^\n^\{^\}]+ {  /*copy the GLSL data*/
        char  *yptr  =  yytext;
        while  (  *yptr  )
        *string_buf_ptr++  =  *yptr++;
    }
}
    /*==================================================================
      rules for inside a  section
    */
<technique>{
    Pass {
        comment_caller  =  INITIAL;
        yy_push_state(pass);
		return yy::parser::make_PASS(loc);
    }
    \{  {
		return CURRENT_SYMBOL;
    }
    \} {
        //yy_pop_state();//BEGIN(INITIAL);
		return CURRENT_SYMBOL;
    }
}
<INITIAL,pass>{
    /*\{  {
        return CURRENT_SYMBOL;
    }
    \} {
        //yy_pop_state();
        return CURRENT_SYMBOL;
    }*/
    
    /*VertexProgram       return VERTEXPROGRAM;*/
    VertexShader        return yy::parser::make_VERTEXPROGRAM(IShader::Type::E_VERTEX, loc);
    /*FragmentProgram     return FRAGMENTPROGRAM;*/
    /*FragmentShader      return FRAGMENTPROGRAM;*/
    PixelShader         return yy::parser::make_FRAGMENTPROGRAM(IShader::Type::E_FRAGMENT, loc);
    /*GeometryProgram     return GEOMETRYPROGRAM;*/
    GeometryShader      return yy::parser::make_GEOMETRYPROGRAM(IShader::Type::E_GEOMETRY, loc);
}

<pass,pr_state>{
    \{  {
		return CURRENT_SYMBOL;
    }
    \} {
		return CURRENT_SYMBOL;
    }
}

<INITIAL,cstbuffer,sampler_state,dst_state,pr_state,color_sample_state,rasterization_state,technique,pass,shader,resource,resource1,fbo,fbo1>\n+ {
    loc.lines (yyleng); loc.step ();
}
<INITIAL,cstbuffer,sampler_state,dst_state,pr_state,color_sample_state,rasterization_state,technique,pass,shader,resource,resource1,fbo,fbo1>{blank}+ {
    loc.step ();
}

    /*
"-"        {
              std::cout << loc.begin.line << " " << loc.end.line << std::endl;
              return yy::parser::make_MINUS  (loc);
           }
"+"        return yy::parser::make_PLUS   (loc);
"*"        return yy::parser::make_STAR   (loc);
"/"        return yy::parser::make_SLASH  (loc);
"("        return yy::parser::make_LPAREN (loc);
")"        return yy::parser::make_RPAREN (loc);
"="        return yy::parser::make_ASSIGN (loc);
";"        return yy::parser::make_SEMICOLON(loc);
"{"        return yy::parser::make_LEFTSCOPE(loc);
"}"        return yy::parser::make_RIGHTSCOPE(loc);
","        return yy::parser::make_COMMA(loc);
    */

{int}      return make_INT(yytext, loc);
<INITIAL,cstbuffer,technique,sampler_state,dst_state,pr_state,color_sample_state,rasterization_state,pass,resource,resource1,fbo,fbo1>{id}   return yy::parser::make_IDENTIFIER(yytext, loc);
<INITIAL,cstbuffer,technique,sampler_state,dst_state,pr_state,color_sample_state,rasterization_state,pass,resource,resource1,fbo,fbo1>. {   
    if((yytext[0] >= 33) && (yytext[0] <= 126))
        return CURRENT_SYMBOL;
    else {
        CryLog("Warning: line %d : odd character found (%u)...\n", 0xffff, (unsigned char)yytext[0]);
    }
    return CURRENT_SYMBOL;
}
<<EOF>>    return yy::parser::make_END (loc);




%%

yy::parser::symbol_type make_INT(
  const std::string &s,
  const yy::parser::location_type& loc
) {
  errno = 0;
  long n = strtol(s.c_str(), NULL, 10);
  if (! (INT_MIN <= n && n <= INT_MAX && errno != ERANGE))
    throw yy::parser::syntax_error(loc, "integer is out of range: " + s);
  return yy::parser::make_INT((int) n, loc);
}

#pragma warning(pop)
