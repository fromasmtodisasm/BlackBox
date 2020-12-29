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
    #define CURRENT_SYMBOL yy::parser::symbol_type(yy::parser::token::yytokentype(*YYText()), loc)

    struct IncludeData {
        IncludeData(YY_BUFFER_STATE s, yy::location ln, const char *fname, std::unique_ptr<std::ifstream> fd = nullptr) :
            state(s), location(ln), file_name(fname), fileToClose(std::move(fd)) {}
        YY_BUFFER_STATE state;
        yy::location    location;
        std::string     file_name; // the name of the current file (when in #include, for example)
        std::unique_ptr<std::ifstream>   fileToClose; // keep track of the file descriptor to make sure we will close it
    };
    std::stack<IncludeData> include_stack;

    // we need to do this because yy_pop_state is generated as static
    //static void yy_pop_state();
    #if 0
    void lex_pop_state()
    {
        yy_pop_state();
    }
    #endif

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
%x fbo fbo1 clearmode rendermode incl comment comment2 str shader shaderbody cstbuffer technique pass sampler_state dst_state pr_state color_sample_state rasterization_state resource resource1 input_layout lang

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

Language {
    yy_push_state(lang);
	return yy::parser::make_LANGUAGE(loc);
}
<lang>{
    HLSL return yy::parser::make_LANG_ID(ShaderLangId::Hlsl, loc); 
    GLSL return yy::parser::make_LANG_ID(ShaderLangId::Glsl, loc); 
}

GLSLShader {
    //comment_caller  =  INITIAL;
    bracket_level = 0;
    yy_push_state(shader);
	return yy::parser::make_GLSLSHADER(loc);
}
HLSLShader {
    //comment_caller  =  INITIAL;
    bracket_level = 0;
    yy_push_state(shader);
	return yy::parser::make_HLSL11SHADER(loc);
	//return yy::parser::make_GLSLSHADER(loc);
}
[Tt]echnique {
    yy_push_state(technique);
	return yy::parser::make_TECHNIQUE(loc);
}


<INITIAL,cstbuffer>{
    \{  {
        return CURRENT_SYMBOL;
    }
    \} {
        //yy_pop_state();
        return CURRENT_SYMBOL;
    }
}
<INITIAL,cstbuffer,shader,input_layout>{
    void   return yy::parser::make_VOID_TYPE(loc);
    unsigned return yy::parser::make_UNSIGNED(loc);
    float  return yy::parser::make_FLOAT_TYPE(loc);
    float2 return yy::parser::make_FLOAT2_TYPE(loc);
    float3 return yy::parser::make_FLOAT3_TYPE(loc);
    float4 return yy::parser::make_FLOAT4_TYPE(loc);
    vec2   return yy::parser::make_FLOAT2_TYPE(loc);
    vec3   return yy::parser::make_FLOAT3_TYPE(loc);
    vec4   return yy::parser::make_FLOAT4_TYPE(loc);
    mat2   return yy::parser::make_MAT2_TYPE(loc);
    mat3   return yy::parser::make_MAT3_TYPE(loc);
    mat4   return yy::parser::make_MAT4_TYPE(loc);
    float2x2   return yy::parser::make_MAT2_TYPE(loc);
    float3x3   return yy::parser::make_MAT3_TYPE(loc);
    float4x4   return yy::parser::make_MAT4_TYPE(loc);
    bool   return yy::parser::make_BOOL_TYPE(loc);
    bvec2  return yy::parser::make_BOOL2_TYPE(loc);
    bvec3  return yy::parser::make_BOOL3_TYPE(loc);
    bvec4  return yy::parser::make_BOOL4_TYPE(loc);
    int    return yy::parser::make_INT_TYPE(loc);
    int2   return yy::parser::make_INT2_TYPE(loc);
    int3   return yy::parser::make_INT3_TYPE(loc);
    int4   return yy::parser::make_INT4_TYPE(loc);
    ivec2  return yy::parser::make_INT2_TYPE(loc);
    ivec3  return yy::parser::make_INT3_TYPE(loc);
    ivec4  return yy::parser::make_INT4_TYPE(loc);
    uniform return yy::parser::make_UNIFORM(loc);
}

VertexFormat return yy::parser::make_VERTEXFORMAT(loc);

    /*==================================================================
      Comment starting points
    */
<INITIAL,str,shader,cstbuffer,technique,pass,sampler_state,dst_state,pr_state,color_sample_state,rasterization_state,resource,resource1,fbo,fbo1,input_layout>"/*" {
    comment_caller  =  INITIAL;
    yy_push_state(comment);
}

<INITIAL,str,shader,cstbuffer,technique,pass,sampler_state,dst_state,pr_state,color_sample_state,rasterization_state,resource,resource1,fbo,fbo1,input_layout>"//" {
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
		return CURRENT_SYMBOL;
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
    [Pp]ass {
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
    \{  {
        return CURRENT_SYMBOL;
    }
    \} {
        //yy_pop_state();
        return CURRENT_SYMBOL;
    }

    InputLayout {
        yy_push_state(input_layout);
        return yy::parser::make_INPUTLAYOUT(loc);
    }
    
    /*VertexProgram       return VERTEXPROGRAM;*/
    VertexShader        return yy::parser::make_VERTEXPROGRAM(IShader::Type::E_VERTEX, loc);
    /*FragmentProgram     return FRAGMENTPROGRAM;*/
    /*FragmentShader      return FRAGMENTPROGRAM;*/
    PixelShader         return yy::parser::make_FRAGMENTPROGRAM(IShader::Type::E_FRAGMENT, loc);
    /*GeometryProgram     return GEOMETRYPROGRAM;*/
    GeometryShader      return yy::parser::make_GEOMETRYPROGRAM(IShader::Type::E_GEOMETRY, loc);
}

<pass,pr_state,input_layout>{
    \{  {
        CryLog("{pass,pr_state,input_layout");
		return CURRENT_SYMBOL;
    }
    \} {
        CryLog("}pass,pr_state,input_layout");
		return CURRENT_SYMBOL;
    }
}

<INITIAL,cstbuffer,sampler_state,dst_state,pr_state,color_sample_state,rasterization_state,technique,pass,shader,resource,resource1,fbo,fbo1,input_layout>\n+ {
    loc.lines (yyleng); loc.step ();
}
<INITIAL,cstbuffer,sampler_state,dst_state,pr_state,color_sample_state,rasterization_state,technique,pass,shader,resource,resource1,fbo,fbo1,input_layout>{blank}+ {
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
<INITIAL,cstbuffer,technique,sampler_state,dst_state,pr_state,color_sample_state,rasterization_state,pass,resource,resource1,fbo,fbo1,input_layout>{id}   return yy::parser::make_IDENTIFIER(yytext, loc);
<INITIAL,cstbuffer,technique,sampler_state,dst_state,pr_state,color_sample_state,rasterization_state,pass,resource,resource1,fbo,fbo1,input_layout>. {   
    if((yytext[0] >= 33) && (yytext[0] <= 126))
        return CURRENT_SYMBOL;
    else {
        CryLog("Warning: line %d : odd character found (%u)...\n", 0xffff, (unsigned char)yytext[0]);
    }
    return CURRENT_SYMBOL;
}
    /*==================================================================
      ==================================================================
      !! SIMPLIFIED PREPROCESSOR !!
      More complex preprocessor would require a complete parser that would
      pre-process the code. Here we avoid doing so : directly loading nested 
      includes : Not even using the grammar analyzer !
      ==================================================================
      ==================================================================
    */
#include BEGIN(incl);
<incl>[<" \t]* /*  eat  the  whitespace and " or < as often in #include */
<incl>[^ \t\n]+     {  /*  got  the  include  file  name  */
    char * s = (char*)strchr(YYText(), '\"'); // get rid of the quote or >
    if(!s) s = (char*)strchr(YYText(), '>');
    if(s)
        *s = '\0';
    include_stack.push(IncludeData(YY_CURRENT_BUFFER, driver.location, driver.file.c_str()));
    driver.file = "res/shaders/fx/" + std::string(YYText());
    driver.location.initialize(&driver.file);
    //line_num  = 1;
    //FILE *fd = NULL;
    std::unique_ptr<std::ifstream> fd;
    const char *buf = NULL;
    #if 0
    if(nvFX::g_includeCb)
        nvFX::g_includeCb(YYText(), fd, buf);
    if(buf)
        yy_switch_to_buffer(yy_scan_string(buf) );
    else if(fd)
        yy_switch_to_buffer(yy_create_buffer(fd, YY_BUF_SIZE));
    else 
    #else
    {
        fd  =  std::make_unique<std::ifstream>(driver.file);
        if(!fd->is_open())
        {
            CryError("err loading file %s", YYText());
        } else {
            CryLog("Including file %s", YYText());
            yy_switch_to_buffer( yy_create_buffer(fd.get(), YY_BUF_SIZE));
        }
    }
    #endif
    if(fd->is_open())
    {
        // let's keep track of this newly opened file : when poping, we will have to close it
        IncludeData &incData = include_stack.top();
        incData.fileToClose = std::move(fd);
        BEGIN(INITIAL);
    } else {
        YY_FATAL_ERROR( "failure in including a file" );
    }
}
<<EOF>> {
	if(include_stack.empty())//(  --include_stack_ptr  <  0  )
    {
        return yy::parser::make_END(loc);
        //yyterminate();
    }
    else
    {
        yy_delete_buffer(  YY_CURRENT_BUFFER  );
        IncludeData &incData = include_stack.top();
        yy_switch_to_buffer(incData.state);//[include_stack_ptr]);
        driver.location = incData.location;
        driver.file = incData.file_name;
        if(incData.fileToClose->is_open())
            incData.fileToClose->close();
        include_stack.pop();
    }
}

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

#include "ScannerHelpers.cpp"

#pragma warning(pop)
