%skeleton "lalr1.cc"
%require "3.0"


%defines
%define api.token.constructor
%define api.value.type variant
%define parse.assert

%code requires {
    #include <string>
    #include <vulkan/vulkan.h>
    class Scanner;
    class Driver;
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

%token  VERTEXPROGRAM
%token  FRAGMENTPROGRAM
%token  GEOMETRYPROGRAM
%token  HULLPROGRAM
%token  EVALPROGRAM
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

// VK_FORMATS
/*
%token <VkFormat> FORMAT_UNDEFINED
%token <VkFormat> FORMAT_R4G4_UNORM_PACK8
%token <VkFormat> FORMAT_R4G4B4A4_UNORM_PACK16
%token <VkFormat> FORMAT_B4G4R4A4_UNORM_PACK16
%token <VkFormat> FORMAT_R5G6B5_UNORM_PACK16
%token <VkFormat> FORMAT_B5G6R5_UNORM_PACK16
%token <VkFormat> FORMAT_R5G5B5A1_UNORM_PACK16
%token <VkFormat> FORMAT_B5G5R5A1_UNORM_PACK16
%token <VkFormat> FORMAT_A1R5G5B5_UNORM_PACK16
%token <VkFormat> FORMAT_R8_UNORM
%token <VkFormat> FORMAT_R8_SNORM
%token <VkFormat> FORMAT_R8_USCALED
%token <VkFormat> FORMAT_R8_SSCALED
%token <VkFormat> FORMAT_R8_UINT
%token <VkFormat> FORMAT_R8_SINT
%token <VkFormat> FORMAT_R8_SRGB
%token <VkFormat> FORMAT_R8G8_UNORM
%token <VkFormat> FORMAT_R8G8_SNORM
%token <VkFormat> FORMAT_R8G8_USCALED
%token <VkFormat> FORMAT_R8G8_SSCALED
%token <VkFormat> FORMAT_R8G8_UINT
%token <VkFormat> FORMAT_R8G8_SINT
%token <VkFormat> FORMAT_R8G8_SRGB
%token <VkFormat> FORMAT_R8G8B8_UNORM
%token <VkFormat> FORMAT_R8G8B8_SNORM
%token <VkFormat> FORMAT_R8G8B8_USCALED
%token <VkFormat> FORMAT_R8G8B8_SSCALED
%token <VkFormat> FORMAT_R8G8B8_UINT
%token <VkFormat> FORMAT_R8G8B8_SINT
%token <VkFormat> FORMAT_R8G8B8_SRGB
%token <VkFormat> FORMAT_B8G8R8_UNORM
%token <VkFormat> FORMAT_B8G8R8_SNORM
%token <VkFormat> FORMAT_B8G8R8_USCALED
%token <VkFormat> FORMAT_B8G8R8_SSCALED
%token <VkFormat> FORMAT_B8G8R8_UINT
%token <VkFormat> FORMAT_B8G8R8_SINT
%token <VkFormat> FORMAT_B8G8R8_SRGB
%token <VkFormat> FORMAT_R8G8B8A8_UNORM
%token <VkFormat> FORMAT_R8G8B8A8_SNORM
%token <VkFormat> FORMAT_R8G8B8A8_USCALED
%token <VkFormat> FORMAT_R8G8B8A8_SSCALED
%token <VkFormat> FORMAT_R8G8B8A8_UINT
%token <VkFormat> FORMAT_R8G8B8A8_SINT
%token <VkFormat> FORMAT_R8G8B8A8_SRGB
%token <VkFormat> FORMAT_B8G8R8A8_UNORM
%token <VkFormat> FORMAT_B8G8R8A8_SNORM
%token <VkFormat> FORMAT_B8G8R8A8_USCALED
%token <VkFormat> FORMAT_B8G8R8A8_SSCALED
%token <VkFormat> FORMAT_B8G8R8A8_UINT
%token <VkFormat> FORMAT_B8G8R8A8_SINT
%token <VkFormat> FORMAT_B8G8R8A8_SRGB
%token <VkFormat> FORMAT_A8B8G8R8_UNORM_PACK32
%token <VkFormat> FORMAT_A8B8G8R8_SNORM_PACK32
%token <VkFormat> FORMAT_A8B8G8R8_USCALED_PACK32
%token <VkFormat> FORMAT_A8B8G8R8_SSCALED_PACK32
%token <VkFormat> FORMAT_A8B8G8R8_UINT_PACK32
%token <VkFormat> FORMAT_A8B8G8R8_SINT_PACK32
%token <VkFormat> FORMAT_A8B8G8R8_SRGB_PACK32
%token <VkFormat> FORMAT_A2R10G10B10_UNORM_PACK32
%token <VkFormat> FORMAT_A2R10G10B10_SNORM_PACK32
%token <VkFormat> FORMAT_A2R10G10B10_USCALED_PACK32
%token <VkFormat> FORMAT_A2R10G10B10_SSCALED_PACK32
%token <VkFormat> FORMAT_A2R10G10B10_UINT_PACK32
%token <VkFormat> FORMAT_A2R10G10B10_SINT_PACK32
%token <VkFormat> FORMAT_A2B10G10R10_UNORM_PACK32
%token <VkFormat> FORMAT_A2B10G10R10_SNORM_PACK32
%token <VkFormat> FORMAT_A2B10G10R10_USCALED_PACK32
%token <VkFormat> FORMAT_A2B10G10R10_SSCALED_PACK32
%token <VkFormat> FORMAT_A2B10G10R10_UINT_PACK32
%token <VkFormat> FORMAT_A2B10G10R10_SINT_PACK32
%token <VkFormat> FORMAT_R16_UNORM
%token <VkFormat> FORMAT_R16_SNORM
%token <VkFormat> FORMAT_R16_USCALED
%token <VkFormat> FORMAT_R16_SSCALED
%token <VkFormat> FORMAT_R16_UINT
%token <VkFormat> FORMAT_R16_SINT
%token <VkFormat> FORMAT_R16_SFLOAT
%token <VkFormat> FORMAT_R16G16_UNORM
%token <VkFormat> FORMAT_R16G16_SNORM
%token <VkFormat> FORMAT_R16G16_USCALED
%token <VkFormat> FORMAT_R16G16_SSCALED
%token <VkFormat> FORMAT_R16G16_UINT
%token <VkFormat> FORMAT_R16G16_SINT
%token <VkFormat> FORMAT_R16G16_SFLOAT
%token <VkFormat> FORMAT_R16G16B16_UNORM
%token <VkFormat> FORMAT_R16G16B16_SNORM
%token <VkFormat> FORMAT_R16G16B16_USCALED
%token <VkFormat> FORMAT_R16G16B16_SSCALED
%token <VkFormat> FORMAT_R16G16B16_UINT
%token <VkFormat> FORMAT_R16G16B16_SINT
%token <VkFormat> FORMAT_R16G16B16_SFLOAT
%token <VkFormat> FORMAT_R16G16B16A16_UNORM
%token <VkFormat> FORMAT_R16G16B16A16_SNORM
%token <VkFormat> FORMAT_R16G16B16A16_USCALED
%token <VkFormat> FORMAT_R16G16B16A16_SSCALED
%token <VkFormat> FORMAT_R16G16B16A16_UINT
%token <VkFormat> FORMAT_R16G16B16A16_SINT
%token <VkFormat> FORMAT_R16G16B16A16_SFLOAT
%token <VkFormat> FORMAT_R32_UINT
%token <VkFormat> FORMAT_R32_SINT
%token <VkFormat> FORMAT_R32_SFLOAT
%token <VkFormat> FORMAT_R32G32_UINT
%token <VkFormat> FORMAT_R32G32_SINT
%token <VkFormat> FORMAT_R32G32_SFLOAT
%token <VkFormat> FORMAT_R32G32B32_UINT
%token <VkFormat> FORMAT_R32G32B32_SINT
%token <VkFormat> FORMAT_R32G32B32_SFLOAT
%token <VkFormat> FORMAT_R32G32B32A32_UINT
%token <VkFormat> FORMAT_R32G32B32A32_SINT
%token <VkFormat> FORMAT_R32G32B32A32_SFLOAT
%token <VkFormat> FORMAT_R64_UINT
%token <VkFormat> FORMAT_R64_SINT
%token <VkFormat> FORMAT_R64_SFLOAT
%token <VkFormat> FORMAT_R64G64_UINT
%token <VkFormat> FORMAT_R64G64_SINT
%token <VkFormat> FORMAT_R64G64_SFLOAT
%token <VkFormat> FORMAT_R64G64B64_UINT
%token <VkFormat> FORMAT_R64G64B64_SINT
%token <VkFormat> FORMAT_R64G64B64_SFLOAT
%token <VkFormat> FORMAT_R64G64B64A64_UINT
%token <VkFormat> FORMAT_R64G64B64A64_SINT
%token <VkFormat> FORMAT_R64G64B64A64_SFLOAT
%token <VkFormat> FORMAT_B10G11R11_UFLOAT_PACK32
%token <VkFormat> FORMAT_E5B9G9R9_UFLOAT_PACK32
%token <VkFormat> FORMAT_D16_UNORM
%token <VkFormat> FORMAT_X8_D24_UNORM_PACK32
%token <VkFormat> FORMAT_D32_SFLOAT
%token <VkFormat> FORMAT_S8_UINT
%token <VkFormat> FORMAT_D16_UNORM_S8_UINT
%token <VkFormat> FORMAT_D24_UNORM_S8_UINT
%token <VkFormat> FORMAT_D32_SFLOAT_S8_UINT
%token <VkFormat> FORMAT_BC1_RGB_UNORM_BLOCK
%token <VkFormat> FORMAT_BC1_RGB_SRGB_BLOCK
%token <VkFormat> FORMAT_BC1_RGBA_UNORM_BLOCK
%token <VkFormat> FORMAT_BC1_RGBA_SRGB_BLOCK
%token <VkFormat> FORMAT_BC2_UNORM_BLOCK
%token <VkFormat> FORMAT_BC2_SRGB_BLOCK
%token <VkFormat> FORMAT_BC3_UNORM_BLOCK
%token <VkFormat> FORMAT_BC3_SRGB_BLOCK
%token <VkFormat> FORMAT_BC4_UNORM_BLOCK
%token <VkFormat> FORMAT_BC4_SNORM_BLOCK
%token <VkFormat> FORMAT_BC5_UNORM_BLOCK
%token <VkFormat> FORMAT_BC5_SNORM_BLOCK
%token <VkFormat> FORMAT_BC6H_UFLOAT_BLOCK
%token <VkFormat> FORMAT_BC6H_SFLOAT_BLOCK
%token <VkFormat> FORMAT_BC7_UNORM_BLOCK
%token <VkFormat> FORMAT_BC7_SRGB_BLOCK
%token <VkFormat> FORMAT_ETC2_R8G8B8_UNORM_BLOCK
%token <VkFormat> FORMAT_ETC2_R8G8B8_SRGB_BLOCK
%token <VkFormat> FORMAT_ETC2_R8G8B8A1_UNORM_BLOCK
%token <VkFormat> FORMAT_ETC2_R8G8B8A1_SRGB_BLOCK
%token <VkFormat> FORMAT_ETC2_R8G8B8A8_UNORM_BLOCK
%token <VkFormat> FORMAT_ETC2_R8G8B8A8_SRGB_BLOCK
%token <VkFormat> FORMAT_EAC_R11_UNORM_BLOCK
%token <VkFormat> FORMAT_EAC_R11_SNORM_BLOCK
%token <VkFormat> FORMAT_EAC_R11G11_UNORM_BLOCK
%token <VkFormat> FORMAT_EAC_R11G11_SNORM_BLOCK
%token <VkFormat> FORMAT_ASTC_4x4_UNORM_BLOCK
%token <VkFormat> FORMAT_ASTC_4x4_SRGB_BLOCK
%token <VkFormat> FORMAT_ASTC_5x4_UNORM_BLOCK
%token <VkFormat> FORMAT_ASTC_5x4_SRGB_BLOCK
%token <VkFormat> FORMAT_ASTC_5x5_UNORM_BLOCK
%token <VkFormat> FORMAT_ASTC_5x5_SRGB_BLOCK
%token <VkFormat> FORMAT_ASTC_6x5_UNORM_BLOCK
%token <VkFormat> FORMAT_ASTC_6x5_SRGB_BLOCK
%token <VkFormat> FORMAT_ASTC_6x6_UNORM_BLOCK
%token <VkFormat> FORMAT_ASTC_6x6_SRGB_BLOCK
%token <VkFormat> FORMAT_ASTC_8x5_UNORM_BLOCK
%token <VkFormat> FORMAT_ASTC_8x5_SRGB_BLOCK
%token <VkFormat> FORMAT_ASTC_8x6_UNORM_BLOCK
%token <VkFormat> FORMAT_ASTC_8x6_SRGB_BLOCK
%token <VkFormat> FORMAT_ASTC_8x8_UNORM_BLOCK
%token <VkFormat> FORMAT_ASTC_8x8_SRGB_BLOCK
%token <VkFormat> FORMAT_ASTC_10x5_UNORM_BLOCK
%token <VkFormat> FORMAT_ASTC_10x5_SRGB_BLOCK
%token <VkFormat> FORMAT_ASTC_10x6_UNORM_BLOCK
%token <VkFormat> FORMAT_ASTC_10x6_SRGB_BLOCK
%token <VkFormat> FORMAT_ASTC_10x8_UNORM_BLOCK
%token <VkFormat> FORMAT_ASTC_10x8_SRGB_BLOCK
%token <VkFormat> FORMAT_ASTC_10x10_UNORM_BLOCK
%token <VkFormat> FORMAT_ASTC_10x10_SRGB_BLOCK
%token <VkFormat> FORMAT_ASTC_12x10_UNORM_BLOCK
%token <VkFormat> FORMAT_ASTC_12x10_SRGB_BLOCK
%token <VkFormat> FORMAT_ASTC_12x12_UNORM_BLOCK
%token <VkFormat> FORMAT_ASTC_12x12_SRGB_BLOCK
*/

%token VERTEXFORMAT

%nterm <VkFormat> format

// %printer { yyo << $$; } <*>;

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


tech: "tech";
glsl: 
	GLSLSHADER IDENTIFIER "{" CODEBODY { 
		gEnv->pLog->Log("$3 Shader $1%s $3parsed", $2.data()); 
	}
;


vertexformat: VERTEXFORMAT IDENTIFIER "{" IDENTIFIER "=" format ";" "}"
{
    gEnv->pLog->Log(
    "$3 New vertex format <%s> with field %s (%s = %d)", 
    $2.data(), $4.data(), "vkFormat", $6);
}
;

format: %empty { $$ = VkFormat(10); }
/*
FORMAT_UNDEFINED { $$ = $1; }
| FORMAT_R4G4_UNORM_PACK8 { $$ = $1; }
| FORMAT_R4G4B4A4_UNORM_PACK16 { $$ = $1; }
| FORMAT_B4G4R4A4_UNORM_PACK16 { $$ = $1; }
| FORMAT_R5G6B5_UNORM_PACK16 { $$ = $1; }
| FORMAT_B5G6R5_UNORM_PACK16 { $$ = $1; }
| FORMAT_R5G5B5A1_UNORM_PACK16 { $$ = $1; }
| FORMAT_B5G5R5A1_UNORM_PACK16 { $$ = $1; }
| FORMAT_A1R5G5B5_UNORM_PACK16 { $$ = $1; }
| FORMAT_R8_UNORM { $$ = $1; }
| FORMAT_R8_SNORM { $$ = $1; }
| FORMAT_R8_USCALED { $$ = $1; }
| FORMAT_R8_SSCALED { $$ = $1; }
| FORMAT_R8_UINT { $$ = $1; }
| FORMAT_R8_SINT { $$ = $1; }
| FORMAT_R8_SRGB { $$ = $1; }
| FORMAT_R8G8_UNORM { $$ = $1; }
| FORMAT_R8G8_SNORM { $$ = $1; }
| FORMAT_R8G8_USCALED { $$ = $1; }
| FORMAT_R8G8_SSCALED { $$ = $1; }
| FORMAT_R8G8_UINT { $$ = $1; }
| FORMAT_R8G8_SINT { $$ = $1; }
| FORMAT_R8G8_SRGB { $$ = $1; }
| FORMAT_R8G8B8_UNORM { $$ = $1; }
| FORMAT_R8G8B8_SNORM { $$ = $1; }
| FORMAT_R8G8B8_USCALED { $$ = $1; }
| FORMAT_R8G8B8_SSCALED { $$ = $1; }
| FORMAT_R8G8B8_UINT { $$ = $1; }
| FORMAT_R8G8B8_SINT { $$ = $1; }
| FORMAT_R8G8B8_SRGB { $$ = $1; }
| FORMAT_B8G8R8_UNORM { $$ = $1; }
| FORMAT_B8G8R8_SNORM { $$ = $1; }
| FORMAT_B8G8R8_USCALED { $$ = $1; }
| FORMAT_B8G8R8_SSCALED { $$ = $1; }
| FORMAT_B8G8R8_UINT { $$ = $1; }
| FORMAT_B8G8R8_SINT { $$ = $1; }
| FORMAT_B8G8R8_SRGB { $$ = $1; }
| FORMAT_R8G8B8A8_UNORM { $$ = $1; }
| FORMAT_R8G8B8A8_SNORM { $$ = $1; }
| FORMAT_R8G8B8A8_USCALED { $$ = $1; }
| FORMAT_R8G8B8A8_SSCALED { $$ = $1; }
| FORMAT_R8G8B8A8_UINT { $$ = $1; }
| FORMAT_R8G8B8A8_SINT { $$ = $1; }
| FORMAT_R8G8B8A8_SRGB { $$ = $1; }
| FORMAT_B8G8R8A8_UNORM { $$ = $1; }
| FORMAT_B8G8R8A8_SNORM { $$ = $1; }
| FORMAT_B8G8R8A8_USCALED { $$ = $1; }
| FORMAT_B8G8R8A8_SSCALED { $$ = $1; }
| FORMAT_B8G8R8A8_UINT { $$ = $1; }
| FORMAT_B8G8R8A8_SINT { $$ = $1; }
| FORMAT_B8G8R8A8_SRGB { $$ = $1; }
| FORMAT_A8B8G8R8_UNORM_PACK32 { $$ = $1; }
| FORMAT_A8B8G8R8_SNORM_PACK32 { $$ = $1; }
| FORMAT_A8B8G8R8_USCALED_PACK32 { $$ = $1; }
| FORMAT_A8B8G8R8_SSCALED_PACK32 { $$ = $1; }
| FORMAT_A8B8G8R8_UINT_PACK32 { $$ = $1; }
| FORMAT_A8B8G8R8_SINT_PACK32 { $$ = $1; }
| FORMAT_A8B8G8R8_SRGB_PACK32 { $$ = $1; }
| FORMAT_A2R10G10B10_UNORM_PACK32 { $$ = $1; }
| FORMAT_A2R10G10B10_SNORM_PACK32 { $$ = $1; }
| FORMAT_A2R10G10B10_USCALED_PACK32 { $$ = $1; }
| FORMAT_A2R10G10B10_SSCALED_PACK32 { $$ = $1; }
| FORMAT_A2R10G10B10_UINT_PACK32 { $$ = $1; }
| FORMAT_A2R10G10B10_SINT_PACK32 { $$ = $1; }
| FORMAT_A2B10G10R10_UNORM_PACK32 { $$ = $1; }
| FORMAT_A2B10G10R10_SNORM_PACK32 { $$ = $1; }
| FORMAT_A2B10G10R10_USCALED_PACK32 { $$ = $1; }
| FORMAT_A2B10G10R10_SSCALED_PACK32 { $$ = $1; }
| FORMAT_A2B10G10R10_UINT_PACK32 { $$ = $1; }
| FORMAT_A2B10G10R10_SINT_PACK32 { $$ = $1; }
| FORMAT_R16_UNORM { $$ = $1; }
| FORMAT_R16_SNORM { $$ = $1; }
| FORMAT_R16_USCALED { $$ = $1; }
| FORMAT_R16_SSCALED { $$ = $1; }
| FORMAT_R16_UINT { $$ = $1; }
| FORMAT_R16_SINT { $$ = $1; }
| FORMAT_R16_SFLOAT { $$ = $1; }
| FORMAT_R16G16_UNORM { $$ = $1; }
| FORMAT_R16G16_SNORM { $$ = $1; }
| FORMAT_R16G16_USCALED { $$ = $1; }
| FORMAT_R16G16_SSCALED { $$ = $1; }
| FORMAT_R16G16_UINT { $$ = $1; }
| FORMAT_R16G16_SINT { $$ = $1; }
| FORMAT_R16G16_SFLOAT { $$ = $1; }
| FORMAT_R16G16B16_UNORM { $$ = $1; }
| FORMAT_R16G16B16_SNORM { $$ = $1; }
| FORMAT_R16G16B16_USCALED { $$ = $1; }
| FORMAT_R16G16B16_SSCALED { $$ = $1; }
| FORMAT_R16G16B16_UINT { $$ = $1; }
| FORMAT_R16G16B16_SINT { $$ = $1; }
| FORMAT_R16G16B16_SFLOAT { $$ = $1; }
| FORMAT_R16G16B16A16_UNORM { $$ = $1; }
| FORMAT_R16G16B16A16_SNORM { $$ = $1; }
| FORMAT_R16G16B16A16_USCALED { $$ = $1; }
| FORMAT_R16G16B16A16_SSCALED { $$ = $1; }
| FORMAT_R16G16B16A16_UINT { $$ = $1; }
| FORMAT_R16G16B16A16_SINT { $$ = $1; }
| FORMAT_R16G16B16A16_SFLOAT { $$ = $1; }
| FORMAT_R32_UINT { $$ = $1; }
| FORMAT_R32_SINT { $$ = $1; }
| FORMAT_R32_SFLOAT { $$ = $1; }
| FORMAT_R32G32_UINT { $$ = $1; }
| FORMAT_R32G32_SINT { $$ = $1; }
| FORMAT_R32G32_SFLOAT { $$ = $1; }
| FORMAT_R32G32B32_UINT { $$ = $1; }
| FORMAT_R32G32B32_SINT { $$ = $1; }
| FORMAT_R32G32B32_SFLOAT { $$ = $1; }
| FORMAT_R32G32B32A32_UINT { $$ = $1; }
| FORMAT_R32G32B32A32_SINT { $$ = $1; }
| FORMAT_R32G32B32A32_SFLOAT { $$ = $1; }
| FORMAT_R64_UINT { $$ = $1; }
| FORMAT_R64_SINT { $$ = $1; }
| FORMAT_R64_SFLOAT { $$ = $1; }
| FORMAT_R64G64_UINT { $$ = $1; }
| FORMAT_R64G64_SINT { $$ = $1; }
| FORMAT_R64G64_SFLOAT { $$ = $1; }
| FORMAT_R64G64B64_UINT { $$ = $1; }
| FORMAT_R64G64B64_SINT { $$ = $1; }
| FORMAT_R64G64B64_SFLOAT { $$ = $1; }
| FORMAT_R64G64B64A64_UINT { $$ = $1; }
| FORMAT_R64G64B64A64_SINT { $$ = $1; }
| FORMAT_R64G64B64A64_SFLOAT { $$ = $1; }
| FORMAT_B10G11R11_UFLOAT_PACK32 { $$ = $1; }
| FORMAT_E5B9G9R9_UFLOAT_PACK32 { $$ = $1; }
| FORMAT_D16_UNORM { $$ = $1; }
| FORMAT_X8_D24_UNORM_PACK32 { $$ = $1; }
| FORMAT_D32_SFLOAT { $$ = $1; }
| FORMAT_S8_UINT { $$ = $1; }
| FORMAT_D16_UNORM_S8_UINT { $$ = $1; }
| FORMAT_D24_UNORM_S8_UINT { $$ = $1; }
| FORMAT_D32_SFLOAT_S8_UINT { $$ = $1; }
| FORMAT_BC1_RGB_UNORM_BLOCK { $$ = $1; }
| FORMAT_BC1_RGB_SRGB_BLOCK { $$ = $1; }
| FORMAT_BC1_RGBA_UNORM_BLOCK { $$ = $1; }
| FORMAT_BC1_RGBA_SRGB_BLOCK { $$ = $1; }
| FORMAT_BC2_UNORM_BLOCK { $$ = $1; }
| FORMAT_BC2_SRGB_BLOCK { $$ = $1; }
| FORMAT_BC3_UNORM_BLOCK { $$ = $1; }
| FORMAT_BC3_SRGB_BLOCK { $$ = $1; }
| FORMAT_BC4_UNORM_BLOCK { $$ = $1; }
| FORMAT_BC4_SNORM_BLOCK { $$ = $1; }
| FORMAT_BC5_UNORM_BLOCK { $$ = $1; }
| FORMAT_BC5_SNORM_BLOCK { $$ = $1; }
| FORMAT_BC6H_UFLOAT_BLOCK { $$ = $1; }
| FORMAT_BC6H_SFLOAT_BLOCK { $$ = $1; }
| FORMAT_BC7_UNORM_BLOCK { $$ = $1; }
| FORMAT_BC7_SRGB_BLOCK { $$ = $1; }
| FORMAT_ETC2_R8G8B8_UNORM_BLOCK { $$ = $1; }
| FORMAT_ETC2_R8G8B8_SRGB_BLOCK { $$ = $1; }
| FORMAT_ETC2_R8G8B8A1_UNORM_BLOCK { $$ = $1; }
| FORMAT_ETC2_R8G8B8A1_SRGB_BLOCK { $$ = $1; }
| FORMAT_ETC2_R8G8B8A8_UNORM_BLOCK { $$ = $1; }
| FORMAT_ETC2_R8G8B8A8_SRGB_BLOCK { $$ = $1; }
| FORMAT_EAC_R11_UNORM_BLOCK { $$ = $1; }
| FORMAT_EAC_R11_SNORM_BLOCK { $$ = $1; }
| FORMAT_EAC_R11G11_UNORM_BLOCK { $$ = $1; }
| FORMAT_EAC_R11G11_SNORM_BLOCK { $$ = $1; }
| FORMAT_ASTC_4x4_UNORM_BLOCK { $$ = $1; }
| FORMAT_ASTC_4x4_SRGB_BLOCK { $$ = $1; }
| FORMAT_ASTC_5x4_UNORM_BLOCK { $$ = $1; }
| FORMAT_ASTC_5x4_SRGB_BLOCK { $$ = $1; }
| FORMAT_ASTC_5x5_UNORM_BLOCK { $$ = $1; }
| FORMAT_ASTC_5x5_SRGB_BLOCK { $$ = $1; }
| FORMAT_ASTC_6x5_UNORM_BLOCK { $$ = $1; }
| FORMAT_ASTC_6x5_SRGB_BLOCK { $$ = $1; }
| FORMAT_ASTC_6x6_UNORM_BLOCK { $$ = $1; }
| FORMAT_ASTC_6x6_SRGB_BLOCK { $$ = $1; }
| FORMAT_ASTC_8x5_UNORM_BLOCK { $$ = $1; }
| FORMAT_ASTC_8x5_SRGB_BLOCK { $$ = $1; }
| FORMAT_ASTC_8x6_UNORM_BLOCK { $$ = $1; }
| FORMAT_ASTC_8x6_SRGB_BLOCK { $$ = $1; }
| FORMAT_ASTC_8x8_UNORM_BLOCK { $$ = $1; }
| FORMAT_ASTC_8x8_SRGB_BLOCK { $$ = $1; }
| FORMAT_ASTC_10x5_UNORM_BLOCK { $$ = $1; }
| FORMAT_ASTC_10x5_SRGB_BLOCK { $$ = $1; }
| FORMAT_ASTC_10x6_UNORM_BLOCK { $$ = $1; }
| FORMAT_ASTC_10x6_SRGB_BLOCK { $$ = $1; }
| FORMAT_ASTC_10x8_UNORM_BLOCK { $$ = $1; }
| FORMAT_ASTC_10x8_SRGB_BLOCK { $$ = $1; }
| FORMAT_ASTC_10x10_UNORM_BLOCK { $$ = $1; }
| FORMAT_ASTC_10x10_SRGB_BLOCK { $$ = $1; }
| FORMAT_ASTC_12x10_UNORM_BLOCK { $$ = $1; }
| FORMAT_ASTC_12x10_SRGB_BLOCK { $$ = $1; }
| FORMAT_ASTC_12x12_UNORM_BLOCK { $$ = $1; }
| FORMAT_ASTC_12x12_SRGB_BLOCK { $$ = $1; }
*/
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
