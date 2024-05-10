%top{
    /* This code goes at the "top" of the generated file. */
    #include "pch.hpp"
    #include "FxParser.h"
}

%{
    #include <cerrno>
    #include <climits>
    #include <cstdlib>
    #include <cstring> // strerror
    #include <string>
    #include <iostream>
    #include "Driver.hpp"
    #include "Scanner.hpp"
	#include <BlackBox/Core/Path.hpp>
	#pragma warning(push, 0)
	#include "Parser.hpp"
    #define MAX_STR_CONST 10 * 16382 // TODO: Change this and make it dynamic... functions could be big

    #define RSTATE_COMMON1(x,...) {  push_state_opt(__VA_ARGS__); print_state(); return yy::parser::make_##x(fx::ERenderState::##x, loc); }
    #define RSTATE_COMMON2(x,type,...) { push_state_opt(__VA_ARGS__); print_state(); return yy::parser::make_##x(type, loc); }

    #define RSTATE(x,...) { push_state_opt(__VA_ARGS__);print_state(); return yy::parser::make_##x(fx::ERenderState::##x, loc); }
    #define BSTATE(x,...) RSTATE_COMMON2(BLEND_VALUE, D3D11_BLEND::D3D11_BLEND_##x);
    #define BSTATE_OP(x,...) RSTATE_COMMON2(BLEND_OP_VALUE, D3D11_BLEND_OP::D3D11_BLEND_OP_##x);
    #define ZCMPSTATE(x,...) RSTATE_COMMON2(COMPARISION_FUNC, D3D11_COMPARISON_FUNC::D3D11_COMPARISON_##x);
    #define RCULLSTATE(x,...) RSTATE_COMMON2(CULLMODE_VALUE, D3D11_CULL_MODE::D3D11_CULL_##x);
    #define RFILLSTATE(x,...) RSTATE_COMMON2(FILLMODE_VALUE, D3D11_FILL_MODE::D3D11_FILL_##x);
    #define SSTATE(x,...) RSTATE_COMMON2(SAMPLER_STATE_VALUE, D3D11_FILTER::D3D11_FILTER_##x);
    #define POP_STATE yy_pop_state(); return CURRENT_SYMBOL;


    //void yy_push_state(int);;

    //void push_state_opt(int state)
    //{
    //    yy_push_state(state);
    //}

    //void push_state_opt()
    //{

    //}

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
    bool return_from_func = true;
    void print_state(int state);
    void print_current_state();
  const char* state_to_string(int state);

    void Scanner::print_state()
    {
      ::print_state(YY_START);
    }


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

    yy::parser::symbol_type make_FLOAT(
    const std::string &s,
    const yy::parser::location_type& loc
    );
%}

boolean         true|false
id              [_a-zA-Z][a-zA-Z_0-9]*
int             [0-9]+
blank           [ \t\r]
float_number    [+-]?([0-9]*[.])?[0-9]+(f|F)?

%option stack
%x skiptoendofline ifdef endif getname define defname defval fbo fbo1 clearmode rendermode 
%x incl comment comment2 str function functionbody cstbuffer technique pass sampler_state 
%x dst_state pr_state color_sample_state rasterization_state  resource resource1 input_layout
%x blend_state blend_state_op compare_func
%x blend_state_top depth_stencil_state_top sampler_state_top
%x cull_mode fill_mode
%x sampler_addressing sampler_filter

%{
  // Code run each time a pattern is matched.
  # define YY_USER_ACTION  \
  loc.columns (yyleng);\
  add_shader_fragment();
  //::print_state(YY_START);\
  //CryLog("Current token text: %s", YYText());
%}
%%
%{
  // A handy shortcut to the location held by the driver.
  yy::location& loc = driver.location;
  // Code run each time yylex is called.
  loc.step();
  int  comment_caller;
  char  string_buf[MAX_STR_CONST];
  //string_buf.clear();
  //CryLog("Begin yylex");

%}

FatalError {
	return yy::parser::make_FATALERROR(loc);
}

static {
    return yy::parser::make_STATIC(StorageClass::Static, loc);
}

[Tt]echnique {
    this->canNowAddFragment = false;
    yy_push_state(technique);
	return yy::parser::make_TECHNIQUE(loc);
}

<INITIAL,cstbuffer>register {
	return yy::parser::make_REGISTER(loc);
}
<INITIAL>cbuffer {
  yy_push_state(cstbuffer);
	return yy::parser::make_CSTBUFFER(loc);
}
<INITIAL>ConstantBuffer {
  yy_push_state(cstbuffer);
	return yy::parser::make_CONSTANTBUFFER(loc);
}
<INITIAL>SamplerState {
    //yy_push_state(sampler_state_top);
	return yy::parser::make_SAMPLERSTATE(loc);
}
<INITIAL>DepthStencilState {
    //yy_push_state(depth_stencil_state_top);
    return yy::parser::make_DEPTHSTENCILSTATE(loc);
}
<INITIAL>BlendState {
    //yy_push_state(blend_state_top);
    return yy::parser::make_BLENDSTATE(loc);
}
<INITIAL>RasterizerState {
    return yy::parser::make_RASTERSTATE(loc);
}


<INITIAL,cstbuffer>struct {
	return yy::parser::make_STRUCT(loc);
}

<INITIAL,function>{
"in"    return yy::parser::make_INSPECYFIER(loc);
"out"   return yy::parser::make_OUTSPECYFIER(loc);
}

<INITIAL,functionbody>{
"typedef" return yy::parser::make_TYPEDEF(loc);
"const" return yy::parser::make_CONST(loc);
}

<INITIAL,pass>{
(?i:DepthEnable)    RSTATE(ZENABLE);
(?i:DepthWriteMask)    RSTATE(ZWRITEMASK); 
(?i:ZEnable)           RSTATE(ZENABLE);
(?i:ZWriteEnable)      RSTATE(ZWRITEENABLE);
(?i:ZFunc)             RSTATE(ZFUNC, compare_func);
(?i:StencilEnable)     RSTATE(STENCILENABLE);
(?i:StencilFunc)       RSTATE(STENCILFUNC);
(?i:StencilPass)       RSTATE(STENCILPASS);
(?i:StencilFail)       RSTATE(STENCILFAIL);
(?i:StencilZFail)      RSTATE(STENCILZFAIL);
(?i:StencilMask)       RSTATE(STENCILMASK);
(?i:CullMode)          RSTATE(CULLMODE, cull_mode);
(?i:FillMode)          RSTATE(FILLMODE, fill_mode);
(?i:FrontCounterClockwise)     RSTATE(FRONTCOUNTERCLOCKWISE);
(?i:DepthBias)                 RSTATE(DEPTHBIAS);
(?i:DepthBiasClamp)            RSTATE(DEPTHBIASCLAMP);
(?i:SlopeScaledDepthBias)      RSTATE(SLOPESCALEDDEPTHBIAS);
(?i:DepthClipEnable)           RSTATE(DEPTHCLIPENABLE);
(?i:ScissorEnable)             RSTATE(SCISSORENABLE);
(?i:MultisampleEnable)         RSTATE(MULTISAMPLEENABLE);
(?i:AntialiasedLineEnable)     RSTATE(ANTIALIASEDLINEENABLE);


(?i:BlendEnable)       RSTATE(ALPHABLENDENABLE);
(?i:AlphaFunc)         RSTATE(ALPHAFUNC, blend_state_op);
    /*"AlphaRef"          RSTATE(ALPHAREF, blend_state);*/
(?i:BlendOp)           RSTATE(BLENDOP, blend_state_op);
(?i:SrcBlend)          RSTATE(SRCBLEND, blend_state);
(?i:DestBlend)         RSTATE(DESTBLEND, blend_state);
(?i:SrcBlendAlpha)     RSTATE(ALPHABLEND_SRC, blend_state);
(?i:DestBlendAlpha)    RSTATE(ALPHABLEND_DST, blend_state);
(?i:BlendOpAlpha)      RSTATE(ALPHABLEND_EQUATION, blend_state_op);

    /* Sampler States

    D3D11_FILTER Filter;
    D3D11_TEXTURE_ADDRESS_MODE AddressU;
    D3D11_TEXTURE_ADDRESS_MODE AddressV;
    D3D11_TEXTURE_ADDRESS_MODE AddressW;
    FLOAT MipLODBias;
    UINT MaxAnisotropy;
    D3D11_COMPARISON_FUNC ComparisonFunc;
    FLOAT BorderColor[4];
    FLOAT MinLOD;
    FLOAT MaxLOD;
    */
(?i:Filter)            RSTATE(SAMPLER_FILTER, sampler_filter);
(?i:AddressU)          RSTATE(SAMPLER_ADDRESSU, sampler_addressing);
(?i:AddressV)          RSTATE(SAMPLER_ADDRESSV, sampler_addressing);
(?i:AddressW)          RSTATE(SAMPLER_ADDRESSW, sampler_addressing);
(?i:MipLODBias)        RSTATE(SAMPLER_MIPLODBIAS);
(?i:MaxAnisotropy)     RSTATE(SAMPLER_MAXANISOTROPY);
(?i:ComparisonFunc)    RSTATE(SAMPLER_COMPARISONFUNC, compare_func);
(?i:BorderColor)       RSTATE(SAMPLER_BORDERCOLOR);
(?i:MaxLOD)            RSTATE(SAMPLER_MAXLOD);
(?i:MinLOD)            RSTATE(SAMPLER_MINLOD);
}

<blend_state>{
(?i:ZERO)              BSTATE(ZERO);
(?i:ONE)               BSTATE(ONE);
(?i:SRC_COLOR)         BSTATE(SRC_COLOR);
(?i:INV_SRC_COLOR)     BSTATE(INV_SRC_COLOR);
(?i:SRC_ALPHA)         BSTATE(SRC_ALPHA);
(?i:INV_SRC_ALPHA)     BSTATE(INV_SRC_ALPHA);
(?i:DEST_ALPHA)        BSTATE(DEST_ALPHA);
(?i:INV_DEST_ALPHA)    BSTATE(INV_DEST_ALPHA);
(?i:DEST_COLOR)        BSTATE(DEST_COLOR);
(?i:INV_DEST_COLOR)    BSTATE(INV_DEST_COLOR);
(?i:SRC_ALPHA_SAT)     BSTATE(SRC_ALPHA_SAT);
(?i:BLEND_FACTOR)      BSTATE(BLEND_FACTOR);
(?i:INV_BLEND_FACTOR)  BSTATE(INV_BLEND_FACTOR);
(?i:SRC1_COLOR)        BSTATE(SRC1_COLOR);
(?i:INV_SRC1_COLOR)    BSTATE(INV_SRC1_COLOR);
(?i:SRC1_ALPHA)        BSTATE(SRC1_ALPHA);
(?i:INV_SRC1_ALPHA)    BSTATE(INV_SRC1_ALPHA);

";"                 POP_STATE;
}

<blend_state_op>{
(?i:ADD)               BSTATE_OP(ADD);
(?i:SUBTRACT)          BSTATE_OP(SUBTRACT);
(?i:REV_SUBTRACT)      BSTATE_OP(REV_SUBTRACT);
(?i:MIN)               BSTATE_OP(MIN);
(?i:MAX)               BSTATE_OP(MAX);
";"                 POP_STATE;
}

<compare_func>{
(?i:NEVER)             ZCMPSTATE(NEVER);
(?i:LESS)              ZCMPSTATE(LESS);
(?i:EQUAL)             ZCMPSTATE(EQUAL);
(?i:LESS_EQUAL)        ZCMPSTATE(LESS_EQUAL);
(?i:GREATER)           ZCMPSTATE(GREATER);
(?i:NOT_EQUAL)         ZCMPSTATE(NOT_EQUAL);
(?i:GREATER_EQUAL)     ZCMPSTATE(GREATER_EQUAL);
(?i:ALWAYS)            ZCMPSTATE(ALWAYS);
";"                 POP_STATE;

}

<cull_mode>{
(?i:NONE)              RCULLSTATE(NONE);
(?i:FRONT)             RCULLSTATE(FRONT);
(?i:BACK)              RCULLSTATE(BACK);
";"                 POP_STATE;
}

<fill_mode>{
(?i:SOLID)             RFILLSTATE(SOLID);
(?i:WIREFRAME)         RFILLSTATE(WIREFRAME);
";"                 POP_STATE;
}

<sampler_filter>{
(?i:MIN_MAG_MIP_POINT) SSTATE(MIN_MAG_MIP_POINT);
(?i:MIN_MAG_POINT_MIP_LINEAR) SSTATE(MIN_MAG_POINT_MIP_LINEAR);
(?i:MIN_POINT_MAG_LINEAR_MIP_POINT) SSTATE(MIN_POINT_MAG_LINEAR_MIP_POINT);
(?i:MIN_POINT_MAG_MIP_LINEAR) SSTATE(MIN_POINT_MAG_MIP_LINEAR);
(?i:MIN_LINEAR_MAG_MIP_POINT) SSTATE(MIN_LINEAR_MAG_MIP_POINT);
(?i:MIN_LINEAR_MAG_POINT_MIP_LINEAR) SSTATE(MIN_LINEAR_MAG_POINT_MIP_LINEAR);
(?i:MIN_MAG_LINEAR_MIP_POINT) SSTATE(MIN_MAG_LINEAR_MIP_POINT);
(?i:MIN_MAG_MIP_LINEAR) SSTATE(MIN_MAG_MIP_LINEAR);
(?i:ANISOTROPIC) SSTATE(ANISOTROPIC);
(?i:COMPARISON_MIN_MAG_MIP_POINT) SSTATE(COMPARISON_MIN_MAG_MIP_POINT);
(?i:COMPARISON_MIN_MAG_POINT_MIP_LINEAR) SSTATE(COMPARISON_MIN_MAG_POINT_MIP_LINEAR);
(?i:COMPARISON_MIN_POINT_MAG_LINEAR_MIP_POINT) SSTATE(COMPARISON_MIN_POINT_MAG_LINEAR_MIP_POINT);
(?i:COMPARISON_MIN_POINT_MAG_MIP_LINEAR) SSTATE(COMPARISON_MIN_POINT_MAG_MIP_LINEAR);
(?i:COMPARISON_MIN_LINEAR_MAG_MIP_POINT) SSTATE(COMPARISON_MIN_LINEAR_MAG_MIP_POINT);
(?i:COMPARISON_MIN_LINEAR_MAG_POINT_MIP_LINEAR) SSTATE(COMPARISON_MIN_LINEAR_MAG_POINT_MIP_LINEAR);
(?i:COMPARISON_MIN_MAG_LINEAR_MIP_POINT) SSTATE(COMPARISON_MIN_MAG_LINEAR_MIP_POINT);
(?i:COMPARISON_MIN_MAG_MIP_LINEAR) SSTATE(COMPARISON_MIN_MAG_MIP_LINEAR);
(?i:COMPARISON_ANISOTROPIC) SSTATE(COMPARISON_ANISOTROPIC);
(?i:MINIMUM_MIN_MAG_MIP_POINT) SSTATE(MINIMUM_MIN_MAG_MIP_POINT);
(?i:MINIMUM_MIN_MAG_POINT_MIP_LINEAR) SSTATE(MINIMUM_MIN_MAG_POINT_MIP_LINEAR);
(?i:MINIMUM_MIN_POINT_MAG_LINEAR_MIP_POINT) SSTATE(MINIMUM_MIN_POINT_MAG_LINEAR_MIP_POINT);
(?i:MINIMUM_MIN_POINT_MAG_MIP_LINEAR) SSTATE(MINIMUM_MIN_POINT_MAG_MIP_LINEAR);
(?i:MINIMUM_MIN_LINEAR_MAG_MIP_POINT) SSTATE(MINIMUM_MIN_LINEAR_MAG_MIP_POINT);
(?i:MINIMUM_MIN_LINEAR_MAG_POINT_MIP_LINEAR) SSTATE(MINIMUM_MIN_LINEAR_MAG_POINT_MIP_LINEAR);
(?i:MINIMUM_MIN_MAG_LINEAR_MIP_POINT) SSTATE(MINIMUM_MIN_MAG_LINEAR_MIP_POINT);
(?i:MINIMUM_MIN_MAG_MIP_LINEAR) SSTATE(MINIMUM_MIN_MAG_MIP_LINEAR);
(?i:MINIMUM_ANISOTROPIC) SSTATE(MINIMUM_ANISOTROPIC);
(?i:MAXIMUM_MIN_MAG_MIP_POINT) SSTATE(MAXIMUM_MIN_MAG_MIP_POINT);
(?i:MAXIMUM_MIN_MAG_POINT_MIP_LINEAR) SSTATE(MAXIMUM_MIN_MAG_POINT_MIP_LINEAR);
(?i:MAXIMUM_MIN_POINT_MAG_LINEAR_MIP_POINT) SSTATE(MAXIMUM_MIN_POINT_MAG_LINEAR_MIP_POINT);
(?i:MAXIMUM_MIN_POINT_MAG_MIP_LINEAR) SSTATE(MAXIMUM_MIN_POINT_MAG_MIP_LINEAR);
(?i:MAXIMUM_MIN_LINEAR_MAG_MIP_POINT) SSTATE(MAXIMUM_MIN_LINEAR_MAG_MIP_POINT);
(?i:MAXIMUM_MIN_LINEAR_MAG_POINT_MIP_LINEAR) SSTATE(MAXIMUM_MIN_LINEAR_MAG_POINT_MIP_LINEAR);
(?i:MAXIMUM_MIN_MAG_LINEAR_MIP_POINT) SSTATE(MAXIMUM_MIN_MAG_LINEAR_MIP_POINT);
(?i:MAXIMUM_MIN_MAG_MIP_LINEAR) SSTATE(MAXIMUM_MIN_MAG_MIP_LINEAR);
(?i:MAXIMUM_ANISOTROPIC) SSTATE(MAXIMUM_ANISOTROPIC);
";"                 POP_STATE;
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
<INITIAL,cstbuffer,function,functionbody,input_layout,pass,technique>{
"void"   return yy::parser::make_VOID_TYPE(loc);
"unsigned" return yy::parser::make_UNSIGNED_TYPE(loc);
"float"  return yy::parser::make_FLOAT_TYPE(loc);
"float2" return yy::parser::make_FLOAT2_TYPE(loc);
"float3" return yy::parser::make_FLOAT3_TYPE(loc);
"float4" return yy::parser::make_FLOAT4_TYPE(loc);
"vec2"   return yy::parser::make_FLOAT2_TYPE(loc);
"vec3"   return yy::parser::make_FLOAT3_TYPE(loc);
    /*"vec4"   return yy::parser::make_FLOAT4_TYPE(loc);*/
"mat2"   return yy::parser::make_MAT2_TYPE(loc);
"mat3"   return yy::parser::make_MAT3_TYPE(loc);
"mat4"   return yy::parser::make_MAT4_TYPE(loc);
"float2x2"   return yy::parser::make_MAT2_TYPE(loc);
"float2x4"   return yy::parser::make_MAT2x4_TYPE(loc);
"float3x3"   return yy::parser::make_MAT3_TYPE(loc);
"float3x4"   return yy::parser::make_MAT34_TYPE(loc);
"float4x4"   return yy::parser::make_MAT4_TYPE(loc);
"bool"   return yy::parser::make_BOOL_TYPE(loc);
"bvec2"  return yy::parser::make_BOOL2_TYPE(loc);
"bvec3"  return yy::parser::make_BOOL3_TYPE(loc);
"bvec4"  return yy::parser::make_BOOL4_TYPE(loc);
"int"    return yy::parser::make_INT_TYPE(loc);
"int2"   return yy::parser::make_INT2_TYPE(loc);
"int3"   return yy::parser::make_INT3_TYPE(loc);
"int4"   return yy::parser::make_INT4_TYPE(loc);
"ivec2"  return yy::parser::make_INT2_TYPE(loc);
"ivec3"  return yy::parser::make_INT3_TYPE(loc);
"ivec4"  return yy::parser::make_INT4_TYPE(loc);
"uniform" return yy::parser::make_UNIFORM(loc);
"string" return yy::parser::make_STRING_TYPE(loc);
"Texture2D" return yy::parser::make_TEXTURE2D_TYPE(loc);
}

    /*==================================================================
      Comment starting points
    */
<INITIAL,str,cstbuffer,technique,pass,sampler_state,dst_state,pr_state,color_sample_state,rasterization_state,blend_state,blend_state_op,cull_mode,fill_mode,blend_state_top,compare_func,resource,resource1,fbo,fbo1,input_layout,function,functionbody>{
    "/*" {
        comment_caller  =  INITIAL;
        yy_push_state(comment);
    }
    "//" {
    comment_caller  =  YY_START;
    yy_push_state(comment2);
    ::print_state(YY_START);
    }
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
      Start of string
    */
<INITIAL,resource,resource1,pass,technique,cstbuffer,function>{
    \"    string_buf_ptr  =  string_buf;  yy_push_state(str);
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
        Env::Log()->LogError("\nunterminated  string  constant at unknown line, FIXME!!!");
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
<function>"\{" {
    CryLog("function body");
    bracket_level = 1; // must be one...
    string_buf_ptr  =  string_buf;
    *string_buf_ptr = '\0';
    yy_push_state(functionbody);
    return CURRENT_SYMBOL;
}

<functionbody>{
    "\}" {
        //CryLog("bracket level: %d", bracket_level);
        bracket_level--;
        if((bracket_level) == 0)
        {
            *string_buf_ptr  =  '\0';
            // shall I just do BEGIN(INITIAL) ?
            yy_pop_state(); // back to shader
            //yy_pop_state();// back to INITIAL
            //return_from_func = true;
            return CURRENT_SYMBOL;
        } else {
            *string_buf_ptr++  =  yytext[0];
        }
    }

    "{" {
        bracket_level++;
        *string_buf_ptr++ = '{';
    }

    "return" {
        return yy::parser::make_RETURN(loc);
    }
    \n {  /*copy the GLSL data*/
        char  *yptr  =  yytext;
        while  (  *yptr  )
        *string_buf_ptr++  =  *yptr++;
        //TODO:
		loc.lines (yyleng); loc.step ();
        return yy::parser::make_ANYLINE(loc); 
    }

    [^\\/\n^\{^\}]+ {  /*copy the GLSL data*/
        //CryLog("Copy function data");
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

    VertexShader        return yy::parser::make_VERTEXPROGRAM(IShader::Type::E_VERTEX, loc);
    PixelShader         return yy::parser::make_FRAGMENTPROGRAM(IShader::Type::E_FRAGMENT, loc);
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

<INITIAL,cstbuffer,sampler_state,dst_state,pr_state,color_sample_state,rasterization_state,blend_state,blend_state_op,cull_mode,fill_mode,blend_state_top,compare_func,technique,pass,function,functionbody,resource,resource1,fbo,fbo1,input_layout>\n+ {
    loc.lines (yyleng); loc.step ();
}
<INITIAL,cstbuffer,sampler_state,dst_state,pr_state,color_sample_state,rasterization_state,blend_state,blend_state_op,cull_mode,fill_mode,blend_state_top,compare_func,technique,pass,function,functionbody,resource,resource1,fbo,fbo1,input_layout>{blank}+ {
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

<INITIAL,cstbuffer,technique,sampler_state,dst_state,pr_state,color_sample_state,rasterization_state,blend_state,blend_state_op,cull_mode,fill_mode,blend_state_top,compare_func,pass,resource,resource1,fbo,fbo1,input_layout,function>{
    "true"          return yy::parser::make_BOOL(true, loc);
    "false"         return yy::parser::make_BOOL(false, loc);
    {int}           return make_INT(yytext, loc);
    {float_number}  return make_FLOAT(yytext, loc);
    {id}            return check_type(yytext, loc);
    . {   
        if((yytext[0] >= 33) && (yytext[0] <= 126))
            return CURRENT_SYMBOL;
        else {
            CryLog("Warning: line %d : odd character found (%u)...\n", 0xffff, (unsigned char)yytext[0]);
        }
        return CURRENT_SYMBOL;
    }

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
#include BEGIN(incl); add_shader_fragment("//");
<incl>[<" \t]* /*  eat  the  whitespace and " or < as often in #include */
<incl>[^ \t\n]+     {  /*  got  the  include  file  name  */
    char * s = (char*)strchr(YYText(), '\"'); // get rid of the quote or >
    if(!s) s = (char*)strchr(YYText(), '>');
    if(s)
        *s = '\0';
    include_stack.push(IncludeData(YY_CURRENT_BUFFER, driver.location, driver.file.c_str()));
    //driver.file = "Data/shaders/fx/" + std::string(YYText());
    driver.file = PathUtil::Make(PathUtil::Make(PathUtil::GetEnginePath(), string("Engine/shaders/fx")).c_str(), YYText());
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

#define yy_push_state(skiptoendofline);
#if yy_push_state(skiptoendofline);
#endif yy_push_state(skiptoendofline);
#elif yy_push_state(skiptoendofline);

<skiptoendofline>[^\n]*
<skiptoendofline>\n     loc.lines (yyleng); loc.step (); yy_pop_state();

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

#include "pch.hpp"

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

yy::parser::symbol_type make_FLOAT(
  const std::string &s,
  const yy::parser::location_type& loc
) {
  errno = 0;
  char* stop_str;
  float n = strtof(s.c_str(), &stop_str);
  if (errno == ERANGE)
    throw yy::parser::syntax_error(loc, "float is out of range: " + s);
  return yy::parser::make_FLOAT((float) n, loc);
}

#include <stack>
#include <fstream>
#include <BlackBox/System/ISystem.hpp>

//#include "Parser.hpp"
//#include "Scanner.hpp"

std::stack<std::shared_ptr<std::ifstream>> includes;
bool Scanner::MakeInclude(const char* file_name)
{
	bool result = false;
	CryLog("include <%s>", file_name);
	//include_stack.push(IncludeData(YY_CURRENT_BUFFER, lineno(), driver.file.c_str()));
	auto includeFile = std::make_shared<std::ifstream>(std::ifstream((std::string("res/shaders/fx/") + file_name)));
	const char* buf	 = nullptr;
	{
		//driver.stream = std::ifstream(file_name);
		if (!includeFile->is_open())
		{
			Env::Log()->LogError("[FX] File %s not found", file_name);
			LexerError("failure in including a file");
			return result;
		}
		includes.emplace(includeFile);
		Env::Log()->Log("$3[FX] File %s opened", file_name);
		yypush_buffer_state(yy_create_buffer(includes.top().get(), YY_BUF_SIZE));
		//yyrestart(includeFile);
		result = true;
	}
	return result;
}

void Scanner::eof()
{
	yypop_buffer_state();
	Env::Log()->LogError("$3[FX] File %s ended", driver.file.c_str());
	//yypop_buffer_state();
	//driver.file = incData.file_name;
	//driver.location.initialize(&driver.file);

#if 0
	if(incData.fileToClose)
		fclose(incData.fileToClose);
#endif
	//include_stack.pop();
}

yy::parser::symbol_type Scanner::check_type(
const std::string &s,
const yy::parser::location_type& loc
)
{
    if (auto id = get_type_id(s); id != -1)
    {
        if (nvFX::IUniform::Type(id) == nvFX::IUniform::Type::TTexture2D)
        {
            return yy::parser::make_TEXTURE2D_TYPE(loc);
        }
        return yy::parser::make_TYPE_NAME(s, loc);
    }
    else
    {
        return yy::parser::make_IDENTIFIER(s, loc);
    }
}

void Scanner::begin_function_body()
{
    bracket_level = 1; // must be one...
    //CryLog("begin function body");
    yy_push_state(functionbody);
}

void Scanner::end_function_body()
{
    yy_pop_state();
}


const char* state_to_string(int state)
{
    static char buffer[256];
    const char* cState{};
    switch(state)
    {
        case ifdef: cState = "ifdef"; break; 
        case endif: cState = "endif"; break;
        case getname: cState = "getname"; break;
        case define: cState = "define"; break;
        case defname: cState = "defname"; break;
        case defval: cState = "defval"; break;
        case fbo: cState = "fbo"; break;
        case fbo1: cState = "fbo1"; break;
        case clearmode: cState = "clearmode"; break;
        case rendermode: cState = "rendermode"; break;
        case incl: cState = "incl"; break;
        case comment: cState = "comment"; break;
        case comment2: cState = "comment2"; break;
        case str: cState = "str"; break;
        case function: cState = "function"; break;
        case functionbody: cState = "functionbody"; break;
        case cstbuffer: cState = "cstbuffer"; break;
        case technique: cState = "technique"; break;
        case pass: cState = "pass"; break;
        case sampler_state: cState = "sampler_state"; break;
        case dst_state: cState = "dst_state"; break;
        case pr_state: cState = "pr_state"; break;
        case color_sample_state: cState = "color_sample_state"; break;
        case rasterization_state: cState = "rasterization_state"; break;
        case resource: cState = "resource"; break;
        case resource1: cState = "resource1"; break;
        case input_layout: cState = "input_layout"; break;
        case INITIAL: cState = "INITIAL"; break;
        case blend_state: cState = "blend_state"; break;
        case blend_state_op: cState = "blend_state_op"; break;
        case compare_func: cState = "compare_func"; break;
        case blend_state_top: cState = "blend_state_top"; break;
        case depth_stencil_state_top: cState = "depth_stencil_state_top"; break;
        case sampler_state_top: cState = "sampler_state_top"; break;
        case cull_mode: cState = "cull_mode"; break;
        case fill_mode: cState = "fill_mode"; break;

        default: cState = "unknown";
    }

    sprintf(buffer, "%s(%d)", cState, state);
    return buffer;
}

void print_state(int state){
  CryLog("$1Current state: %s", state_to_string(state));
}

#if 0
void print_current_state()
{
  print_state(YY_START);
}
#endif


#pragma warning(pop)
