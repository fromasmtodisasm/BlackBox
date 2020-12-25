// A Bison parser, made by GNU Bison 3.0.4.

// Skeleton implementation for Bison LALR(1) parsers in C++

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
// //                    "%code top" blocks.
#line 110 "/cygdrive/e/code/fromasmtodisasm/TestEngine/src/Engine/Renderer/Shaders/Parser.yy" // lalr1.cc:397

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

#line 84 "/cygdrive/e/code/fromasmtodisasm/TestEngine/src/Engine/Renderer/Shaders/Parser.cpp" // lalr1.cc:397


// First part of user declarations.

#line 89 "/cygdrive/e/code/fromasmtodisasm/TestEngine/src/Engine/Renderer/Shaders/Parser.cpp" // lalr1.cc:404

# ifndef YY_NULLPTR
#  if defined __cplusplus && 201103L <= __cplusplus
#   define YY_NULLPTR nullptr
#  else
#   define YY_NULLPTR 0
#  endif
# endif

#include "Parser.hpp"

// User implementation prologue.

#line 103 "/cygdrive/e/code/fromasmtodisasm/TestEngine/src/Engine/Renderer/Shaders/Parser.cpp" // lalr1.cc:412


#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> // FIXME: INFRINGES ON USER NAME SPACE.
#   define YY_(msgid) dgettext ("bison-runtime", msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(msgid) msgid
# endif
#endif

#define YYRHSLOC(Rhs, K) ((Rhs)[K].location)
/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

# ifndef YYLLOC_DEFAULT
#  define YYLLOC_DEFAULT(Current, Rhs, N)                               \
    do                                                                  \
      if (N)                                                            \
        {                                                               \
          (Current).begin  = YYRHSLOC (Rhs, 1).begin;                   \
          (Current).end    = YYRHSLOC (Rhs, N).end;                     \
        }                                                               \
      else                                                              \
        {                                                               \
          (Current).begin = (Current).end = YYRHSLOC (Rhs, 0).end;      \
        }                                                               \
    while (/*CONSTCOND*/ false)
# endif


// Suppress unused-variable warnings by "using" E.
#define YYUSE(E) ((void) (E))

// Enable debugging if requested.
#if YYDEBUG

// A pseudo ostream that takes yydebug_ into account.
# define YYCDEBUG if (yydebug_) (*yycdebug_)

# define YY_SYMBOL_PRINT(Title, Symbol)         \
  do {                                          \
    if (yydebug_)                               \
    {                                           \
      *yycdebug_ << Title << ' ';               \
      yy_print_ (*yycdebug_, Symbol);           \
      *yycdebug_ << std::endl;                  \
    }                                           \
  } while (false)

# define YY_REDUCE_PRINT(Rule)          \
  do {                                  \
    if (yydebug_)                       \
      yy_reduce_print_ (Rule);          \
  } while (false)

# define YY_STACK_PRINT()               \
  do {                                  \
    if (yydebug_)                       \
      yystack_print_ ();                \
  } while (false)

#else // !YYDEBUG

# define YYCDEBUG if (false) std::cerr
# define YY_SYMBOL_PRINT(Title, Symbol)  YYUSE(Symbol)
# define YY_REDUCE_PRINT(Rule)           static_cast<void>(0)
# define YY_STACK_PRINT()                static_cast<void>(0)

#endif // !YYDEBUG

#define yyerrok         (yyerrstatus_ = 0)
#define yyclearin       (yyla.clear ())

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab
#define YYRECOVERING()  (!!yyerrstatus_)


namespace yy {
#line 189 "/cygdrive/e/code/fromasmtodisasm/TestEngine/src/Engine/Renderer/Shaders/Parser.cpp" // lalr1.cc:479

  /* Return YYSTR after stripping away unnecessary quotes and
     backslashes, so that it's suitable for yyerror.  The heuristic is
     that double-quoting is unnecessary unless the string contains an
     apostrophe, a comma, or backslash (other than backslash-backslash).
     YYSTR is taken from yytname.  */
  std::string
  parser::yytnamerr_ (const char *yystr)
  {
    if (*yystr == '"')
      {
        std::string yyr = "";
        char const *yyp = yystr;

        for (;;)
          switch (*++yyp)
            {
            case '\'':
            case ',':
              goto do_not_strip_quotes;

            case '\\':
              if (*++yyp != '\\')
                goto do_not_strip_quotes;
              // Fall through.
            default:
              yyr += *yyp;
              break;

            case '"':
              return yyr;
            }
      do_not_strip_quotes: ;
      }

    return yystr;
  }


  /// Build a parser object.
  parser::parser (Scanner &scanner_yyarg, Driver &driver_yyarg)
    :
#if YYDEBUG
      yydebug_ (false),
      yycdebug_ (&std::cerr),
#endif
      scanner (scanner_yyarg),
      driver (driver_yyarg)
  {}

  parser::~parser ()
  {}


  /*---------------.
  | Symbol types.  |
  `---------------*/



  // by_state.
  inline
  parser::by_state::by_state ()
    : state (empty_state)
  {}

  inline
  parser::by_state::by_state (const by_state& other)
    : state (other.state)
  {}

  inline
  void
  parser::by_state::clear ()
  {
    state = empty_state;
  }

  inline
  void
  parser::by_state::move (by_state& that)
  {
    state = that.state;
    that.clear ();
  }

  inline
  parser::by_state::by_state (state_type s)
    : state (s)
  {}

  inline
  parser::symbol_number_type
  parser::by_state::type_get () const
  {
    if (state == empty_state)
      return empty_symbol;
    else
      return yystos_[state];
  }

  inline
  parser::stack_symbol_type::stack_symbol_type ()
  {}


  inline
  parser::stack_symbol_type::stack_symbol_type (state_type s, symbol_type& that)
    : super_type (s, that.location)
  {
      switch (that.type_get ())
    {
      case 66: // VERTEXPROGRAM
      case 67: // FRAGMENTPROGRAM
      case 68: // GEOMETRYPROGRAM
      case 69: // HULLPROGRAM
      case 70: // EVALPROGRAM
      case 108: // shader_type
        value.move< IShader::Type > (that.value);
        break;

      case 4: // TRUE
      case 5: // FALSE
      case 8: // BOOL
        value.move< bool > (that.value);
        break;

      case 6: // FLOAT
        value.move< float > (that.value);
        break;

      case 7: // INT
        value.move< int > (that.value);
        break;

      case 115: // base_type
        value.move< nvFX::IUniform::Type > (that.value);
        break;

      case 3: // IDENTIFIER
      case 9: // STR
      case 21: // CODEBODY
      case 22: // VARNAME
      case 109: // shader_assignment
      case 114: // var_decl
      case 116: // semantic
      case 128: // glsl_header
        value.move< std::string > (that.value);
        break;

      default:
        break;
    }

    // that is emptied.
    that.type = empty_symbol;
  }

  inline
  parser::stack_symbol_type&
  parser::stack_symbol_type::operator= (const stack_symbol_type& that)
  {
    state = that.state;
      switch (that.type_get ())
    {
      case 66: // VERTEXPROGRAM
      case 67: // FRAGMENTPROGRAM
      case 68: // GEOMETRYPROGRAM
      case 69: // HULLPROGRAM
      case 70: // EVALPROGRAM
      case 108: // shader_type
        value.copy< IShader::Type > (that.value);
        break;

      case 4: // TRUE
      case 5: // FALSE
      case 8: // BOOL
        value.copy< bool > (that.value);
        break;

      case 6: // FLOAT
        value.copy< float > (that.value);
        break;

      case 7: // INT
        value.copy< int > (that.value);
        break;

      case 115: // base_type
        value.copy< nvFX::IUniform::Type > (that.value);
        break;

      case 3: // IDENTIFIER
      case 9: // STR
      case 21: // CODEBODY
      case 22: // VARNAME
      case 109: // shader_assignment
      case 114: // var_decl
      case 116: // semantic
      case 128: // glsl_header
        value.copy< std::string > (that.value);
        break;

      default:
        break;
    }

    location = that.location;
    return *this;
  }


  template <typename Base>
  inline
  void
  parser::yy_destroy_ (const char* yymsg, basic_symbol<Base>& yysym) const
  {
    if (yymsg)
      YY_SYMBOL_PRINT (yymsg, yysym);
  }

#if YYDEBUG
  template <typename Base>
  void
  parser::yy_print_ (std::ostream& yyo,
                                     const basic_symbol<Base>& yysym) const
  {
    std::ostream& yyoutput = yyo;
    YYUSE (yyoutput);
    symbol_number_type yytype = yysym.type_get ();
    // Avoid a (spurious) G++ 4.8 warning about "array subscript is
    // below array bounds".
    if (yysym.empty ())
      std::abort ();
    yyo << (yytype < yyntokens_ ? "token" : "nterm")
        << ' ' << yytname_[yytype] << " ("
        << yysym.location << ": ";
    YYUSE (yytype);
    yyo << ')';
  }
#endif

  inline
  void
  parser::yypush_ (const char* m, state_type s, symbol_type& sym)
  {
    stack_symbol_type t (s, sym);
    yypush_ (m, t);
  }

  inline
  void
  parser::yypush_ (const char* m, stack_symbol_type& s)
  {
    if (m)
      YY_SYMBOL_PRINT (m, s);
    yystack_.push (s);
  }

  inline
  void
  parser::yypop_ (unsigned int n)
  {
    yystack_.pop (n);
  }

#if YYDEBUG
  std::ostream&
  parser::debug_stream () const
  {
    return *yycdebug_;
  }

  void
  parser::set_debug_stream (std::ostream& o)
  {
    yycdebug_ = &o;
  }


  parser::debug_level_type
  parser::debug_level () const
  {
    return yydebug_;
  }

  void
  parser::set_debug_level (debug_level_type l)
  {
    yydebug_ = l;
  }
#endif // YYDEBUG

  inline parser::state_type
  parser::yy_lr_goto_state_ (state_type yystate, int yysym)
  {
    int yyr = yypgoto_[yysym - yyntokens_] + yystate;
    if (0 <= yyr && yyr <= yylast_ && yycheck_[yyr] == yystate)
      return yytable_[yyr];
    else
      return yydefgoto_[yysym - yyntokens_];
  }

  inline bool
  parser::yy_pact_value_is_default_ (int yyvalue)
  {
    return yyvalue == yypact_ninf_;
  }

  inline bool
  parser::yy_table_value_is_error_ (int yyvalue)
  {
    return yyvalue == yytable_ninf_;
  }

  int
  parser::parse ()
  {
    // State.
    int yyn;
    /// Length of the RHS of the rule being reduced.
    int yylen = 0;

    // Error handling.
    int yynerrs_ = 0;
    int yyerrstatus_ = 0;

    /// The lookahead symbol.
    symbol_type yyla;

    /// The locations where the error started and ended.
    stack_symbol_type yyerror_range[3];

    /// The return value of parse ().
    int yyresult;

    // FIXME: This shoud be completely indented.  It is not yet to
    // avoid gratuitous conflicts when merging into the master branch.
    try
      {
    YYCDEBUG << "Starting parse" << std::endl;


    // User initialization code.
    #line 100 "/cygdrive/e/code/fromasmtodisasm/TestEngine/src/Engine/Renderer/Shaders/Parser.yy" // lalr1.cc:741
{
    is_common = false;
    CommonCode.clear();
}

#line 540 "/cygdrive/e/code/fromasmtodisasm/TestEngine/src/Engine/Renderer/Shaders/Parser.cpp" // lalr1.cc:741

    /* Initialize the stack.  The initial state will be set in
       yynewstate, since the latter expects the semantical and the
       location values to have been already stored, initialize these
       stacks with a primary value.  */
    yystack_.clear ();
    yypush_ (YY_NULLPTR, 0, yyla);

    // A new symbol was pushed on the stack.
  yynewstate:
    YYCDEBUG << "Entering state " << yystack_[0].state << std::endl;

    // Accept?
    if (yystack_[0].state == yyfinal_)
      goto yyacceptlab;

    goto yybackup;

    // Backup.
  yybackup:

    // Try to take a decision without lookahead.
    yyn = yypact_[yystack_[0].state];
    if (yy_pact_value_is_default_ (yyn))
      goto yydefault;

    // Read a lookahead token.
    if (yyla.empty ())
      {
        YYCDEBUG << "Reading a token: ";
        try
          {
            symbol_type yylookahead (yylex (scanner, driver));
            yyla.move (yylookahead);
          }
        catch (const syntax_error& yyexc)
          {
            error (yyexc);
            goto yyerrlab1;
          }
      }
    YY_SYMBOL_PRINT ("Next token is", yyla);

    /* If the proper action on seeing token YYLA.TYPE is to reduce or
       to detect an error, take that action.  */
    yyn += yyla.type_get ();
    if (yyn < 0 || yylast_ < yyn || yycheck_[yyn] != yyla.type_get ())
      goto yydefault;

    // Reduce or error.
    yyn = yytable_[yyn];
    if (yyn <= 0)
      {
        if (yy_table_value_is_error_ (yyn))
          goto yyerrlab;
        yyn = -yyn;
        goto yyreduce;
      }

    // Count tokens shifted since error; after three, turn off error status.
    if (yyerrstatus_)
      --yyerrstatus_;

    // Shift the lookahead token.
    yypush_ ("Shifting", yyn, yyla);
    goto yynewstate;

  /*-----------------------------------------------------------.
  | yydefault -- do the default action for the current state.  |
  `-----------------------------------------------------------*/
  yydefault:
    yyn = yydefact_[yystack_[0].state];
    if (yyn == 0)
      goto yyerrlab;
    goto yyreduce;

  /*-----------------------------.
  | yyreduce -- Do a reduction.  |
  `-----------------------------*/
  yyreduce:
    yylen = yyr2_[yyn];
    {
      stack_symbol_type yylhs;
      yylhs.state = yy_lr_goto_state_(yystack_[yylen].state, yyr1_[yyn]);
      /* Variants are always initialized to an empty instance of the
         correct type. The default '$$ = $1' action is NOT applied
         when using variants.  */
        switch (yyr1_[yyn])
    {
      case 66: // VERTEXPROGRAM
      case 67: // FRAGMENTPROGRAM
      case 68: // GEOMETRYPROGRAM
      case 69: // HULLPROGRAM
      case 70: // EVALPROGRAM
      case 108: // shader_type
        yylhs.value.build< IShader::Type > ();
        break;

      case 4: // TRUE
      case 5: // FALSE
      case 8: // BOOL
        yylhs.value.build< bool > ();
        break;

      case 6: // FLOAT
        yylhs.value.build< float > ();
        break;

      case 7: // INT
        yylhs.value.build< int > ();
        break;

      case 115: // base_type
        yylhs.value.build< nvFX::IUniform::Type > ();
        break;

      case 3: // IDENTIFIER
      case 9: // STR
      case 21: // CODEBODY
      case 22: // VARNAME
      case 109: // shader_assignment
      case 114: // var_decl
      case 116: // semantic
      case 128: // glsl_header
        yylhs.value.build< std::string > ();
        break;

      default:
        break;
    }


      // Compute the default @$.
      {
        slice<stack_symbol_type, stack_type> slice (yystack_, yylen);
        YYLLOC_DEFAULT (yylhs.location, slice, yylen);
      }

      // Perform the reduction.
      YY_REDUCE_PRINT (yyn);
      try
        {
          switch (yyn)
            {
  case 2:
#line 303 "/cygdrive/e/code/fromasmtodisasm/TestEngine/src/Engine/Renderer/Shaders/Parser.yy" // lalr1.cc:859
    { gEnv->pLog->LogWarning("Empty effect"); }
#line 688 "/cygdrive/e/code/fromasmtodisasm/TestEngine/src/Engine/Renderer/Shaders/Parser.cpp" // lalr1.cc:859
    break;

  case 19:
#line 324 "/cygdrive/e/code/fromasmtodisasm/TestEngine/src/Engine/Renderer/Shaders/Parser.yy" // lalr1.cc:859
    {yylhs.value.as< IShader::Type > () = yystack_[0].value.as< IShader::Type > ();}
#line 694 "/cygdrive/e/code/fromasmtodisasm/TestEngine/src/Engine/Renderer/Shaders/Parser.cpp" // lalr1.cc:859
    break;

  case 20:
#line 325 "/cygdrive/e/code/fromasmtodisasm/TestEngine/src/Engine/Renderer/Shaders/Parser.yy" // lalr1.cc:859
    {yylhs.value.as< IShader::Type > () = yystack_[0].value.as< IShader::Type > ();}
#line 700 "/cygdrive/e/code/fromasmtodisasm/TestEngine/src/Engine/Renderer/Shaders/Parser.cpp" // lalr1.cc:859
    break;

  case 21:
#line 326 "/cygdrive/e/code/fromasmtodisasm/TestEngine/src/Engine/Renderer/Shaders/Parser.yy" // lalr1.cc:859
    {yylhs.value.as< IShader::Type > () = yystack_[0].value.as< IShader::Type > ();}
#line 706 "/cygdrive/e/code/fromasmtodisasm/TestEngine/src/Engine/Renderer/Shaders/Parser.cpp" // lalr1.cc:859
    break;

  case 22:
#line 329 "/cygdrive/e/code/fromasmtodisasm/TestEngine/src/Engine/Renderer/Shaders/Parser.yy" // lalr1.cc:859
    {
    //$$ = std::make_pair($1, $3);
	driver.currentEffect->shader_assignment(yystack_[2].value.as< IShader::Type > (),yystack_[0].value.as< std::string > ());
}
#line 715 "/cygdrive/e/code/fromasmtodisasm/TestEngine/src/Engine/Renderer/Shaders/Parser.cpp" // lalr1.cc:859
    break;

  case 25:
#line 337 "/cygdrive/e/code/fromasmtodisasm/TestEngine/src/Engine/Renderer/Shaders/Parser.yy" // lalr1.cc:859
    { error(yystack_[1].location, "Error in shader_assignment list\n");}
#line 721 "/cygdrive/e/code/fromasmtodisasm/TestEngine/src/Engine/Renderer/Shaders/Parser.cpp" // lalr1.cc:859
    break;

  case 26:
#line 344 "/cygdrive/e/code/fromasmtodisasm/TestEngine/src/Engine/Renderer/Shaders/Parser.yy" // lalr1.cc:859
    { CryLog("Hi"); }
#line 727 "/cygdrive/e/code/fromasmtodisasm/TestEngine/src/Engine/Renderer/Shaders/Parser.cpp" // lalr1.cc:859
    break;

  case 29:
#line 349 "/cygdrive/e/code/fromasmtodisasm/TestEngine/src/Engine/Renderer/Shaders/Parser.yy" // lalr1.cc:859
    {
    { CryLog("slkfjlkj123123123123"); }
    lex_pop_state();
}
#line 736 "/cygdrive/e/code/fromasmtodisasm/TestEngine/src/Engine/Renderer/Shaders/Parser.cpp" // lalr1.cc:859
    break;

  case 32:
#line 355 "/cygdrive/e/code/fromasmtodisasm/TestEngine/src/Engine/Renderer/Shaders/Parser.yy" // lalr1.cc:859
    {
    yylhs.value.as< std::string > () = "layout(location = " + location_from_semantic(yystack_[0].value.as< std::string > ()) + ") in " + nvFX::toString(yystack_[2].value.as< nvFX::IUniform::Type > ()) + " " + yystack_[1].value.as< std::string > () + ";";
    driver.currentEffect->m_Techniques.back().Passes.back().InputLayout.push_back(yylhs.value.as< std::string > ());
}
#line 745 "/cygdrive/e/code/fromasmtodisasm/TestEngine/src/Engine/Renderer/Shaders/Parser.cpp" // lalr1.cc:859
    break;

  case 33:
#line 362 "/cygdrive/e/code/fromasmtodisasm/TestEngine/src/Engine/Renderer/Shaders/Parser.yy" // lalr1.cc:859
    { yylhs.value.as< nvFX::IUniform::Type > () = nvFX::IUniform::TFloat; }
#line 751 "/cygdrive/e/code/fromasmtodisasm/TestEngine/src/Engine/Renderer/Shaders/Parser.cpp" // lalr1.cc:859
    break;

  case 34:
#line 363 "/cygdrive/e/code/fromasmtodisasm/TestEngine/src/Engine/Renderer/Shaders/Parser.yy" // lalr1.cc:859
    { yylhs.value.as< nvFX::IUniform::Type > () = nvFX::IUniform::TVec2; }
#line 757 "/cygdrive/e/code/fromasmtodisasm/TestEngine/src/Engine/Renderer/Shaders/Parser.cpp" // lalr1.cc:859
    break;

  case 35:
#line 364 "/cygdrive/e/code/fromasmtodisasm/TestEngine/src/Engine/Renderer/Shaders/Parser.yy" // lalr1.cc:859
    { yylhs.value.as< nvFX::IUniform::Type > () = nvFX::IUniform::TVec3; }
#line 763 "/cygdrive/e/code/fromasmtodisasm/TestEngine/src/Engine/Renderer/Shaders/Parser.cpp" // lalr1.cc:859
    break;

  case 36:
#line 365 "/cygdrive/e/code/fromasmtodisasm/TestEngine/src/Engine/Renderer/Shaders/Parser.yy" // lalr1.cc:859
    { yylhs.value.as< nvFX::IUniform::Type > () = nvFX::IUniform::TVec4; }
#line 769 "/cygdrive/e/code/fromasmtodisasm/TestEngine/src/Engine/Renderer/Shaders/Parser.cpp" // lalr1.cc:859
    break;

  case 37:
#line 366 "/cygdrive/e/code/fromasmtodisasm/TestEngine/src/Engine/Renderer/Shaders/Parser.yy" // lalr1.cc:859
    { yylhs.value.as< nvFX::IUniform::Type > () = nvFX::IUniform::TInt; }
#line 775 "/cygdrive/e/code/fromasmtodisasm/TestEngine/src/Engine/Renderer/Shaders/Parser.cpp" // lalr1.cc:859
    break;

  case 38:
#line 367 "/cygdrive/e/code/fromasmtodisasm/TestEngine/src/Engine/Renderer/Shaders/Parser.yy" // lalr1.cc:859
    { yylhs.value.as< nvFX::IUniform::Type > () = nvFX::IUniform::TInt2; }
#line 781 "/cygdrive/e/code/fromasmtodisasm/TestEngine/src/Engine/Renderer/Shaders/Parser.cpp" // lalr1.cc:859
    break;

  case 39:
#line 368 "/cygdrive/e/code/fromasmtodisasm/TestEngine/src/Engine/Renderer/Shaders/Parser.yy" // lalr1.cc:859
    { yylhs.value.as< nvFX::IUniform::Type > () = nvFX::IUniform::TInt3; }
#line 787 "/cygdrive/e/code/fromasmtodisasm/TestEngine/src/Engine/Renderer/Shaders/Parser.cpp" // lalr1.cc:859
    break;

  case 40:
#line 369 "/cygdrive/e/code/fromasmtodisasm/TestEngine/src/Engine/Renderer/Shaders/Parser.yy" // lalr1.cc:859
    { yylhs.value.as< nvFX::IUniform::Type > () = nvFX::IUniform::TInt4; }
#line 793 "/cygdrive/e/code/fromasmtodisasm/TestEngine/src/Engine/Renderer/Shaders/Parser.cpp" // lalr1.cc:859
    break;

  case 41:
#line 370 "/cygdrive/e/code/fromasmtodisasm/TestEngine/src/Engine/Renderer/Shaders/Parser.yy" // lalr1.cc:859
    { yylhs.value.as< nvFX::IUniform::Type > () = nvFX::IUniform::TBool; }
#line 799 "/cygdrive/e/code/fromasmtodisasm/TestEngine/src/Engine/Renderer/Shaders/Parser.cpp" // lalr1.cc:859
    break;

  case 42:
#line 371 "/cygdrive/e/code/fromasmtodisasm/TestEngine/src/Engine/Renderer/Shaders/Parser.yy" // lalr1.cc:859
    { yylhs.value.as< nvFX::IUniform::Type > () = nvFX::IUniform::TBool2; }
#line 805 "/cygdrive/e/code/fromasmtodisasm/TestEngine/src/Engine/Renderer/Shaders/Parser.cpp" // lalr1.cc:859
    break;

  case 43:
#line 372 "/cygdrive/e/code/fromasmtodisasm/TestEngine/src/Engine/Renderer/Shaders/Parser.yy" // lalr1.cc:859
    { yylhs.value.as< nvFX::IUniform::Type > () = nvFX::IUniform::TBool3; }
#line 811 "/cygdrive/e/code/fromasmtodisasm/TestEngine/src/Engine/Renderer/Shaders/Parser.cpp" // lalr1.cc:859
    break;

  case 44:
#line 373 "/cygdrive/e/code/fromasmtodisasm/TestEngine/src/Engine/Renderer/Shaders/Parser.yy" // lalr1.cc:859
    { yylhs.value.as< nvFX::IUniform::Type > () = nvFX::IUniform::TBool4; }
#line 817 "/cygdrive/e/code/fromasmtodisasm/TestEngine/src/Engine/Renderer/Shaders/Parser.cpp" // lalr1.cc:859
    break;

  case 45:
#line 374 "/cygdrive/e/code/fromasmtodisasm/TestEngine/src/Engine/Renderer/Shaders/Parser.yy" // lalr1.cc:859
    { yylhs.value.as< nvFX::IUniform::Type > () = nvFX::IUniform::TMat2; }
#line 823 "/cygdrive/e/code/fromasmtodisasm/TestEngine/src/Engine/Renderer/Shaders/Parser.cpp" // lalr1.cc:859
    break;

  case 46:
#line 375 "/cygdrive/e/code/fromasmtodisasm/TestEngine/src/Engine/Renderer/Shaders/Parser.yy" // lalr1.cc:859
    { yylhs.value.as< nvFX::IUniform::Type > () = nvFX::IUniform::TMat3; }
#line 829 "/cygdrive/e/code/fromasmtodisasm/TestEngine/src/Engine/Renderer/Shaders/Parser.cpp" // lalr1.cc:859
    break;

  case 47:
#line 376 "/cygdrive/e/code/fromasmtodisasm/TestEngine/src/Engine/Renderer/Shaders/Parser.yy" // lalr1.cc:859
    { yylhs.value.as< nvFX::IUniform::Type > () = nvFX::IUniform::TMat4; }
#line 835 "/cygdrive/e/code/fromasmtodisasm/TestEngine/src/Engine/Renderer/Shaders/Parser.cpp" // lalr1.cc:859
    break;

  case 49:
#line 382 "/cygdrive/e/code/fromasmtodisasm/TestEngine/src/Engine/Renderer/Shaders/Parser.yy" // lalr1.cc:859
    { yylhs.value.as< std::string > () = yystack_[0].value.as< std::string > (); }
#line 841 "/cygdrive/e/code/fromasmtodisasm/TestEngine/src/Engine/Renderer/Shaders/Parser.cpp" // lalr1.cc:859
    break;

  case 50:
#line 388 "/cygdrive/e/code/fromasmtodisasm/TestEngine/src/Engine/Renderer/Shaders/Parser.yy" // lalr1.cc:859
    { 
    CryLog("Creation of PASS");
    }
#line 849 "/cygdrive/e/code/fromasmtodisasm/TestEngine/src/Engine/Renderer/Shaders/Parser.cpp" // lalr1.cc:859
    break;

  case 51:
#line 391 "/cygdrive/e/code/fromasmtodisasm/TestEngine/src/Engine/Renderer/Shaders/Parser.yy" // lalr1.cc:859
    {
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
#line 865 "/cygdrive/e/code/fromasmtodisasm/TestEngine/src/Engine/Renderer/Shaders/Parser.cpp" // lalr1.cc:859
    break;

  case 52:
#line 402 "/cygdrive/e/code/fromasmtodisasm/TestEngine/src/Engine/Renderer/Shaders/Parser.yy" // lalr1.cc:859
    {
    SPass pass;
    pass.Name = yystack_[0].value.as< std::string > ().c_str();
    pass.CommonCode = CommonCode;
    driver.currentEffect->m_Techniques.back().Passes.push_back(pass);
    //driver.currentEffect->m_shaders.push_back(IEffect::ShaderInfo{$1, $3});
    //driver.currentEffect->m_shaders.push_back(IEffect::ShaderInfo{$1, $3});

    CryLog("Creation of PASS %s\n", pass.Name.data());
    //curPass = curTechnique->addPass($2->c_str())->getExInterface();
    //curAnnotations = curPass->annotations()->getExInterface();
    }
#line 882 "/cygdrive/e/code/fromasmtodisasm/TestEngine/src/Engine/Renderer/Shaders/Parser.cpp" // lalr1.cc:859
    break;

  case 53:
#line 414 "/cygdrive/e/code/fromasmtodisasm/TestEngine/src/Engine/Renderer/Shaders/Parser.yy" // lalr1.cc:859
    {
    //LOGD("Pass %s...\n", $2->c_str() );
    //delete $2;
    //curAnnotations = NULL;
    //curRState = NULL;
    //curCSState = NULL;
    //curDSTState = NULL;
    //curPRState = NULL;
    lex_pop_state();
}
#line 897 "/cygdrive/e/code/fromasmtodisasm/TestEngine/src/Engine/Renderer/Shaders/Parser.cpp" // lalr1.cc:859
    break;

  case 54:
#line 424 "/cygdrive/e/code/fromasmtodisasm/TestEngine/src/Engine/Renderer/Shaders/Parser.yy" // lalr1.cc:859
    { error(yystack_[1].location, "Error in Pass declaration\n");}
#line 903 "/cygdrive/e/code/fromasmtodisasm/TestEngine/src/Engine/Renderer/Shaders/Parser.cpp" // lalr1.cc:859
    break;

  case 57:
#line 431 "/cygdrive/e/code/fromasmtodisasm/TestEngine/src/Engine/Renderer/Shaders/Parser.yy" // lalr1.cc:859
    { error(yystack_[1].location, "Error in Pass list\n");}
#line 909 "/cygdrive/e/code/fromasmtodisasm/TestEngine/src/Engine/Renderer/Shaders/Parser.cpp" // lalr1.cc:859
    break;

  case 58:
#line 437 "/cygdrive/e/code/fromasmtodisasm/TestEngine/src/Engine/Renderer/Shaders/Parser.yy" // lalr1.cc:859
    {
    CryLog("Creation of Technique for NO name\n");
    //curTechnique = curContainer->createTechnique()->getExInterface();
    //curAnnotations = curTechnique->annotations()->getExInterface();
}
#line 919 "/cygdrive/e/code/fromasmtodisasm/TestEngine/src/Engine/Renderer/Shaders/Parser.cpp" // lalr1.cc:859
    break;

  case 60:
#line 442 "/cygdrive/e/code/fromasmtodisasm/TestEngine/src/Engine/Renderer/Shaders/Parser.yy" // lalr1.cc:859
    {
    CTechnique tech;
    tech.Name =  yystack_[0].value.as< std::string > ().c_str();
    driver.currentEffect->m_Techniques.push_back(tech);
    CryLog("creation of Technique %s...\n", tech.Name.data());
    //curTechnique = curContainer->createTechnique($2->c_str())->getExInterface();
    //curAnnotations = curTechnique->annotations()->getExInterface();
    //delete $2;
}
#line 933 "/cygdrive/e/code/fromasmtodisasm/TestEngine/src/Engine/Renderer/Shaders/Parser.cpp" // lalr1.cc:859
    break;

  case 61:
#line 450 "/cygdrive/e/code/fromasmtodisasm/TestEngine/src/Engine/Renderer/Shaders/Parser.yy" // lalr1.cc:859
    { 
    lex_pop_state();
    //curAnnotations = NULL;
}
#line 942 "/cygdrive/e/code/fromasmtodisasm/TestEngine/src/Engine/Renderer/Shaders/Parser.cpp" // lalr1.cc:859
    break;

  case 63:
#line 460 "/cygdrive/e/code/fromasmtodisasm/TestEngine/src/Engine/Renderer/Shaders/Parser.yy" // lalr1.cc:859
    {
/*
    if(!curAnnotations)
        curAnnotations = IAnnotationEx::getAnnotationSingleton(2); // need a temporary place since nothing was initialized
    if(!curAnnotations->addAnnotation($2->c_str(), $4->c_str()))
        yyerror("err\n");
    delete $4;
*/
    }
#line 956 "/cygdrive/e/code/fromasmtodisasm/TestEngine/src/Engine/Renderer/Shaders/Parser.cpp" // lalr1.cc:859
    break;

  case 64:
#line 470 "/cygdrive/e/code/fromasmtodisasm/TestEngine/src/Engine/Renderer/Shaders/Parser.yy" // lalr1.cc:859
    {CryLog("Begin annotations"); }
#line 962 "/cygdrive/e/code/fromasmtodisasm/TestEngine/src/Engine/Renderer/Shaders/Parser.cpp" // lalr1.cc:859
    break;

  case 68:
#line 477 "/cygdrive/e/code/fromasmtodisasm/TestEngine/src/Engine/Renderer/Shaders/Parser.yy" // lalr1.cc:859
    {
        yylhs.value.as< std::string > () = yystack_[0].value.as< std::string > (); 
    }
#line 970 "/cygdrive/e/code/fromasmtodisasm/TestEngine/src/Engine/Renderer/Shaders/Parser.cpp" // lalr1.cc:859
    break;

  case 69:
#line 480 "/cygdrive/e/code/fromasmtodisasm/TestEngine/src/Engine/Renderer/Shaders/Parser.yy" // lalr1.cc:859
    {
        yylhs.value.as< std::string > () = "Common";
        is_common = true;
    }
#line 979 "/cygdrive/e/code/fromasmtodisasm/TestEngine/src/Engine/Renderer/Shaders/Parser.cpp" // lalr1.cc:859
    break;

  case 70:
#line 486 "/cygdrive/e/code/fromasmtodisasm/TestEngine/src/Engine/Renderer/Shaders/Parser.yy" // lalr1.cc:859
    { 
		//gEnv->pLog->Log("$3 Shader $1%s $3parsed", $1.data()); 
        driver.currentEffect->m_shaders.push_back(IEffect::ShaderInfo{yystack_[2].value.as< std::string > (), yystack_[0].value.as< std::string > ()});
        if (yystack_[2].value.as< std::string > () == "Common")
            CommonCode.push_back(driver.currentEffect->m_shaders.back().data);
	}
#line 990 "/cygdrive/e/code/fromasmtodisasm/TestEngine/src/Engine/Renderer/Shaders/Parser.cpp" // lalr1.cc:859
    break;

  case 71:
#line 496 "/cygdrive/e/code/fromasmtodisasm/TestEngine/src/Engine/Renderer/Shaders/Parser.yy" // lalr1.cc:859
    {
    gEnv->pLog->Log(
    "$3 New vertex format <%s> with field %s (%s = %d)", 
    yystack_[6].value.as< std::string > ().data(), yystack_[4].value.as< std::string > ().data(), "vkFormat", 10);
}
#line 1000 "/cygdrive/e/code/fromasmtodisasm/TestEngine/src/Engine/Renderer/Shaders/Parser.cpp" // lalr1.cc:859
    break;

  case 72:
#line 503 "/cygdrive/e/code/fromasmtodisasm/TestEngine/src/Engine/Renderer/Shaders/Parser.yy" // lalr1.cc:859
    { 
      //$$ = VkFormat(10); 
      CryLog("format not implemented");

      }
#line 1010 "/cygdrive/e/code/fromasmtodisasm/TestEngine/src/Engine/Renderer/Shaders/Parser.cpp" // lalr1.cc:859
    break;


#line 1014 "/cygdrive/e/code/fromasmtodisasm/TestEngine/src/Engine/Renderer/Shaders/Parser.cpp" // lalr1.cc:859
            default:
              break;
            }
        }
      catch (const syntax_error& yyexc)
        {
          error (yyexc);
          YYERROR;
        }
      YY_SYMBOL_PRINT ("-> $$ =", yylhs);
      yypop_ (yylen);
      yylen = 0;
      YY_STACK_PRINT ();

      // Shift the result of the reduction.
      yypush_ (YY_NULLPTR, yylhs);
    }
    goto yynewstate;

  /*--------------------------------------.
  | yyerrlab -- here on detecting error.  |
  `--------------------------------------*/
  yyerrlab:
    // If not already recovering from an error, report this error.
    if (!yyerrstatus_)
      {
        ++yynerrs_;
        error (yyla.location, yysyntax_error_ (yystack_[0].state, yyla));
      }


    yyerror_range[1].location = yyla.location;
    if (yyerrstatus_ == 3)
      {
        /* If just tried and failed to reuse lookahead token after an
           error, discard it.  */

        // Return failure if at end of input.
        if (yyla.type_get () == yyeof_)
          YYABORT;
        else if (!yyla.empty ())
          {
            yy_destroy_ ("Error: discarding", yyla);
            yyla.clear ();
          }
      }

    // Else will try to reuse lookahead token after shifting the error token.
    goto yyerrlab1;


  /*---------------------------------------------------.
  | yyerrorlab -- error raised explicitly by YYERROR.  |
  `---------------------------------------------------*/
  yyerrorlab:

    /* Pacify compilers like GCC when the user code never invokes
       YYERROR and the label yyerrorlab therefore never appears in user
       code.  */
    if (false)
      goto yyerrorlab;
    yyerror_range[1].location = yystack_[yylen - 1].location;
    /* Do not reclaim the symbols of the rule whose action triggered
       this YYERROR.  */
    yypop_ (yylen);
    yylen = 0;
    goto yyerrlab1;

  /*-------------------------------------------------------------.
  | yyerrlab1 -- common code for both syntax error and YYERROR.  |
  `-------------------------------------------------------------*/
  yyerrlab1:
    yyerrstatus_ = 3;   // Each real token shifted decrements this.
    {
      stack_symbol_type error_token;
      for (;;)
        {
          yyn = yypact_[yystack_[0].state];
          if (!yy_pact_value_is_default_ (yyn))
            {
              yyn += yyterror_;
              if (0 <= yyn && yyn <= yylast_ && yycheck_[yyn] == yyterror_)
                {
                  yyn = yytable_[yyn];
                  if (0 < yyn)
                    break;
                }
            }

          // Pop the current state because it cannot handle the error token.
          if (yystack_.size () == 1)
            YYABORT;

          yyerror_range[1].location = yystack_[0].location;
          yy_destroy_ ("Error: popping", yystack_[0]);
          yypop_ ();
          YY_STACK_PRINT ();
        }

      yyerror_range[2].location = yyla.location;
      YYLLOC_DEFAULT (error_token.location, yyerror_range, 2);

      // Shift the error token.
      error_token.state = yyn;
      yypush_ ("Shifting", error_token);
    }
    goto yynewstate;

    // Accept.
  yyacceptlab:
    yyresult = 0;
    goto yyreturn;

    // Abort.
  yyabortlab:
    yyresult = 1;
    goto yyreturn;

  yyreturn:
    if (!yyla.empty ())
      yy_destroy_ ("Cleanup: discarding lookahead", yyla);

    /* Do not reclaim the symbols of the rule whose action triggered
       this YYABORT or YYACCEPT.  */
    yypop_ (yylen);
    while (1 < yystack_.size ())
      {
        yy_destroy_ ("Cleanup: popping", yystack_[0]);
        yypop_ ();
      }

    return yyresult;
  }
    catch (...)
      {
        YYCDEBUG << "Exception caught: cleaning lookahead and stack"
                 << std::endl;
        // Do not try to display the values of the reclaimed symbols,
        // as their printer might throw an exception.
        if (!yyla.empty ())
          yy_destroy_ (YY_NULLPTR, yyla);

        while (1 < yystack_.size ())
          {
            yy_destroy_ (YY_NULLPTR, yystack_[0]);
            yypop_ ();
          }
        throw;
      }
  }

  void
  parser::error (const syntax_error& yyexc)
  {
    error (yyexc.location, yyexc.what());
  }

  // Generate an error message.
  std::string
  parser::yysyntax_error_ (state_type yystate, const symbol_type& yyla) const
  {
    // Number of reported tokens (one for the "unexpected", one per
    // "expected").
    size_t yycount = 0;
    // Its maximum.
    enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
    // Arguments of yyformat.
    char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];

    /* There are many possibilities here to consider:
       - If this state is a consistent state with a default action, then
         the only way this function was invoked is if the default action
         is an error action.  In that case, don't check for expected
         tokens because there are none.
       - The only way there can be no lookahead present (in yyla) is
         if this state is a consistent state with a default action.
         Thus, detecting the absence of a lookahead is sufficient to
         determine that there is no unexpected or expected token to
         report.  In that case, just report a simple "syntax error".
       - Don't assume there isn't a lookahead just because this state is
         a consistent state with a default action.  There might have
         been a previous inconsistent state, consistent state with a
         non-default action, or user semantic action that manipulated
         yyla.  (However, yyla is currently not documented for users.)
       - Of course, the expected token list depends on states to have
         correct lookahead information, and it depends on the parser not
         to perform extra reductions after fetching a lookahead from the
         scanner and before detecting a syntax error.  Thus, state
         merging (from LALR or IELR) and default reductions corrupt the
         expected token list.  However, the list is correct for
         canonical LR with one exception: it will still contain any
         token that will not be accepted due to an error action in a
         later state.
    */
    if (!yyla.empty ())
      {
        int yytoken = yyla.type_get ();
        yyarg[yycount++] = yytname_[yytoken];
        int yyn = yypact_[yystate];
        if (!yy_pact_value_is_default_ (yyn))
          {
            /* Start YYX at -YYN if negative to avoid negative indexes in
               YYCHECK.  In other words, skip the first -YYN actions for
               this state because they are default actions.  */
            int yyxbegin = yyn < 0 ? -yyn : 0;
            // Stay within bounds of both yycheck and yytname.
            int yychecklim = yylast_ - yyn + 1;
            int yyxend = yychecklim < yyntokens_ ? yychecklim : yyntokens_;
            for (int yyx = yyxbegin; yyx < yyxend; ++yyx)
              if (yycheck_[yyx + yyn] == yyx && yyx != yyterror_
                  && !yy_table_value_is_error_ (yytable_[yyx + yyn]))
                {
                  if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
                    {
                      yycount = 1;
                      break;
                    }
                  else
                    yyarg[yycount++] = yytname_[yyx];
                }
          }
      }

    char const* yyformat = YY_NULLPTR;
    switch (yycount)
      {
#define YYCASE_(N, S)                         \
        case N:                               \
          yyformat = S;                       \
        break
        YYCASE_(0, YY_("syntax error"));
        YYCASE_(1, YY_("syntax error, unexpected %s"));
        YYCASE_(2, YY_("syntax error, unexpected %s, expecting %s"));
        YYCASE_(3, YY_("syntax error, unexpected %s, expecting %s or %s"));
        YYCASE_(4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
        YYCASE_(5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
#undef YYCASE_
      }

    std::string yyres;
    // Argument number.
    size_t yyi = 0;
    for (char const* yyp = yyformat; *yyp; ++yyp)
      if (yyp[0] == '%' && yyp[1] == 's' && yyi < yycount)
        {
          yyres += yytnamerr_ (yyarg[yyi++]);
          ++yyp;
        }
      else
        yyres += *yyp;
    return yyres;
  }


  const signed char parser::yypact_ninf_ = -86;

  const signed char parser::yytable_ninf_ = -57;

  const signed char
  parser::yypact_[] =
  {
     -85,   -86,     0,   -86,     5,     6,     9,   -86,   -86,   -86,
     -86,   -86,   -86,   -86,   -86,   -86,   -86,   -86,   -86,    20,
     -86,   -83,   -86,   -86,   -86,   -86,   -86,   -86,   -86,   -86,
     -86,   -86,   -86,   -86,   -86,   -86,   -86,   -86,   -76,   -73,
     -86,    -3,   -72,     7,    22,   -86,   -86,   -86,   -63,    25,
       3,    21,   -59,   -86,     7,   -86,   -72,   -86,   -86,   -86,
      23,   -86,    -2,    36,   -72,   -57,   -54,   -53,   -86,   -86,
     -55,   -86,   -52,    27,   -86,   -18,   -86,   -47,   -15,   -45,
     -86,   -86,   -86,   -86,   -43,   -86,     2,   -86,   -86,   -86,
     105,    39,   -86,   -86,   -86,   -86,   -86,   -86,   -86,   -86,
     -86,   -86,   -86,   -86,   -86,   -86,   -86,   -86,   -86,    90,
     -86,    43,   -86,   -86,   -86,   -44,    51,   -86,   -86
  };

  const unsigned char
  parser::yydefact_[] =
  {
       2,     3,     2,     1,    69,    58,     0,    73,    74,    75,
      76,    77,    78,    79,    80,    81,    82,    83,    84,     0,
       4,     0,     5,    18,     6,     7,     8,     9,    10,    11,
      12,    13,    14,    15,    16,    17,    68,    60,     0,     0,
      85,     0,    66,     0,     0,    70,    64,    67,     0,    50,
       0,     0,     0,    62,     0,    52,    66,    54,    57,    59,
       0,    72,     0,     0,    66,     0,     0,     0,    65,    61,
       0,    26,     0,     0,    26,     0,    71,     0,     0,     0,
      19,    21,    20,    51,     0,    23,     0,    28,    63,    53,
       0,     0,    25,    24,    33,    34,    35,    36,    45,    46,
      47,    41,    42,    43,    44,    37,    38,    39,    40,     0,
      30,     0,    22,    29,    31,    48,     0,    32,    49
  };

  const signed char
  parser::yypgoto_[] =
  {
     -86,    55,   -86,   -29,   -86,   -16,   -86,   -86,   -50,   -86,
     -86,   -46,   -86,   -86,     8,   -86,   -86,   -86,   -86,   -86,
     -86,   -49,   -86,   -86,   -86,   -86,   -86,   -86,   -86,   -86,
     -86,   -86,   -86,   -86,   -86,   -86,   -86,   -86
  };

  const signed char
  parser::yydefgoto_[] =
  {
      -1,    19,    84,    85,    86,    75,    87,   109,   110,   111,
     117,    50,    56,    64,    51,    20,    38,    42,    62,    47,
      53,    48,    21,    22,    23,    66,    24,    25,    26,    27,
      28,    29,    30,    31,    32,    33,    34,    35
  };

  const signed char
  parser::yytable_[] =
  {
       3,    67,     1,    92,    57,    60,    41,    65,    36,    37,
       4,    79,    39,    43,    79,    70,    44,    60,    45,     5,
      46,    40,    58,   -55,    57,    52,    54,    49,    55,    61,
       4,   -27,    71,    72,    74,    73,    77,    58,    76,     5,
      88,    49,   112,   -56,    90,    91,   115,   116,    80,    81,
      82,    80,    81,    82,   118,     2,    49,    93,    78,   114,
       0,     0,    63,     0,     0,     0,     0,     0,    80,    81,
      82,     0,    83,     0,     0,    89,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     6,     1,     0,     0,
       0,    68,   -27,   -55,     7,     8,     9,    10,    11,    12,
      13,    14,    15,    16,    17,    18,     6,     1,     0,     0,
       0,    59,     0,   -56,     7,     8,     9,    10,    11,    12,
      13,    14,    15,    16,    17,    18,    69,    94,    95,    96,
      97,    98,    99,   100,   101,   102,   103,   104,   105,   106,
     107,   108,    94,    95,    96,    97,    98,    99,   100,   101,
     102,   103,   104,   105,   106,   107,   108,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     113
  };

  const signed char
  parser::yycheck_[] =
  {
       0,     3,    87,     1,     1,    51,    89,    56,     3,     3,
      10,    29,     3,    89,    29,    64,    89,    63,    21,    19,
      92,     1,     1,    20,     1,     3,    89,    20,     3,    88,
      10,    29,    89,    87,    89,    88,     9,     1,    90,    19,
      87,    20,     3,    20,    89,    88,     3,    91,    66,    67,
      68,    66,    67,    68,     3,     0,    20,    86,    74,   109,
      -1,    -1,    54,    -1,    -1,    -1,    -1,    -1,    66,    67,
      68,    -1,    90,    -1,    -1,    90,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    86,    87,    -1,    -1,
      -1,    93,    90,    90,    94,    95,    96,    97,    98,    99,
     100,   101,   102,   103,   104,   105,    86,    87,    -1,    -1,
      -1,    90,    -1,    90,    94,    95,    96,    97,    98,    99,
     100,   101,   102,   103,   104,   105,    90,    37,    38,    39,
      40,    41,    42,    43,    44,    45,    46,    47,    48,    49,
      50,    51,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      90
  };

  const unsigned char
  parser::yystos_[] =
  {
       0,    87,   107,     0,    10,    19,    86,    94,    95,    96,
      97,    98,    99,   100,   101,   102,   103,   104,   105,   107,
     121,   128,   129,   130,   132,   133,   134,   135,   136,   137,
     138,   139,   140,   141,   142,   143,     3,     3,   122,     3,
       1,    89,   123,    89,    89,    21,    92,   125,   127,    20,
     117,   120,     3,   126,    89,     3,   118,     1,     1,    90,
     117,    88,   124,   120,   119,   127,   131,     3,    93,    90,
     127,    89,    87,    88,    89,   111,    90,     9,   111,    29,
      66,    67,    68,    90,   108,   109,   110,   112,    87,    90,
      89,    88,     1,   109,    37,    38,    39,    40,    41,    42,
      43,    44,    45,    46,    47,    48,    49,    50,    51,   113,
     114,   115,     3,    90,   114,     3,    91,   116,     3
  };

  const unsigned char
  parser::yyr1_[] =
  {
       0,   106,   107,   107,   107,   107,   107,   107,   107,   107,
     107,   107,   107,   107,   107,   107,   107,   107,   107,   108,
     108,   108,   109,   110,   110,   110,   111,   111,   111,   112,
     113,   113,   114,   115,   115,   115,   115,   115,   115,   115,
     115,   115,   115,   115,   115,   115,   115,   115,   116,   116,
     118,   117,   119,   117,   117,   120,   120,   120,   122,   121,
     123,   121,   124,   124,   126,   125,   127,   127,   128,   128,
     129,   130,   131,   132,   133,   134,   135,   136,   137,   138,
     139,   140,   141,   142,   143,   143
  };

  const unsigned char
  parser::yyr2_[] =
  {
       0,     2,     0,     1,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     1,
       1,     1,     3,     1,     2,     2,     0,     2,     2,     4,
       1,     2,     3,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     0,     2,
       0,     6,     0,     7,     2,     1,     2,     2,     0,     5,
       0,     7,     0,     5,     0,     4,     0,     1,     2,     1,
       3,     8,     0,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     2
  };



  // YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
  // First, the terminals, then, starting at \a yyntokens_, nonterminals.
  const char*
  const parser::yytname_[] =
  {
  "\"end of file\"", "error", "$undefined", "IDENTIFIER", "TRUE", "FALSE",
  "FLOAT", "INT", "BOOL", "STR", "GLSLSHADER", "HLSL10SHADER",
  "HLSL11SHADER", "CGSHADER", "SAMPLER_STATE", "DST_STATE", "PR_STATE",
  "COLOR_SAMPLE_STATE", "RASTERIZATION_STATE", "TECHNIQUE", "PASS",
  "CODEBODY", "VARNAME", "TEXTURERESOURCE", "TEXTURERESOURCE1D",
  "TEXTURERESOURCE2D", "TEXTURERESOURCE3D", "TEXTURERESOURCERECT",
  "TEXTURERESOURCECUBE", "INPUTLAYOUT", "VOID_TYPE", "UNSIGNED", "HIGHP",
  "MEDIUMP", "LOWP", "UNIFORM", "CSTBUFFER", "FLOAT_TYPE", "FLOAT2_TYPE",
  "FLOAT3_TYPE", "FLOAT4_TYPE", "MAT2_TYPE", "MAT3_TYPE", "MAT4_TYPE",
  "BOOL_TYPE", "BOOL2_TYPE", "BOOL3_TYPE", "BOOL4_TYPE", "INT_TYPE",
  "INT2_TYPE", "INT3_TYPE", "INT4_TYPE", "TEXTURE1D_TYPE",
  "TEXTURE2D_TYPE", "TEXTURE2DSHADOW_TYPE", "TEXTURE2DRECT_TYPE",
  "TEXTURE3D_TYPE", "TEXTURECUBE_TYPE", "SAMPLER1D_TYPE", "SAMPLER2D_TYPE",
  "SAMPLER2DSHADOW_TYPE", "SAMPLER2DRECT_TYPE", "SAMPLER3D_TYPE",
  "SAMPLERCUBE_TYPE", "EXTENSION", "SEPARATE_SHADER", "VERTEXPROGRAM",
  "FRAGMENTPROGRAM", "GEOMETRYPROGRAM", "HULLPROGRAM", "EVALPROGRAM",
  "SHDPROFILE", "SAMPLERRESOURCE", "SAMPLERTEXUNIT", "SETSAMPLERSTATE",
  "SETDSTSTATE", "SETRASTERIZATIONSTATE", "SETCOLORSAMPLESTATE",
  "IMAGERESOURCE", "IMAGEUNIT", "IMAGEACCESS", "IMAGELAYER",
  "IMAGELAYERED", "WRITE_ONLY", "READ_ONLY", "READ_WRITE", "VERTEXFORMAT",
  "';'", "'='", "'{'", "'}'", "':'", "'<'", "'>'", "\"hlsl\"",
  "\"texture\"", "\"resource\"", "\"fbo\"", "\"sstate\"", "\"dststate\"",
  "\"prstate\"", "\"csstate\"", "\"rstate\"", "\"namespace\"",
  "\"cstbuffer\"", "\"uniforms\"", "$accept", "input", "shader_type",
  "shader_assignment", "shader_assignments", "passstates", "input_layout",
  "var_decls", "var_decl", "base_type", "semantic", "pass", "$@1", "$@2",
  "passes", "tech", "$@3", "$@4", "annotation", "annotations2", "$@5",
  "annotations", "glsl_header", "glsl", "vertexformat", "format", "hlsl",
  "texture", "resource", "fbo", "sstate", "dststate", "prstate", "csstate",
  "rstate", "namespace", "cstbuffer", "uniforms", YY_NULLPTR
  };

#if YYDEBUG
  const unsigned short int
  parser::yyrline_[] =
  {
       0,   303,   303,   304,   305,   306,   307,   308,   309,   310,
     311,   312,   313,   314,   315,   316,   317,   318,   319,   324,
     325,   326,   329,   335,   336,   337,   344,   345,   346,   349,
     354,   354,   355,   362,   363,   364,   365,   366,   367,   368,
     369,   370,   371,   372,   373,   374,   375,   376,   381,   382,
     388,   388,   402,   402,   424,   429,   430,   431,   437,   437,
     442,   442,   459,   460,   470,   470,   472,   473,   477,   480,
     486,   495,   503,   510,   511,   512,   513,   514,   515,   516,
     517,   518,   519,   520,   521,   522
  };

  // Print the state stack on the debug stream.
  void
  parser::yystack_print_ ()
  {
    *yycdebug_ << "Stack now";
    for (stack_type::const_iterator
           i = yystack_.begin (),
           i_end = yystack_.end ();
         i != i_end; ++i)
      *yycdebug_ << ' ' << i->state;
    *yycdebug_ << std::endl;
  }

  // Report on the debug stream that the rule \a yyrule is going to be reduced.
  void
  parser::yy_reduce_print_ (int yyrule)
  {
    unsigned int yylno = yyrline_[yyrule];
    int yynrhs = yyr2_[yyrule];
    // Print the symbols being reduced, and their result.
    *yycdebug_ << "Reducing stack by rule " << yyrule - 1
               << " (line " << yylno << "):" << std::endl;
    // The symbols being reduced.
    for (int yyi = 0; yyi < yynrhs; yyi++)
      YY_SYMBOL_PRINT ("   $" << yyi + 1 << " =",
                       yystack_[(yynrhs) - (yyi + 1)]);
  }
#endif // YYDEBUG



} // yy
#line 1504 "/cygdrive/e/code/fromasmtodisasm/TestEngine/src/Engine/Renderer/Shaders/Parser.cpp" // lalr1.cc:1167
#line 525 "/cygdrive/e/code/fromasmtodisasm/TestEngine/src/Engine/Renderer/Shaders/Parser.yy" // lalr1.cc:1168


#include <sstream>
void
yy::parser::error(const location_type& l, const std::string& m)
{
  std::stringstream ss;
  ss << l << ": " << m;
  gEnv->pLog->LogError(ss.str().c_str());    
}
#pragma warning(pop)
