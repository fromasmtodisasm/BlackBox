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
 ** \file /cygdrive/e/code/fromasmtodisasm/TestEngine/src/Engine/Renderer/Shaders/Parser.hpp
 ** Define the yy::parser class.
 */

// C++ LALR(1) parser skeleton written by Akim Demaille.

#ifndef YY_YY_CYGDRIVE_E_CODE_FROMASMTODISASM_TESTENGINE_SRC_ENGINE_RENDERER_SHADERS_PARSER_HPP_INCLUDED
# define YY_YY_CYGDRIVE_E_CODE_FROMASMTODISASM_TESTENGINE_SRC_ENGINE_RENDERER_SHADERS_PARSER_HPP_INCLUDED
// //                    "%code requires" blocks.
#line 11 "/cygdrive/e/code/fromasmtodisasm/TestEngine/src/Engine/Renderer/Shaders/Parser.yy" // lalr1.cc:377

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


#line 132 "/cygdrive/e/code/fromasmtodisasm/TestEngine/src/Engine/Renderer/Shaders/Parser.hpp" // lalr1.cc:377

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
#line 209 "/cygdrive/e/code/fromasmtodisasm/TestEngine/src/Engine/Renderer/Shaders/Parser.hpp" // lalr1.cc:377



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
      // VERTEXPROGRAM
      // FRAGMENTPROGRAM
      // GEOMETRYPROGRAM
      // HULLPROGRAM
      // EVALPROGRAM
      // shader_type
      char dummy1[sizeof(IShader::Type)];

      // LANG_ID
      // lang
      char dummy2[sizeof(ShaderLangId)];

      // TRUE
      // FALSE
      // BOOL
      char dummy3[sizeof(bool)];

      // FLOAT
      char dummy4[sizeof(float)];

      // INT
      char dummy5[sizeof(int)];

      // base_type
      char dummy6[sizeof(nvFX::IUniform::Type)];

      // IDENTIFIER
      // STR
      // CODEBODY
      // VARNAME
      // shader_assignment
      // var_decl
      // semantic
      // glsl_header
      // hlsl_header
      // shader_header
      char dummy7[sizeof(std::string)];
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
        TOK_IDENTIFIER = 258,
        TOK_TRUE = 259,
        TOK_FALSE = 260,
        TOK_FLOAT = 261,
        TOK_INT = 262,
        TOK_BOOL = 263,
        TOK_STR = 264,
        TOK_LANG_ID = 265,
        TOK_LANGUAGE = 266,
        TOK_GLSLSHADER = 267,
        TOK_HLSL10SHADER = 268,
        TOK_HLSL11SHADER = 269,
        TOK_CGSHADER = 270,
        TOK_SAMPLER_STATE = 271,
        TOK_DST_STATE = 272,
        TOK_PR_STATE = 273,
        TOK_COLOR_SAMPLE_STATE = 274,
        TOK_RASTERIZATION_STATE = 275,
        TOK_TECHNIQUE = 276,
        TOK_PASS = 277,
        TOK_CODEBODY = 278,
        TOK_VARNAME = 279,
        TOK_TEXTURERESOURCE = 280,
        TOK_TEXTURERESOURCE1D = 281,
        TOK_TEXTURERESOURCE2D = 282,
        TOK_TEXTURERESOURCE3D = 283,
        TOK_TEXTURERESOURCERECT = 284,
        TOK_TEXTURERESOURCECUBE = 285,
        TOK_INPUTLAYOUT = 286,
        TOK_VOID_TYPE = 287,
        TOK_UNSIGNED = 288,
        TOK_HIGHP = 289,
        TOK_MEDIUMP = 290,
        TOK_LOWP = 291,
        TOK_UNIFORM = 292,
        TOK_CSTBUFFER = 293,
        TOK_FLOAT_TYPE = 294,
        TOK_FLOAT2_TYPE = 295,
        TOK_FLOAT3_TYPE = 296,
        TOK_FLOAT4_TYPE = 297,
        TOK_MAT2_TYPE = 298,
        TOK_MAT3_TYPE = 299,
        TOK_MAT4_TYPE = 300,
        TOK_BOOL_TYPE = 301,
        TOK_BOOL2_TYPE = 302,
        TOK_BOOL3_TYPE = 303,
        TOK_BOOL4_TYPE = 304,
        TOK_INT_TYPE = 305,
        TOK_INT2_TYPE = 306,
        TOK_INT3_TYPE = 307,
        TOK_INT4_TYPE = 308,
        TOK_TEXTURE1D_TYPE = 309,
        TOK_TEXTURE2D_TYPE = 310,
        TOK_TEXTURE2DSHADOW_TYPE = 311,
        TOK_TEXTURE2DRECT_TYPE = 312,
        TOK_TEXTURE3D_TYPE = 313,
        TOK_TEXTURECUBE_TYPE = 314,
        TOK_SAMPLER1D_TYPE = 315,
        TOK_SAMPLER2D_TYPE = 316,
        TOK_SAMPLER2DSHADOW_TYPE = 317,
        TOK_SAMPLER2DRECT_TYPE = 318,
        TOK_SAMPLER3D_TYPE = 319,
        TOK_SAMPLERCUBE_TYPE = 320,
        TOK_EXTENSION = 321,
        TOK_SEPARATE_SHADER = 322,
        TOK_VERTEXPROGRAM = 323,
        TOK_FRAGMENTPROGRAM = 324,
        TOK_GEOMETRYPROGRAM = 325,
        TOK_HULLPROGRAM = 326,
        TOK_EVALPROGRAM = 327,
        TOK_SHDPROFILE = 328,
        TOK_SAMPLERRESOURCE = 329,
        TOK_SAMPLERTEXUNIT = 330,
        TOK_SETSAMPLERSTATE = 331,
        TOK_SETDSTSTATE = 332,
        TOK_SETRASTERIZATIONSTATE = 333,
        TOK_SETCOLORSAMPLESTATE = 334,
        TOK_IMAGERESOURCE = 335,
        TOK_IMAGEUNIT = 336,
        TOK_IMAGEACCESS = 337,
        TOK_IMAGELAYER = 338,
        TOK_IMAGELAYERED = 339,
        TOK_WRITE_ONLY = 340,
        TOK_READ_ONLY = 341,
        TOK_READ_WRITE = 342,
        TOK_VERTEXFORMAT = 343
      };
    };

    /// (External) token type, as returned by yylex.
    typedef token::yytokentype token_type;

    /// Symbol type: an internal symbol number.
    typedef int symbol_number_type;

    /// The symbol type number to denote an empty symbol.
    enum { empty_symbol = -2 };

    /// Internal symbol number for tokens (subsumed by symbol_number_type).
    typedef unsigned char token_number_type;

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

  basic_symbol (typename Base::kind_type t, const IShader::Type v, const location_type& l);

  basic_symbol (typename Base::kind_type t, const ShaderLangId v, const location_type& l);

  basic_symbol (typename Base::kind_type t, const bool v, const location_type& l);

  basic_symbol (typename Base::kind_type t, const float v, const location_type& l);

  basic_symbol (typename Base::kind_type t, const int v, const location_type& l);

  basic_symbol (typename Base::kind_type t, const nvFX::IUniform::Type v, const location_type& l);

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
    make_LANG_ID (const ShaderLangId& v, const location_type& l);

    static inline
    symbol_type
    make_LANGUAGE (const location_type& l);

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
    make_INPUTLAYOUT (const location_type& l);

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
    make_VERTEXPROGRAM (const IShader::Type& v, const location_type& l);

    static inline
    symbol_type
    make_FRAGMENTPROGRAM (const IShader::Type& v, const location_type& l);

    static inline
    symbol_type
    make_GEOMETRYPROGRAM (const IShader::Type& v, const location_type& l);

    static inline
    symbol_type
    make_HULLPROGRAM (const IShader::Type& v, const location_type& l);

    static inline
    symbol_type
    make_EVALPROGRAM (const IShader::Type& v, const location_type& l);

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
  static const signed char yydefgoto_[];

  // YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
  // positive, shift that token.  If negative, reduce the rule whose
  // number is the opposite.  If YYTABLE_NINF, syntax error.
  static const signed char yytable_[];

  static const signed char yycheck_[];

  // YYSTOS[STATE-NUM] -- The (internal number of the) accessing
  // symbol of state STATE-NUM.
  static const unsigned char yystos_[];

  // YYR1[YYN] -- Symbol number of symbol that rule YYN derives.
  static const unsigned char yyr1_[];

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
      yylast_ = 182,     ///< Last index in yytable_.
      yynnts_ = 41,  ///< Number of nonterminal symbols.
      yyfinal_ = 3, ///< Termination state number.
      yyterror_ = 1,
      yyerrcode_ = 256,
      yyntokens_ = 108  ///< Number of tokens.
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
       2,     2,     2,     2,     2,     2,     2,     2,    93,    89,
      94,    90,    95,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    91,     2,    92,     2,     2,     2,     2,
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
      85,    86,    87,    88,    96,    97,    98,    99,   100,   101,
     102,   103,   104,   105,   106,   107
    };
    const unsigned int user_token_number_max_ = 355;
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
      case 68: // VERTEXPROGRAM
      case 69: // FRAGMENTPROGRAM
      case 70: // GEOMETRYPROGRAM
      case 71: // HULLPROGRAM
      case 72: // EVALPROGRAM
      case 111: // shader_type
        value.copy< IShader::Type > (other.value);
        break;

      case 10: // LANG_ID
      case 110: // lang
        value.copy< ShaderLangId > (other.value);
        break;

      case 4: // TRUE
      case 5: // FALSE
      case 8: // BOOL
        value.copy< bool > (other.value);
        break;

      case 6: // FLOAT
        value.copy< float > (other.value);
        break;

      case 7: // INT
        value.copy< int > (other.value);
        break;

      case 118: // base_type
        value.copy< nvFX::IUniform::Type > (other.value);
        break;

      case 3: // IDENTIFIER
      case 9: // STR
      case 23: // CODEBODY
      case 24: // VARNAME
      case 112: // shader_assignment
      case 117: // var_decl
      case 119: // semantic
      case 131: // glsl_header
      case 132: // hlsl_header
      case 134: // shader_header
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
      case 68: // VERTEXPROGRAM
      case 69: // FRAGMENTPROGRAM
      case 70: // GEOMETRYPROGRAM
      case 71: // HULLPROGRAM
      case 72: // EVALPROGRAM
      case 111: // shader_type
        value.copy< IShader::Type > (v);
        break;

      case 10: // LANG_ID
      case 110: // lang
        value.copy< ShaderLangId > (v);
        break;

      case 4: // TRUE
      case 5: // FALSE
      case 8: // BOOL
        value.copy< bool > (v);
        break;

      case 6: // FLOAT
        value.copy< float > (v);
        break;

      case 7: // INT
        value.copy< int > (v);
        break;

      case 118: // base_type
        value.copy< nvFX::IUniform::Type > (v);
        break;

      case 3: // IDENTIFIER
      case 9: // STR
      case 23: // CODEBODY
      case 24: // VARNAME
      case 112: // shader_assignment
      case 117: // var_decl
      case 119: // semantic
      case 131: // glsl_header
      case 132: // hlsl_header
      case 134: // shader_header
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
  parser::basic_symbol<Base>::basic_symbol (typename Base::kind_type t, const IShader::Type v, const location_type& l)
    : Base (t)
    , value (v)
    , location (l)
  {}

  template <typename Base>
  parser::basic_symbol<Base>::basic_symbol (typename Base::kind_type t, const ShaderLangId v, const location_type& l)
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
  parser::basic_symbol<Base>::basic_symbol (typename Base::kind_type t, const nvFX::IUniform::Type v, const location_type& l)
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
      case 68: // VERTEXPROGRAM
      case 69: // FRAGMENTPROGRAM
      case 70: // GEOMETRYPROGRAM
      case 71: // HULLPROGRAM
      case 72: // EVALPROGRAM
      case 111: // shader_type
        value.template destroy< IShader::Type > ();
        break;

      case 10: // LANG_ID
      case 110: // lang
        value.template destroy< ShaderLangId > ();
        break;

      case 4: // TRUE
      case 5: // FALSE
      case 8: // BOOL
        value.template destroy< bool > ();
        break;

      case 6: // FLOAT
        value.template destroy< float > ();
        break;

      case 7: // INT
        value.template destroy< int > ();
        break;

      case 118: // base_type
        value.template destroy< nvFX::IUniform::Type > ();
        break;

      case 3: // IDENTIFIER
      case 9: // STR
      case 23: // CODEBODY
      case 24: // VARNAME
      case 112: // shader_assignment
      case 117: // var_decl
      case 119: // semantic
      case 131: // glsl_header
      case 132: // hlsl_header
      case 134: // shader_header
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
      case 68: // VERTEXPROGRAM
      case 69: // FRAGMENTPROGRAM
      case 70: // GEOMETRYPROGRAM
      case 71: // HULLPROGRAM
      case 72: // EVALPROGRAM
      case 111: // shader_type
        value.move< IShader::Type > (s.value);
        break;

      case 10: // LANG_ID
      case 110: // lang
        value.move< ShaderLangId > (s.value);
        break;

      case 4: // TRUE
      case 5: // FALSE
      case 8: // BOOL
        value.move< bool > (s.value);
        break;

      case 6: // FLOAT
        value.move< float > (s.value);
        break;

      case 7: // INT
        value.move< int > (s.value);
        break;

      case 118: // base_type
        value.move< nvFX::IUniform::Type > (s.value);
        break;

      case 3: // IDENTIFIER
      case 9: // STR
      case 23: // CODEBODY
      case 24: // VARNAME
      case 112: // shader_assignment
      case 117: // var_decl
      case 119: // semantic
      case 131: // glsl_header
      case 132: // hlsl_header
      case 134: // shader_header
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
     335,   336,   337,   338,   339,   340,   341,   342,   343,    59,
      61,   123,   125,    58,    60,    62,   344,   345,   346,   347,
     348,   349,   350,   351,   352,   353,   354,   355
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
  parser::make_LANG_ID (const ShaderLangId& v, const location_type& l)
  {
    return symbol_type (token::TOK_LANG_ID, v, l);
  }

  parser::symbol_type
  parser::make_LANGUAGE (const location_type& l)
  {
    return symbol_type (token::TOK_LANGUAGE, l);
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
  parser::make_INPUTLAYOUT (const location_type& l)
  {
    return symbol_type (token::TOK_INPUTLAYOUT, l);
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
  parser::make_VERTEXPROGRAM (const IShader::Type& v, const location_type& l)
  {
    return symbol_type (token::TOK_VERTEXPROGRAM, v, l);
  }

  parser::symbol_type
  parser::make_FRAGMENTPROGRAM (const IShader::Type& v, const location_type& l)
  {
    return symbol_type (token::TOK_FRAGMENTPROGRAM, v, l);
  }

  parser::symbol_type
  parser::make_GEOMETRYPROGRAM (const IShader::Type& v, const location_type& l)
  {
    return symbol_type (token::TOK_GEOMETRYPROGRAM, v, l);
  }

  parser::symbol_type
  parser::make_HULLPROGRAM (const IShader::Type& v, const location_type& l)
  {
    return symbol_type (token::TOK_HULLPROGRAM, v, l);
  }

  parser::symbol_type
  parser::make_EVALPROGRAM (const IShader::Type& v, const location_type& l)
  {
    return symbol_type (token::TOK_EVALPROGRAM, v, l);
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
  parser::make_VERTEXFORMAT (const location_type& l)
  {
    return symbol_type (token::TOK_VERTEXFORMAT, l);
  }



} // yy
#line 2191 "/cygdrive/e/code/fromasmtodisasm/TestEngine/src/Engine/Renderer/Shaders/Parser.hpp" // lalr1.cc:377




#endif // !YY_YY_CYGDRIVE_E_CODE_FROMASMTODISASM_TESTENGINE_SRC_ENGINE_RENDERER_SHADERS_PARSER_HPP_INCLUDED
