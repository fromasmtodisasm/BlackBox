%skeleton "lalr1.cc"
%require "3.0"

%expect  39

%defines
%define api.token.constructor
%define api.value.type variant
%define parse.assert

%code requires {
    #include <string>
#ifdef VULKAN_SUPPORT
    #include <vulkan/vulkan.h>
#endif
    #include <BlackBox/Renderer/IRender.hpp>
    #include <BlackBox/Renderer/IShader.hpp>
	#include <BlackBox/Renderer/OpenGL/Core.hpp>
    #include <BlackBox/Renderer/BufferManager.hpp>
    class Scanner;
    class Driver;
    //using shader_assignment = std::pair<std::string, std::string>

    //extern void lex_pop_state();
    #define lex_pop_state() scanner.pop_state()


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
            TMat2, TMat3, TMat4, 
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
    CommonCode.clear();
}

// %param { Driver &drv }

%define parse.trace
%define parse.error verbose

%code top{
    #ifdef S_FALSE
    #undef S_FALSE
    #endif
	#pragma warning(push, 0)
    #include "Driver.hpp"
    #include "Scanner.hpp"
    #include "location.hh"

    #define GreenLog(...) gEnv->pLog->Log(__VA_ARGS__)

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
    #include "Effect.hpp"

    std::vector<std::string_view> CommonCode;
    bool is_common;

    static yy::parser::symbol_type yylex(Scanner &scanner, Driver& driver) {
        return scanner.ScanToken();
    }

    std::string location_from_semantic(std::string semantic)
    {
        using namespace std;
        if (semantic == "POSITION")
            return to_string(AttributeLocation::position);
        if (semantic == "NORMAL")
            return to_string(AttributeLocation::normal);
        if (semantic == "TEXCOORD")
            return to_string(AttributeLocation::uv);
        if (semantic == "TANGENT")
            return to_string(AttributeLocation::tangent);
        if (semantic == "COLOR")
            return to_string(AttributeLocation::color);
        return "-1";
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

%type  <std::string>    glsl_header
%type  <std::string>    shader_assignment
%type  <IShader::Type>  shader_type

%type  <std::string>    semantic
%type  <nvFX::IUniform::Type>           base_type

%type  <std::string>    var_decl

%token <std::string>    IDENTIFIER
%token <bool>           TRUE
%token <bool>           FALSE
%token <float>          FLOAT
%token <int>            INT
%token <bool>           BOOL
%token <std::string>    STR

%token              GLSLSHADER /*GLSL shader domain*/
%token              HLSL10SHADER /*HLSL10 shader domain*/
%token              HLSL11SHADER /*HLSL11 shader domain*/
%token              CGSHADER /*CG shader domain*/
%token              SAMPLER_STATE /*Beginning of a sampler state definition*/
%token              DST_STATE
%token              PR_STATE /*Path Rendering State Group*/
%token              COLOR_SAMPLE_STATE
%token              RASTERIZATION_STATE
%token              TECHNIQUE /*Beginning of a technique*/
%token              PASS
%token <std::string>    CODEBODY /* piece of code (GLSL) *including* ending bracket '}' ! But '}' is NOT in the string s */

%token  <std::string>         VARNAME
%token              TEXTURERESOURCE /* resource handle */
%token              TEXTURERESOURCE1D
%token              TEXTURERESOURCE2D
%token              TEXTURERESOURCE3D
%token              TEXTURERESOURCERECT
%token              TEXTURERESOURCECUBE
/*
TODO: New resource types (from D3D)
%token              TEXTURE1DARRAY
%token              TEXTURE2DARRAY
%token              TEXTURECUBEARRAY
%token              TEXTURE2DMS
%token              TEXTURE2MSDARRAY
*/

%token              INPUTLAYOUT

/*------------------------------------------------------------------
  token for uniforms declared outside of any shader code
*/
%token              VOID_TYPE
%token              UNSIGNED
%token              HIGHP
%token              MEDIUMP
%token              LOWP
%token              UNIFORM
%token              CSTBUFFER
%token              FLOAT_TYPE
%token              FLOAT2_TYPE
%token              FLOAT3_TYPE
%token              FLOAT4_TYPE
%token              MAT2_TYPE
%token              MAT3_TYPE
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

%%
%start input;

input: %empty { gEnv->pLog->LogWarning("Empty effect"); }
| ';'
| input tech
| input glsl
| input hlsl
| input texture
| input resource
| input fbo
| input sstate
| input dststate
| input prstate
| input csstate
| input rstate
| input namespace
| input cstbuffer
| input uniforms
| input vertexformat
//| input error
;

shader_type 
: VERTEXPROGRAM {$$ = $1;}
| GEOMETRYPROGRAM {$$ = $1;}
| FRAGMENTPROGRAM {$$ = $1;}
;

shader_assignment: shader_type '=' IDENTIFIER {
    //$$ = std::make_pair($1, $3);
	driver.currentEffect->shader_assignment($1,$3);
}
;
shader_assignments:
shader_assignment
| shader_assignments shader_assignment
| shader_assignments error { error(@1, "Error in shader_assignment list\n");}
;

/*------------------------------------------------------------------
   pass-states
   TODO: Add the states
*/
passstates: %empty { CryLog("Hi"); } 
|passstates shader_assignments 
|passstates input_layout 
;

input_layout: INPUTLAYOUT '{' var_decls  '}' {
    { CryLog("slkfjlkj123123123123"); }
    lex_pop_state();
};

var_decls: var_decl | var_decls var_decl;
var_decl: base_type IDENTIFIER semantic {
    $$ = "layout(location = " + location_from_semantic($3) + ") in " + nvFX::toString($1) + " " + $2 + ";";
    driver.currentEffect->m_Techniques.back().Passes.back().InputLayout.push_back($$);
};


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
|  MAT3_TYPE { $$ = nvFX::IUniform::TMat3; }
|  MAT4_TYPE { $$ = nvFX::IUniform::TMat4; }

;


semantic: %empty
| ':' IDENTIFIER { $$ = $2; }
;
/*----------------------
    what a pass can be
*/
pass:
PASS { 
    CryLog("Creation of PASS");
    }
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
    pass.CommonCode = CommonCode;
    driver.currentEffect->m_Techniques.back().Passes.push_back(pass);
    //driver.currentEffect->m_shaders.push_back(IEffect::ShaderInfo{$1, $3});
    //driver.currentEffect->m_shaders.push_back(IEffect::ShaderInfo{$1, $3});

    CryLog("Creation of PASS %s\n", pass.Name.data());
    //curPass = curTechnique->addPass($2->c_str())->getExInterface();
    //curAnnotations = curPass->annotations()->getExInterface();
    }
  annotations '{' passstates '}'  {
    //LOGD("Pass %s...\n", $2->c_str() );
    //delete $2;
    //curAnnotations = NULL;
    //curRState = NULL;
    //curCSState = NULL;
    //curDSTState = NULL;
    //curPRState = NULL;
    lex_pop_state();
}
| pass error { error(@1, "Error in Pass declaration\n");}
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
annotation: /* empty */
| annotation IDENTIFIER '=' STR ';' {
/*
    if(!curAnnotations)
        curAnnotations = IAnnotationEx::getAnnotationSingleton(2); // need a temporary place since nothing was initialized
    if(!curAnnotations->addAnnotation($2->c_str(), $4->c_str()))
        yyerror("err\n");
    delete $4;
*/
    }
;
annotations2: '<' {CryLog("Begin annotations"); } annotation '>'
;
annotations: /*empty*/
| annotations2
;

glsl_header: 
	GLSLSHADER IDENTIFIER {
        $$ = $2; 
    } 
	| GLSLSHADER {
        $$ = "Common";
        is_common = true;
    }
;

glsl: glsl_header '{' CODEBODY { 
		//gEnv->pLog->Log("$3 Shader $1%s $3parsed", $1.data()); 
        driver.currentEffect->m_shaders.push_back(IEffect::ShaderInfo{$1, $3});
        if ($1 == "Common")
            CommonCode.push_back(driver.currentEffect->m_shaders.back().data);
            CryLog("Current common code in file %s:\n%s", driver.file.data(), CommonCode.back().data());
	}
;


vertexformat: VERTEXFORMAT IDENTIFIER '{' IDENTIFIER '=' format ';' '}'
{
    gEnv->pLog->Log(
    "$3 New vertex format <%s> with field %s (%s = %d)", 
    $2.data(), $4.data(), "vkFormat", 10);
}
;

format: %empty { 
      //$$ = VkFormat(10); 
      CryLog("format not implemented");

      }
;

hlsl: "hlsl";
texture: "texture";
resource: "resource";
fbo: "fbo";
sstate: "sstate";
dststate: "dststate";
prstate: "prstate";
csstate: "csstate";
rstate: "rstate";
namespace: "namespace";
cstbuffer: "cstbuffer";
uniforms: "uniforms";
| input error
;

%%

#include <sstream>
void
yy::parser::error(const location_type& l, const std::string& m)
{
  std::stringstream ss;
  ss << l << ": " << m;
  gEnv->pLog->LogError(ss.str().c_str());    
}
#pragma warning(pop)
