%skeleton "lalr1.cc"
%require "3.0"

//NOTE: now 0 shift reduce conflicts
//%expect  2 //!!! разобраться!!!

%defines
%define api.token.constructor
%define api.value.type variant
%define parse.assert

%code requires {
	#undef new
    #include <string>
#ifdef VULKAN_SUPPORT
    #include <vulkan/vulkan.h>
#endif
    #include <BlackBox/Renderer/IRender.hpp>
    #include <BlackBox/Renderer/IShader.hpp>
    //#include "../BufferManager.hpp"
    #ifdef S_FALSE
    #undef S_FALSE
    #endif
    class Scanner;
    class Driver;
    //using shader_assignment = std::pair<std::string, std::string>

    //extern void lex_pop_state();
    #define lex_pop_state() scanner.pop_state()
    #define lex_print_state() scanner.print_state()


    namespace nvFX {
    /*************************************************************************/ /**
    ** \brief IUniform Parameter interface
    **/ /*************************************************************************/ 
    class IUniform 
    {
    protected:
        IUniform() {}
    public:
        virtual ~IUniform() {}
        enum Type {
            TUndefined = 0,
            TInt, TInt2, TInt3, TInt4,
            //UInt,
            TBool, TBool2, TBool3, TBool4,
            TFloat, TVec2, TVec3, TVec4,
            TMat2, TMat2x4, TMat3, TMat34,TMat4, 
            TUBO, // Uniform Buffer Object
            TCB,  // Constant Buffer (D3D)
            TUniform,
            TTexture, // a generic texture : for example when the app created this uniform to bind a texture to a sampler
            // Note: texture==sampler in this type list. D3D uses texture objects, OpenGL will use samplers...
            // Those typed texture/GL-samplers are when the parser encoutered an external declaration of uniform : it needs to know the type
            TTexture1D,
            TTexture2D,
            TTexture2DShadow,
            TTexture2DRect,
            TTexture3D,
            TTextureCube,
            TTexUnit, //TODO: check about this... used for the Cg case (can't assign texunit)
            // TODO : add missing cases
    #ifndef OGLES2
            TSubroutineUniform,
            TSubroutineUniformByID,
    #endif
        };
        enum PrecisionType /// precision is only used for OpenGL ES
        {
            PHighP = 0,
            PMediumP = 1,
            PLowP = 2,
        };
    };
    inline std::string toString(IUniform::Type type)
    {
        switch(type)
        {
            case nvFX::IUniform::TFloat: return "float";
            case nvFX::IUniform::TVec2: return "vec2";
            case nvFX::IUniform::TVec3: return  "vec3";
            case nvFX::IUniform::TVec4: return  "vec4";
            case nvFX::IUniform::TInt: return  "int";
            case nvFX::IUniform::TInt2: return  "ivec2";
            case nvFX::IUniform::TInt3: return  "ivec3";
            case nvFX::IUniform::TInt4: return  "ivec4";
            case nvFX::IUniform::TBool: return  "bool";
            case nvFX::IUniform::TBool2: return  "bvec2";
            case nvFX::IUniform::TBool3: return  "bvec3";
            case nvFX::IUniform::TBool4: return  "bvec4";
            case nvFX::IUniform::TMat2: return  "mat2";
            case nvFX::IUniform::TMat3: return  "mat3";
            case nvFX::IUniform::TMat4: return  "mat4";
            default: assert(0); return "unknown_type";

        }

    }
    }

}

%initial-action
{
    is_common = false;
    Code.clear();
}

// %param { Driver &drv }

%define parse.trace
%define parse.error verbose

%code top{
	#pragma warning(push, 0)
    #include "Driver.hpp"
    #include "Scanner.hpp"
    #include "location.hh"

    #define GreenLog(...) gEnv->pLog->Log(__VA_ARGS__)

    #include "Effect.hpp"

    #ifdef S_FALSE
    #undef S_FALSE
    #endif

    std::vector<std::string> Code;
    bool is_common;
    std::vector<string> annotations;

    static yy::parser::symbol_type yylex(Scanner &scanner, Driver& driver) {
        return scanner.ScanToken();
    }

    using Type = std::string;
}

%lex-param { Scanner &scanner }
%lex-param { Driver &driver }
%parse-param { Scanner &scanner }
%parse-param { Driver &driver }

%locations

%define api.token.prefix {TOK_}

/*==================================================================
  TOKEN
*/
%token
    END 0 "end of file"
;

%type  <std::string>    hlsl_header
%type  <std::string>    shader_header
%type  <std::string>    shader_assignment
%type  <IShader::Type>  shader_type
%type  <std::string>    struct_footer
%type  <std::string>    struct_header

%token <std::string> TYPE_NAME

//%type  <std::string>    semantic
%type  <nvFX::IUniform::Type>           base_type

%token <std::string>    IDENTIFIER
%token <bool>           TRUE
%token <bool>           FALSE
%token <float>          FLOAT
%token <int>            INT
%token <bool>           BOOL
%token <std::string>    STR

%token              HLSL11SHADER /*HLSL11 shader domain*/
%token              TECHNIQUE /*Beginning of a technique*/
%token              PASS
%token <std::string>    CODEBODY /* piece of code (GLSL) *including* ending bracket '}' ! But '}' is NOT in the string s */

/*------------------------------------------------------------------
  token for uniforms declared outside of any shader code
*/
%token              STRING_TYPE
%token              VOID_TYPE
%token              UNSIGNED
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
//%token <passstateType>      DEPTHFUNC 101 //PassState::DepthFunc

%token  <IShader::Type>  VERTEXPROGRAM
%token  <IShader::Type>  FRAGMENTPROGRAM
%token  <IShader::Type>  GEOMETRYPROGRAM
%token  <IShader::Type>  HULLPROGRAM
%token  <IShader::Type>  EVALPROGRAM
%token  SHDPROFILE
%token  SAMPLERRESOURCE
%token  SAMPLERTEXUNIT
%token  SETSAMPLERSTATE
%token  SETDSTSTATE
%token  SETRASTERIZATIONSTATE
%token  SETCOLORSAMPLESTATE
/* for GL_ARB_shader_image_load_store */
%token  IMAGERESOURCE
%token  IMAGEUNIT
%token  IMAGEACCESS
%token  IMAGELAYER
%token  IMAGELAYERED
%token  WRITE_ONLY
%token  READ_ONLY
%token  READ_WRITE

%token VERTEXFORMAT

%token REGISTER
%token FATALERROR

%token STRUCT

%token INSPECYFIER
%token OUTSPECYFIER

%%
%start input;

input: %empty { CryLog("Empty effect"); }
| input ';'
| input tech
| input hlsl
| input var_decl
| input shader_resource
| input function_definition
| input function_declaration {CryLog("Pop lex state from declaration"); lex_pop_state();}
| input fatal_error
| input struct
| input error
;

var_spec: INSPECYFIER | OUTSPECYFIER | %empty;

arguments:  var_spec var_decl | 
            var_spec var_decl ',' arguments  |
            %empty;
        
function_definition: function_declaration semantic '{' { 
    //CryLog("Open function scope");
    } CODEBODY {
    //CryLog("Close function scope"); 
    auto body = $CODEBODY;
    //CryLog("FuncBody: %s", body.data());
}

object_type: TYPE_NAME | base_type;

function_declaration: object_type IDENTIFIER[name] '(' {}arguments ')'{
    CryLog("Parsed function declaration for: [%s]", $name.data());
    lex_print_state();
}; 

fatal_error: FATALERROR { CryFatalError("Stopping paring!!!"); }

register_value: INT | IDENTIFIER;

register_declaration: ':' REGISTER '(' register_value ')' | %empty;

cbuffer: CSTBUFFER IDENTIFIER register_declaration '{' var_decls '}'
{
    CryLog("New CBuffer %s", $2.data());
    lex_pop_state();
}

template_parameter: '<' object_type '>';

cbuffer: CONSTANTBUFFER template_parameter IDENTIFIER register_declaration
{
    CryLog("New CBuffer %s", $3.data());
    lex_pop_state();
}

struct: STRUCT struct_header struct_body struct_footer
{
    //CryLog("New Struct");
}

struct_header: IDENTIFIER{CryLog("StructName: %s", $1.data()); scanner.register_type($1.data());} | %empty {$$="";};
struct_body: '{' var_decls '}';
struct_footer: IDENTIFIER {CryLog("Declared and defined struct with name: %s", $1.data());} 
| %empty {$$="";} ;

var_decls: var_decls  var_decl ';' | var_decl ';' | struct';';

shader_resource: cbuffer | texture2d | sampler_state;

resource_initializer: %empty | '=' IDENTIFIER;
texture2d: TEXTURE2D_TYPE IDENTIFIER register_declaration resource_initializer
{
    CryLog("texture2d");
}

sampler_state: SAMPLERSTATE IDENTIFIER register_declaration resource_initializer
{
    CryLog("sampler");
}

shader_type 
: VERTEXPROGRAM {$$ = $1;}
| GEOMETRYPROGRAM {$$ = $1;}
| FRAGMENTPROGRAM {$$ = $1;}
;

shader_assignment_shader: direct_declarator | IDENTIFIER;

shader_assignment: shader_type '=' shader_assignment_shader ';' {
    //$$ = std::make_pair($1, $3);
  //FIXME:
  #if 0
	//driver.currentEffect->shader_assignment($1,$3);
  #endif
}
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
passstates: '{' shader_assignments '}'
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
	: IDENTIFIER
	| '(' declarator ')'
	| direct_declarator '[' constant_expression ']'
	| direct_declarator '[' ']'
	| direct_declarator '(' parameter_type_list ')'
	| direct_declarator '(' identifier_list ')'
	| direct_declarator '(' ')'
	;

semantic: ':' IDENTIFIER | %empty ;


value: INT | FLOAT;

value_list: value | value_list ',' value;

basic_type_constructor:
                      FLOAT_TYPE '('value')'
                      | INT_TYPE '('value')'
                      | FLOAT2_TYPE '('value ',' value ')'
                      | FLOAT3_TYPE '('value ',' value  ',' value ')'
                      | FLOAT4_TYPE '('value ',' value  ',' value  ',' value ')'
type_constructor: basic_type_constructor;


var_decl: 
object_type direct_declarator semantic
{
    //CryLog("TryParseVarDecl");
}
| object_type direct_declarator semantic annotations '=' type_constructor
| object_type direct_declarator semantic annotations '=' value
| object_type direct_declarator semantic annotations ';'
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
    CryLog("Creating PASS");
    }
  //annotations '{' passstates '}'  {
  annotations passstates  {
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
    pass.Code = Code;
    driver.currentEffect->m_Techniques.back().Passes.push_back(pass);
    //driver.currentEffect->m_shaders.push_back(IEffect::ShaderInfo{$1, $3});
    //driver.currentEffect->m_shaders.push_back(IEffect::ShaderInfo{$1, $3});

    CryLog("Creating PASS %s\n", pass.Name.data());
    //curPass = curTechnique->addPass($2->c_str())->getExInterface();
    //curAnnotations = curPass->annotations()->getExInterface();
    }
  annotations passstates {
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
    CryLog("Creation of Technique for NO name\n");
    //curTechnique = curContainer->createTechnique()->getExInterface();
    //curAnnotations = curTechnique->annotations()->getExInterface();
} '{' passes '}' 
| TECHNIQUE IDENTIFIER {
    CTechnique tech;
    tech.Name =  $2.c_str();
    driver.currentEffect->m_Techniques.push_back(tech);
    CryLog("creation of Technique %s...\n", tech.Name.data());
    //curTechnique = curContainer->createTechnique($2->c_str())->getExInterface();
    //curAnnotations = curTechnique->annotations()->getExInterface();
    //delete $2;
} annotations '{' passes '}' { 
    lex_pop_state();
    //curAnnotations = NULL;
}
;

/*
   ANNOTATIONS - TODO: more types of annotations
 */

STRING_LIST: STR | STRING_LIST STR;
scalar_type: INT_TYPE | FLOAT_TYPE | UNSIGNED | STRING_TYPE;
// TODO: research for rule[word] (what is it previous?) 
// most likely it is: https://www.gnu.org/software/bison/manual/html_node/Named-References.html
annotation_list: annotation_list[previous] annotation
| annotation;

annotation_value: FLOAT | INT | STRING_LIST | UNSIGNED | IDENTIFIER{CryLog("annotation IDENTIFIER");};

annotation_header: scalar_type IDENTIFIER | REGISTER;
annotation_base: annotation_header '=' annotation_value 
annotation:  annotation_base | annotation_base ';' {
    //CryLog("annotation: %s = ", $IDENTIFIER.c_str());
/*
    if(!curAnnotations)
        curAnnotations = IAnnotationEx::getAnnotationSingleton(2); // need a temporary place since nothing was initialized
    if(!curAnnotations->addAnnotation($2->c_str(), $4->c_str()))
        yyerror("err\n");
    delete $4;
*/
    }
;

annotations: 
 %empty {annotations = std::vector<string>();}
| '<' annotation_list '>'
;


hlsl_header: 
	HLSL11SHADER IDENTIFIER {
        $$ = $2; 
    } 
	| HLSL11SHADER {
        $$ = "Common";
        is_common = true;
    }
;

hlsl: shader_header '{' CODEBODY { 
		//gEnv->pLog->Log("$3 Shader $1%s $3parsed", $1.data()); 
        driver.currentEffect->m_shaders.push_back(IEffect::ShaderInfo{$1, $3});
        if ($1 == "Common")
        {
            Code.push_back(driver.currentEffect->m_shaders.back().data);
            is_common = false;
        }
        #if 0
        if (gEnv->pConsole->GetCVar("dump_shaders_on_load")->GetIVal())
            CryLog("Current shader[%s] code in file %s:\n%s", $1.data(), driver.file.data(), driver.currentEffect->m_shaders.back().data.data());
        #endif

	}
;

shader_header: hlsl_header annotations  { $$ = $1; }

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
