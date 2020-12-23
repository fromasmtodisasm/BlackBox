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
#line 35 "/cygdrive/c/Users/chiap/source/repos/fromasmtodisasm/TestEngine/src/Engine/Renderer/Shaders/Parser.yy" // lalr1.cc:397

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

#line 66 "/cygdrive/c/Users/chiap/source/repos/fromasmtodisasm/TestEngine/src/Engine/Renderer/Shaders/Parser.cpp" // lalr1.cc:397


// First part of user declarations.

#line 71 "/cygdrive/c/Users/chiap/source/repos/fromasmtodisasm/TestEngine/src/Engine/Renderer/Shaders/Parser.cpp" // lalr1.cc:404

# ifndef YY_NULLPTR
#  if defined __cplusplus && 201103L <= __cplusplus
#   define YY_NULLPTR nullptr
#  else
#   define YY_NULLPTR 0
#  endif
# endif

#include "Parser.hpp"

// User implementation prologue.

#line 85 "/cygdrive/c/Users/chiap/source/repos/fromasmtodisasm/TestEngine/src/Engine/Renderer/Shaders/Parser.cpp" // lalr1.cc:412


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
#line 171 "/cygdrive/c/Users/chiap/source/repos/fromasmtodisasm/TestEngine/src/Engine/Renderer/Shaders/Parser.cpp" // lalr1.cc:479

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
      case 65: // VERTEXPROGRAM
      case 66: // FRAGMENTPROGRAM
      case 67: // GEOMETRYPROGRAM
      case 68: // HULLPROGRAM
      case 69: // EVALPROGRAM
      case 106: // shader_type
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

      case 3: // IDENTIFIER
      case 9: // STR
      case 21: // CODEBODY
      case 22: // VARNAME
      case 107: // shader_assignment
      case 121: // glsl_header
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
      case 65: // VERTEXPROGRAM
      case 66: // FRAGMENTPROGRAM
      case 67: // GEOMETRYPROGRAM
      case 68: // HULLPROGRAM
      case 69: // EVALPROGRAM
      case 106: // shader_type
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

      case 3: // IDENTIFIER
      case 9: // STR
      case 21: // CODEBODY
      case 22: // VARNAME
      case 107: // shader_assignment
      case 121: // glsl_header
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
    #line 26 "/cygdrive/c/Users/chiap/source/repos/fromasmtodisasm/TestEngine/src/Engine/Renderer/Shaders/Parser.yy" // lalr1.cc:741
{
    is_common = false;
}

#line 509 "/cygdrive/c/Users/chiap/source/repos/fromasmtodisasm/TestEngine/src/Engine/Renderer/Shaders/Parser.cpp" // lalr1.cc:741

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
      case 65: // VERTEXPROGRAM
      case 66: // FRAGMENTPROGRAM
      case 67: // GEOMETRYPROGRAM
      case 68: // HULLPROGRAM
      case 69: // EVALPROGRAM
      case 106: // shader_type
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

      case 3: // IDENTIFIER
      case 9: // STR
      case 21: // CODEBODY
      case 22: // VARNAME
      case 107: // shader_assignment
      case 121: // glsl_header
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
#line 203 "/cygdrive/c/Users/chiap/source/repos/fromasmtodisasm/TestEngine/src/Engine/Renderer/Shaders/Parser.yy" // lalr1.cc:859
    { gEnv->pLog->LogWarning("Empty effect"); }
#line 651 "/cygdrive/c/Users/chiap/source/repos/fromasmtodisasm/TestEngine/src/Engine/Renderer/Shaders/Parser.cpp" // lalr1.cc:859
    break;

  case 19:
#line 224 "/cygdrive/c/Users/chiap/source/repos/fromasmtodisasm/TestEngine/src/Engine/Renderer/Shaders/Parser.yy" // lalr1.cc:859
    {yylhs.value.as< IShader::Type > () = yystack_[0].value.as< IShader::Type > ();}
#line 657 "/cygdrive/c/Users/chiap/source/repos/fromasmtodisasm/TestEngine/src/Engine/Renderer/Shaders/Parser.cpp" // lalr1.cc:859
    break;

  case 20:
#line 225 "/cygdrive/c/Users/chiap/source/repos/fromasmtodisasm/TestEngine/src/Engine/Renderer/Shaders/Parser.yy" // lalr1.cc:859
    {yylhs.value.as< IShader::Type > () = yystack_[0].value.as< IShader::Type > ();}
#line 663 "/cygdrive/c/Users/chiap/source/repos/fromasmtodisasm/TestEngine/src/Engine/Renderer/Shaders/Parser.cpp" // lalr1.cc:859
    break;

  case 21:
#line 226 "/cygdrive/c/Users/chiap/source/repos/fromasmtodisasm/TestEngine/src/Engine/Renderer/Shaders/Parser.yy" // lalr1.cc:859
    {yylhs.value.as< IShader::Type > () = yystack_[0].value.as< IShader::Type > ();}
#line 669 "/cygdrive/c/Users/chiap/source/repos/fromasmtodisasm/TestEngine/src/Engine/Renderer/Shaders/Parser.cpp" // lalr1.cc:859
    break;

  case 22:
#line 229 "/cygdrive/c/Users/chiap/source/repos/fromasmtodisasm/TestEngine/src/Engine/Renderer/Shaders/Parser.yy" // lalr1.cc:859
    {
    //$$ = std::make_pair($1, $3);
	driver.currentEffect->shader_assignment(yystack_[2].value.as< IShader::Type > (),yystack_[0].value.as< std::string > ());
}
#line 678 "/cygdrive/c/Users/chiap/source/repos/fromasmtodisasm/TestEngine/src/Engine/Renderer/Shaders/Parser.cpp" // lalr1.cc:859
    break;

  case 25:
#line 237 "/cygdrive/c/Users/chiap/source/repos/fromasmtodisasm/TestEngine/src/Engine/Renderer/Shaders/Parser.yy" // lalr1.cc:859
    { error(yystack_[1].location, "Error in shader_assignment list\n");}
#line 684 "/cygdrive/c/Users/chiap/source/repos/fromasmtodisasm/TestEngine/src/Engine/Renderer/Shaders/Parser.cpp" // lalr1.cc:859
    break;

  case 27:
#line 251 "/cygdrive/c/Users/chiap/source/repos/fromasmtodisasm/TestEngine/src/Engine/Renderer/Shaders/Parser.yy" // lalr1.cc:859
    { 
    CryLog("Creation of PASS");
    }
#line 692 "/cygdrive/c/Users/chiap/source/repos/fromasmtodisasm/TestEngine/src/Engine/Renderer/Shaders/Parser.cpp" // lalr1.cc:859
    break;

  case 28:
#line 254 "/cygdrive/c/Users/chiap/source/repos/fromasmtodisasm/TestEngine/src/Engine/Renderer/Shaders/Parser.yy" // lalr1.cc:859
    {
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
#line 708 "/cygdrive/c/Users/chiap/source/repos/fromasmtodisasm/TestEngine/src/Engine/Renderer/Shaders/Parser.cpp" // lalr1.cc:859
    break;

  case 29:
#line 265 "/cygdrive/c/Users/chiap/source/repos/fromasmtodisasm/TestEngine/src/Engine/Renderer/Shaders/Parser.yy" // lalr1.cc:859
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
#line 725 "/cygdrive/c/Users/chiap/source/repos/fromasmtodisasm/TestEngine/src/Engine/Renderer/Shaders/Parser.cpp" // lalr1.cc:859
    break;

  case 30:
#line 277 "/cygdrive/c/Users/chiap/source/repos/fromasmtodisasm/TestEngine/src/Engine/Renderer/Shaders/Parser.yy" // lalr1.cc:859
    {
    //LOGD("Pass %s...\n", $2->c_str() );
    //delete $2;
    //curAnnotations = NULL;
    //curRState = NULL;
    //curCSState = NULL;
    //curDSTState = NULL;
    //curPRState = NULL;
    //lex_pop_state();
}
#line 740 "/cygdrive/c/Users/chiap/source/repos/fromasmtodisasm/TestEngine/src/Engine/Renderer/Shaders/Parser.cpp" // lalr1.cc:859
    break;

  case 31:
#line 287 "/cygdrive/c/Users/chiap/source/repos/fromasmtodisasm/TestEngine/src/Engine/Renderer/Shaders/Parser.yy" // lalr1.cc:859
    { error(yystack_[1].location, "Error in Pass declaration\n");}
#line 746 "/cygdrive/c/Users/chiap/source/repos/fromasmtodisasm/TestEngine/src/Engine/Renderer/Shaders/Parser.cpp" // lalr1.cc:859
    break;

  case 34:
#line 294 "/cygdrive/c/Users/chiap/source/repos/fromasmtodisasm/TestEngine/src/Engine/Renderer/Shaders/Parser.yy" // lalr1.cc:859
    { error(yystack_[1].location, "Error in Pass list\n");}
#line 752 "/cygdrive/c/Users/chiap/source/repos/fromasmtodisasm/TestEngine/src/Engine/Renderer/Shaders/Parser.cpp" // lalr1.cc:859
    break;

  case 35:
#line 300 "/cygdrive/c/Users/chiap/source/repos/fromasmtodisasm/TestEngine/src/Engine/Renderer/Shaders/Parser.yy" // lalr1.cc:859
    {
    CryLog("Creation of Technique for NO name\n");
    //curTechnique = curContainer->createTechnique()->getExInterface();
    //curAnnotations = curTechnique->annotations()->getExInterface();
}
#line 762 "/cygdrive/c/Users/chiap/source/repos/fromasmtodisasm/TestEngine/src/Engine/Renderer/Shaders/Parser.cpp" // lalr1.cc:859
    break;

  case 37:
#line 305 "/cygdrive/c/Users/chiap/source/repos/fromasmtodisasm/TestEngine/src/Engine/Renderer/Shaders/Parser.yy" // lalr1.cc:859
    {
    CTechnique tech;
    tech.Name =  yystack_[0].value.as< std::string > ().c_str();
    driver.currentEffect->m_Techniques.push_back(tech);
    CryLog("creation of Technique %s...\n", tech.Name.data());
    //curTechnique = curContainer->createTechnique($2->c_str())->getExInterface();
    //curAnnotations = curTechnique->annotations()->getExInterface();
    //delete $2;
}
#line 776 "/cygdrive/c/Users/chiap/source/repos/fromasmtodisasm/TestEngine/src/Engine/Renderer/Shaders/Parser.cpp" // lalr1.cc:859
    break;

  case 38:
#line 313 "/cygdrive/c/Users/chiap/source/repos/fromasmtodisasm/TestEngine/src/Engine/Renderer/Shaders/Parser.yy" // lalr1.cc:859
    { 
    //lex_pop_state();
    //curAnnotations = NULL;
}
#line 785 "/cygdrive/c/Users/chiap/source/repos/fromasmtodisasm/TestEngine/src/Engine/Renderer/Shaders/Parser.cpp" // lalr1.cc:859
    break;

  case 40:
#line 323 "/cygdrive/c/Users/chiap/source/repos/fromasmtodisasm/TestEngine/src/Engine/Renderer/Shaders/Parser.yy" // lalr1.cc:859
    {
/*
    if(!curAnnotations)
        curAnnotations = IAnnotationEx::getAnnotationSingleton(2); // need a temporary place since nothing was initialized
    if(!curAnnotations->addAnnotation($2->c_str(), $4->c_str()))
        yyerror("err\n");
    delete $4;
*/
    }
#line 799 "/cygdrive/c/Users/chiap/source/repos/fromasmtodisasm/TestEngine/src/Engine/Renderer/Shaders/Parser.cpp" // lalr1.cc:859
    break;

  case 41:
#line 333 "/cygdrive/c/Users/chiap/source/repos/fromasmtodisasm/TestEngine/src/Engine/Renderer/Shaders/Parser.yy" // lalr1.cc:859
    {CryLog("Begin annotations"); }
#line 805 "/cygdrive/c/Users/chiap/source/repos/fromasmtodisasm/TestEngine/src/Engine/Renderer/Shaders/Parser.cpp" // lalr1.cc:859
    break;

  case 45:
#line 340 "/cygdrive/c/Users/chiap/source/repos/fromasmtodisasm/TestEngine/src/Engine/Renderer/Shaders/Parser.yy" // lalr1.cc:859
    {
        yylhs.value.as< std::string > () = yystack_[0].value.as< std::string > (); 
    }
#line 813 "/cygdrive/c/Users/chiap/source/repos/fromasmtodisasm/TestEngine/src/Engine/Renderer/Shaders/Parser.cpp" // lalr1.cc:859
    break;

  case 46:
#line 343 "/cygdrive/c/Users/chiap/source/repos/fromasmtodisasm/TestEngine/src/Engine/Renderer/Shaders/Parser.yy" // lalr1.cc:859
    {
        yylhs.value.as< std::string > () = "Common";
        is_common = true;
    }
#line 822 "/cygdrive/c/Users/chiap/source/repos/fromasmtodisasm/TestEngine/src/Engine/Renderer/Shaders/Parser.cpp" // lalr1.cc:859
    break;

  case 47:
#line 349 "/cygdrive/c/Users/chiap/source/repos/fromasmtodisasm/TestEngine/src/Engine/Renderer/Shaders/Parser.yy" // lalr1.cc:859
    { 
		//gEnv->pLog->Log("$3 Shader $1%s $3parsed", $1.data()); 
        driver.currentEffect->m_shaders.push_back(IEffect::ShaderInfo{yystack_[2].value.as< std::string > (), yystack_[0].value.as< std::string > ()});
        if (yystack_[2].value.as< std::string > () == "Common")
            CommonCode.push_back(driver.currentEffect->m_shaders.back().data);
	}
#line 833 "/cygdrive/c/Users/chiap/source/repos/fromasmtodisasm/TestEngine/src/Engine/Renderer/Shaders/Parser.cpp" // lalr1.cc:859
    break;

  case 48:
#line 359 "/cygdrive/c/Users/chiap/source/repos/fromasmtodisasm/TestEngine/src/Engine/Renderer/Shaders/Parser.yy" // lalr1.cc:859
    {
    gEnv->pLog->Log(
    "$3 New vertex format <%s> with field %s (%s = %d)", 
    yystack_[6].value.as< std::string > ().data(), yystack_[4].value.as< std::string > ().data(), "vkFormat", 10);
}
#line 843 "/cygdrive/c/Users/chiap/source/repos/fromasmtodisasm/TestEngine/src/Engine/Renderer/Shaders/Parser.cpp" // lalr1.cc:859
    break;

  case 49:
#line 366 "/cygdrive/c/Users/chiap/source/repos/fromasmtodisasm/TestEngine/src/Engine/Renderer/Shaders/Parser.yy" // lalr1.cc:859
    { 
      //$$ = VkFormat(10); 
      CryLog("format not implemented");

      }
#line 853 "/cygdrive/c/Users/chiap/source/repos/fromasmtodisasm/TestEngine/src/Engine/Renderer/Shaders/Parser.cpp" // lalr1.cc:859
    break;


#line 857 "/cygdrive/c/Users/chiap/source/repos/fromasmtodisasm/TestEngine/src/Engine/Renderer/Shaders/Parser.cpp" // lalr1.cc:859
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


  const signed char parser::yypact_ninf_ = -78;

  const signed char parser::yytable_ninf_ = -34;

  const signed char
  parser::yypact_[] =
  {
     -77,   -78,     0,   -78,     8,     9,    11,   -78,   -78,   -78,
     -78,   -78,   -78,   -78,   -78,   -78,   -78,   -78,   -78,    19,
     -78,   -73,   -78,   -78,   -78,   -78,   -78,   -78,   -78,   -78,
     -78,   -78,   -78,   -78,   -78,   -78,   -78,   -78,   -71,   -65,
     -78,     3,   -64,     5,    24,   -78,   -78,   -78,   -60,    27,
       2,    17,   -56,   -78,     5,   -78,   -64,   -78,   -78,   -78,
      20,   -78,    -2,    34,   -64,   -55,   -54,   -53,   -78,   -78,
     -52,   -59,   -50,    32,   -59,   -78,   -78,   -78,   -45,   -78,
       1,   -46,   -78,   -42,   -44,    43,   -78,   -78,   -78,   -78,
     -78,   -78
  };

  const unsigned char
  parser::yydefact_[] =
  {
       2,     3,     2,     1,    46,    35,     0,    50,    51,    52,
      53,    54,    55,    56,    57,    58,    59,    60,    61,     0,
       4,     0,     5,    18,     6,     7,     8,     9,    10,    11,
      12,    13,    14,    15,    16,    17,    45,    37,     0,     0,
      62,     0,    43,     0,     0,    47,    41,    44,     0,    27,
       0,     0,     0,    39,     0,    29,    43,    31,    34,    36,
       0,    49,     0,     0,    43,     0,     0,     0,    42,    38,
       0,     0,     0,     0,     0,    19,    21,    20,     0,    23,
       0,     0,    48,     0,     0,     0,    25,    24,    28,    40,
      30,    22
  };

  const signed char
  parser::yypgoto_[] =
  {
     -78,    47,   -78,   -32,   -78,   -25,   -47,   -78,   -78,    -4,
     -78,   -78,   -78,   -78,   -78,   -78,   -51,   -78,   -78,   -78,
     -78,   -78,   -78,   -78,   -78,   -78,   -78,   -78,   -78,   -78,
     -78,   -78,   -78
  };

  const signed char
  parser::yydefgoto_[] =
  {
      -1,    19,    78,    79,    80,    81,    50,    56,    64,    51,
      20,    38,    42,    62,    47,    53,    48,    21,    22,    23,
      66,    24,    25,    26,    27,    28,    29,    30,    31,    32,
      33,    34,    35
  };

  const signed char
  parser::yytable_[] =
  {
       3,    67,    86,    57,    60,    65,    75,    76,    77,     1,
       4,    36,    37,    70,    39,    41,    60,    43,    58,     5,
      40,    57,   -32,    44,    45,    49,    46,    52,    54,     4,
      55,    61,    72,    71,    73,    58,    74,    49,     5,    82,
     -33,    83,    85,    88,    89,    90,    91,     2,    87,    84,
      63,     0,     0,     0,    49,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    75,    76,    77,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     6,     1,     0,     0,    68,
     -26,   -32,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,     6,     1,    59,     0,     0,   -33,
       0,     7,     8,     9,    10,    11,    12,    13,    14,    15,
      16,    17,    18,    69
  };

  const signed char
  parser::yycheck_[] =
  {
       0,     3,     1,     1,    51,    56,    65,    66,    67,    86,
      10,     3,     3,    64,     3,    88,    63,    88,     1,    19,
       1,     1,    20,    88,    21,    20,    90,     3,    88,    10,
       3,    87,    86,    88,    87,     1,    88,    20,    19,    89,
      20,     9,    87,    89,    86,    89,     3,     0,    80,    74,
      54,    -1,    -1,    -1,    20,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    65,    66,    67,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    85,    86,    -1,    -1,    91,
      89,    89,    92,    93,    94,    95,    96,    97,    98,    99,
     100,   101,   102,   103,    85,    86,    89,    -1,    -1,    89,
      -1,    92,    93,    94,    95,    96,    97,    98,    99,   100,
     101,   102,   103,    89
  };

  const unsigned char
  parser::yystos_[] =
  {
       0,    86,   105,     0,    10,    19,    85,    92,    93,    94,
      95,    96,    97,    98,    99,   100,   101,   102,   103,   105,
     114,   121,   122,   123,   125,   126,   127,   128,   129,   130,
     131,   132,   133,   134,   135,   136,     3,     3,   115,     3,
       1,    88,   116,    88,    88,    21,    90,   118,   120,    20,
     110,   113,     3,   119,    88,     3,   111,     1,     1,    89,
     110,    87,   117,   113,   112,   120,   124,     3,    91,    89,
     120,    88,    86,    87,    88,    65,    66,    67,   106,   107,
     108,   109,    89,     9,   109,    87,     1,   107,    89,    86,
      89,     3
  };

  const unsigned char
  parser::yyr1_[] =
  {
       0,   104,   105,   105,   105,   105,   105,   105,   105,   105,
     105,   105,   105,   105,   105,   105,   105,   105,   105,   106,
     106,   106,   107,   108,   108,   108,   109,   111,   110,   112,
     110,   110,   113,   113,   113,   115,   114,   116,   114,   117,
     117,   119,   118,   120,   120,   121,   121,   122,   123,   124,
     125,   126,   127,   128,   129,   130,   131,   132,   133,   134,
     135,   136,   136
  };

  const unsigned char
  parser::yyr2_[] =
  {
       0,     2,     0,     1,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     1,
       1,     1,     3,     1,     2,     2,     1,     0,     6,     0,
       7,     2,     1,     2,     2,     0,     5,     0,     7,     0,
       5,     0,     4,     0,     1,     2,     1,     3,     8,     0,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     2
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
  "TEXTURERESOURCECUBE", "VOID_TYPE", "UNSIGNED", "HIGHP", "MEDIUMP",
  "LOWP", "UNIFORM", "CSTBUFFER", "FLOAT_TYPE", "FLOAT2_TYPE",
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
  "';'", "'='", "'{'", "'}'", "'<'", "'>'", "\"hlsl\"", "\"texture\"",
  "\"resource\"", "\"fbo\"", "\"sstate\"", "\"dststate\"", "\"prstate\"",
  "\"csstate\"", "\"rstate\"", "\"namespace\"", "\"cstbuffer\"",
  "\"uniforms\"", "$accept", "input", "shader_type", "shader_assignment",
  "shader_assignments", "passstates", "pass", "$@1", "$@2", "passes",
  "tech", "$@3", "$@4", "annotation", "annotations2", "$@5", "annotations",
  "glsl_header", "glsl", "vertexformat", "format", "hlsl", "texture",
  "resource", "fbo", "sstate", "dststate", "prstate", "csstate", "rstate",
  "namespace", "cstbuffer", "uniforms", YY_NULLPTR
  };

#if YYDEBUG
  const unsigned short int
  parser::yyrline_[] =
  {
       0,   203,   203,   204,   205,   206,   207,   208,   209,   210,
     211,   212,   213,   214,   215,   216,   217,   218,   219,   224,
     225,   226,   229,   235,   236,   237,   245,   251,   251,   265,
     265,   287,   292,   293,   294,   300,   300,   305,   305,   322,
     323,   333,   333,   335,   336,   340,   343,   349,   358,   366,
     373,   374,   375,   376,   377,   378,   379,   380,   381,   382,
     383,   384,   385
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
#line 1322 "/cygdrive/c/Users/chiap/source/repos/fromasmtodisasm/TestEngine/src/Engine/Renderer/Shaders/Parser.cpp" // lalr1.cc:1167
#line 388 "/cygdrive/c/Users/chiap/source/repos/fromasmtodisasm/TestEngine/src/Engine/Renderer/Shaders/Parser.yy" // lalr1.cc:1168


#include <sstream>
void
yy::parser::error(const location_type& l, const std::string& m)
{
  std::stringstream ss;
  ss << l << ": " << m;
  gEnv->pLog->LogError(ss.str().c_str());    
}
#pragma warning(pop)
