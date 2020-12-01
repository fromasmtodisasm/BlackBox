// A Bison parser, made by GNU Bison 3.0.4.

// Skeleton interface for Bison LALR(1) parsers in C++

// Copyright (C) 2002-2015 Free Software Foundation, Inc.

// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

// As a special exception, you may create a larger work that contains
// part or all of the Bison parser skeleton and distribute that work
// under terms of your choice, so long as that work isn't itself a
// parser generator using the skeleton or a modified version thereof
// as a parser skeleton.  Alternatively, if you modify or redistribute
// the parser skeleton itself, you may (at your option) remove this
// special exception, which will cause the skeleton and the resulting
// Bison output files to be licensed under the GNU General Public
// License without this special exception.

// This special exception was added by the Free Software Foundation in
// version 2.2 of Bison.

/**
 ** \file Parser.hpp
 ** Define the yy::parser class.
 */

// C++ LALR(1) parser skeleton written by Akim Demaille.

#ifndef YY_YY_PARSER_HPP_INCLUDED
# define YY_YY_PARSER_HPP_INCLUDED
// //                    "%code requires" blocks.
#line 10 "Parser.yy" // lalr1.cc:377

    #include <string>
    #include <vulkan/vulkan.h>
    class Scanner;
    class Driver;

#line 51 "Parser.hpp" // lalr1.cc:377

# include <cassert>
# include <cstdlib> // std::abort
# include <iostream>
# include <stdexcept>
# include <string>
# include <vector>
# include "stack.hh"
# include "location.hh"
#include <typeinfo>
#ifndef YYASSERT
# include <cassert>
# define YYASSERT assert
#endif


#ifndef YY_ATTRIBUTE
# if (defined __GNUC__                                               \
      && (2 < __GNUC__ || (__GNUC__ == 2 && 96 <= __GNUC_MINOR__)))  \
     || defined __SUNPRO_C && 0x5110 <= __SUNPRO_C
#  define YY_ATTRIBUTE(Spec) __attribute__(Spec)
# else
#  define YY_ATTRIBUTE(Spec) /* empty */
# endif
#endif

#ifndef YY_ATTRIBUTE_PURE
# define YY_ATTRIBUTE_PURE   YY_ATTRIBUTE ((__pure__))
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# define YY_ATTRIBUTE_UNUSED YY_ATTRIBUTE ((__unused__))
#endif

#if !defined _Noreturn \
     && (!defined __STDC_VERSION__ || __STDC_VERSION__ < 201112)
# if defined _MSC_VER && 1200 <= _MSC_VER
#  define _Noreturn __declspec (noreturn)
# else
#  define _Noreturn YY_ATTRIBUTE ((__noreturn__))
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(E) ((void) (E))
#else
# define YYUSE(E) /* empty */
#endif

#if defined __GNUC__ && 407 <= __GNUC__ * 100 + __GNUC_MINOR__
/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN \
    _Pragma ("GCC diagnostic push") \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")\
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# define YY_IGNORE_MAYBE_UNINITIALIZED_END \
    _Pragma ("GCC diagnostic pop")
#else
# define YY_INITIAL_VALUE(Value) Value
#endif
#ifndef YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_VALUE
# define YY_INITIAL_VALUE(Value) /* Nothing. */
#endif

/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 1
#endif


namespace yy {
#line 128 "Parser.hpp" // lalr1.cc:377



  /// A char[S] buffer to store and retrieve objects.
  ///
  /// Sort of a variant, but does not keep track of the nature
  /// of the stored data, since that knowledge is available
  /// via the current state.
  template <size_t S>
  struct variant
  {
    /// Type of *this.
    typedef variant<S> self_type;

    /// Empty construction.
    variant ()
      : yytypeid_ (YY_NULLPTR)
    {}

    /// Construct and fill.
    template <typename T>
    variant (const T& t)
      : yytypeid_ (&typeid (T))
    {
      YYASSERT (sizeof (T) <= S);
      new (yyas_<T> ()) T (t);
    }

    /// Destruction, allowed only if empty.
    ~variant ()
    {
      YYASSERT (!yytypeid_);
    }

    /// Instantiate an empty \a T in here.
    template <typename T>
    T&
    build ()
    {
      YYASSERT (!yytypeid_);
      YYASSERT (sizeof (T) <= S);
      yytypeid_ = & typeid (T);
      return *new (yyas_<T> ()) T;
    }

    /// Instantiate a \a T in here from \a t.
    template <typename T>
    T&
    build (const T& t)
    {
      YYASSERT (!yytypeid_);
      YYASSERT (sizeof (T) <= S);
      yytypeid_ = & typeid (T);
      return *new (yyas_<T> ()) T (t);
    }

    /// Accessor to a built \a T.
    template <typename T>
    T&
    as ()
    {
      YYASSERT (*yytypeid_ == typeid (T));
      YYASSERT (sizeof (T) <= S);
      return *yyas_<T> ();
    }

    /// Const accessor to a built \a T (for %printer).
    template <typename T>
    const T&
    as () const
    {
      YYASSERT (*yytypeid_ == typeid (T));
      YYASSERT (sizeof (T) <= S);
      return *yyas_<T> ();
    }

    /// Swap the content with \a other, of same type.
    ///
    /// Both variants must be built beforehand, because swapping the actual
    /// data requires reading it (with as()), and this is not possible on
    /// unconstructed variants: it would require some dynamic testing, which
    /// should not be the variant's responsability.
    /// Swapping between built and (possibly) non-built is done with
    /// variant::move ().
    template <typename T>
    void
    swap (self_type& other)
    {
      YYASSERT (yytypeid_);
      YYASSERT (*yytypeid_ == *other.yytypeid_);
      std::swap (as<T> (), other.as<T> ());
    }

    /// Move the content of \a other to this.
    ///
    /// Destroys \a other.
    template <typename T>
    void
    move (self_type& other)
    {
      build<T> ();
      swap<T> (other);
      other.destroy<T> ();
    }

    /// Copy the content of \a other to this.
    template <typename T>
    void
    copy (const self_type& other)
    {
      build<T> (other.as<T> ());
    }

    /// Destroy the stored \a T.
    template <typename T>
    void
    destroy ()
    {
      as<T> ().~T ();
      yytypeid_ = YY_NULLPTR;
    }

  private:
    /// Prohibit blind copies.
    self_type& operator=(const self_type&);
    variant (const self_type&);

    /// Accessor to raw memory as \a T.
    template <typename T>
    T*
    yyas_ ()
    {
      void *yyp = yybuffer_.yyraw;
      return static_cast<T*> (yyp);
     }

    /// Const accessor to raw memory as \a T.
    template <typename T>
    const T*
    yyas_ () const
    {
      const void *yyp = yybuffer_.yyraw;
      return static_cast<const T*> (yyp);
     }

    union
    {
      /// Strongest alignment constraints.
      long double yyalign_me;
      /// A buffer large enough to store any of the semantic values.
      char yyraw[S];
    } yybuffer_;

    /// Whether the content is built: if defined, the name of the stored type.
    const std::type_info *yytypeid_;
  };


  /// A Bison parser.
  class parser
  {
  public:
#ifndef YYSTYPE
    /// An auxiliary type to compute the largest semantic type.
    union union_type
    {
      // FORMAT_UNDEFINED
      // FORMAT_R4G4_UNORM_PACK8
      // FORMAT_R4G4B4A4_UNORM_PACK16
      // FORMAT_B4G4R4A4_UNORM_PACK16
      // FORMAT_R5G6B5_UNORM_PACK16
      // FORMAT_B5G6R5_UNORM_PACK16
      // FORMAT_R5G5B5A1_UNORM_PACK16
      // FORMAT_B5G5R5A1_UNORM_PACK16
      // FORMAT_A1R5G5B5_UNORM_PACK16
      // FORMAT_R8_UNORM
      // FORMAT_R8_SNORM
      // FORMAT_R8_USCALED
      // FORMAT_R8_SSCALED
      // FORMAT_R8_UINT
      // FORMAT_R8_SINT
      // FORMAT_R8_SRGB
      // FORMAT_R8G8_UNORM
      // FORMAT_R8G8_SNORM
      // FORMAT_R8G8_USCALED
      // FORMAT_R8G8_SSCALED
      // FORMAT_R8G8_UINT
      // FORMAT_R8G8_SINT
      // FORMAT_R8G8_SRGB
      // FORMAT_R8G8B8_UNORM
      // FORMAT_R8G8B8_SNORM
      // FORMAT_R8G8B8_USCALED
      // FORMAT_R8G8B8_SSCALED
      // FORMAT_R8G8B8_UINT
      // FORMAT_R8G8B8_SINT
      // FORMAT_R8G8B8_SRGB
      // FORMAT_B8G8R8_UNORM
      // FORMAT_B8G8R8_SNORM
      // FORMAT_B8G8R8_USCALED
      // FORMAT_B8G8R8_SSCALED
      // FORMAT_B8G8R8_UINT
      // FORMAT_B8G8R8_SINT
      // FORMAT_B8G8R8_SRGB
      // FORMAT_R8G8B8A8_UNORM
      // FORMAT_R8G8B8A8_SNORM
      // FORMAT_R8G8B8A8_USCALED
      // FORMAT_R8G8B8A8_SSCALED
      // FORMAT_R8G8B8A8_UINT
      // FORMAT_R8G8B8A8_SINT
      // FORMAT_R8G8B8A8_SRGB
      // FORMAT_B8G8R8A8_UNORM
      // FORMAT_B8G8R8A8_SNORM
      // FORMAT_B8G8R8A8_USCALED
      // FORMAT_B8G8R8A8_SSCALED
      // FORMAT_B8G8R8A8_UINT
      // FORMAT_B8G8R8A8_SINT
      // FORMAT_B8G8R8A8_SRGB
      // FORMAT_A8B8G8R8_UNORM_PACK32
      // FORMAT_A8B8G8R8_SNORM_PACK32
      // FORMAT_A8B8G8R8_USCALED_PACK32
      // FORMAT_A8B8G8R8_SSCALED_PACK32
      // FORMAT_A8B8G8R8_UINT_PACK32
      // FORMAT_A8B8G8R8_SINT_PACK32
      // FORMAT_A8B8G8R8_SRGB_PACK32
      // FORMAT_A2R10G10B10_UNORM_PACK32
      // FORMAT_A2R10G10B10_SNORM_PACK32
      // FORMAT_A2R10G10B10_USCALED_PACK32
      // FORMAT_A2R10G10B10_SSCALED_PACK32
      // FORMAT_A2R10G10B10_UINT_PACK32
      // FORMAT_A2R10G10B10_SINT_PACK32
      // FORMAT_A2B10G10R10_UNORM_PACK32
      // FORMAT_A2B10G10R10_SNORM_PACK32
      // FORMAT_A2B10G10R10_USCALED_PACK32
      // FORMAT_A2B10G10R10_SSCALED_PACK32
      // FORMAT_A2B10G10R10_UINT_PACK32
      // FORMAT_A2B10G10R10_SINT_PACK32
      // FORMAT_R16_UNORM
      // FORMAT_R16_SNORM
      // FORMAT_R16_USCALED
      // FORMAT_R16_SSCALED
      // FORMAT_R16_UINT
      // FORMAT_R16_SINT
      // FORMAT_R16_SFLOAT
      // FORMAT_R16G16_UNORM
      // FORMAT_R16G16_SNORM
      // FORMAT_R16G16_USCALED
      // FORMAT_R16G16_SSCALED
      // FORMAT_R16G16_UINT
      // FORMAT_R16G16_SINT
      // FORMAT_R16G16_SFLOAT
      // FORMAT_R16G16B16_UNORM
      // FORMAT_R16G16B16_SNORM
      // FORMAT_R16G16B16_USCALED
      // FORMAT_R16G16B16_SSCALED
      // FORMAT_R16G16B16_UINT
      // FORMAT_R16G16B16_SINT
      // FORMAT_R16G16B16_SFLOAT
      // FORMAT_R16G16B16A16_UNORM
      // FORMAT_R16G16B16A16_SNORM
      // FORMAT_R16G16B16A16_USCALED
      // FORMAT_R16G16B16A16_SSCALED
      // FORMAT_R16G16B16A16_UINT
      // FORMAT_R16G16B16A16_SINT
      // FORMAT_R16G16B16A16_SFLOAT
      // FORMAT_R32_UINT
      // FORMAT_R32_SINT
      // FORMAT_R32_SFLOAT
      // FORMAT_R32G32_UINT
      // FORMAT_R32G32_SINT
      // FORMAT_R32G32_SFLOAT
      // FORMAT_R32G32B32_UINT
      // FORMAT_R32G32B32_SINT
      // FORMAT_R32G32B32_SFLOAT
      // FORMAT_R32G32B32A32_UINT
      // FORMAT_R32G32B32A32_SINT
      // FORMAT_R32G32B32A32_SFLOAT
      // FORMAT_R64_UINT
      // FORMAT_R64_SINT
      // FORMAT_R64_SFLOAT
      // FORMAT_R64G64_UINT
      // FORMAT_R64G64_SINT
      // FORMAT_R64G64_SFLOAT
      // FORMAT_R64G64B64_UINT
      // FORMAT_R64G64B64_SINT
      // FORMAT_R64G64B64_SFLOAT
      // FORMAT_R64G64B64A64_UINT
      // FORMAT_R64G64B64A64_SINT
      // FORMAT_R64G64B64A64_SFLOAT
      // FORMAT_B10G11R11_UFLOAT_PACK32
      // FORMAT_E5B9G9R9_UFLOAT_PACK32
      // FORMAT_D16_UNORM
      // FORMAT_X8_D24_UNORM_PACK32
      // FORMAT_D32_SFLOAT
      // FORMAT_S8_UINT
      // FORMAT_D16_UNORM_S8_UINT
      // FORMAT_D24_UNORM_S8_UINT
      // FORMAT_D32_SFLOAT_S8_UINT
      // FORMAT_BC1_RGB_UNORM_BLOCK
      // FORMAT_BC1_RGB_SRGB_BLOCK
      // FORMAT_BC1_RGBA_UNORM_BLOCK
      // FORMAT_BC1_RGBA_SRGB_BLOCK
      // FORMAT_BC2_UNORM_BLOCK
      // FORMAT_BC2_SRGB_BLOCK
      // FORMAT_BC3_UNORM_BLOCK
      // FORMAT_BC3_SRGB_BLOCK
      // FORMAT_BC4_UNORM_BLOCK
      // FORMAT_BC4_SNORM_BLOCK
      // FORMAT_BC5_UNORM_BLOCK
      // FORMAT_BC5_SNORM_BLOCK
      // FORMAT_BC6H_UFLOAT_BLOCK
      // FORMAT_BC6H_SFLOAT_BLOCK
      // FORMAT_BC7_UNORM_BLOCK
      // FORMAT_BC7_SRGB_BLOCK
      // FORMAT_ETC2_R8G8B8_UNORM_BLOCK
      // FORMAT_ETC2_R8G8B8_SRGB_BLOCK
      // FORMAT_ETC2_R8G8B8A1_UNORM_BLOCK
      // FORMAT_ETC2_R8G8B8A1_SRGB_BLOCK
      // FORMAT_ETC2_R8G8B8A8_UNORM_BLOCK
      // FORMAT_ETC2_R8G8B8A8_SRGB_BLOCK
      // FORMAT_EAC_R11_UNORM_BLOCK
      // FORMAT_EAC_R11_SNORM_BLOCK
      // FORMAT_EAC_R11G11_UNORM_BLOCK
      // FORMAT_EAC_R11G11_SNORM_BLOCK
      // FORMAT_ASTC_4x4_UNORM_BLOCK
      // FORMAT_ASTC_4x4_SRGB_BLOCK
      // FORMAT_ASTC_5x4_UNORM_BLOCK
      // FORMAT_ASTC_5x4_SRGB_BLOCK
      // FORMAT_ASTC_5x5_UNORM_BLOCK
      // FORMAT_ASTC_5x5_SRGB_BLOCK
      // FORMAT_ASTC_6x5_UNORM_BLOCK
      // FORMAT_ASTC_6x5_SRGB_BLOCK
      // FORMAT_ASTC_6x6_UNORM_BLOCK
      // FORMAT_ASTC_6x6_SRGB_BLOCK
      // FORMAT_ASTC_8x5_UNORM_BLOCK
      // FORMAT_ASTC_8x5_SRGB_BLOCK
      // FORMAT_ASTC_8x6_UNORM_BLOCK
      // FORMAT_ASTC_8x6_SRGB_BLOCK
      // FORMAT_ASTC_8x8_UNORM_BLOCK
      // FORMAT_ASTC_8x8_SRGB_BLOCK
      // FORMAT_ASTC_10x5_UNORM_BLOCK
      // FORMAT_ASTC_10x5_SRGB_BLOCK
      // FORMAT_ASTC_10x6_UNORM_BLOCK
      // FORMAT_ASTC_10x6_SRGB_BLOCK
      // FORMAT_ASTC_10x8_UNORM_BLOCK
      // FORMAT_ASTC_10x8_SRGB_BLOCK
      // FORMAT_ASTC_10x10_UNORM_BLOCK
      // FORMAT_ASTC_10x10_SRGB_BLOCK
      // FORMAT_ASTC_12x10_UNORM_BLOCK
      // FORMAT_ASTC_12x10_SRGB_BLOCK
      // FORMAT_ASTC_12x12_UNORM_BLOCK
      // FORMAT_ASTC_12x12_SRGB_BLOCK
      // format
      char dummy1[sizeof(VkFormat)];

      // TRUE
      // FALSE
      // BOOL
      char dummy2[sizeof(bool)];

      // FLOAT
      char dummy3[sizeof(float)];

      // INT
      char dummy4[sizeof(int)];

      // IDENTIFIER
      // STR
      // CODEBODY
      // VARNAME
      char dummy5[sizeof(std::string)];
};

    /// Symbol semantic values.
    typedef variant<sizeof(union_type)> semantic_type;
#else
    typedef YYSTYPE semantic_type;
#endif
    /// Symbol locations.
    typedef location location_type;

    /// Syntax errors thrown from user actions.
    struct syntax_error : std::runtime_error
    {
      syntax_error (const location_type& l, const std::string& m);
      location_type location;
    };

    /// Tokens.
    struct token
    {
      enum yytokentype
      {
        TOK_END = 0,
        TOK_ASSIGN = 258,
        TOK_MINUS = 259,
        TOK_PLUS = 260,
        TOK_STAR = 261,
        TOK_SLASH = 262,
        TOK_LPAREN = 263,
        TOK_RPAREN = 264,
        TOK_LEFTSCOPE = 265,
        TOK_RIGHTSCOPE = 266,
        TOK_SEMICOLON = 267,
        TOK_COMMA = 268,
        TOK_IDENTIFIER = 269,
        TOK_TRUE = 270,
        TOK_FALSE = 271,
        TOK_FLOAT = 272,
        TOK_INT = 273,
        TOK_BOOL = 274,
        TOK_STR = 275,
        TOK_GLSLSHADER = 276,
        TOK_HLSL10SHADER = 277,
        TOK_HLSL11SHADER = 278,
        TOK_CGSHADER = 279,
        TOK_SAMPLER_STATE = 280,
        TOK_DST_STATE = 281,
        TOK_PR_STATE = 282,
        TOK_COLOR_SAMPLE_STATE = 283,
        TOK_RASTERIZATION_STATE = 284,
        TOK_TECHNIQUE = 285,
        TOK_PASS = 286,
        TOK_CODEBODY = 287,
        TOK_VARNAME = 288,
        TOK_TEXTURERESOURCE = 289,
        TOK_TEXTURERESOURCE1D = 290,
        TOK_TEXTURERESOURCE2D = 291,
        TOK_TEXTURERESOURCE3D = 292,
        TOK_TEXTURERESOURCERECT = 293,
        TOK_TEXTURERESOURCECUBE = 294,
        TOK_VOID_TYPE = 295,
        TOK_UNSIGNED = 296,
        TOK_HIGHP = 297,
        TOK_MEDIUMP = 298,
        TOK_LOWP = 299,
        TOK_UNIFORM = 300,
        TOK_CSTBUFFER = 301,
        TOK_FLOAT_TYPE = 302,
        TOK_FLOAT2_TYPE = 303,
        TOK_FLOAT3_TYPE = 304,
        TOK_FLOAT4_TYPE = 305,
        TOK_MAT2_TYPE = 306,
        TOK_MAT3_TYPE = 307,
        TOK_MAT4_TYPE = 308,
        TOK_BOOL_TYPE = 309,
        TOK_BOOL2_TYPE = 310,
        TOK_BOOL3_TYPE = 311,
        TOK_BOOL4_TYPE = 312,
        TOK_INT_TYPE = 313,
        TOK_INT2_TYPE = 314,
        TOK_INT3_TYPE = 315,
        TOK_INT4_TYPE = 316,
        TOK_TEXTURE1D_TYPE = 317,
        TOK_TEXTURE2D_TYPE = 318,
        TOK_TEXTURE2DSHADOW_TYPE = 319,
        TOK_TEXTURE2DRECT_TYPE = 320,
        TOK_TEXTURE3D_TYPE = 321,
        TOK_TEXTURECUBE_TYPE = 322,
        TOK_SAMPLER1D_TYPE = 323,
        TOK_SAMPLER2D_TYPE = 324,
        TOK_SAMPLER2DSHADOW_TYPE = 325,
        TOK_SAMPLER2DRECT_TYPE = 326,
        TOK_SAMPLER3D_TYPE = 327,
        TOK_SAMPLERCUBE_TYPE = 328,
        TOK_EXTENSION = 329,
        TOK_SEPARATE_SHADER = 330,
        TOK_VERTEXPROGRAM = 331,
        TOK_FRAGMENTPROGRAM = 332,
        TOK_GEOMETRYPROGRAM = 333,
        TOK_HULLPROGRAM = 334,
        TOK_EVALPROGRAM = 335,
        TOK_SHDPROFILE = 336,
        TOK_SAMPLERRESOURCE = 337,
        TOK_SAMPLERTEXUNIT = 338,
        TOK_SETSAMPLERSTATE = 339,
        TOK_SETDSTSTATE = 340,
        TOK_SETRASTERIZATIONSTATE = 341,
        TOK_SETCOLORSAMPLESTATE = 342,
        TOK_IMAGERESOURCE = 343,
        TOK_IMAGEUNIT = 344,
        TOK_IMAGEACCESS = 345,
        TOK_IMAGELAYER = 346,
        TOK_IMAGELAYERED = 347,
        TOK_WRITE_ONLY = 348,
        TOK_READ_ONLY = 349,
        TOK_READ_WRITE = 350,
        TOK_FORMAT_UNDEFINED = 351,
        TOK_FORMAT_R4G4_UNORM_PACK8 = 352,
        TOK_FORMAT_R4G4B4A4_UNORM_PACK16 = 353,
        TOK_FORMAT_B4G4R4A4_UNORM_PACK16 = 354,
        TOK_FORMAT_R5G6B5_UNORM_PACK16 = 355,
        TOK_FORMAT_B5G6R5_UNORM_PACK16 = 356,
        TOK_FORMAT_R5G5B5A1_UNORM_PACK16 = 357,
        TOK_FORMAT_B5G5R5A1_UNORM_PACK16 = 358,
        TOK_FORMAT_A1R5G5B5_UNORM_PACK16 = 359,
        TOK_FORMAT_R8_UNORM = 360,
        TOK_FORMAT_R8_SNORM = 361,
        TOK_FORMAT_R8_USCALED = 362,
        TOK_FORMAT_R8_SSCALED = 363,
        TOK_FORMAT_R8_UINT = 364,
        TOK_FORMAT_R8_SINT = 365,
        TOK_FORMAT_R8_SRGB = 366,
        TOK_FORMAT_R8G8_UNORM = 367,
        TOK_FORMAT_R8G8_SNORM = 368,
        TOK_FORMAT_R8G8_USCALED = 369,
        TOK_FORMAT_R8G8_SSCALED = 370,
        TOK_FORMAT_R8G8_UINT = 371,
        TOK_FORMAT_R8G8_SINT = 372,
        TOK_FORMAT_R8G8_SRGB = 373,
        TOK_FORMAT_R8G8B8_UNORM = 374,
        TOK_FORMAT_R8G8B8_SNORM = 375,
        TOK_FORMAT_R8G8B8_USCALED = 376,
        TOK_FORMAT_R8G8B8_SSCALED = 377,
        TOK_FORMAT_R8G8B8_UINT = 378,
        TOK_FORMAT_R8G8B8_SINT = 379,
        TOK_FORMAT_R8G8B8_SRGB = 380,
        TOK_FORMAT_B8G8R8_UNORM = 381,
        TOK_FORMAT_B8G8R8_SNORM = 382,
        TOK_FORMAT_B8G8R8_USCALED = 383,
        TOK_FORMAT_B8G8R8_SSCALED = 384,
        TOK_FORMAT_B8G8R8_UINT = 385,
        TOK_FORMAT_B8G8R8_SINT = 386,
        TOK_FORMAT_B8G8R8_SRGB = 387,
        TOK_FORMAT_R8G8B8A8_UNORM = 388,
        TOK_FORMAT_R8G8B8A8_SNORM = 389,
        TOK_FORMAT_R8G8B8A8_USCALED = 390,
        TOK_FORMAT_R8G8B8A8_SSCALED = 391,
        TOK_FORMAT_R8G8B8A8_UINT = 392,
        TOK_FORMAT_R8G8B8A8_SINT = 393,
        TOK_FORMAT_R8G8B8A8_SRGB = 394,
        TOK_FORMAT_B8G8R8A8_UNORM = 395,
        TOK_FORMAT_B8G8R8A8_SNORM = 396,
        TOK_FORMAT_B8G8R8A8_USCALED = 397,
        TOK_FORMAT_B8G8R8A8_SSCALED = 398,
        TOK_FORMAT_B8G8R8A8_UINT = 399,
        TOK_FORMAT_B8G8R8A8_SINT = 400,
        TOK_FORMAT_B8G8R8A8_SRGB = 401,
        TOK_FORMAT_A8B8G8R8_UNORM_PACK32 = 402,
        TOK_FORMAT_A8B8G8R8_SNORM_PACK32 = 403,
        TOK_FORMAT_A8B8G8R8_USCALED_PACK32 = 404,
        TOK_FORMAT_A8B8G8R8_SSCALED_PACK32 = 405,
        TOK_FORMAT_A8B8G8R8_UINT_PACK32 = 406,
        TOK_FORMAT_A8B8G8R8_SINT_PACK32 = 407,
        TOK_FORMAT_A8B8G8R8_SRGB_PACK32 = 408,
        TOK_FORMAT_A2R10G10B10_UNORM_PACK32 = 409,
        TOK_FORMAT_A2R10G10B10_SNORM_PACK32 = 410,
        TOK_FORMAT_A2R10G10B10_USCALED_PACK32 = 411,
        TOK_FORMAT_A2R10G10B10_SSCALED_PACK32 = 412,
        TOK_FORMAT_A2R10G10B10_UINT_PACK32 = 413,
        TOK_FORMAT_A2R10G10B10_SINT_PACK32 = 414,
        TOK_FORMAT_A2B10G10R10_UNORM_PACK32 = 415,
        TOK_FORMAT_A2B10G10R10_SNORM_PACK32 = 416,
        TOK_FORMAT_A2B10G10R10_USCALED_PACK32 = 417,
        TOK_FORMAT_A2B10G10R10_SSCALED_PACK32 = 418,
        TOK_FORMAT_A2B10G10R10_UINT_PACK32 = 419,
        TOK_FORMAT_A2B10G10R10_SINT_PACK32 = 420,
        TOK_FORMAT_R16_UNORM = 421,
        TOK_FORMAT_R16_SNORM = 422,
        TOK_FORMAT_R16_USCALED = 423,
        TOK_FORMAT_R16_SSCALED = 424,
        TOK_FORMAT_R16_UINT = 425,
        TOK_FORMAT_R16_SINT = 426,
        TOK_FORMAT_R16_SFLOAT = 427,
        TOK_FORMAT_R16G16_UNORM = 428,
        TOK_FORMAT_R16G16_SNORM = 429,
        TOK_FORMAT_R16G16_USCALED = 430,
        TOK_FORMAT_R16G16_SSCALED = 431,
        TOK_FORMAT_R16G16_UINT = 432,
        TOK_FORMAT_R16G16_SINT = 433,
        TOK_FORMAT_R16G16_SFLOAT = 434,
        TOK_FORMAT_R16G16B16_UNORM = 435,
        TOK_FORMAT_R16G16B16_SNORM = 436,
        TOK_FORMAT_R16G16B16_USCALED = 437,
        TOK_FORMAT_R16G16B16_SSCALED = 438,
        TOK_FORMAT_R16G16B16_UINT = 439,
        TOK_FORMAT_R16G16B16_SINT = 440,
        TOK_FORMAT_R16G16B16_SFLOAT = 441,
        TOK_FORMAT_R16G16B16A16_UNORM = 442,
        TOK_FORMAT_R16G16B16A16_SNORM = 443,
        TOK_FORMAT_R16G16B16A16_USCALED = 444,
        TOK_FORMAT_R16G16B16A16_SSCALED = 445,
        TOK_FORMAT_R16G16B16A16_UINT = 446,
        TOK_FORMAT_R16G16B16A16_SINT = 447,
        TOK_FORMAT_R16G16B16A16_SFLOAT = 448,
        TOK_FORMAT_R32_UINT = 449,
        TOK_FORMAT_R32_SINT = 450,
        TOK_FORMAT_R32_SFLOAT = 451,
        TOK_FORMAT_R32G32_UINT = 452,
        TOK_FORMAT_R32G32_SINT = 453,
        TOK_FORMAT_R32G32_SFLOAT = 454,
        TOK_FORMAT_R32G32B32_UINT = 455,
        TOK_FORMAT_R32G32B32_SINT = 456,
        TOK_FORMAT_R32G32B32_SFLOAT = 457,
        TOK_FORMAT_R32G32B32A32_UINT = 458,
        TOK_FORMAT_R32G32B32A32_SINT = 459,
        TOK_FORMAT_R32G32B32A32_SFLOAT = 460,
        TOK_FORMAT_R64_UINT = 461,
        TOK_FORMAT_R64_SINT = 462,
        TOK_FORMAT_R64_SFLOAT = 463,
        TOK_FORMAT_R64G64_UINT = 464,
        TOK_FORMAT_R64G64_SINT = 465,
        TOK_FORMAT_R64G64_SFLOAT = 466,
        TOK_FORMAT_R64G64B64_UINT = 467,
        TOK_FORMAT_R64G64B64_SINT = 468,
        TOK_FORMAT_R64G64B64_SFLOAT = 469,
        TOK_FORMAT_R64G64B64A64_UINT = 470,
        TOK_FORMAT_R64G64B64A64_SINT = 471,
        TOK_FORMAT_R64G64B64A64_SFLOAT = 472,
        TOK_FORMAT_B10G11R11_UFLOAT_PACK32 = 473,
        TOK_FORMAT_E5B9G9R9_UFLOAT_PACK32 = 474,
        TOK_FORMAT_D16_UNORM = 475,
        TOK_FORMAT_X8_D24_UNORM_PACK32 = 476,
        TOK_FORMAT_D32_SFLOAT = 477,
        TOK_FORMAT_S8_UINT = 478,
        TOK_FORMAT_D16_UNORM_S8_UINT = 479,
        TOK_FORMAT_D24_UNORM_S8_UINT = 480,
        TOK_FORMAT_D32_SFLOAT_S8_UINT = 481,
        TOK_FORMAT_BC1_RGB_UNORM_BLOCK = 482,
        TOK_FORMAT_BC1_RGB_SRGB_BLOCK = 483,
        TOK_FORMAT_BC1_RGBA_UNORM_BLOCK = 484,
        TOK_FORMAT_BC1_RGBA_SRGB_BLOCK = 485,
        TOK_FORMAT_BC2_UNORM_BLOCK = 486,
        TOK_FORMAT_BC2_SRGB_BLOCK = 487,
        TOK_FORMAT_BC3_UNORM_BLOCK = 488,
        TOK_FORMAT_BC3_SRGB_BLOCK = 489,
        TOK_FORMAT_BC4_UNORM_BLOCK = 490,
        TOK_FORMAT_BC4_SNORM_BLOCK = 491,
        TOK_FORMAT_BC5_UNORM_BLOCK = 492,
        TOK_FORMAT_BC5_SNORM_BLOCK = 493,
        TOK_FORMAT_BC6H_UFLOAT_BLOCK = 494,
        TOK_FORMAT_BC6H_SFLOAT_BLOCK = 495,
        TOK_FORMAT_BC7_UNORM_BLOCK = 496,
        TOK_FORMAT_BC7_SRGB_BLOCK = 497,
        TOK_FORMAT_ETC2_R8G8B8_UNORM_BLOCK = 498,
        TOK_FORMAT_ETC2_R8G8B8_SRGB_BLOCK = 499,
        TOK_FORMAT_ETC2_R8G8B8A1_UNORM_BLOCK = 500,
        TOK_FORMAT_ETC2_R8G8B8A1_SRGB_BLOCK = 501,
        TOK_FORMAT_ETC2_R8G8B8A8_UNORM_BLOCK = 502,
        TOK_FORMAT_ETC2_R8G8B8A8_SRGB_BLOCK = 503,
        TOK_FORMAT_EAC_R11_UNORM_BLOCK = 504,
        TOK_FORMAT_EAC_R11_SNORM_BLOCK = 505,
        TOK_FORMAT_EAC_R11G11_UNORM_BLOCK = 506,
        TOK_FORMAT_EAC_R11G11_SNORM_BLOCK = 507,
        TOK_FORMAT_ASTC_4x4_UNORM_BLOCK = 508,
        TOK_FORMAT_ASTC_4x4_SRGB_BLOCK = 509,
        TOK_FORMAT_ASTC_5x4_UNORM_BLOCK = 510,
        TOK_FORMAT_ASTC_5x4_SRGB_BLOCK = 511,
        TOK_FORMAT_ASTC_5x5_UNORM_BLOCK = 512,
        TOK_FORMAT_ASTC_5x5_SRGB_BLOCK = 513,
        TOK_FORMAT_ASTC_6x5_UNORM_BLOCK = 514,
        TOK_FORMAT_ASTC_6x5_SRGB_BLOCK = 515,
        TOK_FORMAT_ASTC_6x6_UNORM_BLOCK = 516,
        TOK_FORMAT_ASTC_6x6_SRGB_BLOCK = 517,
        TOK_FORMAT_ASTC_8x5_UNORM_BLOCK = 518,
        TOK_FORMAT_ASTC_8x5_SRGB_BLOCK = 519,
        TOK_FORMAT_ASTC_8x6_UNORM_BLOCK = 520,
        TOK_FORMAT_ASTC_8x6_SRGB_BLOCK = 521,
        TOK_FORMAT_ASTC_8x8_UNORM_BLOCK = 522,
        TOK_FORMAT_ASTC_8x8_SRGB_BLOCK = 523,
        TOK_FORMAT_ASTC_10x5_UNORM_BLOCK = 524,
        TOK_FORMAT_ASTC_10x5_SRGB_BLOCK = 525,
        TOK_FORMAT_ASTC_10x6_UNORM_BLOCK = 526,
        TOK_FORMAT_ASTC_10x6_SRGB_BLOCK = 527,
        TOK_FORMAT_ASTC_10x8_UNORM_BLOCK = 528,
        TOK_FORMAT_ASTC_10x8_SRGB_BLOCK = 529,
        TOK_FORMAT_ASTC_10x10_UNORM_BLOCK = 530,
        TOK_FORMAT_ASTC_10x10_SRGB_BLOCK = 531,
        TOK_FORMAT_ASTC_12x10_UNORM_BLOCK = 532,
        TOK_FORMAT_ASTC_12x10_SRGB_BLOCK = 533,
        TOK_FORMAT_ASTC_12x12_UNORM_BLOCK = 534,
        TOK_FORMAT_ASTC_12x12_SRGB_BLOCK = 535,
        TOK_VERTEXFORMAT = 536
      };
    };

    /// (External) token type, as returned by yylex.
    typedef token::yytokentype token_type;

    /// Symbol type: an internal symbol number.
    typedef int symbol_number_type;

    /// The symbol type number to denote an empty symbol.
    enum { empty_symbol = -2 };

    /// Internal symbol number for tokens (subsumed by symbol_number_type).
    typedef unsigned short int token_number_type;

    /// A complete symbol.
    ///
    /// Expects its Base type to provide access to the symbol type
    /// via type_get().
    ///
    /// Provide access to semantic value and location.
    template <typename Base>
    struct basic_symbol : Base
    {
      /// Alias to Base.
      typedef Base super_type;

      /// Default constructor.
      basic_symbol ();

      /// Copy constructor.
      basic_symbol (const basic_symbol& other);

      /// Constructor for valueless symbols, and symbols from each type.

  basic_symbol (typename Base::kind_type t, const location_type& l);

  basic_symbol (typename Base::kind_type t, const VkFormat v, const location_type& l);

  basic_symbol (typename Base::kind_type t, const bool v, const location_type& l);

  basic_symbol (typename Base::kind_type t, const float v, const location_type& l);

  basic_symbol (typename Base::kind_type t, const int v, const location_type& l);

  basic_symbol (typename Base::kind_type t, const std::string v, const location_type& l);


      /// Constructor for symbols with semantic value.
      basic_symbol (typename Base::kind_type t,
                    const semantic_type& v,
                    const location_type& l);

      /// Destroy the symbol.
      ~basic_symbol ();

      /// Destroy contents, and record that is empty.
      void clear ();

      /// Whether empty.
      bool empty () const;

      /// Destructive move, \a s is emptied into this.
      void move (basic_symbol& s);

      /// The semantic value.
      semantic_type value;

      /// The location.
      location_type location;

    private:
      /// Assignment operator.
      basic_symbol& operator= (const basic_symbol& other);
    };

    /// Type access provider for token (enum) based symbols.
    struct by_type
    {
      /// Default constructor.
      by_type ();

      /// Copy constructor.
      by_type (const by_type& other);

      /// The symbol type as needed by the constructor.
      typedef token_type kind_type;

      /// Constructor from (external) token numbers.
      by_type (kind_type t);

      /// Record that this symbol is empty.
      void clear ();

      /// Steal the symbol type from \a that.
      void move (by_type& that);

      /// The (internal) type number (corresponding to \a type).
      /// \a empty when empty.
      symbol_number_type type_get () const;

      /// The token.
      token_type token () const;

      /// The symbol type.
      /// \a empty_symbol when empty.
      /// An int, not token_number_type, to be able to store empty_symbol.
      int type;
    };

    /// "External" symbols: returned by the scanner.
    typedef basic_symbol<by_type> symbol_type;

    // Symbol constructors declarations.
    static inline
    symbol_type
    make_END (const location_type& l);

    static inline
    symbol_type
    make_ASSIGN (const location_type& l);

    static inline
    symbol_type
    make_MINUS (const location_type& l);

    static inline
    symbol_type
    make_PLUS (const location_type& l);

    static inline
    symbol_type
    make_STAR (const location_type& l);

    static inline
    symbol_type
    make_SLASH (const location_type& l);

    static inline
    symbol_type
    make_LPAREN (const location_type& l);

    static inline
    symbol_type
    make_RPAREN (const location_type& l);

    static inline
    symbol_type
    make_LEFTSCOPE (const location_type& l);

    static inline
    symbol_type
    make_RIGHTSCOPE (const location_type& l);

    static inline
    symbol_type
    make_SEMICOLON (const location_type& l);

    static inline
    symbol_type
    make_COMMA (const location_type& l);

    static inline
    symbol_type
    make_IDENTIFIER (const std::string& v, const location_type& l);

    static inline
    symbol_type
    make_TRUE (const bool& v, const location_type& l);

    static inline
    symbol_type
    make_FALSE (const bool& v, const location_type& l);

    static inline
    symbol_type
    make_FLOAT (const float& v, const location_type& l);

    static inline
    symbol_type
    make_INT (const int& v, const location_type& l);

    static inline
    symbol_type
    make_BOOL (const bool& v, const location_type& l);

    static inline
    symbol_type
    make_STR (const std::string& v, const location_type& l);

    static inline
    symbol_type
    make_GLSLSHADER (const location_type& l);

    static inline
    symbol_type
    make_HLSL10SHADER (const location_type& l);

    static inline
    symbol_type
    make_HLSL11SHADER (const location_type& l);

    static inline
    symbol_type
    make_CGSHADER (const location_type& l);

    static inline
    symbol_type
    make_SAMPLER_STATE (const location_type& l);

    static inline
    symbol_type
    make_DST_STATE (const location_type& l);

    static inline
    symbol_type
    make_PR_STATE (const location_type& l);

    static inline
    symbol_type
    make_COLOR_SAMPLE_STATE (const location_type& l);

    static inline
    symbol_type
    make_RASTERIZATION_STATE (const location_type& l);

    static inline
    symbol_type
    make_TECHNIQUE (const location_type& l);

    static inline
    symbol_type
    make_PASS (const location_type& l);

    static inline
    symbol_type
    make_CODEBODY (const std::string& v, const location_type& l);

    static inline
    symbol_type
    make_VARNAME (const std::string& v, const location_type& l);

    static inline
    symbol_type
    make_TEXTURERESOURCE (const location_type& l);

    static inline
    symbol_type
    make_TEXTURERESOURCE1D (const location_type& l);

    static inline
    symbol_type
    make_TEXTURERESOURCE2D (const location_type& l);

    static inline
    symbol_type
    make_TEXTURERESOURCE3D (const location_type& l);

    static inline
    symbol_type
    make_TEXTURERESOURCERECT (const location_type& l);

    static inline
    symbol_type
    make_TEXTURERESOURCECUBE (const location_type& l);

    static inline
    symbol_type
    make_VOID_TYPE (const location_type& l);

    static inline
    symbol_type
    make_UNSIGNED (const location_type& l);

    static inline
    symbol_type
    make_HIGHP (const location_type& l);

    static inline
    symbol_type
    make_MEDIUMP (const location_type& l);

    static inline
    symbol_type
    make_LOWP (const location_type& l);

    static inline
    symbol_type
    make_UNIFORM (const location_type& l);

    static inline
    symbol_type
    make_CSTBUFFER (const location_type& l);

    static inline
    symbol_type
    make_FLOAT_TYPE (const location_type& l);

    static inline
    symbol_type
    make_FLOAT2_TYPE (const location_type& l);

    static inline
    symbol_type
    make_FLOAT3_TYPE (const location_type& l);

    static inline
    symbol_type
    make_FLOAT4_TYPE (const location_type& l);

    static inline
    symbol_type
    make_MAT2_TYPE (const location_type& l);

    static inline
    symbol_type
    make_MAT3_TYPE (const location_type& l);

    static inline
    symbol_type
    make_MAT4_TYPE (const location_type& l);

    static inline
    symbol_type
    make_BOOL_TYPE (const location_type& l);

    static inline
    symbol_type
    make_BOOL2_TYPE (const location_type& l);

    static inline
    symbol_type
    make_BOOL3_TYPE (const location_type& l);

    static inline
    symbol_type
    make_BOOL4_TYPE (const location_type& l);

    static inline
    symbol_type
    make_INT_TYPE (const location_type& l);

    static inline
    symbol_type
    make_INT2_TYPE (const location_type& l);

    static inline
    symbol_type
    make_INT3_TYPE (const location_type& l);

    static inline
    symbol_type
    make_INT4_TYPE (const location_type& l);

    static inline
    symbol_type
    make_TEXTURE1D_TYPE (const location_type& l);

    static inline
    symbol_type
    make_TEXTURE2D_TYPE (const location_type& l);

    static inline
    symbol_type
    make_TEXTURE2DSHADOW_TYPE (const location_type& l);

    static inline
    symbol_type
    make_TEXTURE2DRECT_TYPE (const location_type& l);

    static inline
    symbol_type
    make_TEXTURE3D_TYPE (const location_type& l);

    static inline
    symbol_type
    make_TEXTURECUBE_TYPE (const location_type& l);

    static inline
    symbol_type
    make_SAMPLER1D_TYPE (const location_type& l);

    static inline
    symbol_type
    make_SAMPLER2D_TYPE (const location_type& l);

    static inline
    symbol_type
    make_SAMPLER2DSHADOW_TYPE (const location_type& l);

    static inline
    symbol_type
    make_SAMPLER2DRECT_TYPE (const location_type& l);

    static inline
    symbol_type
    make_SAMPLER3D_TYPE (const location_type& l);

    static inline
    symbol_type
    make_SAMPLERCUBE_TYPE (const location_type& l);

    static inline
    symbol_type
    make_EXTENSION (const location_type& l);

    static inline
    symbol_type
    make_SEPARATE_SHADER (const location_type& l);

    static inline
    symbol_type
    make_VERTEXPROGRAM (const location_type& l);

    static inline
    symbol_type
    make_FRAGMENTPROGRAM (const location_type& l);

    static inline
    symbol_type
    make_GEOMETRYPROGRAM (const location_type& l);

    static inline
    symbol_type
    make_HULLPROGRAM (const location_type& l);

    static inline
    symbol_type
    make_EVALPROGRAM (const location_type& l);

    static inline
    symbol_type
    make_SHDPROFILE (const location_type& l);

    static inline
    symbol_type
    make_SAMPLERRESOURCE (const location_type& l);

    static inline
    symbol_type
    make_SAMPLERTEXUNIT (const location_type& l);

    static inline
    symbol_type
    make_SETSAMPLERSTATE (const location_type& l);

    static inline
    symbol_type
    make_SETDSTSTATE (const location_type& l);

    static inline
    symbol_type
    make_SETRASTERIZATIONSTATE (const location_type& l);

    static inline
    symbol_type
    make_SETCOLORSAMPLESTATE (const location_type& l);

    static inline
    symbol_type
    make_IMAGERESOURCE (const location_type& l);

    static inline
    symbol_type
    make_IMAGEUNIT (const location_type& l);

    static inline
    symbol_type
    make_IMAGEACCESS (const location_type& l);

    static inline
    symbol_type
    make_IMAGELAYER (const location_type& l);

    static inline
    symbol_type
    make_IMAGELAYERED (const location_type& l);

    static inline
    symbol_type
    make_WRITE_ONLY (const location_type& l);

    static inline
    symbol_type
    make_READ_ONLY (const location_type& l);

    static inline
    symbol_type
    make_READ_WRITE (const location_type& l);

    static inline
    symbol_type
    make_FORMAT_UNDEFINED (const VkFormat& v, const location_type& l);

    static inline
    symbol_type
    make_FORMAT_R4G4_UNORM_PACK8 (const VkFormat& v, const location_type& l);

    static inline
    symbol_type
    make_FORMAT_R4G4B4A4_UNORM_PACK16 (const VkFormat& v, const location_type& l);

    static inline
    symbol_type
    make_FORMAT_B4G4R4A4_UNORM_PACK16 (const VkFormat& v, const location_type& l);

    static inline
    symbol_type
    make_FORMAT_R5G6B5_UNORM_PACK16 (const VkFormat& v, const location_type& l);

    static inline
    symbol_type
    make_FORMAT_B5G6R5_UNORM_PACK16 (const VkFormat& v, const location_type& l);

    static inline
    symbol_type
    make_FORMAT_R5G5B5A1_UNORM_PACK16 (const VkFormat& v, const location_type& l);

    static inline
    symbol_type
    make_FORMAT_B5G5R5A1_UNORM_PACK16 (const VkFormat& v, const location_type& l);

    static inline
    symbol_type
    make_FORMAT_A1R5G5B5_UNORM_PACK16 (const VkFormat& v, const location_type& l);

    static inline
    symbol_type
    make_FORMAT_R8_UNORM (const VkFormat& v, const location_type& l);

    static inline
    symbol_type
    make_FORMAT_R8_SNORM (const VkFormat& v, const location_type& l);

    static inline
    symbol_type
    make_FORMAT_R8_USCALED (const VkFormat& v, const location_type& l);

    static inline
    symbol_type
    make_FORMAT_R8_SSCALED (const VkFormat& v, const location_type& l);

    static inline
    symbol_type
    make_FORMAT_R8_UINT (const VkFormat& v, const location_type& l);

    static inline
    symbol_type
    make_FORMAT_R8_SINT (const VkFormat& v, const location_type& l);

    static inline
    symbol_type
    make_FORMAT_R8_SRGB (const VkFormat& v, const location_type& l);

    static inline
    symbol_type
    make_FORMAT_R8G8_UNORM (const VkFormat& v, const location_type& l);

    static inline
    symbol_type
    make_FORMAT_R8G8_SNORM (const VkFormat& v, const location_type& l);

    static inline
    symbol_type
    make_FORMAT_R8G8_USCALED (const VkFormat& v, const location_type& l);

    static inline
    symbol_type
    make_FORMAT_R8G8_SSCALED (const VkFormat& v, const location_type& l);

    static inline
    symbol_type
    make_FORMAT_R8G8_UINT (const VkFormat& v, const location_type& l);

    static inline
    symbol_type
    make_FORMAT_R8G8_SINT (const VkFormat& v, const location_type& l);

    static inline
    symbol_type
    make_FORMAT_R8G8_SRGB (const VkFormat& v, const location_type& l);

    static inline
    symbol_type
    make_FORMAT_R8G8B8_UNORM (const VkFormat& v, const location_type& l);

    static inline
    symbol_type
    make_FORMAT_R8G8B8_SNORM (const VkFormat& v, const location_type& l);

    static inline
    symbol_type
    make_FORMAT_R8G8B8_USCALED (const VkFormat& v, const location_type& l);

    static inline
    symbol_type
    make_FORMAT_R8G8B8_SSCALED (const VkFormat& v, const location_type& l);

    static inline
    symbol_type
    make_FORMAT_R8G8B8_UINT (const VkFormat& v, const location_type& l);

    static inline
    symbol_type
    make_FORMAT_R8G8B8_SINT (const VkFormat& v, const location_type& l);

    static inline
    symbol_type
    make_FORMAT_R8G8B8_SRGB (const VkFormat& v, const location_type& l);

    static inline
    symbol_type
    make_FORMAT_B8G8R8_UNORM (const VkFormat& v, const location_type& l);

    static inline
    symbol_type
    make_FORMAT_B8G8R8_SNORM (const VkFormat& v, const location_type& l);

    static inline
    symbol_type
    make_FORMAT_B8G8R8_USCALED (const VkFormat& v, const location_type& l);

    static inline
    symbol_type
    make_FORMAT_B8G8R8_SSCALED (const VkFormat& v, const location_type& l);

    static inline
    symbol_type
    make_FORMAT_B8G8R8_UINT (const VkFormat& v, const location_type& l);

    static inline
    symbol_type
    make_FORMAT_B8G8R8_SINT (const VkFormat& v, const location_type& l);

    static inline
    symbol_type
    make_FORMAT_B8G8R8_SRGB (const VkFormat& v, const location_type& l);

    static inline
    symbol_type
    make_FORMAT_R8G8B8A8_UNORM (const VkFormat& v, const location_type& l);

    static inline
    symbol_type
    make_FORMAT_R8G8B8A8_SNORM (const VkFormat& v, const location_type& l);

    static inline
    symbol_type
    make_FORMAT_R8G8B8A8_USCALED (const VkFormat& v, const location_type& l);

    static inline
    symbol_type
    make_FORMAT_R8G8B8A8_SSCALED (const VkFormat& v, const location_type& l);

    static inline
    symbol_type
    make_FORMAT_R8G8B8A8_UINT (const VkFormat& v, const location_type& l);

    static inline
    symbol_type
    make_FORMAT_R8G8B8A8_SINT (const VkFormat& v, const location_type& l);

    static inline
    symbol_type
    make_FORMAT_R8G8B8A8_SRGB (const VkFormat& v, const location_type& l);

    static inline
    symbol_type
    make_FORMAT_B8G8R8A8_UNORM (const VkFormat& v, const location_type& l);

    static inline
    symbol_type
    make_FORMAT_B8G8R8A8_SNORM (const VkFormat& v, const location_type& l);

    static inline
    symbol_type
    make_FORMAT_B8G8R8A8_USCALED (const VkFormat& v, const location_type& l);

    static inline
    symbol_type
    make_FORMAT_B8G8R8A8_SSCALED (const VkFormat& v, const location_type& l);

    static inline
    symbol_type
    make_FORMAT_B8G8R8A8_UINT (const VkFormat& v, const location_type& l);

    static inline
    symbol_type
    make_FORMAT_B8G8R8A8_SINT (const VkFormat& v, const location_type& l);

    static inline
    symbol_type
    make_FORMAT_B8G8R8A8_SRGB (const VkFormat& v, const location_type& l);

    static inline
    symbol_type
    make_FORMAT_A8B8G8R8_UNORM_PACK32 (const VkFormat& v, const location_type& l);

    static inline
    symbol_type
    make_FORMAT_A8B8G8R8_SNORM_PACK32 (const VkFormat& v, const location_type& l);

    static inline
    symbol_type
    make_FORMAT_A8B8G8R8_USCALED_PACK32 (const VkFormat& v, const location_type& l);

    static inline
    symbol_type
    make_FORMAT_A8B8G8R8_SSCALED_PACK32 (const VkFormat& v, const location_type& l);

    static inline
    symbol_type
    make_FORMAT_A8B8G8R8_UINT_PACK32 (const VkFormat& v, const location_type& l);

    static inline
    symbol_type
    make_FORMAT_A8B8G8R8_SINT_PACK32 (const VkFormat& v, const location_type& l);

    static inline
    symbol_type
    make_FORMAT_A8B8G8R8_SRGB_PACK32 (const VkFormat& v, const location_type& l);

    static inline
    symbol_type
    make_FORMAT_A2R10G10B10_UNORM_PACK32 (const VkFormat& v, const location_type& l);

    static inline
    symbol_type
    make_FORMAT_A2R10G10B10_SNORM_PACK32 (const VkFormat& v, const location_type& l);

    static inline
    symbol_type
    make_FORMAT_A2R10G10B10_USCALED_PACK32 (const VkFormat& v, const location_type& l);

    static inline
    symbol_type
    make_FORMAT_A2R10G10B10_SSCALED_PACK32 (const VkFormat& v, const location_type& l);

    static inline
    symbol_type
    make_FORMAT_A2R10G10B10_UINT_PACK32 (const VkFormat& v, const location_type& l);

    static inline
    symbol_type
    make_FORMAT_A2R10G10B10_SINT_PACK32 (const VkFormat& v, const location_type& l);

    static inline
    symbol_type
    make_FORMAT_A2B10G10R10_UNORM_PACK32 (const VkFormat& v, const location_type& l);

    static inline
    symbol_type
    make_FORMAT_A2B10G10R10_SNORM_PACK32 (const VkFormat& v, const location_type& l);

    static inline
    symbol_type
    make_FORMAT_A2B10G10R10_USCALED_PACK32 (const VkFormat& v, const location_type& l);

    static inline
    symbol_type
    make_FORMAT_A2B10G10R10_SSCALED_PACK32 (const VkFormat& v, const location_type& l);

    static inline
    symbol_type
    make_FORMAT_A2B10G10R10_UINT_PACK32 (const VkFormat& v, const location_type& l);

    static inline
    symbol_type
    make_FORMAT_A2B10G10R10_SINT_PACK32 (const VkFormat& v, const location_type& l);

    static inline
    symbol_type
    make_FORMAT_R16_UNORM (const VkFormat& v, const location_type& l);

    static inline
    symbol_type
    make_FORMAT_R16_SNORM (const VkFormat& v, const location_type& l);

    static inline
    symbol_type
    make_FORMAT_R16_USCALED (const VkFormat& v, const location_type& l);

    static inline
    symbol_type
    make_FORMAT_R16_SSCALED (const VkFormat& v, const location_type& l);

    static inline
    symbol_type
    make_FORMAT_R16_UINT (const VkFormat& v, const location_type& l);

    static inline
    symbol_type
    make_FORMAT_R16_SINT (const VkFormat& v, const location_type& l);

    static inline
    symbol_type
    make_FORMAT_R16_SFLOAT (const VkFormat& v, const location_type& l);

    static inline
    symbol_type
    make_FORMAT_R16G16_UNORM (const VkFormat& v, const location_type& l);

    static inline
    symbol_type
    make_FORMAT_R16G16_SNORM (const VkFormat& v, const location_type& l);

    static inline
    symbol_type
    make_FORMAT_R16G16_USCALED (const VkFormat& v, const location_type& l);

    static inline
    symbol_type
    make_FORMAT_R16G16_SSCALED (const VkFormat& v, const location_type& l);

    static inline
    symbol_type
    make_FORMAT_R16G16_UINT (const VkFormat& v, const location_type& l);

    static inline
    symbol_type
    make_FORMAT_R16G16_SINT (const VkFormat& v, const location_type& l);

    static inline
    symbol_type
    make_FORMAT_R16G16_SFLOAT (const VkFormat& v, const location_type& l);

    static inline
    symbol_type
    make_FORMAT_R16G16B16_UNORM (const VkFormat& v, const location_type& l);

    static inline
    symbol_type
    make_FORMAT_R16G16B16_SNORM (const VkFormat& v, const location_type& l);

    static inline
    symbol_type
    make_FORMAT_R16G16B16_USCALED (const VkFormat& v, const location_type& l);

    static inline
    symbol_type
    make_FORMAT_R16G16B16_SSCALED (const VkFormat& v, const location_type& l);

    static inline
    symbol_type
    make_FORMAT_R16G16B16_UINT (const VkFormat& v, const location_type& l);

    static inline
    symbol_type
    make_FORMAT_R16G16B16_SINT (const VkFormat& v, const location_type& l);

    static inline
    symbol_type
    make_FORMAT_R16G16B16_SFLOAT (const VkFormat& v, const location_type& l);

    static inline
    symbol_type
    make_FORMAT_R16G16B16A16_UNORM (const VkFormat& v, const location_type& l);

    static inline
    symbol_type
    make_FORMAT_R16G16B16A16_SNORM (const VkFormat& v, const location_type& l);

    static inline
    symbol_type
    make_FORMAT_R16G16B16A16_USCALED (const VkFormat& v, const location_type& l);

    static inline
    symbol_type
    make_FORMAT_R16G16B16A16_SSCALED (const VkFormat& v, const location_type& l);

    static inline
    symbol_type
    make_FORMAT_R16G16B16A16_UINT (const VkFormat& v, const location_type& l);

    static inline
    symbol_type
    make_FORMAT_R16G16B16A16_SINT (const VkFormat& v, const location_type& l);

    static inline
    symbol_type
    make_FORMAT_R16G16B16A16_SFLOAT (const VkFormat& v, const location_type& l);

    static inline
    symbol_type
    make_FORMAT_R32_UINT (const VkFormat& v, const location_type& l);

    static inline
    symbol_type
    make_FORMAT_R32_SINT (const VkFormat& v, const location_type& l);

    static inline
    symbol_type
    make_FORMAT_R32_SFLOAT (const VkFormat& v, const location_type& l);

    static inline
    symbol_type
    make_FORMAT_R32G32_UINT (const VkFormat& v, const location_type& l);

    static inline
    symbol_type
    make_FORMAT_R32G32_SINT (const VkFormat& v, const location_type& l);

    static inline
    symbol_type
    make_FORMAT_R32G32_SFLOAT (const VkFormat& v, const location_type& l);

    static inline
    symbol_type
    make_FORMAT_R32G32B32_UINT (const VkFormat& v, const location_type& l);

    static inline
    symbol_type
    make_FORMAT_R32G32B32_SINT (const VkFormat& v, const location_type& l);

    static inline
    symbol_type
    make_FORMAT_R32G32B32_SFLOAT (const VkFormat& v, const location_type& l);

    static inline
    symbol_type
    make_FORMAT_R32G32B32A32_UINT (const VkFormat& v, const location_type& l);

    static inline
    symbol_type
    make_FORMAT_R32G32B32A32_SINT (const VkFormat& v, const location_type& l);

    static inline
    symbol_type
    make_FORMAT_R32G32B32A32_SFLOAT (const VkFormat& v, const location_type& l);

    static inline
    symbol_type
    make_FORMAT_R64_UINT (const VkFormat& v, const location_type& l);

    static inline
    symbol_type
    make_FORMAT_R64_SINT (const VkFormat& v, const location_type& l);

    static inline
    symbol_type
    make_FORMAT_R64_SFLOAT (const VkFormat& v, const location_type& l);

    static inline
    symbol_type
    make_FORMAT_R64G64_UINT (const VkFormat& v, const location_type& l);

    static inline
    symbol_type
    make_FORMAT_R64G64_SINT (const VkFormat& v, const location_type& l);

    static inline
    symbol_type
    make_FORMAT_R64G64_SFLOAT (const VkFormat& v, const location_type& l);

    static inline
    symbol_type
    make_FORMAT_R64G64B64_UINT (const VkFormat& v, const location_type& l);

    static inline
    symbol_type
    make_FORMAT_R64G64B64_SINT (const VkFormat& v, const location_type& l);

    static inline
    symbol_type
    make_FORMAT_R64G64B64_SFLOAT (const VkFormat& v, const location_type& l);

    static inline
    symbol_type
    make_FORMAT_R64G64B64A64_UINT (const VkFormat& v, const location_type& l);

    static inline
    symbol_type
    make_FORMAT_R64G64B64A64_SINT (const VkFormat& v, const location_type& l);

    static inline
    symbol_type
    make_FORMAT_R64G64B64A64_SFLOAT (const VkFormat& v, const location_type& l);

    static inline
    symbol_type
    make_FORMAT_B10G11R11_UFLOAT_PACK32 (const VkFormat& v, const location_type& l);

    static inline
    symbol_type
    make_FORMAT_E5B9G9R9_UFLOAT_PACK32 (const VkFormat& v, const location_type& l);

    static inline
    symbol_type
    make_FORMAT_D16_UNORM (const VkFormat& v, const location_type& l);

    static inline
    symbol_type
    make_FORMAT_X8_D24_UNORM_PACK32 (const VkFormat& v, const location_type& l);

    static inline
    symbol_type
    make_FORMAT_D32_SFLOAT (const VkFormat& v, const location_type& l);

    static inline
    symbol_type
    make_FORMAT_S8_UINT (const VkFormat& v, const location_type& l);

    static inline
    symbol_type
    make_FORMAT_D16_UNORM_S8_UINT (const VkFormat& v, const location_type& l);

    static inline
    symbol_type
    make_FORMAT_D24_UNORM_S8_UINT (const VkFormat& v, const location_type& l);

    static inline
    symbol_type
    make_FORMAT_D32_SFLOAT_S8_UINT (const VkFormat& v, const location_type& l);

    static inline
    symbol_type
    make_FORMAT_BC1_RGB_UNORM_BLOCK (const VkFormat& v, const location_type& l);

    static inline
    symbol_type
    make_FORMAT_BC1_RGB_SRGB_BLOCK (const VkFormat& v, const location_type& l);

    static inline
    symbol_type
    make_FORMAT_BC1_RGBA_UNORM_BLOCK (const VkFormat& v, const location_type& l);

    static inline
    symbol_type
    make_FORMAT_BC1_RGBA_SRGB_BLOCK (const VkFormat& v, const location_type& l);

    static inline
    symbol_type
    make_FORMAT_BC2_UNORM_BLOCK (const VkFormat& v, const location_type& l);

    static inline
    symbol_type
    make_FORMAT_BC2_SRGB_BLOCK (const VkFormat& v, const location_type& l);

    static inline
    symbol_type
    make_FORMAT_BC3_UNORM_BLOCK (const VkFormat& v, const location_type& l);

    static inline
    symbol_type
    make_FORMAT_BC3_SRGB_BLOCK (const VkFormat& v, const location_type& l);

    static inline
    symbol_type
    make_FORMAT_BC4_UNORM_BLOCK (const VkFormat& v, const location_type& l);

    static inline
    symbol_type
    make_FORMAT_BC4_SNORM_BLOCK (const VkFormat& v, const location_type& l);

    static inline
    symbol_type
    make_FORMAT_BC5_UNORM_BLOCK (const VkFormat& v, const location_type& l);

    static inline
    symbol_type
    make_FORMAT_BC5_SNORM_BLOCK (const VkFormat& v, const location_type& l);

    static inline
    symbol_type
    make_FORMAT_BC6H_UFLOAT_BLOCK (const VkFormat& v, const location_type& l);

    static inline
    symbol_type
    make_FORMAT_BC6H_SFLOAT_BLOCK (const VkFormat& v, const location_type& l);

    static inline
    symbol_type
    make_FORMAT_BC7_UNORM_BLOCK (const VkFormat& v, const location_type& l);

    static inline
    symbol_type
    make_FORMAT_BC7_SRGB_BLOCK (const VkFormat& v, const location_type& l);

    static inline
    symbol_type
    make_FORMAT_ETC2_R8G8B8_UNORM_BLOCK (const VkFormat& v, const location_type& l);

    static inline
    symbol_type
    make_FORMAT_ETC2_R8G8B8_SRGB_BLOCK (const VkFormat& v, const location_type& l);

    static inline
    symbol_type
    make_FORMAT_ETC2_R8G8B8A1_UNORM_BLOCK (const VkFormat& v, const location_type& l);

    static inline
    symbol_type
    make_FORMAT_ETC2_R8G8B8A1_SRGB_BLOCK (const VkFormat& v, const location_type& l);

    static inline
    symbol_type
    make_FORMAT_ETC2_R8G8B8A8_UNORM_BLOCK (const VkFormat& v, const location_type& l);

    static inline
    symbol_type
    make_FORMAT_ETC2_R8G8B8A8_SRGB_BLOCK (const VkFormat& v, const location_type& l);

    static inline
    symbol_type
    make_FORMAT_EAC_R11_UNORM_BLOCK (const VkFormat& v, const location_type& l);

    static inline
    symbol_type
    make_FORMAT_EAC_R11_SNORM_BLOCK (const VkFormat& v, const location_type& l);

    static inline
    symbol_type
    make_FORMAT_EAC_R11G11_UNORM_BLOCK (const VkFormat& v, const location_type& l);

    static inline
    symbol_type
    make_FORMAT_EAC_R11G11_SNORM_BLOCK (const VkFormat& v, const location_type& l);

    static inline
    symbol_type
    make_FORMAT_ASTC_4x4_UNORM_BLOCK (const VkFormat& v, const location_type& l);

    static inline
    symbol_type
    make_FORMAT_ASTC_4x4_SRGB_BLOCK (const VkFormat& v, const location_type& l);

    static inline
    symbol_type
    make_FORMAT_ASTC_5x4_UNORM_BLOCK (const VkFormat& v, const location_type& l);

    static inline
    symbol_type
    make_FORMAT_ASTC_5x4_SRGB_BLOCK (const VkFormat& v, const location_type& l);

    static inline
    symbol_type
    make_FORMAT_ASTC_5x5_UNORM_BLOCK (const VkFormat& v, const location_type& l);

    static inline
    symbol_type
    make_FORMAT_ASTC_5x5_SRGB_BLOCK (const VkFormat& v, const location_type& l);

    static inline
    symbol_type
    make_FORMAT_ASTC_6x5_UNORM_BLOCK (const VkFormat& v, const location_type& l);

    static inline
    symbol_type
    make_FORMAT_ASTC_6x5_SRGB_BLOCK (const VkFormat& v, const location_type& l);

    static inline
    symbol_type
    make_FORMAT_ASTC_6x6_UNORM_BLOCK (const VkFormat& v, const location_type& l);

    static inline
    symbol_type
    make_FORMAT_ASTC_6x6_SRGB_BLOCK (const VkFormat& v, const location_type& l);

    static inline
    symbol_type
    make_FORMAT_ASTC_8x5_UNORM_BLOCK (const VkFormat& v, const location_type& l);

    static inline
    symbol_type
    make_FORMAT_ASTC_8x5_SRGB_BLOCK (const VkFormat& v, const location_type& l);

    static inline
    symbol_type
    make_FORMAT_ASTC_8x6_UNORM_BLOCK (const VkFormat& v, const location_type& l);

    static inline
    symbol_type
    make_FORMAT_ASTC_8x6_SRGB_BLOCK (const VkFormat& v, const location_type& l);

    static inline
    symbol_type
    make_FORMAT_ASTC_8x8_UNORM_BLOCK (const VkFormat& v, const location_type& l);

    static inline
    symbol_type
    make_FORMAT_ASTC_8x8_SRGB_BLOCK (const VkFormat& v, const location_type& l);

    static inline
    symbol_type
    make_FORMAT_ASTC_10x5_UNORM_BLOCK (const VkFormat& v, const location_type& l);

    static inline
    symbol_type
    make_FORMAT_ASTC_10x5_SRGB_BLOCK (const VkFormat& v, const location_type& l);

    static inline
    symbol_type
    make_FORMAT_ASTC_10x6_UNORM_BLOCK (const VkFormat& v, const location_type& l);

    static inline
    symbol_type
    make_FORMAT_ASTC_10x6_SRGB_BLOCK (const VkFormat& v, const location_type& l);

    static inline
    symbol_type
    make_FORMAT_ASTC_10x8_UNORM_BLOCK (const VkFormat& v, const location_type& l);

    static inline
    symbol_type
    make_FORMAT_ASTC_10x8_SRGB_BLOCK (const VkFormat& v, const location_type& l);

    static inline
    symbol_type
    make_FORMAT_ASTC_10x10_UNORM_BLOCK (const VkFormat& v, const location_type& l);

    static inline
    symbol_type
    make_FORMAT_ASTC_10x10_SRGB_BLOCK (const VkFormat& v, const location_type& l);

    static inline
    symbol_type
    make_FORMAT_ASTC_12x10_UNORM_BLOCK (const VkFormat& v, const location_type& l);

    static inline
    symbol_type
    make_FORMAT_ASTC_12x10_SRGB_BLOCK (const VkFormat& v, const location_type& l);

    static inline
    symbol_type
    make_FORMAT_ASTC_12x12_UNORM_BLOCK (const VkFormat& v, const location_type& l);

    static inline
    symbol_type
    make_FORMAT_ASTC_12x12_SRGB_BLOCK (const VkFormat& v, const location_type& l);

    static inline
    symbol_type
    make_VERTEXFORMAT (const location_type& l);


    /// Build a parser object.
    parser (Scanner &scanner_yyarg, Driver &driver_yyarg);
    virtual ~parser ();

    /// Parse.
    /// \returns  0 iff parsing succeeded.
    virtual int parse ();

#if YYDEBUG
    /// The current debugging stream.
    std::ostream& debug_stream () const YY_ATTRIBUTE_PURE;
    /// Set the current debugging stream.
    void set_debug_stream (std::ostream &);

    /// Type for debugging levels.
    typedef int debug_level_type;
    /// The current debugging level.
    debug_level_type debug_level () const YY_ATTRIBUTE_PURE;
    /// Set the current debugging level.
    void set_debug_level (debug_level_type l);
#endif

    /// Report a syntax error.
    /// \param loc    where the syntax error is found.
    /// \param msg    a description of the syntax error.
    virtual void error (const location_type& loc, const std::string& msg);

    /// Report a syntax error.
    void error (const syntax_error& err);

  private:
    /// This class is not copyable.
    parser (const parser&);
    parser& operator= (const parser&);

    /// State numbers.
    typedef int state_type;

    /// Generate an error message.
    /// \param yystate   the state where the error occurred.
    /// \param yyla      the lookahead token.
    virtual std::string yysyntax_error_ (state_type yystate,
                                         const symbol_type& yyla) const;

    /// Compute post-reduction state.
    /// \param yystate   the current state
    /// \param yysym     the nonterminal to push on the stack
    state_type yy_lr_goto_state_ (state_type yystate, int yysym);

    /// Whether the given \c yypact_ value indicates a defaulted state.
    /// \param yyvalue   the value to check
    static bool yy_pact_value_is_default_ (int yyvalue);

    /// Whether the given \c yytable_ value indicates a syntax error.
    /// \param yyvalue   the value to check
    static bool yy_table_value_is_error_ (int yyvalue);

    static const signed char yypact_ninf_;
    static const signed char yytable_ninf_;

    /// Convert a scanner token number \a t to a symbol number.
    static token_number_type yytranslate_ (token_type t);

    // Tables.
  // YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
  // STATE-NUM.
  static const signed char yypact_[];

  // YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
  // Performed when YYTABLE does not specify something else to do.  Zero
  // means the default is an error.
  static const unsigned char yydefact_[];

  // YYPGOTO[NTERM-NUM].
  static const signed char yypgoto_[];

  // YYDEFGOTO[NTERM-NUM].
  static const short int yydefgoto_[];

  // YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
  // positive, shift that token.  If negative, reduce the rule whose
  // number is the opposite.  If YYTABLE_NINF, syntax error.
  static const unsigned char yytable_[];

  static const short int yycheck_[];

  // YYSTOS[STATE-NUM] -- The (internal number of the) accessing
  // symbol of state STATE-NUM.
  static const unsigned short int yystos_[];

  // YYR1[YYN] -- Symbol number of symbol that rule YYN derives.
  static const unsigned short int yyr1_[];

  // YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.
  static const unsigned char yyr2_[];


    /// Convert the symbol name \a n to a form suitable for a diagnostic.
    static std::string yytnamerr_ (const char *n);


    /// For a symbol, its name in clear.
    static const char* const yytname_[];
#if YYDEBUG
  // YYRLINE[YYN] -- Source line where rule number YYN was defined.
  static const unsigned short int yyrline_[];
    /// Report on the debug stream that the rule \a r is going to be reduced.
    virtual void yy_reduce_print_ (int r);
    /// Print the state stack on the debug stream.
    virtual void yystack_print_ ();

    // Debugging.
    int yydebug_;
    std::ostream* yycdebug_;

    /// \brief Display a symbol type, value and location.
    /// \param yyo    The output stream.
    /// \param yysym  The symbol.
    template <typename Base>
    void yy_print_ (std::ostream& yyo, const basic_symbol<Base>& yysym) const;
#endif

    /// \brief Reclaim the memory associated to a symbol.
    /// \param yymsg     Why this token is reclaimed.
    ///                  If null, print nothing.
    /// \param yysym     The symbol.
    template <typename Base>
    void yy_destroy_ (const char* yymsg, basic_symbol<Base>& yysym) const;

  private:
    /// Type access provider for state based symbols.
    struct by_state
    {
      /// Default constructor.
      by_state ();

      /// The symbol type as needed by the constructor.
      typedef state_type kind_type;

      /// Constructor.
      by_state (kind_type s);

      /// Copy constructor.
      by_state (const by_state& other);

      /// Record that this symbol is empty.
      void clear ();

      /// Steal the symbol type from \a that.
      void move (by_state& that);

      /// The (internal) type number (corresponding to \a state).
      /// \a empty_symbol when empty.
      symbol_number_type type_get () const;

      /// The state number used to denote an empty symbol.
      enum { empty_state = -1 };

      /// The state.
      /// \a empty when empty.
      state_type state;
    };

    /// "Internal" symbol: element of the stack.
    struct stack_symbol_type : basic_symbol<by_state>
    {
      /// Superclass.
      typedef basic_symbol<by_state> super_type;
      /// Construct an empty symbol.
      stack_symbol_type ();
      /// Steal the contents from \a sym to build this.
      stack_symbol_type (state_type s, symbol_type& sym);
      /// Assignment, needed by push_back.
      stack_symbol_type& operator= (const stack_symbol_type& that);
    };

    /// Stack type.
    typedef stack<stack_symbol_type> stack_type;

    /// The stack.
    stack_type yystack_;

    /// Push a new state on the stack.
    /// \param m    a debug message to display
    ///             if null, no trace is output.
    /// \param s    the symbol
    /// \warning the contents of \a s.value is stolen.
    void yypush_ (const char* m, stack_symbol_type& s);

    /// Push a new look ahead token on the state on the stack.
    /// \param m    a debug message to display
    ///             if null, no trace is output.
    /// \param s    the state
    /// \param sym  the symbol (for its value and location).
    /// \warning the contents of \a s.value is stolen.
    void yypush_ (const char* m, state_type s, symbol_type& sym);

    /// Pop \a n symbols the three stacks.
    void yypop_ (unsigned int n = 1);

    /// Constants.
    enum
    {
      yyeof_ = 0,
      yylast_ = 308,     ///< Last index in yytable_.
      yynnts_ = 18,  ///< Number of nonterminal symbols.
      yyfinal_ = 3, ///< Termination state number.
      yyterror_ = 1,
      yyerrcode_ = 256,
      yyntokens_ = 295  ///< Number of tokens.
    };


    // User arguments.
    Scanner &scanner;
    Driver &driver;
  };

  // Symbol number corresponding to token number t.
  inline
  parser::token_number_type
  parser::yytranslate_ (token_type t)
  {
    static
    const token_number_type
    translate_table[] =
    {
     0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    86,    87,    88,    89,    90,    91,    92,    93,    94,
      95,    96,    97,    98,    99,   100,   101,   102,   103,   104,
     105,   106,   107,   108,   109,   110,   111,   112,   113,   114,
     115,   116,   117,   118,   119,   120,   121,   122,   123,   124,
     125,   126,   127,   128,   129,   130,   131,   132,   133,   134,
     135,   136,   137,   138,   139,   140,   141,   142,   143,   144,
     145,   146,   147,   148,   149,   150,   151,   152,   153,   154,
     155,   156,   157,   158,   159,   160,   161,   162,   163,   164,
     165,   166,   167,   168,   169,   170,   171,   172,   173,   174,
     175,   176,   177,   178,   179,   180,   181,   182,   183,   184,
     185,   186,   187,   188,   189,   190,   191,   192,   193,   194,
     195,   196,   197,   198,   199,   200,   201,   202,   203,   204,
     205,   206,   207,   208,   209,   210,   211,   212,   213,   214,
     215,   216,   217,   218,   219,   220,   221,   222,   223,   224,
     225,   226,   227,   228,   229,   230,   231,   232,   233,   234,
     235,   236,   237,   238,   239,   240,   241,   242,   243,   244,
     245,   246,   247,   248,   249,   250,   251,   252,   253,   254,
     255,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,   292,   293,   294
    };
    const unsigned int user_token_number_max_ = 549;
    const token_number_type undef_token_ = 2;

    if (static_cast<int>(t) <= yyeof_)
      return yyeof_;
    else if (static_cast<unsigned int> (t) <= user_token_number_max_)
      return translate_table[t];
    else
      return undef_token_;
  }

  inline
  parser::syntax_error::syntax_error (const location_type& l, const std::string& m)
    : std::runtime_error (m)
    , location (l)
  {}

  // basic_symbol.
  template <typename Base>
  inline
  parser::basic_symbol<Base>::basic_symbol ()
    : value ()
  {}

  template <typename Base>
  inline
  parser::basic_symbol<Base>::basic_symbol (const basic_symbol& other)
    : Base (other)
    , value ()
    , location (other.location)
  {
      switch (other.type_get ())
    {
      case 96: // FORMAT_UNDEFINED
      case 97: // FORMAT_R4G4_UNORM_PACK8
      case 98: // FORMAT_R4G4B4A4_UNORM_PACK16
      case 99: // FORMAT_B4G4R4A4_UNORM_PACK16
      case 100: // FORMAT_R5G6B5_UNORM_PACK16
      case 101: // FORMAT_B5G6R5_UNORM_PACK16
      case 102: // FORMAT_R5G5B5A1_UNORM_PACK16
      case 103: // FORMAT_B5G5R5A1_UNORM_PACK16
      case 104: // FORMAT_A1R5G5B5_UNORM_PACK16
      case 105: // FORMAT_R8_UNORM
      case 106: // FORMAT_R8_SNORM
      case 107: // FORMAT_R8_USCALED
      case 108: // FORMAT_R8_SSCALED
      case 109: // FORMAT_R8_UINT
      case 110: // FORMAT_R8_SINT
      case 111: // FORMAT_R8_SRGB
      case 112: // FORMAT_R8G8_UNORM
      case 113: // FORMAT_R8G8_SNORM
      case 114: // FORMAT_R8G8_USCALED
      case 115: // FORMAT_R8G8_SSCALED
      case 116: // FORMAT_R8G8_UINT
      case 117: // FORMAT_R8G8_SINT
      case 118: // FORMAT_R8G8_SRGB
      case 119: // FORMAT_R8G8B8_UNORM
      case 120: // FORMAT_R8G8B8_SNORM
      case 121: // FORMAT_R8G8B8_USCALED
      case 122: // FORMAT_R8G8B8_SSCALED
      case 123: // FORMAT_R8G8B8_UINT
      case 124: // FORMAT_R8G8B8_SINT
      case 125: // FORMAT_R8G8B8_SRGB
      case 126: // FORMAT_B8G8R8_UNORM
      case 127: // FORMAT_B8G8R8_SNORM
      case 128: // FORMAT_B8G8R8_USCALED
      case 129: // FORMAT_B8G8R8_SSCALED
      case 130: // FORMAT_B8G8R8_UINT
      case 131: // FORMAT_B8G8R8_SINT
      case 132: // FORMAT_B8G8R8_SRGB
      case 133: // FORMAT_R8G8B8A8_UNORM
      case 134: // FORMAT_R8G8B8A8_SNORM
      case 135: // FORMAT_R8G8B8A8_USCALED
      case 136: // FORMAT_R8G8B8A8_SSCALED
      case 137: // FORMAT_R8G8B8A8_UINT
      case 138: // FORMAT_R8G8B8A8_SINT
      case 139: // FORMAT_R8G8B8A8_SRGB
      case 140: // FORMAT_B8G8R8A8_UNORM
      case 141: // FORMAT_B8G8R8A8_SNORM
      case 142: // FORMAT_B8G8R8A8_USCALED
      case 143: // FORMAT_B8G8R8A8_SSCALED
      case 144: // FORMAT_B8G8R8A8_UINT
      case 145: // FORMAT_B8G8R8A8_SINT
      case 146: // FORMAT_B8G8R8A8_SRGB
      case 147: // FORMAT_A8B8G8R8_UNORM_PACK32
      case 148: // FORMAT_A8B8G8R8_SNORM_PACK32
      case 149: // FORMAT_A8B8G8R8_USCALED_PACK32
      case 150: // FORMAT_A8B8G8R8_SSCALED_PACK32
      case 151: // FORMAT_A8B8G8R8_UINT_PACK32
      case 152: // FORMAT_A8B8G8R8_SINT_PACK32
      case 153: // FORMAT_A8B8G8R8_SRGB_PACK32
      case 154: // FORMAT_A2R10G10B10_UNORM_PACK32
      case 155: // FORMAT_A2R10G10B10_SNORM_PACK32
      case 156: // FORMAT_A2R10G10B10_USCALED_PACK32
      case 157: // FORMAT_A2R10G10B10_SSCALED_PACK32
      case 158: // FORMAT_A2R10G10B10_UINT_PACK32
      case 159: // FORMAT_A2R10G10B10_SINT_PACK32
      case 160: // FORMAT_A2B10G10R10_UNORM_PACK32
      case 161: // FORMAT_A2B10G10R10_SNORM_PACK32
      case 162: // FORMAT_A2B10G10R10_USCALED_PACK32
      case 163: // FORMAT_A2B10G10R10_SSCALED_PACK32
      case 164: // FORMAT_A2B10G10R10_UINT_PACK32
      case 165: // FORMAT_A2B10G10R10_SINT_PACK32
      case 166: // FORMAT_R16_UNORM
      case 167: // FORMAT_R16_SNORM
      case 168: // FORMAT_R16_USCALED
      case 169: // FORMAT_R16_SSCALED
      case 170: // FORMAT_R16_UINT
      case 171: // FORMAT_R16_SINT
      case 172: // FORMAT_R16_SFLOAT
      case 173: // FORMAT_R16G16_UNORM
      case 174: // FORMAT_R16G16_SNORM
      case 175: // FORMAT_R16G16_USCALED
      case 176: // FORMAT_R16G16_SSCALED
      case 177: // FORMAT_R16G16_UINT
      case 178: // FORMAT_R16G16_SINT
      case 179: // FORMAT_R16G16_SFLOAT
      case 180: // FORMAT_R16G16B16_UNORM
      case 181: // FORMAT_R16G16B16_SNORM
      case 182: // FORMAT_R16G16B16_USCALED
      case 183: // FORMAT_R16G16B16_SSCALED
      case 184: // FORMAT_R16G16B16_UINT
      case 185: // FORMAT_R16G16B16_SINT
      case 186: // FORMAT_R16G16B16_SFLOAT
      case 187: // FORMAT_R16G16B16A16_UNORM
      case 188: // FORMAT_R16G16B16A16_SNORM
      case 189: // FORMAT_R16G16B16A16_USCALED
      case 190: // FORMAT_R16G16B16A16_SSCALED
      case 191: // FORMAT_R16G16B16A16_UINT
      case 192: // FORMAT_R16G16B16A16_SINT
      case 193: // FORMAT_R16G16B16A16_SFLOAT
      case 194: // FORMAT_R32_UINT
      case 195: // FORMAT_R32_SINT
      case 196: // FORMAT_R32_SFLOAT
      case 197: // FORMAT_R32G32_UINT
      case 198: // FORMAT_R32G32_SINT
      case 199: // FORMAT_R32G32_SFLOAT
      case 200: // FORMAT_R32G32B32_UINT
      case 201: // FORMAT_R32G32B32_SINT
      case 202: // FORMAT_R32G32B32_SFLOAT
      case 203: // FORMAT_R32G32B32A32_UINT
      case 204: // FORMAT_R32G32B32A32_SINT
      case 205: // FORMAT_R32G32B32A32_SFLOAT
      case 206: // FORMAT_R64_UINT
      case 207: // FORMAT_R64_SINT
      case 208: // FORMAT_R64_SFLOAT
      case 209: // FORMAT_R64G64_UINT
      case 210: // FORMAT_R64G64_SINT
      case 211: // FORMAT_R64G64_SFLOAT
      case 212: // FORMAT_R64G64B64_UINT
      case 213: // FORMAT_R64G64B64_SINT
      case 214: // FORMAT_R64G64B64_SFLOAT
      case 215: // FORMAT_R64G64B64A64_UINT
      case 216: // FORMAT_R64G64B64A64_SINT
      case 217: // FORMAT_R64G64B64A64_SFLOAT
      case 218: // FORMAT_B10G11R11_UFLOAT_PACK32
      case 219: // FORMAT_E5B9G9R9_UFLOAT_PACK32
      case 220: // FORMAT_D16_UNORM
      case 221: // FORMAT_X8_D24_UNORM_PACK32
      case 222: // FORMAT_D32_SFLOAT
      case 223: // FORMAT_S8_UINT
      case 224: // FORMAT_D16_UNORM_S8_UINT
      case 225: // FORMAT_D24_UNORM_S8_UINT
      case 226: // FORMAT_D32_SFLOAT_S8_UINT
      case 227: // FORMAT_BC1_RGB_UNORM_BLOCK
      case 228: // FORMAT_BC1_RGB_SRGB_BLOCK
      case 229: // FORMAT_BC1_RGBA_UNORM_BLOCK
      case 230: // FORMAT_BC1_RGBA_SRGB_BLOCK
      case 231: // FORMAT_BC2_UNORM_BLOCK
      case 232: // FORMAT_BC2_SRGB_BLOCK
      case 233: // FORMAT_BC3_UNORM_BLOCK
      case 234: // FORMAT_BC3_SRGB_BLOCK
      case 235: // FORMAT_BC4_UNORM_BLOCK
      case 236: // FORMAT_BC4_SNORM_BLOCK
      case 237: // FORMAT_BC5_UNORM_BLOCK
      case 238: // FORMAT_BC5_SNORM_BLOCK
      case 239: // FORMAT_BC6H_UFLOAT_BLOCK
      case 240: // FORMAT_BC6H_SFLOAT_BLOCK
      case 241: // FORMAT_BC7_UNORM_BLOCK
      case 242: // FORMAT_BC7_SRGB_BLOCK
      case 243: // FORMAT_ETC2_R8G8B8_UNORM_BLOCK
      case 244: // FORMAT_ETC2_R8G8B8_SRGB_BLOCK
      case 245: // FORMAT_ETC2_R8G8B8A1_UNORM_BLOCK
      case 246: // FORMAT_ETC2_R8G8B8A1_SRGB_BLOCK
      case 247: // FORMAT_ETC2_R8G8B8A8_UNORM_BLOCK
      case 248: // FORMAT_ETC2_R8G8B8A8_SRGB_BLOCK
      case 249: // FORMAT_EAC_R11_UNORM_BLOCK
      case 250: // FORMAT_EAC_R11_SNORM_BLOCK
      case 251: // FORMAT_EAC_R11G11_UNORM_BLOCK
      case 252: // FORMAT_EAC_R11G11_SNORM_BLOCK
      case 253: // FORMAT_ASTC_4x4_UNORM_BLOCK
      case 254: // FORMAT_ASTC_4x4_SRGB_BLOCK
      case 255: // FORMAT_ASTC_5x4_UNORM_BLOCK
      case 256: // FORMAT_ASTC_5x4_SRGB_BLOCK
      case 257: // FORMAT_ASTC_5x5_UNORM_BLOCK
      case 258: // FORMAT_ASTC_5x5_SRGB_BLOCK
      case 259: // FORMAT_ASTC_6x5_UNORM_BLOCK
      case 260: // FORMAT_ASTC_6x5_SRGB_BLOCK
      case 261: // FORMAT_ASTC_6x6_UNORM_BLOCK
      case 262: // FORMAT_ASTC_6x6_SRGB_BLOCK
      case 263: // FORMAT_ASTC_8x5_UNORM_BLOCK
      case 264: // FORMAT_ASTC_8x5_SRGB_BLOCK
      case 265: // FORMAT_ASTC_8x6_UNORM_BLOCK
      case 266: // FORMAT_ASTC_8x6_SRGB_BLOCK
      case 267: // FORMAT_ASTC_8x8_UNORM_BLOCK
      case 268: // FORMAT_ASTC_8x8_SRGB_BLOCK
      case 269: // FORMAT_ASTC_10x5_UNORM_BLOCK
      case 270: // FORMAT_ASTC_10x5_SRGB_BLOCK
      case 271: // FORMAT_ASTC_10x6_UNORM_BLOCK
      case 272: // FORMAT_ASTC_10x6_SRGB_BLOCK
      case 273: // FORMAT_ASTC_10x8_UNORM_BLOCK
      case 274: // FORMAT_ASTC_10x8_SRGB_BLOCK
      case 275: // FORMAT_ASTC_10x10_UNORM_BLOCK
      case 276: // FORMAT_ASTC_10x10_SRGB_BLOCK
      case 277: // FORMAT_ASTC_12x10_UNORM_BLOCK
      case 278: // FORMAT_ASTC_12x10_SRGB_BLOCK
      case 279: // FORMAT_ASTC_12x12_UNORM_BLOCK
      case 280: // FORMAT_ASTC_12x12_SRGB_BLOCK
      case 296: // format
        value.copy< VkFormat > (other.value);
        break;

      case 15: // TRUE
      case 16: // FALSE
      case 19: // BOOL
        value.copy< bool > (other.value);
        break;

      case 17: // FLOAT
        value.copy< float > (other.value);
        break;

      case 18: // INT
        value.copy< int > (other.value);
        break;

      case 14: // IDENTIFIER
      case 20: // STR
      case 32: // CODEBODY
      case 33: // VARNAME
        value.copy< std::string > (other.value);
        break;

      default:
        break;
    }

  }


  template <typename Base>
  inline
  parser::basic_symbol<Base>::basic_symbol (typename Base::kind_type t, const semantic_type& v, const location_type& l)
    : Base (t)
    , value ()
    , location (l)
  {
    (void) v;
      switch (this->type_get ())
    {
      case 96: // FORMAT_UNDEFINED
      case 97: // FORMAT_R4G4_UNORM_PACK8
      case 98: // FORMAT_R4G4B4A4_UNORM_PACK16
      case 99: // FORMAT_B4G4R4A4_UNORM_PACK16
      case 100: // FORMAT_R5G6B5_UNORM_PACK16
      case 101: // FORMAT_B5G6R5_UNORM_PACK16
      case 102: // FORMAT_R5G5B5A1_UNORM_PACK16
      case 103: // FORMAT_B5G5R5A1_UNORM_PACK16
      case 104: // FORMAT_A1R5G5B5_UNORM_PACK16
      case 105: // FORMAT_R8_UNORM
      case 106: // FORMAT_R8_SNORM
      case 107: // FORMAT_R8_USCALED
      case 108: // FORMAT_R8_SSCALED
      case 109: // FORMAT_R8_UINT
      case 110: // FORMAT_R8_SINT
      case 111: // FORMAT_R8_SRGB
      case 112: // FORMAT_R8G8_UNORM
      case 113: // FORMAT_R8G8_SNORM
      case 114: // FORMAT_R8G8_USCALED
      case 115: // FORMAT_R8G8_SSCALED
      case 116: // FORMAT_R8G8_UINT
      case 117: // FORMAT_R8G8_SINT
      case 118: // FORMAT_R8G8_SRGB
      case 119: // FORMAT_R8G8B8_UNORM
      case 120: // FORMAT_R8G8B8_SNORM
      case 121: // FORMAT_R8G8B8_USCALED
      case 122: // FORMAT_R8G8B8_SSCALED
      case 123: // FORMAT_R8G8B8_UINT
      case 124: // FORMAT_R8G8B8_SINT
      case 125: // FORMAT_R8G8B8_SRGB
      case 126: // FORMAT_B8G8R8_UNORM
      case 127: // FORMAT_B8G8R8_SNORM
      case 128: // FORMAT_B8G8R8_USCALED
      case 129: // FORMAT_B8G8R8_SSCALED
      case 130: // FORMAT_B8G8R8_UINT
      case 131: // FORMAT_B8G8R8_SINT
      case 132: // FORMAT_B8G8R8_SRGB
      case 133: // FORMAT_R8G8B8A8_UNORM
      case 134: // FORMAT_R8G8B8A8_SNORM
      case 135: // FORMAT_R8G8B8A8_USCALED
      case 136: // FORMAT_R8G8B8A8_SSCALED
      case 137: // FORMAT_R8G8B8A8_UINT
      case 138: // FORMAT_R8G8B8A8_SINT
      case 139: // FORMAT_R8G8B8A8_SRGB
      case 140: // FORMAT_B8G8R8A8_UNORM
      case 141: // FORMAT_B8G8R8A8_SNORM
      case 142: // FORMAT_B8G8R8A8_USCALED
      case 143: // FORMAT_B8G8R8A8_SSCALED
      case 144: // FORMAT_B8G8R8A8_UINT
      case 145: // FORMAT_B8G8R8A8_SINT
      case 146: // FORMAT_B8G8R8A8_SRGB
      case 147: // FORMAT_A8B8G8R8_UNORM_PACK32
      case 148: // FORMAT_A8B8G8R8_SNORM_PACK32
      case 149: // FORMAT_A8B8G8R8_USCALED_PACK32
      case 150: // FORMAT_A8B8G8R8_SSCALED_PACK32
      case 151: // FORMAT_A8B8G8R8_UINT_PACK32
      case 152: // FORMAT_A8B8G8R8_SINT_PACK32
      case 153: // FORMAT_A8B8G8R8_SRGB_PACK32
      case 154: // FORMAT_A2R10G10B10_UNORM_PACK32
      case 155: // FORMAT_A2R10G10B10_SNORM_PACK32
      case 156: // FORMAT_A2R10G10B10_USCALED_PACK32
      case 157: // FORMAT_A2R10G10B10_SSCALED_PACK32
      case 158: // FORMAT_A2R10G10B10_UINT_PACK32
      case 159: // FORMAT_A2R10G10B10_SINT_PACK32
      case 160: // FORMAT_A2B10G10R10_UNORM_PACK32
      case 161: // FORMAT_A2B10G10R10_SNORM_PACK32
      case 162: // FORMAT_A2B10G10R10_USCALED_PACK32
      case 163: // FORMAT_A2B10G10R10_SSCALED_PACK32
      case 164: // FORMAT_A2B10G10R10_UINT_PACK32
      case 165: // FORMAT_A2B10G10R10_SINT_PACK32
      case 166: // FORMAT_R16_UNORM
      case 167: // FORMAT_R16_SNORM
      case 168: // FORMAT_R16_USCALED
      case 169: // FORMAT_R16_SSCALED
      case 170: // FORMAT_R16_UINT
      case 171: // FORMAT_R16_SINT
      case 172: // FORMAT_R16_SFLOAT
      case 173: // FORMAT_R16G16_UNORM
      case 174: // FORMAT_R16G16_SNORM
      case 175: // FORMAT_R16G16_USCALED
      case 176: // FORMAT_R16G16_SSCALED
      case 177: // FORMAT_R16G16_UINT
      case 178: // FORMAT_R16G16_SINT
      case 179: // FORMAT_R16G16_SFLOAT
      case 180: // FORMAT_R16G16B16_UNORM
      case 181: // FORMAT_R16G16B16_SNORM
      case 182: // FORMAT_R16G16B16_USCALED
      case 183: // FORMAT_R16G16B16_SSCALED
      case 184: // FORMAT_R16G16B16_UINT
      case 185: // FORMAT_R16G16B16_SINT
      case 186: // FORMAT_R16G16B16_SFLOAT
      case 187: // FORMAT_R16G16B16A16_UNORM
      case 188: // FORMAT_R16G16B16A16_SNORM
      case 189: // FORMAT_R16G16B16A16_USCALED
      case 190: // FORMAT_R16G16B16A16_SSCALED
      case 191: // FORMAT_R16G16B16A16_UINT
      case 192: // FORMAT_R16G16B16A16_SINT
      case 193: // FORMAT_R16G16B16A16_SFLOAT
      case 194: // FORMAT_R32_UINT
      case 195: // FORMAT_R32_SINT
      case 196: // FORMAT_R32_SFLOAT
      case 197: // FORMAT_R32G32_UINT
      case 198: // FORMAT_R32G32_SINT
      case 199: // FORMAT_R32G32_SFLOAT
      case 200: // FORMAT_R32G32B32_UINT
      case 201: // FORMAT_R32G32B32_SINT
      case 202: // FORMAT_R32G32B32_SFLOAT
      case 203: // FORMAT_R32G32B32A32_UINT
      case 204: // FORMAT_R32G32B32A32_SINT
      case 205: // FORMAT_R32G32B32A32_SFLOAT
      case 206: // FORMAT_R64_UINT
      case 207: // FORMAT_R64_SINT
      case 208: // FORMAT_R64_SFLOAT
      case 209: // FORMAT_R64G64_UINT
      case 210: // FORMAT_R64G64_SINT
      case 211: // FORMAT_R64G64_SFLOAT
      case 212: // FORMAT_R64G64B64_UINT
      case 213: // FORMAT_R64G64B64_SINT
      case 214: // FORMAT_R64G64B64_SFLOAT
      case 215: // FORMAT_R64G64B64A64_UINT
      case 216: // FORMAT_R64G64B64A64_SINT
      case 217: // FORMAT_R64G64B64A64_SFLOAT
      case 218: // FORMAT_B10G11R11_UFLOAT_PACK32
      case 219: // FORMAT_E5B9G9R9_UFLOAT_PACK32
      case 220: // FORMAT_D16_UNORM
      case 221: // FORMAT_X8_D24_UNORM_PACK32
      case 222: // FORMAT_D32_SFLOAT
      case 223: // FORMAT_S8_UINT
      case 224: // FORMAT_D16_UNORM_S8_UINT
      case 225: // FORMAT_D24_UNORM_S8_UINT
      case 226: // FORMAT_D32_SFLOAT_S8_UINT
      case 227: // FORMAT_BC1_RGB_UNORM_BLOCK
      case 228: // FORMAT_BC1_RGB_SRGB_BLOCK
      case 229: // FORMAT_BC1_RGBA_UNORM_BLOCK
      case 230: // FORMAT_BC1_RGBA_SRGB_BLOCK
      case 231: // FORMAT_BC2_UNORM_BLOCK
      case 232: // FORMAT_BC2_SRGB_BLOCK
      case 233: // FORMAT_BC3_UNORM_BLOCK
      case 234: // FORMAT_BC3_SRGB_BLOCK
      case 235: // FORMAT_BC4_UNORM_BLOCK
      case 236: // FORMAT_BC4_SNORM_BLOCK
      case 237: // FORMAT_BC5_UNORM_BLOCK
      case 238: // FORMAT_BC5_SNORM_BLOCK
      case 239: // FORMAT_BC6H_UFLOAT_BLOCK
      case 240: // FORMAT_BC6H_SFLOAT_BLOCK
      case 241: // FORMAT_BC7_UNORM_BLOCK
      case 242: // FORMAT_BC7_SRGB_BLOCK
      case 243: // FORMAT_ETC2_R8G8B8_UNORM_BLOCK
      case 244: // FORMAT_ETC2_R8G8B8_SRGB_BLOCK
      case 245: // FORMAT_ETC2_R8G8B8A1_UNORM_BLOCK
      case 246: // FORMAT_ETC2_R8G8B8A1_SRGB_BLOCK
      case 247: // FORMAT_ETC2_R8G8B8A8_UNORM_BLOCK
      case 248: // FORMAT_ETC2_R8G8B8A8_SRGB_BLOCK
      case 249: // FORMAT_EAC_R11_UNORM_BLOCK
      case 250: // FORMAT_EAC_R11_SNORM_BLOCK
      case 251: // FORMAT_EAC_R11G11_UNORM_BLOCK
      case 252: // FORMAT_EAC_R11G11_SNORM_BLOCK
      case 253: // FORMAT_ASTC_4x4_UNORM_BLOCK
      case 254: // FORMAT_ASTC_4x4_SRGB_BLOCK
      case 255: // FORMAT_ASTC_5x4_UNORM_BLOCK
      case 256: // FORMAT_ASTC_5x4_SRGB_BLOCK
      case 257: // FORMAT_ASTC_5x5_UNORM_BLOCK
      case 258: // FORMAT_ASTC_5x5_SRGB_BLOCK
      case 259: // FORMAT_ASTC_6x5_UNORM_BLOCK
      case 260: // FORMAT_ASTC_6x5_SRGB_BLOCK
      case 261: // FORMAT_ASTC_6x6_UNORM_BLOCK
      case 262: // FORMAT_ASTC_6x6_SRGB_BLOCK
      case 263: // FORMAT_ASTC_8x5_UNORM_BLOCK
      case 264: // FORMAT_ASTC_8x5_SRGB_BLOCK
      case 265: // FORMAT_ASTC_8x6_UNORM_BLOCK
      case 266: // FORMAT_ASTC_8x6_SRGB_BLOCK
      case 267: // FORMAT_ASTC_8x8_UNORM_BLOCK
      case 268: // FORMAT_ASTC_8x8_SRGB_BLOCK
      case 269: // FORMAT_ASTC_10x5_UNORM_BLOCK
      case 270: // FORMAT_ASTC_10x5_SRGB_BLOCK
      case 271: // FORMAT_ASTC_10x6_UNORM_BLOCK
      case 272: // FORMAT_ASTC_10x6_SRGB_BLOCK
      case 273: // FORMAT_ASTC_10x8_UNORM_BLOCK
      case 274: // FORMAT_ASTC_10x8_SRGB_BLOCK
      case 275: // FORMAT_ASTC_10x10_UNORM_BLOCK
      case 276: // FORMAT_ASTC_10x10_SRGB_BLOCK
      case 277: // FORMAT_ASTC_12x10_UNORM_BLOCK
      case 278: // FORMAT_ASTC_12x10_SRGB_BLOCK
      case 279: // FORMAT_ASTC_12x12_UNORM_BLOCK
      case 280: // FORMAT_ASTC_12x12_SRGB_BLOCK
      case 296: // format
        value.copy< VkFormat > (v);
        break;

      case 15: // TRUE
      case 16: // FALSE
      case 19: // BOOL
        value.copy< bool > (v);
        break;

      case 17: // FLOAT
        value.copy< float > (v);
        break;

      case 18: // INT
        value.copy< int > (v);
        break;

      case 14: // IDENTIFIER
      case 20: // STR
      case 32: // CODEBODY
      case 33: // VARNAME
        value.copy< std::string > (v);
        break;

      default:
        break;
    }
}


  // Implementation of basic_symbol constructor for each type.

  template <typename Base>
  parser::basic_symbol<Base>::basic_symbol (typename Base::kind_type t, const location_type& l)
    : Base (t)
    , value ()
    , location (l)
  {}

  template <typename Base>
  parser::basic_symbol<Base>::basic_symbol (typename Base::kind_type t, const VkFormat v, const location_type& l)
    : Base (t)
    , value (v)
    , location (l)
  {}

  template <typename Base>
  parser::basic_symbol<Base>::basic_symbol (typename Base::kind_type t, const bool v, const location_type& l)
    : Base (t)
    , value (v)
    , location (l)
  {}

  template <typename Base>
  parser::basic_symbol<Base>::basic_symbol (typename Base::kind_type t, const float v, const location_type& l)
    : Base (t)
    , value (v)
    , location (l)
  {}

  template <typename Base>
  parser::basic_symbol<Base>::basic_symbol (typename Base::kind_type t, const int v, const location_type& l)
    : Base (t)
    , value (v)
    , location (l)
  {}

  template <typename Base>
  parser::basic_symbol<Base>::basic_symbol (typename Base::kind_type t, const std::string v, const location_type& l)
    : Base (t)
    , value (v)
    , location (l)
  {}


  template <typename Base>
  inline
  parser::basic_symbol<Base>::~basic_symbol ()
  {
    clear ();
  }

  template <typename Base>
  inline
  void
  parser::basic_symbol<Base>::clear ()
  {
    // User destructor.
    symbol_number_type yytype = this->type_get ();
    basic_symbol<Base>& yysym = *this;
    (void) yysym;
    switch (yytype)
    {
   default:
      break;
    }

    // Type destructor.
    switch (yytype)
    {
      case 96: // FORMAT_UNDEFINED
      case 97: // FORMAT_R4G4_UNORM_PACK8
      case 98: // FORMAT_R4G4B4A4_UNORM_PACK16
      case 99: // FORMAT_B4G4R4A4_UNORM_PACK16
      case 100: // FORMAT_R5G6B5_UNORM_PACK16
      case 101: // FORMAT_B5G6R5_UNORM_PACK16
      case 102: // FORMAT_R5G5B5A1_UNORM_PACK16
      case 103: // FORMAT_B5G5R5A1_UNORM_PACK16
      case 104: // FORMAT_A1R5G5B5_UNORM_PACK16
      case 105: // FORMAT_R8_UNORM
      case 106: // FORMAT_R8_SNORM
      case 107: // FORMAT_R8_USCALED
      case 108: // FORMAT_R8_SSCALED
      case 109: // FORMAT_R8_UINT
      case 110: // FORMAT_R8_SINT
      case 111: // FORMAT_R8_SRGB
      case 112: // FORMAT_R8G8_UNORM
      case 113: // FORMAT_R8G8_SNORM
      case 114: // FORMAT_R8G8_USCALED
      case 115: // FORMAT_R8G8_SSCALED
      case 116: // FORMAT_R8G8_UINT
      case 117: // FORMAT_R8G8_SINT
      case 118: // FORMAT_R8G8_SRGB
      case 119: // FORMAT_R8G8B8_UNORM
      case 120: // FORMAT_R8G8B8_SNORM
      case 121: // FORMAT_R8G8B8_USCALED
      case 122: // FORMAT_R8G8B8_SSCALED
      case 123: // FORMAT_R8G8B8_UINT
      case 124: // FORMAT_R8G8B8_SINT
      case 125: // FORMAT_R8G8B8_SRGB
      case 126: // FORMAT_B8G8R8_UNORM
      case 127: // FORMAT_B8G8R8_SNORM
      case 128: // FORMAT_B8G8R8_USCALED
      case 129: // FORMAT_B8G8R8_SSCALED
      case 130: // FORMAT_B8G8R8_UINT
      case 131: // FORMAT_B8G8R8_SINT
      case 132: // FORMAT_B8G8R8_SRGB
      case 133: // FORMAT_R8G8B8A8_UNORM
      case 134: // FORMAT_R8G8B8A8_SNORM
      case 135: // FORMAT_R8G8B8A8_USCALED
      case 136: // FORMAT_R8G8B8A8_SSCALED
      case 137: // FORMAT_R8G8B8A8_UINT
      case 138: // FORMAT_R8G8B8A8_SINT
      case 139: // FORMAT_R8G8B8A8_SRGB
      case 140: // FORMAT_B8G8R8A8_UNORM
      case 141: // FORMAT_B8G8R8A8_SNORM
      case 142: // FORMAT_B8G8R8A8_USCALED
      case 143: // FORMAT_B8G8R8A8_SSCALED
      case 144: // FORMAT_B8G8R8A8_UINT
      case 145: // FORMAT_B8G8R8A8_SINT
      case 146: // FORMAT_B8G8R8A8_SRGB
      case 147: // FORMAT_A8B8G8R8_UNORM_PACK32
      case 148: // FORMAT_A8B8G8R8_SNORM_PACK32
      case 149: // FORMAT_A8B8G8R8_USCALED_PACK32
      case 150: // FORMAT_A8B8G8R8_SSCALED_PACK32
      case 151: // FORMAT_A8B8G8R8_UINT_PACK32
      case 152: // FORMAT_A8B8G8R8_SINT_PACK32
      case 153: // FORMAT_A8B8G8R8_SRGB_PACK32
      case 154: // FORMAT_A2R10G10B10_UNORM_PACK32
      case 155: // FORMAT_A2R10G10B10_SNORM_PACK32
      case 156: // FORMAT_A2R10G10B10_USCALED_PACK32
      case 157: // FORMAT_A2R10G10B10_SSCALED_PACK32
      case 158: // FORMAT_A2R10G10B10_UINT_PACK32
      case 159: // FORMAT_A2R10G10B10_SINT_PACK32
      case 160: // FORMAT_A2B10G10R10_UNORM_PACK32
      case 161: // FORMAT_A2B10G10R10_SNORM_PACK32
      case 162: // FORMAT_A2B10G10R10_USCALED_PACK32
      case 163: // FORMAT_A2B10G10R10_SSCALED_PACK32
      case 164: // FORMAT_A2B10G10R10_UINT_PACK32
      case 165: // FORMAT_A2B10G10R10_SINT_PACK32
      case 166: // FORMAT_R16_UNORM
      case 167: // FORMAT_R16_SNORM
      case 168: // FORMAT_R16_USCALED
      case 169: // FORMAT_R16_SSCALED
      case 170: // FORMAT_R16_UINT
      case 171: // FORMAT_R16_SINT
      case 172: // FORMAT_R16_SFLOAT
      case 173: // FORMAT_R16G16_UNORM
      case 174: // FORMAT_R16G16_SNORM
      case 175: // FORMAT_R16G16_USCALED
      case 176: // FORMAT_R16G16_SSCALED
      case 177: // FORMAT_R16G16_UINT
      case 178: // FORMAT_R16G16_SINT
      case 179: // FORMAT_R16G16_SFLOAT
      case 180: // FORMAT_R16G16B16_UNORM
      case 181: // FORMAT_R16G16B16_SNORM
      case 182: // FORMAT_R16G16B16_USCALED
      case 183: // FORMAT_R16G16B16_SSCALED
      case 184: // FORMAT_R16G16B16_UINT
      case 185: // FORMAT_R16G16B16_SINT
      case 186: // FORMAT_R16G16B16_SFLOAT
      case 187: // FORMAT_R16G16B16A16_UNORM
      case 188: // FORMAT_R16G16B16A16_SNORM
      case 189: // FORMAT_R16G16B16A16_USCALED
      case 190: // FORMAT_R16G16B16A16_SSCALED
      case 191: // FORMAT_R16G16B16A16_UINT
      case 192: // FORMAT_R16G16B16A16_SINT
      case 193: // FORMAT_R16G16B16A16_SFLOAT
      case 194: // FORMAT_R32_UINT
      case 195: // FORMAT_R32_SINT
      case 196: // FORMAT_R32_SFLOAT
      case 197: // FORMAT_R32G32_UINT
      case 198: // FORMAT_R32G32_SINT
      case 199: // FORMAT_R32G32_SFLOAT
      case 200: // FORMAT_R32G32B32_UINT
      case 201: // FORMAT_R32G32B32_SINT
      case 202: // FORMAT_R32G32B32_SFLOAT
      case 203: // FORMAT_R32G32B32A32_UINT
      case 204: // FORMAT_R32G32B32A32_SINT
      case 205: // FORMAT_R32G32B32A32_SFLOAT
      case 206: // FORMAT_R64_UINT
      case 207: // FORMAT_R64_SINT
      case 208: // FORMAT_R64_SFLOAT
      case 209: // FORMAT_R64G64_UINT
      case 210: // FORMAT_R64G64_SINT
      case 211: // FORMAT_R64G64_SFLOAT
      case 212: // FORMAT_R64G64B64_UINT
      case 213: // FORMAT_R64G64B64_SINT
      case 214: // FORMAT_R64G64B64_SFLOAT
      case 215: // FORMAT_R64G64B64A64_UINT
      case 216: // FORMAT_R64G64B64A64_SINT
      case 217: // FORMAT_R64G64B64A64_SFLOAT
      case 218: // FORMAT_B10G11R11_UFLOAT_PACK32
      case 219: // FORMAT_E5B9G9R9_UFLOAT_PACK32
      case 220: // FORMAT_D16_UNORM
      case 221: // FORMAT_X8_D24_UNORM_PACK32
      case 222: // FORMAT_D32_SFLOAT
      case 223: // FORMAT_S8_UINT
      case 224: // FORMAT_D16_UNORM_S8_UINT
      case 225: // FORMAT_D24_UNORM_S8_UINT
      case 226: // FORMAT_D32_SFLOAT_S8_UINT
      case 227: // FORMAT_BC1_RGB_UNORM_BLOCK
      case 228: // FORMAT_BC1_RGB_SRGB_BLOCK
      case 229: // FORMAT_BC1_RGBA_UNORM_BLOCK
      case 230: // FORMAT_BC1_RGBA_SRGB_BLOCK
      case 231: // FORMAT_BC2_UNORM_BLOCK
      case 232: // FORMAT_BC2_SRGB_BLOCK
      case 233: // FORMAT_BC3_UNORM_BLOCK
      case 234: // FORMAT_BC3_SRGB_BLOCK
      case 235: // FORMAT_BC4_UNORM_BLOCK
      case 236: // FORMAT_BC4_SNORM_BLOCK
      case 237: // FORMAT_BC5_UNORM_BLOCK
      case 238: // FORMAT_BC5_SNORM_BLOCK
      case 239: // FORMAT_BC6H_UFLOAT_BLOCK
      case 240: // FORMAT_BC6H_SFLOAT_BLOCK
      case 241: // FORMAT_BC7_UNORM_BLOCK
      case 242: // FORMAT_BC7_SRGB_BLOCK
      case 243: // FORMAT_ETC2_R8G8B8_UNORM_BLOCK
      case 244: // FORMAT_ETC2_R8G8B8_SRGB_BLOCK
      case 245: // FORMAT_ETC2_R8G8B8A1_UNORM_BLOCK
      case 246: // FORMAT_ETC2_R8G8B8A1_SRGB_BLOCK
      case 247: // FORMAT_ETC2_R8G8B8A8_UNORM_BLOCK
      case 248: // FORMAT_ETC2_R8G8B8A8_SRGB_BLOCK
      case 249: // FORMAT_EAC_R11_UNORM_BLOCK
      case 250: // FORMAT_EAC_R11_SNORM_BLOCK
      case 251: // FORMAT_EAC_R11G11_UNORM_BLOCK
      case 252: // FORMAT_EAC_R11G11_SNORM_BLOCK
      case 253: // FORMAT_ASTC_4x4_UNORM_BLOCK
      case 254: // FORMAT_ASTC_4x4_SRGB_BLOCK
      case 255: // FORMAT_ASTC_5x4_UNORM_BLOCK
      case 256: // FORMAT_ASTC_5x4_SRGB_BLOCK
      case 257: // FORMAT_ASTC_5x5_UNORM_BLOCK
      case 258: // FORMAT_ASTC_5x5_SRGB_BLOCK
      case 259: // FORMAT_ASTC_6x5_UNORM_BLOCK
      case 260: // FORMAT_ASTC_6x5_SRGB_BLOCK
      case 261: // FORMAT_ASTC_6x6_UNORM_BLOCK
      case 262: // FORMAT_ASTC_6x6_SRGB_BLOCK
      case 263: // FORMAT_ASTC_8x5_UNORM_BLOCK
      case 264: // FORMAT_ASTC_8x5_SRGB_BLOCK
      case 265: // FORMAT_ASTC_8x6_UNORM_BLOCK
      case 266: // FORMAT_ASTC_8x6_SRGB_BLOCK
      case 267: // FORMAT_ASTC_8x8_UNORM_BLOCK
      case 268: // FORMAT_ASTC_8x8_SRGB_BLOCK
      case 269: // FORMAT_ASTC_10x5_UNORM_BLOCK
      case 270: // FORMAT_ASTC_10x5_SRGB_BLOCK
      case 271: // FORMAT_ASTC_10x6_UNORM_BLOCK
      case 272: // FORMAT_ASTC_10x6_SRGB_BLOCK
      case 273: // FORMAT_ASTC_10x8_UNORM_BLOCK
      case 274: // FORMAT_ASTC_10x8_SRGB_BLOCK
      case 275: // FORMAT_ASTC_10x10_UNORM_BLOCK
      case 276: // FORMAT_ASTC_10x10_SRGB_BLOCK
      case 277: // FORMAT_ASTC_12x10_UNORM_BLOCK
      case 278: // FORMAT_ASTC_12x10_SRGB_BLOCK
      case 279: // FORMAT_ASTC_12x12_UNORM_BLOCK
      case 280: // FORMAT_ASTC_12x12_SRGB_BLOCK
      case 296: // format
        value.template destroy< VkFormat > ();
        break;

      case 15: // TRUE
      case 16: // FALSE
      case 19: // BOOL
        value.template destroy< bool > ();
        break;

      case 17: // FLOAT
        value.template destroy< float > ();
        break;

      case 18: // INT
        value.template destroy< int > ();
        break;

      case 14: // IDENTIFIER
      case 20: // STR
      case 32: // CODEBODY
      case 33: // VARNAME
        value.template destroy< std::string > ();
        break;

      default:
        break;
    }

    Base::clear ();
  }

  template <typename Base>
  inline
  bool
  parser::basic_symbol<Base>::empty () const
  {
    return Base::type_get () == empty_symbol;
  }

  template <typename Base>
  inline
  void
  parser::basic_symbol<Base>::move (basic_symbol& s)
  {
    super_type::move(s);
      switch (this->type_get ())
    {
      case 96: // FORMAT_UNDEFINED
      case 97: // FORMAT_R4G4_UNORM_PACK8
      case 98: // FORMAT_R4G4B4A4_UNORM_PACK16
      case 99: // FORMAT_B4G4R4A4_UNORM_PACK16
      case 100: // FORMAT_R5G6B5_UNORM_PACK16
      case 101: // FORMAT_B5G6R5_UNORM_PACK16
      case 102: // FORMAT_R5G5B5A1_UNORM_PACK16
      case 103: // FORMAT_B5G5R5A1_UNORM_PACK16
      case 104: // FORMAT_A1R5G5B5_UNORM_PACK16
      case 105: // FORMAT_R8_UNORM
      case 106: // FORMAT_R8_SNORM
      case 107: // FORMAT_R8_USCALED
      case 108: // FORMAT_R8_SSCALED
      case 109: // FORMAT_R8_UINT
      case 110: // FORMAT_R8_SINT
      case 111: // FORMAT_R8_SRGB
      case 112: // FORMAT_R8G8_UNORM
      case 113: // FORMAT_R8G8_SNORM
      case 114: // FORMAT_R8G8_USCALED
      case 115: // FORMAT_R8G8_SSCALED
      case 116: // FORMAT_R8G8_UINT
      case 117: // FORMAT_R8G8_SINT
      case 118: // FORMAT_R8G8_SRGB
      case 119: // FORMAT_R8G8B8_UNORM
      case 120: // FORMAT_R8G8B8_SNORM
      case 121: // FORMAT_R8G8B8_USCALED
      case 122: // FORMAT_R8G8B8_SSCALED
      case 123: // FORMAT_R8G8B8_UINT
      case 124: // FORMAT_R8G8B8_SINT
      case 125: // FORMAT_R8G8B8_SRGB
      case 126: // FORMAT_B8G8R8_UNORM
      case 127: // FORMAT_B8G8R8_SNORM
      case 128: // FORMAT_B8G8R8_USCALED
      case 129: // FORMAT_B8G8R8_SSCALED
      case 130: // FORMAT_B8G8R8_UINT
      case 131: // FORMAT_B8G8R8_SINT
      case 132: // FORMAT_B8G8R8_SRGB
      case 133: // FORMAT_R8G8B8A8_UNORM
      case 134: // FORMAT_R8G8B8A8_SNORM
      case 135: // FORMAT_R8G8B8A8_USCALED
      case 136: // FORMAT_R8G8B8A8_SSCALED
      case 137: // FORMAT_R8G8B8A8_UINT
      case 138: // FORMAT_R8G8B8A8_SINT
      case 139: // FORMAT_R8G8B8A8_SRGB
      case 140: // FORMAT_B8G8R8A8_UNORM
      case 141: // FORMAT_B8G8R8A8_SNORM
      case 142: // FORMAT_B8G8R8A8_USCALED
      case 143: // FORMAT_B8G8R8A8_SSCALED
      case 144: // FORMAT_B8G8R8A8_UINT
      case 145: // FORMAT_B8G8R8A8_SINT
      case 146: // FORMAT_B8G8R8A8_SRGB
      case 147: // FORMAT_A8B8G8R8_UNORM_PACK32
      case 148: // FORMAT_A8B8G8R8_SNORM_PACK32
      case 149: // FORMAT_A8B8G8R8_USCALED_PACK32
      case 150: // FORMAT_A8B8G8R8_SSCALED_PACK32
      case 151: // FORMAT_A8B8G8R8_UINT_PACK32
      case 152: // FORMAT_A8B8G8R8_SINT_PACK32
      case 153: // FORMAT_A8B8G8R8_SRGB_PACK32
      case 154: // FORMAT_A2R10G10B10_UNORM_PACK32
      case 155: // FORMAT_A2R10G10B10_SNORM_PACK32
      case 156: // FORMAT_A2R10G10B10_USCALED_PACK32
      case 157: // FORMAT_A2R10G10B10_SSCALED_PACK32
      case 158: // FORMAT_A2R10G10B10_UINT_PACK32
      case 159: // FORMAT_A2R10G10B10_SINT_PACK32
      case 160: // FORMAT_A2B10G10R10_UNORM_PACK32
      case 161: // FORMAT_A2B10G10R10_SNORM_PACK32
      case 162: // FORMAT_A2B10G10R10_USCALED_PACK32
      case 163: // FORMAT_A2B10G10R10_SSCALED_PACK32
      case 164: // FORMAT_A2B10G10R10_UINT_PACK32
      case 165: // FORMAT_A2B10G10R10_SINT_PACK32
      case 166: // FORMAT_R16_UNORM
      case 167: // FORMAT_R16_SNORM
      case 168: // FORMAT_R16_USCALED
      case 169: // FORMAT_R16_SSCALED
      case 170: // FORMAT_R16_UINT
      case 171: // FORMAT_R16_SINT
      case 172: // FORMAT_R16_SFLOAT
      case 173: // FORMAT_R16G16_UNORM
      case 174: // FORMAT_R16G16_SNORM
      case 175: // FORMAT_R16G16_USCALED
      case 176: // FORMAT_R16G16_SSCALED
      case 177: // FORMAT_R16G16_UINT
      case 178: // FORMAT_R16G16_SINT
      case 179: // FORMAT_R16G16_SFLOAT
      case 180: // FORMAT_R16G16B16_UNORM
      case 181: // FORMAT_R16G16B16_SNORM
      case 182: // FORMAT_R16G16B16_USCALED
      case 183: // FORMAT_R16G16B16_SSCALED
      case 184: // FORMAT_R16G16B16_UINT
      case 185: // FORMAT_R16G16B16_SINT
      case 186: // FORMAT_R16G16B16_SFLOAT
      case 187: // FORMAT_R16G16B16A16_UNORM
      case 188: // FORMAT_R16G16B16A16_SNORM
      case 189: // FORMAT_R16G16B16A16_USCALED
      case 190: // FORMAT_R16G16B16A16_SSCALED
      case 191: // FORMAT_R16G16B16A16_UINT
      case 192: // FORMAT_R16G16B16A16_SINT
      case 193: // FORMAT_R16G16B16A16_SFLOAT
      case 194: // FORMAT_R32_UINT
      case 195: // FORMAT_R32_SINT
      case 196: // FORMAT_R32_SFLOAT
      case 197: // FORMAT_R32G32_UINT
      case 198: // FORMAT_R32G32_SINT
      case 199: // FORMAT_R32G32_SFLOAT
      case 200: // FORMAT_R32G32B32_UINT
      case 201: // FORMAT_R32G32B32_SINT
      case 202: // FORMAT_R32G32B32_SFLOAT
      case 203: // FORMAT_R32G32B32A32_UINT
      case 204: // FORMAT_R32G32B32A32_SINT
      case 205: // FORMAT_R32G32B32A32_SFLOAT
      case 206: // FORMAT_R64_UINT
      case 207: // FORMAT_R64_SINT
      case 208: // FORMAT_R64_SFLOAT
      case 209: // FORMAT_R64G64_UINT
      case 210: // FORMAT_R64G64_SINT
      case 211: // FORMAT_R64G64_SFLOAT
      case 212: // FORMAT_R64G64B64_UINT
      case 213: // FORMAT_R64G64B64_SINT
      case 214: // FORMAT_R64G64B64_SFLOAT
      case 215: // FORMAT_R64G64B64A64_UINT
      case 216: // FORMAT_R64G64B64A64_SINT
      case 217: // FORMAT_R64G64B64A64_SFLOAT
      case 218: // FORMAT_B10G11R11_UFLOAT_PACK32
      case 219: // FORMAT_E5B9G9R9_UFLOAT_PACK32
      case 220: // FORMAT_D16_UNORM
      case 221: // FORMAT_X8_D24_UNORM_PACK32
      case 222: // FORMAT_D32_SFLOAT
      case 223: // FORMAT_S8_UINT
      case 224: // FORMAT_D16_UNORM_S8_UINT
      case 225: // FORMAT_D24_UNORM_S8_UINT
      case 226: // FORMAT_D32_SFLOAT_S8_UINT
      case 227: // FORMAT_BC1_RGB_UNORM_BLOCK
      case 228: // FORMAT_BC1_RGB_SRGB_BLOCK
      case 229: // FORMAT_BC1_RGBA_UNORM_BLOCK
      case 230: // FORMAT_BC1_RGBA_SRGB_BLOCK
      case 231: // FORMAT_BC2_UNORM_BLOCK
      case 232: // FORMAT_BC2_SRGB_BLOCK
      case 233: // FORMAT_BC3_UNORM_BLOCK
      case 234: // FORMAT_BC3_SRGB_BLOCK
      case 235: // FORMAT_BC4_UNORM_BLOCK
      case 236: // FORMAT_BC4_SNORM_BLOCK
      case 237: // FORMAT_BC5_UNORM_BLOCK
      case 238: // FORMAT_BC5_SNORM_BLOCK
      case 239: // FORMAT_BC6H_UFLOAT_BLOCK
      case 240: // FORMAT_BC6H_SFLOAT_BLOCK
      case 241: // FORMAT_BC7_UNORM_BLOCK
      case 242: // FORMAT_BC7_SRGB_BLOCK
      case 243: // FORMAT_ETC2_R8G8B8_UNORM_BLOCK
      case 244: // FORMAT_ETC2_R8G8B8_SRGB_BLOCK
      case 245: // FORMAT_ETC2_R8G8B8A1_UNORM_BLOCK
      case 246: // FORMAT_ETC2_R8G8B8A1_SRGB_BLOCK
      case 247: // FORMAT_ETC2_R8G8B8A8_UNORM_BLOCK
      case 248: // FORMAT_ETC2_R8G8B8A8_SRGB_BLOCK
      case 249: // FORMAT_EAC_R11_UNORM_BLOCK
      case 250: // FORMAT_EAC_R11_SNORM_BLOCK
      case 251: // FORMAT_EAC_R11G11_UNORM_BLOCK
      case 252: // FORMAT_EAC_R11G11_SNORM_BLOCK
      case 253: // FORMAT_ASTC_4x4_UNORM_BLOCK
      case 254: // FORMAT_ASTC_4x4_SRGB_BLOCK
      case 255: // FORMAT_ASTC_5x4_UNORM_BLOCK
      case 256: // FORMAT_ASTC_5x4_SRGB_BLOCK
      case 257: // FORMAT_ASTC_5x5_UNORM_BLOCK
      case 258: // FORMAT_ASTC_5x5_SRGB_BLOCK
      case 259: // FORMAT_ASTC_6x5_UNORM_BLOCK
      case 260: // FORMAT_ASTC_6x5_SRGB_BLOCK
      case 261: // FORMAT_ASTC_6x6_UNORM_BLOCK
      case 262: // FORMAT_ASTC_6x6_SRGB_BLOCK
      case 263: // FORMAT_ASTC_8x5_UNORM_BLOCK
      case 264: // FORMAT_ASTC_8x5_SRGB_BLOCK
      case 265: // FORMAT_ASTC_8x6_UNORM_BLOCK
      case 266: // FORMAT_ASTC_8x6_SRGB_BLOCK
      case 267: // FORMAT_ASTC_8x8_UNORM_BLOCK
      case 268: // FORMAT_ASTC_8x8_SRGB_BLOCK
      case 269: // FORMAT_ASTC_10x5_UNORM_BLOCK
      case 270: // FORMAT_ASTC_10x5_SRGB_BLOCK
      case 271: // FORMAT_ASTC_10x6_UNORM_BLOCK
      case 272: // FORMAT_ASTC_10x6_SRGB_BLOCK
      case 273: // FORMAT_ASTC_10x8_UNORM_BLOCK
      case 274: // FORMAT_ASTC_10x8_SRGB_BLOCK
      case 275: // FORMAT_ASTC_10x10_UNORM_BLOCK
      case 276: // FORMAT_ASTC_10x10_SRGB_BLOCK
      case 277: // FORMAT_ASTC_12x10_UNORM_BLOCK
      case 278: // FORMAT_ASTC_12x10_SRGB_BLOCK
      case 279: // FORMAT_ASTC_12x12_UNORM_BLOCK
      case 280: // FORMAT_ASTC_12x12_SRGB_BLOCK
      case 296: // format
        value.move< VkFormat > (s.value);
        break;

      case 15: // TRUE
      case 16: // FALSE
      case 19: // BOOL
        value.move< bool > (s.value);
        break;

      case 17: // FLOAT
        value.move< float > (s.value);
        break;

      case 18: // INT
        value.move< int > (s.value);
        break;

      case 14: // IDENTIFIER
      case 20: // STR
      case 32: // CODEBODY
      case 33: // VARNAME
        value.move< std::string > (s.value);
        break;

      default:
        break;
    }

    location = s.location;
  }

  // by_type.
  inline
  parser::by_type::by_type ()
    : type (empty_symbol)
  {}

  inline
  parser::by_type::by_type (const by_type& other)
    : type (other.type)
  {}

  inline
  parser::by_type::by_type (token_type t)
    : type (yytranslate_ (t))
  {}

  inline
  void
  parser::by_type::clear ()
  {
    type = empty_symbol;
  }

  inline
  void
  parser::by_type::move (by_type& that)
  {
    type = that.type;
    that.clear ();
  }

  inline
  int
  parser::by_type::type_get () const
  {
    return type;
  }

  inline
  parser::token_type
  parser::by_type::token () const
  {
    // YYTOKNUM[NUM] -- (External) token number corresponding to the
    // (internal) symbol number NUM (which must be that of a token).  */
    static
    const unsigned short int
    yytoken_number_[] =
    {
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,   292,   293,   294,
     295,   296,   297,   298,   299,   300,   301,   302,   303,   304,
     305,   306,   307,   308,   309,   310,   311,   312,   313,   314,
     315,   316,   317,   318,   319,   320,   321,   322,   323,   324,
     325,   326,   327,   328,   329,   330,   331,   332,   333,   334,
     335,   336,   337,   338,   339,   340,   341,   342,   343,   344,
     345,   346,   347,   348,   349,   350,   351,   352,   353,   354,
     355,   356,   357,   358,   359,   360,   361,   362,   363,   364,
     365,   366,   367,   368,   369,   370,   371,   372,   373,   374,
     375,   376,   377,   378,   379,   380,   381,   382,   383,   384,
     385,   386,   387,   388,   389,   390,   391,   392,   393,   394,
     395,   396,   397,   398,   399,   400,   401,   402,   403,   404,
     405,   406,   407,   408,   409,   410,   411,   412,   413,   414,
     415,   416,   417,   418,   419,   420,   421,   422,   423,   424,
     425,   426,   427,   428,   429,   430,   431,   432,   433,   434,
     435,   436,   437,   438,   439,   440,   441,   442,   443,   444,
     445,   446,   447,   448,   449,   450,   451,   452,   453,   454,
     455,   456,   457,   458,   459,   460,   461,   462,   463,   464,
     465,   466,   467,   468,   469,   470,   471,   472,   473,   474,
     475,   476,   477,   478,   479,   480,   481,   482,   483,   484,
     485,   486,   487,   488,   489,   490,   491,   492,   493,   494,
     495,   496,   497,   498,   499,   500,   501,   502,   503,   504,
     505,   506,   507,   508,   509,   510,   511,   512,   513,   514,
     515,   516,   517,   518,   519,   520,   521,   522,   523,   524,
     525,   526,   527,   528,   529,   530,   531,   532,   533,   534,
     535,   536,   537,   538,   539,   540,   541,   542,   543,   544,
     545,   546,   547,   548,   549
    };
    return static_cast<token_type> (yytoken_number_[type]);
  }
  // Implementation of make_symbol for each symbol type.
  parser::symbol_type
  parser::make_END (const location_type& l)
  {
    return symbol_type (token::TOK_END, l);
  }

  parser::symbol_type
  parser::make_ASSIGN (const location_type& l)
  {
    return symbol_type (token::TOK_ASSIGN, l);
  }

  parser::symbol_type
  parser::make_MINUS (const location_type& l)
  {
    return symbol_type (token::TOK_MINUS, l);
  }

  parser::symbol_type
  parser::make_PLUS (const location_type& l)
  {
    return symbol_type (token::TOK_PLUS, l);
  }

  parser::symbol_type
  parser::make_STAR (const location_type& l)
  {
    return symbol_type (token::TOK_STAR, l);
  }

  parser::symbol_type
  parser::make_SLASH (const location_type& l)
  {
    return symbol_type (token::TOK_SLASH, l);
  }

  parser::symbol_type
  parser::make_LPAREN (const location_type& l)
  {
    return symbol_type (token::TOK_LPAREN, l);
  }

  parser::symbol_type
  parser::make_RPAREN (const location_type& l)
  {
    return symbol_type (token::TOK_RPAREN, l);
  }

  parser::symbol_type
  parser::make_LEFTSCOPE (const location_type& l)
  {
    return symbol_type (token::TOK_LEFTSCOPE, l);
  }

  parser::symbol_type
  parser::make_RIGHTSCOPE (const location_type& l)
  {
    return symbol_type (token::TOK_RIGHTSCOPE, l);
  }

  parser::symbol_type
  parser::make_SEMICOLON (const location_type& l)
  {
    return symbol_type (token::TOK_SEMICOLON, l);
  }

  parser::symbol_type
  parser::make_COMMA (const location_type& l)
  {
    return symbol_type (token::TOK_COMMA, l);
  }

  parser::symbol_type
  parser::make_IDENTIFIER (const std::string& v, const location_type& l)
  {
    return symbol_type (token::TOK_IDENTIFIER, v, l);
  }

  parser::symbol_type
  parser::make_TRUE (const bool& v, const location_type& l)
  {
    return symbol_type (token::TOK_TRUE, v, l);
  }

  parser::symbol_type
  parser::make_FALSE (const bool& v, const location_type& l)
  {
    return symbol_type (token::TOK_FALSE, v, l);
  }

  parser::symbol_type
  parser::make_FLOAT (const float& v, const location_type& l)
  {
    return symbol_type (token::TOK_FLOAT, v, l);
  }

  parser::symbol_type
  parser::make_INT (const int& v, const location_type& l)
  {
    return symbol_type (token::TOK_INT, v, l);
  }

  parser::symbol_type
  parser::make_BOOL (const bool& v, const location_type& l)
  {
    return symbol_type (token::TOK_BOOL, v, l);
  }

  parser::symbol_type
  parser::make_STR (const std::string& v, const location_type& l)
  {
    return symbol_type (token::TOK_STR, v, l);
  }

  parser::symbol_type
  parser::make_GLSLSHADER (const location_type& l)
  {
    return symbol_type (token::TOK_GLSLSHADER, l);
  }

  parser::symbol_type
  parser::make_HLSL10SHADER (const location_type& l)
  {
    return symbol_type (token::TOK_HLSL10SHADER, l);
  }

  parser::symbol_type
  parser::make_HLSL11SHADER (const location_type& l)
  {
    return symbol_type (token::TOK_HLSL11SHADER, l);
  }

  parser::symbol_type
  parser::make_CGSHADER (const location_type& l)
  {
    return symbol_type (token::TOK_CGSHADER, l);
  }

  parser::symbol_type
  parser::make_SAMPLER_STATE (const location_type& l)
  {
    return symbol_type (token::TOK_SAMPLER_STATE, l);
  }

  parser::symbol_type
  parser::make_DST_STATE (const location_type& l)
  {
    return symbol_type (token::TOK_DST_STATE, l);
  }

  parser::symbol_type
  parser::make_PR_STATE (const location_type& l)
  {
    return symbol_type (token::TOK_PR_STATE, l);
  }

  parser::symbol_type
  parser::make_COLOR_SAMPLE_STATE (const location_type& l)
  {
    return symbol_type (token::TOK_COLOR_SAMPLE_STATE, l);
  }

  parser::symbol_type
  parser::make_RASTERIZATION_STATE (const location_type& l)
  {
    return symbol_type (token::TOK_RASTERIZATION_STATE, l);
  }

  parser::symbol_type
  parser::make_TECHNIQUE (const location_type& l)
  {
    return symbol_type (token::TOK_TECHNIQUE, l);
  }

  parser::symbol_type
  parser::make_PASS (const location_type& l)
  {
    return symbol_type (token::TOK_PASS, l);
  }

  parser::symbol_type
  parser::make_CODEBODY (const std::string& v, const location_type& l)
  {
    return symbol_type (token::TOK_CODEBODY, v, l);
  }

  parser::symbol_type
  parser::make_VARNAME (const std::string& v, const location_type& l)
  {
    return symbol_type (token::TOK_VARNAME, v, l);
  }

  parser::symbol_type
  parser::make_TEXTURERESOURCE (const location_type& l)
  {
    return symbol_type (token::TOK_TEXTURERESOURCE, l);
  }

  parser::symbol_type
  parser::make_TEXTURERESOURCE1D (const location_type& l)
  {
    return symbol_type (token::TOK_TEXTURERESOURCE1D, l);
  }

  parser::symbol_type
  parser::make_TEXTURERESOURCE2D (const location_type& l)
  {
    return symbol_type (token::TOK_TEXTURERESOURCE2D, l);
  }

  parser::symbol_type
  parser::make_TEXTURERESOURCE3D (const location_type& l)
  {
    return symbol_type (token::TOK_TEXTURERESOURCE3D, l);
  }

  parser::symbol_type
  parser::make_TEXTURERESOURCERECT (const location_type& l)
  {
    return symbol_type (token::TOK_TEXTURERESOURCERECT, l);
  }

  parser::symbol_type
  parser::make_TEXTURERESOURCECUBE (const location_type& l)
  {
    return symbol_type (token::TOK_TEXTURERESOURCECUBE, l);
  }

  parser::symbol_type
  parser::make_VOID_TYPE (const location_type& l)
  {
    return symbol_type (token::TOK_VOID_TYPE, l);
  }

  parser::symbol_type
  parser::make_UNSIGNED (const location_type& l)
  {
    return symbol_type (token::TOK_UNSIGNED, l);
  }

  parser::symbol_type
  parser::make_HIGHP (const location_type& l)
  {
    return symbol_type (token::TOK_HIGHP, l);
  }

  parser::symbol_type
  parser::make_MEDIUMP (const location_type& l)
  {
    return symbol_type (token::TOK_MEDIUMP, l);
  }

  parser::symbol_type
  parser::make_LOWP (const location_type& l)
  {
    return symbol_type (token::TOK_LOWP, l);
  }

  parser::symbol_type
  parser::make_UNIFORM (const location_type& l)
  {
    return symbol_type (token::TOK_UNIFORM, l);
  }

  parser::symbol_type
  parser::make_CSTBUFFER (const location_type& l)
  {
    return symbol_type (token::TOK_CSTBUFFER, l);
  }

  parser::symbol_type
  parser::make_FLOAT_TYPE (const location_type& l)
  {
    return symbol_type (token::TOK_FLOAT_TYPE, l);
  }

  parser::symbol_type
  parser::make_FLOAT2_TYPE (const location_type& l)
  {
    return symbol_type (token::TOK_FLOAT2_TYPE, l);
  }

  parser::symbol_type
  parser::make_FLOAT3_TYPE (const location_type& l)
  {
    return symbol_type (token::TOK_FLOAT3_TYPE, l);
  }

  parser::symbol_type
  parser::make_FLOAT4_TYPE (const location_type& l)
  {
    return symbol_type (token::TOK_FLOAT4_TYPE, l);
  }

  parser::symbol_type
  parser::make_MAT2_TYPE (const location_type& l)
  {
    return symbol_type (token::TOK_MAT2_TYPE, l);
  }

  parser::symbol_type
  parser::make_MAT3_TYPE (const location_type& l)
  {
    return symbol_type (token::TOK_MAT3_TYPE, l);
  }

  parser::symbol_type
  parser::make_MAT4_TYPE (const location_type& l)
  {
    return symbol_type (token::TOK_MAT4_TYPE, l);
  }

  parser::symbol_type
  parser::make_BOOL_TYPE (const location_type& l)
  {
    return symbol_type (token::TOK_BOOL_TYPE, l);
  }

  parser::symbol_type
  parser::make_BOOL2_TYPE (const location_type& l)
  {
    return symbol_type (token::TOK_BOOL2_TYPE, l);
  }

  parser::symbol_type
  parser::make_BOOL3_TYPE (const location_type& l)
  {
    return symbol_type (token::TOK_BOOL3_TYPE, l);
  }

  parser::symbol_type
  parser::make_BOOL4_TYPE (const location_type& l)
  {
    return symbol_type (token::TOK_BOOL4_TYPE, l);
  }

  parser::symbol_type
  parser::make_INT_TYPE (const location_type& l)
  {
    return symbol_type (token::TOK_INT_TYPE, l);
  }

  parser::symbol_type
  parser::make_INT2_TYPE (const location_type& l)
  {
    return symbol_type (token::TOK_INT2_TYPE, l);
  }

  parser::symbol_type
  parser::make_INT3_TYPE (const location_type& l)
  {
    return symbol_type (token::TOK_INT3_TYPE, l);
  }

  parser::symbol_type
  parser::make_INT4_TYPE (const location_type& l)
  {
    return symbol_type (token::TOK_INT4_TYPE, l);
  }

  parser::symbol_type
  parser::make_TEXTURE1D_TYPE (const location_type& l)
  {
    return symbol_type (token::TOK_TEXTURE1D_TYPE, l);
  }

  parser::symbol_type
  parser::make_TEXTURE2D_TYPE (const location_type& l)
  {
    return symbol_type (token::TOK_TEXTURE2D_TYPE, l);
  }

  parser::symbol_type
  parser::make_TEXTURE2DSHADOW_TYPE (const location_type& l)
  {
    return symbol_type (token::TOK_TEXTURE2DSHADOW_TYPE, l);
  }

  parser::symbol_type
  parser::make_TEXTURE2DRECT_TYPE (const location_type& l)
  {
    return symbol_type (token::TOK_TEXTURE2DRECT_TYPE, l);
  }

  parser::symbol_type
  parser::make_TEXTURE3D_TYPE (const location_type& l)
  {
    return symbol_type (token::TOK_TEXTURE3D_TYPE, l);
  }

  parser::symbol_type
  parser::make_TEXTURECUBE_TYPE (const location_type& l)
  {
    return symbol_type (token::TOK_TEXTURECUBE_TYPE, l);
  }

  parser::symbol_type
  parser::make_SAMPLER1D_TYPE (const location_type& l)
  {
    return symbol_type (token::TOK_SAMPLER1D_TYPE, l);
  }

  parser::symbol_type
  parser::make_SAMPLER2D_TYPE (const location_type& l)
  {
    return symbol_type (token::TOK_SAMPLER2D_TYPE, l);
  }

  parser::symbol_type
  parser::make_SAMPLER2DSHADOW_TYPE (const location_type& l)
  {
    return symbol_type (token::TOK_SAMPLER2DSHADOW_TYPE, l);
  }

  parser::symbol_type
  parser::make_SAMPLER2DRECT_TYPE (const location_type& l)
  {
    return symbol_type (token::TOK_SAMPLER2DRECT_TYPE, l);
  }

  parser::symbol_type
  parser::make_SAMPLER3D_TYPE (const location_type& l)
  {
    return symbol_type (token::TOK_SAMPLER3D_TYPE, l);
  }

  parser::symbol_type
  parser::make_SAMPLERCUBE_TYPE (const location_type& l)
  {
    return symbol_type (token::TOK_SAMPLERCUBE_TYPE, l);
  }

  parser::symbol_type
  parser::make_EXTENSION (const location_type& l)
  {
    return symbol_type (token::TOK_EXTENSION, l);
  }

  parser::symbol_type
  parser::make_SEPARATE_SHADER (const location_type& l)
  {
    return symbol_type (token::TOK_SEPARATE_SHADER, l);
  }

  parser::symbol_type
  parser::make_VERTEXPROGRAM (const location_type& l)
  {
    return symbol_type (token::TOK_VERTEXPROGRAM, l);
  }

  parser::symbol_type
  parser::make_FRAGMENTPROGRAM (const location_type& l)
  {
    return symbol_type (token::TOK_FRAGMENTPROGRAM, l);
  }

  parser::symbol_type
  parser::make_GEOMETRYPROGRAM (const location_type& l)
  {
    return symbol_type (token::TOK_GEOMETRYPROGRAM, l);
  }

  parser::symbol_type
  parser::make_HULLPROGRAM (const location_type& l)
  {
    return symbol_type (token::TOK_HULLPROGRAM, l);
  }

  parser::symbol_type
  parser::make_EVALPROGRAM (const location_type& l)
  {
    return symbol_type (token::TOK_EVALPROGRAM, l);
  }

  parser::symbol_type
  parser::make_SHDPROFILE (const location_type& l)
  {
    return symbol_type (token::TOK_SHDPROFILE, l);
  }

  parser::symbol_type
  parser::make_SAMPLERRESOURCE (const location_type& l)
  {
    return symbol_type (token::TOK_SAMPLERRESOURCE, l);
  }

  parser::symbol_type
  parser::make_SAMPLERTEXUNIT (const location_type& l)
  {
    return symbol_type (token::TOK_SAMPLERTEXUNIT, l);
  }

  parser::symbol_type
  parser::make_SETSAMPLERSTATE (const location_type& l)
  {
    return symbol_type (token::TOK_SETSAMPLERSTATE, l);
  }

  parser::symbol_type
  parser::make_SETDSTSTATE (const location_type& l)
  {
    return symbol_type (token::TOK_SETDSTSTATE, l);
  }

  parser::symbol_type
  parser::make_SETRASTERIZATIONSTATE (const location_type& l)
  {
    return symbol_type (token::TOK_SETRASTERIZATIONSTATE, l);
  }

  parser::symbol_type
  parser::make_SETCOLORSAMPLESTATE (const location_type& l)
  {
    return symbol_type (token::TOK_SETCOLORSAMPLESTATE, l);
  }

  parser::symbol_type
  parser::make_IMAGERESOURCE (const location_type& l)
  {
    return symbol_type (token::TOK_IMAGERESOURCE, l);
  }

  parser::symbol_type
  parser::make_IMAGEUNIT (const location_type& l)
  {
    return symbol_type (token::TOK_IMAGEUNIT, l);
  }

  parser::symbol_type
  parser::make_IMAGEACCESS (const location_type& l)
  {
    return symbol_type (token::TOK_IMAGEACCESS, l);
  }

  parser::symbol_type
  parser::make_IMAGELAYER (const location_type& l)
  {
    return symbol_type (token::TOK_IMAGELAYER, l);
  }

  parser::symbol_type
  parser::make_IMAGELAYERED (const location_type& l)
  {
    return symbol_type (token::TOK_IMAGELAYERED, l);
  }

  parser::symbol_type
  parser::make_WRITE_ONLY (const location_type& l)
  {
    return symbol_type (token::TOK_WRITE_ONLY, l);
  }

  parser::symbol_type
  parser::make_READ_ONLY (const location_type& l)
  {
    return symbol_type (token::TOK_READ_ONLY, l);
  }

  parser::symbol_type
  parser::make_READ_WRITE (const location_type& l)
  {
    return symbol_type (token::TOK_READ_WRITE, l);
  }

  parser::symbol_type
  parser::make_FORMAT_UNDEFINED (const VkFormat& v, const location_type& l)
  {
    return symbol_type (token::TOK_FORMAT_UNDEFINED, v, l);
  }

  parser::symbol_type
  parser::make_FORMAT_R4G4_UNORM_PACK8 (const VkFormat& v, const location_type& l)
  {
    return symbol_type (token::TOK_FORMAT_R4G4_UNORM_PACK8, v, l);
  }

  parser::symbol_type
  parser::make_FORMAT_R4G4B4A4_UNORM_PACK16 (const VkFormat& v, const location_type& l)
  {
    return symbol_type (token::TOK_FORMAT_R4G4B4A4_UNORM_PACK16, v, l);
  }

  parser::symbol_type
  parser::make_FORMAT_B4G4R4A4_UNORM_PACK16 (const VkFormat& v, const location_type& l)
  {
    return symbol_type (token::TOK_FORMAT_B4G4R4A4_UNORM_PACK16, v, l);
  }

  parser::symbol_type
  parser::make_FORMAT_R5G6B5_UNORM_PACK16 (const VkFormat& v, const location_type& l)
  {
    return symbol_type (token::TOK_FORMAT_R5G6B5_UNORM_PACK16, v, l);
  }

  parser::symbol_type
  parser::make_FORMAT_B5G6R5_UNORM_PACK16 (const VkFormat& v, const location_type& l)
  {
    return symbol_type (token::TOK_FORMAT_B5G6R5_UNORM_PACK16, v, l);
  }

  parser::symbol_type
  parser::make_FORMAT_R5G5B5A1_UNORM_PACK16 (const VkFormat& v, const location_type& l)
  {
    return symbol_type (token::TOK_FORMAT_R5G5B5A1_UNORM_PACK16, v, l);
  }

  parser::symbol_type
  parser::make_FORMAT_B5G5R5A1_UNORM_PACK16 (const VkFormat& v, const location_type& l)
  {
    return symbol_type (token::TOK_FORMAT_B5G5R5A1_UNORM_PACK16, v, l);
  }

  parser::symbol_type
  parser::make_FORMAT_A1R5G5B5_UNORM_PACK16 (const VkFormat& v, const location_type& l)
  {
    return symbol_type (token::TOK_FORMAT_A1R5G5B5_UNORM_PACK16, v, l);
  }

  parser::symbol_type
  parser::make_FORMAT_R8_UNORM (const VkFormat& v, const location_type& l)
  {
    return symbol_type (token::TOK_FORMAT_R8_UNORM, v, l);
  }

  parser::symbol_type
  parser::make_FORMAT_R8_SNORM (const VkFormat& v, const location_type& l)
  {
    return symbol_type (token::TOK_FORMAT_R8_SNORM, v, l);
  }

  parser::symbol_type
  parser::make_FORMAT_R8_USCALED (const VkFormat& v, const location_type& l)
  {
    return symbol_type (token::TOK_FORMAT_R8_USCALED, v, l);
  }

  parser::symbol_type
  parser::make_FORMAT_R8_SSCALED (const VkFormat& v, const location_type& l)
  {
    return symbol_type (token::TOK_FORMAT_R8_SSCALED, v, l);
  }

  parser::symbol_type
  parser::make_FORMAT_R8_UINT (const VkFormat& v, const location_type& l)
  {
    return symbol_type (token::TOK_FORMAT_R8_UINT, v, l);
  }

  parser::symbol_type
  parser::make_FORMAT_R8_SINT (const VkFormat& v, const location_type& l)
  {
    return symbol_type (token::TOK_FORMAT_R8_SINT, v, l);
  }

  parser::symbol_type
  parser::make_FORMAT_R8_SRGB (const VkFormat& v, const location_type& l)
  {
    return symbol_type (token::TOK_FORMAT_R8_SRGB, v, l);
  }

  parser::symbol_type
  parser::make_FORMAT_R8G8_UNORM (const VkFormat& v, const location_type& l)
  {
    return symbol_type (token::TOK_FORMAT_R8G8_UNORM, v, l);
  }

  parser::symbol_type
  parser::make_FORMAT_R8G8_SNORM (const VkFormat& v, const location_type& l)
  {
    return symbol_type (token::TOK_FORMAT_R8G8_SNORM, v, l);
  }

  parser::symbol_type
  parser::make_FORMAT_R8G8_USCALED (const VkFormat& v, const location_type& l)
  {
    return symbol_type (token::TOK_FORMAT_R8G8_USCALED, v, l);
  }

  parser::symbol_type
  parser::make_FORMAT_R8G8_SSCALED (const VkFormat& v, const location_type& l)
  {
    return symbol_type (token::TOK_FORMAT_R8G8_SSCALED, v, l);
  }

  parser::symbol_type
  parser::make_FORMAT_R8G8_UINT (const VkFormat& v, const location_type& l)
  {
    return symbol_type (token::TOK_FORMAT_R8G8_UINT, v, l);
  }

  parser::symbol_type
  parser::make_FORMAT_R8G8_SINT (const VkFormat& v, const location_type& l)
  {
    return symbol_type (token::TOK_FORMAT_R8G8_SINT, v, l);
  }

  parser::symbol_type
  parser::make_FORMAT_R8G8_SRGB (const VkFormat& v, const location_type& l)
  {
    return symbol_type (token::TOK_FORMAT_R8G8_SRGB, v, l);
  }

  parser::symbol_type
  parser::make_FORMAT_R8G8B8_UNORM (const VkFormat& v, const location_type& l)
  {
    return symbol_type (token::TOK_FORMAT_R8G8B8_UNORM, v, l);
  }

  parser::symbol_type
  parser::make_FORMAT_R8G8B8_SNORM (const VkFormat& v, const location_type& l)
  {
    return symbol_type (token::TOK_FORMAT_R8G8B8_SNORM, v, l);
  }

  parser::symbol_type
  parser::make_FORMAT_R8G8B8_USCALED (const VkFormat& v, const location_type& l)
  {
    return symbol_type (token::TOK_FORMAT_R8G8B8_USCALED, v, l);
  }

  parser::symbol_type
  parser::make_FORMAT_R8G8B8_SSCALED (const VkFormat& v, const location_type& l)
  {
    return symbol_type (token::TOK_FORMAT_R8G8B8_SSCALED, v, l);
  }

  parser::symbol_type
  parser::make_FORMAT_R8G8B8_UINT (const VkFormat& v, const location_type& l)
  {
    return symbol_type (token::TOK_FORMAT_R8G8B8_UINT, v, l);
  }

  parser::symbol_type
  parser::make_FORMAT_R8G8B8_SINT (const VkFormat& v, const location_type& l)
  {
    return symbol_type (token::TOK_FORMAT_R8G8B8_SINT, v, l);
  }

  parser::symbol_type
  parser::make_FORMAT_R8G8B8_SRGB (const VkFormat& v, const location_type& l)
  {
    return symbol_type (token::TOK_FORMAT_R8G8B8_SRGB, v, l);
  }

  parser::symbol_type
  parser::make_FORMAT_B8G8R8_UNORM (const VkFormat& v, const location_type& l)
  {
    return symbol_type (token::TOK_FORMAT_B8G8R8_UNORM, v, l);
  }

  parser::symbol_type
  parser::make_FORMAT_B8G8R8_SNORM (const VkFormat& v, const location_type& l)
  {
    return symbol_type (token::TOK_FORMAT_B8G8R8_SNORM, v, l);
  }

  parser::symbol_type
  parser::make_FORMAT_B8G8R8_USCALED (const VkFormat& v, const location_type& l)
  {
    return symbol_type (token::TOK_FORMAT_B8G8R8_USCALED, v, l);
  }

  parser::symbol_type
  parser::make_FORMAT_B8G8R8_SSCALED (const VkFormat& v, const location_type& l)
  {
    return symbol_type (token::TOK_FORMAT_B8G8R8_SSCALED, v, l);
  }

  parser::symbol_type
  parser::make_FORMAT_B8G8R8_UINT (const VkFormat& v, const location_type& l)
  {
    return symbol_type (token::TOK_FORMAT_B8G8R8_UINT, v, l);
  }

  parser::symbol_type
  parser::make_FORMAT_B8G8R8_SINT (const VkFormat& v, const location_type& l)
  {
    return symbol_type (token::TOK_FORMAT_B8G8R8_SINT, v, l);
  }

  parser::symbol_type
  parser::make_FORMAT_B8G8R8_SRGB (const VkFormat& v, const location_type& l)
  {
    return symbol_type (token::TOK_FORMAT_B8G8R8_SRGB, v, l);
  }

  parser::symbol_type
  parser::make_FORMAT_R8G8B8A8_UNORM (const VkFormat& v, const location_type& l)
  {
    return symbol_type (token::TOK_FORMAT_R8G8B8A8_UNORM, v, l);
  }

  parser::symbol_type
  parser::make_FORMAT_R8G8B8A8_SNORM (const VkFormat& v, const location_type& l)
  {
    return symbol_type (token::TOK_FORMAT_R8G8B8A8_SNORM, v, l);
  }

  parser::symbol_type
  parser::make_FORMAT_R8G8B8A8_USCALED (const VkFormat& v, const location_type& l)
  {
    return symbol_type (token::TOK_FORMAT_R8G8B8A8_USCALED, v, l);
  }

  parser::symbol_type
  parser::make_FORMAT_R8G8B8A8_SSCALED (const VkFormat& v, const location_type& l)
  {
    return symbol_type (token::TOK_FORMAT_R8G8B8A8_SSCALED, v, l);
  }

  parser::symbol_type
  parser::make_FORMAT_R8G8B8A8_UINT (const VkFormat& v, const location_type& l)
  {
    return symbol_type (token::TOK_FORMAT_R8G8B8A8_UINT, v, l);
  }

  parser::symbol_type
  parser::make_FORMAT_R8G8B8A8_SINT (const VkFormat& v, const location_type& l)
  {
    return symbol_type (token::TOK_FORMAT_R8G8B8A8_SINT, v, l);
  }

  parser::symbol_type
  parser::make_FORMAT_R8G8B8A8_SRGB (const VkFormat& v, const location_type& l)
  {
    return symbol_type (token::TOK_FORMAT_R8G8B8A8_SRGB, v, l);
  }

  parser::symbol_type
  parser::make_FORMAT_B8G8R8A8_UNORM (const VkFormat& v, const location_type& l)
  {
    return symbol_type (token::TOK_FORMAT_B8G8R8A8_UNORM, v, l);
  }

  parser::symbol_type
  parser::make_FORMAT_B8G8R8A8_SNORM (const VkFormat& v, const location_type& l)
  {
    return symbol_type (token::TOK_FORMAT_B8G8R8A8_SNORM, v, l);
  }

  parser::symbol_type
  parser::make_FORMAT_B8G8R8A8_USCALED (const VkFormat& v, const location_type& l)
  {
    return symbol_type (token::TOK_FORMAT_B8G8R8A8_USCALED, v, l);
  }

  parser::symbol_type
  parser::make_FORMAT_B8G8R8A8_SSCALED (const VkFormat& v, const location_type& l)
  {
    return symbol_type (token::TOK_FORMAT_B8G8R8A8_SSCALED, v, l);
  }

  parser::symbol_type
  parser::make_FORMAT_B8G8R8A8_UINT (const VkFormat& v, const location_type& l)
  {
    return symbol_type (token::TOK_FORMAT_B8G8R8A8_UINT, v, l);
  }

  parser::symbol_type
  parser::make_FORMAT_B8G8R8A8_SINT (const VkFormat& v, const location_type& l)
  {
    return symbol_type (token::TOK_FORMAT_B8G8R8A8_SINT, v, l);
  }

  parser::symbol_type
  parser::make_FORMAT_B8G8R8A8_SRGB (const VkFormat& v, const location_type& l)
  {
    return symbol_type (token::TOK_FORMAT_B8G8R8A8_SRGB, v, l);
  }

  parser::symbol_type
  parser::make_FORMAT_A8B8G8R8_UNORM_PACK32 (const VkFormat& v, const location_type& l)
  {
    return symbol_type (token::TOK_FORMAT_A8B8G8R8_UNORM_PACK32, v, l);
  }

  parser::symbol_type
  parser::make_FORMAT_A8B8G8R8_SNORM_PACK32 (const VkFormat& v, const location_type& l)
  {
    return symbol_type (token::TOK_FORMAT_A8B8G8R8_SNORM_PACK32, v, l);
  }

  parser::symbol_type
  parser::make_FORMAT_A8B8G8R8_USCALED_PACK32 (const VkFormat& v, const location_type& l)
  {
    return symbol_type (token::TOK_FORMAT_A8B8G8R8_USCALED_PACK32, v, l);
  }

  parser::symbol_type
  parser::make_FORMAT_A8B8G8R8_SSCALED_PACK32 (const VkFormat& v, const location_type& l)
  {
    return symbol_type (token::TOK_FORMAT_A8B8G8R8_SSCALED_PACK32, v, l);
  }

  parser::symbol_type
  parser::make_FORMAT_A8B8G8R8_UINT_PACK32 (const VkFormat& v, const location_type& l)
  {
    return symbol_type (token::TOK_FORMAT_A8B8G8R8_UINT_PACK32, v, l);
  }

  parser::symbol_type
  parser::make_FORMAT_A8B8G8R8_SINT_PACK32 (const VkFormat& v, const location_type& l)
  {
    return symbol_type (token::TOK_FORMAT_A8B8G8R8_SINT_PACK32, v, l);
  }

  parser::symbol_type
  parser::make_FORMAT_A8B8G8R8_SRGB_PACK32 (const VkFormat& v, const location_type& l)
  {
    return symbol_type (token::TOK_FORMAT_A8B8G8R8_SRGB_PACK32, v, l);
  }

  parser::symbol_type
  parser::make_FORMAT_A2R10G10B10_UNORM_PACK32 (const VkFormat& v, const location_type& l)
  {
    return symbol_type (token::TOK_FORMAT_A2R10G10B10_UNORM_PACK32, v, l);
  }

  parser::symbol_type
  parser::make_FORMAT_A2R10G10B10_SNORM_PACK32 (const VkFormat& v, const location_type& l)
  {
    return symbol_type (token::TOK_FORMAT_A2R10G10B10_SNORM_PACK32, v, l);
  }

  parser::symbol_type
  parser::make_FORMAT_A2R10G10B10_USCALED_PACK32 (const VkFormat& v, const location_type& l)
  {
    return symbol_type (token::TOK_FORMAT_A2R10G10B10_USCALED_PACK32, v, l);
  }

  parser::symbol_type
  parser::make_FORMAT_A2R10G10B10_SSCALED_PACK32 (const VkFormat& v, const location_type& l)
  {
    return symbol_type (token::TOK_FORMAT_A2R10G10B10_SSCALED_PACK32, v, l);
  }

  parser::symbol_type
  parser::make_FORMAT_A2R10G10B10_UINT_PACK32 (const VkFormat& v, const location_type& l)
  {
    return symbol_type (token::TOK_FORMAT_A2R10G10B10_UINT_PACK32, v, l);
  }

  parser::symbol_type
  parser::make_FORMAT_A2R10G10B10_SINT_PACK32 (const VkFormat& v, const location_type& l)
  {
    return symbol_type (token::TOK_FORMAT_A2R10G10B10_SINT_PACK32, v, l);
  }

  parser::symbol_type
  parser::make_FORMAT_A2B10G10R10_UNORM_PACK32 (const VkFormat& v, const location_type& l)
  {
    return symbol_type (token::TOK_FORMAT_A2B10G10R10_UNORM_PACK32, v, l);
  }

  parser::symbol_type
  parser::make_FORMAT_A2B10G10R10_SNORM_PACK32 (const VkFormat& v, const location_type& l)
  {
    return symbol_type (token::TOK_FORMAT_A2B10G10R10_SNORM_PACK32, v, l);
  }

  parser::symbol_type
  parser::make_FORMAT_A2B10G10R10_USCALED_PACK32 (const VkFormat& v, const location_type& l)
  {
    return symbol_type (token::TOK_FORMAT_A2B10G10R10_USCALED_PACK32, v, l);
  }

  parser::symbol_type
  parser::make_FORMAT_A2B10G10R10_SSCALED_PACK32 (const VkFormat& v, const location_type& l)
  {
    return symbol_type (token::TOK_FORMAT_A2B10G10R10_SSCALED_PACK32, v, l);
  }

  parser::symbol_type
  parser::make_FORMAT_A2B10G10R10_UINT_PACK32 (const VkFormat& v, const location_type& l)
  {
    return symbol_type (token::TOK_FORMAT_A2B10G10R10_UINT_PACK32, v, l);
  }

  parser::symbol_type
  parser::make_FORMAT_A2B10G10R10_SINT_PACK32 (const VkFormat& v, const location_type& l)
  {
    return symbol_type (token::TOK_FORMAT_A2B10G10R10_SINT_PACK32, v, l);
  }

  parser::symbol_type
  parser::make_FORMAT_R16_UNORM (const VkFormat& v, const location_type& l)
  {
    return symbol_type (token::TOK_FORMAT_R16_UNORM, v, l);
  }

  parser::symbol_type
  parser::make_FORMAT_R16_SNORM (const VkFormat& v, const location_type& l)
  {
    return symbol_type (token::TOK_FORMAT_R16_SNORM, v, l);
  }

  parser::symbol_type
  parser::make_FORMAT_R16_USCALED (const VkFormat& v, const location_type& l)
  {
    return symbol_type (token::TOK_FORMAT_R16_USCALED, v, l);
  }

  parser::symbol_type
  parser::make_FORMAT_R16_SSCALED (const VkFormat& v, const location_type& l)
  {
    return symbol_type (token::TOK_FORMAT_R16_SSCALED, v, l);
  }

  parser::symbol_type
  parser::make_FORMAT_R16_UINT (const VkFormat& v, const location_type& l)
  {
    return symbol_type (token::TOK_FORMAT_R16_UINT, v, l);
  }

  parser::symbol_type
  parser::make_FORMAT_R16_SINT (const VkFormat& v, const location_type& l)
  {
    return symbol_type (token::TOK_FORMAT_R16_SINT, v, l);
  }

  parser::symbol_type
  parser::make_FORMAT_R16_SFLOAT (const VkFormat& v, const location_type& l)
  {
    return symbol_type (token::TOK_FORMAT_R16_SFLOAT, v, l);
  }

  parser::symbol_type
  parser::make_FORMAT_R16G16_UNORM (const VkFormat& v, const location_type& l)
  {
    return symbol_type (token::TOK_FORMAT_R16G16_UNORM, v, l);
  }

  parser::symbol_type
  parser::make_FORMAT_R16G16_SNORM (const VkFormat& v, const location_type& l)
  {
    return symbol_type (token::TOK_FORMAT_R16G16_SNORM, v, l);
  }

  parser::symbol_type
  parser::make_FORMAT_R16G16_USCALED (const VkFormat& v, const location_type& l)
  {
    return symbol_type (token::TOK_FORMAT_R16G16_USCALED, v, l);
  }

  parser::symbol_type
  parser::make_FORMAT_R16G16_SSCALED (const VkFormat& v, const location_type& l)
  {
    return symbol_type (token::TOK_FORMAT_R16G16_SSCALED, v, l);
  }

  parser::symbol_type
  parser::make_FORMAT_R16G16_UINT (const VkFormat& v, const location_type& l)
  {
    return symbol_type (token::TOK_FORMAT_R16G16_UINT, v, l);
  }

  parser::symbol_type
  parser::make_FORMAT_R16G16_SINT (const VkFormat& v, const location_type& l)
  {
    return symbol_type (token::TOK_FORMAT_R16G16_SINT, v, l);
  }

  parser::symbol_type
  parser::make_FORMAT_R16G16_SFLOAT (const VkFormat& v, const location_type& l)
  {
    return symbol_type (token::TOK_FORMAT_R16G16_SFLOAT, v, l);
  }

  parser::symbol_type
  parser::make_FORMAT_R16G16B16_UNORM (const VkFormat& v, const location_type& l)
  {
    return symbol_type (token::TOK_FORMAT_R16G16B16_UNORM, v, l);
  }

  parser::symbol_type
  parser::make_FORMAT_R16G16B16_SNORM (const VkFormat& v, const location_type& l)
  {
    return symbol_type (token::TOK_FORMAT_R16G16B16_SNORM, v, l);
  }

  parser::symbol_type
  parser::make_FORMAT_R16G16B16_USCALED (const VkFormat& v, const location_type& l)
  {
    return symbol_type (token::TOK_FORMAT_R16G16B16_USCALED, v, l);
  }

  parser::symbol_type
  parser::make_FORMAT_R16G16B16_SSCALED (const VkFormat& v, const location_type& l)
  {
    return symbol_type (token::TOK_FORMAT_R16G16B16_SSCALED, v, l);
  }

  parser::symbol_type
  parser::make_FORMAT_R16G16B16_UINT (const VkFormat& v, const location_type& l)
  {
    return symbol_type (token::TOK_FORMAT_R16G16B16_UINT, v, l);
  }

  parser::symbol_type
  parser::make_FORMAT_R16G16B16_SINT (const VkFormat& v, const location_type& l)
  {
    return symbol_type (token::TOK_FORMAT_R16G16B16_SINT, v, l);
  }

  parser::symbol_type
  parser::make_FORMAT_R16G16B16_SFLOAT (const VkFormat& v, const location_type& l)
  {
    return symbol_type (token::TOK_FORMAT_R16G16B16_SFLOAT, v, l);
  }

  parser::symbol_type
  parser::make_FORMAT_R16G16B16A16_UNORM (const VkFormat& v, const location_type& l)
  {
    return symbol_type (token::TOK_FORMAT_R16G16B16A16_UNORM, v, l);
  }

  parser::symbol_type
  parser::make_FORMAT_R16G16B16A16_SNORM (const VkFormat& v, const location_type& l)
  {
    return symbol_type (token::TOK_FORMAT_R16G16B16A16_SNORM, v, l);
  }

  parser::symbol_type
  parser::make_FORMAT_R16G16B16A16_USCALED (const VkFormat& v, const location_type& l)
  {
    return symbol_type (token::TOK_FORMAT_R16G16B16A16_USCALED, v, l);
  }

  parser::symbol_type
  parser::make_FORMAT_R16G16B16A16_SSCALED (const VkFormat& v, const location_type& l)
  {
    return symbol_type (token::TOK_FORMAT_R16G16B16A16_SSCALED, v, l);
  }

  parser::symbol_type
  parser::make_FORMAT_R16G16B16A16_UINT (const VkFormat& v, const location_type& l)
  {
    return symbol_type (token::TOK_FORMAT_R16G16B16A16_UINT, v, l);
  }

  parser::symbol_type
  parser::make_FORMAT_R16G16B16A16_SINT (const VkFormat& v, const location_type& l)
  {
    return symbol_type (token::TOK_FORMAT_R16G16B16A16_SINT, v, l);
  }

  parser::symbol_type
  parser::make_FORMAT_R16G16B16A16_SFLOAT (const VkFormat& v, const location_type& l)
  {
    return symbol_type (token::TOK_FORMAT_R16G16B16A16_SFLOAT, v, l);
  }

  parser::symbol_type
  parser::make_FORMAT_R32_UINT (const VkFormat& v, const location_type& l)
  {
    return symbol_type (token::TOK_FORMAT_R32_UINT, v, l);
  }

  parser::symbol_type
  parser::make_FORMAT_R32_SINT (const VkFormat& v, const location_type& l)
  {
    return symbol_type (token::TOK_FORMAT_R32_SINT, v, l);
  }

  parser::symbol_type
  parser::make_FORMAT_R32_SFLOAT (const VkFormat& v, const location_type& l)
  {
    return symbol_type (token::TOK_FORMAT_R32_SFLOAT, v, l);
  }

  parser::symbol_type
  parser::make_FORMAT_R32G32_UINT (const VkFormat& v, const location_type& l)
  {
    return symbol_type (token::TOK_FORMAT_R32G32_UINT, v, l);
  }

  parser::symbol_type
  parser::make_FORMAT_R32G32_SINT (const VkFormat& v, const location_type& l)
  {
    return symbol_type (token::TOK_FORMAT_R32G32_SINT, v, l);
  }

  parser::symbol_type
  parser::make_FORMAT_R32G32_SFLOAT (const VkFormat& v, const location_type& l)
  {
    return symbol_type (token::TOK_FORMAT_R32G32_SFLOAT, v, l);
  }

  parser::symbol_type
  parser::make_FORMAT_R32G32B32_UINT (const VkFormat& v, const location_type& l)
  {
    return symbol_type (token::TOK_FORMAT_R32G32B32_UINT, v, l);
  }

  parser::symbol_type
  parser::make_FORMAT_R32G32B32_SINT (const VkFormat& v, const location_type& l)
  {
    return symbol_type (token::TOK_FORMAT_R32G32B32_SINT, v, l);
  }

  parser::symbol_type
  parser::make_FORMAT_R32G32B32_SFLOAT (const VkFormat& v, const location_type& l)
  {
    return symbol_type (token::TOK_FORMAT_R32G32B32_SFLOAT, v, l);
  }

  parser::symbol_type
  parser::make_FORMAT_R32G32B32A32_UINT (const VkFormat& v, const location_type& l)
  {
    return symbol_type (token::TOK_FORMAT_R32G32B32A32_UINT, v, l);
  }

  parser::symbol_type
  parser::make_FORMAT_R32G32B32A32_SINT (const VkFormat& v, const location_type& l)
  {
    return symbol_type (token::TOK_FORMAT_R32G32B32A32_SINT, v, l);
  }

  parser::symbol_type
  parser::make_FORMAT_R32G32B32A32_SFLOAT (const VkFormat& v, const location_type& l)
  {
    return symbol_type (token::TOK_FORMAT_R32G32B32A32_SFLOAT, v, l);
  }

  parser::symbol_type
  parser::make_FORMAT_R64_UINT (const VkFormat& v, const location_type& l)
  {
    return symbol_type (token::TOK_FORMAT_R64_UINT, v, l);
  }

  parser::symbol_type
  parser::make_FORMAT_R64_SINT (const VkFormat& v, const location_type& l)
  {
    return symbol_type (token::TOK_FORMAT_R64_SINT, v, l);
  }

  parser::symbol_type
  parser::make_FORMAT_R64_SFLOAT (const VkFormat& v, const location_type& l)
  {
    return symbol_type (token::TOK_FORMAT_R64_SFLOAT, v, l);
  }

  parser::symbol_type
  parser::make_FORMAT_R64G64_UINT (const VkFormat& v, const location_type& l)
  {
    return symbol_type (token::TOK_FORMAT_R64G64_UINT, v, l);
  }

  parser::symbol_type
  parser::make_FORMAT_R64G64_SINT (const VkFormat& v, const location_type& l)
  {
    return symbol_type (token::TOK_FORMAT_R64G64_SINT, v, l);
  }

  parser::symbol_type
  parser::make_FORMAT_R64G64_SFLOAT (const VkFormat& v, const location_type& l)
  {
    return symbol_type (token::TOK_FORMAT_R64G64_SFLOAT, v, l);
  }

  parser::symbol_type
  parser::make_FORMAT_R64G64B64_UINT (const VkFormat& v, const location_type& l)
  {
    return symbol_type (token::TOK_FORMAT_R64G64B64_UINT, v, l);
  }

  parser::symbol_type
  parser::make_FORMAT_R64G64B64_SINT (const VkFormat& v, const location_type& l)
  {
    return symbol_type (token::TOK_FORMAT_R64G64B64_SINT, v, l);
  }

  parser::symbol_type
  parser::make_FORMAT_R64G64B64_SFLOAT (const VkFormat& v, const location_type& l)
  {
    return symbol_type (token::TOK_FORMAT_R64G64B64_SFLOAT, v, l);
  }

  parser::symbol_type
  parser::make_FORMAT_R64G64B64A64_UINT (const VkFormat& v, const location_type& l)
  {
    return symbol_type (token::TOK_FORMAT_R64G64B64A64_UINT, v, l);
  }

  parser::symbol_type
  parser::make_FORMAT_R64G64B64A64_SINT (const VkFormat& v, const location_type& l)
  {
    return symbol_type (token::TOK_FORMAT_R64G64B64A64_SINT, v, l);
  }

  parser::symbol_type
  parser::make_FORMAT_R64G64B64A64_SFLOAT (const VkFormat& v, const location_type& l)
  {
    return symbol_type (token::TOK_FORMAT_R64G64B64A64_SFLOAT, v, l);
  }

  parser::symbol_type
  parser::make_FORMAT_B10G11R11_UFLOAT_PACK32 (const VkFormat& v, const location_type& l)
  {
    return symbol_type (token::TOK_FORMAT_B10G11R11_UFLOAT_PACK32, v, l);
  }

  parser::symbol_type
  parser::make_FORMAT_E5B9G9R9_UFLOAT_PACK32 (const VkFormat& v, const location_type& l)
  {
    return symbol_type (token::TOK_FORMAT_E5B9G9R9_UFLOAT_PACK32, v, l);
  }

  parser::symbol_type
  parser::make_FORMAT_D16_UNORM (const VkFormat& v, const location_type& l)
  {
    return symbol_type (token::TOK_FORMAT_D16_UNORM, v, l);
  }

  parser::symbol_type
  parser::make_FORMAT_X8_D24_UNORM_PACK32 (const VkFormat& v, const location_type& l)
  {
    return symbol_type (token::TOK_FORMAT_X8_D24_UNORM_PACK32, v, l);
  }

  parser::symbol_type
  parser::make_FORMAT_D32_SFLOAT (const VkFormat& v, const location_type& l)
  {
    return symbol_type (token::TOK_FORMAT_D32_SFLOAT, v, l);
  }

  parser::symbol_type
  parser::make_FORMAT_S8_UINT (const VkFormat& v, const location_type& l)
  {
    return symbol_type (token::TOK_FORMAT_S8_UINT, v, l);
  }

  parser::symbol_type
  parser::make_FORMAT_D16_UNORM_S8_UINT (const VkFormat& v, const location_type& l)
  {
    return symbol_type (token::TOK_FORMAT_D16_UNORM_S8_UINT, v, l);
  }

  parser::symbol_type
  parser::make_FORMAT_D24_UNORM_S8_UINT (const VkFormat& v, const location_type& l)
  {
    return symbol_type (token::TOK_FORMAT_D24_UNORM_S8_UINT, v, l);
  }

  parser::symbol_type
  parser::make_FORMAT_D32_SFLOAT_S8_UINT (const VkFormat& v, const location_type& l)
  {
    return symbol_type (token::TOK_FORMAT_D32_SFLOAT_S8_UINT, v, l);
  }

  parser::symbol_type
  parser::make_FORMAT_BC1_RGB_UNORM_BLOCK (const VkFormat& v, const location_type& l)
  {
    return symbol_type (token::TOK_FORMAT_BC1_RGB_UNORM_BLOCK, v, l);
  }

  parser::symbol_type
  parser::make_FORMAT_BC1_RGB_SRGB_BLOCK (const VkFormat& v, const location_type& l)
  {
    return symbol_type (token::TOK_FORMAT_BC1_RGB_SRGB_BLOCK, v, l);
  }

  parser::symbol_type
  parser::make_FORMAT_BC1_RGBA_UNORM_BLOCK (const VkFormat& v, const location_type& l)
  {
    return symbol_type (token::TOK_FORMAT_BC1_RGBA_UNORM_BLOCK, v, l);
  }

  parser::symbol_type
  parser::make_FORMAT_BC1_RGBA_SRGB_BLOCK (const VkFormat& v, const location_type& l)
  {
    return symbol_type (token::TOK_FORMAT_BC1_RGBA_SRGB_BLOCK, v, l);
  }

  parser::symbol_type
  parser::make_FORMAT_BC2_UNORM_BLOCK (const VkFormat& v, const location_type& l)
  {
    return symbol_type (token::TOK_FORMAT_BC2_UNORM_BLOCK, v, l);
  }

  parser::symbol_type
  parser::make_FORMAT_BC2_SRGB_BLOCK (const VkFormat& v, const location_type& l)
  {
    return symbol_type (token::TOK_FORMAT_BC2_SRGB_BLOCK, v, l);
  }

  parser::symbol_type
  parser::make_FORMAT_BC3_UNORM_BLOCK (const VkFormat& v, const location_type& l)
  {
    return symbol_type (token::TOK_FORMAT_BC3_UNORM_BLOCK, v, l);
  }

  parser::symbol_type
  parser::make_FORMAT_BC3_SRGB_BLOCK (const VkFormat& v, const location_type& l)
  {
    return symbol_type (token::TOK_FORMAT_BC3_SRGB_BLOCK, v, l);
  }

  parser::symbol_type
  parser::make_FORMAT_BC4_UNORM_BLOCK (const VkFormat& v, const location_type& l)
  {
    return symbol_type (token::TOK_FORMAT_BC4_UNORM_BLOCK, v, l);
  }

  parser::symbol_type
  parser::make_FORMAT_BC4_SNORM_BLOCK (const VkFormat& v, const location_type& l)
  {
    return symbol_type (token::TOK_FORMAT_BC4_SNORM_BLOCK, v, l);
  }

  parser::symbol_type
  parser::make_FORMAT_BC5_UNORM_BLOCK (const VkFormat& v, const location_type& l)
  {
    return symbol_type (token::TOK_FORMAT_BC5_UNORM_BLOCK, v, l);
  }

  parser::symbol_type
  parser::make_FORMAT_BC5_SNORM_BLOCK (const VkFormat& v, const location_type& l)
  {
    return symbol_type (token::TOK_FORMAT_BC5_SNORM_BLOCK, v, l);
  }

  parser::symbol_type
  parser::make_FORMAT_BC6H_UFLOAT_BLOCK (const VkFormat& v, const location_type& l)
  {
    return symbol_type (token::TOK_FORMAT_BC6H_UFLOAT_BLOCK, v, l);
  }

  parser::symbol_type
  parser::make_FORMAT_BC6H_SFLOAT_BLOCK (const VkFormat& v, const location_type& l)
  {
    return symbol_type (token::TOK_FORMAT_BC6H_SFLOAT_BLOCK, v, l);
  }

  parser::symbol_type
  parser::make_FORMAT_BC7_UNORM_BLOCK (const VkFormat& v, const location_type& l)
  {
    return symbol_type (token::TOK_FORMAT_BC7_UNORM_BLOCK, v, l);
  }

  parser::symbol_type
  parser::make_FORMAT_BC7_SRGB_BLOCK (const VkFormat& v, const location_type& l)
  {
    return symbol_type (token::TOK_FORMAT_BC7_SRGB_BLOCK, v, l);
  }

  parser::symbol_type
  parser::make_FORMAT_ETC2_R8G8B8_UNORM_BLOCK (const VkFormat& v, const location_type& l)
  {
    return symbol_type (token::TOK_FORMAT_ETC2_R8G8B8_UNORM_BLOCK, v, l);
  }

  parser::symbol_type
  parser::make_FORMAT_ETC2_R8G8B8_SRGB_BLOCK (const VkFormat& v, const location_type& l)
  {
    return symbol_type (token::TOK_FORMAT_ETC2_R8G8B8_SRGB_BLOCK, v, l);
  }

  parser::symbol_type
  parser::make_FORMAT_ETC2_R8G8B8A1_UNORM_BLOCK (const VkFormat& v, const location_type& l)
  {
    return symbol_type (token::TOK_FORMAT_ETC2_R8G8B8A1_UNORM_BLOCK, v, l);
  }

  parser::symbol_type
  parser::make_FORMAT_ETC2_R8G8B8A1_SRGB_BLOCK (const VkFormat& v, const location_type& l)
  {
    return symbol_type (token::TOK_FORMAT_ETC2_R8G8B8A1_SRGB_BLOCK, v, l);
  }

  parser::symbol_type
  parser::make_FORMAT_ETC2_R8G8B8A8_UNORM_BLOCK (const VkFormat& v, const location_type& l)
  {
    return symbol_type (token::TOK_FORMAT_ETC2_R8G8B8A8_UNORM_BLOCK, v, l);
  }

  parser::symbol_type
  parser::make_FORMAT_ETC2_R8G8B8A8_SRGB_BLOCK (const VkFormat& v, const location_type& l)
  {
    return symbol_type (token::TOK_FORMAT_ETC2_R8G8B8A8_SRGB_BLOCK, v, l);
  }

  parser::symbol_type
  parser::make_FORMAT_EAC_R11_UNORM_BLOCK (const VkFormat& v, const location_type& l)
  {
    return symbol_type (token::TOK_FORMAT_EAC_R11_UNORM_BLOCK, v, l);
  }

  parser::symbol_type
  parser::make_FORMAT_EAC_R11_SNORM_BLOCK (const VkFormat& v, const location_type& l)
  {
    return symbol_type (token::TOK_FORMAT_EAC_R11_SNORM_BLOCK, v, l);
  }

  parser::symbol_type
  parser::make_FORMAT_EAC_R11G11_UNORM_BLOCK (const VkFormat& v, const location_type& l)
  {
    return symbol_type (token::TOK_FORMAT_EAC_R11G11_UNORM_BLOCK, v, l);
  }

  parser::symbol_type
  parser::make_FORMAT_EAC_R11G11_SNORM_BLOCK (const VkFormat& v, const location_type& l)
  {
    return symbol_type (token::TOK_FORMAT_EAC_R11G11_SNORM_BLOCK, v, l);
  }

  parser::symbol_type
  parser::make_FORMAT_ASTC_4x4_UNORM_BLOCK (const VkFormat& v, const location_type& l)
  {
    return symbol_type (token::TOK_FORMAT_ASTC_4x4_UNORM_BLOCK, v, l);
  }

  parser::symbol_type
  parser::make_FORMAT_ASTC_4x4_SRGB_BLOCK (const VkFormat& v, const location_type& l)
  {
    return symbol_type (token::TOK_FORMAT_ASTC_4x4_SRGB_BLOCK, v, l);
  }

  parser::symbol_type
  parser::make_FORMAT_ASTC_5x4_UNORM_BLOCK (const VkFormat& v, const location_type& l)
  {
    return symbol_type (token::TOK_FORMAT_ASTC_5x4_UNORM_BLOCK, v, l);
  }

  parser::symbol_type
  parser::make_FORMAT_ASTC_5x4_SRGB_BLOCK (const VkFormat& v, const location_type& l)
  {
    return symbol_type (token::TOK_FORMAT_ASTC_5x4_SRGB_BLOCK, v, l);
  }

  parser::symbol_type
  parser::make_FORMAT_ASTC_5x5_UNORM_BLOCK (const VkFormat& v, const location_type& l)
  {
    return symbol_type (token::TOK_FORMAT_ASTC_5x5_UNORM_BLOCK, v, l);
  }

  parser::symbol_type
  parser::make_FORMAT_ASTC_5x5_SRGB_BLOCK (const VkFormat& v, const location_type& l)
  {
    return symbol_type (token::TOK_FORMAT_ASTC_5x5_SRGB_BLOCK, v, l);
  }

  parser::symbol_type
  parser::make_FORMAT_ASTC_6x5_UNORM_BLOCK (const VkFormat& v, const location_type& l)
  {
    return symbol_type (token::TOK_FORMAT_ASTC_6x5_UNORM_BLOCK, v, l);
  }

  parser::symbol_type
  parser::make_FORMAT_ASTC_6x5_SRGB_BLOCK (const VkFormat& v, const location_type& l)
  {
    return symbol_type (token::TOK_FORMAT_ASTC_6x5_SRGB_BLOCK, v, l);
  }

  parser::symbol_type
  parser::make_FORMAT_ASTC_6x6_UNORM_BLOCK (const VkFormat& v, const location_type& l)
  {
    return symbol_type (token::TOK_FORMAT_ASTC_6x6_UNORM_BLOCK, v, l);
  }

  parser::symbol_type
  parser::make_FORMAT_ASTC_6x6_SRGB_BLOCK (const VkFormat& v, const location_type& l)
  {
    return symbol_type (token::TOK_FORMAT_ASTC_6x6_SRGB_BLOCK, v, l);
  }

  parser::symbol_type
  parser::make_FORMAT_ASTC_8x5_UNORM_BLOCK (const VkFormat& v, const location_type& l)
  {
    return symbol_type (token::TOK_FORMAT_ASTC_8x5_UNORM_BLOCK, v, l);
  }

  parser::symbol_type
  parser::make_FORMAT_ASTC_8x5_SRGB_BLOCK (const VkFormat& v, const location_type& l)
  {
    return symbol_type (token::TOK_FORMAT_ASTC_8x5_SRGB_BLOCK, v, l);
  }

  parser::symbol_type
  parser::make_FORMAT_ASTC_8x6_UNORM_BLOCK (const VkFormat& v, const location_type& l)
  {
    return symbol_type (token::TOK_FORMAT_ASTC_8x6_UNORM_BLOCK, v, l);
  }

  parser::symbol_type
  parser::make_FORMAT_ASTC_8x6_SRGB_BLOCK (const VkFormat& v, const location_type& l)
  {
    return symbol_type (token::TOK_FORMAT_ASTC_8x6_SRGB_BLOCK, v, l);
  }

  parser::symbol_type
  parser::make_FORMAT_ASTC_8x8_UNORM_BLOCK (const VkFormat& v, const location_type& l)
  {
    return symbol_type (token::TOK_FORMAT_ASTC_8x8_UNORM_BLOCK, v, l);
  }

  parser::symbol_type
  parser::make_FORMAT_ASTC_8x8_SRGB_BLOCK (const VkFormat& v, const location_type& l)
  {
    return symbol_type (token::TOK_FORMAT_ASTC_8x8_SRGB_BLOCK, v, l);
  }

  parser::symbol_type
  parser::make_FORMAT_ASTC_10x5_UNORM_BLOCK (const VkFormat& v, const location_type& l)
  {
    return symbol_type (token::TOK_FORMAT_ASTC_10x5_UNORM_BLOCK, v, l);
  }

  parser::symbol_type
  parser::make_FORMAT_ASTC_10x5_SRGB_BLOCK (const VkFormat& v, const location_type& l)
  {
    return symbol_type (token::TOK_FORMAT_ASTC_10x5_SRGB_BLOCK, v, l);
  }

  parser::symbol_type
  parser::make_FORMAT_ASTC_10x6_UNORM_BLOCK (const VkFormat& v, const location_type& l)
  {
    return symbol_type (token::TOK_FORMAT_ASTC_10x6_UNORM_BLOCK, v, l);
  }

  parser::symbol_type
  parser::make_FORMAT_ASTC_10x6_SRGB_BLOCK (const VkFormat& v, const location_type& l)
  {
    return symbol_type (token::TOK_FORMAT_ASTC_10x6_SRGB_BLOCK, v, l);
  }

  parser::symbol_type
  parser::make_FORMAT_ASTC_10x8_UNORM_BLOCK (const VkFormat& v, const location_type& l)
  {
    return symbol_type (token::TOK_FORMAT_ASTC_10x8_UNORM_BLOCK, v, l);
  }

  parser::symbol_type
  parser::make_FORMAT_ASTC_10x8_SRGB_BLOCK (const VkFormat& v, const location_type& l)
  {
    return symbol_type (token::TOK_FORMAT_ASTC_10x8_SRGB_BLOCK, v, l);
  }

  parser::symbol_type
  parser::make_FORMAT_ASTC_10x10_UNORM_BLOCK (const VkFormat& v, const location_type& l)
  {
    return symbol_type (token::TOK_FORMAT_ASTC_10x10_UNORM_BLOCK, v, l);
  }

  parser::symbol_type
  parser::make_FORMAT_ASTC_10x10_SRGB_BLOCK (const VkFormat& v, const location_type& l)
  {
    return symbol_type (token::TOK_FORMAT_ASTC_10x10_SRGB_BLOCK, v, l);
  }

  parser::symbol_type
  parser::make_FORMAT_ASTC_12x10_UNORM_BLOCK (const VkFormat& v, const location_type& l)
  {
    return symbol_type (token::TOK_FORMAT_ASTC_12x10_UNORM_BLOCK, v, l);
  }

  parser::symbol_type
  parser::make_FORMAT_ASTC_12x10_SRGB_BLOCK (const VkFormat& v, const location_type& l)
  {
    return symbol_type (token::TOK_FORMAT_ASTC_12x10_SRGB_BLOCK, v, l);
  }

  parser::symbol_type
  parser::make_FORMAT_ASTC_12x12_UNORM_BLOCK (const VkFormat& v, const location_type& l)
  {
    return symbol_type (token::TOK_FORMAT_ASTC_12x12_UNORM_BLOCK, v, l);
  }

  parser::symbol_type
  parser::make_FORMAT_ASTC_12x12_SRGB_BLOCK (const VkFormat& v, const location_type& l)
  {
    return symbol_type (token::TOK_FORMAT_ASTC_12x12_SRGB_BLOCK, v, l);
  }

  parser::symbol_type
  parser::make_VERTEXFORMAT (const location_type& l)
  {
    return symbol_type (token::TOK_VERTEXFORMAT, l);
  }



} // yy
#line 5080 "Parser.hpp" // lalr1.cc:377




#endif // !YY_YY_PARSER_HPP_INCLUDED
