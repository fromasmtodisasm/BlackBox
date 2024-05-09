%skeleton "lalr1.cc"
%require "3.0"

//NOTE: now 0 shift reduce conflicts
//%expect  2 //!!! разобраться!!!

%defines
%define api.token.constructor
%define api.value.type variant

%define         parse.assert
%define         parse.lac           full

%code requires {
	#undef new
    #include <string>
    #include <vector>
#ifdef VULKAN_SUPPORT
    #include <vulkan/vulkan.h>
#endif
    #include <BlackBox/Renderer/IRender.hpp>
    #include <BlackBox/Renderer/IShader.hpp>
    //#include "../BufferManager.hpp"
    #include <d3d11.h>
    #include "FxParser.h"
    #include "Effect.hpp"
    #ifdef S_FALSE
    #undef S_FALSE
    #endif
    class Scanner;
    class Driver;
    //using shader_assignment = std::pair<std::string, std::string>

    //extern void lex_pop_state();
    #define lex_pop_state() scanner.pop_state()
    #define lex_print_state() scanner.print_state()

    #define lex_begin_funciton_body() scanner.begin_function_body()


    //struct SAnnotation
    //{
    //    string Name;
    //    string Value;
    //};

    #define FxLog(...) CryLog("[FX] " __VA_ARGS__)

}

%initial-action
{
    is_common = false;
}

%define parse.trace
%define parse.error verbose

%code top{
    #include "pch.hpp"
	#pragma warning(push, 0)
    #include "Effect.hpp"
    #include "Driver.hpp"
    #include "Scanner.hpp"
    #include "location.hh"

    #define GreenLog(...) Env::Log()->Log(__VA_ARGS__)

    #include <algorithm>
    #include <numeric>

    #ifdef S_FALSE
    #undef S_FALSE
    #endif

    bool is_common;
    std::vector<string> annotations;

    static yy::parser::symbol_type yylex(Scanner &scanner, Driver& driver) {
        return scanner.ScanToken();
    }

    using Type = std::string;

    float toFloat(const SimpleValue& value)
    {
        switch (value.type)
        {
        case nvFX::IUniform::TFloat:
            return value.f;
        case nvFX::IUniform::TInt:
            return value.i;
        default:
            return 0;
        }
    }
}

%lex-param { Scanner &scanner }
%lex-param { Driver &driver }
%parse-param { Scanner &scanner }
%parse-param { Driver &driver }
%parse-param { FxEffect &effect }

%locations

%define api.token.prefix {TOK_}

/*==================================================================
  TOKEN
*/
%token
    END 0 "end of file"
;

// value type
%type  <SimpleValue>    value
%type  <SimpleValue>    numeric_constant
// basic type constructor
%type  <SimpleValue>    basic_type_constructor
%type  <SimpleValue>    type_constructor


%type  <std::string>    shader_assignment
%type  <IShader::Type>  shader_type
%type  <std::string>    struct_footer
%type  <std::string>    struct_header
%type  <DirectDeclarator> direct_declarator
%type  <DirectDeclarator> declarator
%type  <DirectDeclarator> shader_assignment_shader

// storage class
%type <StorageClass> storage_class
// annotation base
%type <SAnnotation> annotation_base
%type <SAnnotation> annotation
%type <std::string> annotation_header
%type <std::string> annotation_value
%type <StringList>  STRING_LIST

%type <std::vector<SAnnotation>> annotation_list
%type <std::vector<SAnnotation>> annotations

%type <std::string>             var_spec
%type <SObjectTypeInfo>         object_type
%type <SVariable>               var_decl
%type <std::vector<SVariable>>  var_decls
%type <std::string>            semantic



%type <std::vector<SVariable>>  arguments
%type <SFunction>               function_declaration
%type <SFunction>               function_definition

%type <CTechnique>              tech




%token <std::string> TYPE_NAME

%type  <nvFX::IUniform::Type>           base_type

%token <std::string>    IDENTIFIER
%token <bool>           TRUE
%token <bool>           FALSE
%token <float>          FLOAT
%token <int>            INT
%token <unsigned>       UNSIGNED
%token <bool>           BOOL
%token <std::string>    STR

%token              HLSL11SHADER /*HLSL11 shader domain*/
%token              TECHNIQUE /*Beginning of a technique*/
%token              PASS
%token <std::string>    CODEBODY /* piece of code (GLSL) *including* ending bracket '}' ! But '}' is NOT in the string s */
%token <StorageClass>   STATIC

/*------------------------------------------------------------------
  token for uniforms declared outside of any shader code
*/
%token              STRING_TYPE
%token              VOID_TYPE
%token              UNSIGNED_TYPE
%token              HIGHP
%token              MEDIUMP
%token              LOWP
%token              UNIFORM
%token              CSTBUFFER
%token              CONSTANTBUFFER
%token              FLOAT_TYPE
%token              FLOAT2_TYPE
%token              FLOAT3_TYPE
%token              FLOAT4_TYPE
%token              MAT2_TYPE
%token              MAT2x4_TYPE
%token              MAT3_TYPE
%token              MAT34_TYPE
%token              MAT4_TYPE
%token              BOOL_TYPE
%token              BOOL2_TYPE
%token              BOOL3_TYPE
%token              BOOL4_TYPE
%token              INT_TYPE
%token              INT2_TYPE
%token              INT3_TYPE
%token              INT4_TYPE
%token              TEXTURE1D_TYPE
%token              TEXTURE2D_TYPE
%token              TEXTURE2DSHADOW_TYPE
%token              TEXTURE2DRECT_TYPE
%token              TEXTURE3D_TYPE
%token              TEXTURECUBE_TYPE
%token              SAMPLER1D_TYPE
%token              SAMPLER2D_TYPE
%token              SAMPLER2DSHADOW_TYPE
%token              SAMPLER2DRECT_TYPE
%token              SAMPLER3D_TYPE
%token              SAMPLERCUBE_TYPE

%token              BLENDSTATE
%token              DEPTHSTENCILSTATE
%token              RASTERSTATE
%token              SAMPLERSTATE

/*------------------------------------------------------------------
  token for extensions
*/

%token              EXTENSION
%token              SEPARATE_SHADER

/*------------------------------------------------------------------
  token that match what is in PassState::Type for OpenGL state settings
*/


//%token <passstateType>      DEPTHMASK 100 //PassState::DepthMask
%token <fx::ERenderState> ALPHABLENDENABLE
%token <fx::ERenderState> ALPHABLEND_COLOR
%token <fx::ERenderState> ALPHABLEND_DST
%token <fx::ERenderState> ALPHABLEND_DST_ALPHA
%token <fx::ERenderState> ALPHABLEND_EQUATION
%token <fx::ERenderState> ALPHABLEND_EQUATION_ALPHA
%token <fx::ERenderState> ALPHABLEND_SRC
%token <fx::ERenderState> ALPHABLEND_SRC_ALPHA
%token <fx::ERenderState> ALPHAFUNC
%token <fx::ERenderState> ALPHAREF
%token <fx::ERenderState> BLENDENABLE
%token <fx::ERenderState> BLENDOP

%token <fx::ERenderState> CULLMODE
%token <fx::ERenderState> FILLMODE
%token <fx::ERenderState> FRONTCOUNTERCLOCKWISE
%token <fx::ERenderState> DEPTHBIAS
%token <fx::ERenderState> DEPTHBIASCLAMP;
%token <fx::ERenderState> SLOPESCALEDDEPTHBIAS;
%token <fx::ERenderState> DEPTHCLIPENABLE;
%token <fx::ERenderState> SCISSORENABLE;
%token <fx::ERenderState> MULTISAMPLEENABLE;
%token <fx::ERenderState> ANTIALIASEDLINEENABLE;


%token <fx::ERenderState> DESTBLEND
%token <fx::ERenderState> SRCBLEND
%token <fx::ERenderState> STENCILENABLE
%token <fx::ERenderState> STENCILFAIL
%token <fx::ERenderState> STENCILFUNC
%token <fx::ERenderState> STENCILMASK
%token <fx::ERenderState> STENCILPASS
%token <fx::ERenderState> STENCILZFAIL
%token <fx::ERenderState> ZENABLE
%token <fx::ERenderState> ZFUNC
%token <fx::ERenderState> ZWRITEENABLE
%token <fx::ERenderState> ZWRITEMASK
%token <fx::ERenderState> COLORWRITEENABLE

/*------------------------------------------------------------------
  token for blend operations
*/
%token <D3D11_BLEND_OP> BLEND_OP_VALUE

%token <D3D11_BLEND> BLEND_VALUE
%token <D3D11_COMPARISON_FUNC> COMPARISION_FUNC

%token <D3D11_FILL_MODE> FILLMODE_VALUE
%token <D3D11_CULL_MODE> CULLMODE_VALUE

%type <SRenderStateValue> render_state

%type <SRenderStateValue> ds_state
%type <SRenderStateValue> blendstate
%type <SRenderStateValue> blendstate_type
%type <SRenderStateValue> raster_state


%token  <IShader::Type>  VERTEXPROGRAM
%token  <IShader::Type>  FRAGMENTPROGRAM
%token  <IShader::Type>  GEOMETRYPROGRAM

%token REGISTER
%token FATALERROR

%token STRUCT

%token INSPECYFIER
%token OUTSPECYFIER
%token ANYLINE

%token RETURN

%%
%start input;

numeric_constant
    : INT { $$ = SimpleValue{.type = nvFX::IUniform::TInt, .i = $1}; }
    | FLOAT { $$ = SimpleValue{.type = nvFX::IUniform::TFloat, .f = $1}; }
    ;

input: %empty { FxLog("Empty effect"); }
| input ';'
| input tech
| input var_decl
{
    if ($2.Name == "Script")
    {
        FxLog("Found Script variable");

        for (const auto& annotation : $2.Annotations)
        {
            FxLog("Annotation: %s = %s", annotation.Name.c_str(), annotation.Value.c_str());
        }
    }
    effect.AddVariable($2);
}
| input blend_state_declaration ';'
| input depth_state_declaration ';'
| input raster_state_declaration ';'
| input shader_resource
| input function_definition {
    $2.Dump();
    effect.AddFunction($2);
}
| input function_declaration {FxLog("Pop lex state from declaration"); lex_pop_state();}
| input fatal_error
| input struct
| input error
;

blend_state_declaration: BLENDSTATE IDENTIFIER '{' blendstate_type_list '}'
{
    FxLog("New BlendState %s", $2.data());
    //lex_pop_state();
}

depth_state_declaration: DEPTHSTENCILSTATE IDENTIFIER '{' ds_state_list '}' 
{
    FxLog("New DepthState %s", $2.data());
    //lex_pop_state();
}

raster_state_declaration: RASTERSTATE IDENTIFIER '{' raster_state_list '}'
{
    FxLog("New RasterState %s", $2.data());
    //lex_pop_state();
}

ds_state: 
ZENABLE '=' BOOL { $$ = SRenderStateValue{ .Type = $1, .b = $3 }; }
| ZWRITEENABLE '=' BOOL { $$ = SRenderStateValue{ .Type = $1, .b = $3 }; }
| ZFUNC '=' COMPARISION_FUNC { $$ = SRenderStateValue{ .Type = $1, .i = $3 }; }
| ZWRITEMASK '=' INT { $$ = SRenderStateValue{ .Type = $1, .i = $3 }; }
| STENCILENABLE '=' BOOL { $$ = SRenderStateValue{ .Type = $1, .b = $3 }; }

ds_state_list: 
ds_state_list ds_state ';'
{
}
| ds_state ';'
{
}

//| STENCILENABLE { $$ = $1; }
//| STENCILFAIL { $$ = $1; }
//| STENCILFUNC { $$ = $1; }
//| STENCILMASK { $$ = $1; }
//| STENCILPASS { $$ = $1; }
//| STENCILZFAIL { $$ = $1; }
;

blendstate: BLEND_VALUE { $$ = $1; }
| BLEND_OP_VALUE { $$ = $1; }
;

blendstate_type: 
ALPHABLENDENABLE '=' BOOL { $$ = SRenderStateValue{ .Type = $1, .b = $3 }; }
| SRCBLEND '=' BLEND_VALUE { $$ = SRenderStateValue{ .Type = $1, .i = $3 }; }
| DESTBLEND '=' BLEND_VALUE { $$ = SRenderStateValue{ .Type = $1, .i = $3 }; }
| BLENDOP '=' BLEND_OP_VALUE { $$ = SRenderStateValue{ .Type = $1, .i = $3 }; }

| ALPHABLEND_SRC '=' BLEND_VALUE { $$ = SRenderStateValue{ .Type = $1, .i = $3 }; }
| ALPHABLEND_DST '=' BLEND_VALUE { $$ = SRenderStateValue{ .Type = $1, .i = $3 }; }
| ALPHABLEND_EQUATION '=' BLEND_OP_VALUE { $$ = SRenderStateValue{ .Type = $1, .i = $3 }; }
| ALPHABLEND_SRC_ALPHA '=' BLEND_VALUE { $$ = SRenderStateValue{ .Type = $1, .i = $3 }; }
| ALPHABLEND_DST_ALPHA '=' BLEND_VALUE { $$ = SRenderStateValue{ .Type = $1, .i = $3 }; }
| ALPHABLEND_EQUATION_ALPHA '=' BLEND_OP_VALUE { $$ = SRenderStateValue{ .Type = $1, .i = $3 }; }
| ALPHABLEND_COLOR '=' BLEND_VALUE { $$ = SRenderStateValue{ .Type = $1, .i = $3 }; }


;

blendstate_type_list: 
    blendstate_type_list blendstate_type ';'
    {
    }
    | blendstate_type ';'
    {
    }
;

raster_state:
    CULLMODE '=' CULLMODE_VALUE { $$ = SRenderStateValue{ .Type = $1, .i = $3 }; }
    | FILLMODE '=' FILLMODE_VALUE { $$ = SRenderStateValue{ .Type = $1, .i = $3 }; }
    | FRONTCOUNTERCLOCKWISE '=' BOOL { $$ = SRenderStateValue{ .Type = $1, .b = $3 }; }
    | DEPTHBIAS '=' numeric_constant { $$ = SRenderStateValue{ .Type = $1, .f = toFloat($3) }; }
    | DEPTHBIASCLAMP '=' numeric_constant { $$ = SRenderStateValue{ .Type = $1, .f = toFloat($3) }; }
    | SLOPESCALEDDEPTHBIAS '=' numeric_constant { $$ = SRenderStateValue{ .Type = $1, .f = toFloat($3) }; }
    | DEPTHCLIPENABLE '=' BOOL { $$ = SRenderStateValue{ .Type = $1, .b = $3 }; }
    | SCISSORENABLE '=' BOOL { $$ = SRenderStateValue{ .Type = $1, .b = $3 }; }
    | MULTISAMPLEENABLE '=' BOOL { $$ = SRenderStateValue{ .Type = $1, .b = $3 }; }
    | ANTIALIASEDLINEENABLE '=' BOOL { $$ = SRenderStateValue{ .Type = $1, .b = $3 }; }
    ;

raster_state_list: 
    raster_state_list raster_state ';'
    {
    }
    | raster_state ';'
    {
    }
;


render_state:
    ds_state ';'
    {
        $$ = $1;
    }
    | blendstate_type ';'
    {
        $$ = $1;
    }
    | raster_state ';'
    {
        $$ = $1;
    }
;

/*
    | ZENABLE           { $$ = $1; }
    | ZWRITEENABLE      { $$ = $1; }
    | ZFUNC             { $$ = $1; }
    | CULLMODE          { $$ = $1; }
    | ALPHABLENDENABLE  { $$ = $1; }
    | BLENDENABLE  { $$ = $1; }
    | BLENDOP  { $$ = $1; }
    | ALPHABLEND_SRC    { $$ = $1; }
    | ALPHABLEND_DST    { $$ = $1; }
    | ALPHABLEND_EQUATION { $$ = $1; }
    | ALPHABLEND_SRC_ALPHA { $$ = $1; }
    | ALPHABLEND_DST_ALPHA { $$ = $1; }
    | ALPHABLEND_EQUATION_ALPHA { $$ = $1; }
    | ALPHABLEND_COLOR { $$ = $1; }

;
*/

var_spec:
    INSPECYFIER { $$ = "in"; }
    | OUTSPECYFIER { $$ = "out"; }
    | %empty { $$ = ""; }
    ;

arguments:
    var_spec var_decl {
        $$ = { $2 }; // Создаем вектор с одним аргументом
        $$[0].Spec = $1; // Присваиваем спецификацию
    }
    | var_spec var_decl ',' arguments {
        $$ = $4;
        $$.insert($$.begin(), $2); // Вставляем в начало списка
        $$[0].Spec = $1;
    }
    | %empty { 
        $$ = {}; 
        }
    ;

function_declaration: // Ваше правило для объявления функции
    object_type IDENTIFIER[name] '(' {} arguments ')'{
        $$.Name = $name;
        $$.ReturnType = $object_type; // Предполагается, что это тип возвращаемого значения
        $$.Arguments = $arguments;
        lex_print_state();
    }
    ;

function_definition:
    function_declaration semantic '{' {
        lex_begin_funciton_body();
    }
    function_body_content '}' {
        //FxLog("Close function scope");
        // Возврат обновленного значения функции
        $$.Name = $1.Name; // Возвращает значение из function_declaration
        $$.ReturnType = $1.ReturnType; // Возвращает значение из function_declaration
        $$.Arguments = $1.Arguments; // Возвращает значение из function_declaration
        $$.Semantic = $semantic;
    }
    ;

function_body_content: 
    function_body_content ANYLINE 
    | %empty;

storage_class:
    STATIC { $$ = StorageClass::Static; }
    | %empty { $$ = StorageClass::Extern; }
;

object_type: 
    storage_class TYPE_NAME { 
        $$.Name = $2;
        $$.Storage = $1;
    }
    | storage_class base_type { 
        $$.Name = toString($2);
        $$.Storage = $1;
        $$.Type = $2 ;
    } 
;



fatal_error: FATALERROR { CryFatalError("Stopping paring!!!"); }

register_value: INT | IDENTIFIER;

register_declaration: ':' REGISTER '(' register_value ')' | %empty;

cbuffer: CSTBUFFER IDENTIFIER register_declaration '{' var_decls '}'
{
    FxLog("New CBuffer %s", $2.data());
    lex_pop_state();
}
| CONSTANTBUFFER template_parameter IDENTIFIER register_declaration
{
    FxLog("New CBuffer %s", $3.data());
    lex_pop_state();
};

template_parameter: '<' object_type '>';


struct: STRUCT struct_header struct_body struct_footer
{
    //FxLog("New Struct");
}

struct_header: IDENTIFIER{FxLog("StructName: %s", $1.data()); scanner.register_type($1.data());} | %empty {$$="";};
struct_body: '{' var_decls '}';
struct_footer: IDENTIFIER {FxLog("Declared and defined struct with name: %s", $1.data());} 
| %empty {$$="";} ;

var_decls: var_decls  var_decl ';' | var_decl ';' | struct';';

shader_resource: cbuffer | texture2d | sampler_state;

resource_initializer: %empty | '=' IDENTIFIER;
texture2d: TEXTURE2D_TYPE IDENTIFIER register_declaration resource_initializer
{
    FxLog("texture2d");
}

sampler_state: SAMPLERSTATE IDENTIFIER register_declaration resource_initializer
{
    FxLog("sampler");
}

shader_type 
: VERTEXPROGRAM {$$ = $1;}
| GEOMETRYPROGRAM {$$ = $1;}
| FRAGMENTPROGRAM {$$ = $1;}
;

shader_assignment_shader: direct_declarator {
    $$ = $1;
}
| IDENTIFIER {
    $$ = DirectDeclarator{$1};
};


shader_assignment: shader_type '=' shader_assignment_shader ';' {
    //$$ = std::make_pair($1, $3);
	effect.shader_assignment($1,$3.Name);
}
|
render_state 
{
    //$$ = std::make_pair($1, $3);
    //effect.render_state_assignment($1, $3);
    effect.render_state_assignment($1);
}
//| render_state '=' type_constructor ';'
//{
//    //$$ = std::make_pair($1, $3);
//    //effect.render_state_assignment($1, $3);
//    effect.render_state_assignment($1, $3);
//}

;
shader_assignments:
shader_assignment
| shader_assignments shader_assignment
| %empty
;

/*------------------------------------------------------------------
   pass-states
   TODO: Add the states
*/
passstates: 
    shader_assignments
    {

    }

;

base_type: 
   FLOAT_TYPE { $$ = nvFX::IUniform::TFloat; }
|  FLOAT2_TYPE { $$ = nvFX::IUniform::TVec2; }
|  FLOAT3_TYPE { $$ = nvFX::IUniform::TVec3; }
|  FLOAT4_TYPE { $$ = nvFX::IUniform::TVec4; }
|  INT_TYPE    { $$ = nvFX::IUniform::TInt; }
|  INT2_TYPE { $$ = nvFX::IUniform::TInt2; }
|  INT3_TYPE { $$ = nvFX::IUniform::TInt3; }
|  INT4_TYPE { $$ = nvFX::IUniform::TInt4; }
|  BOOL_TYPE { $$ = nvFX::IUniform::TBool; }
|  BOOL2_TYPE { $$ = nvFX::IUniform::TBool2; }
|  BOOL3_TYPE { $$ = nvFX::IUniform::TBool3; }
|  BOOL4_TYPE { $$ = nvFX::IUniform::TBool4; }
|  MAT2_TYPE { $$ = nvFX::IUniform::TMat2; }
|  MAT2x4_TYPE { $$ = nvFX::IUniform::TMat2x4; }
|  MAT3_TYPE { $$ = nvFX::IUniform::TMat3; }
|  MAT34_TYPE { $$ = nvFX::IUniform::TMat3; }
|  MAT4_TYPE { $$ = nvFX::IUniform::TMat4; }

;

//expression: 

primary_expression
	: IDENTIFIER
	//| CONSTANT
  | INT
  | FLOAT
	//| STRING_LITERAL
	| '(' postfix_expression ')'
	;

postfix_expression
	: primary_expression
	| postfix_expression '[' postfix_expression ']'
	| postfix_expression '(' ')'
//	| postfix_expression '(' argument_expression_list ')'
	| postfix_expression '.' IDENTIFIER
//	| postfix_expression PTR_OP IDENTIFIER
//	| postfix_expression INC_OP
//	| postfix_expression DEC_OP
	;
declarator: direct_declarator
	;
constant_expression: postfix_expression;

parameter_type_list: var_decls;

identifier_list
	: IDENTIFIER
	| identifier_list ',' IDENTIFIER
	;

direct_declarator
	: IDENTIFIER {
        $$ = DirectDeclarator{$1};
        //FxLog("DirectDeclarator: %s", $1.c_str());
    }
	| '(' declarator ')' {$$ = $declarator;}
	| direct_declarator '[' constant_expression ']'
	| direct_declarator '[' ']'
	| direct_declarator '(' parameter_type_list ')'
	| direct_declarator '(' identifier_list ')'
	| direct_declarator '(' ')'{
        $$ = $1;
    }
	;

semantic: 
':' IDENTIFIER {
    //FxLog("Semantic: %s", $2.c_str());
    $$ = $2;
}
| %empty
;


value: 
    INT { $$ = SimpleValue{
        .type = nvFX::IUniform::TInt,
        .i = $1}; 
    }
    | FLOAT { $$ = SimpleValue{
        .type = nvFX::IUniform::TFloat,
        .f = $1}; 
    } 
    | BOOL { $$ = SimpleValue{
        .type = nvFX::IUniform::TBool,
        .b = $1}; 
    }
    | IDENTIFIER { $$ = SimpleValue{
        .type = nvFX::IUniform::TInt,
        .i = 0}; 
    }
;

basic_type_constructor:
                      FLOAT_TYPE '('value')'
                      | BOOL_TYPE '('value')'
                      | INT_TYPE '('value')'
                      | FLOAT2_TYPE '('value ',' value ')'
                      | FLOAT3_TYPE '('value ',' value  ',' value ')'
                      | FLOAT4_TYPE '('value ',' value  ',' value  ',' value ')'
type_constructor: basic_type_constructor;


var_decl: 
object_type direct_declarator semantic
{
    $$ = SVariable{ 
        .Name = $direct_declarator.Name,
        .Type = $object_type, 
        .Semantic = $semantic
    };
}
| object_type direct_declarator semantic annotations '=' type_constructor
{
    $$ = SVariable{ 
        .Name = $direct_declarator.Name,
        .Type = $object_type, 
        .Semantic = $semantic,
        .Annotations = $annotations,
    };
}
| object_type direct_declarator semantic annotations '=' value
{
    $$ = SVariable{ 
        .Name = $direct_declarator.Name,
        .Type = $object_type, 
        .Semantic = $semantic,
        .Annotations = $annotations,
    };
}
| object_type direct_declarator semantic annotations ';'
{
    $$ = SVariable{ 
        .Name = $direct_declarator.Name,
        .Type = $object_type, 
        .Semantic = $semantic,
        .Annotations = $annotations,
    };
}
;


/*
semantic: %empty
| ':' IDENTIFIER { $$ = $2; }
;
*/
/*----------------------
    what a pass can be
*/
pass:
PASS { 
    FxLog("Creating PASS");
    }
  //annotations '{' passstates '}'  {
  annotations '{' passstates '}'  {
  /*
    LOGI("Pass with no name...\n");
    curAnnotations = NULL;
    curRState = NULL;
    curCSState = NULL;
    curDSTState = NULL;
    curPRState = NULL;
*/
    lex_pop_state();
}
| PASS IDENTIFIER {
    SPass pass;
    pass.Name = $2.c_str();
    effect.m_Techniques.back().Passes.push_back(pass);
    //effect.m_shaders.push_back(IEffect::ShaderInfo{$1, $3});
    //effect.m_shaders.push_back(IEffect::ShaderInfo{$1, $3});

    FxLog("Creating PASS %s\n", pass.Name.data());
    //curPass = curTechnique->addPass($2->c_str())->getExInterface();
    //curAnnotations = curPass->annotations()->getExInterface();
    }
  annotations '{' passstates '}' {
    //LOGD("Pass %s...\n", $2->c_str() );
    //delete $2;
    //curAnnotations = NULL;
    //curRState = NULL;
    //curCSState = NULL;
    //curDSTState = NULL;
    //curPRState = NULL;
    lex_pop_state();
}
//| pass error { error(@1, "Error in Pass declaration\n");}
;
/*------------------------------------------------------------------
   passes
*/
passes:
pass
| passes pass
| passes error { error(@1, "Error in Pass list\n");}
;
/*------------------------------------------------------------------
   technique
*/
tech:
TECHNIQUE {
    FxLog("Creation of Technique for NO name\n");
    //curTechnique = curContainer->createTechnique()->getExInterface();
    //curAnnotations = curTechnique->annotations()->getExInterface();
} '{' passes '}' 
| TECHNIQUE IDENTIFIER {
    effect.AddTechnique(CTechnique($2.c_str()));
    //FxLog("creation of Technique %s...\n", tech.Name.data());
    //curTechnique = curContainer->createTechnique($2->c_str())->getExInterface();
    //curAnnotations = curTechnique->annotations()->getExInterface();
    //delete $2;
} annotations '{' passes '}' { 
    lex_pop_state();
    scanner.canNowAddFragment = true;
    //curAnnotations = NULL;
}
;

/*
   ANNOTATIONS - TODO: more types of annotations
 */

STRING_LIST: 
    STR 
{ 
    $$ = std::vector<string>{ $1 };
} 
    | STRING_LIST STR
{
    //FxLog("STRING_LIST: %s", $2.c_str());
    $1.push_back($2);
    $$ = $1;
}
scalar_type: INT_TYPE | FLOAT_TYPE | UNSIGNED_TYPE | STRING_TYPE;
// TODO: research for rule[word] (what is it previous?) 
// most likely it is: https://www.gnu.org/software/bison/manual/html_node/Named-References.html
//annotation_list: annotation_list[previous] annotation
//| annotation;
annotation_list
    : annotation_list annotation {
        $1.push_back($2); // Добавляем аннотацию к уже существующему списку в $1
        $$ = std::move($1);
      }
    | annotation {
        $$.push_back($1); // Добавляем аннотацию к существующему списку
      }
    ;


annotation_value: 
    FLOAT { $$ = std::to_string($1);}
    | INT { $$ = std::to_string($1);}
    | STRING_LIST {
        $$ = std::accumulate($1.begin(), $1.end(), std::string());;
    }
    | UNSIGNED { $$ = std::to_string($1);}
    | IDENTIFIER { $$ = $1; }
;

annotation_header: scalar_type IDENTIFIER 
{
    $$ = $2;
};

| REGISTER;

annotation_base: annotation_header '=' annotation_value {
    $$ = SAnnotation{$1, $3};
};

annotation:  
    annotation_base 
    {
        $$ = $annotation_base;
    }
    | annotation_base ';' 
    {
        $$ = $annotation_base;
    }
;

annotations
    : %empty 
    | '<' annotation_list '>' {
        $$ = $2; // Использование вектора аннотаций из annotation_list
      }
    ;

%%

#include <sstream>
void
yy::parser::error(const location_type& l, const std::string& m)
{
  std::stringstream ss;
  ss << l << ": " << m;
  CryError(ss.str().c_str());    
}
#pragma warning(pop)
