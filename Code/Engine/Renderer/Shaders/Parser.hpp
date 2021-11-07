// A Bison parser, made by GNU Bison 3.8.2.

// Skeleton interface for Bison LALR(1) parsers in C++

// Copyright (C) 2002-2015, 2018-2021 Free Software Foundation, Inc.

// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <https://www.gnu.org/licenses/>.

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
 ** \file /cygdrive/c/Users/chiap_000/source/repos/fromasmtodisasm/TestEngine/Code/Engine/Renderer/Shaders/Parser.hpp
 ** Define the yy::parser class.
 */

// C++ LALR(1) parser skeleton written by Akim Demaille.

// DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
// especially those whose name start with YY_ or yy_.  They are
// private implementation details that can be changed or removed.

#ifndef YY_YY_CYGDRIVE_C_USERS_CHIAP_000_SOURCE_REPOS_FROMASMTODISASM_TESTENGINE_CODE_ENGINE_RENDERER_SHADERS_PARSER_HPP_INCLUDED
# define YY_YY_CYGDRIVE_C_USERS_CHIAP_000_SOURCE_REPOS_FROMASMTODISASM_TESTENGINE_CODE_ENGINE_RENDERER_SHADERS_PARSER_HPP_INCLUDED
// "%code requires" blocks.
#line 12 "/cygdrive/c/Users/chiap_000/source/repos/fromasmtodisasm/TestEngine/Code/Engine/Renderer/Shaders/Parser.yy"

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

    struct DirectDeclarator
    {
        string Name;
    };



#line 147 "/cygdrive/c/Users/chiap_000/source/repos/fromasmtodisasm/TestEngine/Code/Engine/Renderer/Shaders/Parser.hpp"

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
# define YY_USE(E) ((void) (E))
#else
# define YY_USE(E) /* empty */
#endif

/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
#if defined __GNUC__ && ! defined __ICC && 406 <= __GNUC__ * 100 + __GNUC_MINOR__
# if __GNUC__ * 100 + __GNUC_MINOR__ < 407
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")
# else
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")              \
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# endif
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
#line 287 "/cygdrive/c/Users/chiap_000/source/repos/fromasmtodisasm/TestEngine/Code/Engine/Renderer/Shaders/Parser.hpp"




  /// A Bison parser.
  class parser
  {
  public:
#ifdef YYSTYPE
# ifdef __GNUC__
#  pragma GCC message "bison: do not #define YYSTYPE in C++, use %define api.value.type"
# endif
    typedef YYSTYPE value_type;
#else
  /// A buffer to store and retrieve objects.
  ///
  /// Sort of a variant, but does not keep track of the nature
  /// of the stored data, since that knowledge is available
  /// via the current parser state.
  class value_type
  {
  public:
    /// Type of *this.
    typedef value_type self_type;

    /// Empty construction.
    value_type () YY_NOEXCEPT
      : yyraw_ ()
      , yytypeid_ (YY_NULLPTR)
    {}

    /// Construct and fill.
    template <typename T>
    value_type (YY_RVREF (T) t)
      : yytypeid_ (&typeid (T))
    {
      YY_ASSERT (sizeof (T) <= size);
      new (yyas_<T> ()) T (YY_MOVE (t));
    }

#if 201103L <= YY_CPLUSPLUS
    /// Non copyable.
    value_type (const self_type&) = delete;
    /// Non copyable.
    self_type& operator= (const self_type&) = delete;
#endif

    /// Destruction, allowed only if empty.
    ~value_type () YY_NOEXCEPT
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
    value_type (const self_type&);
    /// Non copyable.
    self_type& operator= (const self_type&);
#endif

    /// Accessor to raw memory as \a T.
    template <typename T>
    T*
    yyas_ () YY_NOEXCEPT
    {
      void *yyp = yyraw_;
      return static_cast<T*> (yyp);
     }

    /// Const accessor to raw memory as \a T.
    template <typename T>
    const T*
    yyas_ () const YY_NOEXCEPT
    {
      const void *yyp = yyraw_;
      return static_cast<const T*> (yyp);
     }

    /// An auxiliary type to compute the largest semantic type.
    union union_type
    {
      // shader_assignment_shader
      // declarator
      // direct_declarator
      char dummy1[sizeof (DirectDeclarator)];

      // VERTEXPROGRAM
      // FRAGMENTPROGRAM
      // GEOMETRYPROGRAM
      // HULLPROGRAM
      // EVALPROGRAM
      // shader_type
      char dummy2[sizeof (IShader::Type)];

      // TRUE
      // FALSE
      // BOOL
      char dummy3[sizeof (bool)];

      // FLOAT
      char dummy4[sizeof (float)];

      // INT
      char dummy5[sizeof (int)];

      // base_type
      char dummy6[sizeof (nvFX::IUniform::Type)];

      // TYPE_NAME
      // IDENTIFIER
      // STR
      // CODEBODY
      // struct_header
      // struct_footer
      // shader_assignment
      char dummy7[sizeof (std::string)];
    };

    /// The size of the largest semantic type.
    enum { size = sizeof (union_type) };

    /// A buffer to store semantic values.
    union
    {
      /// Strongest alignment constraints.
      long double yyalign_me_;
      /// A buffer large enough to store any of the semantic values.
      char yyraw_[size];
    };

    /// Whether the content is built: if defined, the name of the stored type.
    const std::type_info *yytypeid_;
  };

#endif
    /// Backward compatibility (Bison 3.8).
    typedef value_type semantic_type;

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
    TOK_TYPE_NAME = 258,           // TYPE_NAME
    TOK_IDENTIFIER = 259,          // IDENTIFIER
    TOK_TRUE = 260,                // TRUE
    TOK_FALSE = 261,               // FALSE
    TOK_FLOAT = 262,               // FLOAT
    TOK_INT = 263,                 // INT
    TOK_BOOL = 264,                // BOOL
    TOK_STR = 265,                 // STR
    TOK_HLSL11SHADER = 266,        // HLSL11SHADER
    TOK_TECHNIQUE = 267,           // TECHNIQUE
    TOK_PASS = 268,                // PASS
    TOK_CODEBODY = 269,            // CODEBODY
    TOK_STRING_TYPE = 270,         // STRING_TYPE
    TOK_VOID_TYPE = 271,           // VOID_TYPE
    TOK_UNSIGNED = 272,            // UNSIGNED
    TOK_HIGHP = 273,               // HIGHP
    TOK_MEDIUMP = 274,             // MEDIUMP
    TOK_LOWP = 275,                // LOWP
    TOK_UNIFORM = 276,             // UNIFORM
    TOK_CSTBUFFER = 277,           // CSTBUFFER
    TOK_CONSTANTBUFFER = 278,      // CONSTANTBUFFER
    TOK_FLOAT_TYPE = 279,          // FLOAT_TYPE
    TOK_FLOAT2_TYPE = 280,         // FLOAT2_TYPE
    TOK_FLOAT3_TYPE = 281,         // FLOAT3_TYPE
    TOK_FLOAT4_TYPE = 282,         // FLOAT4_TYPE
    TOK_MAT2_TYPE = 283,           // MAT2_TYPE
    TOK_MAT2x4_TYPE = 284,         // MAT2x4_TYPE
    TOK_MAT3_TYPE = 285,           // MAT3_TYPE
    TOK_MAT34_TYPE = 286,          // MAT34_TYPE
    TOK_MAT4_TYPE = 287,           // MAT4_TYPE
    TOK_BOOL_TYPE = 288,           // BOOL_TYPE
    TOK_BOOL2_TYPE = 289,          // BOOL2_TYPE
    TOK_BOOL3_TYPE = 290,          // BOOL3_TYPE
    TOK_BOOL4_TYPE = 291,          // BOOL4_TYPE
    TOK_INT_TYPE = 292,            // INT_TYPE
    TOK_INT2_TYPE = 293,           // INT2_TYPE
    TOK_INT3_TYPE = 294,           // INT3_TYPE
    TOK_INT4_TYPE = 295,           // INT4_TYPE
    TOK_TEXTURE1D_TYPE = 296,      // TEXTURE1D_TYPE
    TOK_TEXTURE2D_TYPE = 297,      // TEXTURE2D_TYPE
    TOK_TEXTURE2DSHADOW_TYPE = 298, // TEXTURE2DSHADOW_TYPE
    TOK_TEXTURE2DRECT_TYPE = 299,  // TEXTURE2DRECT_TYPE
    TOK_TEXTURE3D_TYPE = 300,      // TEXTURE3D_TYPE
    TOK_TEXTURECUBE_TYPE = 301,    // TEXTURECUBE_TYPE
    TOK_SAMPLER1D_TYPE = 302,      // SAMPLER1D_TYPE
    TOK_SAMPLER2D_TYPE = 303,      // SAMPLER2D_TYPE
    TOK_SAMPLER2DSHADOW_TYPE = 304, // SAMPLER2DSHADOW_TYPE
    TOK_SAMPLER2DRECT_TYPE = 305,  // SAMPLER2DRECT_TYPE
    TOK_SAMPLER3D_TYPE = 306,      // SAMPLER3D_TYPE
    TOK_SAMPLERCUBE_TYPE = 307,    // SAMPLERCUBE_TYPE
    TOK_SAMPLERSTATE = 308,        // SAMPLERSTATE
    TOK_EXTENSION = 309,           // EXTENSION
    TOK_SEPARATE_SHADER = 310,     // SEPARATE_SHADER
    TOK_VERTEXPROGRAM = 311,       // VERTEXPROGRAM
    TOK_FRAGMENTPROGRAM = 312,     // FRAGMENTPROGRAM
    TOK_GEOMETRYPROGRAM = 313,     // GEOMETRYPROGRAM
    TOK_HULLPROGRAM = 314,         // HULLPROGRAM
    TOK_EVALPROGRAM = 315,         // EVALPROGRAM
    TOK_SHDPROFILE = 316,          // SHDPROFILE
    TOK_SAMPLERRESOURCE = 317,     // SAMPLERRESOURCE
    TOK_SAMPLERTEXUNIT = 318,      // SAMPLERTEXUNIT
    TOK_SETSAMPLERSTATE = 319,     // SETSAMPLERSTATE
    TOK_SETDSTSTATE = 320,         // SETDSTSTATE
    TOK_SETRASTERIZATIONSTATE = 321, // SETRASTERIZATIONSTATE
    TOK_SETCOLORSAMPLESTATE = 322, // SETCOLORSAMPLESTATE
    TOK_IMAGERESOURCE = 323,       // IMAGERESOURCE
    TOK_IMAGEUNIT = 324,           // IMAGEUNIT
    TOK_IMAGEACCESS = 325,         // IMAGEACCESS
    TOK_IMAGELAYER = 326,          // IMAGELAYER
    TOK_IMAGELAYERED = 327,        // IMAGELAYERED
    TOK_WRITE_ONLY = 328,          // WRITE_ONLY
    TOK_READ_ONLY = 329,           // READ_ONLY
    TOK_READ_WRITE = 330,          // READ_WRITE
    TOK_VERTEXFORMAT = 331,        // VERTEXFORMAT
    TOK_REGISTER = 332,            // REGISTER
    TOK_FATALERROR = 333,          // FATALERROR
    TOK_STRUCT = 334,              // STRUCT
    TOK_INSPECYFIER = 335,         // INSPECYFIER
    TOK_OUTSPECYFIER = 336         // OUTSPECYFIER
      };
      /// Backward compatibility alias (Bison 3.6).
      typedef token_kind_type yytokentype;
    };

    /// Token kind, as returned by yylex.
    typedef token::token_kind_type token_kind_type;

    /// Backward compatibility alias (Bison 3.6).
    typedef token_kind_type token_type;

    /// Symbol kinds.
    struct symbol_kind
    {
      enum symbol_kind_type
      {
        YYNTOKENS = 95, ///< Number of tokens.
        S_YYEMPTY = -2,
        S_YYEOF = 0,                             // "end of file"
        S_YYerror = 1,                           // error
        S_YYUNDEF = 2,                           // "invalid token"
        S_TYPE_NAME = 3,                         // TYPE_NAME
        S_IDENTIFIER = 4,                        // IDENTIFIER
        S_TRUE = 5,                              // TRUE
        S_FALSE = 6,                             // FALSE
        S_FLOAT = 7,                             // FLOAT
        S_INT = 8,                               // INT
        S_BOOL = 9,                              // BOOL
        S_STR = 10,                              // STR
        S_HLSL11SHADER = 11,                     // HLSL11SHADER
        S_TECHNIQUE = 12,                        // TECHNIQUE
        S_PASS = 13,                             // PASS
        S_CODEBODY = 14,                         // CODEBODY
        S_STRING_TYPE = 15,                      // STRING_TYPE
        S_VOID_TYPE = 16,                        // VOID_TYPE
        S_UNSIGNED = 17,                         // UNSIGNED
        S_HIGHP = 18,                            // HIGHP
        S_MEDIUMP = 19,                          // MEDIUMP
        S_LOWP = 20,                             // LOWP
        S_UNIFORM = 21,                          // UNIFORM
        S_CSTBUFFER = 22,                        // CSTBUFFER
        S_CONSTANTBUFFER = 23,                   // CONSTANTBUFFER
        S_FLOAT_TYPE = 24,                       // FLOAT_TYPE
        S_FLOAT2_TYPE = 25,                      // FLOAT2_TYPE
        S_FLOAT3_TYPE = 26,                      // FLOAT3_TYPE
        S_FLOAT4_TYPE = 27,                      // FLOAT4_TYPE
        S_MAT2_TYPE = 28,                        // MAT2_TYPE
        S_MAT2x4_TYPE = 29,                      // MAT2x4_TYPE
        S_MAT3_TYPE = 30,                        // MAT3_TYPE
        S_MAT34_TYPE = 31,                       // MAT34_TYPE
        S_MAT4_TYPE = 32,                        // MAT4_TYPE
        S_BOOL_TYPE = 33,                        // BOOL_TYPE
        S_BOOL2_TYPE = 34,                       // BOOL2_TYPE
        S_BOOL3_TYPE = 35,                       // BOOL3_TYPE
        S_BOOL4_TYPE = 36,                       // BOOL4_TYPE
        S_INT_TYPE = 37,                         // INT_TYPE
        S_INT2_TYPE = 38,                        // INT2_TYPE
        S_INT3_TYPE = 39,                        // INT3_TYPE
        S_INT4_TYPE = 40,                        // INT4_TYPE
        S_TEXTURE1D_TYPE = 41,                   // TEXTURE1D_TYPE
        S_TEXTURE2D_TYPE = 42,                   // TEXTURE2D_TYPE
        S_TEXTURE2DSHADOW_TYPE = 43,             // TEXTURE2DSHADOW_TYPE
        S_TEXTURE2DRECT_TYPE = 44,               // TEXTURE2DRECT_TYPE
        S_TEXTURE3D_TYPE = 45,                   // TEXTURE3D_TYPE
        S_TEXTURECUBE_TYPE = 46,                 // TEXTURECUBE_TYPE
        S_SAMPLER1D_TYPE = 47,                   // SAMPLER1D_TYPE
        S_SAMPLER2D_TYPE = 48,                   // SAMPLER2D_TYPE
        S_SAMPLER2DSHADOW_TYPE = 49,             // SAMPLER2DSHADOW_TYPE
        S_SAMPLER2DRECT_TYPE = 50,               // SAMPLER2DRECT_TYPE
        S_SAMPLER3D_TYPE = 51,                   // SAMPLER3D_TYPE
        S_SAMPLERCUBE_TYPE = 52,                 // SAMPLERCUBE_TYPE
        S_SAMPLERSTATE = 53,                     // SAMPLERSTATE
        S_EXTENSION = 54,                        // EXTENSION
        S_SEPARATE_SHADER = 55,                  // SEPARATE_SHADER
        S_VERTEXPROGRAM = 56,                    // VERTEXPROGRAM
        S_FRAGMENTPROGRAM = 57,                  // FRAGMENTPROGRAM
        S_GEOMETRYPROGRAM = 58,                  // GEOMETRYPROGRAM
        S_HULLPROGRAM = 59,                      // HULLPROGRAM
        S_EVALPROGRAM = 60,                      // EVALPROGRAM
        S_SHDPROFILE = 61,                       // SHDPROFILE
        S_SAMPLERRESOURCE = 62,                  // SAMPLERRESOURCE
        S_SAMPLERTEXUNIT = 63,                   // SAMPLERTEXUNIT
        S_SETSAMPLERSTATE = 64,                  // SETSAMPLERSTATE
        S_SETDSTSTATE = 65,                      // SETDSTSTATE
        S_SETRASTERIZATIONSTATE = 66,            // SETRASTERIZATIONSTATE
        S_SETCOLORSAMPLESTATE = 67,              // SETCOLORSAMPLESTATE
        S_IMAGERESOURCE = 68,                    // IMAGERESOURCE
        S_IMAGEUNIT = 69,                        // IMAGEUNIT
        S_IMAGEACCESS = 70,                      // IMAGEACCESS
        S_IMAGELAYER = 71,                       // IMAGELAYER
        S_IMAGELAYERED = 72,                     // IMAGELAYERED
        S_WRITE_ONLY = 73,                       // WRITE_ONLY
        S_READ_ONLY = 74,                        // READ_ONLY
        S_READ_WRITE = 75,                       // READ_WRITE
        S_VERTEXFORMAT = 76,                     // VERTEXFORMAT
        S_REGISTER = 77,                         // REGISTER
        S_FATALERROR = 78,                       // FATALERROR
        S_STRUCT = 79,                           // STRUCT
        S_INSPECYFIER = 80,                      // INSPECYFIER
        S_OUTSPECYFIER = 81,                     // OUTSPECYFIER
        S_82_ = 82,                              // ';'
        S_83_ = 83,                              // ','
        S_84_ = 84,                              // '{'
        S_85_ = 85,                              // '('
        S_86_ = 86,                              // ')'
        S_87_ = 87,                              // ':'
        S_88_ = 88,                              // '}'
        S_89_ = 89,                              // '<'
        S_90_ = 90,                              // '>'
        S_91_ = 91,                              // '='
        S_92_ = 92,                              // '['
        S_93_ = 93,                              // ']'
        S_94_ = 94,                              // '.'
        S_YYACCEPT = 95,                         // $accept
        S_input = 96,                            // input
        S_var_spec = 97,                         // var_spec
        S_arguments = 98,                        // arguments
        S_function_definition = 99,              // function_definition
        S_100_1 = 100,                           // $@1
        S_object_type = 101,                     // object_type
        S_function_declaration = 102,            // function_declaration
        S_103_2 = 103,                           // $@2
        S_fatal_error = 104,                     // fatal_error
        S_register_value = 105,                  // register_value
        S_register_declaration = 106,            // register_declaration
        S_cbuffer = 107,                         // cbuffer
        S_template_parameter = 108,              // template_parameter
        S_struct = 109,                          // struct
        S_struct_header = 110,                   // struct_header
        S_struct_body = 111,                     // struct_body
        S_struct_footer = 112,                   // struct_footer
        S_var_decls = 113,                       // var_decls
        S_shader_resource = 114,                 // shader_resource
        S_resource_initializer = 115,            // resource_initializer
        S_texture2d = 116,                       // texture2d
        S_sampler_state = 117,                   // sampler_state
        S_shader_type = 118,                     // shader_type
        S_shader_assignment_shader = 119,        // shader_assignment_shader
        S_shader_assignment = 120,               // shader_assignment
        S_shader_assignments = 121,              // shader_assignments
        S_passstates = 122,                      // passstates
        S_base_type = 123,                       // base_type
        S_primary_expression = 124,              // primary_expression
        S_postfix_expression = 125,              // postfix_expression
        S_declarator = 126,                      // declarator
        S_constant_expression = 127,             // constant_expression
        S_parameter_type_list = 128,             // parameter_type_list
        S_identifier_list = 129,                 // identifier_list
        S_direct_declarator = 130,               // direct_declarator
        S_semantic = 131,                        // semantic
        S_value = 132,                           // value
        S_basic_type_constructor = 133,          // basic_type_constructor
        S_type_constructor = 134,                // type_constructor
        S_var_decl = 135,                        // var_decl
        S_pass = 136,                            // pass
        S_137_3 = 137,                           // $@3
        S_138_4 = 138,                           // $@4
        S_passes = 139,                          // passes
        S_tech = 140,                            // tech
        S_141_5 = 141,                           // $@5
        S_142_6 = 142,                           // $@6
        S_STRING_LIST = 143,                     // STRING_LIST
        S_scalar_type = 144,                     // scalar_type
        S_annotation_list = 145,                 // annotation_list
        S_annotation_value = 146,                // annotation_value
        S_annotation_header = 147,               // annotation_header
        S_annotation_base = 148,                 // annotation_base
        S_annotation = 149,                      // annotation
        S_annotations = 150                      // annotations
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
      basic_symbol () YY_NOEXCEPT
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
      case symbol_kind::S_shader_assignment_shader: // shader_assignment_shader
      case symbol_kind::S_declarator: // declarator
      case symbol_kind::S_direct_declarator: // direct_declarator
        value.move< DirectDeclarator > (std::move (that.value));
        break;

      case symbol_kind::S_VERTEXPROGRAM: // VERTEXPROGRAM
      case symbol_kind::S_FRAGMENTPROGRAM: // FRAGMENTPROGRAM
      case symbol_kind::S_GEOMETRYPROGRAM: // GEOMETRYPROGRAM
      case symbol_kind::S_HULLPROGRAM: // HULLPROGRAM
      case symbol_kind::S_EVALPROGRAM: // EVALPROGRAM
      case symbol_kind::S_shader_type: // shader_type
        value.move< IShader::Type > (std::move (that.value));
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

      case symbol_kind::S_base_type: // base_type
        value.move< nvFX::IUniform::Type > (std::move (that.value));
        break;

      case symbol_kind::S_TYPE_NAME: // TYPE_NAME
      case symbol_kind::S_IDENTIFIER: // IDENTIFIER
      case symbol_kind::S_STR: // STR
      case symbol_kind::S_CODEBODY: // CODEBODY
      case symbol_kind::S_struct_header: // struct_header
      case symbol_kind::S_struct_footer: // struct_footer
      case symbol_kind::S_shader_assignment: // shader_assignment
        value.move< std::string > (std::move (that.value));
        break;

      default:
        break;
    }

      }
#endif

      /// Copy constructor.
      basic_symbol (const basic_symbol& that);

      /// Constructors for typed symbols.
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
      basic_symbol (typename Base::kind_type t, DirectDeclarator&& v, location_type&& l)
        : Base (t)
        , value (std::move (v))
        , location (std::move (l))
      {}
#else
      basic_symbol (typename Base::kind_type t, const DirectDeclarator& v, const location_type& l)
        : Base (t)
        , value (v)
        , location (l)
      {}
#endif

#if 201103L <= YY_CPLUSPLUS
      basic_symbol (typename Base::kind_type t, IShader::Type&& v, location_type&& l)
        : Base (t)
        , value (std::move (v))
        , location (std::move (l))
      {}
#else
      basic_symbol (typename Base::kind_type t, const IShader::Type& v, const location_type& l)
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
      basic_symbol (typename Base::kind_type t, nvFX::IUniform::Type&& v, location_type&& l)
        : Base (t)
        , value (std::move (v))
        , location (std::move (l))
      {}
#else
      basic_symbol (typename Base::kind_type t, const nvFX::IUniform::Type& v, const location_type& l)
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
      void clear () YY_NOEXCEPT
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
      case symbol_kind::S_shader_assignment_shader: // shader_assignment_shader
      case symbol_kind::S_declarator: // declarator
      case symbol_kind::S_direct_declarator: // direct_declarator
        value.template destroy< DirectDeclarator > ();
        break;

      case symbol_kind::S_VERTEXPROGRAM: // VERTEXPROGRAM
      case symbol_kind::S_FRAGMENTPROGRAM: // FRAGMENTPROGRAM
      case symbol_kind::S_GEOMETRYPROGRAM: // GEOMETRYPROGRAM
      case symbol_kind::S_HULLPROGRAM: // HULLPROGRAM
      case symbol_kind::S_EVALPROGRAM: // EVALPROGRAM
      case symbol_kind::S_shader_type: // shader_type
        value.template destroy< IShader::Type > ();
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

      case symbol_kind::S_base_type: // base_type
        value.template destroy< nvFX::IUniform::Type > ();
        break;

      case symbol_kind::S_TYPE_NAME: // TYPE_NAME
      case symbol_kind::S_IDENTIFIER: // IDENTIFIER
      case symbol_kind::S_STR: // STR
      case symbol_kind::S_CODEBODY: // CODEBODY
      case symbol_kind::S_struct_header: // struct_header
      case symbol_kind::S_struct_footer: // struct_footer
      case symbol_kind::S_shader_assignment: // shader_assignment
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
      value_type value;

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
      /// The symbol kind as needed by the constructor.
      typedef token_kind_type kind_type;

      /// Default constructor.
      by_kind () YY_NOEXCEPT;

#if 201103L <= YY_CPLUSPLUS
      /// Move constructor.
      by_kind (by_kind&& that) YY_NOEXCEPT;
#endif

      /// Copy constructor.
      by_kind (const by_kind& that) YY_NOEXCEPT;

      /// Constructor from (external) token numbers.
      by_kind (kind_type t) YY_NOEXCEPT;



      /// Record that this symbol is empty.
      void clear () YY_NOEXCEPT;

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
      symbol_type () YY_NOEXCEPT {}

      /// Constructor for valueless symbols, and symbols from each type.
#if 201103L <= YY_CPLUSPLUS
      symbol_type (int tok, location_type l)
        : super_type (token_kind_type (tok), std::move (l))
#else
      symbol_type (int tok, const location_type& l)
        : super_type (token_kind_type (tok), l)
#endif
      {
#if !defined _MSC_VER || defined __clang__
        YY_ASSERT (tok == token::TOK_END
                   || (token::TOK_YYerror <= tok && tok <= token::TOK_YYUNDEF)
                   || (token::TOK_HLSL11SHADER <= tok && tok <= token::TOK_PASS)
                   || (token::TOK_STRING_TYPE <= tok && tok <= token::TOK_SEPARATE_SHADER)
                   || (token::TOK_SHDPROFILE <= tok && tok <= token::TOK_OUTSPECYFIER)
                   || tok == 59
                   || tok == 44
                   || tok == 123
                   || (40 <= tok && tok <= 41)
                   || tok == 58
                   || tok == 125
                   || tok == 60
                   || tok == 62
                   || tok == 61
                   || tok == 91
                   || tok == 93
                   || tok == 46);
#endif
      }
#if 201103L <= YY_CPLUSPLUS
      symbol_type (int tok, IShader::Type v, location_type l)
        : super_type (token_kind_type (tok), std::move (v), std::move (l))
#else
      symbol_type (int tok, const IShader::Type& v, const location_type& l)
        : super_type (token_kind_type (tok), v, l)
#endif
      {
#if !defined _MSC_VER || defined __clang__
        YY_ASSERT ((token::TOK_VERTEXPROGRAM <= tok && tok <= token::TOK_EVALPROGRAM));
#endif
      }
#if 201103L <= YY_CPLUSPLUS
      symbol_type (int tok, bool v, location_type l)
        : super_type (token_kind_type (tok), std::move (v), std::move (l))
#else
      symbol_type (int tok, const bool& v, const location_type& l)
        : super_type (token_kind_type (tok), v, l)
#endif
      {
#if !defined _MSC_VER || defined __clang__
        YY_ASSERT ((token::TOK_TRUE <= tok && tok <= token::TOK_FALSE)
                   || tok == token::TOK_BOOL);
#endif
      }
#if 201103L <= YY_CPLUSPLUS
      symbol_type (int tok, float v, location_type l)
        : super_type (token_kind_type (tok), std::move (v), std::move (l))
#else
      symbol_type (int tok, const float& v, const location_type& l)
        : super_type (token_kind_type (tok), v, l)
#endif
      {
#if !defined _MSC_VER || defined __clang__
        YY_ASSERT (tok == token::TOK_FLOAT);
#endif
      }
#if 201103L <= YY_CPLUSPLUS
      symbol_type (int tok, int v, location_type l)
        : super_type (token_kind_type (tok), std::move (v), std::move (l))
#else
      symbol_type (int tok, const int& v, const location_type& l)
        : super_type (token_kind_type (tok), v, l)
#endif
      {
#if !defined _MSC_VER || defined __clang__
        YY_ASSERT (tok == token::TOK_INT);
#endif
      }
#if 201103L <= YY_CPLUSPLUS
      symbol_type (int tok, std::string v, location_type l)
        : super_type (token_kind_type (tok), std::move (v), std::move (l))
#else
      symbol_type (int tok, const std::string& v, const location_type& l)
        : super_type (token_kind_type (tok), v, l)
#endif
      {
#if !defined _MSC_VER || defined __clang__
        YY_ASSERT ((token::TOK_TYPE_NAME <= tok && tok <= token::TOK_IDENTIFIER)
                   || tok == token::TOK_STR
                   || tok == token::TOK_CODEBODY);
#endif
      }
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

    // Implementation of make_symbol for each token kind.
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
      make_TYPE_NAME (std::string v, location_type l)
      {
        return symbol_type (token::TOK_TYPE_NAME, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_TYPE_NAME (const std::string& v, const location_type& l)
      {
        return symbol_type (token::TOK_TYPE_NAME, v, l);
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
      make_STRING_TYPE (location_type l)
      {
        return symbol_type (token::TOK_STRING_TYPE, std::move (l));
      }
#else
      static
      symbol_type
      make_STRING_TYPE (const location_type& l)
      {
        return symbol_type (token::TOK_STRING_TYPE, l);
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
      make_CONSTANTBUFFER (location_type l)
      {
        return symbol_type (token::TOK_CONSTANTBUFFER, std::move (l));
      }
#else
      static
      symbol_type
      make_CONSTANTBUFFER (const location_type& l)
      {
        return symbol_type (token::TOK_CONSTANTBUFFER, l);
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
      make_MAT2x4_TYPE (location_type l)
      {
        return symbol_type (token::TOK_MAT2x4_TYPE, std::move (l));
      }
#else
      static
      symbol_type
      make_MAT2x4_TYPE (const location_type& l)
      {
        return symbol_type (token::TOK_MAT2x4_TYPE, l);
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
      make_MAT34_TYPE (location_type l)
      {
        return symbol_type (token::TOK_MAT34_TYPE, std::move (l));
      }
#else
      static
      symbol_type
      make_MAT34_TYPE (const location_type& l)
      {
        return symbol_type (token::TOK_MAT34_TYPE, l);
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
      make_SAMPLERSTATE (location_type l)
      {
        return symbol_type (token::TOK_SAMPLERSTATE, std::move (l));
      }
#else
      static
      symbol_type
      make_SAMPLERSTATE (const location_type& l)
      {
        return symbol_type (token::TOK_SAMPLERSTATE, l);
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
      make_VERTEXPROGRAM (IShader::Type v, location_type l)
      {
        return symbol_type (token::TOK_VERTEXPROGRAM, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_VERTEXPROGRAM (const IShader::Type& v, const location_type& l)
      {
        return symbol_type (token::TOK_VERTEXPROGRAM, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_FRAGMENTPROGRAM (IShader::Type v, location_type l)
      {
        return symbol_type (token::TOK_FRAGMENTPROGRAM, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_FRAGMENTPROGRAM (const IShader::Type& v, const location_type& l)
      {
        return symbol_type (token::TOK_FRAGMENTPROGRAM, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_GEOMETRYPROGRAM (IShader::Type v, location_type l)
      {
        return symbol_type (token::TOK_GEOMETRYPROGRAM, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_GEOMETRYPROGRAM (const IShader::Type& v, const location_type& l)
      {
        return symbol_type (token::TOK_GEOMETRYPROGRAM, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_HULLPROGRAM (IShader::Type v, location_type l)
      {
        return symbol_type (token::TOK_HULLPROGRAM, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_HULLPROGRAM (const IShader::Type& v, const location_type& l)
      {
        return symbol_type (token::TOK_HULLPROGRAM, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_EVALPROGRAM (IShader::Type v, location_type l)
      {
        return symbol_type (token::TOK_EVALPROGRAM, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_EVALPROGRAM (const IShader::Type& v, const location_type& l)
      {
        return symbol_type (token::TOK_EVALPROGRAM, v, l);
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
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_REGISTER (location_type l)
      {
        return symbol_type (token::TOK_REGISTER, std::move (l));
      }
#else
      static
      symbol_type
      make_REGISTER (const location_type& l)
      {
        return symbol_type (token::TOK_REGISTER, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_FATALERROR (location_type l)
      {
        return symbol_type (token::TOK_FATALERROR, std::move (l));
      }
#else
      static
      symbol_type
      make_FATALERROR (const location_type& l)
      {
        return symbol_type (token::TOK_FATALERROR, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_STRUCT (location_type l)
      {
        return symbol_type (token::TOK_STRUCT, std::move (l));
      }
#else
      static
      symbol_type
      make_STRUCT (const location_type& l)
      {
        return symbol_type (token::TOK_STRUCT, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_INSPECYFIER (location_type l)
      {
        return symbol_type (token::TOK_INSPECYFIER, std::move (l));
      }
#else
      static
      symbol_type
      make_INSPECYFIER (const location_type& l)
      {
        return symbol_type (token::TOK_INSPECYFIER, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_OUTSPECYFIER (location_type l)
      {
        return symbol_type (token::TOK_OUTSPECYFIER, std::move (l));
      }
#else
      static
      symbol_type
      make_OUTSPECYFIER (const location_type& l)
      {
        return symbol_type (token::TOK_OUTSPECYFIER, l);
      }
#endif


    class context
    {
    public:
      context (const parser& yyparser, const symbol_type& yyla);
      const symbol_type& lookahead () const YY_NOEXCEPT { return yyla_; }
      symbol_kind_type token () const YY_NOEXCEPT { return yyla_.kind (); }
      const location_type& location () const YY_NOEXCEPT { return yyla_.location; }

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
    static bool yy_pact_value_is_default_ (int yyvalue) YY_NOEXCEPT;

    /// Whether the given \c yytable_ value indicates a syntax error.
    /// \param yyvalue   the value to check
    static bool yy_table_value_is_error_ (int yyvalue) YY_NOEXCEPT;

    static const signed char yypact_ninf_;
    static const short yytable_ninf_;

    /// Convert a scanner token kind \a t to a symbol kind.
    /// In theory \a t should be a token_kind_type, but character literals
    /// are valid, yet not members of the token_kind_type enum.
    static symbol_kind_type yytranslate_ (int t) YY_NOEXCEPT;

    /// Convert the symbol name \a n to a form suitable for a diagnostic.
    static std::string yytnamerr_ (const char *yystr);

    /// For a symbol, its name in clear.
    static const char* const yytname_[];


    // Tables.
    // YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
    // STATE-NUM.
    static const short yypact_[];

    // YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
    // Performed when YYTABLE does not specify something else to do.  Zero
    // means the default is an error.
    static const unsigned char yydefact_[];

    // YYPGOTO[NTERM-NUM].
    static const short yypgoto_[];

    // YYDEFGOTO[NTERM-NUM].
    static const unsigned char yydefgoto_[];

    // YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
    // positive, shift that token.  If negative, reduce the rule whose
    // number is the opposite.  If YYTABLE_NINF, syntax error.
    static const short yytable_[];

    static const short yycheck_[];

    // YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
    // state STATE-NUM.
    static const unsigned char yystos_[];

    // YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.
    static const unsigned char yyr1_[];

    // YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.
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

      stack (size_type n = 200) YY_NOEXCEPT
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
        slice (const stack& stack, index_type range) YY_NOEXCEPT
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
    void yypop_ (int n = 1) YY_NOEXCEPT;

    /// Constants.
    enum
    {
      yylast_ = 315,     ///< Last index in yytable_.
      yynnts_ = 56,  ///< Number of nonterminal symbols.
      yyfinal_ = 2 ///< Termination state number.
    };


    // User arguments.
    Scanner &scanner;
    Driver &driver;

  };

  inline
  parser::symbol_kind_type
  parser::yytranslate_ (int t) YY_NOEXCEPT
  {
    // YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to
    // TOKEN-NUM as returned by yylex.
    static
    const signed char
    translate_table[] =
    {
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
      85,    86,     2,     2,    83,     2,    94,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,    87,    82,
      89,    91,    90,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,    92,     2,    93,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    84,     2,    88,     2,     2,     2,     2,
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
      75,    76,    77,    78,    79,    80,    81
    };
    // Last valid token kind.
    const int code_max = 336;

    if (t <= 0)
      return symbol_kind::S_YYEOF;
    else if (t <= code_max)
      return static_cast <symbol_kind_type> (translate_table[t]);
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
      case symbol_kind::S_shader_assignment_shader: // shader_assignment_shader
      case symbol_kind::S_declarator: // declarator
      case symbol_kind::S_direct_declarator: // direct_declarator
        value.copy< DirectDeclarator > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_VERTEXPROGRAM: // VERTEXPROGRAM
      case symbol_kind::S_FRAGMENTPROGRAM: // FRAGMENTPROGRAM
      case symbol_kind::S_GEOMETRYPROGRAM: // GEOMETRYPROGRAM
      case symbol_kind::S_HULLPROGRAM: // HULLPROGRAM
      case symbol_kind::S_EVALPROGRAM: // EVALPROGRAM
      case symbol_kind::S_shader_type: // shader_type
        value.copy< IShader::Type > (YY_MOVE (that.value));
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

      case symbol_kind::S_base_type: // base_type
        value.copy< nvFX::IUniform::Type > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_TYPE_NAME: // TYPE_NAME
      case symbol_kind::S_IDENTIFIER: // IDENTIFIER
      case symbol_kind::S_STR: // STR
      case symbol_kind::S_CODEBODY: // CODEBODY
      case symbol_kind::S_struct_header: // struct_header
      case symbol_kind::S_struct_footer: // struct_footer
      case symbol_kind::S_shader_assignment: // shader_assignment
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
      case symbol_kind::S_shader_assignment_shader: // shader_assignment_shader
      case symbol_kind::S_declarator: // declarator
      case symbol_kind::S_direct_declarator: // direct_declarator
        value.move< DirectDeclarator > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_VERTEXPROGRAM: // VERTEXPROGRAM
      case symbol_kind::S_FRAGMENTPROGRAM: // FRAGMENTPROGRAM
      case symbol_kind::S_GEOMETRYPROGRAM: // GEOMETRYPROGRAM
      case symbol_kind::S_HULLPROGRAM: // HULLPROGRAM
      case symbol_kind::S_EVALPROGRAM: // EVALPROGRAM
      case symbol_kind::S_shader_type: // shader_type
        value.move< IShader::Type > (YY_MOVE (s.value));
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

      case symbol_kind::S_base_type: // base_type
        value.move< nvFX::IUniform::Type > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_TYPE_NAME: // TYPE_NAME
      case symbol_kind::S_IDENTIFIER: // IDENTIFIER
      case symbol_kind::S_STR: // STR
      case symbol_kind::S_CODEBODY: // CODEBODY
      case symbol_kind::S_struct_header: // struct_header
      case symbol_kind::S_struct_footer: // struct_footer
      case symbol_kind::S_shader_assignment: // shader_assignment
        value.move< std::string > (YY_MOVE (s.value));
        break;

      default:
        break;
    }

    location = YY_MOVE (s.location);
  }

  // by_kind.
  inline
  parser::by_kind::by_kind () YY_NOEXCEPT
    : kind_ (symbol_kind::S_YYEMPTY)
  {}

#if 201103L <= YY_CPLUSPLUS
  inline
  parser::by_kind::by_kind (by_kind&& that) YY_NOEXCEPT
    : kind_ (that.kind_)
  {
    that.clear ();
  }
#endif

  inline
  parser::by_kind::by_kind (const by_kind& that) YY_NOEXCEPT
    : kind_ (that.kind_)
  {}

  inline
  parser::by_kind::by_kind (token_kind_type t) YY_NOEXCEPT
    : kind_ (yytranslate_ (t))
  {}



  inline
  void
  parser::by_kind::clear () YY_NOEXCEPT
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
#line 3155 "/cygdrive/c/Users/chiap_000/source/repos/fromasmtodisasm/TestEngine/Code/Engine/Renderer/Shaders/Parser.hpp"




#endif // !YY_YY_CYGDRIVE_C_USERS_CHIAP_000_SOURCE_REPOS_FROMASMTODISASM_TESTENGINE_CODE_ENGINE_RENDERER_SHADERS_PARSER_HPP_INCLUDED
