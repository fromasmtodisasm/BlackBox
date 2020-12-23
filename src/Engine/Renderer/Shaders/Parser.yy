%skeleton "lalr1.cc"
%require "3.0"

%expect  36

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
    class Scanner;
    class Driver;
    //using shader_assignment = std::pair<std::string, std::string>

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

/*==================================================================
  TOKEN
*/
%token
    END 0 "end of file"
    ASSIGN "="
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
;

%type  <std::string>    glsl_header
%type  <std::string>    shader_assignment
%type  <IShader::Type>  shader_type

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
| ";"
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
: VERTEXPROGRAM
| GEOMETRYPROGRAM
| FRAGMENTPROGRAM
;

shader_assignment: shader_type '=' IDENTIFIER {
    //$$ = std::make_pair($1, $3);
    CryLog("ident for shader_type: %s", $3.data());
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
passstates 
: shader_assignments 
;
/*----------------------
    what a pass can be
*/
pass:
PASS { 
    CryLog("Creation of PASS");
    }
  annotations "{" passstates "}"  {
  /*
    LOGI("Pass with no name...\n");
    curAnnotations = NULL;
    curRState = NULL;
    curCSState = NULL;
    curDSTState = NULL;
    curPRState = NULL;
    lex_pop_state();
*/
}
| PASS IDENTIFIER {
    SPass pass;
    pass.Name = $2.c_str();
    //driver.currentEffect->m_shaders.push_back(IEffect::ShaderInfo{$1, $3});
    //driver.currentEffect->m_shaders.push_back(IEffect::ShaderInfo{$1, $3});

    CryLog("Creation of PASS %s\n", pass.Name);
    //curPass = curTechnique->addPass($2->c_str())->getExInterface();
    //curAnnotations = curPass->annotations()->getExInterface();
    }
  annotations "{" passstates "}"  {
    //LOGD("Pass %s...\n", $2->c_str() );
    //delete $2;
    //curAnnotations = NULL;
    //curRState = NULL;
    //curCSState = NULL;
    //curDSTState = NULL;
    //curPRState = NULL;
    //lex_pop_state();
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
} "{" passes "}" 
| TECHNIQUE IDENTIFIER {
    CryLog("creation of Technique %s...\n", $2.c_str() );
    //curTechnique = curContainer->createTechnique($2->c_str())->getExInterface();
    //curAnnotations = curTechnique->annotations()->getExInterface();
    //delete $2;
} annotations "{" passes "}" { 
    //lex_pop_state();
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
    }
;

glsl: glsl_header "{" CODEBODY { 
		//gEnv->pLog->Log("$3 Shader $1%s $3parsed", $1.data()); 
        driver.currentEffect->m_shaders.push_back(IEffect::ShaderInfo{$1, $3});
	}
;


vertexformat: VERTEXFORMAT IDENTIFIER "{" IDENTIFIER "=" format ";" "}"
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
