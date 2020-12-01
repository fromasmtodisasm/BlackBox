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

FORMAT_UNDEFINED { return yy::parser::make_FORMAT_UNDEFINED(VK_FORMAT_UNDEFINED, loc); }
FORMAT_R4G4_UNORM_PACK8 { return yy::parser::make_FORMAT_R4G4_UNORM_PACK8(VK_FORMAT_R4G4_UNORM_PACK8, loc); }
FORMAT_R4G4B4A4_UNORM_PACK16 { return yy::parser::make_FORMAT_R4G4B4A4_UNORM_PACK16(VK_FORMAT_R4G4B4A4_UNORM_PACK16, loc); }
FORMAT_B4G4R4A4_UNORM_PACK16 { return yy::parser::make_FORMAT_B4G4R4A4_UNORM_PACK16(VK_FORMAT_B4G4R4A4_UNORM_PACK16, loc); }
FORMAT_R5G6B5_UNORM_PACK16 { return yy::parser::make_FORMAT_R5G6B5_UNORM_PACK16(VK_FORMAT_R5G6B5_UNORM_PACK16, loc); }
FORMAT_B5G6R5_UNORM_PACK16 { return yy::parser::make_FORMAT_B5G6R5_UNORM_PACK16(VK_FORMAT_B5G6R5_UNORM_PACK16, loc); }
FORMAT_R5G5B5A1_UNORM_PACK16 { return yy::parser::make_FORMAT_R5G5B5A1_UNORM_PACK16(VK_FORMAT_R5G5B5A1_UNORM_PACK16, loc); }
FORMAT_B5G5R5A1_UNORM_PACK16 { return yy::parser::make_FORMAT_B5G5R5A1_UNORM_PACK16(VK_FORMAT_B5G5R5A1_UNORM_PACK16, loc); }
FORMAT_A1R5G5B5_UNORM_PACK16 { return yy::parser::make_FORMAT_A1R5G5B5_UNORM_PACK16(VK_FORMAT_A1R5G5B5_UNORM_PACK16, loc); }
FORMAT_R8_UNORM { return yy::parser::make_FORMAT_R8_UNORM(VK_FORMAT_R8_UNORM, loc); }
FORMAT_R8_SNORM { return yy::parser::make_FORMAT_R8_SNORM(VK_FORMAT_R8_SNORM, loc); }
FORMAT_R8_USCALED { return yy::parser::make_FORMAT_R8_USCALED(VK_FORMAT_R8_USCALED, loc); }
FORMAT_R8_SSCALED { return yy::parser::make_FORMAT_R8_SSCALED(VK_FORMAT_R8_SSCALED, loc); }
FORMAT_R8_UINT { return yy::parser::make_FORMAT_R8_UINT(VK_FORMAT_R8_UINT, loc); }
FORMAT_R8_SINT { return yy::parser::make_FORMAT_R8_SINT(VK_FORMAT_R8_SINT, loc); }
FORMAT_R8_SRGB { return yy::parser::make_FORMAT_R8_SRGB(VK_FORMAT_R8_SRGB, loc); }
FORMAT_R8G8_UNORM { return yy::parser::make_FORMAT_R8G8_UNORM(VK_FORMAT_R8G8_UNORM, loc); }
FORMAT_R8G8_SNORM { return yy::parser::make_FORMAT_R8G8_SNORM(VK_FORMAT_R8G8_SNORM, loc); }
FORMAT_R8G8_USCALED { return yy::parser::make_FORMAT_R8G8_USCALED(VK_FORMAT_R8G8_USCALED, loc); }
FORMAT_R8G8_SSCALED { return yy::parser::make_FORMAT_R8G8_SSCALED(VK_FORMAT_R8G8_SSCALED, loc); }
FORMAT_R8G8_UINT { return yy::parser::make_FORMAT_R8G8_UINT(VK_FORMAT_R8G8_UINT, loc); }
FORMAT_R8G8_SINT { return yy::parser::make_FORMAT_R8G8_SINT(VK_FORMAT_R8G8_SINT, loc); }
FORMAT_R8G8_SRGB { return yy::parser::make_FORMAT_R8G8_SRGB(VK_FORMAT_R8G8_SRGB, loc); }
FORMAT_R8G8B8_UNORM { return yy::parser::make_FORMAT_R8G8B8_UNORM(VK_FORMAT_R8G8B8_UNORM, loc); }
FORMAT_R8G8B8_SNORM { return yy::parser::make_FORMAT_R8G8B8_SNORM(VK_FORMAT_R8G8B8_SNORM, loc); }
FORMAT_R8G8B8_USCALED { return yy::parser::make_FORMAT_R8G8B8_USCALED(VK_FORMAT_R8G8B8_USCALED, loc); }
FORMAT_R8G8B8_SSCALED { return yy::parser::make_FORMAT_R8G8B8_SSCALED(VK_FORMAT_R8G8B8_SSCALED, loc); }
FORMAT_R8G8B8_UINT { return yy::parser::make_FORMAT_R8G8B8_UINT(VK_FORMAT_R8G8B8_UINT, loc); }
FORMAT_R8G8B8_SINT { return yy::parser::make_FORMAT_R8G8B8_SINT(VK_FORMAT_R8G8B8_SINT, loc); }
FORMAT_R8G8B8_SRGB { return yy::parser::make_FORMAT_R8G8B8_SRGB(VK_FORMAT_R8G8B8_SRGB, loc); }
FORMAT_B8G8R8_UNORM { return yy::parser::make_FORMAT_B8G8R8_UNORM(VK_FORMAT_B8G8R8_UNORM, loc); }
FORMAT_B8G8R8_SNORM { return yy::parser::make_FORMAT_B8G8R8_SNORM(VK_FORMAT_B8G8R8_SNORM, loc); }
FORMAT_B8G8R8_USCALED { return yy::parser::make_FORMAT_B8G8R8_USCALED(VK_FORMAT_B8G8R8_USCALED, loc); }
FORMAT_B8G8R8_SSCALED { return yy::parser::make_FORMAT_B8G8R8_SSCALED(VK_FORMAT_B8G8R8_SSCALED, loc); }
FORMAT_B8G8R8_UINT { return yy::parser::make_FORMAT_B8G8R8_UINT(VK_FORMAT_B8G8R8_UINT, loc); }
FORMAT_B8G8R8_SINT { return yy::parser::make_FORMAT_B8G8R8_SINT(VK_FORMAT_B8G8R8_SINT, loc); }
FORMAT_B8G8R8_SRGB { return yy::parser::make_FORMAT_B8G8R8_SRGB(VK_FORMAT_B8G8R8_SRGB, loc); }
FORMAT_R8G8B8A8_UNORM { return yy::parser::make_FORMAT_R8G8B8A8_UNORM(VK_FORMAT_R8G8B8A8_UNORM, loc); }
FORMAT_R8G8B8A8_SNORM { return yy::parser::make_FORMAT_R8G8B8A8_SNORM(VK_FORMAT_R8G8B8A8_SNORM, loc); }
FORMAT_R8G8B8A8_USCALED { return yy::parser::make_FORMAT_R8G8B8A8_USCALED(VK_FORMAT_R8G8B8A8_USCALED, loc); }
FORMAT_R8G8B8A8_SSCALED { return yy::parser::make_FORMAT_R8G8B8A8_SSCALED(VK_FORMAT_R8G8B8A8_SSCALED, loc); }
FORMAT_R8G8B8A8_UINT { return yy::parser::make_FORMAT_R8G8B8A8_UINT(VK_FORMAT_R8G8B8A8_UINT, loc); }
FORMAT_R8G8B8A8_SINT { return yy::parser::make_FORMAT_R8G8B8A8_SINT(VK_FORMAT_R8G8B8A8_SINT, loc); }
FORMAT_R8G8B8A8_SRGB { return yy::parser::make_FORMAT_R8G8B8A8_SRGB(VK_FORMAT_R8G8B8A8_SRGB, loc); }
FORMAT_B8G8R8A8_UNORM { return yy::parser::make_FORMAT_B8G8R8A8_UNORM(VK_FORMAT_B8G8R8A8_UNORM, loc); }
FORMAT_B8G8R8A8_SNORM { return yy::parser::make_FORMAT_B8G8R8A8_SNORM(VK_FORMAT_B8G8R8A8_SNORM, loc); }
FORMAT_B8G8R8A8_USCALED { return yy::parser::make_FORMAT_B8G8R8A8_USCALED(VK_FORMAT_B8G8R8A8_USCALED, loc); }
FORMAT_B8G8R8A8_SSCALED { return yy::parser::make_FORMAT_B8G8R8A8_SSCALED(VK_FORMAT_B8G8R8A8_SSCALED, loc); }
FORMAT_B8G8R8A8_UINT { return yy::parser::make_FORMAT_B8G8R8A8_UINT(VK_FORMAT_B8G8R8A8_UINT, loc); }
FORMAT_B8G8R8A8_SINT { return yy::parser::make_FORMAT_B8G8R8A8_SINT(VK_FORMAT_B8G8R8A8_SINT, loc); }
FORMAT_B8G8R8A8_SRGB { return yy::parser::make_FORMAT_B8G8R8A8_SRGB(VK_FORMAT_B8G8R8A8_SRGB, loc); }
FORMAT_A8B8G8R8_UNORM_PACK32 { return yy::parser::make_FORMAT_A8B8G8R8_UNORM_PACK32(VK_FORMAT_A8B8G8R8_UNORM_PACK32, loc); }
FORMAT_A8B8G8R8_SNORM_PACK32 { return yy::parser::make_FORMAT_A8B8G8R8_SNORM_PACK32(VK_FORMAT_A8B8G8R8_SNORM_PACK32, loc); }
FORMAT_A8B8G8R8_USCALED_PACK32 { return yy::parser::make_FORMAT_A8B8G8R8_USCALED_PACK32(VK_FORMAT_A8B8G8R8_USCALED_PACK32, loc); }
FORMAT_A8B8G8R8_SSCALED_PACK32 { return yy::parser::make_FORMAT_A8B8G8R8_SSCALED_PACK32(VK_FORMAT_A8B8G8R8_SSCALED_PACK32, loc); }
FORMAT_A8B8G8R8_UINT_PACK32 { return yy::parser::make_FORMAT_A8B8G8R8_UINT_PACK32(VK_FORMAT_A8B8G8R8_UINT_PACK32, loc); }
FORMAT_A8B8G8R8_SINT_PACK32 { return yy::parser::make_FORMAT_A8B8G8R8_SINT_PACK32(VK_FORMAT_A8B8G8R8_SINT_PACK32, loc); }
FORMAT_A8B8G8R8_SRGB_PACK32 { return yy::parser::make_FORMAT_A8B8G8R8_SRGB_PACK32(VK_FORMAT_A8B8G8R8_SRGB_PACK32, loc); }
FORMAT_A2R10G10B10_UNORM_PACK32 { return yy::parser::make_FORMAT_A2R10G10B10_UNORM_PACK32(VK_FORMAT_A2R10G10B10_UNORM_PACK32, loc); }
FORMAT_A2R10G10B10_SNORM_PACK32 { return yy::parser::make_FORMAT_A2R10G10B10_SNORM_PACK32(VK_FORMAT_A2R10G10B10_SNORM_PACK32, loc); }
FORMAT_A2R10G10B10_USCALED_PACK32 { return yy::parser::make_FORMAT_A2R10G10B10_USCALED_PACK32(VK_FORMAT_A2R10G10B10_USCALED_PACK32, loc); }
FORMAT_A2R10G10B10_SSCALED_PACK32 { return yy::parser::make_FORMAT_A2R10G10B10_SSCALED_PACK32(VK_FORMAT_A2R10G10B10_SSCALED_PACK32, loc); }
FORMAT_A2R10G10B10_UINT_PACK32 { return yy::parser::make_FORMAT_A2R10G10B10_UINT_PACK32(VK_FORMAT_A2R10G10B10_UINT_PACK32, loc); }
FORMAT_A2R10G10B10_SINT_PACK32 { return yy::parser::make_FORMAT_A2R10G10B10_SINT_PACK32(VK_FORMAT_A2R10G10B10_SINT_PACK32, loc); }
FORMAT_A2B10G10R10_UNORM_PACK32 { return yy::parser::make_FORMAT_A2B10G10R10_UNORM_PACK32(VK_FORMAT_A2B10G10R10_UNORM_PACK32, loc); }
FORMAT_A2B10G10R10_SNORM_PACK32 { return yy::parser::make_FORMAT_A2B10G10R10_SNORM_PACK32(VK_FORMAT_A2B10G10R10_SNORM_PACK32, loc); }
FORMAT_A2B10G10R10_USCALED_PACK32 { return yy::parser::make_FORMAT_A2B10G10R10_USCALED_PACK32(VK_FORMAT_A2B10G10R10_USCALED_PACK32, loc); }
FORMAT_A2B10G10R10_SSCALED_PACK32 { return yy::parser::make_FORMAT_A2B10G10R10_SSCALED_PACK32(VK_FORMAT_A2B10G10R10_SSCALED_PACK32, loc); }
FORMAT_A2B10G10R10_UINT_PACK32 { return yy::parser::make_FORMAT_A2B10G10R10_UINT_PACK32(VK_FORMAT_A2B10G10R10_UINT_PACK32, loc); }
FORMAT_A2B10G10R10_SINT_PACK32 { return yy::parser::make_FORMAT_A2B10G10R10_SINT_PACK32(VK_FORMAT_A2B10G10R10_SINT_PACK32, loc); }
FORMAT_R16_UNORM { return yy::parser::make_FORMAT_R16_UNORM(VK_FORMAT_R16_UNORM, loc); }
FORMAT_R16_SNORM { return yy::parser::make_FORMAT_R16_SNORM(VK_FORMAT_R16_SNORM, loc); }
FORMAT_R16_USCALED { return yy::parser::make_FORMAT_R16_USCALED(VK_FORMAT_R16_USCALED, loc); }
FORMAT_R16_SSCALED { return yy::parser::make_FORMAT_R16_SSCALED(VK_FORMAT_R16_SSCALED, loc); }
FORMAT_R16_UINT { return yy::parser::make_FORMAT_R16_UINT(VK_FORMAT_R16_UINT, loc); }
FORMAT_R16_SINT { return yy::parser::make_FORMAT_R16_SINT(VK_FORMAT_R16_SINT, loc); }
FORMAT_R16_SFLOAT { return yy::parser::make_FORMAT_R16_SFLOAT(VK_FORMAT_R16_SFLOAT, loc); }
FORMAT_R16G16_UNORM { return yy::parser::make_FORMAT_R16G16_UNORM(VK_FORMAT_R16G16_UNORM, loc); }
FORMAT_R16G16_SNORM { return yy::parser::make_FORMAT_R16G16_SNORM(VK_FORMAT_R16G16_SNORM, loc); }
FORMAT_R16G16_USCALED { return yy::parser::make_FORMAT_R16G16_USCALED(VK_FORMAT_R16G16_USCALED, loc); }
FORMAT_R16G16_SSCALED { return yy::parser::make_FORMAT_R16G16_SSCALED(VK_FORMAT_R16G16_SSCALED, loc); }
FORMAT_R16G16_UINT { return yy::parser::make_FORMAT_R16G16_UINT(VK_FORMAT_R16G16_UINT, loc); }
FORMAT_R16G16_SINT { return yy::parser::make_FORMAT_R16G16_SINT(VK_FORMAT_R16G16_SINT, loc); }
FORMAT_R16G16_SFLOAT { return yy::parser::make_FORMAT_R16G16_SFLOAT(VK_FORMAT_R16G16_SFLOAT, loc); }
FORMAT_R16G16B16_UNORM { return yy::parser::make_FORMAT_R16G16B16_UNORM(VK_FORMAT_R16G16B16_UNORM, loc); }
FORMAT_R16G16B16_SNORM { return yy::parser::make_FORMAT_R16G16B16_SNORM(VK_FORMAT_R16G16B16_SNORM, loc); }
FORMAT_R16G16B16_USCALED { return yy::parser::make_FORMAT_R16G16B16_USCALED(VK_FORMAT_R16G16B16_USCALED, loc); }
FORMAT_R16G16B16_SSCALED { return yy::parser::make_FORMAT_R16G16B16_SSCALED(VK_FORMAT_R16G16B16_SSCALED, loc); }
FORMAT_R16G16B16_UINT { return yy::parser::make_FORMAT_R16G16B16_UINT(VK_FORMAT_R16G16B16_UINT, loc); }
FORMAT_R16G16B16_SINT { return yy::parser::make_FORMAT_R16G16B16_SINT(VK_FORMAT_R16G16B16_SINT, loc); }
FORMAT_R16G16B16_SFLOAT { return yy::parser::make_FORMAT_R16G16B16_SFLOAT(VK_FORMAT_R16G16B16_SFLOAT, loc); }
FORMAT_R16G16B16A16_UNORM { return yy::parser::make_FORMAT_R16G16B16A16_UNORM(VK_FORMAT_R16G16B16A16_UNORM, loc); }
FORMAT_R16G16B16A16_SNORM { return yy::parser::make_FORMAT_R16G16B16A16_SNORM(VK_FORMAT_R16G16B16A16_SNORM, loc); }
FORMAT_R16G16B16A16_USCALED { return yy::parser::make_FORMAT_R16G16B16A16_USCALED(VK_FORMAT_R16G16B16A16_USCALED, loc); }
FORMAT_R16G16B16A16_SSCALED { return yy::parser::make_FORMAT_R16G16B16A16_SSCALED(VK_FORMAT_R16G16B16A16_SSCALED, loc); }
FORMAT_R16G16B16A16_UINT { return yy::parser::make_FORMAT_R16G16B16A16_UINT(VK_FORMAT_R16G16B16A16_UINT, loc); }
FORMAT_R16G16B16A16_SINT { return yy::parser::make_FORMAT_R16G16B16A16_SINT(VK_FORMAT_R16G16B16A16_SINT, loc); }
FORMAT_R16G16B16A16_SFLOAT { return yy::parser::make_FORMAT_R16G16B16A16_SFLOAT(VK_FORMAT_R16G16B16A16_SFLOAT, loc); }
FORMAT_R32_UINT { return yy::parser::make_FORMAT_R32_UINT(VK_FORMAT_R32_UINT, loc); }
FORMAT_R32_SINT { return yy::parser::make_FORMAT_R32_SINT(VK_FORMAT_R32_SINT, loc); }
FORMAT_R32_SFLOAT { return yy::parser::make_FORMAT_R32_SFLOAT(VK_FORMAT_R32_SFLOAT, loc); }
FORMAT_R32G32_UINT { return yy::parser::make_FORMAT_R32G32_UINT(VK_FORMAT_R32G32_UINT, loc); }
FORMAT_R32G32_SINT { return yy::parser::make_FORMAT_R32G32_SINT(VK_FORMAT_R32G32_SINT, loc); }
FORMAT_R32G32_SFLOAT { return yy::parser::make_FORMAT_R32G32_SFLOAT(VK_FORMAT_R32G32_SFLOAT, loc); }
FORMAT_R32G32B32_UINT { return yy::parser::make_FORMAT_R32G32B32_UINT(VK_FORMAT_R32G32B32_UINT, loc); }
FORMAT_R32G32B32_SINT { return yy::parser::make_FORMAT_R32G32B32_SINT(VK_FORMAT_R32G32B32_SINT, loc); }
FORMAT_R32G32B32_SFLOAT { return yy::parser::make_FORMAT_R32G32B32_SFLOAT(VK_FORMAT_R32G32B32_SFLOAT, loc); }
FORMAT_R32G32B32A32_UINT { return yy::parser::make_FORMAT_R32G32B32A32_UINT(VK_FORMAT_R32G32B32A32_UINT, loc); }
FORMAT_R32G32B32A32_SINT { return yy::parser::make_FORMAT_R32G32B32A32_SINT(VK_FORMAT_R32G32B32A32_SINT, loc); }
FORMAT_R32G32B32A32_SFLOAT { gEnv->pLog->LogWarning("Suka %d", VK_FORMAT_R32G32B32A32_SFLOAT); return yy::parser::make_FORMAT_R32G32B32A32_SFLOAT(VK_FORMAT_R32G32B32A32_SFLOAT, loc); }
FORMAT_R64_UINT { return yy::parser::make_FORMAT_R64_UINT(VK_FORMAT_R64_UINT, loc); }
FORMAT_R64_SINT { return yy::parser::make_FORMAT_R64_SINT(VK_FORMAT_R64_SINT, loc); }
FORMAT_R64_SFLOAT { return yy::parser::make_FORMAT_R64_SFLOAT(VK_FORMAT_R64_SFLOAT, loc); }
FORMAT_R64G64_UINT { return yy::parser::make_FORMAT_R64G64_UINT(VK_FORMAT_R64G64_UINT, loc); }
FORMAT_R64G64_SINT { return yy::parser::make_FORMAT_R64G64_SINT(VK_FORMAT_R64G64_SINT, loc); }
FORMAT_R64G64_SFLOAT { return yy::parser::make_FORMAT_R64G64_SFLOAT(VK_FORMAT_R64G64_SFLOAT, loc); }
FORMAT_R64G64B64_UINT { return yy::parser::make_FORMAT_R64G64B64_UINT(VK_FORMAT_R64G64B64_UINT, loc); }
FORMAT_R64G64B64_SINT { return yy::parser::make_FORMAT_R64G64B64_SINT(VK_FORMAT_R64G64B64_SINT, loc); }
FORMAT_R64G64B64_SFLOAT { return yy::parser::make_FORMAT_R64G64B64_SFLOAT(VK_FORMAT_R64G64B64_SFLOAT, loc); }
FORMAT_R64G64B64A64_UINT { return yy::parser::make_FORMAT_R64G64B64A64_UINT(VK_FORMAT_R64G64B64A64_UINT, loc); }
FORMAT_R64G64B64A64_SINT { return yy::parser::make_FORMAT_R64G64B64A64_SINT(VK_FORMAT_R64G64B64A64_SINT, loc); }
FORMAT_R64G64B64A64_SFLOAT { return yy::parser::make_FORMAT_R64G64B64A64_SFLOAT(VK_FORMAT_R64G64B64A64_SFLOAT, loc); }
FORMAT_B10G11R11_UFLOAT_PACK32 { return yy::parser::make_FORMAT_B10G11R11_UFLOAT_PACK32(VK_FORMAT_B10G11R11_UFLOAT_PACK32, loc); }
FORMAT_E5B9G9R9_UFLOAT_PACK32 { return yy::parser::make_FORMAT_E5B9G9R9_UFLOAT_PACK32(VK_FORMAT_E5B9G9R9_UFLOAT_PACK32, loc); }
FORMAT_D16_UNORM { return yy::parser::make_FORMAT_D16_UNORM(VK_FORMAT_D16_UNORM, loc); }
FORMAT_X8_D24_UNORM_PACK32 { return yy::parser::make_FORMAT_X8_D24_UNORM_PACK32(VK_FORMAT_X8_D24_UNORM_PACK32, loc); }
FORMAT_D32_SFLOAT { return yy::parser::make_FORMAT_D32_SFLOAT(VK_FORMAT_D32_SFLOAT, loc); }
FORMAT_S8_UINT { return yy::parser::make_FORMAT_S8_UINT(VK_FORMAT_S8_UINT, loc); }
FORMAT_D16_UNORM_S8_UINT { return yy::parser::make_FORMAT_D16_UNORM_S8_UINT(VK_FORMAT_D16_UNORM_S8_UINT, loc); }
FORMAT_D24_UNORM_S8_UINT { return yy::parser::make_FORMAT_D24_UNORM_S8_UINT(VK_FORMAT_D24_UNORM_S8_UINT, loc); }
FORMAT_D32_SFLOAT_S8_UINT { return yy::parser::make_FORMAT_D32_SFLOAT_S8_UINT(VK_FORMAT_D32_SFLOAT_S8_UINT, loc); }
FORMAT_BC1_RGB_UNORM_BLOCK { return yy::parser::make_FORMAT_BC1_RGB_UNORM_BLOCK(VK_FORMAT_BC1_RGB_UNORM_BLOCK, loc); }
FORMAT_BC1_RGB_SRGB_BLOCK { return yy::parser::make_FORMAT_BC1_RGB_SRGB_BLOCK(VK_FORMAT_BC1_RGB_SRGB_BLOCK, loc); }
FORMAT_BC1_RGBA_UNORM_BLOCK { return yy::parser::make_FORMAT_BC1_RGBA_UNORM_BLOCK(VK_FORMAT_BC1_RGBA_UNORM_BLOCK, loc); }
FORMAT_BC1_RGBA_SRGB_BLOCK { return yy::parser::make_FORMAT_BC1_RGBA_SRGB_BLOCK(VK_FORMAT_BC1_RGBA_SRGB_BLOCK, loc); }
FORMAT_BC2_UNORM_BLOCK { return yy::parser::make_FORMAT_BC2_UNORM_BLOCK(VK_FORMAT_BC2_UNORM_BLOCK, loc); }
FORMAT_BC2_SRGB_BLOCK { return yy::parser::make_FORMAT_BC2_SRGB_BLOCK(VK_FORMAT_BC2_SRGB_BLOCK, loc); }
FORMAT_BC3_UNORM_BLOCK { return yy::parser::make_FORMAT_BC3_UNORM_BLOCK(VK_FORMAT_BC3_UNORM_BLOCK, loc); }
FORMAT_BC3_SRGB_BLOCK { return yy::parser::make_FORMAT_BC3_SRGB_BLOCK(VK_FORMAT_BC3_SRGB_BLOCK, loc); }
FORMAT_BC4_UNORM_BLOCK { return yy::parser::make_FORMAT_BC4_UNORM_BLOCK(VK_FORMAT_BC4_UNORM_BLOCK, loc); }
FORMAT_BC4_SNORM_BLOCK { return yy::parser::make_FORMAT_BC4_SNORM_BLOCK(VK_FORMAT_BC4_SNORM_BLOCK, loc); }
FORMAT_BC5_UNORM_BLOCK { return yy::parser::make_FORMAT_BC5_UNORM_BLOCK(VK_FORMAT_BC5_UNORM_BLOCK, loc); }
FORMAT_BC5_SNORM_BLOCK { return yy::parser::make_FORMAT_BC5_SNORM_BLOCK(VK_FORMAT_BC5_SNORM_BLOCK, loc); }
FORMAT_BC6H_UFLOAT_BLOCK { return yy::parser::make_FORMAT_BC6H_UFLOAT_BLOCK(VK_FORMAT_BC6H_UFLOAT_BLOCK, loc); }
FORMAT_BC6H_SFLOAT_BLOCK { return yy::parser::make_FORMAT_BC6H_SFLOAT_BLOCK(VK_FORMAT_BC6H_SFLOAT_BLOCK, loc); }
FORMAT_BC7_UNORM_BLOCK { return yy::parser::make_FORMAT_BC7_UNORM_BLOCK(VK_FORMAT_BC7_UNORM_BLOCK, loc); }
FORMAT_BC7_SRGB_BLOCK { return yy::parser::make_FORMAT_BC7_SRGB_BLOCK(VK_FORMAT_BC7_SRGB_BLOCK, loc); }
FORMAT_ETC2_R8G8B8_UNORM_BLOCK { return yy::parser::make_FORMAT_ETC2_R8G8B8_UNORM_BLOCK(VK_FORMAT_ETC2_R8G8B8_UNORM_BLOCK, loc); }
FORMAT_ETC2_R8G8B8_SRGB_BLOCK { return yy::parser::make_FORMAT_ETC2_R8G8B8_SRGB_BLOCK(VK_FORMAT_ETC2_R8G8B8_SRGB_BLOCK, loc); }
FORMAT_ETC2_R8G8B8A1_UNORM_BLOCK { return yy::parser::make_FORMAT_ETC2_R8G8B8A1_UNORM_BLOCK(VK_FORMAT_ETC2_R8G8B8A1_UNORM_BLOCK, loc); }
FORMAT_ETC2_R8G8B8A1_SRGB_BLOCK { return yy::parser::make_FORMAT_ETC2_R8G8B8A1_SRGB_BLOCK(VK_FORMAT_ETC2_R8G8B8A1_SRGB_BLOCK, loc); }
FORMAT_ETC2_R8G8B8A8_UNORM_BLOCK { return yy::parser::make_FORMAT_ETC2_R8G8B8A8_UNORM_BLOCK(VK_FORMAT_ETC2_R8G8B8A8_UNORM_BLOCK, loc); }
FORMAT_ETC2_R8G8B8A8_SRGB_BLOCK { return yy::parser::make_FORMAT_ETC2_R8G8B8A8_SRGB_BLOCK(VK_FORMAT_ETC2_R8G8B8A8_SRGB_BLOCK, loc); }
FORMAT_EAC_R11_UNORM_BLOCK { return yy::parser::make_FORMAT_EAC_R11_UNORM_BLOCK(VK_FORMAT_EAC_R11_UNORM_BLOCK, loc); }
FORMAT_EAC_R11_SNORM_BLOCK { return yy::parser::make_FORMAT_EAC_R11_SNORM_BLOCK(VK_FORMAT_EAC_R11_SNORM_BLOCK, loc); }
FORMAT_EAC_R11G11_UNORM_BLOCK { return yy::parser::make_FORMAT_EAC_R11G11_UNORM_BLOCK(VK_FORMAT_EAC_R11G11_UNORM_BLOCK, loc); }
FORMAT_EAC_R11G11_SNORM_BLOCK { return yy::parser::make_FORMAT_EAC_R11G11_SNORM_BLOCK(VK_FORMAT_EAC_R11G11_SNORM_BLOCK, loc); }
FORMAT_ASTC_4x4_UNORM_BLOCK { return yy::parser::make_FORMAT_ASTC_4x4_UNORM_BLOCK(VK_FORMAT_ASTC_4x4_UNORM_BLOCK, loc); }
FORMAT_ASTC_4x4_SRGB_BLOCK { return yy::parser::make_FORMAT_ASTC_4x4_SRGB_BLOCK(VK_FORMAT_ASTC_4x4_SRGB_BLOCK, loc); }
FORMAT_ASTC_5x4_UNORM_BLOCK { return yy::parser::make_FORMAT_ASTC_5x4_UNORM_BLOCK(VK_FORMAT_ASTC_5x4_UNORM_BLOCK, loc); }
FORMAT_ASTC_5x4_SRGB_BLOCK { return yy::parser::make_FORMAT_ASTC_5x4_SRGB_BLOCK(VK_FORMAT_ASTC_5x4_SRGB_BLOCK, loc); }
FORMAT_ASTC_5x5_UNORM_BLOCK { return yy::parser::make_FORMAT_ASTC_5x5_UNORM_BLOCK(VK_FORMAT_ASTC_5x5_UNORM_BLOCK, loc); }
FORMAT_ASTC_5x5_SRGB_BLOCK { return yy::parser::make_FORMAT_ASTC_5x5_SRGB_BLOCK(VK_FORMAT_ASTC_5x5_SRGB_BLOCK, loc); }
FORMAT_ASTC_6x5_UNORM_BLOCK { return yy::parser::make_FORMAT_ASTC_6x5_UNORM_BLOCK(VK_FORMAT_ASTC_6x5_UNORM_BLOCK, loc); }
FORMAT_ASTC_6x5_SRGB_BLOCK { return yy::parser::make_FORMAT_ASTC_6x5_SRGB_BLOCK(VK_FORMAT_ASTC_6x5_SRGB_BLOCK, loc); }
FORMAT_ASTC_6x6_UNORM_BLOCK { return yy::parser::make_FORMAT_ASTC_6x6_UNORM_BLOCK(VK_FORMAT_ASTC_6x6_UNORM_BLOCK, loc); }
FORMAT_ASTC_6x6_SRGB_BLOCK { return yy::parser::make_FORMAT_ASTC_6x6_SRGB_BLOCK(VK_FORMAT_ASTC_6x6_SRGB_BLOCK, loc); }
FORMAT_ASTC_8x5_UNORM_BLOCK { return yy::parser::make_FORMAT_ASTC_8x5_UNORM_BLOCK(VK_FORMAT_ASTC_8x5_UNORM_BLOCK, loc); }
FORMAT_ASTC_8x5_SRGB_BLOCK { return yy::parser::make_FORMAT_ASTC_8x5_SRGB_BLOCK(VK_FORMAT_ASTC_8x5_SRGB_BLOCK, loc); }
FORMAT_ASTC_8x6_UNORM_BLOCK { return yy::parser::make_FORMAT_ASTC_8x6_UNORM_BLOCK(VK_FORMAT_ASTC_8x6_UNORM_BLOCK, loc); }
FORMAT_ASTC_8x6_SRGB_BLOCK { return yy::parser::make_FORMAT_ASTC_8x6_SRGB_BLOCK(VK_FORMAT_ASTC_8x6_SRGB_BLOCK, loc); }
FORMAT_ASTC_8x8_UNORM_BLOCK { return yy::parser::make_FORMAT_ASTC_8x8_UNORM_BLOCK(VK_FORMAT_ASTC_8x8_UNORM_BLOCK, loc); }
FORMAT_ASTC_8x8_SRGB_BLOCK { return yy::parser::make_FORMAT_ASTC_8x8_SRGB_BLOCK(VK_FORMAT_ASTC_8x8_SRGB_BLOCK, loc); }
FORMAT_ASTC_10x5_UNORM_BLOCK { return yy::parser::make_FORMAT_ASTC_10x5_UNORM_BLOCK(VK_FORMAT_ASTC_10x5_UNORM_BLOCK, loc); }
FORMAT_ASTC_10x5_SRGB_BLOCK { return yy::parser::make_FORMAT_ASTC_10x5_SRGB_BLOCK(VK_FORMAT_ASTC_10x5_SRGB_BLOCK, loc); }
FORMAT_ASTC_10x6_UNORM_BLOCK { return yy::parser::make_FORMAT_ASTC_10x6_UNORM_BLOCK(VK_FORMAT_ASTC_10x6_UNORM_BLOCK, loc); }
FORMAT_ASTC_10x6_SRGB_BLOCK { return yy::parser::make_FORMAT_ASTC_10x6_SRGB_BLOCK(VK_FORMAT_ASTC_10x6_SRGB_BLOCK, loc); }
FORMAT_ASTC_10x8_UNORM_BLOCK { return yy::parser::make_FORMAT_ASTC_10x8_UNORM_BLOCK(VK_FORMAT_ASTC_10x8_UNORM_BLOCK, loc); }
FORMAT_ASTC_10x8_SRGB_BLOCK { return yy::parser::make_FORMAT_ASTC_10x8_SRGB_BLOCK(VK_FORMAT_ASTC_10x8_SRGB_BLOCK, loc); }
FORMAT_ASTC_10x10_UNORM_BLOCK { return yy::parser::make_FORMAT_ASTC_10x10_UNORM_BLOCK(VK_FORMAT_ASTC_10x10_UNORM_BLOCK, loc); }
FORMAT_ASTC_10x10_SRGB_BLOCK { return yy::parser::make_FORMAT_ASTC_10x10_SRGB_BLOCK(VK_FORMAT_ASTC_10x10_SRGB_BLOCK, loc); }
FORMAT_ASTC_12x10_UNORM_BLOCK { return yy::parser::make_FORMAT_ASTC_12x10_UNORM_BLOCK(VK_FORMAT_ASTC_12x10_UNORM_BLOCK, loc); }
FORMAT_ASTC_12x10_SRGB_BLOCK { return yy::parser::make_FORMAT_ASTC_12x10_SRGB_BLOCK(VK_FORMAT_ASTC_12x10_SRGB_BLOCK, loc); }
FORMAT_ASTC_12x12_UNORM_BLOCK { return yy::parser::make_FORMAT_ASTC_12x12_UNORM_BLOCK(VK_FORMAT_ASTC_12x12_UNORM_BLOCK, loc); }
FORMAT_ASTC_12x12_SRGB_BLOCK { return yy::parser::make_FORMAT_ASTC_12x12_SRGB_BLOCK(VK_FORMAT_ASTC_12x12_SRGB_BLOCK, loc); }

GLSLShader {
    //comment_caller  =  INITIAL;
    bracket_level = 0;
    yy_push_state(shader);
	return yy::parser::make_GLSLSHADER(loc);
}

VertexFormat return yy::parser::make_VERTEXFORMAT(loc);

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

<INITIAL,cstbuffer,sampler_state,dst_state,pr_state,color_sample_state,rasterization_state,technique,pass,shader,resource,resource1,fbo,fbo1>\n+ {
    loc.lines (yyleng); loc.step ();
}
<INITIAL,cstbuffer,sampler_state,dst_state,pr_state,color_sample_state,rasterization_state,technique,pass,shader,resource,resource1,fbo,fbo1>{blank}+ {
    loc.step ();
}

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

{int}      return make_INT(yytext, loc);
{id}       return yy::parser::make_IDENTIFIER(yytext, loc);
.          {
                throw yy::parser::syntax_error(loc, "invalid character: " + std::string(yytext));
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
