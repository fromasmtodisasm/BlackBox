// A Bison parser, made by GNU Bison 3.7.2.

// Skeleton interface for Bison LALR(1) parsers in C++

// Copyright (C) 2002-2015, 2018-2020 Free Software Foundation, Inc.

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
 ** \file C:/Users/HackMan/code/TestEngine/src/Engine/Renderer/parser.hh
 ** Define the yy::parser class.
 */

// C++ LALR(1) parser skeleton written by Akim Demaille.

// DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
// especially those whose name start with YY_ or yy_.  They are
// private implementation details that can be changed or removed.

#ifndef YY_YY_C_USERS_HACKMAN_CODE_TESTENGINE_SRC_ENGINE_RENDERER_PARSER_HH_INCLUDED
# define YY_YY_C_USERS_HACKMAN_CODE_TESTENGINE_SRC_ENGINE_RENDERER_PARSER_HH_INCLUDED
// "%code requires" blocks.
#line 10 "C:/Users/HackMan/code/TestEngine/src/Engine/Renderer/Shaders/Parser.yy"

    #include <string>
    #include <vulkan/vulkan.h>
    class Scanner;
    class Driver;

#line 56 "C:/Users/HackMan/code/TestEngine/src/Engine/Renderer/parser.hh"

# include <cassert>
# include <cstdlib> // std::abort
# include <iostream>
# include <stdexcept>
# include <string>
# include <vector>

#if defined __cplusplus
# define YY_CPLUSPLUS __cplusplus
#else
# define YY_CPLUSPLUS 199711L
#endif

// Support move semantics when possible.
#if 201103L <= YY_CPLUSPLUS
# define YY_MOVE           std::move
# define YY_MOVE_OR_COPY   move
# define YY_MOVE_REF(Type) Type&&
# define YY_RVREF(Type)    Type&&
# define YY_COPY(Type)     Type
#else
# define YY_MOVE
# define YY_MOVE_OR_COPY   copy
# define YY_MOVE_REF(Type) Type&
# define YY_RVREF(Type)    const Type&
# define YY_COPY(Type)     const Type&
#endif

// Support noexcept when possible.
#if 201103L <= YY_CPLUSPLUS
# define YY_NOEXCEPT noexcept
# define YY_NOTHROW
#else
# define YY_NOEXCEPT
# define YY_NOTHROW throw ()
#endif

// Support constexpr when possible.
#if 201703 <= YY_CPLUSPLUS
# define YY_CONSTEXPR constexpr
#else
# define YY_CONSTEXPR
#endif
# include "location.hh"
#include <typeinfo>
#ifndef YY_ASSERT
# include <cassert>
# define YY_ASSERT assert
#endif


#ifndef YY_ATTRIBUTE_PURE
# if defined __GNUC__ && 2 < __GNUC__ + (96 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_PURE __attribute__ ((__pure__))
# else
#  define YY_ATTRIBUTE_PURE
# endif
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# if defined __GNUC__ && 2 < __GNUC__ + (7 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_UNUSED __attribute__ ((__unused__))
# else
#  define YY_ATTRIBUTE_UNUSED
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(E) ((void) (E))
#else
# define YYUSE(E) /* empty */
#endif

#if defined __GNUC__ && ! defined __ICC && 407 <= __GNUC__ * 100 + __GNUC_MINOR__
/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                            \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")              \
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# define YY_IGNORE_MAYBE_UNINITIALIZED_END      \
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

#if defined __cplusplus && defined __GNUC__ && ! defined __ICC && 6 <= __GNUC__
# define YY_IGNORE_USELESS_CAST_BEGIN                          \
    _Pragma ("GCC diagnostic push")                            \
    _Pragma ("GCC diagnostic ignored \"-Wuseless-cast\"")
# define YY_IGNORE_USELESS_CAST_END            \
    _Pragma ("GCC diagnostic pop")
#endif
#ifndef YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_END
#endif

# ifndef YY_CAST
#  ifdef __cplusplus
#   define YY_CAST(Type, Val) static_cast<Type> (Val)
#   define YY_REINTERPRET_CAST(Type, Val) reinterpret_cast<Type> (Val)
#  else
#   define YY_CAST(Type, Val) ((Type) (Val))
#   define YY_REINTERPRET_CAST(Type, Val) ((Type) (Val))
#  endif
# endif
# ifndef YY_NULLPTR
#  if defined __cplusplus
#   if 201103L <= __cplusplus
#    define YY_NULLPTR nullptr
#   else
#    define YY_NULLPTR 0
#   endif
#  else
#   define YY_NULLPTR ((void*)0)
#  endif
# endif

/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 1
#endif

namespace yy {
#line 190 "C:/Users/HackMan/code/TestEngine/src/Engine/Renderer/parser.hh"




  /// A Bison parser.
  class parser
  {
  public:
#ifndef YYSTYPE
  /// A buffer to store and retrieve objects.
  ///
  /// Sort of a variant, but does not keep track of the nature
  /// of the stored data, since that knowledge is available
  /// via the current parser state.
  class semantic_type
  {
  public:
    /// Type of *this.
    typedef semantic_type self_type;

    /// Empty construction.
    semantic_type () YY_NOEXCEPT
      : yybuffer_ ()
      , yytypeid_ (YY_NULLPTR)
    {}

    /// Construct and fill.
    template <typename T>
    semantic_type (YY_RVREF (T) t)
      : yytypeid_ (&typeid (T))
    {
      YY_ASSERT (sizeof (T) <= size);
      new (yyas_<T> ()) T (YY_MOVE (t));
    }

#if 201103L <= YY_CPLUSPLUS
    /// Non copyable.
    semantic_type (const self_type&) = delete;
    /// Non copyable.
    self_type& operator= (const self_type&) = delete;
#endif

    /// Destruction, allowed only if empty.
    ~semantic_type () YY_NOEXCEPT
    {
      YY_ASSERT (!yytypeid_);
    }

# if 201103L <= YY_CPLUSPLUS
    /// Instantiate a \a T in here from \a t.
    template <typename T, typename... U>
    T&
    emplace (U&&... u)
    {
      YY_ASSERT (!yytypeid_);
      YY_ASSERT (sizeof (T) <= size);
      yytypeid_ = & typeid (T);
      return *new (yyas_<T> ()) T (std::forward <U>(u)...);
    }
# else
    /// Instantiate an empty \a T in here.
    template <typename T>
    T&
    emplace ()
    {
      YY_ASSERT (!yytypeid_);
      YY_ASSERT (sizeof (T) <= size);
      yytypeid_ = & typeid (T);
      return *new (yyas_<T> ()) T ();
    }

    /// Instantiate a \a T in here from \a t.
    template <typename T>
    T&
    emplace (const T& t)
    {
      YY_ASSERT (!yytypeid_);
      YY_ASSERT (sizeof (T) <= size);
      yytypeid_ = & typeid (T);
      return *new (yyas_<T> ()) T (t);
    }
# endif

    /// Instantiate an empty \a T in here.
    /// Obsolete, use emplace.
    template <typename T>
    T&
    build ()
    {
      return emplace<T> ();
    }

    /// Instantiate a \a T in here from \a t.
    /// Obsolete, use emplace.
    template <typename T>
    T&
    build (const T& t)
    {
      return emplace<T> (t);
    }

    /// Accessor to a built \a T.
    template <typename T>
    T&
    as () YY_NOEXCEPT
    {
      YY_ASSERT (yytypeid_);
      YY_ASSERT (*yytypeid_ == typeid (T));
      YY_ASSERT (sizeof (T) <= size);
      return *yyas_<T> ();
    }

    /// Const accessor to a built \a T (for %printer).
    template <typename T>
    const T&
    as () const YY_NOEXCEPT
    {
      YY_ASSERT (yytypeid_);
      YY_ASSERT (*yytypeid_ == typeid (T));
      YY_ASSERT (sizeof (T) <= size);
      return *yyas_<T> ();
    }

    /// Swap the content with \a that, of same type.
    ///
    /// Both variants must be built beforehand, because swapping the actual
    /// data requires reading it (with as()), and this is not possible on
    /// unconstructed variants: it would require some dynamic testing, which
    /// should not be the variant's responsibility.
    /// Swapping between built and (possibly) non-built is done with
    /// self_type::move ().
    template <typename T>
    void
    swap (self_type& that) YY_NOEXCEPT
    {
      YY_ASSERT (yytypeid_);
      YY_ASSERT (*yytypeid_ == *that.yytypeid_);
      std::swap (as<T> (), that.as<T> ());
    }

    /// Move the content of \a that to this.
    ///
    /// Destroys \a that.
    template <typename T>
    void
    move (self_type& that)
    {
# if 201103L <= YY_CPLUSPLUS
      emplace<T> (std::move (that.as<T> ()));
# else
      emplace<T> ();
      swap<T> (that);
# endif
      that.destroy<T> ();
    }

# if 201103L <= YY_CPLUSPLUS
    /// Move the content of \a that to this.
    template <typename T>
    void
    move (self_type&& that)
    {
      emplace<T> (std::move (that.as<T> ()));
      that.destroy<T> ();
    }
#endif

    /// Copy the content of \a that to this.
    template <typename T>
    void
    copy (const self_type& that)
    {
      emplace<T> (that.as<T> ());
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
#if YY_CPLUSPLUS < 201103L
    /// Non copyable.
    semantic_type (const self_type&);
    /// Non copyable.
    self_type& operator= (const self_type&);
#endif

    /// Accessor to raw memory as \a T.
    template <typename T>
    T*
    yyas_ () YY_NOEXCEPT
    {
      void *yyp = yybuffer_.yyraw;
      return static_cast<T*> (yyp);
     }

    /// Const accessor to raw memory as \a T.
    template <typename T>
    const T*
    yyas_ () const YY_NOEXCEPT
    {
      const void *yyp = yybuffer_.yyraw;
      return static_cast<const T*> (yyp);
     }

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
      char dummy1[sizeof (VkFormat)];

      // TRUE
      // FALSE
      // BOOL
      char dummy2[sizeof (bool)];

      // FLOAT
      char dummy3[sizeof (float)];

      // INT
      char dummy4[sizeof (int)];

      // IDENTIFIER
      // STR
      // CODEBODY
      // VARNAME
      char dummy5[sizeof (std::string)];
    };

    /// The size of the largest semantic type.
    enum { size = sizeof (union_type) };

    /// A buffer to store semantic values.
    union
    {
      /// Strongest alignment constraints.
      long double yyalign_me;
      /// A buffer large enough to store any of the semantic values.
      char yyraw[size];
    } yybuffer_;

    /// Whether the content is built: if defined, the name of the stored type.
    const std::type_info *yytypeid_;
  };

#else
    typedef YYSTYPE semantic_type;
#endif
    /// Symbol locations.
    typedef location location_type;

    /// Syntax errors thrown from user actions.
    struct syntax_error : std::runtime_error
    {
      syntax_error (const location_type& l, const std::string& m)
        : std::runtime_error (m)
        , location (l)
      {}

      syntax_error (const syntax_error& s)
        : std::runtime_error (s.what ())
        , location (s.location)
      {}

      ~syntax_error () YY_NOEXCEPT YY_NOTHROW;

      location_type location;
    };

    /// Token kinds.
    struct token
    {
      enum token_kind_type
      {
        TOK_YYEMPTY = -2,
    TOK_END = 0,                   // "end of file"
    TOK_YYerror = 256,             // error
    TOK_YYUNDEF = 257,             // "invalid token"
    TOK_ASSIGN = 258,              // "="
    TOK_MINUS = 259,               // "-"
    TOK_PLUS = 260,                // "+"
    TOK_STAR = 261,                // "*"
    TOK_SLASH = 262,               // "/"
    TOK_LPAREN = 263,              // "("
    TOK_RPAREN = 264,              // ")"
    TOK_LEFTSCOPE = 265,           // "{"
    TOK_RIGHTSCOPE = 266,          // "}"
    TOK_SEMICOLON = 267,           // ";"
    TOK_COMMA = 268,               // ","
    TOK_IDENTIFIER = 269,          // IDENTIFIER
    TOK_TRUE = 270,                // TRUE
    TOK_FALSE = 271,               // FALSE
    TOK_FLOAT = 272,               // FLOAT
    TOK_INT = 273,                 // INT
    TOK_BOOL = 274,                // BOOL
    TOK_STR = 275,                 // STR
    TOK_GLSLSHADER = 276,          // GLSLSHADER
    TOK_HLSL10SHADER = 277,        // HLSL10SHADER
    TOK_HLSL11SHADER = 278,        // HLSL11SHADER
    TOK_CGSHADER = 279,            // CGSHADER
    TOK_SAMPLER_STATE = 280,       // SAMPLER_STATE
    TOK_DST_STATE = 281,           // DST_STATE
    TOK_PR_STATE = 282,            // PR_STATE
    TOK_COLOR_SAMPLE_STATE = 283,  // COLOR_SAMPLE_STATE
    TOK_RASTERIZATION_STATE = 284, // RASTERIZATION_STATE
    TOK_TECHNIQUE = 285,           // TECHNIQUE
    TOK_PASS = 286,                // PASS
    TOK_CODEBODY = 287,            // CODEBODY
    TOK_VARNAME = 288,             // VARNAME
    TOK_TEXTURERESOURCE = 289,     // TEXTURERESOURCE
    TOK_TEXTURERESOURCE1D = 290,   // TEXTURERESOURCE1D
    TOK_TEXTURERESOURCE2D = 291,   // TEXTURERESOURCE2D
    TOK_TEXTURERESOURCE3D = 292,   // TEXTURERESOURCE3D
    TOK_TEXTURERESOURCERECT = 293, // TEXTURERESOURCERECT
    TOK_TEXTURERESOURCECUBE = 294, // TEXTURERESOURCECUBE
    TOK_VOID_TYPE = 295,           // VOID_TYPE
    TOK_UNSIGNED = 296,            // UNSIGNED
    TOK_HIGHP = 297,               // HIGHP
    TOK_MEDIUMP = 298,             // MEDIUMP
    TOK_LOWP = 299,                // LOWP
    TOK_UNIFORM = 300,             // UNIFORM
    TOK_CSTBUFFER = 301,           // CSTBUFFER
    TOK_FLOAT_TYPE = 302,          // FLOAT_TYPE
    TOK_FLOAT2_TYPE = 303,         // FLOAT2_TYPE
    TOK_FLOAT3_TYPE = 304,         // FLOAT3_TYPE
    TOK_FLOAT4_TYPE = 305,         // FLOAT4_TYPE
    TOK_MAT2_TYPE = 306,           // MAT2_TYPE
    TOK_MAT3_TYPE = 307,           // MAT3_TYPE
    TOK_MAT4_TYPE = 308,           // MAT4_TYPE
    TOK_BOOL_TYPE = 309,           // BOOL_TYPE
    TOK_BOOL2_TYPE = 310,          // BOOL2_TYPE
    TOK_BOOL3_TYPE = 311,          // BOOL3_TYPE
    TOK_BOOL4_TYPE = 312,          // BOOL4_TYPE
    TOK_INT_TYPE = 313,            // INT_TYPE
    TOK_INT2_TYPE = 314,           // INT2_TYPE
    TOK_INT3_TYPE = 315,           // INT3_TYPE
    TOK_INT4_TYPE = 316,           // INT4_TYPE
    TOK_TEXTURE1D_TYPE = 317,      // TEXTURE1D_TYPE
    TOK_TEXTURE2D_TYPE = 318,      // TEXTURE2D_TYPE
    TOK_TEXTURE2DSHADOW_TYPE = 319, // TEXTURE2DSHADOW_TYPE
    TOK_TEXTURE2DRECT_TYPE = 320,  // TEXTURE2DRECT_TYPE
    TOK_TEXTURE3D_TYPE = 321,      // TEXTURE3D_TYPE
    TOK_TEXTURECUBE_TYPE = 322,    // TEXTURECUBE_TYPE
    TOK_SAMPLER1D_TYPE = 323,      // SAMPLER1D_TYPE
    TOK_SAMPLER2D_TYPE = 324,      // SAMPLER2D_TYPE
    TOK_SAMPLER2DSHADOW_TYPE = 325, // SAMPLER2DSHADOW_TYPE
    TOK_SAMPLER2DRECT_TYPE = 326,  // SAMPLER2DRECT_TYPE
    TOK_SAMPLER3D_TYPE = 327,      // SAMPLER3D_TYPE
    TOK_SAMPLERCUBE_TYPE = 328,    // SAMPLERCUBE_TYPE
    TOK_EXTENSION = 329,           // EXTENSION
    TOK_SEPARATE_SHADER = 330,     // SEPARATE_SHADER
    TOK_VERTEXPROGRAM = 331,       // VERTEXPROGRAM
    TOK_FRAGMENTPROGRAM = 332,     // FRAGMENTPROGRAM
    TOK_GEOMETRYPROGRAM = 333,     // GEOMETRYPROGRAM
    TOK_HULLPROGRAM = 334,         // HULLPROGRAM
    TOK_EVALPROGRAM = 335,         // EVALPROGRAM
    TOK_SHDPROFILE = 336,          // SHDPROFILE
    TOK_SAMPLERRESOURCE = 337,     // SAMPLERRESOURCE
    TOK_SAMPLERTEXUNIT = 338,      // SAMPLERTEXUNIT
    TOK_SETSAMPLERSTATE = 339,     // SETSAMPLERSTATE
    TOK_SETDSTSTATE = 340,         // SETDSTSTATE
    TOK_SETRASTERIZATIONSTATE = 341, // SETRASTERIZATIONSTATE
    TOK_SETCOLORSAMPLESTATE = 342, // SETCOLORSAMPLESTATE
    TOK_IMAGERESOURCE = 343,       // IMAGERESOURCE
    TOK_IMAGEUNIT = 344,           // IMAGEUNIT
    TOK_IMAGEACCESS = 345,         // IMAGEACCESS
    TOK_IMAGELAYER = 346,          // IMAGELAYER
    TOK_IMAGELAYERED = 347,        // IMAGELAYERED
    TOK_WRITE_ONLY = 348,          // WRITE_ONLY
    TOK_READ_ONLY = 349,           // READ_ONLY
    TOK_READ_WRITE = 350,          // READ_WRITE
    TOK_FORMAT_UNDEFINED = 351,    // FORMAT_UNDEFINED
    TOK_FORMAT_R4G4_UNORM_PACK8 = 352, // FORMAT_R4G4_UNORM_PACK8
    TOK_FORMAT_R4G4B4A4_UNORM_PACK16 = 353, // FORMAT_R4G4B4A4_UNORM_PACK16
    TOK_FORMAT_B4G4R4A4_UNORM_PACK16 = 354, // FORMAT_B4G4R4A4_UNORM_PACK16
    TOK_FORMAT_R5G6B5_UNORM_PACK16 = 355, // FORMAT_R5G6B5_UNORM_PACK16
    TOK_FORMAT_B5G6R5_UNORM_PACK16 = 356, // FORMAT_B5G6R5_UNORM_PACK16
    TOK_FORMAT_R5G5B5A1_UNORM_PACK16 = 357, // FORMAT_R5G5B5A1_UNORM_PACK16
    TOK_FORMAT_B5G5R5A1_UNORM_PACK16 = 358, // FORMAT_B5G5R5A1_UNORM_PACK16
    TOK_FORMAT_A1R5G5B5_UNORM_PACK16 = 359, // FORMAT_A1R5G5B5_UNORM_PACK16
    TOK_FORMAT_R8_UNORM = 360,     // FORMAT_R8_UNORM
    TOK_FORMAT_R8_SNORM = 361,     // FORMAT_R8_SNORM
    TOK_FORMAT_R8_USCALED = 362,   // FORMAT_R8_USCALED
    TOK_FORMAT_R8_SSCALED = 363,   // FORMAT_R8_SSCALED
    TOK_FORMAT_R8_UINT = 364,      // FORMAT_R8_UINT
    TOK_FORMAT_R8_SINT = 365,      // FORMAT_R8_SINT
    TOK_FORMAT_R8_SRGB = 366,      // FORMAT_R8_SRGB
    TOK_FORMAT_R8G8_UNORM = 367,   // FORMAT_R8G8_UNORM
    TOK_FORMAT_R8G8_SNORM = 368,   // FORMAT_R8G8_SNORM
    TOK_FORMAT_R8G8_USCALED = 369, // FORMAT_R8G8_USCALED
    TOK_FORMAT_R8G8_SSCALED = 370, // FORMAT_R8G8_SSCALED
    TOK_FORMAT_R8G8_UINT = 371,    // FORMAT_R8G8_UINT
    TOK_FORMAT_R8G8_SINT = 372,    // FORMAT_R8G8_SINT
    TOK_FORMAT_R8G8_SRGB = 373,    // FORMAT_R8G8_SRGB
    TOK_FORMAT_R8G8B8_UNORM = 374, // FORMAT_R8G8B8_UNORM
    TOK_FORMAT_R8G8B8_SNORM = 375, // FORMAT_R8G8B8_SNORM
    TOK_FORMAT_R8G8B8_USCALED = 376, // FORMAT_R8G8B8_USCALED
    TOK_FORMAT_R8G8B8_SSCALED = 377, // FORMAT_R8G8B8_SSCALED
    TOK_FORMAT_R8G8B8_UINT = 378,  // FORMAT_R8G8B8_UINT
    TOK_FORMAT_R8G8B8_SINT = 379,  // FORMAT_R8G8B8_SINT
    TOK_FORMAT_R8G8B8_SRGB = 380,  // FORMAT_R8G8B8_SRGB
    TOK_FORMAT_B8G8R8_UNORM = 381, // FORMAT_B8G8R8_UNORM
    TOK_FORMAT_B8G8R8_SNORM = 382, // FORMAT_B8G8R8_SNORM
    TOK_FORMAT_B8G8R8_USCALED = 383, // FORMAT_B8G8R8_USCALED
    TOK_FORMAT_B8G8R8_SSCALED = 384, // FORMAT_B8G8R8_SSCALED
    TOK_FORMAT_B8G8R8_UINT = 385,  // FORMAT_B8G8R8_UINT
    TOK_FORMAT_B8G8R8_SINT = 386,  // FORMAT_B8G8R8_SINT
    TOK_FORMAT_B8G8R8_SRGB = 387,  // FORMAT_B8G8R8_SRGB
    TOK_FORMAT_R8G8B8A8_UNORM = 388, // FORMAT_R8G8B8A8_UNORM
    TOK_FORMAT_R8G8B8A8_SNORM = 389, // FORMAT_R8G8B8A8_SNORM
    TOK_FORMAT_R8G8B8A8_USCALED = 390, // FORMAT_R8G8B8A8_USCALED
    TOK_FORMAT_R8G8B8A8_SSCALED = 391, // FORMAT_R8G8B8A8_SSCALED
    TOK_FORMAT_R8G8B8A8_UINT = 392, // FORMAT_R8G8B8A8_UINT
    TOK_FORMAT_R8G8B8A8_SINT = 393, // FORMAT_R8G8B8A8_SINT
    TOK_FORMAT_R8G8B8A8_SRGB = 394, // FORMAT_R8G8B8A8_SRGB
    TOK_FORMAT_B8G8R8A8_UNORM = 395, // FORMAT_B8G8R8A8_UNORM
    TOK_FORMAT_B8G8R8A8_SNORM = 396, // FORMAT_B8G8R8A8_SNORM
    TOK_FORMAT_B8G8R8A8_USCALED = 397, // FORMAT_B8G8R8A8_USCALED
    TOK_FORMAT_B8G8R8A8_SSCALED = 398, // FORMAT_B8G8R8A8_SSCALED
    TOK_FORMAT_B8G8R8A8_UINT = 399, // FORMAT_B8G8R8A8_UINT
    TOK_FORMAT_B8G8R8A8_SINT = 400, // FORMAT_B8G8R8A8_SINT
    TOK_FORMAT_B8G8R8A8_SRGB = 401, // FORMAT_B8G8R8A8_SRGB
    TOK_FORMAT_A8B8G8R8_UNORM_PACK32 = 402, // FORMAT_A8B8G8R8_UNORM_PACK32
    TOK_FORMAT_A8B8G8R8_SNORM_PACK32 = 403, // FORMAT_A8B8G8R8_SNORM_PACK32
    TOK_FORMAT_A8B8G8R8_USCALED_PACK32 = 404, // FORMAT_A8B8G8R8_USCALED_PACK32
    TOK_FORMAT_A8B8G8R8_SSCALED_PACK32 = 405, // FORMAT_A8B8G8R8_SSCALED_PACK32
    TOK_FORMAT_A8B8G8R8_UINT_PACK32 = 406, // FORMAT_A8B8G8R8_UINT_PACK32
    TOK_FORMAT_A8B8G8R8_SINT_PACK32 = 407, // FORMAT_A8B8G8R8_SINT_PACK32
    TOK_FORMAT_A8B8G8R8_SRGB_PACK32 = 408, // FORMAT_A8B8G8R8_SRGB_PACK32
    TOK_FORMAT_A2R10G10B10_UNORM_PACK32 = 409, // FORMAT_A2R10G10B10_UNORM_PACK32
    TOK_FORMAT_A2R10G10B10_SNORM_PACK32 = 410, // FORMAT_A2R10G10B10_SNORM_PACK32
    TOK_FORMAT_A2R10G10B10_USCALED_PACK32 = 411, // FORMAT_A2R10G10B10_USCALED_PACK32
    TOK_FORMAT_A2R10G10B10_SSCALED_PACK32 = 412, // FORMAT_A2R10G10B10_SSCALED_PACK32
    TOK_FORMAT_A2R10G10B10_UINT_PACK32 = 413, // FORMAT_A2R10G10B10_UINT_PACK32
    TOK_FORMAT_A2R10G10B10_SINT_PACK32 = 414, // FORMAT_A2R10G10B10_SINT_PACK32
    TOK_FORMAT_A2B10G10R10_UNORM_PACK32 = 415, // FORMAT_A2B10G10R10_UNORM_PACK32
    TOK_FORMAT_A2B10G10R10_SNORM_PACK32 = 416, // FORMAT_A2B10G10R10_SNORM_PACK32
    TOK_FORMAT_A2B10G10R10_USCALED_PACK32 = 417, // FORMAT_A2B10G10R10_USCALED_PACK32
    TOK_FORMAT_A2B10G10R10_SSCALED_PACK32 = 418, // FORMAT_A2B10G10R10_SSCALED_PACK32
    TOK_FORMAT_A2B10G10R10_UINT_PACK32 = 419, // FORMAT_A2B10G10R10_UINT_PACK32
    TOK_FORMAT_A2B10G10R10_SINT_PACK32 = 420, // FORMAT_A2B10G10R10_SINT_PACK32
    TOK_FORMAT_R16_UNORM = 421,    // FORMAT_R16_UNORM
    TOK_FORMAT_R16_SNORM = 422,    // FORMAT_R16_SNORM
    TOK_FORMAT_R16_USCALED = 423,  // FORMAT_R16_USCALED
    TOK_FORMAT_R16_SSCALED = 424,  // FORMAT_R16_SSCALED
    TOK_FORMAT_R16_UINT = 425,     // FORMAT_R16_UINT
    TOK_FORMAT_R16_SINT = 426,     // FORMAT_R16_SINT
    TOK_FORMAT_R16_SFLOAT = 427,   // FORMAT_R16_SFLOAT
    TOK_FORMAT_R16G16_UNORM = 428, // FORMAT_R16G16_UNORM
    TOK_FORMAT_R16G16_SNORM = 429, // FORMAT_R16G16_SNORM
    TOK_FORMAT_R16G16_USCALED = 430, // FORMAT_R16G16_USCALED
    TOK_FORMAT_R16G16_SSCALED = 431, // FORMAT_R16G16_SSCALED
    TOK_FORMAT_R16G16_UINT = 432,  // FORMAT_R16G16_UINT
    TOK_FORMAT_R16G16_SINT = 433,  // FORMAT_R16G16_SINT
    TOK_FORMAT_R16G16_SFLOAT = 434, // FORMAT_R16G16_SFLOAT
    TOK_FORMAT_R16G16B16_UNORM = 435, // FORMAT_R16G16B16_UNORM
    TOK_FORMAT_R16G16B16_SNORM = 436, // FORMAT_R16G16B16_SNORM
    TOK_FORMAT_R16G16B16_USCALED = 437, // FORMAT_R16G16B16_USCALED
    TOK_FORMAT_R16G16B16_SSCALED = 438, // FORMAT_R16G16B16_SSCALED
    TOK_FORMAT_R16G16B16_UINT = 439, // FORMAT_R16G16B16_UINT
    TOK_FORMAT_R16G16B16_SINT = 440, // FORMAT_R16G16B16_SINT
    TOK_FORMAT_R16G16B16_SFLOAT = 441, // FORMAT_R16G16B16_SFLOAT
    TOK_FORMAT_R16G16B16A16_UNORM = 442, // FORMAT_R16G16B16A16_UNORM
    TOK_FORMAT_R16G16B16A16_SNORM = 443, // FORMAT_R16G16B16A16_SNORM
    TOK_FORMAT_R16G16B16A16_USCALED = 444, // FORMAT_R16G16B16A16_USCALED
    TOK_FORMAT_R16G16B16A16_SSCALED = 445, // FORMAT_R16G16B16A16_SSCALED
    TOK_FORMAT_R16G16B16A16_UINT = 446, // FORMAT_R16G16B16A16_UINT
    TOK_FORMAT_R16G16B16A16_SINT = 447, // FORMAT_R16G16B16A16_SINT
    TOK_FORMAT_R16G16B16A16_SFLOAT = 448, // FORMAT_R16G16B16A16_SFLOAT
    TOK_FORMAT_R32_UINT = 449,     // FORMAT_R32_UINT
    TOK_FORMAT_R32_SINT = 450,     // FORMAT_R32_SINT
    TOK_FORMAT_R32_SFLOAT = 451,   // FORMAT_R32_SFLOAT
    TOK_FORMAT_R32G32_UINT = 452,  // FORMAT_R32G32_UINT
    TOK_FORMAT_R32G32_SINT = 453,  // FORMAT_R32G32_SINT
    TOK_FORMAT_R32G32_SFLOAT = 454, // FORMAT_R32G32_SFLOAT
    TOK_FORMAT_R32G32B32_UINT = 455, // FORMAT_R32G32B32_UINT
    TOK_FORMAT_R32G32B32_SINT = 456, // FORMAT_R32G32B32_SINT
    TOK_FORMAT_R32G32B32_SFLOAT = 457, // FORMAT_R32G32B32_SFLOAT
    TOK_FORMAT_R32G32B32A32_UINT = 458, // FORMAT_R32G32B32A32_UINT
    TOK_FORMAT_R32G32B32A32_SINT = 459, // FORMAT_R32G32B32A32_SINT
    TOK_FORMAT_R32G32B32A32_SFLOAT = 460, // FORMAT_R32G32B32A32_SFLOAT
    TOK_FORMAT_R64_UINT = 461,     // FORMAT_R64_UINT
    TOK_FORMAT_R64_SINT = 462,     // FORMAT_R64_SINT
    TOK_FORMAT_R64_SFLOAT = 463,   // FORMAT_R64_SFLOAT
    TOK_FORMAT_R64G64_UINT = 464,  // FORMAT_R64G64_UINT
    TOK_FORMAT_R64G64_SINT = 465,  // FORMAT_R64G64_SINT
    TOK_FORMAT_R64G64_SFLOAT = 466, // FORMAT_R64G64_SFLOAT
    TOK_FORMAT_R64G64B64_UINT = 467, // FORMAT_R64G64B64_UINT
    TOK_FORMAT_R64G64B64_SINT = 468, // FORMAT_R64G64B64_SINT
    TOK_FORMAT_R64G64B64_SFLOAT = 469, // FORMAT_R64G64B64_SFLOAT
    TOK_FORMAT_R64G64B64A64_UINT = 470, // FORMAT_R64G64B64A64_UINT
    TOK_FORMAT_R64G64B64A64_SINT = 471, // FORMAT_R64G64B64A64_SINT
    TOK_FORMAT_R64G64B64A64_SFLOAT = 472, // FORMAT_R64G64B64A64_SFLOAT
    TOK_FORMAT_B10G11R11_UFLOAT_PACK32 = 473, // FORMAT_B10G11R11_UFLOAT_PACK32
    TOK_FORMAT_E5B9G9R9_UFLOAT_PACK32 = 474, // FORMAT_E5B9G9R9_UFLOAT_PACK32
    TOK_FORMAT_D16_UNORM = 475,    // FORMAT_D16_UNORM
    TOK_FORMAT_X8_D24_UNORM_PACK32 = 476, // FORMAT_X8_D24_UNORM_PACK32
    TOK_FORMAT_D32_SFLOAT = 477,   // FORMAT_D32_SFLOAT
    TOK_FORMAT_S8_UINT = 478,      // FORMAT_S8_UINT
    TOK_FORMAT_D16_UNORM_S8_UINT = 479, // FORMAT_D16_UNORM_S8_UINT
    TOK_FORMAT_D24_UNORM_S8_UINT = 480, // FORMAT_D24_UNORM_S8_UINT
    TOK_FORMAT_D32_SFLOAT_S8_UINT = 481, // FORMAT_D32_SFLOAT_S8_UINT
    TOK_FORMAT_BC1_RGB_UNORM_BLOCK = 482, // FORMAT_BC1_RGB_UNORM_BLOCK
    TOK_FORMAT_BC1_RGB_SRGB_BLOCK = 483, // FORMAT_BC1_RGB_SRGB_BLOCK
    TOK_FORMAT_BC1_RGBA_UNORM_BLOCK = 484, // FORMAT_BC1_RGBA_UNORM_BLOCK
    TOK_FORMAT_BC1_RGBA_SRGB_BLOCK = 485, // FORMAT_BC1_RGBA_SRGB_BLOCK
    TOK_FORMAT_BC2_UNORM_BLOCK = 486, // FORMAT_BC2_UNORM_BLOCK
    TOK_FORMAT_BC2_SRGB_BLOCK = 487, // FORMAT_BC2_SRGB_BLOCK
    TOK_FORMAT_BC3_UNORM_BLOCK = 488, // FORMAT_BC3_UNORM_BLOCK
    TOK_FORMAT_BC3_SRGB_BLOCK = 489, // FORMAT_BC3_SRGB_BLOCK
    TOK_FORMAT_BC4_UNORM_BLOCK = 490, // FORMAT_BC4_UNORM_BLOCK
    TOK_FORMAT_BC4_SNORM_BLOCK = 491, // FORMAT_BC4_SNORM_BLOCK
    TOK_FORMAT_BC5_UNORM_BLOCK = 492, // FORMAT_BC5_UNORM_BLOCK
    TOK_FORMAT_BC5_SNORM_BLOCK = 493, // FORMAT_BC5_SNORM_BLOCK
    TOK_FORMAT_BC6H_UFLOAT_BLOCK = 494, // FORMAT_BC6H_UFLOAT_BLOCK
    TOK_FORMAT_BC6H_SFLOAT_BLOCK = 495, // FORMAT_BC6H_SFLOAT_BLOCK
    TOK_FORMAT_BC7_UNORM_BLOCK = 496, // FORMAT_BC7_UNORM_BLOCK
    TOK_FORMAT_BC7_SRGB_BLOCK = 497, // FORMAT_BC7_SRGB_BLOCK
    TOK_FORMAT_ETC2_R8G8B8_UNORM_BLOCK = 498, // FORMAT_ETC2_R8G8B8_UNORM_BLOCK
    TOK_FORMAT_ETC2_R8G8B8_SRGB_BLOCK = 499, // FORMAT_ETC2_R8G8B8_SRGB_BLOCK
    TOK_FORMAT_ETC2_R8G8B8A1_UNORM_BLOCK = 500, // FORMAT_ETC2_R8G8B8A1_UNORM_BLOCK
    TOK_FORMAT_ETC2_R8G8B8A1_SRGB_BLOCK = 501, // FORMAT_ETC2_R8G8B8A1_SRGB_BLOCK
    TOK_FORMAT_ETC2_R8G8B8A8_UNORM_BLOCK = 502, // FORMAT_ETC2_R8G8B8A8_UNORM_BLOCK
    TOK_FORMAT_ETC2_R8G8B8A8_SRGB_BLOCK = 503, // FORMAT_ETC2_R8G8B8A8_SRGB_BLOCK
    TOK_FORMAT_EAC_R11_UNORM_BLOCK = 504, // FORMAT_EAC_R11_UNORM_BLOCK
    TOK_FORMAT_EAC_R11_SNORM_BLOCK = 505, // FORMAT_EAC_R11_SNORM_BLOCK
    TOK_FORMAT_EAC_R11G11_UNORM_BLOCK = 506, // FORMAT_EAC_R11G11_UNORM_BLOCK
    TOK_FORMAT_EAC_R11G11_SNORM_BLOCK = 507, // FORMAT_EAC_R11G11_SNORM_BLOCK
    TOK_FORMAT_ASTC_4x4_UNORM_BLOCK = 508, // FORMAT_ASTC_4x4_UNORM_BLOCK
    TOK_FORMAT_ASTC_4x4_SRGB_BLOCK = 509, // FORMAT_ASTC_4x4_SRGB_BLOCK
    TOK_FORMAT_ASTC_5x4_UNORM_BLOCK = 510, // FORMAT_ASTC_5x4_UNORM_BLOCK
    TOK_FORMAT_ASTC_5x4_SRGB_BLOCK = 511, // FORMAT_ASTC_5x4_SRGB_BLOCK
    TOK_FORMAT_ASTC_5x5_UNORM_BLOCK = 512, // FORMAT_ASTC_5x5_UNORM_BLOCK
    TOK_FORMAT_ASTC_5x5_SRGB_BLOCK = 513, // FORMAT_ASTC_5x5_SRGB_BLOCK
    TOK_FORMAT_ASTC_6x5_UNORM_BLOCK = 514, // FORMAT_ASTC_6x5_UNORM_BLOCK
    TOK_FORMAT_ASTC_6x5_SRGB_BLOCK = 515, // FORMAT_ASTC_6x5_SRGB_BLOCK
    TOK_FORMAT_ASTC_6x6_UNORM_BLOCK = 516, // FORMAT_ASTC_6x6_UNORM_BLOCK
    TOK_FORMAT_ASTC_6x6_SRGB_BLOCK = 517, // FORMAT_ASTC_6x6_SRGB_BLOCK
    TOK_FORMAT_ASTC_8x5_UNORM_BLOCK = 518, // FORMAT_ASTC_8x5_UNORM_BLOCK
    TOK_FORMAT_ASTC_8x5_SRGB_BLOCK = 519, // FORMAT_ASTC_8x5_SRGB_BLOCK
    TOK_FORMAT_ASTC_8x6_UNORM_BLOCK = 520, // FORMAT_ASTC_8x6_UNORM_BLOCK
    TOK_FORMAT_ASTC_8x6_SRGB_BLOCK = 521, // FORMAT_ASTC_8x6_SRGB_BLOCK
    TOK_FORMAT_ASTC_8x8_UNORM_BLOCK = 522, // FORMAT_ASTC_8x8_UNORM_BLOCK
    TOK_FORMAT_ASTC_8x8_SRGB_BLOCK = 523, // FORMAT_ASTC_8x8_SRGB_BLOCK
    TOK_FORMAT_ASTC_10x5_UNORM_BLOCK = 524, // FORMAT_ASTC_10x5_UNORM_BLOCK
    TOK_FORMAT_ASTC_10x5_SRGB_BLOCK = 525, // FORMAT_ASTC_10x5_SRGB_BLOCK
    TOK_FORMAT_ASTC_10x6_UNORM_BLOCK = 526, // FORMAT_ASTC_10x6_UNORM_BLOCK
    TOK_FORMAT_ASTC_10x6_SRGB_BLOCK = 527, // FORMAT_ASTC_10x6_SRGB_BLOCK
    TOK_FORMAT_ASTC_10x8_UNORM_BLOCK = 528, // FORMAT_ASTC_10x8_UNORM_BLOCK
    TOK_FORMAT_ASTC_10x8_SRGB_BLOCK = 529, // FORMAT_ASTC_10x8_SRGB_BLOCK
    TOK_FORMAT_ASTC_10x10_UNORM_BLOCK = 530, // FORMAT_ASTC_10x10_UNORM_BLOCK
    TOK_FORMAT_ASTC_10x10_SRGB_BLOCK = 531, // FORMAT_ASTC_10x10_SRGB_BLOCK
    TOK_FORMAT_ASTC_12x10_UNORM_BLOCK = 532, // FORMAT_ASTC_12x10_UNORM_BLOCK
    TOK_FORMAT_ASTC_12x10_SRGB_BLOCK = 533, // FORMAT_ASTC_12x10_SRGB_BLOCK
    TOK_FORMAT_ASTC_12x12_UNORM_BLOCK = 534, // FORMAT_ASTC_12x12_UNORM_BLOCK
    TOK_FORMAT_ASTC_12x12_SRGB_BLOCK = 535, // FORMAT_ASTC_12x12_SRGB_BLOCK
    TOK_VERTEXFORMAT = 536         // VERTEXFORMAT
      };
      /// Backward compatibility alias (Bison 3.6).
      typedef token_kind_type yytokentype;
    };

    /// Token kind, as returned by yylex.
    typedef token::yytokentype token_kind_type;

    /// Backward compatibility alias (Bison 3.6).
    typedef token_kind_type token_type;

    /// Symbol kinds.
    struct symbol_kind
    {
      enum symbol_kind_type
      {
        YYNTOKENS = 295, ///< Number of tokens.
        S_YYEMPTY = -2,
        S_YYEOF = 0,                             // "end of file"
        S_YYerror = 1,                           // error
        S_YYUNDEF = 2,                           // "invalid token"
        S_ASSIGN = 3,                            // "="
        S_MINUS = 4,                             // "-"
        S_PLUS = 5,                              // "+"
        S_STAR = 6,                              // "*"
        S_SLASH = 7,                             // "/"
        S_LPAREN = 8,                            // "("
        S_RPAREN = 9,                            // ")"
        S_LEFTSCOPE = 10,                        // "{"
        S_RIGHTSCOPE = 11,                       // "}"
        S_SEMICOLON = 12,                        // ";"
        S_COMMA = 13,                            // ","
        S_IDENTIFIER = 14,                       // IDENTIFIER
        S_TRUE = 15,                             // TRUE
        S_FALSE = 16,                            // FALSE
        S_FLOAT = 17,                            // FLOAT
        S_INT = 18,                              // INT
        S_BOOL = 19,                             // BOOL
        S_STR = 20,                              // STR
        S_GLSLSHADER = 21,                       // GLSLSHADER
        S_HLSL10SHADER = 22,                     // HLSL10SHADER
        S_HLSL11SHADER = 23,                     // HLSL11SHADER
        S_CGSHADER = 24,                         // CGSHADER
        S_SAMPLER_STATE = 25,                    // SAMPLER_STATE
        S_DST_STATE = 26,                        // DST_STATE
        S_PR_STATE = 27,                         // PR_STATE
        S_COLOR_SAMPLE_STATE = 28,               // COLOR_SAMPLE_STATE
        S_RASTERIZATION_STATE = 29,              // RASTERIZATION_STATE
        S_TECHNIQUE = 30,                        // TECHNIQUE
        S_PASS = 31,                             // PASS
        S_CODEBODY = 32,                         // CODEBODY
        S_VARNAME = 33,                          // VARNAME
        S_TEXTURERESOURCE = 34,                  // TEXTURERESOURCE
        S_TEXTURERESOURCE1D = 35,                // TEXTURERESOURCE1D
        S_TEXTURERESOURCE2D = 36,                // TEXTURERESOURCE2D
        S_TEXTURERESOURCE3D = 37,                // TEXTURERESOURCE3D
        S_TEXTURERESOURCERECT = 38,              // TEXTURERESOURCERECT
        S_TEXTURERESOURCECUBE = 39,              // TEXTURERESOURCECUBE
        S_VOID_TYPE = 40,                        // VOID_TYPE
        S_UNSIGNED = 41,                         // UNSIGNED
        S_HIGHP = 42,                            // HIGHP
        S_MEDIUMP = 43,                          // MEDIUMP
        S_LOWP = 44,                             // LOWP
        S_UNIFORM = 45,                          // UNIFORM
        S_CSTBUFFER = 46,                        // CSTBUFFER
        S_FLOAT_TYPE = 47,                       // FLOAT_TYPE
        S_FLOAT2_TYPE = 48,                      // FLOAT2_TYPE
        S_FLOAT3_TYPE = 49,                      // FLOAT3_TYPE
        S_FLOAT4_TYPE = 50,                      // FLOAT4_TYPE
        S_MAT2_TYPE = 51,                        // MAT2_TYPE
        S_MAT3_TYPE = 52,                        // MAT3_TYPE
        S_MAT4_TYPE = 53,                        // MAT4_TYPE
        S_BOOL_TYPE = 54,                        // BOOL_TYPE
        S_BOOL2_TYPE = 55,                       // BOOL2_TYPE
        S_BOOL3_TYPE = 56,                       // BOOL3_TYPE
        S_BOOL4_TYPE = 57,                       // BOOL4_TYPE
        S_INT_TYPE = 58,                         // INT_TYPE
        S_INT2_TYPE = 59,                        // INT2_TYPE
        S_INT3_TYPE = 60,                        // INT3_TYPE
        S_INT4_TYPE = 61,                        // INT4_TYPE
        S_TEXTURE1D_TYPE = 62,                   // TEXTURE1D_TYPE
        S_TEXTURE2D_TYPE = 63,                   // TEXTURE2D_TYPE
        S_TEXTURE2DSHADOW_TYPE = 64,             // TEXTURE2DSHADOW_TYPE
        S_TEXTURE2DRECT_TYPE = 65,               // TEXTURE2DRECT_TYPE
        S_TEXTURE3D_TYPE = 66,                   // TEXTURE3D_TYPE
        S_TEXTURECUBE_TYPE = 67,                 // TEXTURECUBE_TYPE
        S_SAMPLER1D_TYPE = 68,                   // SAMPLER1D_TYPE
        S_SAMPLER2D_TYPE = 69,                   // SAMPLER2D_TYPE
        S_SAMPLER2DSHADOW_TYPE = 70,             // SAMPLER2DSHADOW_TYPE
        S_SAMPLER2DRECT_TYPE = 71,               // SAMPLER2DRECT_TYPE
        S_SAMPLER3D_TYPE = 72,                   // SAMPLER3D_TYPE
        S_SAMPLERCUBE_TYPE = 73,                 // SAMPLERCUBE_TYPE
        S_EXTENSION = 74,                        // EXTENSION
        S_SEPARATE_SHADER = 75,                  // SEPARATE_SHADER
        S_VERTEXPROGRAM = 76,                    // VERTEXPROGRAM
        S_FRAGMENTPROGRAM = 77,                  // FRAGMENTPROGRAM
        S_GEOMETRYPROGRAM = 78,                  // GEOMETRYPROGRAM
        S_HULLPROGRAM = 79,                      // HULLPROGRAM
        S_EVALPROGRAM = 80,                      // EVALPROGRAM
        S_SHDPROFILE = 81,                       // SHDPROFILE
        S_SAMPLERRESOURCE = 82,                  // SAMPLERRESOURCE
        S_SAMPLERTEXUNIT = 83,                   // SAMPLERTEXUNIT
        S_SETSAMPLERSTATE = 84,                  // SETSAMPLERSTATE
        S_SETDSTSTATE = 85,                      // SETDSTSTATE
        S_SETRASTERIZATIONSTATE = 86,            // SETRASTERIZATIONSTATE
        S_SETCOLORSAMPLESTATE = 87,              // SETCOLORSAMPLESTATE
        S_IMAGERESOURCE = 88,                    // IMAGERESOURCE
        S_IMAGEUNIT = 89,                        // IMAGEUNIT
        S_IMAGEACCESS = 90,                      // IMAGEACCESS
        S_IMAGELAYER = 91,                       // IMAGELAYER
        S_IMAGELAYERED = 92,                     // IMAGELAYERED
        S_WRITE_ONLY = 93,                       // WRITE_ONLY
        S_READ_ONLY = 94,                        // READ_ONLY
        S_READ_WRITE = 95,                       // READ_WRITE
        S_FORMAT_UNDEFINED = 96,                 // FORMAT_UNDEFINED
        S_FORMAT_R4G4_UNORM_PACK8 = 97,          // FORMAT_R4G4_UNORM_PACK8
        S_FORMAT_R4G4B4A4_UNORM_PACK16 = 98,     // FORMAT_R4G4B4A4_UNORM_PACK16
        S_FORMAT_B4G4R4A4_UNORM_PACK16 = 99,     // FORMAT_B4G4R4A4_UNORM_PACK16
        S_FORMAT_R5G6B5_UNORM_PACK16 = 100,      // FORMAT_R5G6B5_UNORM_PACK16
        S_FORMAT_B5G6R5_UNORM_PACK16 = 101,      // FORMAT_B5G6R5_UNORM_PACK16
        S_FORMAT_R5G5B5A1_UNORM_PACK16 = 102,    // FORMAT_R5G5B5A1_UNORM_PACK16
        S_FORMAT_B5G5R5A1_UNORM_PACK16 = 103,    // FORMAT_B5G5R5A1_UNORM_PACK16
        S_FORMAT_A1R5G5B5_UNORM_PACK16 = 104,    // FORMAT_A1R5G5B5_UNORM_PACK16
        S_FORMAT_R8_UNORM = 105,                 // FORMAT_R8_UNORM
        S_FORMAT_R8_SNORM = 106,                 // FORMAT_R8_SNORM
        S_FORMAT_R8_USCALED = 107,               // FORMAT_R8_USCALED
        S_FORMAT_R8_SSCALED = 108,               // FORMAT_R8_SSCALED
        S_FORMAT_R8_UINT = 109,                  // FORMAT_R8_UINT
        S_FORMAT_R8_SINT = 110,                  // FORMAT_R8_SINT
        S_FORMAT_R8_SRGB = 111,                  // FORMAT_R8_SRGB
        S_FORMAT_R8G8_UNORM = 112,               // FORMAT_R8G8_UNORM
        S_FORMAT_R8G8_SNORM = 113,               // FORMAT_R8G8_SNORM
        S_FORMAT_R8G8_USCALED = 114,             // FORMAT_R8G8_USCALED
        S_FORMAT_R8G8_SSCALED = 115,             // FORMAT_R8G8_SSCALED
        S_FORMAT_R8G8_UINT = 116,                // FORMAT_R8G8_UINT
        S_FORMAT_R8G8_SINT = 117,                // FORMAT_R8G8_SINT
        S_FORMAT_R8G8_SRGB = 118,                // FORMAT_R8G8_SRGB
        S_FORMAT_R8G8B8_UNORM = 119,             // FORMAT_R8G8B8_UNORM
        S_FORMAT_R8G8B8_SNORM = 120,             // FORMAT_R8G8B8_SNORM
        S_FORMAT_R8G8B8_USCALED = 121,           // FORMAT_R8G8B8_USCALED
        S_FORMAT_R8G8B8_SSCALED = 122,           // FORMAT_R8G8B8_SSCALED
        S_FORMAT_R8G8B8_UINT = 123,              // FORMAT_R8G8B8_UINT
        S_FORMAT_R8G8B8_SINT = 124,              // FORMAT_R8G8B8_SINT
        S_FORMAT_R8G8B8_SRGB = 125,              // FORMAT_R8G8B8_SRGB
        S_FORMAT_B8G8R8_UNORM = 126,             // FORMAT_B8G8R8_UNORM
        S_FORMAT_B8G8R8_SNORM = 127,             // FORMAT_B8G8R8_SNORM
        S_FORMAT_B8G8R8_USCALED = 128,           // FORMAT_B8G8R8_USCALED
        S_FORMAT_B8G8R8_SSCALED = 129,           // FORMAT_B8G8R8_SSCALED
        S_FORMAT_B8G8R8_UINT = 130,              // FORMAT_B8G8R8_UINT
        S_FORMAT_B8G8R8_SINT = 131,              // FORMAT_B8G8R8_SINT
        S_FORMAT_B8G8R8_SRGB = 132,              // FORMAT_B8G8R8_SRGB
        S_FORMAT_R8G8B8A8_UNORM = 133,           // FORMAT_R8G8B8A8_UNORM
        S_FORMAT_R8G8B8A8_SNORM = 134,           // FORMAT_R8G8B8A8_SNORM
        S_FORMAT_R8G8B8A8_USCALED = 135,         // FORMAT_R8G8B8A8_USCALED
        S_FORMAT_R8G8B8A8_SSCALED = 136,         // FORMAT_R8G8B8A8_SSCALED
        S_FORMAT_R8G8B8A8_UINT = 137,            // FORMAT_R8G8B8A8_UINT
        S_FORMAT_R8G8B8A8_SINT = 138,            // FORMAT_R8G8B8A8_SINT
        S_FORMAT_R8G8B8A8_SRGB = 139,            // FORMAT_R8G8B8A8_SRGB
        S_FORMAT_B8G8R8A8_UNORM = 140,           // FORMAT_B8G8R8A8_UNORM
        S_FORMAT_B8G8R8A8_SNORM = 141,           // FORMAT_B8G8R8A8_SNORM
        S_FORMAT_B8G8R8A8_USCALED = 142,         // FORMAT_B8G8R8A8_USCALED
        S_FORMAT_B8G8R8A8_SSCALED = 143,         // FORMAT_B8G8R8A8_SSCALED
        S_FORMAT_B8G8R8A8_UINT = 144,            // FORMAT_B8G8R8A8_UINT
        S_FORMAT_B8G8R8A8_SINT = 145,            // FORMAT_B8G8R8A8_SINT
        S_FORMAT_B8G8R8A8_SRGB = 146,            // FORMAT_B8G8R8A8_SRGB
        S_FORMAT_A8B8G8R8_UNORM_PACK32 = 147,    // FORMAT_A8B8G8R8_UNORM_PACK32
        S_FORMAT_A8B8G8R8_SNORM_PACK32 = 148,    // FORMAT_A8B8G8R8_SNORM_PACK32
        S_FORMAT_A8B8G8R8_USCALED_PACK32 = 149,  // FORMAT_A8B8G8R8_USCALED_PACK32
        S_FORMAT_A8B8G8R8_SSCALED_PACK32 = 150,  // FORMAT_A8B8G8R8_SSCALED_PACK32
        S_FORMAT_A8B8G8R8_UINT_PACK32 = 151,     // FORMAT_A8B8G8R8_UINT_PACK32
        S_FORMAT_A8B8G8R8_SINT_PACK32 = 152,     // FORMAT_A8B8G8R8_SINT_PACK32
        S_FORMAT_A8B8G8R8_SRGB_PACK32 = 153,     // FORMAT_A8B8G8R8_SRGB_PACK32
        S_FORMAT_A2R10G10B10_UNORM_PACK32 = 154, // FORMAT_A2R10G10B10_UNORM_PACK32
        S_FORMAT_A2R10G10B10_SNORM_PACK32 = 155, // FORMAT_A2R10G10B10_SNORM_PACK32
        S_FORMAT_A2R10G10B10_USCALED_PACK32 = 156, // FORMAT_A2R10G10B10_USCALED_PACK32
        S_FORMAT_A2R10G10B10_SSCALED_PACK32 = 157, // FORMAT_A2R10G10B10_SSCALED_PACK32
        S_FORMAT_A2R10G10B10_UINT_PACK32 = 158,  // FORMAT_A2R10G10B10_UINT_PACK32
        S_FORMAT_A2R10G10B10_SINT_PACK32 = 159,  // FORMAT_A2R10G10B10_SINT_PACK32
        S_FORMAT_A2B10G10R10_UNORM_PACK32 = 160, // FORMAT_A2B10G10R10_UNORM_PACK32
        S_FORMAT_A2B10G10R10_SNORM_PACK32 = 161, // FORMAT_A2B10G10R10_SNORM_PACK32
        S_FORMAT_A2B10G10R10_USCALED_PACK32 = 162, // FORMAT_A2B10G10R10_USCALED_PACK32
        S_FORMAT_A2B10G10R10_SSCALED_PACK32 = 163, // FORMAT_A2B10G10R10_SSCALED_PACK32
        S_FORMAT_A2B10G10R10_UINT_PACK32 = 164,  // FORMAT_A2B10G10R10_UINT_PACK32
        S_FORMAT_A2B10G10R10_SINT_PACK32 = 165,  // FORMAT_A2B10G10R10_SINT_PACK32
        S_FORMAT_R16_UNORM = 166,                // FORMAT_R16_UNORM
        S_FORMAT_R16_SNORM = 167,                // FORMAT_R16_SNORM
        S_FORMAT_R16_USCALED = 168,              // FORMAT_R16_USCALED
        S_FORMAT_R16_SSCALED = 169,              // FORMAT_R16_SSCALED
        S_FORMAT_R16_UINT = 170,                 // FORMAT_R16_UINT
        S_FORMAT_R16_SINT = 171,                 // FORMAT_R16_SINT
        S_FORMAT_R16_SFLOAT = 172,               // FORMAT_R16_SFLOAT
        S_FORMAT_R16G16_UNORM = 173,             // FORMAT_R16G16_UNORM
        S_FORMAT_R16G16_SNORM = 174,             // FORMAT_R16G16_SNORM
        S_FORMAT_R16G16_USCALED = 175,           // FORMAT_R16G16_USCALED
        S_FORMAT_R16G16_SSCALED = 176,           // FORMAT_R16G16_SSCALED
        S_FORMAT_R16G16_UINT = 177,              // FORMAT_R16G16_UINT
        S_FORMAT_R16G16_SINT = 178,              // FORMAT_R16G16_SINT
        S_FORMAT_R16G16_SFLOAT = 179,            // FORMAT_R16G16_SFLOAT
        S_FORMAT_R16G16B16_UNORM = 180,          // FORMAT_R16G16B16_UNORM
        S_FORMAT_R16G16B16_SNORM = 181,          // FORMAT_R16G16B16_SNORM
        S_FORMAT_R16G16B16_USCALED = 182,        // FORMAT_R16G16B16_USCALED
        S_FORMAT_R16G16B16_SSCALED = 183,        // FORMAT_R16G16B16_SSCALED
        S_FORMAT_R16G16B16_UINT = 184,           // FORMAT_R16G16B16_UINT
        S_FORMAT_R16G16B16_SINT = 185,           // FORMAT_R16G16B16_SINT
        S_FORMAT_R16G16B16_SFLOAT = 186,         // FORMAT_R16G16B16_SFLOAT
        S_FORMAT_R16G16B16A16_UNORM = 187,       // FORMAT_R16G16B16A16_UNORM
        S_FORMAT_R16G16B16A16_SNORM = 188,       // FORMAT_R16G16B16A16_SNORM
        S_FORMAT_R16G16B16A16_USCALED = 189,     // FORMAT_R16G16B16A16_USCALED
        S_FORMAT_R16G16B16A16_SSCALED = 190,     // FORMAT_R16G16B16A16_SSCALED
        S_FORMAT_R16G16B16A16_UINT = 191,        // FORMAT_R16G16B16A16_UINT
        S_FORMAT_R16G16B16A16_SINT = 192,        // FORMAT_R16G16B16A16_SINT
        S_FORMAT_R16G16B16A16_SFLOAT = 193,      // FORMAT_R16G16B16A16_SFLOAT
        S_FORMAT_R32_UINT = 194,                 // FORMAT_R32_UINT
        S_FORMAT_R32_SINT = 195,                 // FORMAT_R32_SINT
        S_FORMAT_R32_SFLOAT = 196,               // FORMAT_R32_SFLOAT
        S_FORMAT_R32G32_UINT = 197,              // FORMAT_R32G32_UINT
        S_FORMAT_R32G32_SINT = 198,              // FORMAT_R32G32_SINT
        S_FORMAT_R32G32_SFLOAT = 199,            // FORMAT_R32G32_SFLOAT
        S_FORMAT_R32G32B32_UINT = 200,           // FORMAT_R32G32B32_UINT
        S_FORMAT_R32G32B32_SINT = 201,           // FORMAT_R32G32B32_SINT
        S_FORMAT_R32G32B32_SFLOAT = 202,         // FORMAT_R32G32B32_SFLOAT
        S_FORMAT_R32G32B32A32_UINT = 203,        // FORMAT_R32G32B32A32_UINT
        S_FORMAT_R32G32B32A32_SINT = 204,        // FORMAT_R32G32B32A32_SINT
        S_FORMAT_R32G32B32A32_SFLOAT = 205,      // FORMAT_R32G32B32A32_SFLOAT
        S_FORMAT_R64_UINT = 206,                 // FORMAT_R64_UINT
        S_FORMAT_R64_SINT = 207,                 // FORMAT_R64_SINT
        S_FORMAT_R64_SFLOAT = 208,               // FORMAT_R64_SFLOAT
        S_FORMAT_R64G64_UINT = 209,              // FORMAT_R64G64_UINT
        S_FORMAT_R64G64_SINT = 210,              // FORMAT_R64G64_SINT
        S_FORMAT_R64G64_SFLOAT = 211,            // FORMAT_R64G64_SFLOAT
        S_FORMAT_R64G64B64_UINT = 212,           // FORMAT_R64G64B64_UINT
        S_FORMAT_R64G64B64_SINT = 213,           // FORMAT_R64G64B64_SINT
        S_FORMAT_R64G64B64_SFLOAT = 214,         // FORMAT_R64G64B64_SFLOAT
        S_FORMAT_R64G64B64A64_UINT = 215,        // FORMAT_R64G64B64A64_UINT
        S_FORMAT_R64G64B64A64_SINT = 216,        // FORMAT_R64G64B64A64_SINT
        S_FORMAT_R64G64B64A64_SFLOAT = 217,      // FORMAT_R64G64B64A64_SFLOAT
        S_FORMAT_B10G11R11_UFLOAT_PACK32 = 218,  // FORMAT_B10G11R11_UFLOAT_PACK32
        S_FORMAT_E5B9G9R9_UFLOAT_PACK32 = 219,   // FORMAT_E5B9G9R9_UFLOAT_PACK32
        S_FORMAT_D16_UNORM = 220,                // FORMAT_D16_UNORM
        S_FORMAT_X8_D24_UNORM_PACK32 = 221,      // FORMAT_X8_D24_UNORM_PACK32
        S_FORMAT_D32_SFLOAT = 222,               // FORMAT_D32_SFLOAT
        S_FORMAT_S8_UINT = 223,                  // FORMAT_S8_UINT
        S_FORMAT_D16_UNORM_S8_UINT = 224,        // FORMAT_D16_UNORM_S8_UINT
        S_FORMAT_D24_UNORM_S8_UINT = 225,        // FORMAT_D24_UNORM_S8_UINT
        S_FORMAT_D32_SFLOAT_S8_UINT = 226,       // FORMAT_D32_SFLOAT_S8_UINT
        S_FORMAT_BC1_RGB_UNORM_BLOCK = 227,      // FORMAT_BC1_RGB_UNORM_BLOCK
        S_FORMAT_BC1_RGB_SRGB_BLOCK = 228,       // FORMAT_BC1_RGB_SRGB_BLOCK
        S_FORMAT_BC1_RGBA_UNORM_BLOCK = 229,     // FORMAT_BC1_RGBA_UNORM_BLOCK
        S_FORMAT_BC1_RGBA_SRGB_BLOCK = 230,      // FORMAT_BC1_RGBA_SRGB_BLOCK
        S_FORMAT_BC2_UNORM_BLOCK = 231,          // FORMAT_BC2_UNORM_BLOCK
        S_FORMAT_BC2_SRGB_BLOCK = 232,           // FORMAT_BC2_SRGB_BLOCK
        S_FORMAT_BC3_UNORM_BLOCK = 233,          // FORMAT_BC3_UNORM_BLOCK
        S_FORMAT_BC3_SRGB_BLOCK = 234,           // FORMAT_BC3_SRGB_BLOCK
        S_FORMAT_BC4_UNORM_BLOCK = 235,          // FORMAT_BC4_UNORM_BLOCK
        S_FORMAT_BC4_SNORM_BLOCK = 236,          // FORMAT_BC4_SNORM_BLOCK
        S_FORMAT_BC5_UNORM_BLOCK = 237,          // FORMAT_BC5_UNORM_BLOCK
        S_FORMAT_BC5_SNORM_BLOCK = 238,          // FORMAT_BC5_SNORM_BLOCK
        S_FORMAT_BC6H_UFLOAT_BLOCK = 239,        // FORMAT_BC6H_UFLOAT_BLOCK
        S_FORMAT_BC6H_SFLOAT_BLOCK = 240,        // FORMAT_BC6H_SFLOAT_BLOCK
        S_FORMAT_BC7_UNORM_BLOCK = 241,          // FORMAT_BC7_UNORM_BLOCK
        S_FORMAT_BC7_SRGB_BLOCK = 242,           // FORMAT_BC7_SRGB_BLOCK
        S_FORMAT_ETC2_R8G8B8_UNORM_BLOCK = 243,  // FORMAT_ETC2_R8G8B8_UNORM_BLOCK
        S_FORMAT_ETC2_R8G8B8_SRGB_BLOCK = 244,   // FORMAT_ETC2_R8G8B8_SRGB_BLOCK
        S_FORMAT_ETC2_R8G8B8A1_UNORM_BLOCK = 245, // FORMAT_ETC2_R8G8B8A1_UNORM_BLOCK
        S_FORMAT_ETC2_R8G8B8A1_SRGB_BLOCK = 246, // FORMAT_ETC2_R8G8B8A1_SRGB_BLOCK
        S_FORMAT_ETC2_R8G8B8A8_UNORM_BLOCK = 247, // FORMAT_ETC2_R8G8B8A8_UNORM_BLOCK
        S_FORMAT_ETC2_R8G8B8A8_SRGB_BLOCK = 248, // FORMAT_ETC2_R8G8B8A8_SRGB_BLOCK
        S_FORMAT_EAC_R11_UNORM_BLOCK = 249,      // FORMAT_EAC_R11_UNORM_BLOCK
        S_FORMAT_EAC_R11_SNORM_BLOCK = 250,      // FORMAT_EAC_R11_SNORM_BLOCK
        S_FORMAT_EAC_R11G11_UNORM_BLOCK = 251,   // FORMAT_EAC_R11G11_UNORM_BLOCK
        S_FORMAT_EAC_R11G11_SNORM_BLOCK = 252,   // FORMAT_EAC_R11G11_SNORM_BLOCK
        S_FORMAT_ASTC_4x4_UNORM_BLOCK = 253,     // FORMAT_ASTC_4x4_UNORM_BLOCK
        S_FORMAT_ASTC_4x4_SRGB_BLOCK = 254,      // FORMAT_ASTC_4x4_SRGB_BLOCK
        S_FORMAT_ASTC_5x4_UNORM_BLOCK = 255,     // FORMAT_ASTC_5x4_UNORM_BLOCK
        S_FORMAT_ASTC_5x4_SRGB_BLOCK = 256,      // FORMAT_ASTC_5x4_SRGB_BLOCK
        S_FORMAT_ASTC_5x5_UNORM_BLOCK = 257,     // FORMAT_ASTC_5x5_UNORM_BLOCK
        S_FORMAT_ASTC_5x5_SRGB_BLOCK = 258,      // FORMAT_ASTC_5x5_SRGB_BLOCK
        S_FORMAT_ASTC_6x5_UNORM_BLOCK = 259,     // FORMAT_ASTC_6x5_UNORM_BLOCK
        S_FORMAT_ASTC_6x5_SRGB_BLOCK = 260,      // FORMAT_ASTC_6x5_SRGB_BLOCK
        S_FORMAT_ASTC_6x6_UNORM_BLOCK = 261,     // FORMAT_ASTC_6x6_UNORM_BLOCK
        S_FORMAT_ASTC_6x6_SRGB_BLOCK = 262,      // FORMAT_ASTC_6x6_SRGB_BLOCK
        S_FORMAT_ASTC_8x5_UNORM_BLOCK = 263,     // FORMAT_ASTC_8x5_UNORM_BLOCK
        S_FORMAT_ASTC_8x5_SRGB_BLOCK = 264,      // FORMAT_ASTC_8x5_SRGB_BLOCK
        S_FORMAT_ASTC_8x6_UNORM_BLOCK = 265,     // FORMAT_ASTC_8x6_UNORM_BLOCK
        S_FORMAT_ASTC_8x6_SRGB_BLOCK = 266,      // FORMAT_ASTC_8x6_SRGB_BLOCK
        S_FORMAT_ASTC_8x8_UNORM_BLOCK = 267,     // FORMAT_ASTC_8x8_UNORM_BLOCK
        S_FORMAT_ASTC_8x8_SRGB_BLOCK = 268,      // FORMAT_ASTC_8x8_SRGB_BLOCK
        S_FORMAT_ASTC_10x5_UNORM_BLOCK = 269,    // FORMAT_ASTC_10x5_UNORM_BLOCK
        S_FORMAT_ASTC_10x5_SRGB_BLOCK = 270,     // FORMAT_ASTC_10x5_SRGB_BLOCK
        S_FORMAT_ASTC_10x6_UNORM_BLOCK = 271,    // FORMAT_ASTC_10x6_UNORM_BLOCK
        S_FORMAT_ASTC_10x6_SRGB_BLOCK = 272,     // FORMAT_ASTC_10x6_SRGB_BLOCK
        S_FORMAT_ASTC_10x8_UNORM_BLOCK = 273,    // FORMAT_ASTC_10x8_UNORM_BLOCK
        S_FORMAT_ASTC_10x8_SRGB_BLOCK = 274,     // FORMAT_ASTC_10x8_SRGB_BLOCK
        S_FORMAT_ASTC_10x10_UNORM_BLOCK = 275,   // FORMAT_ASTC_10x10_UNORM_BLOCK
        S_FORMAT_ASTC_10x10_SRGB_BLOCK = 276,    // FORMAT_ASTC_10x10_SRGB_BLOCK
        S_FORMAT_ASTC_12x10_UNORM_BLOCK = 277,   // FORMAT_ASTC_12x10_UNORM_BLOCK
        S_FORMAT_ASTC_12x10_SRGB_BLOCK = 278,    // FORMAT_ASTC_12x10_SRGB_BLOCK
        S_FORMAT_ASTC_12x12_UNORM_BLOCK = 279,   // FORMAT_ASTC_12x12_UNORM_BLOCK
        S_FORMAT_ASTC_12x12_SRGB_BLOCK = 280,    // FORMAT_ASTC_12x12_SRGB_BLOCK
        S_VERTEXFORMAT = 281,                    // VERTEXFORMAT
        S_282_tech_ = 282,                       // "tech"
        S_283_hlsl_ = 283,                       // "hlsl"
        S_284_texture_ = 284,                    // "texture"
        S_285_resource_ = 285,                   // "resource"
        S_286_fbo_ = 286,                        // "fbo"
        S_287_sstate_ = 287,                     // "sstate"
        S_288_dststate_ = 288,                   // "dststate"
        S_289_prstate_ = 289,                    // "prstate"
        S_290_csstate_ = 290,                    // "csstate"
        S_291_rstate_ = 291,                     // "rstate"
        S_292_namespace_ = 292,                  // "namespace"
        S_293_cstbuffer_ = 293,                  // "cstbuffer"
        S_294_uniforms_ = 294,                   // "uniforms"
        S_YYACCEPT = 295,                        // $accept
        S_input = 296,                           // input
        S_tech = 297,                            // tech
        S_glsl = 298,                            // glsl
        S_vertexformat = 299,                    // vertexformat
        S_format = 300,                          // format
        S_hlsl = 301,                            // hlsl
        S_texture = 302,                         // texture
        S_resource = 303,                        // resource
        S_fbo = 304,                             // fbo
        S_sstate = 305,                          // sstate
        S_dststate = 306,                        // dststate
        S_prstate = 307,                         // prstate
        S_csstate = 308,                         // csstate
        S_rstate = 309,                          // rstate
        S_namespace = 310,                       // namespace
        S_cstbuffer = 311,                       // cstbuffer
        S_uniforms = 312                         // uniforms
      };
    };

    /// (Internal) symbol kind.
    typedef symbol_kind::symbol_kind_type symbol_kind_type;

    /// The number of tokens.
    static const symbol_kind_type YYNTOKENS = symbol_kind::YYNTOKENS;

    /// A complete symbol.
    ///
    /// Expects its Base type to provide access to the symbol kind
    /// via kind ().
    ///
    /// Provide access to semantic value and location.
    template <typename Base>
    struct basic_symbol : Base
    {
      /// Alias to Base.
      typedef Base super_type;

      /// Default constructor.
      basic_symbol ()
        : value ()
        , location ()
      {}

#if 201103L <= YY_CPLUSPLUS
      /// Move constructor.
      basic_symbol (basic_symbol&& that)
        : Base (std::move (that))
        , value ()
        , location (std::move (that.location))
      {
        switch (this->kind ())
    {
      case symbol_kind::S_FORMAT_UNDEFINED: // FORMAT_UNDEFINED
      case symbol_kind::S_FORMAT_R4G4_UNORM_PACK8: // FORMAT_R4G4_UNORM_PACK8
      case symbol_kind::S_FORMAT_R4G4B4A4_UNORM_PACK16: // FORMAT_R4G4B4A4_UNORM_PACK16
      case symbol_kind::S_FORMAT_B4G4R4A4_UNORM_PACK16: // FORMAT_B4G4R4A4_UNORM_PACK16
      case symbol_kind::S_FORMAT_R5G6B5_UNORM_PACK16: // FORMAT_R5G6B5_UNORM_PACK16
      case symbol_kind::S_FORMAT_B5G6R5_UNORM_PACK16: // FORMAT_B5G6R5_UNORM_PACK16
      case symbol_kind::S_FORMAT_R5G5B5A1_UNORM_PACK16: // FORMAT_R5G5B5A1_UNORM_PACK16
      case symbol_kind::S_FORMAT_B5G5R5A1_UNORM_PACK16: // FORMAT_B5G5R5A1_UNORM_PACK16
      case symbol_kind::S_FORMAT_A1R5G5B5_UNORM_PACK16: // FORMAT_A1R5G5B5_UNORM_PACK16
      case symbol_kind::S_FORMAT_R8_UNORM: // FORMAT_R8_UNORM
      case symbol_kind::S_FORMAT_R8_SNORM: // FORMAT_R8_SNORM
      case symbol_kind::S_FORMAT_R8_USCALED: // FORMAT_R8_USCALED
      case symbol_kind::S_FORMAT_R8_SSCALED: // FORMAT_R8_SSCALED
      case symbol_kind::S_FORMAT_R8_UINT: // FORMAT_R8_UINT
      case symbol_kind::S_FORMAT_R8_SINT: // FORMAT_R8_SINT
      case symbol_kind::S_FORMAT_R8_SRGB: // FORMAT_R8_SRGB
      case symbol_kind::S_FORMAT_R8G8_UNORM: // FORMAT_R8G8_UNORM
      case symbol_kind::S_FORMAT_R8G8_SNORM: // FORMAT_R8G8_SNORM
      case symbol_kind::S_FORMAT_R8G8_USCALED: // FORMAT_R8G8_USCALED
      case symbol_kind::S_FORMAT_R8G8_SSCALED: // FORMAT_R8G8_SSCALED
      case symbol_kind::S_FORMAT_R8G8_UINT: // FORMAT_R8G8_UINT
      case symbol_kind::S_FORMAT_R8G8_SINT: // FORMAT_R8G8_SINT
      case symbol_kind::S_FORMAT_R8G8_SRGB: // FORMAT_R8G8_SRGB
      case symbol_kind::S_FORMAT_R8G8B8_UNORM: // FORMAT_R8G8B8_UNORM
      case symbol_kind::S_FORMAT_R8G8B8_SNORM: // FORMAT_R8G8B8_SNORM
      case symbol_kind::S_FORMAT_R8G8B8_USCALED: // FORMAT_R8G8B8_USCALED
      case symbol_kind::S_FORMAT_R8G8B8_SSCALED: // FORMAT_R8G8B8_SSCALED
      case symbol_kind::S_FORMAT_R8G8B8_UINT: // FORMAT_R8G8B8_UINT
      case symbol_kind::S_FORMAT_R8G8B8_SINT: // FORMAT_R8G8B8_SINT
      case symbol_kind::S_FORMAT_R8G8B8_SRGB: // FORMAT_R8G8B8_SRGB
      case symbol_kind::S_FORMAT_B8G8R8_UNORM: // FORMAT_B8G8R8_UNORM
      case symbol_kind::S_FORMAT_B8G8R8_SNORM: // FORMAT_B8G8R8_SNORM
      case symbol_kind::S_FORMAT_B8G8R8_USCALED: // FORMAT_B8G8R8_USCALED
      case symbol_kind::S_FORMAT_B8G8R8_SSCALED: // FORMAT_B8G8R8_SSCALED
      case symbol_kind::S_FORMAT_B8G8R8_UINT: // FORMAT_B8G8R8_UINT
      case symbol_kind::S_FORMAT_B8G8R8_SINT: // FORMAT_B8G8R8_SINT
      case symbol_kind::S_FORMAT_B8G8R8_SRGB: // FORMAT_B8G8R8_SRGB
      case symbol_kind::S_FORMAT_R8G8B8A8_UNORM: // FORMAT_R8G8B8A8_UNORM
      case symbol_kind::S_FORMAT_R8G8B8A8_SNORM: // FORMAT_R8G8B8A8_SNORM
      case symbol_kind::S_FORMAT_R8G8B8A8_USCALED: // FORMAT_R8G8B8A8_USCALED
      case symbol_kind::S_FORMAT_R8G8B8A8_SSCALED: // FORMAT_R8G8B8A8_SSCALED
      case symbol_kind::S_FORMAT_R8G8B8A8_UINT: // FORMAT_R8G8B8A8_UINT
      case symbol_kind::S_FORMAT_R8G8B8A8_SINT: // FORMAT_R8G8B8A8_SINT
      case symbol_kind::S_FORMAT_R8G8B8A8_SRGB: // FORMAT_R8G8B8A8_SRGB
      case symbol_kind::S_FORMAT_B8G8R8A8_UNORM: // FORMAT_B8G8R8A8_UNORM
      case symbol_kind::S_FORMAT_B8G8R8A8_SNORM: // FORMAT_B8G8R8A8_SNORM
      case symbol_kind::S_FORMAT_B8G8R8A8_USCALED: // FORMAT_B8G8R8A8_USCALED
      case symbol_kind::S_FORMAT_B8G8R8A8_SSCALED: // FORMAT_B8G8R8A8_SSCALED
      case symbol_kind::S_FORMAT_B8G8R8A8_UINT: // FORMAT_B8G8R8A8_UINT
      case symbol_kind::S_FORMAT_B8G8R8A8_SINT: // FORMAT_B8G8R8A8_SINT
      case symbol_kind::S_FORMAT_B8G8R8A8_SRGB: // FORMAT_B8G8R8A8_SRGB
      case symbol_kind::S_FORMAT_A8B8G8R8_UNORM_PACK32: // FORMAT_A8B8G8R8_UNORM_PACK32
      case symbol_kind::S_FORMAT_A8B8G8R8_SNORM_PACK32: // FORMAT_A8B8G8R8_SNORM_PACK32
      case symbol_kind::S_FORMAT_A8B8G8R8_USCALED_PACK32: // FORMAT_A8B8G8R8_USCALED_PACK32
      case symbol_kind::S_FORMAT_A8B8G8R8_SSCALED_PACK32: // FORMAT_A8B8G8R8_SSCALED_PACK32
      case symbol_kind::S_FORMAT_A8B8G8R8_UINT_PACK32: // FORMAT_A8B8G8R8_UINT_PACK32
      case symbol_kind::S_FORMAT_A8B8G8R8_SINT_PACK32: // FORMAT_A8B8G8R8_SINT_PACK32
      case symbol_kind::S_FORMAT_A8B8G8R8_SRGB_PACK32: // FORMAT_A8B8G8R8_SRGB_PACK32
      case symbol_kind::S_FORMAT_A2R10G10B10_UNORM_PACK32: // FORMAT_A2R10G10B10_UNORM_PACK32
      case symbol_kind::S_FORMAT_A2R10G10B10_SNORM_PACK32: // FORMAT_A2R10G10B10_SNORM_PACK32
      case symbol_kind::S_FORMAT_A2R10G10B10_USCALED_PACK32: // FORMAT_A2R10G10B10_USCALED_PACK32
      case symbol_kind::S_FORMAT_A2R10G10B10_SSCALED_PACK32: // FORMAT_A2R10G10B10_SSCALED_PACK32
      case symbol_kind::S_FORMAT_A2R10G10B10_UINT_PACK32: // FORMAT_A2R10G10B10_UINT_PACK32
      case symbol_kind::S_FORMAT_A2R10G10B10_SINT_PACK32: // FORMAT_A2R10G10B10_SINT_PACK32
      case symbol_kind::S_FORMAT_A2B10G10R10_UNORM_PACK32: // FORMAT_A2B10G10R10_UNORM_PACK32
      case symbol_kind::S_FORMAT_A2B10G10R10_SNORM_PACK32: // FORMAT_A2B10G10R10_SNORM_PACK32
      case symbol_kind::S_FORMAT_A2B10G10R10_USCALED_PACK32: // FORMAT_A2B10G10R10_USCALED_PACK32
      case symbol_kind::S_FORMAT_A2B10G10R10_SSCALED_PACK32: // FORMAT_A2B10G10R10_SSCALED_PACK32
      case symbol_kind::S_FORMAT_A2B10G10R10_UINT_PACK32: // FORMAT_A2B10G10R10_UINT_PACK32
      case symbol_kind::S_FORMAT_A2B10G10R10_SINT_PACK32: // FORMAT_A2B10G10R10_SINT_PACK32
      case symbol_kind::S_FORMAT_R16_UNORM: // FORMAT_R16_UNORM
      case symbol_kind::S_FORMAT_R16_SNORM: // FORMAT_R16_SNORM
      case symbol_kind::S_FORMAT_R16_USCALED: // FORMAT_R16_USCALED
      case symbol_kind::S_FORMAT_R16_SSCALED: // FORMAT_R16_SSCALED
      case symbol_kind::S_FORMAT_R16_UINT: // FORMAT_R16_UINT
      case symbol_kind::S_FORMAT_R16_SINT: // FORMAT_R16_SINT
      case symbol_kind::S_FORMAT_R16_SFLOAT: // FORMAT_R16_SFLOAT
      case symbol_kind::S_FORMAT_R16G16_UNORM: // FORMAT_R16G16_UNORM
      case symbol_kind::S_FORMAT_R16G16_SNORM: // FORMAT_R16G16_SNORM
      case symbol_kind::S_FORMAT_R16G16_USCALED: // FORMAT_R16G16_USCALED
      case symbol_kind::S_FORMAT_R16G16_SSCALED: // FORMAT_R16G16_SSCALED
      case symbol_kind::S_FORMAT_R16G16_UINT: // FORMAT_R16G16_UINT
      case symbol_kind::S_FORMAT_R16G16_SINT: // FORMAT_R16G16_SINT
      case symbol_kind::S_FORMAT_R16G16_SFLOAT: // FORMAT_R16G16_SFLOAT
      case symbol_kind::S_FORMAT_R16G16B16_UNORM: // FORMAT_R16G16B16_UNORM
      case symbol_kind::S_FORMAT_R16G16B16_SNORM: // FORMAT_R16G16B16_SNORM
      case symbol_kind::S_FORMAT_R16G16B16_USCALED: // FORMAT_R16G16B16_USCALED
      case symbol_kind::S_FORMAT_R16G16B16_SSCALED: // FORMAT_R16G16B16_SSCALED
      case symbol_kind::S_FORMAT_R16G16B16_UINT: // FORMAT_R16G16B16_UINT
      case symbol_kind::S_FORMAT_R16G16B16_SINT: // FORMAT_R16G16B16_SINT
      case symbol_kind::S_FORMAT_R16G16B16_SFLOAT: // FORMAT_R16G16B16_SFLOAT
      case symbol_kind::S_FORMAT_R16G16B16A16_UNORM: // FORMAT_R16G16B16A16_UNORM
      case symbol_kind::S_FORMAT_R16G16B16A16_SNORM: // FORMAT_R16G16B16A16_SNORM
      case symbol_kind::S_FORMAT_R16G16B16A16_USCALED: // FORMAT_R16G16B16A16_USCALED
      case symbol_kind::S_FORMAT_R16G16B16A16_SSCALED: // FORMAT_R16G16B16A16_SSCALED
      case symbol_kind::S_FORMAT_R16G16B16A16_UINT: // FORMAT_R16G16B16A16_UINT
      case symbol_kind::S_FORMAT_R16G16B16A16_SINT: // FORMAT_R16G16B16A16_SINT
      case symbol_kind::S_FORMAT_R16G16B16A16_SFLOAT: // FORMAT_R16G16B16A16_SFLOAT
      case symbol_kind::S_FORMAT_R32_UINT: // FORMAT_R32_UINT
      case symbol_kind::S_FORMAT_R32_SINT: // FORMAT_R32_SINT
      case symbol_kind::S_FORMAT_R32_SFLOAT: // FORMAT_R32_SFLOAT
      case symbol_kind::S_FORMAT_R32G32_UINT: // FORMAT_R32G32_UINT
      case symbol_kind::S_FORMAT_R32G32_SINT: // FORMAT_R32G32_SINT
      case symbol_kind::S_FORMAT_R32G32_SFLOAT: // FORMAT_R32G32_SFLOAT
      case symbol_kind::S_FORMAT_R32G32B32_UINT: // FORMAT_R32G32B32_UINT
      case symbol_kind::S_FORMAT_R32G32B32_SINT: // FORMAT_R32G32B32_SINT
      case symbol_kind::S_FORMAT_R32G32B32_SFLOAT: // FORMAT_R32G32B32_SFLOAT
      case symbol_kind::S_FORMAT_R32G32B32A32_UINT: // FORMAT_R32G32B32A32_UINT
      case symbol_kind::S_FORMAT_R32G32B32A32_SINT: // FORMAT_R32G32B32A32_SINT
      case symbol_kind::S_FORMAT_R32G32B32A32_SFLOAT: // FORMAT_R32G32B32A32_SFLOAT
      case symbol_kind::S_FORMAT_R64_UINT: // FORMAT_R64_UINT
      case symbol_kind::S_FORMAT_R64_SINT: // FORMAT_R64_SINT
      case symbol_kind::S_FORMAT_R64_SFLOAT: // FORMAT_R64_SFLOAT
      case symbol_kind::S_FORMAT_R64G64_UINT: // FORMAT_R64G64_UINT
      case symbol_kind::S_FORMAT_R64G64_SINT: // FORMAT_R64G64_SINT
      case symbol_kind::S_FORMAT_R64G64_SFLOAT: // FORMAT_R64G64_SFLOAT
      case symbol_kind::S_FORMAT_R64G64B64_UINT: // FORMAT_R64G64B64_UINT
      case symbol_kind::S_FORMAT_R64G64B64_SINT: // FORMAT_R64G64B64_SINT
      case symbol_kind::S_FORMAT_R64G64B64_SFLOAT: // FORMAT_R64G64B64_SFLOAT
      case symbol_kind::S_FORMAT_R64G64B64A64_UINT: // FORMAT_R64G64B64A64_UINT
      case symbol_kind::S_FORMAT_R64G64B64A64_SINT: // FORMAT_R64G64B64A64_SINT
      case symbol_kind::S_FORMAT_R64G64B64A64_SFLOAT: // FORMAT_R64G64B64A64_SFLOAT
      case symbol_kind::S_FORMAT_B10G11R11_UFLOAT_PACK32: // FORMAT_B10G11R11_UFLOAT_PACK32
      case symbol_kind::S_FORMAT_E5B9G9R9_UFLOAT_PACK32: // FORMAT_E5B9G9R9_UFLOAT_PACK32
      case symbol_kind::S_FORMAT_D16_UNORM: // FORMAT_D16_UNORM
      case symbol_kind::S_FORMAT_X8_D24_UNORM_PACK32: // FORMAT_X8_D24_UNORM_PACK32
      case symbol_kind::S_FORMAT_D32_SFLOAT: // FORMAT_D32_SFLOAT
      case symbol_kind::S_FORMAT_S8_UINT: // FORMAT_S8_UINT
      case symbol_kind::S_FORMAT_D16_UNORM_S8_UINT: // FORMAT_D16_UNORM_S8_UINT
      case symbol_kind::S_FORMAT_D24_UNORM_S8_UINT: // FORMAT_D24_UNORM_S8_UINT
      case symbol_kind::S_FORMAT_D32_SFLOAT_S8_UINT: // FORMAT_D32_SFLOAT_S8_UINT
      case symbol_kind::S_FORMAT_BC1_RGB_UNORM_BLOCK: // FORMAT_BC1_RGB_UNORM_BLOCK
      case symbol_kind::S_FORMAT_BC1_RGB_SRGB_BLOCK: // FORMAT_BC1_RGB_SRGB_BLOCK
      case symbol_kind::S_FORMAT_BC1_RGBA_UNORM_BLOCK: // FORMAT_BC1_RGBA_UNORM_BLOCK
      case symbol_kind::S_FORMAT_BC1_RGBA_SRGB_BLOCK: // FORMAT_BC1_RGBA_SRGB_BLOCK
      case symbol_kind::S_FORMAT_BC2_UNORM_BLOCK: // FORMAT_BC2_UNORM_BLOCK
      case symbol_kind::S_FORMAT_BC2_SRGB_BLOCK: // FORMAT_BC2_SRGB_BLOCK
      case symbol_kind::S_FORMAT_BC3_UNORM_BLOCK: // FORMAT_BC3_UNORM_BLOCK
      case symbol_kind::S_FORMAT_BC3_SRGB_BLOCK: // FORMAT_BC3_SRGB_BLOCK
      case symbol_kind::S_FORMAT_BC4_UNORM_BLOCK: // FORMAT_BC4_UNORM_BLOCK
      case symbol_kind::S_FORMAT_BC4_SNORM_BLOCK: // FORMAT_BC4_SNORM_BLOCK
      case symbol_kind::S_FORMAT_BC5_UNORM_BLOCK: // FORMAT_BC5_UNORM_BLOCK
      case symbol_kind::S_FORMAT_BC5_SNORM_BLOCK: // FORMAT_BC5_SNORM_BLOCK
      case symbol_kind::S_FORMAT_BC6H_UFLOAT_BLOCK: // FORMAT_BC6H_UFLOAT_BLOCK
      case symbol_kind::S_FORMAT_BC6H_SFLOAT_BLOCK: // FORMAT_BC6H_SFLOAT_BLOCK
      case symbol_kind::S_FORMAT_BC7_UNORM_BLOCK: // FORMAT_BC7_UNORM_BLOCK
      case symbol_kind::S_FORMAT_BC7_SRGB_BLOCK: // FORMAT_BC7_SRGB_BLOCK
      case symbol_kind::S_FORMAT_ETC2_R8G8B8_UNORM_BLOCK: // FORMAT_ETC2_R8G8B8_UNORM_BLOCK
      case symbol_kind::S_FORMAT_ETC2_R8G8B8_SRGB_BLOCK: // FORMAT_ETC2_R8G8B8_SRGB_BLOCK
      case symbol_kind::S_FORMAT_ETC2_R8G8B8A1_UNORM_BLOCK: // FORMAT_ETC2_R8G8B8A1_UNORM_BLOCK
      case symbol_kind::S_FORMAT_ETC2_R8G8B8A1_SRGB_BLOCK: // FORMAT_ETC2_R8G8B8A1_SRGB_BLOCK
      case symbol_kind::S_FORMAT_ETC2_R8G8B8A8_UNORM_BLOCK: // FORMAT_ETC2_R8G8B8A8_UNORM_BLOCK
      case symbol_kind::S_FORMAT_ETC2_R8G8B8A8_SRGB_BLOCK: // FORMAT_ETC2_R8G8B8A8_SRGB_BLOCK
      case symbol_kind::S_FORMAT_EAC_R11_UNORM_BLOCK: // FORMAT_EAC_R11_UNORM_BLOCK
      case symbol_kind::S_FORMAT_EAC_R11_SNORM_BLOCK: // FORMAT_EAC_R11_SNORM_BLOCK
      case symbol_kind::S_FORMAT_EAC_R11G11_UNORM_BLOCK: // FORMAT_EAC_R11G11_UNORM_BLOCK
      case symbol_kind::S_FORMAT_EAC_R11G11_SNORM_BLOCK: // FORMAT_EAC_R11G11_SNORM_BLOCK
      case symbol_kind::S_FORMAT_ASTC_4x4_UNORM_BLOCK: // FORMAT_ASTC_4x4_UNORM_BLOCK
      case symbol_kind::S_FORMAT_ASTC_4x4_SRGB_BLOCK: // FORMAT_ASTC_4x4_SRGB_BLOCK
      case symbol_kind::S_FORMAT_ASTC_5x4_UNORM_BLOCK: // FORMAT_ASTC_5x4_UNORM_BLOCK
      case symbol_kind::S_FORMAT_ASTC_5x4_SRGB_BLOCK: // FORMAT_ASTC_5x4_SRGB_BLOCK
      case symbol_kind::S_FORMAT_ASTC_5x5_UNORM_BLOCK: // FORMAT_ASTC_5x5_UNORM_BLOCK
      case symbol_kind::S_FORMAT_ASTC_5x5_SRGB_BLOCK: // FORMAT_ASTC_5x5_SRGB_BLOCK
      case symbol_kind::S_FORMAT_ASTC_6x5_UNORM_BLOCK: // FORMAT_ASTC_6x5_UNORM_BLOCK
      case symbol_kind::S_FORMAT_ASTC_6x5_SRGB_BLOCK: // FORMAT_ASTC_6x5_SRGB_BLOCK
      case symbol_kind::S_FORMAT_ASTC_6x6_UNORM_BLOCK: // FORMAT_ASTC_6x6_UNORM_BLOCK
      case symbol_kind::S_FORMAT_ASTC_6x6_SRGB_BLOCK: // FORMAT_ASTC_6x6_SRGB_BLOCK
      case symbol_kind::S_FORMAT_ASTC_8x5_UNORM_BLOCK: // FORMAT_ASTC_8x5_UNORM_BLOCK
      case symbol_kind::S_FORMAT_ASTC_8x5_SRGB_BLOCK: // FORMAT_ASTC_8x5_SRGB_BLOCK
      case symbol_kind::S_FORMAT_ASTC_8x6_UNORM_BLOCK: // FORMAT_ASTC_8x6_UNORM_BLOCK
      case symbol_kind::S_FORMAT_ASTC_8x6_SRGB_BLOCK: // FORMAT_ASTC_8x6_SRGB_BLOCK
      case symbol_kind::S_FORMAT_ASTC_8x8_UNORM_BLOCK: // FORMAT_ASTC_8x8_UNORM_BLOCK
      case symbol_kind::S_FORMAT_ASTC_8x8_SRGB_BLOCK: // FORMAT_ASTC_8x8_SRGB_BLOCK
      case symbol_kind::S_FORMAT_ASTC_10x5_UNORM_BLOCK: // FORMAT_ASTC_10x5_UNORM_BLOCK
      case symbol_kind::S_FORMAT_ASTC_10x5_SRGB_BLOCK: // FORMAT_ASTC_10x5_SRGB_BLOCK
      case symbol_kind::S_FORMAT_ASTC_10x6_UNORM_BLOCK: // FORMAT_ASTC_10x6_UNORM_BLOCK
      case symbol_kind::S_FORMAT_ASTC_10x6_SRGB_BLOCK: // FORMAT_ASTC_10x6_SRGB_BLOCK
      case symbol_kind::S_FORMAT_ASTC_10x8_UNORM_BLOCK: // FORMAT_ASTC_10x8_UNORM_BLOCK
      case symbol_kind::S_FORMAT_ASTC_10x8_SRGB_BLOCK: // FORMAT_ASTC_10x8_SRGB_BLOCK
      case symbol_kind::S_FORMAT_ASTC_10x10_UNORM_BLOCK: // FORMAT_ASTC_10x10_UNORM_BLOCK
      case symbol_kind::S_FORMAT_ASTC_10x10_SRGB_BLOCK: // FORMAT_ASTC_10x10_SRGB_BLOCK
      case symbol_kind::S_FORMAT_ASTC_12x10_UNORM_BLOCK: // FORMAT_ASTC_12x10_UNORM_BLOCK
      case symbol_kind::S_FORMAT_ASTC_12x10_SRGB_BLOCK: // FORMAT_ASTC_12x10_SRGB_BLOCK
      case symbol_kind::S_FORMAT_ASTC_12x12_UNORM_BLOCK: // FORMAT_ASTC_12x12_UNORM_BLOCK
      case symbol_kind::S_FORMAT_ASTC_12x12_SRGB_BLOCK: // FORMAT_ASTC_12x12_SRGB_BLOCK
      case symbol_kind::S_format: // format
        value.move< VkFormat > (std::move (that.value));
        break;

      case symbol_kind::S_TRUE: // TRUE
      case symbol_kind::S_FALSE: // FALSE
      case symbol_kind::S_BOOL: // BOOL
        value.move< bool > (std::move (that.value));
        break;

      case symbol_kind::S_FLOAT: // FLOAT
        value.move< float > (std::move (that.value));
        break;

      case symbol_kind::S_INT: // INT
        value.move< int > (std::move (that.value));
        break;

      case symbol_kind::S_IDENTIFIER: // IDENTIFIER
      case symbol_kind::S_STR: // STR
      case symbol_kind::S_CODEBODY: // CODEBODY
      case symbol_kind::S_VARNAME: // VARNAME
        value.move< std::string > (std::move (that.value));
        break;

      default:
        break;
    }

      }
#endif

      /// Copy constructor.
      basic_symbol (const basic_symbol& that);

      /// Constructor for valueless symbols, and symbols from each type.
#if 201103L <= YY_CPLUSPLUS
      basic_symbol (typename Base::kind_type t, location_type&& l)
        : Base (t)
        , location (std::move (l))
      {}
#else
      basic_symbol (typename Base::kind_type t, const location_type& l)
        : Base (t)
        , location (l)
      {}
#endif
#if 201103L <= YY_CPLUSPLUS
      basic_symbol (typename Base::kind_type t, VkFormat&& v, location_type&& l)
        : Base (t)
        , value (std::move (v))
        , location (std::move (l))
      {}
#else
      basic_symbol (typename Base::kind_type t, const VkFormat& v, const location_type& l)
        : Base (t)
        , value (v)
        , location (l)
      {}
#endif
#if 201103L <= YY_CPLUSPLUS
      basic_symbol (typename Base::kind_type t, bool&& v, location_type&& l)
        : Base (t)
        , value (std::move (v))
        , location (std::move (l))
      {}
#else
      basic_symbol (typename Base::kind_type t, const bool& v, const location_type& l)
        : Base (t)
        , value (v)
        , location (l)
      {}
#endif
#if 201103L <= YY_CPLUSPLUS
      basic_symbol (typename Base::kind_type t, float&& v, location_type&& l)
        : Base (t)
        , value (std::move (v))
        , location (std::move (l))
      {}
#else
      basic_symbol (typename Base::kind_type t, const float& v, const location_type& l)
        : Base (t)
        , value (v)
        , location (l)
      {}
#endif
#if 201103L <= YY_CPLUSPLUS
      basic_symbol (typename Base::kind_type t, int&& v, location_type&& l)
        : Base (t)
        , value (std::move (v))
        , location (std::move (l))
      {}
#else
      basic_symbol (typename Base::kind_type t, const int& v, const location_type& l)
        : Base (t)
        , value (v)
        , location (l)
      {}
#endif
#if 201103L <= YY_CPLUSPLUS
      basic_symbol (typename Base::kind_type t, std::string&& v, location_type&& l)
        : Base (t)
        , value (std::move (v))
        , location (std::move (l))
      {}
#else
      basic_symbol (typename Base::kind_type t, const std::string& v, const location_type& l)
        : Base (t)
        , value (v)
        , location (l)
      {}
#endif

      /// Destroy the symbol.
      ~basic_symbol ()
      {
        clear ();
      }

      /// Destroy contents, and record that is empty.
      void clear ()
      {
        // User destructor.
        symbol_kind_type yykind = this->kind ();
        basic_symbol<Base>& yysym = *this;
        (void) yysym;
        switch (yykind)
        {
       default:
          break;
        }

        // Value type destructor.
switch (yykind)
    {
      case symbol_kind::S_FORMAT_UNDEFINED: // FORMAT_UNDEFINED
      case symbol_kind::S_FORMAT_R4G4_UNORM_PACK8: // FORMAT_R4G4_UNORM_PACK8
      case symbol_kind::S_FORMAT_R4G4B4A4_UNORM_PACK16: // FORMAT_R4G4B4A4_UNORM_PACK16
      case symbol_kind::S_FORMAT_B4G4R4A4_UNORM_PACK16: // FORMAT_B4G4R4A4_UNORM_PACK16
      case symbol_kind::S_FORMAT_R5G6B5_UNORM_PACK16: // FORMAT_R5G6B5_UNORM_PACK16
      case symbol_kind::S_FORMAT_B5G6R5_UNORM_PACK16: // FORMAT_B5G6R5_UNORM_PACK16
      case symbol_kind::S_FORMAT_R5G5B5A1_UNORM_PACK16: // FORMAT_R5G5B5A1_UNORM_PACK16
      case symbol_kind::S_FORMAT_B5G5R5A1_UNORM_PACK16: // FORMAT_B5G5R5A1_UNORM_PACK16
      case symbol_kind::S_FORMAT_A1R5G5B5_UNORM_PACK16: // FORMAT_A1R5G5B5_UNORM_PACK16
      case symbol_kind::S_FORMAT_R8_UNORM: // FORMAT_R8_UNORM
      case symbol_kind::S_FORMAT_R8_SNORM: // FORMAT_R8_SNORM
      case symbol_kind::S_FORMAT_R8_USCALED: // FORMAT_R8_USCALED
      case symbol_kind::S_FORMAT_R8_SSCALED: // FORMAT_R8_SSCALED
      case symbol_kind::S_FORMAT_R8_UINT: // FORMAT_R8_UINT
      case symbol_kind::S_FORMAT_R8_SINT: // FORMAT_R8_SINT
      case symbol_kind::S_FORMAT_R8_SRGB: // FORMAT_R8_SRGB
      case symbol_kind::S_FORMAT_R8G8_UNORM: // FORMAT_R8G8_UNORM
      case symbol_kind::S_FORMAT_R8G8_SNORM: // FORMAT_R8G8_SNORM
      case symbol_kind::S_FORMAT_R8G8_USCALED: // FORMAT_R8G8_USCALED
      case symbol_kind::S_FORMAT_R8G8_SSCALED: // FORMAT_R8G8_SSCALED
      case symbol_kind::S_FORMAT_R8G8_UINT: // FORMAT_R8G8_UINT
      case symbol_kind::S_FORMAT_R8G8_SINT: // FORMAT_R8G8_SINT
      case symbol_kind::S_FORMAT_R8G8_SRGB: // FORMAT_R8G8_SRGB
      case symbol_kind::S_FORMAT_R8G8B8_UNORM: // FORMAT_R8G8B8_UNORM
      case symbol_kind::S_FORMAT_R8G8B8_SNORM: // FORMAT_R8G8B8_SNORM
      case symbol_kind::S_FORMAT_R8G8B8_USCALED: // FORMAT_R8G8B8_USCALED
      case symbol_kind::S_FORMAT_R8G8B8_SSCALED: // FORMAT_R8G8B8_SSCALED
      case symbol_kind::S_FORMAT_R8G8B8_UINT: // FORMAT_R8G8B8_UINT
      case symbol_kind::S_FORMAT_R8G8B8_SINT: // FORMAT_R8G8B8_SINT
      case symbol_kind::S_FORMAT_R8G8B8_SRGB: // FORMAT_R8G8B8_SRGB
      case symbol_kind::S_FORMAT_B8G8R8_UNORM: // FORMAT_B8G8R8_UNORM
      case symbol_kind::S_FORMAT_B8G8R8_SNORM: // FORMAT_B8G8R8_SNORM
      case symbol_kind::S_FORMAT_B8G8R8_USCALED: // FORMAT_B8G8R8_USCALED
      case symbol_kind::S_FORMAT_B8G8R8_SSCALED: // FORMAT_B8G8R8_SSCALED
      case symbol_kind::S_FORMAT_B8G8R8_UINT: // FORMAT_B8G8R8_UINT
      case symbol_kind::S_FORMAT_B8G8R8_SINT: // FORMAT_B8G8R8_SINT
      case symbol_kind::S_FORMAT_B8G8R8_SRGB: // FORMAT_B8G8R8_SRGB
      case symbol_kind::S_FORMAT_R8G8B8A8_UNORM: // FORMAT_R8G8B8A8_UNORM
      case symbol_kind::S_FORMAT_R8G8B8A8_SNORM: // FORMAT_R8G8B8A8_SNORM
      case symbol_kind::S_FORMAT_R8G8B8A8_USCALED: // FORMAT_R8G8B8A8_USCALED
      case symbol_kind::S_FORMAT_R8G8B8A8_SSCALED: // FORMAT_R8G8B8A8_SSCALED
      case symbol_kind::S_FORMAT_R8G8B8A8_UINT: // FORMAT_R8G8B8A8_UINT
      case symbol_kind::S_FORMAT_R8G8B8A8_SINT: // FORMAT_R8G8B8A8_SINT
      case symbol_kind::S_FORMAT_R8G8B8A8_SRGB: // FORMAT_R8G8B8A8_SRGB
      case symbol_kind::S_FORMAT_B8G8R8A8_UNORM: // FORMAT_B8G8R8A8_UNORM
      case symbol_kind::S_FORMAT_B8G8R8A8_SNORM: // FORMAT_B8G8R8A8_SNORM
      case symbol_kind::S_FORMAT_B8G8R8A8_USCALED: // FORMAT_B8G8R8A8_USCALED
      case symbol_kind::S_FORMAT_B8G8R8A8_SSCALED: // FORMAT_B8G8R8A8_SSCALED
      case symbol_kind::S_FORMAT_B8G8R8A8_UINT: // FORMAT_B8G8R8A8_UINT
      case symbol_kind::S_FORMAT_B8G8R8A8_SINT: // FORMAT_B8G8R8A8_SINT
      case symbol_kind::S_FORMAT_B8G8R8A8_SRGB: // FORMAT_B8G8R8A8_SRGB
      case symbol_kind::S_FORMAT_A8B8G8R8_UNORM_PACK32: // FORMAT_A8B8G8R8_UNORM_PACK32
      case symbol_kind::S_FORMAT_A8B8G8R8_SNORM_PACK32: // FORMAT_A8B8G8R8_SNORM_PACK32
      case symbol_kind::S_FORMAT_A8B8G8R8_USCALED_PACK32: // FORMAT_A8B8G8R8_USCALED_PACK32
      case symbol_kind::S_FORMAT_A8B8G8R8_SSCALED_PACK32: // FORMAT_A8B8G8R8_SSCALED_PACK32
      case symbol_kind::S_FORMAT_A8B8G8R8_UINT_PACK32: // FORMAT_A8B8G8R8_UINT_PACK32
      case symbol_kind::S_FORMAT_A8B8G8R8_SINT_PACK32: // FORMAT_A8B8G8R8_SINT_PACK32
      case symbol_kind::S_FORMAT_A8B8G8R8_SRGB_PACK32: // FORMAT_A8B8G8R8_SRGB_PACK32
      case symbol_kind::S_FORMAT_A2R10G10B10_UNORM_PACK32: // FORMAT_A2R10G10B10_UNORM_PACK32
      case symbol_kind::S_FORMAT_A2R10G10B10_SNORM_PACK32: // FORMAT_A2R10G10B10_SNORM_PACK32
      case symbol_kind::S_FORMAT_A2R10G10B10_USCALED_PACK32: // FORMAT_A2R10G10B10_USCALED_PACK32
      case symbol_kind::S_FORMAT_A2R10G10B10_SSCALED_PACK32: // FORMAT_A2R10G10B10_SSCALED_PACK32
      case symbol_kind::S_FORMAT_A2R10G10B10_UINT_PACK32: // FORMAT_A2R10G10B10_UINT_PACK32
      case symbol_kind::S_FORMAT_A2R10G10B10_SINT_PACK32: // FORMAT_A2R10G10B10_SINT_PACK32
      case symbol_kind::S_FORMAT_A2B10G10R10_UNORM_PACK32: // FORMAT_A2B10G10R10_UNORM_PACK32
      case symbol_kind::S_FORMAT_A2B10G10R10_SNORM_PACK32: // FORMAT_A2B10G10R10_SNORM_PACK32
      case symbol_kind::S_FORMAT_A2B10G10R10_USCALED_PACK32: // FORMAT_A2B10G10R10_USCALED_PACK32
      case symbol_kind::S_FORMAT_A2B10G10R10_SSCALED_PACK32: // FORMAT_A2B10G10R10_SSCALED_PACK32
      case symbol_kind::S_FORMAT_A2B10G10R10_UINT_PACK32: // FORMAT_A2B10G10R10_UINT_PACK32
      case symbol_kind::S_FORMAT_A2B10G10R10_SINT_PACK32: // FORMAT_A2B10G10R10_SINT_PACK32
      case symbol_kind::S_FORMAT_R16_UNORM: // FORMAT_R16_UNORM
      case symbol_kind::S_FORMAT_R16_SNORM: // FORMAT_R16_SNORM
      case symbol_kind::S_FORMAT_R16_USCALED: // FORMAT_R16_USCALED
      case symbol_kind::S_FORMAT_R16_SSCALED: // FORMAT_R16_SSCALED
      case symbol_kind::S_FORMAT_R16_UINT: // FORMAT_R16_UINT
      case symbol_kind::S_FORMAT_R16_SINT: // FORMAT_R16_SINT
      case symbol_kind::S_FORMAT_R16_SFLOAT: // FORMAT_R16_SFLOAT
      case symbol_kind::S_FORMAT_R16G16_UNORM: // FORMAT_R16G16_UNORM
      case symbol_kind::S_FORMAT_R16G16_SNORM: // FORMAT_R16G16_SNORM
      case symbol_kind::S_FORMAT_R16G16_USCALED: // FORMAT_R16G16_USCALED
      case symbol_kind::S_FORMAT_R16G16_SSCALED: // FORMAT_R16G16_SSCALED
      case symbol_kind::S_FORMAT_R16G16_UINT: // FORMAT_R16G16_UINT
      case symbol_kind::S_FORMAT_R16G16_SINT: // FORMAT_R16G16_SINT
      case symbol_kind::S_FORMAT_R16G16_SFLOAT: // FORMAT_R16G16_SFLOAT
      case symbol_kind::S_FORMAT_R16G16B16_UNORM: // FORMAT_R16G16B16_UNORM
      case symbol_kind::S_FORMAT_R16G16B16_SNORM: // FORMAT_R16G16B16_SNORM
      case symbol_kind::S_FORMAT_R16G16B16_USCALED: // FORMAT_R16G16B16_USCALED
      case symbol_kind::S_FORMAT_R16G16B16_SSCALED: // FORMAT_R16G16B16_SSCALED
      case symbol_kind::S_FORMAT_R16G16B16_UINT: // FORMAT_R16G16B16_UINT
      case symbol_kind::S_FORMAT_R16G16B16_SINT: // FORMAT_R16G16B16_SINT
      case symbol_kind::S_FORMAT_R16G16B16_SFLOAT: // FORMAT_R16G16B16_SFLOAT
      case symbol_kind::S_FORMAT_R16G16B16A16_UNORM: // FORMAT_R16G16B16A16_UNORM
      case symbol_kind::S_FORMAT_R16G16B16A16_SNORM: // FORMAT_R16G16B16A16_SNORM
      case symbol_kind::S_FORMAT_R16G16B16A16_USCALED: // FORMAT_R16G16B16A16_USCALED
      case symbol_kind::S_FORMAT_R16G16B16A16_SSCALED: // FORMAT_R16G16B16A16_SSCALED
      case symbol_kind::S_FORMAT_R16G16B16A16_UINT: // FORMAT_R16G16B16A16_UINT
      case symbol_kind::S_FORMAT_R16G16B16A16_SINT: // FORMAT_R16G16B16A16_SINT
      case symbol_kind::S_FORMAT_R16G16B16A16_SFLOAT: // FORMAT_R16G16B16A16_SFLOAT
      case symbol_kind::S_FORMAT_R32_UINT: // FORMAT_R32_UINT
      case symbol_kind::S_FORMAT_R32_SINT: // FORMAT_R32_SINT
      case symbol_kind::S_FORMAT_R32_SFLOAT: // FORMAT_R32_SFLOAT
      case symbol_kind::S_FORMAT_R32G32_UINT: // FORMAT_R32G32_UINT
      case symbol_kind::S_FORMAT_R32G32_SINT: // FORMAT_R32G32_SINT
      case symbol_kind::S_FORMAT_R32G32_SFLOAT: // FORMAT_R32G32_SFLOAT
      case symbol_kind::S_FORMAT_R32G32B32_UINT: // FORMAT_R32G32B32_UINT
      case symbol_kind::S_FORMAT_R32G32B32_SINT: // FORMAT_R32G32B32_SINT
      case symbol_kind::S_FORMAT_R32G32B32_SFLOAT: // FORMAT_R32G32B32_SFLOAT
      case symbol_kind::S_FORMAT_R32G32B32A32_UINT: // FORMAT_R32G32B32A32_UINT
      case symbol_kind::S_FORMAT_R32G32B32A32_SINT: // FORMAT_R32G32B32A32_SINT
      case symbol_kind::S_FORMAT_R32G32B32A32_SFLOAT: // FORMAT_R32G32B32A32_SFLOAT
      case symbol_kind::S_FORMAT_R64_UINT: // FORMAT_R64_UINT
      case symbol_kind::S_FORMAT_R64_SINT: // FORMAT_R64_SINT
      case symbol_kind::S_FORMAT_R64_SFLOAT: // FORMAT_R64_SFLOAT
      case symbol_kind::S_FORMAT_R64G64_UINT: // FORMAT_R64G64_UINT
      case symbol_kind::S_FORMAT_R64G64_SINT: // FORMAT_R64G64_SINT
      case symbol_kind::S_FORMAT_R64G64_SFLOAT: // FORMAT_R64G64_SFLOAT
      case symbol_kind::S_FORMAT_R64G64B64_UINT: // FORMAT_R64G64B64_UINT
      case symbol_kind::S_FORMAT_R64G64B64_SINT: // FORMAT_R64G64B64_SINT
      case symbol_kind::S_FORMAT_R64G64B64_SFLOAT: // FORMAT_R64G64B64_SFLOAT
      case symbol_kind::S_FORMAT_R64G64B64A64_UINT: // FORMAT_R64G64B64A64_UINT
      case symbol_kind::S_FORMAT_R64G64B64A64_SINT: // FORMAT_R64G64B64A64_SINT
      case symbol_kind::S_FORMAT_R64G64B64A64_SFLOAT: // FORMAT_R64G64B64A64_SFLOAT
      case symbol_kind::S_FORMAT_B10G11R11_UFLOAT_PACK32: // FORMAT_B10G11R11_UFLOAT_PACK32
      case symbol_kind::S_FORMAT_E5B9G9R9_UFLOAT_PACK32: // FORMAT_E5B9G9R9_UFLOAT_PACK32
      case symbol_kind::S_FORMAT_D16_UNORM: // FORMAT_D16_UNORM
      case symbol_kind::S_FORMAT_X8_D24_UNORM_PACK32: // FORMAT_X8_D24_UNORM_PACK32
      case symbol_kind::S_FORMAT_D32_SFLOAT: // FORMAT_D32_SFLOAT
      case symbol_kind::S_FORMAT_S8_UINT: // FORMAT_S8_UINT
      case symbol_kind::S_FORMAT_D16_UNORM_S8_UINT: // FORMAT_D16_UNORM_S8_UINT
      case symbol_kind::S_FORMAT_D24_UNORM_S8_UINT: // FORMAT_D24_UNORM_S8_UINT
      case symbol_kind::S_FORMAT_D32_SFLOAT_S8_UINT: // FORMAT_D32_SFLOAT_S8_UINT
      case symbol_kind::S_FORMAT_BC1_RGB_UNORM_BLOCK: // FORMAT_BC1_RGB_UNORM_BLOCK
      case symbol_kind::S_FORMAT_BC1_RGB_SRGB_BLOCK: // FORMAT_BC1_RGB_SRGB_BLOCK
      case symbol_kind::S_FORMAT_BC1_RGBA_UNORM_BLOCK: // FORMAT_BC1_RGBA_UNORM_BLOCK
      case symbol_kind::S_FORMAT_BC1_RGBA_SRGB_BLOCK: // FORMAT_BC1_RGBA_SRGB_BLOCK
      case symbol_kind::S_FORMAT_BC2_UNORM_BLOCK: // FORMAT_BC2_UNORM_BLOCK
      case symbol_kind::S_FORMAT_BC2_SRGB_BLOCK: // FORMAT_BC2_SRGB_BLOCK
      case symbol_kind::S_FORMAT_BC3_UNORM_BLOCK: // FORMAT_BC3_UNORM_BLOCK
      case symbol_kind::S_FORMAT_BC3_SRGB_BLOCK: // FORMAT_BC3_SRGB_BLOCK
      case symbol_kind::S_FORMAT_BC4_UNORM_BLOCK: // FORMAT_BC4_UNORM_BLOCK
      case symbol_kind::S_FORMAT_BC4_SNORM_BLOCK: // FORMAT_BC4_SNORM_BLOCK
      case symbol_kind::S_FORMAT_BC5_UNORM_BLOCK: // FORMAT_BC5_UNORM_BLOCK
      case symbol_kind::S_FORMAT_BC5_SNORM_BLOCK: // FORMAT_BC5_SNORM_BLOCK
      case symbol_kind::S_FORMAT_BC6H_UFLOAT_BLOCK: // FORMAT_BC6H_UFLOAT_BLOCK
      case symbol_kind::S_FORMAT_BC6H_SFLOAT_BLOCK: // FORMAT_BC6H_SFLOAT_BLOCK
      case symbol_kind::S_FORMAT_BC7_UNORM_BLOCK: // FORMAT_BC7_UNORM_BLOCK
      case symbol_kind::S_FORMAT_BC7_SRGB_BLOCK: // FORMAT_BC7_SRGB_BLOCK
      case symbol_kind::S_FORMAT_ETC2_R8G8B8_UNORM_BLOCK: // FORMAT_ETC2_R8G8B8_UNORM_BLOCK
      case symbol_kind::S_FORMAT_ETC2_R8G8B8_SRGB_BLOCK: // FORMAT_ETC2_R8G8B8_SRGB_BLOCK
      case symbol_kind::S_FORMAT_ETC2_R8G8B8A1_UNORM_BLOCK: // FORMAT_ETC2_R8G8B8A1_UNORM_BLOCK
      case symbol_kind::S_FORMAT_ETC2_R8G8B8A1_SRGB_BLOCK: // FORMAT_ETC2_R8G8B8A1_SRGB_BLOCK
      case symbol_kind::S_FORMAT_ETC2_R8G8B8A8_UNORM_BLOCK: // FORMAT_ETC2_R8G8B8A8_UNORM_BLOCK
      case symbol_kind::S_FORMAT_ETC2_R8G8B8A8_SRGB_BLOCK: // FORMAT_ETC2_R8G8B8A8_SRGB_BLOCK
      case symbol_kind::S_FORMAT_EAC_R11_UNORM_BLOCK: // FORMAT_EAC_R11_UNORM_BLOCK
      case symbol_kind::S_FORMAT_EAC_R11_SNORM_BLOCK: // FORMAT_EAC_R11_SNORM_BLOCK
      case symbol_kind::S_FORMAT_EAC_R11G11_UNORM_BLOCK: // FORMAT_EAC_R11G11_UNORM_BLOCK
      case symbol_kind::S_FORMAT_EAC_R11G11_SNORM_BLOCK: // FORMAT_EAC_R11G11_SNORM_BLOCK
      case symbol_kind::S_FORMAT_ASTC_4x4_UNORM_BLOCK: // FORMAT_ASTC_4x4_UNORM_BLOCK
      case symbol_kind::S_FORMAT_ASTC_4x4_SRGB_BLOCK: // FORMAT_ASTC_4x4_SRGB_BLOCK
      case symbol_kind::S_FORMAT_ASTC_5x4_UNORM_BLOCK: // FORMAT_ASTC_5x4_UNORM_BLOCK
      case symbol_kind::S_FORMAT_ASTC_5x4_SRGB_BLOCK: // FORMAT_ASTC_5x4_SRGB_BLOCK
      case symbol_kind::S_FORMAT_ASTC_5x5_UNORM_BLOCK: // FORMAT_ASTC_5x5_UNORM_BLOCK
      case symbol_kind::S_FORMAT_ASTC_5x5_SRGB_BLOCK: // FORMAT_ASTC_5x5_SRGB_BLOCK
      case symbol_kind::S_FORMAT_ASTC_6x5_UNORM_BLOCK: // FORMAT_ASTC_6x5_UNORM_BLOCK
      case symbol_kind::S_FORMAT_ASTC_6x5_SRGB_BLOCK: // FORMAT_ASTC_6x5_SRGB_BLOCK
      case symbol_kind::S_FORMAT_ASTC_6x6_UNORM_BLOCK: // FORMAT_ASTC_6x6_UNORM_BLOCK
      case symbol_kind::S_FORMAT_ASTC_6x6_SRGB_BLOCK: // FORMAT_ASTC_6x6_SRGB_BLOCK
      case symbol_kind::S_FORMAT_ASTC_8x5_UNORM_BLOCK: // FORMAT_ASTC_8x5_UNORM_BLOCK
      case symbol_kind::S_FORMAT_ASTC_8x5_SRGB_BLOCK: // FORMAT_ASTC_8x5_SRGB_BLOCK
      case symbol_kind::S_FORMAT_ASTC_8x6_UNORM_BLOCK: // FORMAT_ASTC_8x6_UNORM_BLOCK
      case symbol_kind::S_FORMAT_ASTC_8x6_SRGB_BLOCK: // FORMAT_ASTC_8x6_SRGB_BLOCK
      case symbol_kind::S_FORMAT_ASTC_8x8_UNORM_BLOCK: // FORMAT_ASTC_8x8_UNORM_BLOCK
      case symbol_kind::S_FORMAT_ASTC_8x8_SRGB_BLOCK: // FORMAT_ASTC_8x8_SRGB_BLOCK
      case symbol_kind::S_FORMAT_ASTC_10x5_UNORM_BLOCK: // FORMAT_ASTC_10x5_UNORM_BLOCK
      case symbol_kind::S_FORMAT_ASTC_10x5_SRGB_BLOCK: // FORMAT_ASTC_10x5_SRGB_BLOCK
      case symbol_kind::S_FORMAT_ASTC_10x6_UNORM_BLOCK: // FORMAT_ASTC_10x6_UNORM_BLOCK
      case symbol_kind::S_FORMAT_ASTC_10x6_SRGB_BLOCK: // FORMAT_ASTC_10x6_SRGB_BLOCK
      case symbol_kind::S_FORMAT_ASTC_10x8_UNORM_BLOCK: // FORMAT_ASTC_10x8_UNORM_BLOCK
      case symbol_kind::S_FORMAT_ASTC_10x8_SRGB_BLOCK: // FORMAT_ASTC_10x8_SRGB_BLOCK
      case symbol_kind::S_FORMAT_ASTC_10x10_UNORM_BLOCK: // FORMAT_ASTC_10x10_UNORM_BLOCK
      case symbol_kind::S_FORMAT_ASTC_10x10_SRGB_BLOCK: // FORMAT_ASTC_10x10_SRGB_BLOCK
      case symbol_kind::S_FORMAT_ASTC_12x10_UNORM_BLOCK: // FORMAT_ASTC_12x10_UNORM_BLOCK
      case symbol_kind::S_FORMAT_ASTC_12x10_SRGB_BLOCK: // FORMAT_ASTC_12x10_SRGB_BLOCK
      case symbol_kind::S_FORMAT_ASTC_12x12_UNORM_BLOCK: // FORMAT_ASTC_12x12_UNORM_BLOCK
      case symbol_kind::S_FORMAT_ASTC_12x12_SRGB_BLOCK: // FORMAT_ASTC_12x12_SRGB_BLOCK
      case symbol_kind::S_format: // format
        value.template destroy< VkFormat > ();
        break;

      case symbol_kind::S_TRUE: // TRUE
      case symbol_kind::S_FALSE: // FALSE
      case symbol_kind::S_BOOL: // BOOL
        value.template destroy< bool > ();
        break;

      case symbol_kind::S_FLOAT: // FLOAT
        value.template destroy< float > ();
        break;

      case symbol_kind::S_INT: // INT
        value.template destroy< int > ();
        break;

      case symbol_kind::S_IDENTIFIER: // IDENTIFIER
      case symbol_kind::S_STR: // STR
      case symbol_kind::S_CODEBODY: // CODEBODY
      case symbol_kind::S_VARNAME: // VARNAME
        value.template destroy< std::string > ();
        break;

      default:
        break;
    }

        Base::clear ();
      }

      /// The user-facing name of this symbol.
      std::string name () const YY_NOEXCEPT
      {
        return parser::symbol_name (this->kind ());
      }

      /// Backward compatibility (Bison 3.6).
      symbol_kind_type type_get () const YY_NOEXCEPT;

      /// Whether empty.
      bool empty () const YY_NOEXCEPT;

      /// Destructive move, \a s is emptied into this.
      void move (basic_symbol& s);

      /// The semantic value.
      semantic_type value;

      /// The location.
      location_type location;

    private:
#if YY_CPLUSPLUS < 201103L
      /// Assignment operator.
      basic_symbol& operator= (const basic_symbol& that);
#endif
    };

    /// Type access provider for token (enum) based symbols.
    struct by_kind
    {
      /// Default constructor.
      by_kind ();

#if 201103L <= YY_CPLUSPLUS
      /// Move constructor.
      by_kind (by_kind&& that);
#endif

      /// Copy constructor.
      by_kind (const by_kind& that);

      /// The symbol kind as needed by the constructor.
      typedef token_kind_type kind_type;

      /// Constructor from (external) token numbers.
      by_kind (kind_type t);

      /// Record that this symbol is empty.
      void clear ();

      /// Steal the symbol kind from \a that.
      void move (by_kind& that);

      /// The (internal) type number (corresponding to \a type).
      /// \a empty when empty.
      symbol_kind_type kind () const YY_NOEXCEPT;

      /// Backward compatibility (Bison 3.6).
      symbol_kind_type type_get () const YY_NOEXCEPT;

      /// The symbol kind.
      /// \a S_YYEMPTY when empty.
      symbol_kind_type kind_;
    };

    /// Backward compatibility for a private implementation detail (Bison 3.6).
    typedef by_kind by_type;

    /// "External" symbols: returned by the scanner.
    struct symbol_type : basic_symbol<by_kind>
    {
      /// Superclass.
      typedef basic_symbol<by_kind> super_type;

      /// Empty symbol.
      symbol_type () {}

      /// Constructor for valueless symbols, and symbols from each type.
#if 201103L <= YY_CPLUSPLUS
      symbol_type (int tok, location_type l)
        : super_type(token_type (tok), std::move (l))
      {
        YY_ASSERT (tok == token::TOK_END || tok == token::TOK_YYerror || tok == token::TOK_YYUNDEF || tok == token::TOK_ASSIGN || tok == token::TOK_MINUS || tok == token::TOK_PLUS || tok == token::TOK_STAR || tok == token::TOK_SLASH || tok == token::TOK_LPAREN || tok == token::TOK_RPAREN || tok == token::TOK_LEFTSCOPE || tok == token::TOK_RIGHTSCOPE || tok == token::TOK_SEMICOLON || tok == token::TOK_COMMA || tok == token::TOK_GLSLSHADER || tok == token::TOK_HLSL10SHADER || tok == token::TOK_HLSL11SHADER || tok == token::TOK_CGSHADER || tok == token::TOK_SAMPLER_STATE || tok == token::TOK_DST_STATE || tok == token::TOK_PR_STATE || tok == token::TOK_COLOR_SAMPLE_STATE || tok == token::TOK_RASTERIZATION_STATE || tok == token::TOK_TECHNIQUE || tok == token::TOK_PASS || tok == token::TOK_TEXTURERESOURCE || tok == token::TOK_TEXTURERESOURCE1D || tok == token::TOK_TEXTURERESOURCE2D || tok == token::TOK_TEXTURERESOURCE3D || tok == token::TOK_TEXTURERESOURCERECT || tok == token::TOK_TEXTURERESOURCECUBE || tok == token::TOK_VOID_TYPE || tok == token::TOK_UNSIGNED || tok == token::TOK_HIGHP || tok == token::TOK_MEDIUMP || tok == token::TOK_LOWP || tok == token::TOK_UNIFORM || tok == token::TOK_CSTBUFFER || tok == token::TOK_FLOAT_TYPE || tok == token::TOK_FLOAT2_TYPE || tok == token::TOK_FLOAT3_TYPE || tok == token::TOK_FLOAT4_TYPE || tok == token::TOK_MAT2_TYPE || tok == token::TOK_MAT3_TYPE || tok == token::TOK_MAT4_TYPE || tok == token::TOK_BOOL_TYPE || tok == token::TOK_BOOL2_TYPE || tok == token::TOK_BOOL3_TYPE || tok == token::TOK_BOOL4_TYPE || tok == token::TOK_INT_TYPE || tok == token::TOK_INT2_TYPE || tok == token::TOK_INT3_TYPE || tok == token::TOK_INT4_TYPE || tok == token::TOK_TEXTURE1D_TYPE || tok == token::TOK_TEXTURE2D_TYPE || tok == token::TOK_TEXTURE2DSHADOW_TYPE || tok == token::TOK_TEXTURE2DRECT_TYPE || tok == token::TOK_TEXTURE3D_TYPE || tok == token::TOK_TEXTURECUBE_TYPE || tok == token::TOK_SAMPLER1D_TYPE || tok == token::TOK_SAMPLER2D_TYPE || tok == token::TOK_SAMPLER2DSHADOW_TYPE || tok == token::TOK_SAMPLER2DRECT_TYPE || tok == token::TOK_SAMPLER3D_TYPE || tok == token::TOK_SAMPLERCUBE_TYPE || tok == token::TOK_EXTENSION || tok == token::TOK_SEPARATE_SHADER || tok == token::TOK_VERTEXPROGRAM || tok == token::TOK_FRAGMENTPROGRAM || tok == token::TOK_GEOMETRYPROGRAM || tok == token::TOK_HULLPROGRAM || tok == token::TOK_EVALPROGRAM || tok == token::TOK_SHDPROFILE || tok == token::TOK_SAMPLERRESOURCE || tok == token::TOK_SAMPLERTEXUNIT || tok == token::TOK_SETSAMPLERSTATE || tok == token::TOK_SETDSTSTATE || tok == token::TOK_SETRASTERIZATIONSTATE || tok == token::TOK_SETCOLORSAMPLESTATE || tok == token::TOK_IMAGERESOURCE || tok == token::TOK_IMAGEUNIT || tok == token::TOK_IMAGEACCESS || tok == token::TOK_IMAGELAYER || tok == token::TOK_IMAGELAYERED || tok == token::TOK_WRITE_ONLY || tok == token::TOK_READ_ONLY || tok == token::TOK_READ_WRITE || tok == token::TOK_VERTEXFORMAT || tok == 537 || tok == 538 || tok == 539 || tok == 540 || tok == 541 || tok == 542 || tok == 543 || tok == 544 || tok == 545 || tok == 546 || tok == 547 || tok == 548 || tok == 549);
      }
#else
      symbol_type (int tok, const location_type& l)
        : super_type(token_type (tok), l)
      {
        YY_ASSERT (tok == token::TOK_END || tok == token::TOK_YYerror || tok == token::TOK_YYUNDEF || tok == token::TOK_ASSIGN || tok == token::TOK_MINUS || tok == token::TOK_PLUS || tok == token::TOK_STAR || tok == token::TOK_SLASH || tok == token::TOK_LPAREN || tok == token::TOK_RPAREN || tok == token::TOK_LEFTSCOPE || tok == token::TOK_RIGHTSCOPE || tok == token::TOK_SEMICOLON || tok == token::TOK_COMMA || tok == token::TOK_GLSLSHADER || tok == token::TOK_HLSL10SHADER || tok == token::TOK_HLSL11SHADER || tok == token::TOK_CGSHADER || tok == token::TOK_SAMPLER_STATE || tok == token::TOK_DST_STATE || tok == token::TOK_PR_STATE || tok == token::TOK_COLOR_SAMPLE_STATE || tok == token::TOK_RASTERIZATION_STATE || tok == token::TOK_TECHNIQUE || tok == token::TOK_PASS || tok == token::TOK_TEXTURERESOURCE || tok == token::TOK_TEXTURERESOURCE1D || tok == token::TOK_TEXTURERESOURCE2D || tok == token::TOK_TEXTURERESOURCE3D || tok == token::TOK_TEXTURERESOURCERECT || tok == token::TOK_TEXTURERESOURCECUBE || tok == token::TOK_VOID_TYPE || tok == token::TOK_UNSIGNED || tok == token::TOK_HIGHP || tok == token::TOK_MEDIUMP || tok == token::TOK_LOWP || tok == token::TOK_UNIFORM || tok == token::TOK_CSTBUFFER || tok == token::TOK_FLOAT_TYPE || tok == token::TOK_FLOAT2_TYPE || tok == token::TOK_FLOAT3_TYPE || tok == token::TOK_FLOAT4_TYPE || tok == token::TOK_MAT2_TYPE || tok == token::TOK_MAT3_TYPE || tok == token::TOK_MAT4_TYPE || tok == token::TOK_BOOL_TYPE || tok == token::TOK_BOOL2_TYPE || tok == token::TOK_BOOL3_TYPE || tok == token::TOK_BOOL4_TYPE || tok == token::TOK_INT_TYPE || tok == token::TOK_INT2_TYPE || tok == token::TOK_INT3_TYPE || tok == token::TOK_INT4_TYPE || tok == token::TOK_TEXTURE1D_TYPE || tok == token::TOK_TEXTURE2D_TYPE || tok == token::TOK_TEXTURE2DSHADOW_TYPE || tok == token::TOK_TEXTURE2DRECT_TYPE || tok == token::TOK_TEXTURE3D_TYPE || tok == token::TOK_TEXTURECUBE_TYPE || tok == token::TOK_SAMPLER1D_TYPE || tok == token::TOK_SAMPLER2D_TYPE || tok == token::TOK_SAMPLER2DSHADOW_TYPE || tok == token::TOK_SAMPLER2DRECT_TYPE || tok == token::TOK_SAMPLER3D_TYPE || tok == token::TOK_SAMPLERCUBE_TYPE || tok == token::TOK_EXTENSION || tok == token::TOK_SEPARATE_SHADER || tok == token::TOK_VERTEXPROGRAM || tok == token::TOK_FRAGMENTPROGRAM || tok == token::TOK_GEOMETRYPROGRAM || tok == token::TOK_HULLPROGRAM || tok == token::TOK_EVALPROGRAM || tok == token::TOK_SHDPROFILE || tok == token::TOK_SAMPLERRESOURCE || tok == token::TOK_SAMPLERTEXUNIT || tok == token::TOK_SETSAMPLERSTATE || tok == token::TOK_SETDSTSTATE || tok == token::TOK_SETRASTERIZATIONSTATE || tok == token::TOK_SETCOLORSAMPLESTATE || tok == token::TOK_IMAGERESOURCE || tok == token::TOK_IMAGEUNIT || tok == token::TOK_IMAGEACCESS || tok == token::TOK_IMAGELAYER || tok == token::TOK_IMAGELAYERED || tok == token::TOK_WRITE_ONLY || tok == token::TOK_READ_ONLY || tok == token::TOK_READ_WRITE || tok == token::TOK_VERTEXFORMAT || tok == 537 || tok == 538 || tok == 539 || tok == 540 || tok == 541 || tok == 542 || tok == 543 || tok == 544 || tok == 545 || tok == 546 || tok == 547 || tok == 548 || tok == 549);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      symbol_type (int tok, VkFormat v, location_type l)
        : super_type(token_type (tok), std::move (v), std::move (l))
      {
        YY_ASSERT (tok == token::TOK_FORMAT_UNDEFINED || tok == token::TOK_FORMAT_R4G4_UNORM_PACK8 || tok == token::TOK_FORMAT_R4G4B4A4_UNORM_PACK16 || tok == token::TOK_FORMAT_B4G4R4A4_UNORM_PACK16 || tok == token::TOK_FORMAT_R5G6B5_UNORM_PACK16 || tok == token::TOK_FORMAT_B5G6R5_UNORM_PACK16 || tok == token::TOK_FORMAT_R5G5B5A1_UNORM_PACK16 || tok == token::TOK_FORMAT_B5G5R5A1_UNORM_PACK16 || tok == token::TOK_FORMAT_A1R5G5B5_UNORM_PACK16 || tok == token::TOK_FORMAT_R8_UNORM || tok == token::TOK_FORMAT_R8_SNORM || tok == token::TOK_FORMAT_R8_USCALED || tok == token::TOK_FORMAT_R8_SSCALED || tok == token::TOK_FORMAT_R8_UINT || tok == token::TOK_FORMAT_R8_SINT || tok == token::TOK_FORMAT_R8_SRGB || tok == token::TOK_FORMAT_R8G8_UNORM || tok == token::TOK_FORMAT_R8G8_SNORM || tok == token::TOK_FORMAT_R8G8_USCALED || tok == token::TOK_FORMAT_R8G8_SSCALED || tok == token::TOK_FORMAT_R8G8_UINT || tok == token::TOK_FORMAT_R8G8_SINT || tok == token::TOK_FORMAT_R8G8_SRGB || tok == token::TOK_FORMAT_R8G8B8_UNORM || tok == token::TOK_FORMAT_R8G8B8_SNORM || tok == token::TOK_FORMAT_R8G8B8_USCALED || tok == token::TOK_FORMAT_R8G8B8_SSCALED || tok == token::TOK_FORMAT_R8G8B8_UINT || tok == token::TOK_FORMAT_R8G8B8_SINT || tok == token::TOK_FORMAT_R8G8B8_SRGB || tok == token::TOK_FORMAT_B8G8R8_UNORM || tok == token::TOK_FORMAT_B8G8R8_SNORM || tok == token::TOK_FORMAT_B8G8R8_USCALED || tok == token::TOK_FORMAT_B8G8R8_SSCALED || tok == token::TOK_FORMAT_B8G8R8_UINT || tok == token::TOK_FORMAT_B8G8R8_SINT || tok == token::TOK_FORMAT_B8G8R8_SRGB || tok == token::TOK_FORMAT_R8G8B8A8_UNORM || tok == token::TOK_FORMAT_R8G8B8A8_SNORM || tok == token::TOK_FORMAT_R8G8B8A8_USCALED || tok == token::TOK_FORMAT_R8G8B8A8_SSCALED || tok == token::TOK_FORMAT_R8G8B8A8_UINT || tok == token::TOK_FORMAT_R8G8B8A8_SINT || tok == token::TOK_FORMAT_R8G8B8A8_SRGB || tok == token::TOK_FORMAT_B8G8R8A8_UNORM || tok == token::TOK_FORMAT_B8G8R8A8_SNORM || tok == token::TOK_FORMAT_B8G8R8A8_USCALED || tok == token::TOK_FORMAT_B8G8R8A8_SSCALED || tok == token::TOK_FORMAT_B8G8R8A8_UINT || tok == token::TOK_FORMAT_B8G8R8A8_SINT || tok == token::TOK_FORMAT_B8G8R8A8_SRGB || tok == token::TOK_FORMAT_A8B8G8R8_UNORM_PACK32 || tok == token::TOK_FORMAT_A8B8G8R8_SNORM_PACK32 || tok == token::TOK_FORMAT_A8B8G8R8_USCALED_PACK32 || tok == token::TOK_FORMAT_A8B8G8R8_SSCALED_PACK32 || tok == token::TOK_FORMAT_A8B8G8R8_UINT_PACK32 || tok == token::TOK_FORMAT_A8B8G8R8_SINT_PACK32 || tok == token::TOK_FORMAT_A8B8G8R8_SRGB_PACK32 || tok == token::TOK_FORMAT_A2R10G10B10_UNORM_PACK32 || tok == token::TOK_FORMAT_A2R10G10B10_SNORM_PACK32 || tok == token::TOK_FORMAT_A2R10G10B10_USCALED_PACK32 || tok == token::TOK_FORMAT_A2R10G10B10_SSCALED_PACK32 || tok == token::TOK_FORMAT_A2R10G10B10_UINT_PACK32 || tok == token::TOK_FORMAT_A2R10G10B10_SINT_PACK32 || tok == token::TOK_FORMAT_A2B10G10R10_UNORM_PACK32 || tok == token::TOK_FORMAT_A2B10G10R10_SNORM_PACK32 || tok == token::TOK_FORMAT_A2B10G10R10_USCALED_PACK32 || tok == token::TOK_FORMAT_A2B10G10R10_SSCALED_PACK32 || tok == token::TOK_FORMAT_A2B10G10R10_UINT_PACK32 || tok == token::TOK_FORMAT_A2B10G10R10_SINT_PACK32 || tok == token::TOK_FORMAT_R16_UNORM || tok == token::TOK_FORMAT_R16_SNORM || tok == token::TOK_FORMAT_R16_USCALED || tok == token::TOK_FORMAT_R16_SSCALED || tok == token::TOK_FORMAT_R16_UINT || tok == token::TOK_FORMAT_R16_SINT || tok == token::TOK_FORMAT_R16_SFLOAT || tok == token::TOK_FORMAT_R16G16_UNORM || tok == token::TOK_FORMAT_R16G16_SNORM || tok == token::TOK_FORMAT_R16G16_USCALED || tok == token::TOK_FORMAT_R16G16_SSCALED || tok == token::TOK_FORMAT_R16G16_UINT || tok == token::TOK_FORMAT_R16G16_SINT || tok == token::TOK_FORMAT_R16G16_SFLOAT || tok == token::TOK_FORMAT_R16G16B16_UNORM || tok == token::TOK_FORMAT_R16G16B16_SNORM || tok == token::TOK_FORMAT_R16G16B16_USCALED || tok == token::TOK_FORMAT_R16G16B16_SSCALED || tok == token::TOK_FORMAT_R16G16B16_UINT || tok == token::TOK_FORMAT_R16G16B16_SINT || tok == token::TOK_FORMAT_R16G16B16_SFLOAT || tok == token::TOK_FORMAT_R16G16B16A16_UNORM || tok == token::TOK_FORMAT_R16G16B16A16_SNORM || tok == token::TOK_FORMAT_R16G16B16A16_USCALED || tok == token::TOK_FORMAT_R16G16B16A16_SSCALED || tok == token::TOK_FORMAT_R16G16B16A16_UINT || tok == token::TOK_FORMAT_R16G16B16A16_SINT || tok == token::TOK_FORMAT_R16G16B16A16_SFLOAT || tok == token::TOK_FORMAT_R32_UINT || tok == token::TOK_FORMAT_R32_SINT || tok == token::TOK_FORMAT_R32_SFLOAT || tok == token::TOK_FORMAT_R32G32_UINT || tok == token::TOK_FORMAT_R32G32_SINT || tok == token::TOK_FORMAT_R32G32_SFLOAT || tok == token::TOK_FORMAT_R32G32B32_UINT || tok == token::TOK_FORMAT_R32G32B32_SINT || tok == token::TOK_FORMAT_R32G32B32_SFLOAT || tok == token::TOK_FORMAT_R32G32B32A32_UINT || tok == token::TOK_FORMAT_R32G32B32A32_SINT || tok == token::TOK_FORMAT_R32G32B32A32_SFLOAT || tok == token::TOK_FORMAT_R64_UINT || tok == token::TOK_FORMAT_R64_SINT || tok == token::TOK_FORMAT_R64_SFLOAT || tok == token::TOK_FORMAT_R64G64_UINT || tok == token::TOK_FORMAT_R64G64_SINT || tok == token::TOK_FORMAT_R64G64_SFLOAT || tok == token::TOK_FORMAT_R64G64B64_UINT || tok == token::TOK_FORMAT_R64G64B64_SINT || tok == token::TOK_FORMAT_R64G64B64_SFLOAT || tok == token::TOK_FORMAT_R64G64B64A64_UINT || tok == token::TOK_FORMAT_R64G64B64A64_SINT || tok == token::TOK_FORMAT_R64G64B64A64_SFLOAT || tok == token::TOK_FORMAT_B10G11R11_UFLOAT_PACK32 || tok == token::TOK_FORMAT_E5B9G9R9_UFLOAT_PACK32 || tok == token::TOK_FORMAT_D16_UNORM || tok == token::TOK_FORMAT_X8_D24_UNORM_PACK32 || tok == token::TOK_FORMAT_D32_SFLOAT || tok == token::TOK_FORMAT_S8_UINT || tok == token::TOK_FORMAT_D16_UNORM_S8_UINT || tok == token::TOK_FORMAT_D24_UNORM_S8_UINT || tok == token::TOK_FORMAT_D32_SFLOAT_S8_UINT || tok == token::TOK_FORMAT_BC1_RGB_UNORM_BLOCK || tok == token::TOK_FORMAT_BC1_RGB_SRGB_BLOCK || tok == token::TOK_FORMAT_BC1_RGBA_UNORM_BLOCK || tok == token::TOK_FORMAT_BC1_RGBA_SRGB_BLOCK || tok == token::TOK_FORMAT_BC2_UNORM_BLOCK || tok == token::TOK_FORMAT_BC2_SRGB_BLOCK || tok == token::TOK_FORMAT_BC3_UNORM_BLOCK || tok == token::TOK_FORMAT_BC3_SRGB_BLOCK || tok == token::TOK_FORMAT_BC4_UNORM_BLOCK || tok == token::TOK_FORMAT_BC4_SNORM_BLOCK || tok == token::TOK_FORMAT_BC5_UNORM_BLOCK || tok == token::TOK_FORMAT_BC5_SNORM_BLOCK || tok == token::TOK_FORMAT_BC6H_UFLOAT_BLOCK || tok == token::TOK_FORMAT_BC6H_SFLOAT_BLOCK || tok == token::TOK_FORMAT_BC7_UNORM_BLOCK || tok == token::TOK_FORMAT_BC7_SRGB_BLOCK || tok == token::TOK_FORMAT_ETC2_R8G8B8_UNORM_BLOCK || tok == token::TOK_FORMAT_ETC2_R8G8B8_SRGB_BLOCK || tok == token::TOK_FORMAT_ETC2_R8G8B8A1_UNORM_BLOCK || tok == token::TOK_FORMAT_ETC2_R8G8B8A1_SRGB_BLOCK || tok == token::TOK_FORMAT_ETC2_R8G8B8A8_UNORM_BLOCK || tok == token::TOK_FORMAT_ETC2_R8G8B8A8_SRGB_BLOCK || tok == token::TOK_FORMAT_EAC_R11_UNORM_BLOCK || tok == token::TOK_FORMAT_EAC_R11_SNORM_BLOCK || tok == token::TOK_FORMAT_EAC_R11G11_UNORM_BLOCK || tok == token::TOK_FORMAT_EAC_R11G11_SNORM_BLOCK || tok == token::TOK_FORMAT_ASTC_4x4_UNORM_BLOCK || tok == token::TOK_FORMAT_ASTC_4x4_SRGB_BLOCK || tok == token::TOK_FORMAT_ASTC_5x4_UNORM_BLOCK || tok == token::TOK_FORMAT_ASTC_5x4_SRGB_BLOCK || tok == token::TOK_FORMAT_ASTC_5x5_UNORM_BLOCK || tok == token::TOK_FORMAT_ASTC_5x5_SRGB_BLOCK || tok == token::TOK_FORMAT_ASTC_6x5_UNORM_BLOCK || tok == token::TOK_FORMAT_ASTC_6x5_SRGB_BLOCK || tok == token::TOK_FORMAT_ASTC_6x6_UNORM_BLOCK || tok == token::TOK_FORMAT_ASTC_6x6_SRGB_BLOCK || tok == token::TOK_FORMAT_ASTC_8x5_UNORM_BLOCK || tok == token::TOK_FORMAT_ASTC_8x5_SRGB_BLOCK || tok == token::TOK_FORMAT_ASTC_8x6_UNORM_BLOCK || tok == token::TOK_FORMAT_ASTC_8x6_SRGB_BLOCK || tok == token::TOK_FORMAT_ASTC_8x8_UNORM_BLOCK || tok == token::TOK_FORMAT_ASTC_8x8_SRGB_BLOCK || tok == token::TOK_FORMAT_ASTC_10x5_UNORM_BLOCK || tok == token::TOK_FORMAT_ASTC_10x5_SRGB_BLOCK || tok == token::TOK_FORMAT_ASTC_10x6_UNORM_BLOCK || tok == token::TOK_FORMAT_ASTC_10x6_SRGB_BLOCK || tok == token::TOK_FORMAT_ASTC_10x8_UNORM_BLOCK || tok == token::TOK_FORMAT_ASTC_10x8_SRGB_BLOCK || tok == token::TOK_FORMAT_ASTC_10x10_UNORM_BLOCK || tok == token::TOK_FORMAT_ASTC_10x10_SRGB_BLOCK || tok == token::TOK_FORMAT_ASTC_12x10_UNORM_BLOCK || tok == token::TOK_FORMAT_ASTC_12x10_SRGB_BLOCK || tok == token::TOK_FORMAT_ASTC_12x12_UNORM_BLOCK || tok == token::TOK_FORMAT_ASTC_12x12_SRGB_BLOCK);
      }
#else
      symbol_type (int tok, const VkFormat& v, const location_type& l)
        : super_type(token_type (tok), v, l)
      {
        YY_ASSERT (tok == token::TOK_FORMAT_UNDEFINED || tok == token::TOK_FORMAT_R4G4_UNORM_PACK8 || tok == token::TOK_FORMAT_R4G4B4A4_UNORM_PACK16 || tok == token::TOK_FORMAT_B4G4R4A4_UNORM_PACK16 || tok == token::TOK_FORMAT_R5G6B5_UNORM_PACK16 || tok == token::TOK_FORMAT_B5G6R5_UNORM_PACK16 || tok == token::TOK_FORMAT_R5G5B5A1_UNORM_PACK16 || tok == token::TOK_FORMAT_B5G5R5A1_UNORM_PACK16 || tok == token::TOK_FORMAT_A1R5G5B5_UNORM_PACK16 || tok == token::TOK_FORMAT_R8_UNORM || tok == token::TOK_FORMAT_R8_SNORM || tok == token::TOK_FORMAT_R8_USCALED || tok == token::TOK_FORMAT_R8_SSCALED || tok == token::TOK_FORMAT_R8_UINT || tok == token::TOK_FORMAT_R8_SINT || tok == token::TOK_FORMAT_R8_SRGB || tok == token::TOK_FORMAT_R8G8_UNORM || tok == token::TOK_FORMAT_R8G8_SNORM || tok == token::TOK_FORMAT_R8G8_USCALED || tok == token::TOK_FORMAT_R8G8_SSCALED || tok == token::TOK_FORMAT_R8G8_UINT || tok == token::TOK_FORMAT_R8G8_SINT || tok == token::TOK_FORMAT_R8G8_SRGB || tok == token::TOK_FORMAT_R8G8B8_UNORM || tok == token::TOK_FORMAT_R8G8B8_SNORM || tok == token::TOK_FORMAT_R8G8B8_USCALED || tok == token::TOK_FORMAT_R8G8B8_SSCALED || tok == token::TOK_FORMAT_R8G8B8_UINT || tok == token::TOK_FORMAT_R8G8B8_SINT || tok == token::TOK_FORMAT_R8G8B8_SRGB || tok == token::TOK_FORMAT_B8G8R8_UNORM || tok == token::TOK_FORMAT_B8G8R8_SNORM || tok == token::TOK_FORMAT_B8G8R8_USCALED || tok == token::TOK_FORMAT_B8G8R8_SSCALED || tok == token::TOK_FORMAT_B8G8R8_UINT || tok == token::TOK_FORMAT_B8G8R8_SINT || tok == token::TOK_FORMAT_B8G8R8_SRGB || tok == token::TOK_FORMAT_R8G8B8A8_UNORM || tok == token::TOK_FORMAT_R8G8B8A8_SNORM || tok == token::TOK_FORMAT_R8G8B8A8_USCALED || tok == token::TOK_FORMAT_R8G8B8A8_SSCALED || tok == token::TOK_FORMAT_R8G8B8A8_UINT || tok == token::TOK_FORMAT_R8G8B8A8_SINT || tok == token::TOK_FORMAT_R8G8B8A8_SRGB || tok == token::TOK_FORMAT_B8G8R8A8_UNORM || tok == token::TOK_FORMAT_B8G8R8A8_SNORM || tok == token::TOK_FORMAT_B8G8R8A8_USCALED || tok == token::TOK_FORMAT_B8G8R8A8_SSCALED || tok == token::TOK_FORMAT_B8G8R8A8_UINT || tok == token::TOK_FORMAT_B8G8R8A8_SINT || tok == token::TOK_FORMAT_B8G8R8A8_SRGB || tok == token::TOK_FORMAT_A8B8G8R8_UNORM_PACK32 || tok == token::TOK_FORMAT_A8B8G8R8_SNORM_PACK32 || tok == token::TOK_FORMAT_A8B8G8R8_USCALED_PACK32 || tok == token::TOK_FORMAT_A8B8G8R8_SSCALED_PACK32 || tok == token::TOK_FORMAT_A8B8G8R8_UINT_PACK32 || tok == token::TOK_FORMAT_A8B8G8R8_SINT_PACK32 || tok == token::TOK_FORMAT_A8B8G8R8_SRGB_PACK32 || tok == token::TOK_FORMAT_A2R10G10B10_UNORM_PACK32 || tok == token::TOK_FORMAT_A2R10G10B10_SNORM_PACK32 || tok == token::TOK_FORMAT_A2R10G10B10_USCALED_PACK32 || tok == token::TOK_FORMAT_A2R10G10B10_SSCALED_PACK32 || tok == token::TOK_FORMAT_A2R10G10B10_UINT_PACK32 || tok == token::TOK_FORMAT_A2R10G10B10_SINT_PACK32 || tok == token::TOK_FORMAT_A2B10G10R10_UNORM_PACK32 || tok == token::TOK_FORMAT_A2B10G10R10_SNORM_PACK32 || tok == token::TOK_FORMAT_A2B10G10R10_USCALED_PACK32 || tok == token::TOK_FORMAT_A2B10G10R10_SSCALED_PACK32 || tok == token::TOK_FORMAT_A2B10G10R10_UINT_PACK32 || tok == token::TOK_FORMAT_A2B10G10R10_SINT_PACK32 || tok == token::TOK_FORMAT_R16_UNORM || tok == token::TOK_FORMAT_R16_SNORM || tok == token::TOK_FORMAT_R16_USCALED || tok == token::TOK_FORMAT_R16_SSCALED || tok == token::TOK_FORMAT_R16_UINT || tok == token::TOK_FORMAT_R16_SINT || tok == token::TOK_FORMAT_R16_SFLOAT || tok == token::TOK_FORMAT_R16G16_UNORM || tok == token::TOK_FORMAT_R16G16_SNORM || tok == token::TOK_FORMAT_R16G16_USCALED || tok == token::TOK_FORMAT_R16G16_SSCALED || tok == token::TOK_FORMAT_R16G16_UINT || tok == token::TOK_FORMAT_R16G16_SINT || tok == token::TOK_FORMAT_R16G16_SFLOAT || tok == token::TOK_FORMAT_R16G16B16_UNORM || tok == token::TOK_FORMAT_R16G16B16_SNORM || tok == token::TOK_FORMAT_R16G16B16_USCALED || tok == token::TOK_FORMAT_R16G16B16_SSCALED || tok == token::TOK_FORMAT_R16G16B16_UINT || tok == token::TOK_FORMAT_R16G16B16_SINT || tok == token::TOK_FORMAT_R16G16B16_SFLOAT || tok == token::TOK_FORMAT_R16G16B16A16_UNORM || tok == token::TOK_FORMAT_R16G16B16A16_SNORM || tok == token::TOK_FORMAT_R16G16B16A16_USCALED || tok == token::TOK_FORMAT_R16G16B16A16_SSCALED || tok == token::TOK_FORMAT_R16G16B16A16_UINT || tok == token::TOK_FORMAT_R16G16B16A16_SINT || tok == token::TOK_FORMAT_R16G16B16A16_SFLOAT || tok == token::TOK_FORMAT_R32_UINT || tok == token::TOK_FORMAT_R32_SINT || tok == token::TOK_FORMAT_R32_SFLOAT || tok == token::TOK_FORMAT_R32G32_UINT || tok == token::TOK_FORMAT_R32G32_SINT || tok == token::TOK_FORMAT_R32G32_SFLOAT || tok == token::TOK_FORMAT_R32G32B32_UINT || tok == token::TOK_FORMAT_R32G32B32_SINT || tok == token::TOK_FORMAT_R32G32B32_SFLOAT || tok == token::TOK_FORMAT_R32G32B32A32_UINT || tok == token::TOK_FORMAT_R32G32B32A32_SINT || tok == token::TOK_FORMAT_R32G32B32A32_SFLOAT || tok == token::TOK_FORMAT_R64_UINT || tok == token::TOK_FORMAT_R64_SINT || tok == token::TOK_FORMAT_R64_SFLOAT || tok == token::TOK_FORMAT_R64G64_UINT || tok == token::TOK_FORMAT_R64G64_SINT || tok == token::TOK_FORMAT_R64G64_SFLOAT || tok == token::TOK_FORMAT_R64G64B64_UINT || tok == token::TOK_FORMAT_R64G64B64_SINT || tok == token::TOK_FORMAT_R64G64B64_SFLOAT || tok == token::TOK_FORMAT_R64G64B64A64_UINT || tok == token::TOK_FORMAT_R64G64B64A64_SINT || tok == token::TOK_FORMAT_R64G64B64A64_SFLOAT || tok == token::TOK_FORMAT_B10G11R11_UFLOAT_PACK32 || tok == token::TOK_FORMAT_E5B9G9R9_UFLOAT_PACK32 || tok == token::TOK_FORMAT_D16_UNORM || tok == token::TOK_FORMAT_X8_D24_UNORM_PACK32 || tok == token::TOK_FORMAT_D32_SFLOAT || tok == token::TOK_FORMAT_S8_UINT || tok == token::TOK_FORMAT_D16_UNORM_S8_UINT || tok == token::TOK_FORMAT_D24_UNORM_S8_UINT || tok == token::TOK_FORMAT_D32_SFLOAT_S8_UINT || tok == token::TOK_FORMAT_BC1_RGB_UNORM_BLOCK || tok == token::TOK_FORMAT_BC1_RGB_SRGB_BLOCK || tok == token::TOK_FORMAT_BC1_RGBA_UNORM_BLOCK || tok == token::TOK_FORMAT_BC1_RGBA_SRGB_BLOCK || tok == token::TOK_FORMAT_BC2_UNORM_BLOCK || tok == token::TOK_FORMAT_BC2_SRGB_BLOCK || tok == token::TOK_FORMAT_BC3_UNORM_BLOCK || tok == token::TOK_FORMAT_BC3_SRGB_BLOCK || tok == token::TOK_FORMAT_BC4_UNORM_BLOCK || tok == token::TOK_FORMAT_BC4_SNORM_BLOCK || tok == token::TOK_FORMAT_BC5_UNORM_BLOCK || tok == token::TOK_FORMAT_BC5_SNORM_BLOCK || tok == token::TOK_FORMAT_BC6H_UFLOAT_BLOCK || tok == token::TOK_FORMAT_BC6H_SFLOAT_BLOCK || tok == token::TOK_FORMAT_BC7_UNORM_BLOCK || tok == token::TOK_FORMAT_BC7_SRGB_BLOCK || tok == token::TOK_FORMAT_ETC2_R8G8B8_UNORM_BLOCK || tok == token::TOK_FORMAT_ETC2_R8G8B8_SRGB_BLOCK || tok == token::TOK_FORMAT_ETC2_R8G8B8A1_UNORM_BLOCK || tok == token::TOK_FORMAT_ETC2_R8G8B8A1_SRGB_BLOCK || tok == token::TOK_FORMAT_ETC2_R8G8B8A8_UNORM_BLOCK || tok == token::TOK_FORMAT_ETC2_R8G8B8A8_SRGB_BLOCK || tok == token::TOK_FORMAT_EAC_R11_UNORM_BLOCK || tok == token::TOK_FORMAT_EAC_R11_SNORM_BLOCK || tok == token::TOK_FORMAT_EAC_R11G11_UNORM_BLOCK || tok == token::TOK_FORMAT_EAC_R11G11_SNORM_BLOCK || tok == token::TOK_FORMAT_ASTC_4x4_UNORM_BLOCK || tok == token::TOK_FORMAT_ASTC_4x4_SRGB_BLOCK || tok == token::TOK_FORMAT_ASTC_5x4_UNORM_BLOCK || tok == token::TOK_FORMAT_ASTC_5x4_SRGB_BLOCK || tok == token::TOK_FORMAT_ASTC_5x5_UNORM_BLOCK || tok == token::TOK_FORMAT_ASTC_5x5_SRGB_BLOCK || tok == token::TOK_FORMAT_ASTC_6x5_UNORM_BLOCK || tok == token::TOK_FORMAT_ASTC_6x5_SRGB_BLOCK || tok == token::TOK_FORMAT_ASTC_6x6_UNORM_BLOCK || tok == token::TOK_FORMAT_ASTC_6x6_SRGB_BLOCK || tok == token::TOK_FORMAT_ASTC_8x5_UNORM_BLOCK || tok == token::TOK_FORMAT_ASTC_8x5_SRGB_BLOCK || tok == token::TOK_FORMAT_ASTC_8x6_UNORM_BLOCK || tok == token::TOK_FORMAT_ASTC_8x6_SRGB_BLOCK || tok == token::TOK_FORMAT_ASTC_8x8_UNORM_BLOCK || tok == token::TOK_FORMAT_ASTC_8x8_SRGB_BLOCK || tok == token::TOK_FORMAT_ASTC_10x5_UNORM_BLOCK || tok == token::TOK_FORMAT_ASTC_10x5_SRGB_BLOCK || tok == token::TOK_FORMAT_ASTC_10x6_UNORM_BLOCK || tok == token::TOK_FORMAT_ASTC_10x6_SRGB_BLOCK || tok == token::TOK_FORMAT_ASTC_10x8_UNORM_BLOCK || tok == token::TOK_FORMAT_ASTC_10x8_SRGB_BLOCK || tok == token::TOK_FORMAT_ASTC_10x10_UNORM_BLOCK || tok == token::TOK_FORMAT_ASTC_10x10_SRGB_BLOCK || tok == token::TOK_FORMAT_ASTC_12x10_UNORM_BLOCK || tok == token::TOK_FORMAT_ASTC_12x10_SRGB_BLOCK || tok == token::TOK_FORMAT_ASTC_12x12_UNORM_BLOCK || tok == token::TOK_FORMAT_ASTC_12x12_SRGB_BLOCK);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      symbol_type (int tok, bool v, location_type l)
        : super_type(token_type (tok), std::move (v), std::move (l))
      {
        YY_ASSERT (tok == token::TOK_TRUE || tok == token::TOK_FALSE || tok == token::TOK_BOOL);
      }
#else
      symbol_type (int tok, const bool& v, const location_type& l)
        : super_type(token_type (tok), v, l)
      {
        YY_ASSERT (tok == token::TOK_TRUE || tok == token::TOK_FALSE || tok == token::TOK_BOOL);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      symbol_type (int tok, float v, location_type l)
        : super_type(token_type (tok), std::move (v), std::move (l))
      {
        YY_ASSERT (tok == token::TOK_FLOAT);
      }
#else
      symbol_type (int tok, const float& v, const location_type& l)
        : super_type(token_type (tok), v, l)
      {
        YY_ASSERT (tok == token::TOK_FLOAT);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      symbol_type (int tok, int v, location_type l)
        : super_type(token_type (tok), std::move (v), std::move (l))
      {
        YY_ASSERT (tok == token::TOK_INT);
      }
#else
      symbol_type (int tok, const int& v, const location_type& l)
        : super_type(token_type (tok), v, l)
      {
        YY_ASSERT (tok == token::TOK_INT);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      symbol_type (int tok, std::string v, location_type l)
        : super_type(token_type (tok), std::move (v), std::move (l))
      {
        YY_ASSERT (tok == token::TOK_IDENTIFIER || tok == token::TOK_STR || tok == token::TOK_CODEBODY || tok == token::TOK_VARNAME);
      }
#else
      symbol_type (int tok, const std::string& v, const location_type& l)
        : super_type(token_type (tok), v, l)
      {
        YY_ASSERT (tok == token::TOK_IDENTIFIER || tok == token::TOK_STR || tok == token::TOK_CODEBODY || tok == token::TOK_VARNAME);
      }
#endif
    };

    /// Build a parser object.
    parser (Scanner &scanner_yyarg, Driver &driver_yyarg);
    virtual ~parser ();

#if 201103L <= YY_CPLUSPLUS
    /// Non copyable.
    parser (const parser&) = delete;
    /// Non copyable.
    parser& operator= (const parser&) = delete;
#endif

    /// Parse.  An alias for parse ().
    /// \returns  0 iff parsing succeeded.
    int operator() ();

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

    /// The user-facing name of the symbol whose (internal) number is
    /// YYSYMBOL.  No bounds checking.
    static std::string symbol_name (symbol_kind_type yysymbol);

    // Implementation of make_symbol for each symbol type.
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_END (location_type l)
      {
        return symbol_type (token::TOK_END, std::move (l));
      }
#else
      static
      symbol_type
      make_END (const location_type& l)
      {
        return symbol_type (token::TOK_END, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_YYerror (location_type l)
      {
        return symbol_type (token::TOK_YYerror, std::move (l));
      }
#else
      static
      symbol_type
      make_YYerror (const location_type& l)
      {
        return symbol_type (token::TOK_YYerror, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_YYUNDEF (location_type l)
      {
        return symbol_type (token::TOK_YYUNDEF, std::move (l));
      }
#else
      static
      symbol_type
      make_YYUNDEF (const location_type& l)
      {
        return symbol_type (token::TOK_YYUNDEF, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_ASSIGN (location_type l)
      {
        return symbol_type (token::TOK_ASSIGN, std::move (l));
      }
#else
      static
      symbol_type
      make_ASSIGN (const location_type& l)
      {
        return symbol_type (token::TOK_ASSIGN, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_MINUS (location_type l)
      {
        return symbol_type (token::TOK_MINUS, std::move (l));
      }
#else
      static
      symbol_type
      make_MINUS (const location_type& l)
      {
        return symbol_type (token::TOK_MINUS, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_PLUS (location_type l)
      {
        return symbol_type (token::TOK_PLUS, std::move (l));
      }
#else
      static
      symbol_type
      make_PLUS (const location_type& l)
      {
        return symbol_type (token::TOK_PLUS, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_STAR (location_type l)
      {
        return symbol_type (token::TOK_STAR, std::move (l));
      }
#else
      static
      symbol_type
      make_STAR (const location_type& l)
      {
        return symbol_type (token::TOK_STAR, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_SLASH (location_type l)
      {
        return symbol_type (token::TOK_SLASH, std::move (l));
      }
#else
      static
      symbol_type
      make_SLASH (const location_type& l)
      {
        return symbol_type (token::TOK_SLASH, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_LPAREN (location_type l)
      {
        return symbol_type (token::TOK_LPAREN, std::move (l));
      }
#else
      static
      symbol_type
      make_LPAREN (const location_type& l)
      {
        return symbol_type (token::TOK_LPAREN, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_RPAREN (location_type l)
      {
        return symbol_type (token::TOK_RPAREN, std::move (l));
      }
#else
      static
      symbol_type
      make_RPAREN (const location_type& l)
      {
        return symbol_type (token::TOK_RPAREN, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_LEFTSCOPE (location_type l)
      {
        return symbol_type (token::TOK_LEFTSCOPE, std::move (l));
      }
#else
      static
      symbol_type
      make_LEFTSCOPE (const location_type& l)
      {
        return symbol_type (token::TOK_LEFTSCOPE, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_RIGHTSCOPE (location_type l)
      {
        return symbol_type (token::TOK_RIGHTSCOPE, std::move (l));
      }
#else
      static
      symbol_type
      make_RIGHTSCOPE (const location_type& l)
      {
        return symbol_type (token::TOK_RIGHTSCOPE, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_SEMICOLON (location_type l)
      {
        return symbol_type (token::TOK_SEMICOLON, std::move (l));
      }
#else
      static
      symbol_type
      make_SEMICOLON (const location_type& l)
      {
        return symbol_type (token::TOK_SEMICOLON, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_COMMA (location_type l)
      {
        return symbol_type (token::TOK_COMMA, std::move (l));
      }
#else
      static
      symbol_type
      make_COMMA (const location_type& l)
      {
        return symbol_type (token::TOK_COMMA, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_IDENTIFIER (std::string v, location_type l)
      {
        return symbol_type (token::TOK_IDENTIFIER, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_IDENTIFIER (const std::string& v, const location_type& l)
      {
        return symbol_type (token::TOK_IDENTIFIER, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_TRUE (bool v, location_type l)
      {
        return symbol_type (token::TOK_TRUE, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_TRUE (const bool& v, const location_type& l)
      {
        return symbol_type (token::TOK_TRUE, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_FALSE (bool v, location_type l)
      {
        return symbol_type (token::TOK_FALSE, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_FALSE (const bool& v, const location_type& l)
      {
        return symbol_type (token::TOK_FALSE, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_FLOAT (float v, location_type l)
      {
        return symbol_type (token::TOK_FLOAT, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_FLOAT (const float& v, const location_type& l)
      {
        return symbol_type (token::TOK_FLOAT, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_INT (int v, location_type l)
      {
        return symbol_type (token::TOK_INT, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_INT (const int& v, const location_type& l)
      {
        return symbol_type (token::TOK_INT, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_BOOL (bool v, location_type l)
      {
        return symbol_type (token::TOK_BOOL, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_BOOL (const bool& v, const location_type& l)
      {
        return symbol_type (token::TOK_BOOL, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_STR (std::string v, location_type l)
      {
        return symbol_type (token::TOK_STR, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_STR (const std::string& v, const location_type& l)
      {
        return symbol_type (token::TOK_STR, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_GLSLSHADER (location_type l)
      {
        return symbol_type (token::TOK_GLSLSHADER, std::move (l));
      }
#else
      static
      symbol_type
      make_GLSLSHADER (const location_type& l)
      {
        return symbol_type (token::TOK_GLSLSHADER, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_HLSL10SHADER (location_type l)
      {
        return symbol_type (token::TOK_HLSL10SHADER, std::move (l));
      }
#else
      static
      symbol_type
      make_HLSL10SHADER (const location_type& l)
      {
        return symbol_type (token::TOK_HLSL10SHADER, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_HLSL11SHADER (location_type l)
      {
        return symbol_type (token::TOK_HLSL11SHADER, std::move (l));
      }
#else
      static
      symbol_type
      make_HLSL11SHADER (const location_type& l)
      {
        return symbol_type (token::TOK_HLSL11SHADER, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_CGSHADER (location_type l)
      {
        return symbol_type (token::TOK_CGSHADER, std::move (l));
      }
#else
      static
      symbol_type
      make_CGSHADER (const location_type& l)
      {
        return symbol_type (token::TOK_CGSHADER, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_SAMPLER_STATE (location_type l)
      {
        return symbol_type (token::TOK_SAMPLER_STATE, std::move (l));
      }
#else
      static
      symbol_type
      make_SAMPLER_STATE (const location_type& l)
      {
        return symbol_type (token::TOK_SAMPLER_STATE, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_DST_STATE (location_type l)
      {
        return symbol_type (token::TOK_DST_STATE, std::move (l));
      }
#else
      static
      symbol_type
      make_DST_STATE (const location_type& l)
      {
        return symbol_type (token::TOK_DST_STATE, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_PR_STATE (location_type l)
      {
        return symbol_type (token::TOK_PR_STATE, std::move (l));
      }
#else
      static
      symbol_type
      make_PR_STATE (const location_type& l)
      {
        return symbol_type (token::TOK_PR_STATE, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_COLOR_SAMPLE_STATE (location_type l)
      {
        return symbol_type (token::TOK_COLOR_SAMPLE_STATE, std::move (l));
      }
#else
      static
      symbol_type
      make_COLOR_SAMPLE_STATE (const location_type& l)
      {
        return symbol_type (token::TOK_COLOR_SAMPLE_STATE, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_RASTERIZATION_STATE (location_type l)
      {
        return symbol_type (token::TOK_RASTERIZATION_STATE, std::move (l));
      }
#else
      static
      symbol_type
      make_RASTERIZATION_STATE (const location_type& l)
      {
        return symbol_type (token::TOK_RASTERIZATION_STATE, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_TECHNIQUE (location_type l)
      {
        return symbol_type (token::TOK_TECHNIQUE, std::move (l));
      }
#else
      static
      symbol_type
      make_TECHNIQUE (const location_type& l)
      {
        return symbol_type (token::TOK_TECHNIQUE, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_PASS (location_type l)
      {
        return symbol_type (token::TOK_PASS, std::move (l));
      }
#else
      static
      symbol_type
      make_PASS (const location_type& l)
      {
        return symbol_type (token::TOK_PASS, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_CODEBODY (std::string v, location_type l)
      {
        return symbol_type (token::TOK_CODEBODY, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_CODEBODY (const std::string& v, const location_type& l)
      {
        return symbol_type (token::TOK_CODEBODY, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_VARNAME (std::string v, location_type l)
      {
        return symbol_type (token::TOK_VARNAME, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_VARNAME (const std::string& v, const location_type& l)
      {
        return symbol_type (token::TOK_VARNAME, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_TEXTURERESOURCE (location_type l)
      {
        return symbol_type (token::TOK_TEXTURERESOURCE, std::move (l));
      }
#else
      static
      symbol_type
      make_TEXTURERESOURCE (const location_type& l)
      {
        return symbol_type (token::TOK_TEXTURERESOURCE, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_TEXTURERESOURCE1D (location_type l)
      {
        return symbol_type (token::TOK_TEXTURERESOURCE1D, std::move (l));
      }
#else
      static
      symbol_type
      make_TEXTURERESOURCE1D (const location_type& l)
      {
        return symbol_type (token::TOK_TEXTURERESOURCE1D, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_TEXTURERESOURCE2D (location_type l)
      {
        return symbol_type (token::TOK_TEXTURERESOURCE2D, std::move (l));
      }
#else
      static
      symbol_type
      make_TEXTURERESOURCE2D (const location_type& l)
      {
        return symbol_type (token::TOK_TEXTURERESOURCE2D, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_TEXTURERESOURCE3D (location_type l)
      {
        return symbol_type (token::TOK_TEXTURERESOURCE3D, std::move (l));
      }
#else
      static
      symbol_type
      make_TEXTURERESOURCE3D (const location_type& l)
      {
        return symbol_type (token::TOK_TEXTURERESOURCE3D, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_TEXTURERESOURCERECT (location_type l)
      {
        return symbol_type (token::TOK_TEXTURERESOURCERECT, std::move (l));
      }
#else
      static
      symbol_type
      make_TEXTURERESOURCERECT (const location_type& l)
      {
        return symbol_type (token::TOK_TEXTURERESOURCERECT, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_TEXTURERESOURCECUBE (location_type l)
      {
        return symbol_type (token::TOK_TEXTURERESOURCECUBE, std::move (l));
      }
#else
      static
      symbol_type
      make_TEXTURERESOURCECUBE (const location_type& l)
      {
        return symbol_type (token::TOK_TEXTURERESOURCECUBE, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_VOID_TYPE (location_type l)
      {
        return symbol_type (token::TOK_VOID_TYPE, std::move (l));
      }
#else
      static
      symbol_type
      make_VOID_TYPE (const location_type& l)
      {
        return symbol_type (token::TOK_VOID_TYPE, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_UNSIGNED (location_type l)
      {
        return symbol_type (token::TOK_UNSIGNED, std::move (l));
      }
#else
      static
      symbol_type
      make_UNSIGNED (const location_type& l)
      {
        return symbol_type (token::TOK_UNSIGNED, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_HIGHP (location_type l)
      {
        return symbol_type (token::TOK_HIGHP, std::move (l));
      }
#else
      static
      symbol_type
      make_HIGHP (const location_type& l)
      {
        return symbol_type (token::TOK_HIGHP, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_MEDIUMP (location_type l)
      {
        return symbol_type (token::TOK_MEDIUMP, std::move (l));
      }
#else
      static
      symbol_type
      make_MEDIUMP (const location_type& l)
      {
        return symbol_type (token::TOK_MEDIUMP, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_LOWP (location_type l)
      {
        return symbol_type (token::TOK_LOWP, std::move (l));
      }
#else
      static
      symbol_type
      make_LOWP (const location_type& l)
      {
        return symbol_type (token::TOK_LOWP, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_UNIFORM (location_type l)
      {
        return symbol_type (token::TOK_UNIFORM, std::move (l));
      }
#else
      static
      symbol_type
      make_UNIFORM (const location_type& l)
      {
        return symbol_type (token::TOK_UNIFORM, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_CSTBUFFER (location_type l)
      {
        return symbol_type (token::TOK_CSTBUFFER, std::move (l));
      }
#else
      static
      symbol_type
      make_CSTBUFFER (const location_type& l)
      {
        return symbol_type (token::TOK_CSTBUFFER, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_FLOAT_TYPE (location_type l)
      {
        return symbol_type (token::TOK_FLOAT_TYPE, std::move (l));
      }
#else
      static
      symbol_type
      make_FLOAT_TYPE (const location_type& l)
      {
        return symbol_type (token::TOK_FLOAT_TYPE, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_FLOAT2_TYPE (location_type l)
      {
        return symbol_type (token::TOK_FLOAT2_TYPE, std::move (l));
      }
#else
      static
      symbol_type
      make_FLOAT2_TYPE (const location_type& l)
      {
        return symbol_type (token::TOK_FLOAT2_TYPE, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_FLOAT3_TYPE (location_type l)
      {
        return symbol_type (token::TOK_FLOAT3_TYPE, std::move (l));
      }
#else
      static
      symbol_type
      make_FLOAT3_TYPE (const location_type& l)
      {
        return symbol_type (token::TOK_FLOAT3_TYPE, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_FLOAT4_TYPE (location_type l)
      {
        return symbol_type (token::TOK_FLOAT4_TYPE, std::move (l));
      }
#else
      static
      symbol_type
      make_FLOAT4_TYPE (const location_type& l)
      {
        return symbol_type (token::TOK_FLOAT4_TYPE, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_MAT2_TYPE (location_type l)
      {
        return symbol_type (token::TOK_MAT2_TYPE, std::move (l));
      }
#else
      static
      symbol_type
      make_MAT2_TYPE (const location_type& l)
      {
        return symbol_type (token::TOK_MAT2_TYPE, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_MAT3_TYPE (location_type l)
      {
        return symbol_type (token::TOK_MAT3_TYPE, std::move (l));
      }
#else
      static
      symbol_type
      make_MAT3_TYPE (const location_type& l)
      {
        return symbol_type (token::TOK_MAT3_TYPE, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_MAT4_TYPE (location_type l)
      {
        return symbol_type (token::TOK_MAT4_TYPE, std::move (l));
      }
#else
      static
      symbol_type
      make_MAT4_TYPE (const location_type& l)
      {
        return symbol_type (token::TOK_MAT4_TYPE, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_BOOL_TYPE (location_type l)
      {
        return symbol_type (token::TOK_BOOL_TYPE, std::move (l));
      }
#else
      static
      symbol_type
      make_BOOL_TYPE (const location_type& l)
      {
        return symbol_type (token::TOK_BOOL_TYPE, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_BOOL2_TYPE (location_type l)
      {
        return symbol_type (token::TOK_BOOL2_TYPE, std::move (l));
      }
#else
      static
      symbol_type
      make_BOOL2_TYPE (const location_type& l)
      {
        return symbol_type (token::TOK_BOOL2_TYPE, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_BOOL3_TYPE (location_type l)
      {
        return symbol_type (token::TOK_BOOL3_TYPE, std::move (l));
      }
#else
      static
      symbol_type
      make_BOOL3_TYPE (const location_type& l)
      {
        return symbol_type (token::TOK_BOOL3_TYPE, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_BOOL4_TYPE (location_type l)
      {
        return symbol_type (token::TOK_BOOL4_TYPE, std::move (l));
      }
#else
      static
      symbol_type
      make_BOOL4_TYPE (const location_type& l)
      {
        return symbol_type (token::TOK_BOOL4_TYPE, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_INT_TYPE (location_type l)
      {
        return symbol_type (token::TOK_INT_TYPE, std::move (l));
      }
#else
      static
      symbol_type
      make_INT_TYPE (const location_type& l)
      {
        return symbol_type (token::TOK_INT_TYPE, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_INT2_TYPE (location_type l)
      {
        return symbol_type (token::TOK_INT2_TYPE, std::move (l));
      }
#else
      static
      symbol_type
      make_INT2_TYPE (const location_type& l)
      {
        return symbol_type (token::TOK_INT2_TYPE, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_INT3_TYPE (location_type l)
      {
        return symbol_type (token::TOK_INT3_TYPE, std::move (l));
      }
#else
      static
      symbol_type
      make_INT3_TYPE (const location_type& l)
      {
        return symbol_type (token::TOK_INT3_TYPE, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_INT4_TYPE (location_type l)
      {
        return symbol_type (token::TOK_INT4_TYPE, std::move (l));
      }
#else
      static
      symbol_type
      make_INT4_TYPE (const location_type& l)
      {
        return symbol_type (token::TOK_INT4_TYPE, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_TEXTURE1D_TYPE (location_type l)
      {
        return symbol_type (token::TOK_TEXTURE1D_TYPE, std::move (l));
      }
#else
      static
      symbol_type
      make_TEXTURE1D_TYPE (const location_type& l)
      {
        return symbol_type (token::TOK_TEXTURE1D_TYPE, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_TEXTURE2D_TYPE (location_type l)
      {
        return symbol_type (token::TOK_TEXTURE2D_TYPE, std::move (l));
      }
#else
      static
      symbol_type
      make_TEXTURE2D_TYPE (const location_type& l)
      {
        return symbol_type (token::TOK_TEXTURE2D_TYPE, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_TEXTURE2DSHADOW_TYPE (location_type l)
      {
        return symbol_type (token::TOK_TEXTURE2DSHADOW_TYPE, std::move (l));
      }
#else
      static
      symbol_type
      make_TEXTURE2DSHADOW_TYPE (const location_type& l)
      {
        return symbol_type (token::TOK_TEXTURE2DSHADOW_TYPE, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_TEXTURE2DRECT_TYPE (location_type l)
      {
        return symbol_type (token::TOK_TEXTURE2DRECT_TYPE, std::move (l));
      }
#else
      static
      symbol_type
      make_TEXTURE2DRECT_TYPE (const location_type& l)
      {
        return symbol_type (token::TOK_TEXTURE2DRECT_TYPE, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_TEXTURE3D_TYPE (location_type l)
      {
        return symbol_type (token::TOK_TEXTURE3D_TYPE, std::move (l));
      }
#else
      static
      symbol_type
      make_TEXTURE3D_TYPE (const location_type& l)
      {
        return symbol_type (token::TOK_TEXTURE3D_TYPE, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_TEXTURECUBE_TYPE (location_type l)
      {
        return symbol_type (token::TOK_TEXTURECUBE_TYPE, std::move (l));
      }
#else
      static
      symbol_type
      make_TEXTURECUBE_TYPE (const location_type& l)
      {
        return symbol_type (token::TOK_TEXTURECUBE_TYPE, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_SAMPLER1D_TYPE (location_type l)
      {
        return symbol_type (token::TOK_SAMPLER1D_TYPE, std::move (l));
      }
#else
      static
      symbol_type
      make_SAMPLER1D_TYPE (const location_type& l)
      {
        return symbol_type (token::TOK_SAMPLER1D_TYPE, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_SAMPLER2D_TYPE (location_type l)
      {
        return symbol_type (token::TOK_SAMPLER2D_TYPE, std::move (l));
      }
#else
      static
      symbol_type
      make_SAMPLER2D_TYPE (const location_type& l)
      {
        return symbol_type (token::TOK_SAMPLER2D_TYPE, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_SAMPLER2DSHADOW_TYPE (location_type l)
      {
        return symbol_type (token::TOK_SAMPLER2DSHADOW_TYPE, std::move (l));
      }
#else
      static
      symbol_type
      make_SAMPLER2DSHADOW_TYPE (const location_type& l)
      {
        return symbol_type (token::TOK_SAMPLER2DSHADOW_TYPE, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_SAMPLER2DRECT_TYPE (location_type l)
      {
        return symbol_type (token::TOK_SAMPLER2DRECT_TYPE, std::move (l));
      }
#else
      static
      symbol_type
      make_SAMPLER2DRECT_TYPE (const location_type& l)
      {
        return symbol_type (token::TOK_SAMPLER2DRECT_TYPE, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_SAMPLER3D_TYPE (location_type l)
      {
        return symbol_type (token::TOK_SAMPLER3D_TYPE, std::move (l));
      }
#else
      static
      symbol_type
      make_SAMPLER3D_TYPE (const location_type& l)
      {
        return symbol_type (token::TOK_SAMPLER3D_TYPE, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_SAMPLERCUBE_TYPE (location_type l)
      {
        return symbol_type (token::TOK_SAMPLERCUBE_TYPE, std::move (l));
      }
#else
      static
      symbol_type
      make_SAMPLERCUBE_TYPE (const location_type& l)
      {
        return symbol_type (token::TOK_SAMPLERCUBE_TYPE, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_EXTENSION (location_type l)
      {
        return symbol_type (token::TOK_EXTENSION, std::move (l));
      }
#else
      static
      symbol_type
      make_EXTENSION (const location_type& l)
      {
        return symbol_type (token::TOK_EXTENSION, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_SEPARATE_SHADER (location_type l)
      {
        return symbol_type (token::TOK_SEPARATE_SHADER, std::move (l));
      }
#else
      static
      symbol_type
      make_SEPARATE_SHADER (const location_type& l)
      {
        return symbol_type (token::TOK_SEPARATE_SHADER, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_VERTEXPROGRAM (location_type l)
      {
        return symbol_type (token::TOK_VERTEXPROGRAM, std::move (l));
      }
#else
      static
      symbol_type
      make_VERTEXPROGRAM (const location_type& l)
      {
        return symbol_type (token::TOK_VERTEXPROGRAM, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_FRAGMENTPROGRAM (location_type l)
      {
        return symbol_type (token::TOK_FRAGMENTPROGRAM, std::move (l));
      }
#else
      static
      symbol_type
      make_FRAGMENTPROGRAM (const location_type& l)
      {
        return symbol_type (token::TOK_FRAGMENTPROGRAM, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_GEOMETRYPROGRAM (location_type l)
      {
        return symbol_type (token::TOK_GEOMETRYPROGRAM, std::move (l));
      }
#else
      static
      symbol_type
      make_GEOMETRYPROGRAM (const location_type& l)
      {
        return symbol_type (token::TOK_GEOMETRYPROGRAM, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_HULLPROGRAM (location_type l)
      {
        return symbol_type (token::TOK_HULLPROGRAM, std::move (l));
      }
#else
      static
      symbol_type
      make_HULLPROGRAM (const location_type& l)
      {
        return symbol_type (token::TOK_HULLPROGRAM, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_EVALPROGRAM (location_type l)
      {
        return symbol_type (token::TOK_EVALPROGRAM, std::move (l));
      }
#else
      static
      symbol_type
      make_EVALPROGRAM (const location_type& l)
      {
        return symbol_type (token::TOK_EVALPROGRAM, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_SHDPROFILE (location_type l)
      {
        return symbol_type (token::TOK_SHDPROFILE, std::move (l));
      }
#else
      static
      symbol_type
      make_SHDPROFILE (const location_type& l)
      {
        return symbol_type (token::TOK_SHDPROFILE, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_SAMPLERRESOURCE (location_type l)
      {
        return symbol_type (token::TOK_SAMPLERRESOURCE, std::move (l));
      }
#else
      static
      symbol_type
      make_SAMPLERRESOURCE (const location_type& l)
      {
        return symbol_type (token::TOK_SAMPLERRESOURCE, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_SAMPLERTEXUNIT (location_type l)
      {
        return symbol_type (token::TOK_SAMPLERTEXUNIT, std::move (l));
      }
#else
      static
      symbol_type
      make_SAMPLERTEXUNIT (const location_type& l)
      {
        return symbol_type (token::TOK_SAMPLERTEXUNIT, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_SETSAMPLERSTATE (location_type l)
      {
        return symbol_type (token::TOK_SETSAMPLERSTATE, std::move (l));
      }
#else
      static
      symbol_type
      make_SETSAMPLERSTATE (const location_type& l)
      {
        return symbol_type (token::TOK_SETSAMPLERSTATE, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_SETDSTSTATE (location_type l)
      {
        return symbol_type (token::TOK_SETDSTSTATE, std::move (l));
      }
#else
      static
      symbol_type
      make_SETDSTSTATE (const location_type& l)
      {
        return symbol_type (token::TOK_SETDSTSTATE, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_SETRASTERIZATIONSTATE (location_type l)
      {
        return symbol_type (token::TOK_SETRASTERIZATIONSTATE, std::move (l));
      }
#else
      static
      symbol_type
      make_SETRASTERIZATIONSTATE (const location_type& l)
      {
        return symbol_type (token::TOK_SETRASTERIZATIONSTATE, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_SETCOLORSAMPLESTATE (location_type l)
      {
        return symbol_type (token::TOK_SETCOLORSAMPLESTATE, std::move (l));
      }
#else
      static
      symbol_type
      make_SETCOLORSAMPLESTATE (const location_type& l)
      {
        return symbol_type (token::TOK_SETCOLORSAMPLESTATE, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_IMAGERESOURCE (location_type l)
      {
        return symbol_type (token::TOK_IMAGERESOURCE, std::move (l));
      }
#else
      static
      symbol_type
      make_IMAGERESOURCE (const location_type& l)
      {
        return symbol_type (token::TOK_IMAGERESOURCE, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_IMAGEUNIT (location_type l)
      {
        return symbol_type (token::TOK_IMAGEUNIT, std::move (l));
      }
#else
      static
      symbol_type
      make_IMAGEUNIT (const location_type& l)
      {
        return symbol_type (token::TOK_IMAGEUNIT, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_IMAGEACCESS (location_type l)
      {
        return symbol_type (token::TOK_IMAGEACCESS, std::move (l));
      }
#else
      static
      symbol_type
      make_IMAGEACCESS (const location_type& l)
      {
        return symbol_type (token::TOK_IMAGEACCESS, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_IMAGELAYER (location_type l)
      {
        return symbol_type (token::TOK_IMAGELAYER, std::move (l));
      }
#else
      static
      symbol_type
      make_IMAGELAYER (const location_type& l)
      {
        return symbol_type (token::TOK_IMAGELAYER, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_IMAGELAYERED (location_type l)
      {
        return symbol_type (token::TOK_IMAGELAYERED, std::move (l));
      }
#else
      static
      symbol_type
      make_IMAGELAYERED (const location_type& l)
      {
        return symbol_type (token::TOK_IMAGELAYERED, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_WRITE_ONLY (location_type l)
      {
        return symbol_type (token::TOK_WRITE_ONLY, std::move (l));
      }
#else
      static
      symbol_type
      make_WRITE_ONLY (const location_type& l)
      {
        return symbol_type (token::TOK_WRITE_ONLY, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_READ_ONLY (location_type l)
      {
        return symbol_type (token::TOK_READ_ONLY, std::move (l));
      }
#else
      static
      symbol_type
      make_READ_ONLY (const location_type& l)
      {
        return symbol_type (token::TOK_READ_ONLY, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_READ_WRITE (location_type l)
      {
        return symbol_type (token::TOK_READ_WRITE, std::move (l));
      }
#else
      static
      symbol_type
      make_READ_WRITE (const location_type& l)
      {
        return symbol_type (token::TOK_READ_WRITE, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_FORMAT_UNDEFINED (VkFormat v, location_type l)
      {
        return symbol_type (token::TOK_FORMAT_UNDEFINED, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_FORMAT_UNDEFINED (const VkFormat& v, const location_type& l)
      {
        return symbol_type (token::TOK_FORMAT_UNDEFINED, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_FORMAT_R4G4_UNORM_PACK8 (VkFormat v, location_type l)
      {
        return symbol_type (token::TOK_FORMAT_R4G4_UNORM_PACK8, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_FORMAT_R4G4_UNORM_PACK8 (const VkFormat& v, const location_type& l)
      {
        return symbol_type (token::TOK_FORMAT_R4G4_UNORM_PACK8, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_FORMAT_R4G4B4A4_UNORM_PACK16 (VkFormat v, location_type l)
      {
        return symbol_type (token::TOK_FORMAT_R4G4B4A4_UNORM_PACK16, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_FORMAT_R4G4B4A4_UNORM_PACK16 (const VkFormat& v, const location_type& l)
      {
        return symbol_type (token::TOK_FORMAT_R4G4B4A4_UNORM_PACK16, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_FORMAT_B4G4R4A4_UNORM_PACK16 (VkFormat v, location_type l)
      {
        return symbol_type (token::TOK_FORMAT_B4G4R4A4_UNORM_PACK16, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_FORMAT_B4G4R4A4_UNORM_PACK16 (const VkFormat& v, const location_type& l)
      {
        return symbol_type (token::TOK_FORMAT_B4G4R4A4_UNORM_PACK16, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_FORMAT_R5G6B5_UNORM_PACK16 (VkFormat v, location_type l)
      {
        return symbol_type (token::TOK_FORMAT_R5G6B5_UNORM_PACK16, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_FORMAT_R5G6B5_UNORM_PACK16 (const VkFormat& v, const location_type& l)
      {
        return symbol_type (token::TOK_FORMAT_R5G6B5_UNORM_PACK16, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_FORMAT_B5G6R5_UNORM_PACK16 (VkFormat v, location_type l)
      {
        return symbol_type (token::TOK_FORMAT_B5G6R5_UNORM_PACK16, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_FORMAT_B5G6R5_UNORM_PACK16 (const VkFormat& v, const location_type& l)
      {
        return symbol_type (token::TOK_FORMAT_B5G6R5_UNORM_PACK16, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_FORMAT_R5G5B5A1_UNORM_PACK16 (VkFormat v, location_type l)
      {
        return symbol_type (token::TOK_FORMAT_R5G5B5A1_UNORM_PACK16, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_FORMAT_R5G5B5A1_UNORM_PACK16 (const VkFormat& v, const location_type& l)
      {
        return symbol_type (token::TOK_FORMAT_R5G5B5A1_UNORM_PACK16, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_FORMAT_B5G5R5A1_UNORM_PACK16 (VkFormat v, location_type l)
      {
        return symbol_type (token::TOK_FORMAT_B5G5R5A1_UNORM_PACK16, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_FORMAT_B5G5R5A1_UNORM_PACK16 (const VkFormat& v, const location_type& l)
      {
        return symbol_type (token::TOK_FORMAT_B5G5R5A1_UNORM_PACK16, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_FORMAT_A1R5G5B5_UNORM_PACK16 (VkFormat v, location_type l)
      {
        return symbol_type (token::TOK_FORMAT_A1R5G5B5_UNORM_PACK16, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_FORMAT_A1R5G5B5_UNORM_PACK16 (const VkFormat& v, const location_type& l)
      {
        return symbol_type (token::TOK_FORMAT_A1R5G5B5_UNORM_PACK16, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_FORMAT_R8_UNORM (VkFormat v, location_type l)
      {
        return symbol_type (token::TOK_FORMAT_R8_UNORM, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_FORMAT_R8_UNORM (const VkFormat& v, const location_type& l)
      {
        return symbol_type (token::TOK_FORMAT_R8_UNORM, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_FORMAT_R8_SNORM (VkFormat v, location_type l)
      {
        return symbol_type (token::TOK_FORMAT_R8_SNORM, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_FORMAT_R8_SNORM (const VkFormat& v, const location_type& l)
      {
        return symbol_type (token::TOK_FORMAT_R8_SNORM, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_FORMAT_R8_USCALED (VkFormat v, location_type l)
      {
        return symbol_type (token::TOK_FORMAT_R8_USCALED, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_FORMAT_R8_USCALED (const VkFormat& v, const location_type& l)
      {
        return symbol_type (token::TOK_FORMAT_R8_USCALED, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_FORMAT_R8_SSCALED (VkFormat v, location_type l)
      {
        return symbol_type (token::TOK_FORMAT_R8_SSCALED, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_FORMAT_R8_SSCALED (const VkFormat& v, const location_type& l)
      {
        return symbol_type (token::TOK_FORMAT_R8_SSCALED, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_FORMAT_R8_UINT (VkFormat v, location_type l)
      {
        return symbol_type (token::TOK_FORMAT_R8_UINT, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_FORMAT_R8_UINT (const VkFormat& v, const location_type& l)
      {
        return symbol_type (token::TOK_FORMAT_R8_UINT, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_FORMAT_R8_SINT (VkFormat v, location_type l)
      {
        return symbol_type (token::TOK_FORMAT_R8_SINT, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_FORMAT_R8_SINT (const VkFormat& v, const location_type& l)
      {
        return symbol_type (token::TOK_FORMAT_R8_SINT, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_FORMAT_R8_SRGB (VkFormat v, location_type l)
      {
        return symbol_type (token::TOK_FORMAT_R8_SRGB, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_FORMAT_R8_SRGB (const VkFormat& v, const location_type& l)
      {
        return symbol_type (token::TOK_FORMAT_R8_SRGB, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_FORMAT_R8G8_UNORM (VkFormat v, location_type l)
      {
        return symbol_type (token::TOK_FORMAT_R8G8_UNORM, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_FORMAT_R8G8_UNORM (const VkFormat& v, const location_type& l)
      {
        return symbol_type (token::TOK_FORMAT_R8G8_UNORM, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_FORMAT_R8G8_SNORM (VkFormat v, location_type l)
      {
        return symbol_type (token::TOK_FORMAT_R8G8_SNORM, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_FORMAT_R8G8_SNORM (const VkFormat& v, const location_type& l)
      {
        return symbol_type (token::TOK_FORMAT_R8G8_SNORM, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_FORMAT_R8G8_USCALED (VkFormat v, location_type l)
      {
        return symbol_type (token::TOK_FORMAT_R8G8_USCALED, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_FORMAT_R8G8_USCALED (const VkFormat& v, const location_type& l)
      {
        return symbol_type (token::TOK_FORMAT_R8G8_USCALED, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_FORMAT_R8G8_SSCALED (VkFormat v, location_type l)
      {
        return symbol_type (token::TOK_FORMAT_R8G8_SSCALED, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_FORMAT_R8G8_SSCALED (const VkFormat& v, const location_type& l)
      {
        return symbol_type (token::TOK_FORMAT_R8G8_SSCALED, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_FORMAT_R8G8_UINT (VkFormat v, location_type l)
      {
        return symbol_type (token::TOK_FORMAT_R8G8_UINT, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_FORMAT_R8G8_UINT (const VkFormat& v, const location_type& l)
      {
        return symbol_type (token::TOK_FORMAT_R8G8_UINT, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_FORMAT_R8G8_SINT (VkFormat v, location_type l)
      {
        return symbol_type (token::TOK_FORMAT_R8G8_SINT, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_FORMAT_R8G8_SINT (const VkFormat& v, const location_type& l)
      {
        return symbol_type (token::TOK_FORMAT_R8G8_SINT, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_FORMAT_R8G8_SRGB (VkFormat v, location_type l)
      {
        return symbol_type (token::TOK_FORMAT_R8G8_SRGB, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_FORMAT_R8G8_SRGB (const VkFormat& v, const location_type& l)
      {
        return symbol_type (token::TOK_FORMAT_R8G8_SRGB, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_FORMAT_R8G8B8_UNORM (VkFormat v, location_type l)
      {
        return symbol_type (token::TOK_FORMAT_R8G8B8_UNORM, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_FORMAT_R8G8B8_UNORM (const VkFormat& v, const location_type& l)
      {
        return symbol_type (token::TOK_FORMAT_R8G8B8_UNORM, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_FORMAT_R8G8B8_SNORM (VkFormat v, location_type l)
      {
        return symbol_type (token::TOK_FORMAT_R8G8B8_SNORM, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_FORMAT_R8G8B8_SNORM (const VkFormat& v, const location_type& l)
      {
        return symbol_type (token::TOK_FORMAT_R8G8B8_SNORM, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_FORMAT_R8G8B8_USCALED (VkFormat v, location_type l)
      {
        return symbol_type (token::TOK_FORMAT_R8G8B8_USCALED, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_FORMAT_R8G8B8_USCALED (const VkFormat& v, const location_type& l)
      {
        return symbol_type (token::TOK_FORMAT_R8G8B8_USCALED, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_FORMAT_R8G8B8_SSCALED (VkFormat v, location_type l)
      {
        return symbol_type (token::TOK_FORMAT_R8G8B8_SSCALED, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_FORMAT_R8G8B8_SSCALED (const VkFormat& v, const location_type& l)
      {
        return symbol_type (token::TOK_FORMAT_R8G8B8_SSCALED, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_FORMAT_R8G8B8_UINT (VkFormat v, location_type l)
      {
        return symbol_type (token::TOK_FORMAT_R8G8B8_UINT, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_FORMAT_R8G8B8_UINT (const VkFormat& v, const location_type& l)
      {
        return symbol_type (token::TOK_FORMAT_R8G8B8_UINT, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_FORMAT_R8G8B8_SINT (VkFormat v, location_type l)
      {
        return symbol_type (token::TOK_FORMAT_R8G8B8_SINT, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_FORMAT_R8G8B8_SINT (const VkFormat& v, const location_type& l)
      {
        return symbol_type (token::TOK_FORMAT_R8G8B8_SINT, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_FORMAT_R8G8B8_SRGB (VkFormat v, location_type l)
      {
        return symbol_type (token::TOK_FORMAT_R8G8B8_SRGB, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_FORMAT_R8G8B8_SRGB (const VkFormat& v, const location_type& l)
      {
        return symbol_type (token::TOK_FORMAT_R8G8B8_SRGB, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_FORMAT_B8G8R8_UNORM (VkFormat v, location_type l)
      {
        return symbol_type (token::TOK_FORMAT_B8G8R8_UNORM, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_FORMAT_B8G8R8_UNORM (const VkFormat& v, const location_type& l)
      {
        return symbol_type (token::TOK_FORMAT_B8G8R8_UNORM, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_FORMAT_B8G8R8_SNORM (VkFormat v, location_type l)
      {
        return symbol_type (token::TOK_FORMAT_B8G8R8_SNORM, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_FORMAT_B8G8R8_SNORM (const VkFormat& v, const location_type& l)
      {
        return symbol_type (token::TOK_FORMAT_B8G8R8_SNORM, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_FORMAT_B8G8R8_USCALED (VkFormat v, location_type l)
      {
        return symbol_type (token::TOK_FORMAT_B8G8R8_USCALED, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_FORMAT_B8G8R8_USCALED (const VkFormat& v, const location_type& l)
      {
        return symbol_type (token::TOK_FORMAT_B8G8R8_USCALED, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_FORMAT_B8G8R8_SSCALED (VkFormat v, location_type l)
      {
        return symbol_type (token::TOK_FORMAT_B8G8R8_SSCALED, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_FORMAT_B8G8R8_SSCALED (const VkFormat& v, const location_type& l)
      {
        return symbol_type (token::TOK_FORMAT_B8G8R8_SSCALED, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_FORMAT_B8G8R8_UINT (VkFormat v, location_type l)
      {
        return symbol_type (token::TOK_FORMAT_B8G8R8_UINT, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_FORMAT_B8G8R8_UINT (const VkFormat& v, const location_type& l)
      {
        return symbol_type (token::TOK_FORMAT_B8G8R8_UINT, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_FORMAT_B8G8R8_SINT (VkFormat v, location_type l)
      {
        return symbol_type (token::TOK_FORMAT_B8G8R8_SINT, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_FORMAT_B8G8R8_SINT (const VkFormat& v, const location_type& l)
      {
        return symbol_type (token::TOK_FORMAT_B8G8R8_SINT, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_FORMAT_B8G8R8_SRGB (VkFormat v, location_type l)
      {
        return symbol_type (token::TOK_FORMAT_B8G8R8_SRGB, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_FORMAT_B8G8R8_SRGB (const VkFormat& v, const location_type& l)
      {
        return symbol_type (token::TOK_FORMAT_B8G8R8_SRGB, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_FORMAT_R8G8B8A8_UNORM (VkFormat v, location_type l)
      {
        return symbol_type (token::TOK_FORMAT_R8G8B8A8_UNORM, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_FORMAT_R8G8B8A8_UNORM (const VkFormat& v, const location_type& l)
      {
        return symbol_type (token::TOK_FORMAT_R8G8B8A8_UNORM, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_FORMAT_R8G8B8A8_SNORM (VkFormat v, location_type l)
      {
        return symbol_type (token::TOK_FORMAT_R8G8B8A8_SNORM, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_FORMAT_R8G8B8A8_SNORM (const VkFormat& v, const location_type& l)
      {
        return symbol_type (token::TOK_FORMAT_R8G8B8A8_SNORM, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_FORMAT_R8G8B8A8_USCALED (VkFormat v, location_type l)
      {
        return symbol_type (token::TOK_FORMAT_R8G8B8A8_USCALED, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_FORMAT_R8G8B8A8_USCALED (const VkFormat& v, const location_type& l)
      {
        return symbol_type (token::TOK_FORMAT_R8G8B8A8_USCALED, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_FORMAT_R8G8B8A8_SSCALED (VkFormat v, location_type l)
      {
        return symbol_type (token::TOK_FORMAT_R8G8B8A8_SSCALED, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_FORMAT_R8G8B8A8_SSCALED (const VkFormat& v, const location_type& l)
      {
        return symbol_type (token::TOK_FORMAT_R8G8B8A8_SSCALED, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_FORMAT_R8G8B8A8_UINT (VkFormat v, location_type l)
      {
        return symbol_type (token::TOK_FORMAT_R8G8B8A8_UINT, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_FORMAT_R8G8B8A8_UINT (const VkFormat& v, const location_type& l)
      {
        return symbol_type (token::TOK_FORMAT_R8G8B8A8_UINT, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_FORMAT_R8G8B8A8_SINT (VkFormat v, location_type l)
      {
        return symbol_type (token::TOK_FORMAT_R8G8B8A8_SINT, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_FORMAT_R8G8B8A8_SINT (const VkFormat& v, const location_type& l)
      {
        return symbol_type (token::TOK_FORMAT_R8G8B8A8_SINT, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_FORMAT_R8G8B8A8_SRGB (VkFormat v, location_type l)
      {
        return symbol_type (token::TOK_FORMAT_R8G8B8A8_SRGB, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_FORMAT_R8G8B8A8_SRGB (const VkFormat& v, const location_type& l)
      {
        return symbol_type (token::TOK_FORMAT_R8G8B8A8_SRGB, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_FORMAT_B8G8R8A8_UNORM (VkFormat v, location_type l)
      {
        return symbol_type (token::TOK_FORMAT_B8G8R8A8_UNORM, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_FORMAT_B8G8R8A8_UNORM (const VkFormat& v, const location_type& l)
      {
        return symbol_type (token::TOK_FORMAT_B8G8R8A8_UNORM, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_FORMAT_B8G8R8A8_SNORM (VkFormat v, location_type l)
      {
        return symbol_type (token::TOK_FORMAT_B8G8R8A8_SNORM, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_FORMAT_B8G8R8A8_SNORM (const VkFormat& v, const location_type& l)
      {
        return symbol_type (token::TOK_FORMAT_B8G8R8A8_SNORM, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_FORMAT_B8G8R8A8_USCALED (VkFormat v, location_type l)
      {
        return symbol_type (token::TOK_FORMAT_B8G8R8A8_USCALED, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_FORMAT_B8G8R8A8_USCALED (const VkFormat& v, const location_type& l)
      {
        return symbol_type (token::TOK_FORMAT_B8G8R8A8_USCALED, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_FORMAT_B8G8R8A8_SSCALED (VkFormat v, location_type l)
      {
        return symbol_type (token::TOK_FORMAT_B8G8R8A8_SSCALED, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_FORMAT_B8G8R8A8_SSCALED (const VkFormat& v, const location_type& l)
      {
        return symbol_type (token::TOK_FORMAT_B8G8R8A8_SSCALED, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_FORMAT_B8G8R8A8_UINT (VkFormat v, location_type l)
      {
        return symbol_type (token::TOK_FORMAT_B8G8R8A8_UINT, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_FORMAT_B8G8R8A8_UINT (const VkFormat& v, const location_type& l)
      {
        return symbol_type (token::TOK_FORMAT_B8G8R8A8_UINT, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_FORMAT_B8G8R8A8_SINT (VkFormat v, location_type l)
      {
        return symbol_type (token::TOK_FORMAT_B8G8R8A8_SINT, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_FORMAT_B8G8R8A8_SINT (const VkFormat& v, const location_type& l)
      {
        return symbol_type (token::TOK_FORMAT_B8G8R8A8_SINT, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_FORMAT_B8G8R8A8_SRGB (VkFormat v, location_type l)
      {
        return symbol_type (token::TOK_FORMAT_B8G8R8A8_SRGB, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_FORMAT_B8G8R8A8_SRGB (const VkFormat& v, const location_type& l)
      {
        return symbol_type (token::TOK_FORMAT_B8G8R8A8_SRGB, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_FORMAT_A8B8G8R8_UNORM_PACK32 (VkFormat v, location_type l)
      {
        return symbol_type (token::TOK_FORMAT_A8B8G8R8_UNORM_PACK32, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_FORMAT_A8B8G8R8_UNORM_PACK32 (const VkFormat& v, const location_type& l)
      {
        return symbol_type (token::TOK_FORMAT_A8B8G8R8_UNORM_PACK32, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_FORMAT_A8B8G8R8_SNORM_PACK32 (VkFormat v, location_type l)
      {
        return symbol_type (token::TOK_FORMAT_A8B8G8R8_SNORM_PACK32, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_FORMAT_A8B8G8R8_SNORM_PACK32 (const VkFormat& v, const location_type& l)
      {
        return symbol_type (token::TOK_FORMAT_A8B8G8R8_SNORM_PACK32, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_FORMAT_A8B8G8R8_USCALED_PACK32 (VkFormat v, location_type l)
      {
        return symbol_type (token::TOK_FORMAT_A8B8G8R8_USCALED_PACK32, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_FORMAT_A8B8G8R8_USCALED_PACK32 (const VkFormat& v, const location_type& l)
      {
        return symbol_type (token::TOK_FORMAT_A8B8G8R8_USCALED_PACK32, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_FORMAT_A8B8G8R8_SSCALED_PACK32 (VkFormat v, location_type l)
      {
        return symbol_type (token::TOK_FORMAT_A8B8G8R8_SSCALED_PACK32, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_FORMAT_A8B8G8R8_SSCALED_PACK32 (const VkFormat& v, const location_type& l)
      {
        return symbol_type (token::TOK_FORMAT_A8B8G8R8_SSCALED_PACK32, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_FORMAT_A8B8G8R8_UINT_PACK32 (VkFormat v, location_type l)
      {
        return symbol_type (token::TOK_FORMAT_A8B8G8R8_UINT_PACK32, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_FORMAT_A8B8G8R8_UINT_PACK32 (const VkFormat& v, const location_type& l)
      {
        return symbol_type (token::TOK_FORMAT_A8B8G8R8_UINT_PACK32, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_FORMAT_A8B8G8R8_SINT_PACK32 (VkFormat v, location_type l)
      {
        return symbol_type (token::TOK_FORMAT_A8B8G8R8_SINT_PACK32, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_FORMAT_A8B8G8R8_SINT_PACK32 (const VkFormat& v, const location_type& l)
      {
        return symbol_type (token::TOK_FORMAT_A8B8G8R8_SINT_PACK32, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_FORMAT_A8B8G8R8_SRGB_PACK32 (VkFormat v, location_type l)
      {
        return symbol_type (token::TOK_FORMAT_A8B8G8R8_SRGB_PACK32, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_FORMAT_A8B8G8R8_SRGB_PACK32 (const VkFormat& v, const location_type& l)
      {
        return symbol_type (token::TOK_FORMAT_A8B8G8R8_SRGB_PACK32, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_FORMAT_A2R10G10B10_UNORM_PACK32 (VkFormat v, location_type l)
      {
        return symbol_type (token::TOK_FORMAT_A2R10G10B10_UNORM_PACK32, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_FORMAT_A2R10G10B10_UNORM_PACK32 (const VkFormat& v, const location_type& l)
      {
        return symbol_type (token::TOK_FORMAT_A2R10G10B10_UNORM_PACK32, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_FORMAT_A2R10G10B10_SNORM_PACK32 (VkFormat v, location_type l)
      {
        return symbol_type (token::TOK_FORMAT_A2R10G10B10_SNORM_PACK32, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_FORMAT_A2R10G10B10_SNORM_PACK32 (const VkFormat& v, const location_type& l)
      {
        return symbol_type (token::TOK_FORMAT_A2R10G10B10_SNORM_PACK32, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_FORMAT_A2R10G10B10_USCALED_PACK32 (VkFormat v, location_type l)
      {
        return symbol_type (token::TOK_FORMAT_A2R10G10B10_USCALED_PACK32, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_FORMAT_A2R10G10B10_USCALED_PACK32 (const VkFormat& v, const location_type& l)
      {
        return symbol_type (token::TOK_FORMAT_A2R10G10B10_USCALED_PACK32, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_FORMAT_A2R10G10B10_SSCALED_PACK32 (VkFormat v, location_type l)
      {
        return symbol_type (token::TOK_FORMAT_A2R10G10B10_SSCALED_PACK32, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_FORMAT_A2R10G10B10_SSCALED_PACK32 (const VkFormat& v, const location_type& l)
      {
        return symbol_type (token::TOK_FORMAT_A2R10G10B10_SSCALED_PACK32, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_FORMAT_A2R10G10B10_UINT_PACK32 (VkFormat v, location_type l)
      {
        return symbol_type (token::TOK_FORMAT_A2R10G10B10_UINT_PACK32, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_FORMAT_A2R10G10B10_UINT_PACK32 (const VkFormat& v, const location_type& l)
      {
        return symbol_type (token::TOK_FORMAT_A2R10G10B10_UINT_PACK32, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_FORMAT_A2R10G10B10_SINT_PACK32 (VkFormat v, location_type l)
      {
        return symbol_type (token::TOK_FORMAT_A2R10G10B10_SINT_PACK32, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_FORMAT_A2R10G10B10_SINT_PACK32 (const VkFormat& v, const location_type& l)
      {
        return symbol_type (token::TOK_FORMAT_A2R10G10B10_SINT_PACK32, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_FORMAT_A2B10G10R10_UNORM_PACK32 (VkFormat v, location_type l)
      {
        return symbol_type (token::TOK_FORMAT_A2B10G10R10_UNORM_PACK32, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_FORMAT_A2B10G10R10_UNORM_PACK32 (const VkFormat& v, const location_type& l)
      {
        return symbol_type (token::TOK_FORMAT_A2B10G10R10_UNORM_PACK32, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_FORMAT_A2B10G10R10_SNORM_PACK32 (VkFormat v, location_type l)
      {
        return symbol_type (token::TOK_FORMAT_A2B10G10R10_SNORM_PACK32, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_FORMAT_A2B10G10R10_SNORM_PACK32 (const VkFormat& v, const location_type& l)
      {
        return symbol_type (token::TOK_FORMAT_A2B10G10R10_SNORM_PACK32, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_FORMAT_A2B10G10R10_USCALED_PACK32 (VkFormat v, location_type l)
      {
        return symbol_type (token::TOK_FORMAT_A2B10G10R10_USCALED_PACK32, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_FORMAT_A2B10G10R10_USCALED_PACK32 (const VkFormat& v, const location_type& l)
      {
        return symbol_type (token::TOK_FORMAT_A2B10G10R10_USCALED_PACK32, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_FORMAT_A2B10G10R10_SSCALED_PACK32 (VkFormat v, location_type l)
      {
        return symbol_type (token::TOK_FORMAT_A2B10G10R10_SSCALED_PACK32, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_FORMAT_A2B10G10R10_SSCALED_PACK32 (const VkFormat& v, const location_type& l)
      {
        return symbol_type (token::TOK_FORMAT_A2B10G10R10_SSCALED_PACK32, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_FORMAT_A2B10G10R10_UINT_PACK32 (VkFormat v, location_type l)
      {
        return symbol_type (token::TOK_FORMAT_A2B10G10R10_UINT_PACK32, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_FORMAT_A2B10G10R10_UINT_PACK32 (const VkFormat& v, const location_type& l)
      {
        return symbol_type (token::TOK_FORMAT_A2B10G10R10_UINT_PACK32, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_FORMAT_A2B10G10R10_SINT_PACK32 (VkFormat v, location_type l)
      {
        return symbol_type (token::TOK_FORMAT_A2B10G10R10_SINT_PACK32, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_FORMAT_A2B10G10R10_SINT_PACK32 (const VkFormat& v, const location_type& l)
      {
        return symbol_type (token::TOK_FORMAT_A2B10G10R10_SINT_PACK32, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_FORMAT_R16_UNORM (VkFormat v, location_type l)
      {
        return symbol_type (token::TOK_FORMAT_R16_UNORM, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_FORMAT_R16_UNORM (const VkFormat& v, const location_type& l)
      {
        return symbol_type (token::TOK_FORMAT_R16_UNORM, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_FORMAT_R16_SNORM (VkFormat v, location_type l)
      {
        return symbol_type (token::TOK_FORMAT_R16_SNORM, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_FORMAT_R16_SNORM (const VkFormat& v, const location_type& l)
      {
        return symbol_type (token::TOK_FORMAT_R16_SNORM, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_FORMAT_R16_USCALED (VkFormat v, location_type l)
      {
        return symbol_type (token::TOK_FORMAT_R16_USCALED, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_FORMAT_R16_USCALED (const VkFormat& v, const location_type& l)
      {
        return symbol_type (token::TOK_FORMAT_R16_USCALED, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_FORMAT_R16_SSCALED (VkFormat v, location_type l)
      {
        return symbol_type (token::TOK_FORMAT_R16_SSCALED, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_FORMAT_R16_SSCALED (const VkFormat& v, const location_type& l)
      {
        return symbol_type (token::TOK_FORMAT_R16_SSCALED, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_FORMAT_R16_UINT (VkFormat v, location_type l)
      {
        return symbol_type (token::TOK_FORMAT_R16_UINT, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_FORMAT_R16_UINT (const VkFormat& v, const location_type& l)
      {
        return symbol_type (token::TOK_FORMAT_R16_UINT, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_FORMAT_R16_SINT (VkFormat v, location_type l)
      {
        return symbol_type (token::TOK_FORMAT_R16_SINT, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_FORMAT_R16_SINT (const VkFormat& v, const location_type& l)
      {
        return symbol_type (token::TOK_FORMAT_R16_SINT, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_FORMAT_R16_SFLOAT (VkFormat v, location_type l)
      {
        return symbol_type (token::TOK_FORMAT_R16_SFLOAT, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_FORMAT_R16_SFLOAT (const VkFormat& v, const location_type& l)
      {
        return symbol_type (token::TOK_FORMAT_R16_SFLOAT, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_FORMAT_R16G16_UNORM (VkFormat v, location_type l)
      {
        return symbol_type (token::TOK_FORMAT_R16G16_UNORM, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_FORMAT_R16G16_UNORM (const VkFormat& v, const location_type& l)
      {
        return symbol_type (token::TOK_FORMAT_R16G16_UNORM, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_FORMAT_R16G16_SNORM (VkFormat v, location_type l)
      {
        return symbol_type (token::TOK_FORMAT_R16G16_SNORM, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_FORMAT_R16G16_SNORM (const VkFormat& v, const location_type& l)
      {
        return symbol_type (token::TOK_FORMAT_R16G16_SNORM, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_FORMAT_R16G16_USCALED (VkFormat v, location_type l)
      {
        return symbol_type (token::TOK_FORMAT_R16G16_USCALED, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_FORMAT_R16G16_USCALED (const VkFormat& v, const location_type& l)
      {
        return symbol_type (token::TOK_FORMAT_R16G16_USCALED, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_FORMAT_R16G16_SSCALED (VkFormat v, location_type l)
      {
        return symbol_type (token::TOK_FORMAT_R16G16_SSCALED, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_FORMAT_R16G16_SSCALED (const VkFormat& v, const location_type& l)
      {
        return symbol_type (token::TOK_FORMAT_R16G16_SSCALED, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_FORMAT_R16G16_UINT (VkFormat v, location_type l)
      {
        return symbol_type (token::TOK_FORMAT_R16G16_UINT, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_FORMAT_R16G16_UINT (const VkFormat& v, const location_type& l)
      {
        return symbol_type (token::TOK_FORMAT_R16G16_UINT, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_FORMAT_R16G16_SINT (VkFormat v, location_type l)
      {
        return symbol_type (token::TOK_FORMAT_R16G16_SINT, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_FORMAT_R16G16_SINT (const VkFormat& v, const location_type& l)
      {
        return symbol_type (token::TOK_FORMAT_R16G16_SINT, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_FORMAT_R16G16_SFLOAT (VkFormat v, location_type l)
      {
        return symbol_type (token::TOK_FORMAT_R16G16_SFLOAT, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_FORMAT_R16G16_SFLOAT (const VkFormat& v, const location_type& l)
      {
        return symbol_type (token::TOK_FORMAT_R16G16_SFLOAT, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_FORMAT_R16G16B16_UNORM (VkFormat v, location_type l)
      {
        return symbol_type (token::TOK_FORMAT_R16G16B16_UNORM, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_FORMAT_R16G16B16_UNORM (const VkFormat& v, const location_type& l)
      {
        return symbol_type (token::TOK_FORMAT_R16G16B16_UNORM, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_FORMAT_R16G16B16_SNORM (VkFormat v, location_type l)
      {
        return symbol_type (token::TOK_FORMAT_R16G16B16_SNORM, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_FORMAT_R16G16B16_SNORM (const VkFormat& v, const location_type& l)
      {
        return symbol_type (token::TOK_FORMAT_R16G16B16_SNORM, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_FORMAT_R16G16B16_USCALED (VkFormat v, location_type l)
      {
        return symbol_type (token::TOK_FORMAT_R16G16B16_USCALED, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_FORMAT_R16G16B16_USCALED (const VkFormat& v, const location_type& l)
      {
        return symbol_type (token::TOK_FORMAT_R16G16B16_USCALED, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_FORMAT_R16G16B16_SSCALED (VkFormat v, location_type l)
      {
        return symbol_type (token::TOK_FORMAT_R16G16B16_SSCALED, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_FORMAT_R16G16B16_SSCALED (const VkFormat& v, const location_type& l)
      {
        return symbol_type (token::TOK_FORMAT_R16G16B16_SSCALED, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_FORMAT_R16G16B16_UINT (VkFormat v, location_type l)
      {
        return symbol_type (token::TOK_FORMAT_R16G16B16_UINT, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_FORMAT_R16G16B16_UINT (const VkFormat& v, const location_type& l)
      {
        return symbol_type (token::TOK_FORMAT_R16G16B16_UINT, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_FORMAT_R16G16B16_SINT (VkFormat v, location_type l)
      {
        return symbol_type (token::TOK_FORMAT_R16G16B16_SINT, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_FORMAT_R16G16B16_SINT (const VkFormat& v, const location_type& l)
      {
        return symbol_type (token::TOK_FORMAT_R16G16B16_SINT, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_FORMAT_R16G16B16_SFLOAT (VkFormat v, location_type l)
      {
        return symbol_type (token::TOK_FORMAT_R16G16B16_SFLOAT, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_FORMAT_R16G16B16_SFLOAT (const VkFormat& v, const location_type& l)
      {
        return symbol_type (token::TOK_FORMAT_R16G16B16_SFLOAT, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_FORMAT_R16G16B16A16_UNORM (VkFormat v, location_type l)
      {
        return symbol_type (token::TOK_FORMAT_R16G16B16A16_UNORM, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_FORMAT_R16G16B16A16_UNORM (const VkFormat& v, const location_type& l)
      {
        return symbol_type (token::TOK_FORMAT_R16G16B16A16_UNORM, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_FORMAT_R16G16B16A16_SNORM (VkFormat v, location_type l)
      {
        return symbol_type (token::TOK_FORMAT_R16G16B16A16_SNORM, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_FORMAT_R16G16B16A16_SNORM (const VkFormat& v, const location_type& l)
      {
        return symbol_type (token::TOK_FORMAT_R16G16B16A16_SNORM, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_FORMAT_R16G16B16A16_USCALED (VkFormat v, location_type l)
      {
        return symbol_type (token::TOK_FORMAT_R16G16B16A16_USCALED, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_FORMAT_R16G16B16A16_USCALED (const VkFormat& v, const location_type& l)
      {
        return symbol_type (token::TOK_FORMAT_R16G16B16A16_USCALED, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_FORMAT_R16G16B16A16_SSCALED (VkFormat v, location_type l)
      {
        return symbol_type (token::TOK_FORMAT_R16G16B16A16_SSCALED, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_FORMAT_R16G16B16A16_SSCALED (const VkFormat& v, const location_type& l)
      {
        return symbol_type (token::TOK_FORMAT_R16G16B16A16_SSCALED, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_FORMAT_R16G16B16A16_UINT (VkFormat v, location_type l)
      {
        return symbol_type (token::TOK_FORMAT_R16G16B16A16_UINT, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_FORMAT_R16G16B16A16_UINT (const VkFormat& v, const location_type& l)
      {
        return symbol_type (token::TOK_FORMAT_R16G16B16A16_UINT, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_FORMAT_R16G16B16A16_SINT (VkFormat v, location_type l)
      {
        return symbol_type (token::TOK_FORMAT_R16G16B16A16_SINT, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_FORMAT_R16G16B16A16_SINT (const VkFormat& v, const location_type& l)
      {
        return symbol_type (token::TOK_FORMAT_R16G16B16A16_SINT, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_FORMAT_R16G16B16A16_SFLOAT (VkFormat v, location_type l)
      {
        return symbol_type (token::TOK_FORMAT_R16G16B16A16_SFLOAT, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_FORMAT_R16G16B16A16_SFLOAT (const VkFormat& v, const location_type& l)
      {
        return symbol_type (token::TOK_FORMAT_R16G16B16A16_SFLOAT, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_FORMAT_R32_UINT (VkFormat v, location_type l)
      {
        return symbol_type (token::TOK_FORMAT_R32_UINT, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_FORMAT_R32_UINT (const VkFormat& v, const location_type& l)
      {
        return symbol_type (token::TOK_FORMAT_R32_UINT, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_FORMAT_R32_SINT (VkFormat v, location_type l)
      {
        return symbol_type (token::TOK_FORMAT_R32_SINT, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_FORMAT_R32_SINT (const VkFormat& v, const location_type& l)
      {
        return symbol_type (token::TOK_FORMAT_R32_SINT, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_FORMAT_R32_SFLOAT (VkFormat v, location_type l)
      {
        return symbol_type (token::TOK_FORMAT_R32_SFLOAT, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_FORMAT_R32_SFLOAT (const VkFormat& v, const location_type& l)
      {
        return symbol_type (token::TOK_FORMAT_R32_SFLOAT, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_FORMAT_R32G32_UINT (VkFormat v, location_type l)
      {
        return symbol_type (token::TOK_FORMAT_R32G32_UINT, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_FORMAT_R32G32_UINT (const VkFormat& v, const location_type& l)
      {
        return symbol_type (token::TOK_FORMAT_R32G32_UINT, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_FORMAT_R32G32_SINT (VkFormat v, location_type l)
      {
        return symbol_type (token::TOK_FORMAT_R32G32_SINT, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_FORMAT_R32G32_SINT (const VkFormat& v, const location_type& l)
      {
        return symbol_type (token::TOK_FORMAT_R32G32_SINT, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_FORMAT_R32G32_SFLOAT (VkFormat v, location_type l)
      {
        return symbol_type (token::TOK_FORMAT_R32G32_SFLOAT, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_FORMAT_R32G32_SFLOAT (const VkFormat& v, const location_type& l)
      {
        return symbol_type (token::TOK_FORMAT_R32G32_SFLOAT, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_FORMAT_R32G32B32_UINT (VkFormat v, location_type l)
      {
        return symbol_type (token::TOK_FORMAT_R32G32B32_UINT, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_FORMAT_R32G32B32_UINT (const VkFormat& v, const location_type& l)
      {
        return symbol_type (token::TOK_FORMAT_R32G32B32_UINT, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_FORMAT_R32G32B32_SINT (VkFormat v, location_type l)
      {
        return symbol_type (token::TOK_FORMAT_R32G32B32_SINT, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_FORMAT_R32G32B32_SINT (const VkFormat& v, const location_type& l)
      {
        return symbol_type (token::TOK_FORMAT_R32G32B32_SINT, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_FORMAT_R32G32B32_SFLOAT (VkFormat v, location_type l)
      {
        return symbol_type (token::TOK_FORMAT_R32G32B32_SFLOAT, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_FORMAT_R32G32B32_SFLOAT (const VkFormat& v, const location_type& l)
      {
        return symbol_type (token::TOK_FORMAT_R32G32B32_SFLOAT, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_FORMAT_R32G32B32A32_UINT (VkFormat v, location_type l)
      {
        return symbol_type (token::TOK_FORMAT_R32G32B32A32_UINT, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_FORMAT_R32G32B32A32_UINT (const VkFormat& v, const location_type& l)
      {
        return symbol_type (token::TOK_FORMAT_R32G32B32A32_UINT, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_FORMAT_R32G32B32A32_SINT (VkFormat v, location_type l)
      {
        return symbol_type (token::TOK_FORMAT_R32G32B32A32_SINT, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_FORMAT_R32G32B32A32_SINT (const VkFormat& v, const location_type& l)
      {
        return symbol_type (token::TOK_FORMAT_R32G32B32A32_SINT, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_FORMAT_R32G32B32A32_SFLOAT (VkFormat v, location_type l)
      {
        return symbol_type (token::TOK_FORMAT_R32G32B32A32_SFLOAT, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_FORMAT_R32G32B32A32_SFLOAT (const VkFormat& v, const location_type& l)
      {
        return symbol_type (token::TOK_FORMAT_R32G32B32A32_SFLOAT, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_FORMAT_R64_UINT (VkFormat v, location_type l)
      {
        return symbol_type (token::TOK_FORMAT_R64_UINT, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_FORMAT_R64_UINT (const VkFormat& v, const location_type& l)
      {
        return symbol_type (token::TOK_FORMAT_R64_UINT, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_FORMAT_R64_SINT (VkFormat v, location_type l)
      {
        return symbol_type (token::TOK_FORMAT_R64_SINT, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_FORMAT_R64_SINT (const VkFormat& v, const location_type& l)
      {
        return symbol_type (token::TOK_FORMAT_R64_SINT, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_FORMAT_R64_SFLOAT (VkFormat v, location_type l)
      {
        return symbol_type (token::TOK_FORMAT_R64_SFLOAT, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_FORMAT_R64_SFLOAT (const VkFormat& v, const location_type& l)
      {
        return symbol_type (token::TOK_FORMAT_R64_SFLOAT, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_FORMAT_R64G64_UINT (VkFormat v, location_type l)
      {
        return symbol_type (token::TOK_FORMAT_R64G64_UINT, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_FORMAT_R64G64_UINT (const VkFormat& v, const location_type& l)
      {
        return symbol_type (token::TOK_FORMAT_R64G64_UINT, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_FORMAT_R64G64_SINT (VkFormat v, location_type l)
      {
        return symbol_type (token::TOK_FORMAT_R64G64_SINT, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_FORMAT_R64G64_SINT (const VkFormat& v, const location_type& l)
      {
        return symbol_type (token::TOK_FORMAT_R64G64_SINT, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_FORMAT_R64G64_SFLOAT (VkFormat v, location_type l)
      {
        return symbol_type (token::TOK_FORMAT_R64G64_SFLOAT, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_FORMAT_R64G64_SFLOAT (const VkFormat& v, const location_type& l)
      {
        return symbol_type (token::TOK_FORMAT_R64G64_SFLOAT, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_FORMAT_R64G64B64_UINT (VkFormat v, location_type l)
      {
        return symbol_type (token::TOK_FORMAT_R64G64B64_UINT, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_FORMAT_R64G64B64_UINT (const VkFormat& v, const location_type& l)
      {
        return symbol_type (token::TOK_FORMAT_R64G64B64_UINT, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_FORMAT_R64G64B64_SINT (VkFormat v, location_type l)
      {
        return symbol_type (token::TOK_FORMAT_R64G64B64_SINT, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_FORMAT_R64G64B64_SINT (const VkFormat& v, const location_type& l)
      {
        return symbol_type (token::TOK_FORMAT_R64G64B64_SINT, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_FORMAT_R64G64B64_SFLOAT (VkFormat v, location_type l)
      {
        return symbol_type (token::TOK_FORMAT_R64G64B64_SFLOAT, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_FORMAT_R64G64B64_SFLOAT (const VkFormat& v, const location_type& l)
      {
        return symbol_type (token::TOK_FORMAT_R64G64B64_SFLOAT, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_FORMAT_R64G64B64A64_UINT (VkFormat v, location_type l)
      {
        return symbol_type (token::TOK_FORMAT_R64G64B64A64_UINT, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_FORMAT_R64G64B64A64_UINT (const VkFormat& v, const location_type& l)
      {
        return symbol_type (token::TOK_FORMAT_R64G64B64A64_UINT, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_FORMAT_R64G64B64A64_SINT (VkFormat v, location_type l)
      {
        return symbol_type (token::TOK_FORMAT_R64G64B64A64_SINT, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_FORMAT_R64G64B64A64_SINT (const VkFormat& v, const location_type& l)
      {
        return symbol_type (token::TOK_FORMAT_R64G64B64A64_SINT, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_FORMAT_R64G64B64A64_SFLOAT (VkFormat v, location_type l)
      {
        return symbol_type (token::TOK_FORMAT_R64G64B64A64_SFLOAT, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_FORMAT_R64G64B64A64_SFLOAT (const VkFormat& v, const location_type& l)
      {
        return symbol_type (token::TOK_FORMAT_R64G64B64A64_SFLOAT, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_FORMAT_B10G11R11_UFLOAT_PACK32 (VkFormat v, location_type l)
      {
        return symbol_type (token::TOK_FORMAT_B10G11R11_UFLOAT_PACK32, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_FORMAT_B10G11R11_UFLOAT_PACK32 (const VkFormat& v, const location_type& l)
      {
        return symbol_type (token::TOK_FORMAT_B10G11R11_UFLOAT_PACK32, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_FORMAT_E5B9G9R9_UFLOAT_PACK32 (VkFormat v, location_type l)
      {
        return symbol_type (token::TOK_FORMAT_E5B9G9R9_UFLOAT_PACK32, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_FORMAT_E5B9G9R9_UFLOAT_PACK32 (const VkFormat& v, const location_type& l)
      {
        return symbol_type (token::TOK_FORMAT_E5B9G9R9_UFLOAT_PACK32, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_FORMAT_D16_UNORM (VkFormat v, location_type l)
      {
        return symbol_type (token::TOK_FORMAT_D16_UNORM, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_FORMAT_D16_UNORM (const VkFormat& v, const location_type& l)
      {
        return symbol_type (token::TOK_FORMAT_D16_UNORM, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_FORMAT_X8_D24_UNORM_PACK32 (VkFormat v, location_type l)
      {
        return symbol_type (token::TOK_FORMAT_X8_D24_UNORM_PACK32, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_FORMAT_X8_D24_UNORM_PACK32 (const VkFormat& v, const location_type& l)
      {
        return symbol_type (token::TOK_FORMAT_X8_D24_UNORM_PACK32, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_FORMAT_D32_SFLOAT (VkFormat v, location_type l)
      {
        return symbol_type (token::TOK_FORMAT_D32_SFLOAT, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_FORMAT_D32_SFLOAT (const VkFormat& v, const location_type& l)
      {
        return symbol_type (token::TOK_FORMAT_D32_SFLOAT, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_FORMAT_S8_UINT (VkFormat v, location_type l)
      {
        return symbol_type (token::TOK_FORMAT_S8_UINT, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_FORMAT_S8_UINT (const VkFormat& v, const location_type& l)
      {
        return symbol_type (token::TOK_FORMAT_S8_UINT, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_FORMAT_D16_UNORM_S8_UINT (VkFormat v, location_type l)
      {
        return symbol_type (token::TOK_FORMAT_D16_UNORM_S8_UINT, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_FORMAT_D16_UNORM_S8_UINT (const VkFormat& v, const location_type& l)
      {
        return symbol_type (token::TOK_FORMAT_D16_UNORM_S8_UINT, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_FORMAT_D24_UNORM_S8_UINT (VkFormat v, location_type l)
      {
        return symbol_type (token::TOK_FORMAT_D24_UNORM_S8_UINT, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_FORMAT_D24_UNORM_S8_UINT (const VkFormat& v, const location_type& l)
      {
        return symbol_type (token::TOK_FORMAT_D24_UNORM_S8_UINT, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_FORMAT_D32_SFLOAT_S8_UINT (VkFormat v, location_type l)
      {
        return symbol_type (token::TOK_FORMAT_D32_SFLOAT_S8_UINT, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_FORMAT_D32_SFLOAT_S8_UINT (const VkFormat& v, const location_type& l)
      {
        return symbol_type (token::TOK_FORMAT_D32_SFLOAT_S8_UINT, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_FORMAT_BC1_RGB_UNORM_BLOCK (VkFormat v, location_type l)
      {
        return symbol_type (token::TOK_FORMAT_BC1_RGB_UNORM_BLOCK, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_FORMAT_BC1_RGB_UNORM_BLOCK (const VkFormat& v, const location_type& l)
      {
        return symbol_type (token::TOK_FORMAT_BC1_RGB_UNORM_BLOCK, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_FORMAT_BC1_RGB_SRGB_BLOCK (VkFormat v, location_type l)
      {
        return symbol_type (token::TOK_FORMAT_BC1_RGB_SRGB_BLOCK, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_FORMAT_BC1_RGB_SRGB_BLOCK (const VkFormat& v, const location_type& l)
      {
        return symbol_type (token::TOK_FORMAT_BC1_RGB_SRGB_BLOCK, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_FORMAT_BC1_RGBA_UNORM_BLOCK (VkFormat v, location_type l)
      {
        return symbol_type (token::TOK_FORMAT_BC1_RGBA_UNORM_BLOCK, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_FORMAT_BC1_RGBA_UNORM_BLOCK (const VkFormat& v, const location_type& l)
      {
        return symbol_type (token::TOK_FORMAT_BC1_RGBA_UNORM_BLOCK, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_FORMAT_BC1_RGBA_SRGB_BLOCK (VkFormat v, location_type l)
      {
        return symbol_type (token::TOK_FORMAT_BC1_RGBA_SRGB_BLOCK, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_FORMAT_BC1_RGBA_SRGB_BLOCK (const VkFormat& v, const location_type& l)
      {
        return symbol_type (token::TOK_FORMAT_BC1_RGBA_SRGB_BLOCK, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_FORMAT_BC2_UNORM_BLOCK (VkFormat v, location_type l)
      {
        return symbol_type (token::TOK_FORMAT_BC2_UNORM_BLOCK, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_FORMAT_BC2_UNORM_BLOCK (const VkFormat& v, const location_type& l)
      {
        return symbol_type (token::TOK_FORMAT_BC2_UNORM_BLOCK, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_FORMAT_BC2_SRGB_BLOCK (VkFormat v, location_type l)
      {
        return symbol_type (token::TOK_FORMAT_BC2_SRGB_BLOCK, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_FORMAT_BC2_SRGB_BLOCK (const VkFormat& v, const location_type& l)
      {
        return symbol_type (token::TOK_FORMAT_BC2_SRGB_BLOCK, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_FORMAT_BC3_UNORM_BLOCK (VkFormat v, location_type l)
      {
        return symbol_type (token::TOK_FORMAT_BC3_UNORM_BLOCK, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_FORMAT_BC3_UNORM_BLOCK (const VkFormat& v, const location_type& l)
      {
        return symbol_type (token::TOK_FORMAT_BC3_UNORM_BLOCK, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_FORMAT_BC3_SRGB_BLOCK (VkFormat v, location_type l)
      {
        return symbol_type (token::TOK_FORMAT_BC3_SRGB_BLOCK, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_FORMAT_BC3_SRGB_BLOCK (const VkFormat& v, const location_type& l)
      {
        return symbol_type (token::TOK_FORMAT_BC3_SRGB_BLOCK, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_FORMAT_BC4_UNORM_BLOCK (VkFormat v, location_type l)
      {
        return symbol_type (token::TOK_FORMAT_BC4_UNORM_BLOCK, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_FORMAT_BC4_UNORM_BLOCK (const VkFormat& v, const location_type& l)
      {
        return symbol_type (token::TOK_FORMAT_BC4_UNORM_BLOCK, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_FORMAT_BC4_SNORM_BLOCK (VkFormat v, location_type l)
      {
        return symbol_type (token::TOK_FORMAT_BC4_SNORM_BLOCK, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_FORMAT_BC4_SNORM_BLOCK (const VkFormat& v, const location_type& l)
      {
        return symbol_type (token::TOK_FORMAT_BC4_SNORM_BLOCK, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_FORMAT_BC5_UNORM_BLOCK (VkFormat v, location_type l)
      {
        return symbol_type (token::TOK_FORMAT_BC5_UNORM_BLOCK, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_FORMAT_BC5_UNORM_BLOCK (const VkFormat& v, const location_type& l)
      {
        return symbol_type (token::TOK_FORMAT_BC5_UNORM_BLOCK, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_FORMAT_BC5_SNORM_BLOCK (VkFormat v, location_type l)
      {
        return symbol_type (token::TOK_FORMAT_BC5_SNORM_BLOCK, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_FORMAT_BC5_SNORM_BLOCK (const VkFormat& v, const location_type& l)
      {
        return symbol_type (token::TOK_FORMAT_BC5_SNORM_BLOCK, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_FORMAT_BC6H_UFLOAT_BLOCK (VkFormat v, location_type l)
      {
        return symbol_type (token::TOK_FORMAT_BC6H_UFLOAT_BLOCK, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_FORMAT_BC6H_UFLOAT_BLOCK (const VkFormat& v, const location_type& l)
      {
        return symbol_type (token::TOK_FORMAT_BC6H_UFLOAT_BLOCK, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_FORMAT_BC6H_SFLOAT_BLOCK (VkFormat v, location_type l)
      {
        return symbol_type (token::TOK_FORMAT_BC6H_SFLOAT_BLOCK, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_FORMAT_BC6H_SFLOAT_BLOCK (const VkFormat& v, const location_type& l)
      {
        return symbol_type (token::TOK_FORMAT_BC6H_SFLOAT_BLOCK, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_FORMAT_BC7_UNORM_BLOCK (VkFormat v, location_type l)
      {
        return symbol_type (token::TOK_FORMAT_BC7_UNORM_BLOCK, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_FORMAT_BC7_UNORM_BLOCK (const VkFormat& v, const location_type& l)
      {
        return symbol_type (token::TOK_FORMAT_BC7_UNORM_BLOCK, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_FORMAT_BC7_SRGB_BLOCK (VkFormat v, location_type l)
      {
        return symbol_type (token::TOK_FORMAT_BC7_SRGB_BLOCK, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_FORMAT_BC7_SRGB_BLOCK (const VkFormat& v, const location_type& l)
      {
        return symbol_type (token::TOK_FORMAT_BC7_SRGB_BLOCK, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_FORMAT_ETC2_R8G8B8_UNORM_BLOCK (VkFormat v, location_type l)
      {
        return symbol_type (token::TOK_FORMAT_ETC2_R8G8B8_UNORM_BLOCK, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_FORMAT_ETC2_R8G8B8_UNORM_BLOCK (const VkFormat& v, const location_type& l)
      {
        return symbol_type (token::TOK_FORMAT_ETC2_R8G8B8_UNORM_BLOCK, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_FORMAT_ETC2_R8G8B8_SRGB_BLOCK (VkFormat v, location_type l)
      {
        return symbol_type (token::TOK_FORMAT_ETC2_R8G8B8_SRGB_BLOCK, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_FORMAT_ETC2_R8G8B8_SRGB_BLOCK (const VkFormat& v, const location_type& l)
      {
        return symbol_type (token::TOK_FORMAT_ETC2_R8G8B8_SRGB_BLOCK, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_FORMAT_ETC2_R8G8B8A1_UNORM_BLOCK (VkFormat v, location_type l)
      {
        return symbol_type (token::TOK_FORMAT_ETC2_R8G8B8A1_UNORM_BLOCK, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_FORMAT_ETC2_R8G8B8A1_UNORM_BLOCK (const VkFormat& v, const location_type& l)
      {
        return symbol_type (token::TOK_FORMAT_ETC2_R8G8B8A1_UNORM_BLOCK, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_FORMAT_ETC2_R8G8B8A1_SRGB_BLOCK (VkFormat v, location_type l)
      {
        return symbol_type (token::TOK_FORMAT_ETC2_R8G8B8A1_SRGB_BLOCK, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_FORMAT_ETC2_R8G8B8A1_SRGB_BLOCK (const VkFormat& v, const location_type& l)
      {
        return symbol_type (token::TOK_FORMAT_ETC2_R8G8B8A1_SRGB_BLOCK, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_FORMAT_ETC2_R8G8B8A8_UNORM_BLOCK (VkFormat v, location_type l)
      {
        return symbol_type (token::TOK_FORMAT_ETC2_R8G8B8A8_UNORM_BLOCK, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_FORMAT_ETC2_R8G8B8A8_UNORM_BLOCK (const VkFormat& v, const location_type& l)
      {
        return symbol_type (token::TOK_FORMAT_ETC2_R8G8B8A8_UNORM_BLOCK, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_FORMAT_ETC2_R8G8B8A8_SRGB_BLOCK (VkFormat v, location_type l)
      {
        return symbol_type (token::TOK_FORMAT_ETC2_R8G8B8A8_SRGB_BLOCK, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_FORMAT_ETC2_R8G8B8A8_SRGB_BLOCK (const VkFormat& v, const location_type& l)
      {
        return symbol_type (token::TOK_FORMAT_ETC2_R8G8B8A8_SRGB_BLOCK, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_FORMAT_EAC_R11_UNORM_BLOCK (VkFormat v, location_type l)
      {
        return symbol_type (token::TOK_FORMAT_EAC_R11_UNORM_BLOCK, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_FORMAT_EAC_R11_UNORM_BLOCK (const VkFormat& v, const location_type& l)
      {
        return symbol_type (token::TOK_FORMAT_EAC_R11_UNORM_BLOCK, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_FORMAT_EAC_R11_SNORM_BLOCK (VkFormat v, location_type l)
      {
        return symbol_type (token::TOK_FORMAT_EAC_R11_SNORM_BLOCK, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_FORMAT_EAC_R11_SNORM_BLOCK (const VkFormat& v, const location_type& l)
      {
        return symbol_type (token::TOK_FORMAT_EAC_R11_SNORM_BLOCK, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_FORMAT_EAC_R11G11_UNORM_BLOCK (VkFormat v, location_type l)
      {
        return symbol_type (token::TOK_FORMAT_EAC_R11G11_UNORM_BLOCK, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_FORMAT_EAC_R11G11_UNORM_BLOCK (const VkFormat& v, const location_type& l)
      {
        return symbol_type (token::TOK_FORMAT_EAC_R11G11_UNORM_BLOCK, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_FORMAT_EAC_R11G11_SNORM_BLOCK (VkFormat v, location_type l)
      {
        return symbol_type (token::TOK_FORMAT_EAC_R11G11_SNORM_BLOCK, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_FORMAT_EAC_R11G11_SNORM_BLOCK (const VkFormat& v, const location_type& l)
      {
        return symbol_type (token::TOK_FORMAT_EAC_R11G11_SNORM_BLOCK, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_FORMAT_ASTC_4x4_UNORM_BLOCK (VkFormat v, location_type l)
      {
        return symbol_type (token::TOK_FORMAT_ASTC_4x4_UNORM_BLOCK, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_FORMAT_ASTC_4x4_UNORM_BLOCK (const VkFormat& v, const location_type& l)
      {
        return symbol_type (token::TOK_FORMAT_ASTC_4x4_UNORM_BLOCK, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_FORMAT_ASTC_4x4_SRGB_BLOCK (VkFormat v, location_type l)
      {
        return symbol_type (token::TOK_FORMAT_ASTC_4x4_SRGB_BLOCK, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_FORMAT_ASTC_4x4_SRGB_BLOCK (const VkFormat& v, const location_type& l)
      {
        return symbol_type (token::TOK_FORMAT_ASTC_4x4_SRGB_BLOCK, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_FORMAT_ASTC_5x4_UNORM_BLOCK (VkFormat v, location_type l)
      {
        return symbol_type (token::TOK_FORMAT_ASTC_5x4_UNORM_BLOCK, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_FORMAT_ASTC_5x4_UNORM_BLOCK (const VkFormat& v, const location_type& l)
      {
        return symbol_type (token::TOK_FORMAT_ASTC_5x4_UNORM_BLOCK, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_FORMAT_ASTC_5x4_SRGB_BLOCK (VkFormat v, location_type l)
      {
        return symbol_type (token::TOK_FORMAT_ASTC_5x4_SRGB_BLOCK, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_FORMAT_ASTC_5x4_SRGB_BLOCK (const VkFormat& v, const location_type& l)
      {
        return symbol_type (token::TOK_FORMAT_ASTC_5x4_SRGB_BLOCK, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_FORMAT_ASTC_5x5_UNORM_BLOCK (VkFormat v, location_type l)
      {
        return symbol_type (token::TOK_FORMAT_ASTC_5x5_UNORM_BLOCK, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_FORMAT_ASTC_5x5_UNORM_BLOCK (const VkFormat& v, const location_type& l)
      {
        return symbol_type (token::TOK_FORMAT_ASTC_5x5_UNORM_BLOCK, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_FORMAT_ASTC_5x5_SRGB_BLOCK (VkFormat v, location_type l)
      {
        return symbol_type (token::TOK_FORMAT_ASTC_5x5_SRGB_BLOCK, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_FORMAT_ASTC_5x5_SRGB_BLOCK (const VkFormat& v, const location_type& l)
      {
        return symbol_type (token::TOK_FORMAT_ASTC_5x5_SRGB_BLOCK, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_FORMAT_ASTC_6x5_UNORM_BLOCK (VkFormat v, location_type l)
      {
        return symbol_type (token::TOK_FORMAT_ASTC_6x5_UNORM_BLOCK, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_FORMAT_ASTC_6x5_UNORM_BLOCK (const VkFormat& v, const location_type& l)
      {
        return symbol_type (token::TOK_FORMAT_ASTC_6x5_UNORM_BLOCK, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_FORMAT_ASTC_6x5_SRGB_BLOCK (VkFormat v, location_type l)
      {
        return symbol_type (token::TOK_FORMAT_ASTC_6x5_SRGB_BLOCK, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_FORMAT_ASTC_6x5_SRGB_BLOCK (const VkFormat& v, const location_type& l)
      {
        return symbol_type (token::TOK_FORMAT_ASTC_6x5_SRGB_BLOCK, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_FORMAT_ASTC_6x6_UNORM_BLOCK (VkFormat v, location_type l)
      {
        return symbol_type (token::TOK_FORMAT_ASTC_6x6_UNORM_BLOCK, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_FORMAT_ASTC_6x6_UNORM_BLOCK (const VkFormat& v, const location_type& l)
      {
        return symbol_type (token::TOK_FORMAT_ASTC_6x6_UNORM_BLOCK, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_FORMAT_ASTC_6x6_SRGB_BLOCK (VkFormat v, location_type l)
      {
        return symbol_type (token::TOK_FORMAT_ASTC_6x6_SRGB_BLOCK, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_FORMAT_ASTC_6x6_SRGB_BLOCK (const VkFormat& v, const location_type& l)
      {
        return symbol_type (token::TOK_FORMAT_ASTC_6x6_SRGB_BLOCK, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_FORMAT_ASTC_8x5_UNORM_BLOCK (VkFormat v, location_type l)
      {
        return symbol_type (token::TOK_FORMAT_ASTC_8x5_UNORM_BLOCK, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_FORMAT_ASTC_8x5_UNORM_BLOCK (const VkFormat& v, const location_type& l)
      {
        return symbol_type (token::TOK_FORMAT_ASTC_8x5_UNORM_BLOCK, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_FORMAT_ASTC_8x5_SRGB_BLOCK (VkFormat v, location_type l)
      {
        return symbol_type (token::TOK_FORMAT_ASTC_8x5_SRGB_BLOCK, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_FORMAT_ASTC_8x5_SRGB_BLOCK (const VkFormat& v, const location_type& l)
      {
        return symbol_type (token::TOK_FORMAT_ASTC_8x5_SRGB_BLOCK, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_FORMAT_ASTC_8x6_UNORM_BLOCK (VkFormat v, location_type l)
      {
        return symbol_type (token::TOK_FORMAT_ASTC_8x6_UNORM_BLOCK, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_FORMAT_ASTC_8x6_UNORM_BLOCK (const VkFormat& v, const location_type& l)
      {
        return symbol_type (token::TOK_FORMAT_ASTC_8x6_UNORM_BLOCK, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_FORMAT_ASTC_8x6_SRGB_BLOCK (VkFormat v, location_type l)
      {
        return symbol_type (token::TOK_FORMAT_ASTC_8x6_SRGB_BLOCK, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_FORMAT_ASTC_8x6_SRGB_BLOCK (const VkFormat& v, const location_type& l)
      {
        return symbol_type (token::TOK_FORMAT_ASTC_8x6_SRGB_BLOCK, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_FORMAT_ASTC_8x8_UNORM_BLOCK (VkFormat v, location_type l)
      {
        return symbol_type (token::TOK_FORMAT_ASTC_8x8_UNORM_BLOCK, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_FORMAT_ASTC_8x8_UNORM_BLOCK (const VkFormat& v, const location_type& l)
      {
        return symbol_type (token::TOK_FORMAT_ASTC_8x8_UNORM_BLOCK, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_FORMAT_ASTC_8x8_SRGB_BLOCK (VkFormat v, location_type l)
      {
        return symbol_type (token::TOK_FORMAT_ASTC_8x8_SRGB_BLOCK, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_FORMAT_ASTC_8x8_SRGB_BLOCK (const VkFormat& v, const location_type& l)
      {
        return symbol_type (token::TOK_FORMAT_ASTC_8x8_SRGB_BLOCK, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_FORMAT_ASTC_10x5_UNORM_BLOCK (VkFormat v, location_type l)
      {
        return symbol_type (token::TOK_FORMAT_ASTC_10x5_UNORM_BLOCK, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_FORMAT_ASTC_10x5_UNORM_BLOCK (const VkFormat& v, const location_type& l)
      {
        return symbol_type (token::TOK_FORMAT_ASTC_10x5_UNORM_BLOCK, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_FORMAT_ASTC_10x5_SRGB_BLOCK (VkFormat v, location_type l)
      {
        return symbol_type (token::TOK_FORMAT_ASTC_10x5_SRGB_BLOCK, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_FORMAT_ASTC_10x5_SRGB_BLOCK (const VkFormat& v, const location_type& l)
      {
        return symbol_type (token::TOK_FORMAT_ASTC_10x5_SRGB_BLOCK, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_FORMAT_ASTC_10x6_UNORM_BLOCK (VkFormat v, location_type l)
      {
        return symbol_type (token::TOK_FORMAT_ASTC_10x6_UNORM_BLOCK, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_FORMAT_ASTC_10x6_UNORM_BLOCK (const VkFormat& v, const location_type& l)
      {
        return symbol_type (token::TOK_FORMAT_ASTC_10x6_UNORM_BLOCK, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_FORMAT_ASTC_10x6_SRGB_BLOCK (VkFormat v, location_type l)
      {
        return symbol_type (token::TOK_FORMAT_ASTC_10x6_SRGB_BLOCK, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_FORMAT_ASTC_10x6_SRGB_BLOCK (const VkFormat& v, const location_type& l)
      {
        return symbol_type (token::TOK_FORMAT_ASTC_10x6_SRGB_BLOCK, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_FORMAT_ASTC_10x8_UNORM_BLOCK (VkFormat v, location_type l)
      {
        return symbol_type (token::TOK_FORMAT_ASTC_10x8_UNORM_BLOCK, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_FORMAT_ASTC_10x8_UNORM_BLOCK (const VkFormat& v, const location_type& l)
      {
        return symbol_type (token::TOK_FORMAT_ASTC_10x8_UNORM_BLOCK, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_FORMAT_ASTC_10x8_SRGB_BLOCK (VkFormat v, location_type l)
      {
        return symbol_type (token::TOK_FORMAT_ASTC_10x8_SRGB_BLOCK, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_FORMAT_ASTC_10x8_SRGB_BLOCK (const VkFormat& v, const location_type& l)
      {
        return symbol_type (token::TOK_FORMAT_ASTC_10x8_SRGB_BLOCK, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_FORMAT_ASTC_10x10_UNORM_BLOCK (VkFormat v, location_type l)
      {
        return symbol_type (token::TOK_FORMAT_ASTC_10x10_UNORM_BLOCK, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_FORMAT_ASTC_10x10_UNORM_BLOCK (const VkFormat& v, const location_type& l)
      {
        return symbol_type (token::TOK_FORMAT_ASTC_10x10_UNORM_BLOCK, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_FORMAT_ASTC_10x10_SRGB_BLOCK (VkFormat v, location_type l)
      {
        return symbol_type (token::TOK_FORMAT_ASTC_10x10_SRGB_BLOCK, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_FORMAT_ASTC_10x10_SRGB_BLOCK (const VkFormat& v, const location_type& l)
      {
        return symbol_type (token::TOK_FORMAT_ASTC_10x10_SRGB_BLOCK, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_FORMAT_ASTC_12x10_UNORM_BLOCK (VkFormat v, location_type l)
      {
        return symbol_type (token::TOK_FORMAT_ASTC_12x10_UNORM_BLOCK, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_FORMAT_ASTC_12x10_UNORM_BLOCK (const VkFormat& v, const location_type& l)
      {
        return symbol_type (token::TOK_FORMAT_ASTC_12x10_UNORM_BLOCK, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_FORMAT_ASTC_12x10_SRGB_BLOCK (VkFormat v, location_type l)
      {
        return symbol_type (token::TOK_FORMAT_ASTC_12x10_SRGB_BLOCK, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_FORMAT_ASTC_12x10_SRGB_BLOCK (const VkFormat& v, const location_type& l)
      {
        return symbol_type (token::TOK_FORMAT_ASTC_12x10_SRGB_BLOCK, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_FORMAT_ASTC_12x12_UNORM_BLOCK (VkFormat v, location_type l)
      {
        return symbol_type (token::TOK_FORMAT_ASTC_12x12_UNORM_BLOCK, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_FORMAT_ASTC_12x12_UNORM_BLOCK (const VkFormat& v, const location_type& l)
      {
        return symbol_type (token::TOK_FORMAT_ASTC_12x12_UNORM_BLOCK, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_FORMAT_ASTC_12x12_SRGB_BLOCK (VkFormat v, location_type l)
      {
        return symbol_type (token::TOK_FORMAT_ASTC_12x12_SRGB_BLOCK, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_FORMAT_ASTC_12x12_SRGB_BLOCK (const VkFormat& v, const location_type& l)
      {
        return symbol_type (token::TOK_FORMAT_ASTC_12x12_SRGB_BLOCK, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_VERTEXFORMAT (location_type l)
      {
        return symbol_type (token::TOK_VERTEXFORMAT, std::move (l));
      }
#else
      static
      symbol_type
      make_VERTEXFORMAT (const location_type& l)
      {
        return symbol_type (token::TOK_VERTEXFORMAT, l);
      }
#endif


    class context
    {
    public:
      context (const parser& yyparser, const symbol_type& yyla);
      const symbol_type& lookahead () const { return yyla_; }
      symbol_kind_type token () const { return yyla_.kind (); }
      const location_type& location () const { return yyla_.location; }

      /// Put in YYARG at most YYARGN of the expected tokens, and return the
      /// number of tokens stored in YYARG.  If YYARG is null, return the
      /// number of expected tokens (guaranteed to be less than YYNTOKENS).
      int expected_tokens (symbol_kind_type yyarg[], int yyargn) const;

    private:
      const parser& yyparser_;
      const symbol_type& yyla_;
    };

  private:
#if YY_CPLUSPLUS < 201103L
    /// Non copyable.
    parser (const parser&);
    /// Non copyable.
    parser& operator= (const parser&);
#endif


    /// Stored state numbers (used for stacks).
    typedef unsigned char state_type;

    /// The arguments of the error message.
    int yy_syntax_error_arguments_ (const context& yyctx,
                                    symbol_kind_type yyarg[], int yyargn) const;

    /// Generate an error message.
    /// \param yyctx     the context in which the error occurred.
    virtual std::string yysyntax_error_ (const context& yyctx) const;
    /// Compute post-reduction state.
    /// \param yystate   the current state
    /// \param yysym     the nonterminal to push on the stack
    static state_type yy_lr_goto_state_ (state_type yystate, int yysym);

    /// Whether the given \c yypact_ value indicates a defaulted state.
    /// \param yyvalue   the value to check
    static bool yy_pact_value_is_default_ (int yyvalue);

    /// Whether the given \c yytable_ value indicates a syntax error.
    /// \param yyvalue   the value to check
    static bool yy_table_value_is_error_ (int yyvalue);

    static const signed char yypact_ninf_;
    static const signed char yytable_ninf_;

    /// Convert a scanner token kind \a t to a symbol kind.
    /// In theory \a t should be a token_kind_type, but character literals
    /// are valid, yet not members of the token_type enum.
    static symbol_kind_type yytranslate_ (int t);

    /// Convert the symbol name \a n to a form suitable for a diagnostic.
    static std::string yytnamerr_ (const char *yystr);

    /// For a symbol, its name in clear.
    static const char* const yytname_[];


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
    static const short yydefgoto_[];

    // YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
    // positive, shift that token.  If negative, reduce the rule whose
    // number is the opposite.  If YYTABLE_NINF, syntax error.
    static const unsigned char yytable_[];

    static const short yycheck_[];

    // YYSTOS[STATE-NUM] -- The (internal number of the) accessing
    // symbol of state STATE-NUM.
    static const short yystos_[];

    // YYR1[YYN] -- Symbol number of symbol that rule YYN derives.
    static const short yyr1_[];

    // YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.
    static const signed char yyr2_[];


#if YYDEBUG
    // YYRLINE[YYN] -- Source line where rule number YYN was defined.
    static const short yyrline_[];
    /// Report on the debug stream that the rule \a r is going to be reduced.
    virtual void yy_reduce_print_ (int r) const;
    /// Print the state stack on the debug stream.
    virtual void yy_stack_print_ () const;

    /// Debugging level.
    int yydebug_;
    /// Debug stream.
    std::ostream* yycdebug_;

    /// \brief Display a symbol kind, value and location.
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
      by_state () YY_NOEXCEPT;

      /// The symbol kind as needed by the constructor.
      typedef state_type kind_type;

      /// Constructor.
      by_state (kind_type s) YY_NOEXCEPT;

      /// Copy constructor.
      by_state (const by_state& that) YY_NOEXCEPT;

      /// Record that this symbol is empty.
      void clear () YY_NOEXCEPT;

      /// Steal the symbol kind from \a that.
      void move (by_state& that);

      /// The symbol kind (corresponding to \a state).
      /// \a symbol_kind::S_YYEMPTY when empty.
      symbol_kind_type kind () const YY_NOEXCEPT;

      /// The state number used to denote an empty symbol.
      /// We use the initial state, as it does not have a value.
      enum { empty_state = 0 };

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
      /// Move or copy construction.
      stack_symbol_type (YY_RVREF (stack_symbol_type) that);
      /// Steal the contents from \a sym to build this.
      stack_symbol_type (state_type s, YY_MOVE_REF (symbol_type) sym);
#if YY_CPLUSPLUS < 201103L
      /// Assignment, needed by push_back by some old implementations.
      /// Moves the contents of that.
      stack_symbol_type& operator= (stack_symbol_type& that);

      /// Assignment, needed by push_back by other implementations.
      /// Needed by some other old implementations.
      stack_symbol_type& operator= (const stack_symbol_type& that);
#endif
    };

    /// A stack with random access from its top.
    template <typename T, typename S = std::vector<T> >
    class stack
    {
    public:
      // Hide our reversed order.
      typedef typename S::iterator iterator;
      typedef typename S::const_iterator const_iterator;
      typedef typename S::size_type size_type;
      typedef typename std::ptrdiff_t index_type;

      stack (size_type n = 200)
        : seq_ (n)
      {}

#if 201103L <= YY_CPLUSPLUS
      /// Non copyable.
      stack (const stack&) = delete;
      /// Non copyable.
      stack& operator= (const stack&) = delete;
#endif

      /// Random access.
      ///
      /// Index 0 returns the topmost element.
      const T&
      operator[] (index_type i) const
      {
        return seq_[size_type (size () - 1 - i)];
      }

      /// Random access.
      ///
      /// Index 0 returns the topmost element.
      T&
      operator[] (index_type i)
      {
        return seq_[size_type (size () - 1 - i)];
      }

      /// Steal the contents of \a t.
      ///
      /// Close to move-semantics.
      void
      push (YY_MOVE_REF (T) t)
      {
        seq_.push_back (T ());
        operator[] (0).move (t);
      }

      /// Pop elements from the stack.
      void
      pop (std::ptrdiff_t n = 1) YY_NOEXCEPT
      {
        for (; 0 < n; --n)
          seq_.pop_back ();
      }

      /// Pop all elements from the stack.
      void
      clear () YY_NOEXCEPT
      {
        seq_.clear ();
      }

      /// Number of elements on the stack.
      index_type
      size () const YY_NOEXCEPT
      {
        return index_type (seq_.size ());
      }

      /// Iterator on top of the stack (going downwards).
      const_iterator
      begin () const YY_NOEXCEPT
      {
        return seq_.begin ();
      }

      /// Bottom of the stack.
      const_iterator
      end () const YY_NOEXCEPT
      {
        return seq_.end ();
      }

      /// Present a slice of the top of a stack.
      class slice
      {
      public:
        slice (const stack& stack, index_type range)
          : stack_ (stack)
          , range_ (range)
        {}

        const T&
        operator[] (index_type i) const
        {
          return stack_[range_ - i];
        }

      private:
        const stack& stack_;
        index_type range_;
      };

    private:
#if YY_CPLUSPLUS < 201103L
      /// Non copyable.
      stack (const stack&);
      /// Non copyable.
      stack& operator= (const stack&);
#endif
      /// The wrapped container.
      S seq_;
    };


    /// Stack type.
    typedef stack<stack_symbol_type> stack_type;

    /// The stack.
    stack_type yystack_;

    /// Push a new state on the stack.
    /// \param m    a debug message to display
    ///             if null, no trace is output.
    /// \param sym  the symbol
    /// \warning the contents of \a s.value is stolen.
    void yypush_ (const char* m, YY_MOVE_REF (stack_symbol_type) sym);

    /// Push a new look ahead token on the state on the stack.
    /// \param m    a debug message to display
    ///             if null, no trace is output.
    /// \param s    the state
    /// \param sym  the symbol (for its value and location).
    /// \warning the contents of \a sym.value is stolen.
    void yypush_ (const char* m, state_type s, YY_MOVE_REF (symbol_type) sym);

    /// Pop \a n symbols from the stack.
    void yypop_ (int n = 1);

    /// Constants.
    enum
    {
      yylast_ = 308,     ///< Last index in yytable_.
      yynnts_ = 18,  ///< Number of nonterminal symbols.
      yyfinal_ = 3 ///< Termination state number.
    };


    // User arguments.
    Scanner &scanner;
    Driver &driver;

  };

  inline
  parser::symbol_kind_type
  parser::yytranslate_ (int t)
  {
    // YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to
    // TOKEN-NUM as returned by yylex.
    static
    const short
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
    // Last valid token kind.
    const int code_max = 549;

    if (t <= 0)
      return symbol_kind::S_YYEOF;
    else if (t <= code_max)
      return YY_CAST (symbol_kind_type, translate_table[t]);
    else
      return symbol_kind::S_YYUNDEF;
  }

  // basic_symbol.
  template <typename Base>
  parser::basic_symbol<Base>::basic_symbol (const basic_symbol& that)
    : Base (that)
    , value ()
    , location (that.location)
  {
    switch (this->kind ())
    {
      case symbol_kind::S_FORMAT_UNDEFINED: // FORMAT_UNDEFINED
      case symbol_kind::S_FORMAT_R4G4_UNORM_PACK8: // FORMAT_R4G4_UNORM_PACK8
      case symbol_kind::S_FORMAT_R4G4B4A4_UNORM_PACK16: // FORMAT_R4G4B4A4_UNORM_PACK16
      case symbol_kind::S_FORMAT_B4G4R4A4_UNORM_PACK16: // FORMAT_B4G4R4A4_UNORM_PACK16
      case symbol_kind::S_FORMAT_R5G6B5_UNORM_PACK16: // FORMAT_R5G6B5_UNORM_PACK16
      case symbol_kind::S_FORMAT_B5G6R5_UNORM_PACK16: // FORMAT_B5G6R5_UNORM_PACK16
      case symbol_kind::S_FORMAT_R5G5B5A1_UNORM_PACK16: // FORMAT_R5G5B5A1_UNORM_PACK16
      case symbol_kind::S_FORMAT_B5G5R5A1_UNORM_PACK16: // FORMAT_B5G5R5A1_UNORM_PACK16
      case symbol_kind::S_FORMAT_A1R5G5B5_UNORM_PACK16: // FORMAT_A1R5G5B5_UNORM_PACK16
      case symbol_kind::S_FORMAT_R8_UNORM: // FORMAT_R8_UNORM
      case symbol_kind::S_FORMAT_R8_SNORM: // FORMAT_R8_SNORM
      case symbol_kind::S_FORMAT_R8_USCALED: // FORMAT_R8_USCALED
      case symbol_kind::S_FORMAT_R8_SSCALED: // FORMAT_R8_SSCALED
      case symbol_kind::S_FORMAT_R8_UINT: // FORMAT_R8_UINT
      case symbol_kind::S_FORMAT_R8_SINT: // FORMAT_R8_SINT
      case symbol_kind::S_FORMAT_R8_SRGB: // FORMAT_R8_SRGB
      case symbol_kind::S_FORMAT_R8G8_UNORM: // FORMAT_R8G8_UNORM
      case symbol_kind::S_FORMAT_R8G8_SNORM: // FORMAT_R8G8_SNORM
      case symbol_kind::S_FORMAT_R8G8_USCALED: // FORMAT_R8G8_USCALED
      case symbol_kind::S_FORMAT_R8G8_SSCALED: // FORMAT_R8G8_SSCALED
      case symbol_kind::S_FORMAT_R8G8_UINT: // FORMAT_R8G8_UINT
      case symbol_kind::S_FORMAT_R8G8_SINT: // FORMAT_R8G8_SINT
      case symbol_kind::S_FORMAT_R8G8_SRGB: // FORMAT_R8G8_SRGB
      case symbol_kind::S_FORMAT_R8G8B8_UNORM: // FORMAT_R8G8B8_UNORM
      case symbol_kind::S_FORMAT_R8G8B8_SNORM: // FORMAT_R8G8B8_SNORM
      case symbol_kind::S_FORMAT_R8G8B8_USCALED: // FORMAT_R8G8B8_USCALED
      case symbol_kind::S_FORMAT_R8G8B8_SSCALED: // FORMAT_R8G8B8_SSCALED
      case symbol_kind::S_FORMAT_R8G8B8_UINT: // FORMAT_R8G8B8_UINT
      case symbol_kind::S_FORMAT_R8G8B8_SINT: // FORMAT_R8G8B8_SINT
      case symbol_kind::S_FORMAT_R8G8B8_SRGB: // FORMAT_R8G8B8_SRGB
      case symbol_kind::S_FORMAT_B8G8R8_UNORM: // FORMAT_B8G8R8_UNORM
      case symbol_kind::S_FORMAT_B8G8R8_SNORM: // FORMAT_B8G8R8_SNORM
      case symbol_kind::S_FORMAT_B8G8R8_USCALED: // FORMAT_B8G8R8_USCALED
      case symbol_kind::S_FORMAT_B8G8R8_SSCALED: // FORMAT_B8G8R8_SSCALED
      case symbol_kind::S_FORMAT_B8G8R8_UINT: // FORMAT_B8G8R8_UINT
      case symbol_kind::S_FORMAT_B8G8R8_SINT: // FORMAT_B8G8R8_SINT
      case symbol_kind::S_FORMAT_B8G8R8_SRGB: // FORMAT_B8G8R8_SRGB
      case symbol_kind::S_FORMAT_R8G8B8A8_UNORM: // FORMAT_R8G8B8A8_UNORM
      case symbol_kind::S_FORMAT_R8G8B8A8_SNORM: // FORMAT_R8G8B8A8_SNORM
      case symbol_kind::S_FORMAT_R8G8B8A8_USCALED: // FORMAT_R8G8B8A8_USCALED
      case symbol_kind::S_FORMAT_R8G8B8A8_SSCALED: // FORMAT_R8G8B8A8_SSCALED
      case symbol_kind::S_FORMAT_R8G8B8A8_UINT: // FORMAT_R8G8B8A8_UINT
      case symbol_kind::S_FORMAT_R8G8B8A8_SINT: // FORMAT_R8G8B8A8_SINT
      case symbol_kind::S_FORMAT_R8G8B8A8_SRGB: // FORMAT_R8G8B8A8_SRGB
      case symbol_kind::S_FORMAT_B8G8R8A8_UNORM: // FORMAT_B8G8R8A8_UNORM
      case symbol_kind::S_FORMAT_B8G8R8A8_SNORM: // FORMAT_B8G8R8A8_SNORM
      case symbol_kind::S_FORMAT_B8G8R8A8_USCALED: // FORMAT_B8G8R8A8_USCALED
      case symbol_kind::S_FORMAT_B8G8R8A8_SSCALED: // FORMAT_B8G8R8A8_SSCALED
      case symbol_kind::S_FORMAT_B8G8R8A8_UINT: // FORMAT_B8G8R8A8_UINT
      case symbol_kind::S_FORMAT_B8G8R8A8_SINT: // FORMAT_B8G8R8A8_SINT
      case symbol_kind::S_FORMAT_B8G8R8A8_SRGB: // FORMAT_B8G8R8A8_SRGB
      case symbol_kind::S_FORMAT_A8B8G8R8_UNORM_PACK32: // FORMAT_A8B8G8R8_UNORM_PACK32
      case symbol_kind::S_FORMAT_A8B8G8R8_SNORM_PACK32: // FORMAT_A8B8G8R8_SNORM_PACK32
      case symbol_kind::S_FORMAT_A8B8G8R8_USCALED_PACK32: // FORMAT_A8B8G8R8_USCALED_PACK32
      case symbol_kind::S_FORMAT_A8B8G8R8_SSCALED_PACK32: // FORMAT_A8B8G8R8_SSCALED_PACK32
      case symbol_kind::S_FORMAT_A8B8G8R8_UINT_PACK32: // FORMAT_A8B8G8R8_UINT_PACK32
      case symbol_kind::S_FORMAT_A8B8G8R8_SINT_PACK32: // FORMAT_A8B8G8R8_SINT_PACK32
      case symbol_kind::S_FORMAT_A8B8G8R8_SRGB_PACK32: // FORMAT_A8B8G8R8_SRGB_PACK32
      case symbol_kind::S_FORMAT_A2R10G10B10_UNORM_PACK32: // FORMAT_A2R10G10B10_UNORM_PACK32
      case symbol_kind::S_FORMAT_A2R10G10B10_SNORM_PACK32: // FORMAT_A2R10G10B10_SNORM_PACK32
      case symbol_kind::S_FORMAT_A2R10G10B10_USCALED_PACK32: // FORMAT_A2R10G10B10_USCALED_PACK32
      case symbol_kind::S_FORMAT_A2R10G10B10_SSCALED_PACK32: // FORMAT_A2R10G10B10_SSCALED_PACK32
      case symbol_kind::S_FORMAT_A2R10G10B10_UINT_PACK32: // FORMAT_A2R10G10B10_UINT_PACK32
      case symbol_kind::S_FORMAT_A2R10G10B10_SINT_PACK32: // FORMAT_A2R10G10B10_SINT_PACK32
      case symbol_kind::S_FORMAT_A2B10G10R10_UNORM_PACK32: // FORMAT_A2B10G10R10_UNORM_PACK32
      case symbol_kind::S_FORMAT_A2B10G10R10_SNORM_PACK32: // FORMAT_A2B10G10R10_SNORM_PACK32
      case symbol_kind::S_FORMAT_A2B10G10R10_USCALED_PACK32: // FORMAT_A2B10G10R10_USCALED_PACK32
      case symbol_kind::S_FORMAT_A2B10G10R10_SSCALED_PACK32: // FORMAT_A2B10G10R10_SSCALED_PACK32
      case symbol_kind::S_FORMAT_A2B10G10R10_UINT_PACK32: // FORMAT_A2B10G10R10_UINT_PACK32
      case symbol_kind::S_FORMAT_A2B10G10R10_SINT_PACK32: // FORMAT_A2B10G10R10_SINT_PACK32
      case symbol_kind::S_FORMAT_R16_UNORM: // FORMAT_R16_UNORM
      case symbol_kind::S_FORMAT_R16_SNORM: // FORMAT_R16_SNORM
      case symbol_kind::S_FORMAT_R16_USCALED: // FORMAT_R16_USCALED
      case symbol_kind::S_FORMAT_R16_SSCALED: // FORMAT_R16_SSCALED
      case symbol_kind::S_FORMAT_R16_UINT: // FORMAT_R16_UINT
      case symbol_kind::S_FORMAT_R16_SINT: // FORMAT_R16_SINT
      case symbol_kind::S_FORMAT_R16_SFLOAT: // FORMAT_R16_SFLOAT
      case symbol_kind::S_FORMAT_R16G16_UNORM: // FORMAT_R16G16_UNORM
      case symbol_kind::S_FORMAT_R16G16_SNORM: // FORMAT_R16G16_SNORM
      case symbol_kind::S_FORMAT_R16G16_USCALED: // FORMAT_R16G16_USCALED
      case symbol_kind::S_FORMAT_R16G16_SSCALED: // FORMAT_R16G16_SSCALED
      case symbol_kind::S_FORMAT_R16G16_UINT: // FORMAT_R16G16_UINT
      case symbol_kind::S_FORMAT_R16G16_SINT: // FORMAT_R16G16_SINT
      case symbol_kind::S_FORMAT_R16G16_SFLOAT: // FORMAT_R16G16_SFLOAT
      case symbol_kind::S_FORMAT_R16G16B16_UNORM: // FORMAT_R16G16B16_UNORM
      case symbol_kind::S_FORMAT_R16G16B16_SNORM: // FORMAT_R16G16B16_SNORM
      case symbol_kind::S_FORMAT_R16G16B16_USCALED: // FORMAT_R16G16B16_USCALED
      case symbol_kind::S_FORMAT_R16G16B16_SSCALED: // FORMAT_R16G16B16_SSCALED
      case symbol_kind::S_FORMAT_R16G16B16_UINT: // FORMAT_R16G16B16_UINT
      case symbol_kind::S_FORMAT_R16G16B16_SINT: // FORMAT_R16G16B16_SINT
      case symbol_kind::S_FORMAT_R16G16B16_SFLOAT: // FORMAT_R16G16B16_SFLOAT
      case symbol_kind::S_FORMAT_R16G16B16A16_UNORM: // FORMAT_R16G16B16A16_UNORM
      case symbol_kind::S_FORMAT_R16G16B16A16_SNORM: // FORMAT_R16G16B16A16_SNORM
      case symbol_kind::S_FORMAT_R16G16B16A16_USCALED: // FORMAT_R16G16B16A16_USCALED
      case symbol_kind::S_FORMAT_R16G16B16A16_SSCALED: // FORMAT_R16G16B16A16_SSCALED
      case symbol_kind::S_FORMAT_R16G16B16A16_UINT: // FORMAT_R16G16B16A16_UINT
      case symbol_kind::S_FORMAT_R16G16B16A16_SINT: // FORMAT_R16G16B16A16_SINT
      case symbol_kind::S_FORMAT_R16G16B16A16_SFLOAT: // FORMAT_R16G16B16A16_SFLOAT
      case symbol_kind::S_FORMAT_R32_UINT: // FORMAT_R32_UINT
      case symbol_kind::S_FORMAT_R32_SINT: // FORMAT_R32_SINT
      case symbol_kind::S_FORMAT_R32_SFLOAT: // FORMAT_R32_SFLOAT
      case symbol_kind::S_FORMAT_R32G32_UINT: // FORMAT_R32G32_UINT
      case symbol_kind::S_FORMAT_R32G32_SINT: // FORMAT_R32G32_SINT
      case symbol_kind::S_FORMAT_R32G32_SFLOAT: // FORMAT_R32G32_SFLOAT
      case symbol_kind::S_FORMAT_R32G32B32_UINT: // FORMAT_R32G32B32_UINT
      case symbol_kind::S_FORMAT_R32G32B32_SINT: // FORMAT_R32G32B32_SINT
      case symbol_kind::S_FORMAT_R32G32B32_SFLOAT: // FORMAT_R32G32B32_SFLOAT
      case symbol_kind::S_FORMAT_R32G32B32A32_UINT: // FORMAT_R32G32B32A32_UINT
      case symbol_kind::S_FORMAT_R32G32B32A32_SINT: // FORMAT_R32G32B32A32_SINT
      case symbol_kind::S_FORMAT_R32G32B32A32_SFLOAT: // FORMAT_R32G32B32A32_SFLOAT
      case symbol_kind::S_FORMAT_R64_UINT: // FORMAT_R64_UINT
      case symbol_kind::S_FORMAT_R64_SINT: // FORMAT_R64_SINT
      case symbol_kind::S_FORMAT_R64_SFLOAT: // FORMAT_R64_SFLOAT
      case symbol_kind::S_FORMAT_R64G64_UINT: // FORMAT_R64G64_UINT
      case symbol_kind::S_FORMAT_R64G64_SINT: // FORMAT_R64G64_SINT
      case symbol_kind::S_FORMAT_R64G64_SFLOAT: // FORMAT_R64G64_SFLOAT
      case symbol_kind::S_FORMAT_R64G64B64_UINT: // FORMAT_R64G64B64_UINT
      case symbol_kind::S_FORMAT_R64G64B64_SINT: // FORMAT_R64G64B64_SINT
      case symbol_kind::S_FORMAT_R64G64B64_SFLOAT: // FORMAT_R64G64B64_SFLOAT
      case symbol_kind::S_FORMAT_R64G64B64A64_UINT: // FORMAT_R64G64B64A64_UINT
      case symbol_kind::S_FORMAT_R64G64B64A64_SINT: // FORMAT_R64G64B64A64_SINT
      case symbol_kind::S_FORMAT_R64G64B64A64_SFLOAT: // FORMAT_R64G64B64A64_SFLOAT
      case symbol_kind::S_FORMAT_B10G11R11_UFLOAT_PACK32: // FORMAT_B10G11R11_UFLOAT_PACK32
      case symbol_kind::S_FORMAT_E5B9G9R9_UFLOAT_PACK32: // FORMAT_E5B9G9R9_UFLOAT_PACK32
      case symbol_kind::S_FORMAT_D16_UNORM: // FORMAT_D16_UNORM
      case symbol_kind::S_FORMAT_X8_D24_UNORM_PACK32: // FORMAT_X8_D24_UNORM_PACK32
      case symbol_kind::S_FORMAT_D32_SFLOAT: // FORMAT_D32_SFLOAT
      case symbol_kind::S_FORMAT_S8_UINT: // FORMAT_S8_UINT
      case symbol_kind::S_FORMAT_D16_UNORM_S8_UINT: // FORMAT_D16_UNORM_S8_UINT
      case symbol_kind::S_FORMAT_D24_UNORM_S8_UINT: // FORMAT_D24_UNORM_S8_UINT
      case symbol_kind::S_FORMAT_D32_SFLOAT_S8_UINT: // FORMAT_D32_SFLOAT_S8_UINT
      case symbol_kind::S_FORMAT_BC1_RGB_UNORM_BLOCK: // FORMAT_BC1_RGB_UNORM_BLOCK
      case symbol_kind::S_FORMAT_BC1_RGB_SRGB_BLOCK: // FORMAT_BC1_RGB_SRGB_BLOCK
      case symbol_kind::S_FORMAT_BC1_RGBA_UNORM_BLOCK: // FORMAT_BC1_RGBA_UNORM_BLOCK
      case symbol_kind::S_FORMAT_BC1_RGBA_SRGB_BLOCK: // FORMAT_BC1_RGBA_SRGB_BLOCK
      case symbol_kind::S_FORMAT_BC2_UNORM_BLOCK: // FORMAT_BC2_UNORM_BLOCK
      case symbol_kind::S_FORMAT_BC2_SRGB_BLOCK: // FORMAT_BC2_SRGB_BLOCK
      case symbol_kind::S_FORMAT_BC3_UNORM_BLOCK: // FORMAT_BC3_UNORM_BLOCK
      case symbol_kind::S_FORMAT_BC3_SRGB_BLOCK: // FORMAT_BC3_SRGB_BLOCK
      case symbol_kind::S_FORMAT_BC4_UNORM_BLOCK: // FORMAT_BC4_UNORM_BLOCK
      case symbol_kind::S_FORMAT_BC4_SNORM_BLOCK: // FORMAT_BC4_SNORM_BLOCK
      case symbol_kind::S_FORMAT_BC5_UNORM_BLOCK: // FORMAT_BC5_UNORM_BLOCK
      case symbol_kind::S_FORMAT_BC5_SNORM_BLOCK: // FORMAT_BC5_SNORM_BLOCK
      case symbol_kind::S_FORMAT_BC6H_UFLOAT_BLOCK: // FORMAT_BC6H_UFLOAT_BLOCK
      case symbol_kind::S_FORMAT_BC6H_SFLOAT_BLOCK: // FORMAT_BC6H_SFLOAT_BLOCK
      case symbol_kind::S_FORMAT_BC7_UNORM_BLOCK: // FORMAT_BC7_UNORM_BLOCK
      case symbol_kind::S_FORMAT_BC7_SRGB_BLOCK: // FORMAT_BC7_SRGB_BLOCK
      case symbol_kind::S_FORMAT_ETC2_R8G8B8_UNORM_BLOCK: // FORMAT_ETC2_R8G8B8_UNORM_BLOCK
      case symbol_kind::S_FORMAT_ETC2_R8G8B8_SRGB_BLOCK: // FORMAT_ETC2_R8G8B8_SRGB_BLOCK
      case symbol_kind::S_FORMAT_ETC2_R8G8B8A1_UNORM_BLOCK: // FORMAT_ETC2_R8G8B8A1_UNORM_BLOCK
      case symbol_kind::S_FORMAT_ETC2_R8G8B8A1_SRGB_BLOCK: // FORMAT_ETC2_R8G8B8A1_SRGB_BLOCK
      case symbol_kind::S_FORMAT_ETC2_R8G8B8A8_UNORM_BLOCK: // FORMAT_ETC2_R8G8B8A8_UNORM_BLOCK
      case symbol_kind::S_FORMAT_ETC2_R8G8B8A8_SRGB_BLOCK: // FORMAT_ETC2_R8G8B8A8_SRGB_BLOCK
      case symbol_kind::S_FORMAT_EAC_R11_UNORM_BLOCK: // FORMAT_EAC_R11_UNORM_BLOCK
      case symbol_kind::S_FORMAT_EAC_R11_SNORM_BLOCK: // FORMAT_EAC_R11_SNORM_BLOCK
      case symbol_kind::S_FORMAT_EAC_R11G11_UNORM_BLOCK: // FORMAT_EAC_R11G11_UNORM_BLOCK
      case symbol_kind::S_FORMAT_EAC_R11G11_SNORM_BLOCK: // FORMAT_EAC_R11G11_SNORM_BLOCK
      case symbol_kind::S_FORMAT_ASTC_4x4_UNORM_BLOCK: // FORMAT_ASTC_4x4_UNORM_BLOCK
      case symbol_kind::S_FORMAT_ASTC_4x4_SRGB_BLOCK: // FORMAT_ASTC_4x4_SRGB_BLOCK
      case symbol_kind::S_FORMAT_ASTC_5x4_UNORM_BLOCK: // FORMAT_ASTC_5x4_UNORM_BLOCK
      case symbol_kind::S_FORMAT_ASTC_5x4_SRGB_BLOCK: // FORMAT_ASTC_5x4_SRGB_BLOCK
      case symbol_kind::S_FORMAT_ASTC_5x5_UNORM_BLOCK: // FORMAT_ASTC_5x5_UNORM_BLOCK
      case symbol_kind::S_FORMAT_ASTC_5x5_SRGB_BLOCK: // FORMAT_ASTC_5x5_SRGB_BLOCK
      case symbol_kind::S_FORMAT_ASTC_6x5_UNORM_BLOCK: // FORMAT_ASTC_6x5_UNORM_BLOCK
      case symbol_kind::S_FORMAT_ASTC_6x5_SRGB_BLOCK: // FORMAT_ASTC_6x5_SRGB_BLOCK
      case symbol_kind::S_FORMAT_ASTC_6x6_UNORM_BLOCK: // FORMAT_ASTC_6x6_UNORM_BLOCK
      case symbol_kind::S_FORMAT_ASTC_6x6_SRGB_BLOCK: // FORMAT_ASTC_6x6_SRGB_BLOCK
      case symbol_kind::S_FORMAT_ASTC_8x5_UNORM_BLOCK: // FORMAT_ASTC_8x5_UNORM_BLOCK
      case symbol_kind::S_FORMAT_ASTC_8x5_SRGB_BLOCK: // FORMAT_ASTC_8x5_SRGB_BLOCK
      case symbol_kind::S_FORMAT_ASTC_8x6_UNORM_BLOCK: // FORMAT_ASTC_8x6_UNORM_BLOCK
      case symbol_kind::S_FORMAT_ASTC_8x6_SRGB_BLOCK: // FORMAT_ASTC_8x6_SRGB_BLOCK
      case symbol_kind::S_FORMAT_ASTC_8x8_UNORM_BLOCK: // FORMAT_ASTC_8x8_UNORM_BLOCK
      case symbol_kind::S_FORMAT_ASTC_8x8_SRGB_BLOCK: // FORMAT_ASTC_8x8_SRGB_BLOCK
      case symbol_kind::S_FORMAT_ASTC_10x5_UNORM_BLOCK: // FORMAT_ASTC_10x5_UNORM_BLOCK
      case symbol_kind::S_FORMAT_ASTC_10x5_SRGB_BLOCK: // FORMAT_ASTC_10x5_SRGB_BLOCK
      case symbol_kind::S_FORMAT_ASTC_10x6_UNORM_BLOCK: // FORMAT_ASTC_10x6_UNORM_BLOCK
      case symbol_kind::S_FORMAT_ASTC_10x6_SRGB_BLOCK: // FORMAT_ASTC_10x6_SRGB_BLOCK
      case symbol_kind::S_FORMAT_ASTC_10x8_UNORM_BLOCK: // FORMAT_ASTC_10x8_UNORM_BLOCK
      case symbol_kind::S_FORMAT_ASTC_10x8_SRGB_BLOCK: // FORMAT_ASTC_10x8_SRGB_BLOCK
      case symbol_kind::S_FORMAT_ASTC_10x10_UNORM_BLOCK: // FORMAT_ASTC_10x10_UNORM_BLOCK
      case symbol_kind::S_FORMAT_ASTC_10x10_SRGB_BLOCK: // FORMAT_ASTC_10x10_SRGB_BLOCK
      case symbol_kind::S_FORMAT_ASTC_12x10_UNORM_BLOCK: // FORMAT_ASTC_12x10_UNORM_BLOCK
      case symbol_kind::S_FORMAT_ASTC_12x10_SRGB_BLOCK: // FORMAT_ASTC_12x10_SRGB_BLOCK
      case symbol_kind::S_FORMAT_ASTC_12x12_UNORM_BLOCK: // FORMAT_ASTC_12x12_UNORM_BLOCK
      case symbol_kind::S_FORMAT_ASTC_12x12_SRGB_BLOCK: // FORMAT_ASTC_12x12_SRGB_BLOCK
      case symbol_kind::S_format: // format
        value.copy< VkFormat > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_TRUE: // TRUE
      case symbol_kind::S_FALSE: // FALSE
      case symbol_kind::S_BOOL: // BOOL
        value.copy< bool > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_FLOAT: // FLOAT
        value.copy< float > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_INT: // INT
        value.copy< int > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_IDENTIFIER: // IDENTIFIER
      case symbol_kind::S_STR: // STR
      case symbol_kind::S_CODEBODY: // CODEBODY
      case symbol_kind::S_VARNAME: // VARNAME
        value.copy< std::string > (YY_MOVE (that.value));
        break;

      default:
        break;
    }

  }



  template <typename Base>
  parser::symbol_kind_type
  parser::basic_symbol<Base>::type_get () const YY_NOEXCEPT
  {
    return this->kind ();
  }

  template <typename Base>
  bool
  parser::basic_symbol<Base>::empty () const YY_NOEXCEPT
  {
    return this->kind () == symbol_kind::S_YYEMPTY;
  }

  template <typename Base>
  void
  parser::basic_symbol<Base>::move (basic_symbol& s)
  {
    super_type::move (s);
    switch (this->kind ())
    {
      case symbol_kind::S_FORMAT_UNDEFINED: // FORMAT_UNDEFINED
      case symbol_kind::S_FORMAT_R4G4_UNORM_PACK8: // FORMAT_R4G4_UNORM_PACK8
      case symbol_kind::S_FORMAT_R4G4B4A4_UNORM_PACK16: // FORMAT_R4G4B4A4_UNORM_PACK16
      case symbol_kind::S_FORMAT_B4G4R4A4_UNORM_PACK16: // FORMAT_B4G4R4A4_UNORM_PACK16
      case symbol_kind::S_FORMAT_R5G6B5_UNORM_PACK16: // FORMAT_R5G6B5_UNORM_PACK16
      case symbol_kind::S_FORMAT_B5G6R5_UNORM_PACK16: // FORMAT_B5G6R5_UNORM_PACK16
      case symbol_kind::S_FORMAT_R5G5B5A1_UNORM_PACK16: // FORMAT_R5G5B5A1_UNORM_PACK16
      case symbol_kind::S_FORMAT_B5G5R5A1_UNORM_PACK16: // FORMAT_B5G5R5A1_UNORM_PACK16
      case symbol_kind::S_FORMAT_A1R5G5B5_UNORM_PACK16: // FORMAT_A1R5G5B5_UNORM_PACK16
      case symbol_kind::S_FORMAT_R8_UNORM: // FORMAT_R8_UNORM
      case symbol_kind::S_FORMAT_R8_SNORM: // FORMAT_R8_SNORM
      case symbol_kind::S_FORMAT_R8_USCALED: // FORMAT_R8_USCALED
      case symbol_kind::S_FORMAT_R8_SSCALED: // FORMAT_R8_SSCALED
      case symbol_kind::S_FORMAT_R8_UINT: // FORMAT_R8_UINT
      case symbol_kind::S_FORMAT_R8_SINT: // FORMAT_R8_SINT
      case symbol_kind::S_FORMAT_R8_SRGB: // FORMAT_R8_SRGB
      case symbol_kind::S_FORMAT_R8G8_UNORM: // FORMAT_R8G8_UNORM
      case symbol_kind::S_FORMAT_R8G8_SNORM: // FORMAT_R8G8_SNORM
      case symbol_kind::S_FORMAT_R8G8_USCALED: // FORMAT_R8G8_USCALED
      case symbol_kind::S_FORMAT_R8G8_SSCALED: // FORMAT_R8G8_SSCALED
      case symbol_kind::S_FORMAT_R8G8_UINT: // FORMAT_R8G8_UINT
      case symbol_kind::S_FORMAT_R8G8_SINT: // FORMAT_R8G8_SINT
      case symbol_kind::S_FORMAT_R8G8_SRGB: // FORMAT_R8G8_SRGB
      case symbol_kind::S_FORMAT_R8G8B8_UNORM: // FORMAT_R8G8B8_UNORM
      case symbol_kind::S_FORMAT_R8G8B8_SNORM: // FORMAT_R8G8B8_SNORM
      case symbol_kind::S_FORMAT_R8G8B8_USCALED: // FORMAT_R8G8B8_USCALED
      case symbol_kind::S_FORMAT_R8G8B8_SSCALED: // FORMAT_R8G8B8_SSCALED
      case symbol_kind::S_FORMAT_R8G8B8_UINT: // FORMAT_R8G8B8_UINT
      case symbol_kind::S_FORMAT_R8G8B8_SINT: // FORMAT_R8G8B8_SINT
      case symbol_kind::S_FORMAT_R8G8B8_SRGB: // FORMAT_R8G8B8_SRGB
      case symbol_kind::S_FORMAT_B8G8R8_UNORM: // FORMAT_B8G8R8_UNORM
      case symbol_kind::S_FORMAT_B8G8R8_SNORM: // FORMAT_B8G8R8_SNORM
      case symbol_kind::S_FORMAT_B8G8R8_USCALED: // FORMAT_B8G8R8_USCALED
      case symbol_kind::S_FORMAT_B8G8R8_SSCALED: // FORMAT_B8G8R8_SSCALED
      case symbol_kind::S_FORMAT_B8G8R8_UINT: // FORMAT_B8G8R8_UINT
      case symbol_kind::S_FORMAT_B8G8R8_SINT: // FORMAT_B8G8R8_SINT
      case symbol_kind::S_FORMAT_B8G8R8_SRGB: // FORMAT_B8G8R8_SRGB
      case symbol_kind::S_FORMAT_R8G8B8A8_UNORM: // FORMAT_R8G8B8A8_UNORM
      case symbol_kind::S_FORMAT_R8G8B8A8_SNORM: // FORMAT_R8G8B8A8_SNORM
      case symbol_kind::S_FORMAT_R8G8B8A8_USCALED: // FORMAT_R8G8B8A8_USCALED
      case symbol_kind::S_FORMAT_R8G8B8A8_SSCALED: // FORMAT_R8G8B8A8_SSCALED
      case symbol_kind::S_FORMAT_R8G8B8A8_UINT: // FORMAT_R8G8B8A8_UINT
      case symbol_kind::S_FORMAT_R8G8B8A8_SINT: // FORMAT_R8G8B8A8_SINT
      case symbol_kind::S_FORMAT_R8G8B8A8_SRGB: // FORMAT_R8G8B8A8_SRGB
      case symbol_kind::S_FORMAT_B8G8R8A8_UNORM: // FORMAT_B8G8R8A8_UNORM
      case symbol_kind::S_FORMAT_B8G8R8A8_SNORM: // FORMAT_B8G8R8A8_SNORM
      case symbol_kind::S_FORMAT_B8G8R8A8_USCALED: // FORMAT_B8G8R8A8_USCALED
      case symbol_kind::S_FORMAT_B8G8R8A8_SSCALED: // FORMAT_B8G8R8A8_SSCALED
      case symbol_kind::S_FORMAT_B8G8R8A8_UINT: // FORMAT_B8G8R8A8_UINT
      case symbol_kind::S_FORMAT_B8G8R8A8_SINT: // FORMAT_B8G8R8A8_SINT
      case symbol_kind::S_FORMAT_B8G8R8A8_SRGB: // FORMAT_B8G8R8A8_SRGB
      case symbol_kind::S_FORMAT_A8B8G8R8_UNORM_PACK32: // FORMAT_A8B8G8R8_UNORM_PACK32
      case symbol_kind::S_FORMAT_A8B8G8R8_SNORM_PACK32: // FORMAT_A8B8G8R8_SNORM_PACK32
      case symbol_kind::S_FORMAT_A8B8G8R8_USCALED_PACK32: // FORMAT_A8B8G8R8_USCALED_PACK32
      case symbol_kind::S_FORMAT_A8B8G8R8_SSCALED_PACK32: // FORMAT_A8B8G8R8_SSCALED_PACK32
      case symbol_kind::S_FORMAT_A8B8G8R8_UINT_PACK32: // FORMAT_A8B8G8R8_UINT_PACK32
      case symbol_kind::S_FORMAT_A8B8G8R8_SINT_PACK32: // FORMAT_A8B8G8R8_SINT_PACK32
      case symbol_kind::S_FORMAT_A8B8G8R8_SRGB_PACK32: // FORMAT_A8B8G8R8_SRGB_PACK32
      case symbol_kind::S_FORMAT_A2R10G10B10_UNORM_PACK32: // FORMAT_A2R10G10B10_UNORM_PACK32
      case symbol_kind::S_FORMAT_A2R10G10B10_SNORM_PACK32: // FORMAT_A2R10G10B10_SNORM_PACK32
      case symbol_kind::S_FORMAT_A2R10G10B10_USCALED_PACK32: // FORMAT_A2R10G10B10_USCALED_PACK32
      case symbol_kind::S_FORMAT_A2R10G10B10_SSCALED_PACK32: // FORMAT_A2R10G10B10_SSCALED_PACK32
      case symbol_kind::S_FORMAT_A2R10G10B10_UINT_PACK32: // FORMAT_A2R10G10B10_UINT_PACK32
      case symbol_kind::S_FORMAT_A2R10G10B10_SINT_PACK32: // FORMAT_A2R10G10B10_SINT_PACK32
      case symbol_kind::S_FORMAT_A2B10G10R10_UNORM_PACK32: // FORMAT_A2B10G10R10_UNORM_PACK32
      case symbol_kind::S_FORMAT_A2B10G10R10_SNORM_PACK32: // FORMAT_A2B10G10R10_SNORM_PACK32
      case symbol_kind::S_FORMAT_A2B10G10R10_USCALED_PACK32: // FORMAT_A2B10G10R10_USCALED_PACK32
      case symbol_kind::S_FORMAT_A2B10G10R10_SSCALED_PACK32: // FORMAT_A2B10G10R10_SSCALED_PACK32
      case symbol_kind::S_FORMAT_A2B10G10R10_UINT_PACK32: // FORMAT_A2B10G10R10_UINT_PACK32
      case symbol_kind::S_FORMAT_A2B10G10R10_SINT_PACK32: // FORMAT_A2B10G10R10_SINT_PACK32
      case symbol_kind::S_FORMAT_R16_UNORM: // FORMAT_R16_UNORM
      case symbol_kind::S_FORMAT_R16_SNORM: // FORMAT_R16_SNORM
      case symbol_kind::S_FORMAT_R16_USCALED: // FORMAT_R16_USCALED
      case symbol_kind::S_FORMAT_R16_SSCALED: // FORMAT_R16_SSCALED
      case symbol_kind::S_FORMAT_R16_UINT: // FORMAT_R16_UINT
      case symbol_kind::S_FORMAT_R16_SINT: // FORMAT_R16_SINT
      case symbol_kind::S_FORMAT_R16_SFLOAT: // FORMAT_R16_SFLOAT
      case symbol_kind::S_FORMAT_R16G16_UNORM: // FORMAT_R16G16_UNORM
      case symbol_kind::S_FORMAT_R16G16_SNORM: // FORMAT_R16G16_SNORM
      case symbol_kind::S_FORMAT_R16G16_USCALED: // FORMAT_R16G16_USCALED
      case symbol_kind::S_FORMAT_R16G16_SSCALED: // FORMAT_R16G16_SSCALED
      case symbol_kind::S_FORMAT_R16G16_UINT: // FORMAT_R16G16_UINT
      case symbol_kind::S_FORMAT_R16G16_SINT: // FORMAT_R16G16_SINT
      case symbol_kind::S_FORMAT_R16G16_SFLOAT: // FORMAT_R16G16_SFLOAT
      case symbol_kind::S_FORMAT_R16G16B16_UNORM: // FORMAT_R16G16B16_UNORM
      case symbol_kind::S_FORMAT_R16G16B16_SNORM: // FORMAT_R16G16B16_SNORM
      case symbol_kind::S_FORMAT_R16G16B16_USCALED: // FORMAT_R16G16B16_USCALED
      case symbol_kind::S_FORMAT_R16G16B16_SSCALED: // FORMAT_R16G16B16_SSCALED
      case symbol_kind::S_FORMAT_R16G16B16_UINT: // FORMAT_R16G16B16_UINT
      case symbol_kind::S_FORMAT_R16G16B16_SINT: // FORMAT_R16G16B16_SINT
      case symbol_kind::S_FORMAT_R16G16B16_SFLOAT: // FORMAT_R16G16B16_SFLOAT
      case symbol_kind::S_FORMAT_R16G16B16A16_UNORM: // FORMAT_R16G16B16A16_UNORM
      case symbol_kind::S_FORMAT_R16G16B16A16_SNORM: // FORMAT_R16G16B16A16_SNORM
      case symbol_kind::S_FORMAT_R16G16B16A16_USCALED: // FORMAT_R16G16B16A16_USCALED
      case symbol_kind::S_FORMAT_R16G16B16A16_SSCALED: // FORMAT_R16G16B16A16_SSCALED
      case symbol_kind::S_FORMAT_R16G16B16A16_UINT: // FORMAT_R16G16B16A16_UINT
      case symbol_kind::S_FORMAT_R16G16B16A16_SINT: // FORMAT_R16G16B16A16_SINT
      case symbol_kind::S_FORMAT_R16G16B16A16_SFLOAT: // FORMAT_R16G16B16A16_SFLOAT
      case symbol_kind::S_FORMAT_R32_UINT: // FORMAT_R32_UINT
      case symbol_kind::S_FORMAT_R32_SINT: // FORMAT_R32_SINT
      case symbol_kind::S_FORMAT_R32_SFLOAT: // FORMAT_R32_SFLOAT
      case symbol_kind::S_FORMAT_R32G32_UINT: // FORMAT_R32G32_UINT
      case symbol_kind::S_FORMAT_R32G32_SINT: // FORMAT_R32G32_SINT
      case symbol_kind::S_FORMAT_R32G32_SFLOAT: // FORMAT_R32G32_SFLOAT
      case symbol_kind::S_FORMAT_R32G32B32_UINT: // FORMAT_R32G32B32_UINT
      case symbol_kind::S_FORMAT_R32G32B32_SINT: // FORMAT_R32G32B32_SINT
      case symbol_kind::S_FORMAT_R32G32B32_SFLOAT: // FORMAT_R32G32B32_SFLOAT
      case symbol_kind::S_FORMAT_R32G32B32A32_UINT: // FORMAT_R32G32B32A32_UINT
      case symbol_kind::S_FORMAT_R32G32B32A32_SINT: // FORMAT_R32G32B32A32_SINT
      case symbol_kind::S_FORMAT_R32G32B32A32_SFLOAT: // FORMAT_R32G32B32A32_SFLOAT
      case symbol_kind::S_FORMAT_R64_UINT: // FORMAT_R64_UINT
      case symbol_kind::S_FORMAT_R64_SINT: // FORMAT_R64_SINT
      case symbol_kind::S_FORMAT_R64_SFLOAT: // FORMAT_R64_SFLOAT
      case symbol_kind::S_FORMAT_R64G64_UINT: // FORMAT_R64G64_UINT
      case symbol_kind::S_FORMAT_R64G64_SINT: // FORMAT_R64G64_SINT
      case symbol_kind::S_FORMAT_R64G64_SFLOAT: // FORMAT_R64G64_SFLOAT
      case symbol_kind::S_FORMAT_R64G64B64_UINT: // FORMAT_R64G64B64_UINT
      case symbol_kind::S_FORMAT_R64G64B64_SINT: // FORMAT_R64G64B64_SINT
      case symbol_kind::S_FORMAT_R64G64B64_SFLOAT: // FORMAT_R64G64B64_SFLOAT
      case symbol_kind::S_FORMAT_R64G64B64A64_UINT: // FORMAT_R64G64B64A64_UINT
      case symbol_kind::S_FORMAT_R64G64B64A64_SINT: // FORMAT_R64G64B64A64_SINT
      case symbol_kind::S_FORMAT_R64G64B64A64_SFLOAT: // FORMAT_R64G64B64A64_SFLOAT
      case symbol_kind::S_FORMAT_B10G11R11_UFLOAT_PACK32: // FORMAT_B10G11R11_UFLOAT_PACK32
      case symbol_kind::S_FORMAT_E5B9G9R9_UFLOAT_PACK32: // FORMAT_E5B9G9R9_UFLOAT_PACK32
      case symbol_kind::S_FORMAT_D16_UNORM: // FORMAT_D16_UNORM
      case symbol_kind::S_FORMAT_X8_D24_UNORM_PACK32: // FORMAT_X8_D24_UNORM_PACK32
      case symbol_kind::S_FORMAT_D32_SFLOAT: // FORMAT_D32_SFLOAT
      case symbol_kind::S_FORMAT_S8_UINT: // FORMAT_S8_UINT
      case symbol_kind::S_FORMAT_D16_UNORM_S8_UINT: // FORMAT_D16_UNORM_S8_UINT
      case symbol_kind::S_FORMAT_D24_UNORM_S8_UINT: // FORMAT_D24_UNORM_S8_UINT
      case symbol_kind::S_FORMAT_D32_SFLOAT_S8_UINT: // FORMAT_D32_SFLOAT_S8_UINT
      case symbol_kind::S_FORMAT_BC1_RGB_UNORM_BLOCK: // FORMAT_BC1_RGB_UNORM_BLOCK
      case symbol_kind::S_FORMAT_BC1_RGB_SRGB_BLOCK: // FORMAT_BC1_RGB_SRGB_BLOCK
      case symbol_kind::S_FORMAT_BC1_RGBA_UNORM_BLOCK: // FORMAT_BC1_RGBA_UNORM_BLOCK
      case symbol_kind::S_FORMAT_BC1_RGBA_SRGB_BLOCK: // FORMAT_BC1_RGBA_SRGB_BLOCK
      case symbol_kind::S_FORMAT_BC2_UNORM_BLOCK: // FORMAT_BC2_UNORM_BLOCK
      case symbol_kind::S_FORMAT_BC2_SRGB_BLOCK: // FORMAT_BC2_SRGB_BLOCK
      case symbol_kind::S_FORMAT_BC3_UNORM_BLOCK: // FORMAT_BC3_UNORM_BLOCK
      case symbol_kind::S_FORMAT_BC3_SRGB_BLOCK: // FORMAT_BC3_SRGB_BLOCK
      case symbol_kind::S_FORMAT_BC4_UNORM_BLOCK: // FORMAT_BC4_UNORM_BLOCK
      case symbol_kind::S_FORMAT_BC4_SNORM_BLOCK: // FORMAT_BC4_SNORM_BLOCK
      case symbol_kind::S_FORMAT_BC5_UNORM_BLOCK: // FORMAT_BC5_UNORM_BLOCK
      case symbol_kind::S_FORMAT_BC5_SNORM_BLOCK: // FORMAT_BC5_SNORM_BLOCK
      case symbol_kind::S_FORMAT_BC6H_UFLOAT_BLOCK: // FORMAT_BC6H_UFLOAT_BLOCK
      case symbol_kind::S_FORMAT_BC6H_SFLOAT_BLOCK: // FORMAT_BC6H_SFLOAT_BLOCK
      case symbol_kind::S_FORMAT_BC7_UNORM_BLOCK: // FORMAT_BC7_UNORM_BLOCK
      case symbol_kind::S_FORMAT_BC7_SRGB_BLOCK: // FORMAT_BC7_SRGB_BLOCK
      case symbol_kind::S_FORMAT_ETC2_R8G8B8_UNORM_BLOCK: // FORMAT_ETC2_R8G8B8_UNORM_BLOCK
      case symbol_kind::S_FORMAT_ETC2_R8G8B8_SRGB_BLOCK: // FORMAT_ETC2_R8G8B8_SRGB_BLOCK
      case symbol_kind::S_FORMAT_ETC2_R8G8B8A1_UNORM_BLOCK: // FORMAT_ETC2_R8G8B8A1_UNORM_BLOCK
      case symbol_kind::S_FORMAT_ETC2_R8G8B8A1_SRGB_BLOCK: // FORMAT_ETC2_R8G8B8A1_SRGB_BLOCK
      case symbol_kind::S_FORMAT_ETC2_R8G8B8A8_UNORM_BLOCK: // FORMAT_ETC2_R8G8B8A8_UNORM_BLOCK
      case symbol_kind::S_FORMAT_ETC2_R8G8B8A8_SRGB_BLOCK: // FORMAT_ETC2_R8G8B8A8_SRGB_BLOCK
      case symbol_kind::S_FORMAT_EAC_R11_UNORM_BLOCK: // FORMAT_EAC_R11_UNORM_BLOCK
      case symbol_kind::S_FORMAT_EAC_R11_SNORM_BLOCK: // FORMAT_EAC_R11_SNORM_BLOCK
      case symbol_kind::S_FORMAT_EAC_R11G11_UNORM_BLOCK: // FORMAT_EAC_R11G11_UNORM_BLOCK
      case symbol_kind::S_FORMAT_EAC_R11G11_SNORM_BLOCK: // FORMAT_EAC_R11G11_SNORM_BLOCK
      case symbol_kind::S_FORMAT_ASTC_4x4_UNORM_BLOCK: // FORMAT_ASTC_4x4_UNORM_BLOCK
      case symbol_kind::S_FORMAT_ASTC_4x4_SRGB_BLOCK: // FORMAT_ASTC_4x4_SRGB_BLOCK
      case symbol_kind::S_FORMAT_ASTC_5x4_UNORM_BLOCK: // FORMAT_ASTC_5x4_UNORM_BLOCK
      case symbol_kind::S_FORMAT_ASTC_5x4_SRGB_BLOCK: // FORMAT_ASTC_5x4_SRGB_BLOCK
      case symbol_kind::S_FORMAT_ASTC_5x5_UNORM_BLOCK: // FORMAT_ASTC_5x5_UNORM_BLOCK
      case symbol_kind::S_FORMAT_ASTC_5x5_SRGB_BLOCK: // FORMAT_ASTC_5x5_SRGB_BLOCK
      case symbol_kind::S_FORMAT_ASTC_6x5_UNORM_BLOCK: // FORMAT_ASTC_6x5_UNORM_BLOCK
      case symbol_kind::S_FORMAT_ASTC_6x5_SRGB_BLOCK: // FORMAT_ASTC_6x5_SRGB_BLOCK
      case symbol_kind::S_FORMAT_ASTC_6x6_UNORM_BLOCK: // FORMAT_ASTC_6x6_UNORM_BLOCK
      case symbol_kind::S_FORMAT_ASTC_6x6_SRGB_BLOCK: // FORMAT_ASTC_6x6_SRGB_BLOCK
      case symbol_kind::S_FORMAT_ASTC_8x5_UNORM_BLOCK: // FORMAT_ASTC_8x5_UNORM_BLOCK
      case symbol_kind::S_FORMAT_ASTC_8x5_SRGB_BLOCK: // FORMAT_ASTC_8x5_SRGB_BLOCK
      case symbol_kind::S_FORMAT_ASTC_8x6_UNORM_BLOCK: // FORMAT_ASTC_8x6_UNORM_BLOCK
      case symbol_kind::S_FORMAT_ASTC_8x6_SRGB_BLOCK: // FORMAT_ASTC_8x6_SRGB_BLOCK
      case symbol_kind::S_FORMAT_ASTC_8x8_UNORM_BLOCK: // FORMAT_ASTC_8x8_UNORM_BLOCK
      case symbol_kind::S_FORMAT_ASTC_8x8_SRGB_BLOCK: // FORMAT_ASTC_8x8_SRGB_BLOCK
      case symbol_kind::S_FORMAT_ASTC_10x5_UNORM_BLOCK: // FORMAT_ASTC_10x5_UNORM_BLOCK
      case symbol_kind::S_FORMAT_ASTC_10x5_SRGB_BLOCK: // FORMAT_ASTC_10x5_SRGB_BLOCK
      case symbol_kind::S_FORMAT_ASTC_10x6_UNORM_BLOCK: // FORMAT_ASTC_10x6_UNORM_BLOCK
      case symbol_kind::S_FORMAT_ASTC_10x6_SRGB_BLOCK: // FORMAT_ASTC_10x6_SRGB_BLOCK
      case symbol_kind::S_FORMAT_ASTC_10x8_UNORM_BLOCK: // FORMAT_ASTC_10x8_UNORM_BLOCK
      case symbol_kind::S_FORMAT_ASTC_10x8_SRGB_BLOCK: // FORMAT_ASTC_10x8_SRGB_BLOCK
      case symbol_kind::S_FORMAT_ASTC_10x10_UNORM_BLOCK: // FORMAT_ASTC_10x10_UNORM_BLOCK
      case symbol_kind::S_FORMAT_ASTC_10x10_SRGB_BLOCK: // FORMAT_ASTC_10x10_SRGB_BLOCK
      case symbol_kind::S_FORMAT_ASTC_12x10_UNORM_BLOCK: // FORMAT_ASTC_12x10_UNORM_BLOCK
      case symbol_kind::S_FORMAT_ASTC_12x10_SRGB_BLOCK: // FORMAT_ASTC_12x10_SRGB_BLOCK
      case symbol_kind::S_FORMAT_ASTC_12x12_UNORM_BLOCK: // FORMAT_ASTC_12x12_UNORM_BLOCK
      case symbol_kind::S_FORMAT_ASTC_12x12_SRGB_BLOCK: // FORMAT_ASTC_12x12_SRGB_BLOCK
      case symbol_kind::S_format: // format
        value.move< VkFormat > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_TRUE: // TRUE
      case symbol_kind::S_FALSE: // FALSE
      case symbol_kind::S_BOOL: // BOOL
        value.move< bool > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_FLOAT: // FLOAT
        value.move< float > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_INT: // INT
        value.move< int > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_IDENTIFIER: // IDENTIFIER
      case symbol_kind::S_STR: // STR
      case symbol_kind::S_CODEBODY: // CODEBODY
      case symbol_kind::S_VARNAME: // VARNAME
        value.move< std::string > (YY_MOVE (s.value));
        break;

      default:
        break;
    }

    location = YY_MOVE (s.location);
  }

  // by_kind.
  inline
  parser::by_kind::by_kind ()
    : kind_ (symbol_kind::S_YYEMPTY)
  {}

#if 201103L <= YY_CPLUSPLUS
  inline
  parser::by_kind::by_kind (by_kind&& that)
    : kind_ (that.kind_)
  {
    that.clear ();
  }
#endif

  inline
  parser::by_kind::by_kind (const by_kind& that)
    : kind_ (that.kind_)
  {}

  inline
  parser::by_kind::by_kind (token_kind_type t)
    : kind_ (yytranslate_ (t))
  {}

  inline
  void
  parser::by_kind::clear ()
  {
    kind_ = symbol_kind::S_YYEMPTY;
  }

  inline
  void
  parser::by_kind::move (by_kind& that)
  {
    kind_ = that.kind_;
    that.clear ();
  }

  inline
  parser::symbol_kind_type
  parser::by_kind::kind () const YY_NOEXCEPT
  {
    return kind_;
  }

  inline
  parser::symbol_kind_type
  parser::by_kind::type_get () const YY_NOEXCEPT
  {
    return this->kind ();
  }

} // yy
#line 7216 "C:/Users/HackMan/code/TestEngine/src/Engine/Renderer/parser.hh"




#endif // !YY_YY_C_USERS_HACKMAN_CODE_TESTENGINE_SRC_ENGINE_RENDERER_PARSER_HH_INCLUDED
