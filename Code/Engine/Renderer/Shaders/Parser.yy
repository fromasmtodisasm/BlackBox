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
%type  <SimpleValue>    float4_constructor
// basic type constructor
%type  <SimpleValue>    basic_type_constructor
%type  <SimpleValue>    type_constructor
%type  <SimpleValue>    parameter_value


%type  <SRenderStateValue> shader_assignment
%type  <fx::ERenderState>  shader_type
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
%type <SObjectTypeInfo>         struct
%type <SVariable>               var_decl
%type <std::vector<SVariable>>  var_decls
%type <std::vector<SVariable>>  struct_body
%type <std::string>            semantic



%type <std::vector<SVariable>>  arguments
%type <SFunction>               function_declaration
%type <SFunction>               function_definition

%type <CTechnique>                      tech
%type <SRenderStateValue>               passstate
%type <std::vector<SRenderStateValue>>  passstates
%type <SPass>                           pass
%type <std::vector<SPass>>              passes


%type <StorageClass> const_storage_class

%type <std::vector<SRenderStateValue>> blendstate_type_list
%type <std::vector<SRenderStateValue>> ds_state_list
%type <std::vector<SRenderStateValue>> raster_state_list
%type <std::vector<SRenderStateValue>> sampler_state_list





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

//"SetDepthStencilState" return yy::parser::make_SETDEPTHSTENCILSTATE(loc);
//"SetBlendState" return yy::parser::make_SETBLENDSTATE(loc);
//"SetRasterizerState" return yy::parser::make_SETRASTERIZERSTATE(loc);
//"SetInputLayout" return yy::parser::make_SETINPUTLAYOUT(loc);
//"SetVertexShader" return yy::parser::make_SETVERTEXSHADER(loc);
//"SetPixelShader" return yy::parser::make_SETPIXELSHADER(loc);
//"SetGeometryShader" return yy::parser::make_SETGEOMETRYSHADER(loc);
//"CompileShader" return yy::parser::make_COMPILESHADER(loc);

%token   SETDEPTHSTENCILSTATE
%token   SETBLENDSTATE 
%token   SETRASTERIZERSTATE
%token   SETVERTEXSHADER
%token   SETPIXELSHADER
%token   SETGEOMETRYSHADER
%token   COMPILESHADER

%type <fx::InternalFunctions> internal_functions
%type <std::vector<SimpleValue>> parameters
%type <SRenderStateValue> call_internal_function





%token              TYPEDEF 
%token              CONST

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
%token <fx::ERenderState> ALPHABLEND_SRC
%token <fx::ERenderState> ALPHABLEND_DST_ALPHA
%token <fx::ERenderState> ALPHABLEND_EQUATION
%token <fx::ERenderState> ALPHABLEND_EQUATION_ALPHA
%token <fx::ERenderState> BLEND_SRC
%token <fx::ERenderState> BLEND_DST
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
%token <fx::ERenderState> SAMPLER_FILTER
%token <fx::ERenderState> SAMPLER_ADDRESSU
%token <fx::ERenderState> SAMPLER_ADDRESSV
%token <fx::ERenderState> SAMPLER_ADDRESSW
%token <fx::ERenderState> SAMPLER_MIPLODBIAS
%token <fx::ERenderState> SAMPLER_MAXANISOTROPY
%token <fx::ERenderState> SAMPLER_COMPARISONFUNC
%token <fx::ERenderState> SAMPLER_BORDERCOLOR
%token <fx::ERenderState> SAMPLER_MINLOD
%token <fx::ERenderState> SAMPLER_MAXLOD
/*------------------------------------------------------------------
  token for blend operations
*/
%token <D3D11_BLEND_OP> BLEND_OP_VALUE

%token <D3D11_BLEND> BLEND_VALUE
%token <D3D11_COMPARISON_FUNC> COMPARISION_FUNC

%token <D3D11_FILL_MODE> FILLMODE_VALUE
%token <D3D11_CULL_MODE> CULLMODE_VALUE

%token <D3D11_FILTER>    SAMPLER_STATE_VALUE
%token <D3D11_TEXTURE_ADDRESS_MODE> SAMPLER_STATE_ADDRESS_VALUE

%type <SRenderStateValue> render_state

%type <SRenderStateValue> ds_state
%type <SRenderStateValue> blendstate_type
%type <SRenderStateValue> raster_state
%type <SRenderStateValue> sampler_state


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
%start translation_unit;

numeric_constant
    : INT { $$ = SimpleValue{.type = nvFX::IUniform::TInt, .i = $1}; }
    | FLOAT { $$ = SimpleValue{.type = nvFX::IUniform::TFloat, .f = $1}; }
    ;

declaration:
tech
{
    FxLog("New Technique %s", $1.Name.data());
    effect.AddTechnique(std::move($1));
}
| typedef
| var_decl ';'
{
    effect.AddVariable($1);
}
| blend_state_declaration ';'
| depth_state_declaration ';'
| raster_state_declaration ';'
| sampler_state_declaration ';'
| shader_resource 
| function_declaration {
    FxLog("Pop lex state from declaration"); lex_pop_state();
}
| struct ';'
{
    CryLog("Struct declaration");
}
| error;
;


external_declaration:
    function_definition {
        $1.Dump();
        effect.AddFunction($1);
    }
    | 
    declaration
    {

    }
    | ';'
;

translation_unit: 
    external_declaration
    | translation_unit external_declaration
    ;

array_declaration: 
    '[' INT ']'
    | '[' IDENTIFIER ']'
    | '[' ']'
    ;

optional_array_declaration: 
    array_declaration
    | %empty
    ;

const_storage_class: CONST { $$ = StorageClass::Const; }
    | %empty { $$ = StorageClass::Extern; }
    ;
//template_parameter
typedef: TYPEDEF const_storage_class object_type IDENTIFIER optional_array_declaration ';'
{
    FxLog("New TypeDef %s", $IDENTIFIER.data());
    scanner.register_type($IDENTIFIER.data(), $object_type);
    //lex_pop_state();
}

blend_state_declaration: BLENDSTATE IDENTIFIER '{' blendstate_type_list '}'
{
    FxLog("New BlendState %s", $2.data());

    auto& blend = effect.CurrentBlend();
    blend.Name = $2;

    for (const auto& state : $4)
    {
        blend.AddState(state);
    }

    effect.m_NumBlendStates++;

    //lex_pop_state();
}

depth_state_declaration: DEPTHSTENCILSTATE IDENTIFIER '{' ds_state_list '}' 
{
    FxLog("New DepthState %s", $2.data());

    auto& depth = effect.CurrentDepthStencil();
    depth.Name = $2;

    for (const auto& state : $4)
    {
        depth.AddState(state);
    }

    effect.m_NumDepthStencilStates++;

    //lex_pop_state();
}

raster_state_declaration: RASTERSTATE IDENTIFIER '{' raster_state_list '}'
{
    FxLog("New RasterState %s", $2.data());

    auto& raster = effect.CurrentRasterizer();
    raster.Name = $2;

    for (const auto& state : $4)
    {
        raster.AddState(state);
    }

    effect.m_NumBlendStates++;


    //lex_pop_state();
}

//sampler_state_declaration: SAMPLERSTATE IDENTIFIER register_declaration resource_initializer '{' sampler_state_list '}'
sampler_state_declaration: SAMPLERSTATE IDENTIFIER register_declaration resource_initializer '{' sampler_state_list '}'
{
    FxLog("New SamplerState %s", $2.data());

    auto& sampler = effect.CurrentSampler();
    sampler.Name = $2;

    for (const auto& state : $6)
    {
        sampler.AddState(state);
    }
    effect.m_NumSamplers++;
    //lex_pop_state();
}
| SAMPLERSTATE IDENTIFIER register_declaration resource_initializer
{
    FxLog("New SamplerState %s", $2.data());
}

;

sampler_state_list: 
    sampler_state_list sampler_state ';'
    {
        $1.push_back($2); // Добавляем аннотацию к уже существующему списку в $1
        $$ = std::move($1);

    }
    | sampler_state ';'
    {
        $$.push_back($1); // Добавляем аннотацию к существующему списку
    }
    ;

ds_state: 
ZENABLE '=' BOOL { $$ = SRenderStateValue{ .Type = $1, .b = $3 }; }
| ZWRITEENABLE '=' BOOL { $$ = SRenderStateValue{ .Type = $1, .b = $3 }; }
| ZFUNC '=' COMPARISION_FUNC { $$ = SRenderStateValue{ .Type = $1, .i = $3 }; }
| ZWRITEMASK '=' INT { $$ = SRenderStateValue{ .Type = $1, .i = $3 }; }
| STENCILENABLE '=' BOOL { $$ = SRenderStateValue{ .Type = $1, .b = $3 }; }

ds_state_list: 
ds_state_list ds_state ';'
{
    $1.push_back($2); // Добавляем аннотацию к уже существующему списку в $1
    $$ = std::move($1);
}
| ds_state ';'
{
    $$.push_back($1); // Добавляем аннотацию к существующему списку
}

//| STENCILENABLE { $$ = $1; }
//| STENCILFAIL { $$ = $1; }
//| STENCILFUNC { $$ = $1; }
//| STENCILMASK { $$ = $1; }
//| STENCILPASS { $$ = $1; }
//| STENCILZFAIL { $$ = $1; }
;

blendstate_type: 
ALPHABLENDENABLE '=' BOOL { $$ = SRenderStateValue{ .Type = $1, .b = $3 }; }
| BLEND_SRC '=' BLEND_VALUE { $$ = SRenderStateValue{ .Type = $1, .i = $3 }; }
| BLEND_DST '=' BLEND_VALUE { $$ = SRenderStateValue{ .Type = $1, .i = $3 }; }
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
        $1.push_back($2); // Добавляем аннотацию к уже существующему списку в $1
        $$ = std::move($1);
    }
    | blendstate_type ';'
    {
        $$.push_back($1); // Добавляем аннотацию к существующему списку
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

sampler_state: 
    SAMPLER_FILTER '=' SAMPLER_STATE_VALUE { $$ = SRenderStateValue{ .Type = $1, .i = $3 }; }
    | SAMPLER_ADDRESSU '=' SAMPLER_STATE_ADDRESS_VALUE { $$ = SRenderStateValue{ .Type = $1, .i = $3 }; }
    | SAMPLER_ADDRESSV '=' SAMPLER_STATE_ADDRESS_VALUE { $$ = SRenderStateValue{ .Type = $1, .i = $3 }; }
    | SAMPLER_ADDRESSW '=' SAMPLER_STATE_ADDRESS_VALUE { $$ = SRenderStateValue{ .Type = $1, .i = $3 }; }
    | SAMPLER_MIPLODBIAS '=' numeric_constant { $$ = SRenderStateValue{ .Type = $1, .f = toFloat($3) }; }
    | SAMPLER_MAXANISOTROPY '=' numeric_constant { $$ = SRenderStateValue{ .Type = $1, .f = toFloat($3) }; }
    | SAMPLER_COMPARISONFUNC '=' COMPARISION_FUNC { $$ = SRenderStateValue{ .Type = $1, .i = $3 }; }
    | SAMPLER_BORDERCOLOR '=' float4_constructor { $$ = SRenderStateValue{ .Type = $1, .f = toFloat($3) }; }
    | SAMPLER_MINLOD '=' numeric_constant { $$ = SRenderStateValue{ .Type = $1, .f = toFloat($3) }; }
    | SAMPLER_MAXLOD '=' numeric_constant { $$ = SRenderStateValue{ .Type = $1, .f = toFloat($3) }; }
    ;

raster_state_list: 
    raster_state_list raster_state ';'
    {
        $1.push_back($2); // Добавляем аннотацию к уже существующему списку в $1
        $$ = std::move($1);
    }
    | raster_state ';'
    {
        $$.push_back($1); // Добавляем аннотацию к существующему списку
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
    | BLEND_SRC    { $$ = $1; }
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
        CryLog("Begin arguments 1");
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

function_declaration: 
    object_type IDENTIFIER[name] '(' arguments ')'{
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

template_parameter_opt: 
    template_parameter 
    | %empty
    ;

object_type: 
    storage_class TYPE_NAME template_parameter_opt { 
        $$.Name = $2;
        $$.Storage = $1;
    }
    | storage_class base_type template_parameter_opt { 
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

template_parameter: '<' typelist '>';


struct: STRUCT struct_header struct_body struct_footer
{
    FxLog("Struct");
    $$.Name = $2;
    $$.Members = std::move($3);
    $$.Type = nvFX::IUniform::TStruct;
    annotations.clear();
    //lex_pop_state();
}
;

struct_header: 
    IDENTIFIER
    {
        scanner.register_type($1, SObjectTypeInfo{.Name = $1, .Type = nvFX::IUniform::TStruct});
        $$ = $1;
    } 
    | %empty {$$="";}
;

struct_body: '{' var_decls '}' {
    $$ = std::move($2);
};

struct_footer: 
    IDENTIFIER {
        FxLog("Declared and defined struct with name: %s", $1.data());
    } 
    | %empty 
    {
        $$="";
    }
;

var_decls: 
    var_decls  var_decl ';'
    {
        $1.push_back($2); // Добавляем аннотацию к уже существующему списку в $1
        $$ = std::move($1);
    }
    | var_decl ';'
    {
        $$.push_back($1); // Добавляем аннотацию к существующему списку
    }
    | struct ';'
    {
        FxLog("Struct");

        $$.push_back(
            SVariable{
                .Name = "struct",
                .Type = $1,
            }
        );
    }
;
typelist: 
    object_type ',' typelist | 
    object_type;

shader_resource: cbuffer | texture2d;

resource_initializer: %empty | '=' IDENTIFIER;

texture2d: TEXTURE2D_TYPE IDENTIFIER register_declaration resource_initializer
{
    FxLog("texture2d");
}
;

shader_type 
: VERTEXPROGRAM {$$ = fx::ERenderState::SETVERTEXSHADER;}
| GEOMETRYPROGRAM {$$ = fx::ERenderState::SETGEOMETRYSHADER;}
| FRAGMENTPROGRAM {$$ = fx::ERenderState::SETPIXELSHADER;}
;

shader_assignment_shader: direct_declarator {
    $$ = $1;
};

internal_functions: 
    SETDEPTHSTENCILSTATE { $$ =  fx::InternalFunctions::SetDepthStencilState; }
    | SETBLENDSTATE { $$ =  fx::InternalFunctions::SetBlendState; }
    | SETRASTERIZERSTATE { $$ =  fx::InternalFunctions::SetRasterizerState; }
    | SETVERTEXSHADER { $$ =  fx::InternalFunctions::SetVertexShader; }
    | SETPIXELSHADER { $$ =  fx::InternalFunctions::SetPixelShader; } 
    | SETGEOMETRYSHADER { $$ =  fx::InternalFunctions::SetGeometryShader; }
    | COMPILESHADER { $$ =  fx::InternalFunctions::CompileShader; }
    ;

parameter_value:
    INT { $$ = SimpleValue{.type = nvFX::IUniform::TInt, .i = $1}; }
    | FLOAT { $$ = SimpleValue{.type = nvFX::IUniform::TFloat, .f = $1}; }
    | BOOL { $$ = SimpleValue{.type = nvFX::IUniform::TBool, .b = $1}; }
    | IDENTIFIER { 
        $$ = SimpleValue{.type = nvFX::IUniform::TInt}; 
        strcpy($$.s, $1.c_str());
    }
    ;

parameters: 
    parameters ',' parameter_value
    {
        $1.push_back($3); // Добавляем аннотацию к уже существующему списку в $1
        $$ = std::move($1);
    }
    | parameter_value 
    {
        $$.push_back($1); // Добавляем аннотацию к существующему списку
    }

    |  %empty {
        $$ = {};
    }
    ;

call_internal_function: 
    internal_functions '(' parameters ')' ';'
    {
        $$.Type = fx::ERenderState::CALL_INTERNAL_FUNCTION;
        $$.i = $1;
        $$.Values = $3;
    }
    ;

shader_assignment: 
    shader_type '=' shader_assignment_shader ';' 
    {
        $$.Type = fx::ERenderState::CALL_INTERNAL_FUNCTION;
        switch ($1)
        {
            case fx::ERenderState::SETVERTEXSHADER:
                $$.i = fx::InternalFunctions::SetVertexShader;
                break;
            case fx::ERenderState::SETPIXELSHADER:
                $$.i = fx::InternalFunctions::SetPixelShader;
                break;
            case fx::ERenderState::SETGEOMETRYSHADER:
                $$.i = fx::InternalFunctions::SetGeometryShader;
                break;
        }
        auto value = SimpleValue{.type = nvFX::IUniform::TInt}; 
        strcpy(value.s, $3.Name.c_str());
        $$.Values.push_back(value);
    }
;

/*------------------------------------------------------------------
   pass-states
   TODO: Add the states
*/

passstate:
    render_state {
        $$ = $1;
    }
    | shader_assignment
    {
        $$ = $1;
    }
    | call_internal_function
    {
        $$ = $1;
    }
;

passstates: 
    passstate
    {
        $$.push_back($1); // Добавляем аннотацию к существующему списку
    }
    | passstates passstate
    {
        $1.push_back($2);
        $$ = std::move($1);
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
//|  TEXTURE1D_TYPE { $$ = nvFX::IUniform::TTexture1D; }
//|  TEXTURE2D_TYPE { $$ = nvFX::IUniform::TTexture2D; }

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
    {
        $$ = $1;
        //lex_pop_state();
    }
	| direct_declarator '(' identifier_list ')'
    {
        $$ = $1;
        //lex_pop_state();
    }
	| direct_declarator '(' ')'{
        $$ = $1;
    }
	;

semantic: 
':' IDENTIFIER {
    //FxLog("Semantic: %s", $2.c_str());
    $$ = $2;
}
| %empty {
    $$ = "";
}
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

float4_constructor: 
    FLOAT4_TYPE '(' value ',' value ',' value ',' value ')'
    {
        $$ = SimpleValue {
            .type = nvFX::IUniform::TVec4,
            .f4 = {toFloat($3), toFloat($5), toFloat($7), toFloat($9)}
        };
    }
    ;

basic_type_constructor:
    FLOAT_TYPE '('value')' { 
        $$ = $3;
    }
    | BOOL_TYPE '('value')' { 
        $$ = $3;
    }
    | INT_TYPE '('value')' { 
        $$ = $3;
    }
    | FLOAT2_TYPE '('value ',' value ')' { 
        $$ = SimpleValue{
            .type = nvFX::IUniform::TVec3,
            .f2 = {toFloat($3), toFloat($5)} 
        };
    }
    | FLOAT3_TYPE '('value ',' value  ',' value ')' {
        $$ = SimpleValue{
            .type = nvFX::IUniform::TVec3,
            .f3 = {toFloat($3), toFloat($5), toFloat($7)} 
        };
    }
    | FLOAT4_TYPE '('value ',' value  ',' value  ',' value ')' {
        $$ = SimpleValue{
            .type = nvFX::IUniform::TVec4, 
            .f4 = {toFloat($3), toFloat($5), toFloat($7), toFloat($9)} 
        };
    }
type_constructor: basic_type_constructor;

var_decl: 
object_type direct_declarator semantic annotations 
{
    $$ = SVariable{ 
        .Name = $direct_declarator.Name,
        .Type = $object_type, 
        .Semantic = $semantic,
        .Annotations = $annotations,
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
;

/*----------------------
    what a pass can be
*/
pass:
    PASS annotations '{' passstates '}'  {
        lex_pop_state();

        $$.Name = "";
        $$.RenderStates = $passstates;
    }
    | PASS IDENTIFIER annotations '{' passstates '}' {
        lex_pop_state();
        FxLog("Creating PASS %s\n", $2.data());

        $$.Name = $2;
        $$.RenderStates = $passstates;

    }
   //| pass error { error(@1, "Error in Pass declaration\n");}
;
/*------------------------------------------------------------------
   passes
*/
passes:
    pass {
        $$.push_back(std::move($1));
    }
    | passes pass
    {
        $1.push_back(std::move($2));
        $$ = std::move($1);
    }
    | passes error { error(@1, "Error in Pass list\n");}
;
/*------------------------------------------------------------------
   technique
*/
tech:
    TECHNIQUE '{' passes '}' 
    {
        lex_pop_state();

        scanner.canNowAddFragment = true;

        $$ =  CTechnique{"", std::move($3) };
    }
    | TECHNIQUE IDENTIFIER annotations '{' passes '}' { 
        lex_pop_state();

        scanner.canNowAddFragment = true;

        FxLog("Creating TECHNIQUE %s\n", $2.data());

        $$ =  CTechnique($2, std::move($5));
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
} | REGISTER { $$ = "register"; }

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
    : %empty { $$ = std::vector<SAnnotation>(); }
    | '<' annotation_list '>' {
        $$ = std::move($2); // Использование вектора аннотаций из annotation_list
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
