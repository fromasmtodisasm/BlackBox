// A Bison parser, made by GNU Bison 3.8.2.

// Skeleton implementation for Bison LALR(1) parsers in C++

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

// DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
// especially those whose name start with YY_ or yy_.  They are
// private implementation details that can be changed or removed.

// "%code top" blocks.
#line 115 "/cygdrive/c/Users/chiap/source/repos/fromasmtodisasm/TestEngine/Code/Engine/Renderer/Shaders/Parser.yy"

	#pragma warning(push, 0)
    #include "Driver.hpp"
    #include "Scanner.hpp"
    #include "location.hh"

    #define GreenLog(...) gEnv->pLog->Log(__VA_ARGS__)

    #include "Effect.hpp"

    #ifdef S_FALSE
    #undef S_FALSE
    #endif

    std::vector<std::string> Code;
    bool is_common;
    std::vector<string> annotations;

    static yy::parser::symbol_type yylex(Scanner &scanner, Driver& driver) {
        return scanner.ScanToken();
    }

    using Type = std::string;

#line 64 "/cygdrive/c/Users/chiap/source/repos/fromasmtodisasm/TestEngine/Code/Engine/Renderer/Shaders/Parser.cpp"




#include "Parser.hpp"




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


// Whether we are compiled with exception support.
#ifndef YY_EXCEPTIONS
# if defined __GNUC__ && !defined __EXCEPTIONS
#  define YY_EXCEPTIONS 0
# else
#  define YY_EXCEPTIONS 1
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
    while (false)
# endif


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
      *yycdebug_ << '\n';                       \
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
      yy_stack_print_ ();                \
  } while (false)

#else // !YYDEBUG

# define YYCDEBUG if (false) std::cerr
# define YY_SYMBOL_PRINT(Title, Symbol)  YY_USE (Symbol)
# define YY_REDUCE_PRINT(Rule)           static_cast<void> (0)
# define YY_STACK_PRINT()                static_cast<void> (0)

#endif // !YYDEBUG

#define yyerrok         (yyerrstatus_ = 0)
#define yyclearin       (yyla.clear ())

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab
#define YYRECOVERING()  (!!yyerrstatus_)

namespace yy {
#line 163 "/cygdrive/c/Users/chiap/source/repos/fromasmtodisasm/TestEngine/Code/Engine/Renderer/Shaders/Parser.cpp"

  /// Build a parser object.
  parser::parser (Scanner &scanner_yyarg, Driver &driver_yyarg)
#if YYDEBUG
    : yydebug_ (false),
      yycdebug_ (&std::cerr),
#else
    :
#endif
      scanner (scanner_yyarg),
      driver (driver_yyarg)
  {}

  parser::~parser ()
  {}

  parser::syntax_error::~syntax_error () YY_NOEXCEPT YY_NOTHROW
  {}

  /*---------.
  | symbol.  |
  `---------*/



  // by_state.
  parser::by_state::by_state () YY_NOEXCEPT
    : state (empty_state)
  {}

  parser::by_state::by_state (const by_state& that) YY_NOEXCEPT
    : state (that.state)
  {}

  void
  parser::by_state::clear () YY_NOEXCEPT
  {
    state = empty_state;
  }

  void
  parser::by_state::move (by_state& that)
  {
    state = that.state;
    that.clear ();
  }

  parser::by_state::by_state (state_type s) YY_NOEXCEPT
    : state (s)
  {}

  parser::symbol_kind_type
  parser::by_state::kind () const YY_NOEXCEPT
  {
    if (state == empty_state)
      return symbol_kind::S_YYEMPTY;
    else
      return YY_CAST (symbol_kind_type, yystos_[+state]);
  }

  parser::stack_symbol_type::stack_symbol_type ()
  {}

  parser::stack_symbol_type::stack_symbol_type (YY_RVREF (stack_symbol_type) that)
    : super_type (YY_MOVE (that.state), YY_MOVE (that.location))
  {
    switch (that.kind ())
    {
      case symbol_kind::S_VERTEXPROGRAM: // VERTEXPROGRAM
      case symbol_kind::S_FRAGMENTPROGRAM: // FRAGMENTPROGRAM
      case symbol_kind::S_GEOMETRYPROGRAM: // GEOMETRYPROGRAM
      case symbol_kind::S_HULLPROGRAM: // HULLPROGRAM
      case symbol_kind::S_EVALPROGRAM: // EVALPROGRAM
      case symbol_kind::S_shader_type: // shader_type
        value.YY_MOVE_OR_COPY< IShader::Type > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_TRUE: // TRUE
      case symbol_kind::S_FALSE: // FALSE
      case symbol_kind::S_BOOL: // BOOL
        value.YY_MOVE_OR_COPY< bool > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_FLOAT: // FLOAT
        value.YY_MOVE_OR_COPY< float > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_INT: // INT
        value.YY_MOVE_OR_COPY< int > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_base_type: // base_type
        value.YY_MOVE_OR_COPY< nvFX::IUniform::Type > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_TYPE_NAME: // TYPE_NAME
      case symbol_kind::S_IDENTIFIER: // IDENTIFIER
      case symbol_kind::S_STR: // STR
      case symbol_kind::S_CODEBODY: // CODEBODY
      case symbol_kind::S_struct_header: // struct_header
      case symbol_kind::S_struct_footer: // struct_footer
      case symbol_kind::S_shader_assignment: // shader_assignment
      case symbol_kind::S_hlsl_header: // hlsl_header
      case symbol_kind::S_shader_header: // shader_header
        value.YY_MOVE_OR_COPY< std::string > (YY_MOVE (that.value));
        break;

      default:
        break;
    }

#if 201103L <= YY_CPLUSPLUS
    // that is emptied.
    that.state = empty_state;
#endif
  }

  parser::stack_symbol_type::stack_symbol_type (state_type s, YY_MOVE_REF (symbol_type) that)
    : super_type (s, YY_MOVE (that.location))
  {
    switch (that.kind ())
    {
      case symbol_kind::S_VERTEXPROGRAM: // VERTEXPROGRAM
      case symbol_kind::S_FRAGMENTPROGRAM: // FRAGMENTPROGRAM
      case symbol_kind::S_GEOMETRYPROGRAM: // GEOMETRYPROGRAM
      case symbol_kind::S_HULLPROGRAM: // HULLPROGRAM
      case symbol_kind::S_EVALPROGRAM: // EVALPROGRAM
      case symbol_kind::S_shader_type: // shader_type
        value.move< IShader::Type > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_TRUE: // TRUE
      case symbol_kind::S_FALSE: // FALSE
      case symbol_kind::S_BOOL: // BOOL
        value.move< bool > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_FLOAT: // FLOAT
        value.move< float > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_INT: // INT
        value.move< int > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_base_type: // base_type
        value.move< nvFX::IUniform::Type > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_TYPE_NAME: // TYPE_NAME
      case symbol_kind::S_IDENTIFIER: // IDENTIFIER
      case symbol_kind::S_STR: // STR
      case symbol_kind::S_CODEBODY: // CODEBODY
      case symbol_kind::S_struct_header: // struct_header
      case symbol_kind::S_struct_footer: // struct_footer
      case symbol_kind::S_shader_assignment: // shader_assignment
      case symbol_kind::S_hlsl_header: // hlsl_header
      case symbol_kind::S_shader_header: // shader_header
        value.move< std::string > (YY_MOVE (that.value));
        break;

      default:
        break;
    }

    // that is emptied.
    that.kind_ = symbol_kind::S_YYEMPTY;
  }

#if YY_CPLUSPLUS < 201103L
  parser::stack_symbol_type&
  parser::stack_symbol_type::operator= (const stack_symbol_type& that)
  {
    state = that.state;
    switch (that.kind ())
    {
      case symbol_kind::S_VERTEXPROGRAM: // VERTEXPROGRAM
      case symbol_kind::S_FRAGMENTPROGRAM: // FRAGMENTPROGRAM
      case symbol_kind::S_GEOMETRYPROGRAM: // GEOMETRYPROGRAM
      case symbol_kind::S_HULLPROGRAM: // HULLPROGRAM
      case symbol_kind::S_EVALPROGRAM: // EVALPROGRAM
      case symbol_kind::S_shader_type: // shader_type
        value.copy< IShader::Type > (that.value);
        break;

      case symbol_kind::S_TRUE: // TRUE
      case symbol_kind::S_FALSE: // FALSE
      case symbol_kind::S_BOOL: // BOOL
        value.copy< bool > (that.value);
        break;

      case symbol_kind::S_FLOAT: // FLOAT
        value.copy< float > (that.value);
        break;

      case symbol_kind::S_INT: // INT
        value.copy< int > (that.value);
        break;

      case symbol_kind::S_base_type: // base_type
        value.copy< nvFX::IUniform::Type > (that.value);
        break;

      case symbol_kind::S_TYPE_NAME: // TYPE_NAME
      case symbol_kind::S_IDENTIFIER: // IDENTIFIER
      case symbol_kind::S_STR: // STR
      case symbol_kind::S_CODEBODY: // CODEBODY
      case symbol_kind::S_struct_header: // struct_header
      case symbol_kind::S_struct_footer: // struct_footer
      case symbol_kind::S_shader_assignment: // shader_assignment
      case symbol_kind::S_hlsl_header: // hlsl_header
      case symbol_kind::S_shader_header: // shader_header
        value.copy< std::string > (that.value);
        break;

      default:
        break;
    }

    location = that.location;
    return *this;
  }

  parser::stack_symbol_type&
  parser::stack_symbol_type::operator= (stack_symbol_type& that)
  {
    state = that.state;
    switch (that.kind ())
    {
      case symbol_kind::S_VERTEXPROGRAM: // VERTEXPROGRAM
      case symbol_kind::S_FRAGMENTPROGRAM: // FRAGMENTPROGRAM
      case symbol_kind::S_GEOMETRYPROGRAM: // GEOMETRYPROGRAM
      case symbol_kind::S_HULLPROGRAM: // HULLPROGRAM
      case symbol_kind::S_EVALPROGRAM: // EVALPROGRAM
      case symbol_kind::S_shader_type: // shader_type
        value.move< IShader::Type > (that.value);
        break;

      case symbol_kind::S_TRUE: // TRUE
      case symbol_kind::S_FALSE: // FALSE
      case symbol_kind::S_BOOL: // BOOL
        value.move< bool > (that.value);
        break;

      case symbol_kind::S_FLOAT: // FLOAT
        value.move< float > (that.value);
        break;

      case symbol_kind::S_INT: // INT
        value.move< int > (that.value);
        break;

      case symbol_kind::S_base_type: // base_type
        value.move< nvFX::IUniform::Type > (that.value);
        break;

      case symbol_kind::S_TYPE_NAME: // TYPE_NAME
      case symbol_kind::S_IDENTIFIER: // IDENTIFIER
      case symbol_kind::S_STR: // STR
      case symbol_kind::S_CODEBODY: // CODEBODY
      case symbol_kind::S_struct_header: // struct_header
      case symbol_kind::S_struct_footer: // struct_footer
      case symbol_kind::S_shader_assignment: // shader_assignment
      case symbol_kind::S_hlsl_header: // hlsl_header
      case symbol_kind::S_shader_header: // shader_header
        value.move< std::string > (that.value);
        break;

      default:
        break;
    }

    location = that.location;
    // that is emptied.
    that.state = empty_state;
    return *this;
  }
#endif

  template <typename Base>
  void
  parser::yy_destroy_ (const char* yymsg, basic_symbol<Base>& yysym) const
  {
    if (yymsg)
      YY_SYMBOL_PRINT (yymsg, yysym);
  }

#if YYDEBUG
  template <typename Base>
  void
  parser::yy_print_ (std::ostream& yyo, const basic_symbol<Base>& yysym) const
  {
    std::ostream& yyoutput = yyo;
    YY_USE (yyoutput);
    if (yysym.empty ())
      yyo << "empty symbol";
    else
      {
        symbol_kind_type yykind = yysym.kind ();
        yyo << (yykind < YYNTOKENS ? "token" : "nterm")
            << ' ' << yysym.name () << " ("
            << yysym.location << ": ";
        YY_USE (yykind);
        yyo << ')';
      }
  }
#endif

  void
  parser::yypush_ (const char* m, YY_MOVE_REF (stack_symbol_type) sym)
  {
    if (m)
      YY_SYMBOL_PRINT (m, sym);
    yystack_.push (YY_MOVE (sym));
  }

  void
  parser::yypush_ (const char* m, state_type s, YY_MOVE_REF (symbol_type) sym)
  {
#if 201103L <= YY_CPLUSPLUS
    yypush_ (m, stack_symbol_type (s, std::move (sym)));
#else
    stack_symbol_type ss (s, sym);
    yypush_ (m, ss);
#endif
  }

  void
  parser::yypop_ (int n) YY_NOEXCEPT
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

  parser::state_type
  parser::yy_lr_goto_state_ (state_type yystate, int yysym)
  {
    int yyr = yypgoto_[yysym - YYNTOKENS] + yystate;
    if (0 <= yyr && yyr <= yylast_ && yycheck_[yyr] == yystate)
      return yytable_[yyr];
    else
      return yydefgoto_[yysym - YYNTOKENS];
  }

  bool
  parser::yy_pact_value_is_default_ (int yyvalue) YY_NOEXCEPT
  {
    return yyvalue == yypact_ninf_;
  }

  bool
  parser::yy_table_value_is_error_ (int yyvalue) YY_NOEXCEPT
  {
    return yyvalue == yytable_ninf_;
  }

  int
  parser::operator() ()
  {
    return parse ();
  }

  int
  parser::parse ()
  {
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

#if YY_EXCEPTIONS
    try
#endif // YY_EXCEPTIONS
      {
    YYCDEBUG << "Starting parse\n";


    // User initialization code.
#line 105 "/cygdrive/c/Users/chiap/source/repos/fromasmtodisasm/TestEngine/Code/Engine/Renderer/Shaders/Parser.yy"
{
    is_common = false;
    Code.clear();
}

#line 586 "/cygdrive/c/Users/chiap/source/repos/fromasmtodisasm/TestEngine/Code/Engine/Renderer/Shaders/Parser.cpp"


    /* Initialize the stack.  The initial state will be set in
       yynewstate, since the latter expects the semantical and the
       location values to have been already stored, initialize these
       stacks with a primary value.  */
    yystack_.clear ();
    yypush_ (YY_NULLPTR, 0, YY_MOVE (yyla));

  /*-----------------------------------------------.
  | yynewstate -- push a new symbol on the stack.  |
  `-----------------------------------------------*/
  yynewstate:
    YYCDEBUG << "Entering state " << int (yystack_[0].state) << '\n';
    YY_STACK_PRINT ();

    // Accept?
    if (yystack_[0].state == yyfinal_)
      YYACCEPT;

    goto yybackup;


  /*-----------.
  | yybackup.  |
  `-----------*/
  yybackup:
    // Try to take a decision without lookahead.
    yyn = yypact_[+yystack_[0].state];
    if (yy_pact_value_is_default_ (yyn))
      goto yydefault;

    // Read a lookahead token.
    if (yyla.empty ())
      {
        YYCDEBUG << "Reading a token\n";
#if YY_EXCEPTIONS
        try
#endif // YY_EXCEPTIONS
          {
            symbol_type yylookahead (yylex (scanner, driver));
            yyla.move (yylookahead);
          }
#if YY_EXCEPTIONS
        catch (const syntax_error& yyexc)
          {
            YYCDEBUG << "Caught exception: " << yyexc.what() << '\n';
            error (yyexc);
            goto yyerrlab1;
          }
#endif // YY_EXCEPTIONS
      }
    YY_SYMBOL_PRINT ("Next token is", yyla);

    if (yyla.kind () == symbol_kind::S_YYerror)
    {
      // The scanner already issued an error message, process directly
      // to error recovery.  But do not keep the error token as
      // lookahead, it is too special and may lead us to an endless
      // loop in error recovery. */
      yyla.kind_ = symbol_kind::S_YYUNDEF;
      goto yyerrlab1;
    }

    /* If the proper action on seeing token YYLA.TYPE is to reduce or
       to detect an error, take that action.  */
    yyn += yyla.kind ();
    if (yyn < 0 || yylast_ < yyn || yycheck_[yyn] != yyla.kind ())
      {
        goto yydefault;
      }

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
    yypush_ ("Shifting", state_type (yyn), YY_MOVE (yyla));
    goto yynewstate;


  /*-----------------------------------------------------------.
  | yydefault -- do the default action for the current state.  |
  `-----------------------------------------------------------*/
  yydefault:
    yyn = yydefact_[+yystack_[0].state];
    if (yyn == 0)
      goto yyerrlab;
    goto yyreduce;


  /*-----------------------------.
  | yyreduce -- do a reduction.  |
  `-----------------------------*/
  yyreduce:
    yylen = yyr2_[yyn];
    {
      stack_symbol_type yylhs;
      yylhs.state = yy_lr_goto_state_ (yystack_[yylen].state, yyr1_[yyn]);
      /* Variants are always initialized to an empty instance of the
         correct type. The default '$$ = $1' action is NOT applied
         when using variants.  */
      switch (yyr1_[yyn])
    {
      case symbol_kind::S_VERTEXPROGRAM: // VERTEXPROGRAM
      case symbol_kind::S_FRAGMENTPROGRAM: // FRAGMENTPROGRAM
      case symbol_kind::S_GEOMETRYPROGRAM: // GEOMETRYPROGRAM
      case symbol_kind::S_HULLPROGRAM: // HULLPROGRAM
      case symbol_kind::S_EVALPROGRAM: // EVALPROGRAM
      case symbol_kind::S_shader_type: // shader_type
        yylhs.value.emplace< IShader::Type > ();
        break;

      case symbol_kind::S_TRUE: // TRUE
      case symbol_kind::S_FALSE: // FALSE
      case symbol_kind::S_BOOL: // BOOL
        yylhs.value.emplace< bool > ();
        break;

      case symbol_kind::S_FLOAT: // FLOAT
        yylhs.value.emplace< float > ();
        break;

      case symbol_kind::S_INT: // INT
        yylhs.value.emplace< int > ();
        break;

      case symbol_kind::S_base_type: // base_type
        yylhs.value.emplace< nvFX::IUniform::Type > ();
        break;

      case symbol_kind::S_TYPE_NAME: // TYPE_NAME
      case symbol_kind::S_IDENTIFIER: // IDENTIFIER
      case symbol_kind::S_STR: // STR
      case symbol_kind::S_CODEBODY: // CODEBODY
      case symbol_kind::S_struct_header: // struct_header
      case symbol_kind::S_struct_footer: // struct_footer
      case symbol_kind::S_shader_assignment: // shader_assignment
      case symbol_kind::S_hlsl_header: // hlsl_header
      case symbol_kind::S_shader_header: // shader_header
        yylhs.value.emplace< std::string > ();
        break;

      default:
        break;
    }


      // Default location.
      {
        stack_type::slice range (yystack_, yylen);
        YYLLOC_DEFAULT (yylhs.location, range, yylen);
        yyerror_range[1].location = yylhs.location;
      }

      // Perform the reduction.
      YY_REDUCE_PRINT (yyn);
#if YY_EXCEPTIONS
      try
#endif // YY_EXCEPTIONS
        {
          switch (yyn)
            {
  case 2: // input: %empty
#line 274 "/cygdrive/c/Users/chiap/source/repos/fromasmtodisasm/TestEngine/Code/Engine/Renderer/Shaders/Parser.yy"
              { CryLog("Empty effect"); }
#line 763 "/cygdrive/c/Users/chiap/source/repos/fromasmtodisasm/TestEngine/Code/Engine/Renderer/Shaders/Parser.cpp"
    break;

  case 9: // input: input function_declaration
#line 281 "/cygdrive/c/Users/chiap/source/repos/fromasmtodisasm/TestEngine/Code/Engine/Renderer/Shaders/Parser.yy"
                             {CryLog("Pop lex state from declaration"); lex_pop_state();}
#line 769 "/cygdrive/c/Users/chiap/source/repos/fromasmtodisasm/TestEngine/Code/Engine/Renderer/Shaders/Parser.cpp"
    break;

  case 19: // $@1: %empty
#line 293 "/cygdrive/c/Users/chiap/source/repos/fromasmtodisasm/TestEngine/Code/Engine/Renderer/Shaders/Parser.yy"
                                                       { 
    //CryLog("Open function scope");
    }
#line 777 "/cygdrive/c/Users/chiap/source/repos/fromasmtodisasm/TestEngine/Code/Engine/Renderer/Shaders/Parser.cpp"
    break;

  case 20: // function_definition: function_declaration semantic '{' $@1 CODEBODY
#line 295 "/cygdrive/c/Users/chiap/source/repos/fromasmtodisasm/TestEngine/Code/Engine/Renderer/Shaders/Parser.yy"
               {
    //CryLog("Close function scope"); 
    auto body = yystack_[0].value.as < std::string > ();
    //CryLog("FuncBody: %s", body.data());
}
#line 787 "/cygdrive/c/Users/chiap/source/repos/fromasmtodisasm/TestEngine/Code/Engine/Renderer/Shaders/Parser.cpp"
    break;

  case 23: // $@2: %empty
#line 303 "/cygdrive/c/Users/chiap/source/repos/fromasmtodisasm/TestEngine/Code/Engine/Renderer/Shaders/Parser.yy"
                                                       {}
#line 793 "/cygdrive/c/Users/chiap/source/repos/fromasmtodisasm/TestEngine/Code/Engine/Renderer/Shaders/Parser.cpp"
    break;

  case 24: // function_declaration: object_type IDENTIFIER '(' $@2 arguments ')'
#line 303 "/cygdrive/c/Users/chiap/source/repos/fromasmtodisasm/TestEngine/Code/Engine/Renderer/Shaders/Parser.yy"
                                                                      {
    CryLog("Parsed function declaration for: [%s]", yystack_[4].value.as < std::string > ().data());
    lex_print_state();
}
#line 802 "/cygdrive/c/Users/chiap/source/repos/fromasmtodisasm/TestEngine/Code/Engine/Renderer/Shaders/Parser.cpp"
    break;

  case 25: // fatal_error: FATALERROR
#line 308 "/cygdrive/c/Users/chiap/source/repos/fromasmtodisasm/TestEngine/Code/Engine/Renderer/Shaders/Parser.yy"
                        { CryFatalError("Stopping paring!!!"); }
#line 808 "/cygdrive/c/Users/chiap/source/repos/fromasmtodisasm/TestEngine/Code/Engine/Renderer/Shaders/Parser.cpp"
    break;

  case 30: // cbuffer: CSTBUFFER IDENTIFIER register_declaration '{' var_decls '}'
#line 315 "/cygdrive/c/Users/chiap/source/repos/fromasmtodisasm/TestEngine/Code/Engine/Renderer/Shaders/Parser.yy"
{
    CryLog("New CBuffer %s", yystack_[4].value.as < std::string > ().data());
    lex_pop_state();
}
#line 817 "/cygdrive/c/Users/chiap/source/repos/fromasmtodisasm/TestEngine/Code/Engine/Renderer/Shaders/Parser.cpp"
    break;

  case 32: // cbuffer: CONSTANTBUFFER template_parameter IDENTIFIER register_declaration
#line 323 "/cygdrive/c/Users/chiap/source/repos/fromasmtodisasm/TestEngine/Code/Engine/Renderer/Shaders/Parser.yy"
{
    CryLog("New CBuffer %s", yystack_[1].value.as < std::string > ().data());
    lex_pop_state();
}
#line 826 "/cygdrive/c/Users/chiap/source/repos/fromasmtodisasm/TestEngine/Code/Engine/Renderer/Shaders/Parser.cpp"
    break;

  case 33: // struct: STRUCT struct_header struct_body struct_footer
#line 329 "/cygdrive/c/Users/chiap/source/repos/fromasmtodisasm/TestEngine/Code/Engine/Renderer/Shaders/Parser.yy"
{
    //CryLog("New Struct");
}
#line 834 "/cygdrive/c/Users/chiap/source/repos/fromasmtodisasm/TestEngine/Code/Engine/Renderer/Shaders/Parser.cpp"
    break;

  case 34: // struct_header: IDENTIFIER
#line 333 "/cygdrive/c/Users/chiap/source/repos/fromasmtodisasm/TestEngine/Code/Engine/Renderer/Shaders/Parser.yy"
                         {CryLog("StructName: %s", yystack_[0].value.as < std::string > ().data()); scanner.register_type(yystack_[0].value.as < std::string > ().data());}
#line 840 "/cygdrive/c/Users/chiap/source/repos/fromasmtodisasm/TestEngine/Code/Engine/Renderer/Shaders/Parser.cpp"
    break;

  case 35: // struct_header: %empty
#line 333 "/cygdrive/c/Users/chiap/source/repos/fromasmtodisasm/TestEngine/Code/Engine/Renderer/Shaders/Parser.yy"
                                                                                                           {yylhs.value.as < std::string > ()="";}
#line 846 "/cygdrive/c/Users/chiap/source/repos/fromasmtodisasm/TestEngine/Code/Engine/Renderer/Shaders/Parser.cpp"
    break;

  case 37: // struct_footer: IDENTIFIER
#line 335 "/cygdrive/c/Users/chiap/source/repos/fromasmtodisasm/TestEngine/Code/Engine/Renderer/Shaders/Parser.yy"
                          {CryLog("Declared and defined struct with name: %s", yystack_[0].value.as < std::string > ().data());}
#line 852 "/cygdrive/c/Users/chiap/source/repos/fromasmtodisasm/TestEngine/Code/Engine/Renderer/Shaders/Parser.cpp"
    break;

  case 38: // struct_footer: %empty
#line 336 "/cygdrive/c/Users/chiap/source/repos/fromasmtodisasm/TestEngine/Code/Engine/Renderer/Shaders/Parser.yy"
         {yylhs.value.as < std::string > ()="";}
#line 858 "/cygdrive/c/Users/chiap/source/repos/fromasmtodisasm/TestEngine/Code/Engine/Renderer/Shaders/Parser.cpp"
    break;

  case 47: // texture2d: TEXTURE2D_TYPE IDENTIFIER register_declaration resource_initializer
#line 344 "/cygdrive/c/Users/chiap/source/repos/fromasmtodisasm/TestEngine/Code/Engine/Renderer/Shaders/Parser.yy"
{
    CryLog("texture2d");
}
#line 866 "/cygdrive/c/Users/chiap/source/repos/fromasmtodisasm/TestEngine/Code/Engine/Renderer/Shaders/Parser.cpp"
    break;

  case 48: // sampler_state: SAMPLERSTATE IDENTIFIER register_declaration resource_initializer
#line 349 "/cygdrive/c/Users/chiap/source/repos/fromasmtodisasm/TestEngine/Code/Engine/Renderer/Shaders/Parser.yy"
{
    CryLog("sampler");
}
#line 874 "/cygdrive/c/Users/chiap/source/repos/fromasmtodisasm/TestEngine/Code/Engine/Renderer/Shaders/Parser.cpp"
    break;

  case 49: // shader_type: VERTEXPROGRAM
#line 354 "/cygdrive/c/Users/chiap/source/repos/fromasmtodisasm/TestEngine/Code/Engine/Renderer/Shaders/Parser.yy"
                {yylhs.value.as < IShader::Type > () = yystack_[0].value.as < IShader::Type > ();}
#line 880 "/cygdrive/c/Users/chiap/source/repos/fromasmtodisasm/TestEngine/Code/Engine/Renderer/Shaders/Parser.cpp"
    break;

  case 50: // shader_type: GEOMETRYPROGRAM
#line 355 "/cygdrive/c/Users/chiap/source/repos/fromasmtodisasm/TestEngine/Code/Engine/Renderer/Shaders/Parser.yy"
                  {yylhs.value.as < IShader::Type > () = yystack_[0].value.as < IShader::Type > ();}
#line 886 "/cygdrive/c/Users/chiap/source/repos/fromasmtodisasm/TestEngine/Code/Engine/Renderer/Shaders/Parser.cpp"
    break;

  case 51: // shader_type: FRAGMENTPROGRAM
#line 356 "/cygdrive/c/Users/chiap/source/repos/fromasmtodisasm/TestEngine/Code/Engine/Renderer/Shaders/Parser.yy"
                  {yylhs.value.as < IShader::Type > () = yystack_[0].value.as < IShader::Type > ();}
#line 892 "/cygdrive/c/Users/chiap/source/repos/fromasmtodisasm/TestEngine/Code/Engine/Renderer/Shaders/Parser.cpp"
    break;

  case 54: // shader_assignment: shader_type '=' shader_assignment_shader ';'
#line 361 "/cygdrive/c/Users/chiap/source/repos/fromasmtodisasm/TestEngine/Code/Engine/Renderer/Shaders/Parser.yy"
                                                                {
    //$$ = std::make_pair($1, $3);
  //FIXME:
  #if 0
	//driver.currentEffect->shader_assignment($1,$3);
  #endif
}
#line 904 "/cygdrive/c/Users/chiap/source/repos/fromasmtodisasm/TestEngine/Code/Engine/Renderer/Shaders/Parser.cpp"
    break;

  case 59: // base_type: FLOAT_TYPE
#line 383 "/cygdrive/c/Users/chiap/source/repos/fromasmtodisasm/TestEngine/Code/Engine/Renderer/Shaders/Parser.yy"
              { yylhs.value.as < nvFX::IUniform::Type > () = nvFX::IUniform::TFloat; }
#line 910 "/cygdrive/c/Users/chiap/source/repos/fromasmtodisasm/TestEngine/Code/Engine/Renderer/Shaders/Parser.cpp"
    break;

  case 60: // base_type: FLOAT2_TYPE
#line 384 "/cygdrive/c/Users/chiap/source/repos/fromasmtodisasm/TestEngine/Code/Engine/Renderer/Shaders/Parser.yy"
               { yylhs.value.as < nvFX::IUniform::Type > () = nvFX::IUniform::TVec2; }
#line 916 "/cygdrive/c/Users/chiap/source/repos/fromasmtodisasm/TestEngine/Code/Engine/Renderer/Shaders/Parser.cpp"
    break;

  case 61: // base_type: FLOAT3_TYPE
#line 385 "/cygdrive/c/Users/chiap/source/repos/fromasmtodisasm/TestEngine/Code/Engine/Renderer/Shaders/Parser.yy"
               { yylhs.value.as < nvFX::IUniform::Type > () = nvFX::IUniform::TVec3; }
#line 922 "/cygdrive/c/Users/chiap/source/repos/fromasmtodisasm/TestEngine/Code/Engine/Renderer/Shaders/Parser.cpp"
    break;

  case 62: // base_type: FLOAT4_TYPE
#line 386 "/cygdrive/c/Users/chiap/source/repos/fromasmtodisasm/TestEngine/Code/Engine/Renderer/Shaders/Parser.yy"
               { yylhs.value.as < nvFX::IUniform::Type > () = nvFX::IUniform::TVec4; }
#line 928 "/cygdrive/c/Users/chiap/source/repos/fromasmtodisasm/TestEngine/Code/Engine/Renderer/Shaders/Parser.cpp"
    break;

  case 63: // base_type: INT_TYPE
#line 387 "/cygdrive/c/Users/chiap/source/repos/fromasmtodisasm/TestEngine/Code/Engine/Renderer/Shaders/Parser.yy"
               { yylhs.value.as < nvFX::IUniform::Type > () = nvFX::IUniform::TInt; }
#line 934 "/cygdrive/c/Users/chiap/source/repos/fromasmtodisasm/TestEngine/Code/Engine/Renderer/Shaders/Parser.cpp"
    break;

  case 64: // base_type: INT2_TYPE
#line 388 "/cygdrive/c/Users/chiap/source/repos/fromasmtodisasm/TestEngine/Code/Engine/Renderer/Shaders/Parser.yy"
             { yylhs.value.as < nvFX::IUniform::Type > () = nvFX::IUniform::TInt2; }
#line 940 "/cygdrive/c/Users/chiap/source/repos/fromasmtodisasm/TestEngine/Code/Engine/Renderer/Shaders/Parser.cpp"
    break;

  case 65: // base_type: INT3_TYPE
#line 389 "/cygdrive/c/Users/chiap/source/repos/fromasmtodisasm/TestEngine/Code/Engine/Renderer/Shaders/Parser.yy"
             { yylhs.value.as < nvFX::IUniform::Type > () = nvFX::IUniform::TInt3; }
#line 946 "/cygdrive/c/Users/chiap/source/repos/fromasmtodisasm/TestEngine/Code/Engine/Renderer/Shaders/Parser.cpp"
    break;

  case 66: // base_type: INT4_TYPE
#line 390 "/cygdrive/c/Users/chiap/source/repos/fromasmtodisasm/TestEngine/Code/Engine/Renderer/Shaders/Parser.yy"
             { yylhs.value.as < nvFX::IUniform::Type > () = nvFX::IUniform::TInt4; }
#line 952 "/cygdrive/c/Users/chiap/source/repos/fromasmtodisasm/TestEngine/Code/Engine/Renderer/Shaders/Parser.cpp"
    break;

  case 67: // base_type: BOOL_TYPE
#line 391 "/cygdrive/c/Users/chiap/source/repos/fromasmtodisasm/TestEngine/Code/Engine/Renderer/Shaders/Parser.yy"
             { yylhs.value.as < nvFX::IUniform::Type > () = nvFX::IUniform::TBool; }
#line 958 "/cygdrive/c/Users/chiap/source/repos/fromasmtodisasm/TestEngine/Code/Engine/Renderer/Shaders/Parser.cpp"
    break;

  case 68: // base_type: BOOL2_TYPE
#line 392 "/cygdrive/c/Users/chiap/source/repos/fromasmtodisasm/TestEngine/Code/Engine/Renderer/Shaders/Parser.yy"
              { yylhs.value.as < nvFX::IUniform::Type > () = nvFX::IUniform::TBool2; }
#line 964 "/cygdrive/c/Users/chiap/source/repos/fromasmtodisasm/TestEngine/Code/Engine/Renderer/Shaders/Parser.cpp"
    break;

  case 69: // base_type: BOOL3_TYPE
#line 393 "/cygdrive/c/Users/chiap/source/repos/fromasmtodisasm/TestEngine/Code/Engine/Renderer/Shaders/Parser.yy"
              { yylhs.value.as < nvFX::IUniform::Type > () = nvFX::IUniform::TBool3; }
#line 970 "/cygdrive/c/Users/chiap/source/repos/fromasmtodisasm/TestEngine/Code/Engine/Renderer/Shaders/Parser.cpp"
    break;

  case 70: // base_type: BOOL4_TYPE
#line 394 "/cygdrive/c/Users/chiap/source/repos/fromasmtodisasm/TestEngine/Code/Engine/Renderer/Shaders/Parser.yy"
              { yylhs.value.as < nvFX::IUniform::Type > () = nvFX::IUniform::TBool4; }
#line 976 "/cygdrive/c/Users/chiap/source/repos/fromasmtodisasm/TestEngine/Code/Engine/Renderer/Shaders/Parser.cpp"
    break;

  case 71: // base_type: MAT2_TYPE
#line 395 "/cygdrive/c/Users/chiap/source/repos/fromasmtodisasm/TestEngine/Code/Engine/Renderer/Shaders/Parser.yy"
             { yylhs.value.as < nvFX::IUniform::Type > () = nvFX::IUniform::TMat2; }
#line 982 "/cygdrive/c/Users/chiap/source/repos/fromasmtodisasm/TestEngine/Code/Engine/Renderer/Shaders/Parser.cpp"
    break;

  case 72: // base_type: MAT2x4_TYPE
#line 396 "/cygdrive/c/Users/chiap/source/repos/fromasmtodisasm/TestEngine/Code/Engine/Renderer/Shaders/Parser.yy"
               { yylhs.value.as < nvFX::IUniform::Type > () = nvFX::IUniform::TMat2x4; }
#line 988 "/cygdrive/c/Users/chiap/source/repos/fromasmtodisasm/TestEngine/Code/Engine/Renderer/Shaders/Parser.cpp"
    break;

  case 73: // base_type: MAT3_TYPE
#line 397 "/cygdrive/c/Users/chiap/source/repos/fromasmtodisasm/TestEngine/Code/Engine/Renderer/Shaders/Parser.yy"
             { yylhs.value.as < nvFX::IUniform::Type > () = nvFX::IUniform::TMat3; }
#line 994 "/cygdrive/c/Users/chiap/source/repos/fromasmtodisasm/TestEngine/Code/Engine/Renderer/Shaders/Parser.cpp"
    break;

  case 74: // base_type: MAT34_TYPE
#line 398 "/cygdrive/c/Users/chiap/source/repos/fromasmtodisasm/TestEngine/Code/Engine/Renderer/Shaders/Parser.yy"
              { yylhs.value.as < nvFX::IUniform::Type > () = nvFX::IUniform::TMat3; }
#line 1000 "/cygdrive/c/Users/chiap/source/repos/fromasmtodisasm/TestEngine/Code/Engine/Renderer/Shaders/Parser.cpp"
    break;

  case 75: // base_type: MAT4_TYPE
#line 399 "/cygdrive/c/Users/chiap/source/repos/fromasmtodisasm/TestEngine/Code/Engine/Renderer/Shaders/Parser.yy"
             { yylhs.value.as < nvFX::IUniform::Type > () = nvFX::IUniform::TMat4; }
#line 1006 "/cygdrive/c/Users/chiap/source/repos/fromasmtodisasm/TestEngine/Code/Engine/Renderer/Shaders/Parser.cpp"
    break;

  case 106: // var_decl: object_type direct_declarator semantic
#line 463 "/cygdrive/c/Users/chiap/source/repos/fromasmtodisasm/TestEngine/Code/Engine/Renderer/Shaders/Parser.yy"
{
    //CryLog("TryParseVarDecl");
}
#line 1014 "/cygdrive/c/Users/chiap/source/repos/fromasmtodisasm/TestEngine/Code/Engine/Renderer/Shaders/Parser.cpp"
    break;

  case 110: // $@3: %empty
#line 481 "/cygdrive/c/Users/chiap/source/repos/fromasmtodisasm/TestEngine/Code/Engine/Renderer/Shaders/Parser.yy"
     { 
    CryLog("Creating PASS");
    }
#line 1022 "/cygdrive/c/Users/chiap/source/repos/fromasmtodisasm/TestEngine/Code/Engine/Renderer/Shaders/Parser.cpp"
    break;

  case 111: // pass: PASS $@3 annotations passstates
#line 485 "/cygdrive/c/Users/chiap/source/repos/fromasmtodisasm/TestEngine/Code/Engine/Renderer/Shaders/Parser.yy"
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
#line 1038 "/cygdrive/c/Users/chiap/source/repos/fromasmtodisasm/TestEngine/Code/Engine/Renderer/Shaders/Parser.cpp"
    break;

  case 112: // $@4: %empty
#line 496 "/cygdrive/c/Users/chiap/source/repos/fromasmtodisasm/TestEngine/Code/Engine/Renderer/Shaders/Parser.yy"
                  {
    SPass pass;
    pass.Name = yystack_[0].value.as < std::string > ().c_str();
    pass.Code = Code;
    driver.currentEffect->m_Techniques.back().Passes.push_back(pass);
    //driver.currentEffect->m_shaders.push_back(IEffect::ShaderInfo{$1, $3});
    //driver.currentEffect->m_shaders.push_back(IEffect::ShaderInfo{$1, $3});

    CryLog("Creating PASS %s\n", pass.Name.data());
    //curPass = curTechnique->addPass($2->c_str())->getExInterface();
    //curAnnotations = curPass->annotations()->getExInterface();
    }
#line 1055 "/cygdrive/c/Users/chiap/source/repos/fromasmtodisasm/TestEngine/Code/Engine/Renderer/Shaders/Parser.cpp"
    break;

  case 113: // pass: PASS IDENTIFIER $@4 annotations passstates
#line 508 "/cygdrive/c/Users/chiap/source/repos/fromasmtodisasm/TestEngine/Code/Engine/Renderer/Shaders/Parser.yy"
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
#line 1070 "/cygdrive/c/Users/chiap/source/repos/fromasmtodisasm/TestEngine/Code/Engine/Renderer/Shaders/Parser.cpp"
    break;

  case 116: // passes: passes error
#line 526 "/cygdrive/c/Users/chiap/source/repos/fromasmtodisasm/TestEngine/Code/Engine/Renderer/Shaders/Parser.yy"
               { error(yystack_[1].location, "Error in Pass list\n");}
#line 1076 "/cygdrive/c/Users/chiap/source/repos/fromasmtodisasm/TestEngine/Code/Engine/Renderer/Shaders/Parser.cpp"
    break;

  case 117: // $@5: %empty
#line 532 "/cygdrive/c/Users/chiap/source/repos/fromasmtodisasm/TestEngine/Code/Engine/Renderer/Shaders/Parser.yy"
          {
    CryLog("Creation of Technique for NO name\n");
    //curTechnique = curContainer->createTechnique()->getExInterface();
    //curAnnotations = curTechnique->annotations()->getExInterface();
}
#line 1086 "/cygdrive/c/Users/chiap/source/repos/fromasmtodisasm/TestEngine/Code/Engine/Renderer/Shaders/Parser.cpp"
    break;

  case 119: // $@6: %empty
#line 537 "/cygdrive/c/Users/chiap/source/repos/fromasmtodisasm/TestEngine/Code/Engine/Renderer/Shaders/Parser.yy"
                       {
    CTechnique tech;
    tech.Name =  yystack_[0].value.as < std::string > ().c_str();
    driver.currentEffect->m_Techniques.push_back(tech);
    CryLog("creation of Technique %s...\n", tech.Name.data());
    //curTechnique = curContainer->createTechnique($2->c_str())->getExInterface();
    //curAnnotations = curTechnique->annotations()->getExInterface();
    //delete $2;
}
#line 1100 "/cygdrive/c/Users/chiap/source/repos/fromasmtodisasm/TestEngine/Code/Engine/Renderer/Shaders/Parser.cpp"
    break;

  case 120: // tech: TECHNIQUE IDENTIFIER $@6 annotations '{' passes '}'
#line 545 "/cygdrive/c/Users/chiap/source/repos/fromasmtodisasm/TestEngine/Code/Engine/Renderer/Shaders/Parser.yy"
                             { 
    lex_pop_state();
    //curAnnotations = NULL;
}
#line 1109 "/cygdrive/c/Users/chiap/source/repos/fromasmtodisasm/TestEngine/Code/Engine/Renderer/Shaders/Parser.cpp"
    break;

  case 133: // annotation_value: IDENTIFIER
#line 562 "/cygdrive/c/Users/chiap/source/repos/fromasmtodisasm/TestEngine/Code/Engine/Renderer/Shaders/Parser.yy"
                                                                   {CryLog("annotation IDENTIFIER");}
#line 1115 "/cygdrive/c/Users/chiap/source/repos/fromasmtodisasm/TestEngine/Code/Engine/Renderer/Shaders/Parser.cpp"
    break;

  case 138: // annotation: annotation_base ';'
#line 566 "/cygdrive/c/Users/chiap/source/repos/fromasmtodisasm/TestEngine/Code/Engine/Renderer/Shaders/Parser.yy"
                                                   {
    //CryLog("annotation: %s = ", $IDENTIFIER.c_str());
/*
    if(!curAnnotations)
        curAnnotations = IAnnotationEx::getAnnotationSingleton(2); // need a temporary place since nothing was initialized
    if(!curAnnotations->addAnnotation($2->c_str(), $4->c_str()))
        yyerror("err\n");
    delete $4;
*/
    }
#line 1130 "/cygdrive/c/Users/chiap/source/repos/fromasmtodisasm/TestEngine/Code/Engine/Renderer/Shaders/Parser.cpp"
    break;

  case 139: // annotations: %empty
#line 579 "/cygdrive/c/Users/chiap/source/repos/fromasmtodisasm/TestEngine/Code/Engine/Renderer/Shaders/Parser.yy"
        {annotations = std::vector<string>();}
#line 1136 "/cygdrive/c/Users/chiap/source/repos/fromasmtodisasm/TestEngine/Code/Engine/Renderer/Shaders/Parser.cpp"
    break;

  case 141: // hlsl_header: HLSL11SHADER IDENTIFIER
#line 585 "/cygdrive/c/Users/chiap/source/repos/fromasmtodisasm/TestEngine/Code/Engine/Renderer/Shaders/Parser.yy"
                                {
        yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); 
    }
#line 1144 "/cygdrive/c/Users/chiap/source/repos/fromasmtodisasm/TestEngine/Code/Engine/Renderer/Shaders/Parser.cpp"
    break;

  case 142: // hlsl_header: HLSL11SHADER
#line 588 "/cygdrive/c/Users/chiap/source/repos/fromasmtodisasm/TestEngine/Code/Engine/Renderer/Shaders/Parser.yy"
                       {
        yylhs.value.as < std::string > () = "Common";
        is_common = true;
    }
#line 1153 "/cygdrive/c/Users/chiap/source/repos/fromasmtodisasm/TestEngine/Code/Engine/Renderer/Shaders/Parser.cpp"
    break;

  case 143: // hlsl: shader_header '{' CODEBODY
#line 594 "/cygdrive/c/Users/chiap/source/repos/fromasmtodisasm/TestEngine/Code/Engine/Renderer/Shaders/Parser.yy"
                                 { 
		//gEnv->pLog->Log("$3 Shader $1%s $3parsed", $1.data()); 
        driver.currentEffect->m_shaders.push_back(IEffect::ShaderInfo{yystack_[2].value.as < std::string > (), yystack_[0].value.as < std::string > ()});
        if (yystack_[2].value.as < std::string > () == "Common")
        {
            Code.push_back(driver.currentEffect->m_shaders.back().data);
            is_common = false;
        }
        #if 0
        if (gEnv->pConsole->GetCVar("dump_shaders_on_load")->GetIVal())
            CryLog("Current shader[%s] code in file %s:\n%s", yystack_[2].value.as < std::string > ().data(), driver.file.data(), driver.currentEffect->m_shaders.back().data.data());
        #endif

	}
#line 1172 "/cygdrive/c/Users/chiap/source/repos/fromasmtodisasm/TestEngine/Code/Engine/Renderer/Shaders/Parser.cpp"
    break;

  case 144: // shader_header: hlsl_header annotations
#line 610 "/cygdrive/c/Users/chiap/source/repos/fromasmtodisasm/TestEngine/Code/Engine/Renderer/Shaders/Parser.yy"
                                        { yylhs.value.as < std::string > () = yystack_[1].value.as < std::string > (); }
#line 1178 "/cygdrive/c/Users/chiap/source/repos/fromasmtodisasm/TestEngine/Code/Engine/Renderer/Shaders/Parser.cpp"
    break;


#line 1182 "/cygdrive/c/Users/chiap/source/repos/fromasmtodisasm/TestEngine/Code/Engine/Renderer/Shaders/Parser.cpp"

            default:
              break;
            }
        }
#if YY_EXCEPTIONS
      catch (const syntax_error& yyexc)
        {
          YYCDEBUG << "Caught exception: " << yyexc.what() << '\n';
          error (yyexc);
          YYERROR;
        }
#endif // YY_EXCEPTIONS
      YY_SYMBOL_PRINT ("-> $$ =", yylhs);
      yypop_ (yylen);
      yylen = 0;

      // Shift the result of the reduction.
      yypush_ (YY_NULLPTR, YY_MOVE (yylhs));
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
        context yyctx (*this, yyla);
        std::string msg = yysyntax_error_ (yyctx);
        error (yyla.location, YY_MOVE (msg));
      }


    yyerror_range[1].location = yyla.location;
    if (yyerrstatus_ == 3)
      {
        /* If just tried and failed to reuse lookahead token after an
           error, discard it.  */

        // Return failure if at end of input.
        if (yyla.kind () == symbol_kind::S_YYEOF)
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
    /* Pacify compilers when the user code never invokes YYERROR and
       the label yyerrorlab therefore never appears in user code.  */
    if (false)
      YYERROR;

    /* Do not reclaim the symbols of the rule whose action triggered
       this YYERROR.  */
    yypop_ (yylen);
    yylen = 0;
    YY_STACK_PRINT ();
    goto yyerrlab1;


  /*-------------------------------------------------------------.
  | yyerrlab1 -- common code for both syntax error and YYERROR.  |
  `-------------------------------------------------------------*/
  yyerrlab1:
    yyerrstatus_ = 3;   // Each real token shifted decrements this.
    // Pop stack until we find a state that shifts the error token.
    for (;;)
      {
        yyn = yypact_[+yystack_[0].state];
        if (!yy_pact_value_is_default_ (yyn))
          {
            yyn += symbol_kind::S_YYerror;
            if (0 <= yyn && yyn <= yylast_
                && yycheck_[yyn] == symbol_kind::S_YYerror)
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
    {
      stack_symbol_type error_token;

      yyerror_range[2].location = yyla.location;
      YYLLOC_DEFAULT (error_token.location, yyerror_range, 2);

      // Shift the error token.
      error_token.state = state_type (yyn);
      yypush_ ("Shifting", YY_MOVE (error_token));
    }
    goto yynewstate;


  /*-------------------------------------.
  | yyacceptlab -- YYACCEPT comes here.  |
  `-------------------------------------*/
  yyacceptlab:
    yyresult = 0;
    goto yyreturn;


  /*-----------------------------------.
  | yyabortlab -- YYABORT comes here.  |
  `-----------------------------------*/
  yyabortlab:
    yyresult = 1;
    goto yyreturn;


  /*-----------------------------------------------------.
  | yyreturn -- parsing is finished, return the result.  |
  `-----------------------------------------------------*/
  yyreturn:
    if (!yyla.empty ())
      yy_destroy_ ("Cleanup: discarding lookahead", yyla);

    /* Do not reclaim the symbols of the rule whose action triggered
       this YYABORT or YYACCEPT.  */
    yypop_ (yylen);
    YY_STACK_PRINT ();
    while (1 < yystack_.size ())
      {
        yy_destroy_ ("Cleanup: popping", yystack_[0]);
        yypop_ ();
      }

    return yyresult;
  }
#if YY_EXCEPTIONS
    catch (...)
      {
        YYCDEBUG << "Exception caught: cleaning lookahead and stack\n";
        // Do not try to display the values of the reclaimed symbols,
        // as their printers might throw an exception.
        if (!yyla.empty ())
          yy_destroy_ (YY_NULLPTR, yyla);

        while (1 < yystack_.size ())
          {
            yy_destroy_ (YY_NULLPTR, yystack_[0]);
            yypop_ ();
          }
        throw;
      }
#endif // YY_EXCEPTIONS
  }

  void
  parser::error (const syntax_error& yyexc)
  {
    error (yyexc.location, yyexc.what ());
  }

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
        std::string yyr;
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
              else
                goto append;

            append:
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

  std::string
  parser::symbol_name (symbol_kind_type yysymbol)
  {
    return yytnamerr_ (yytname_[yysymbol]);
  }



  // parser::context.
  parser::context::context (const parser& yyparser, const symbol_type& yyla)
    : yyparser_ (yyparser)
    , yyla_ (yyla)
  {}

  int
  parser::context::expected_tokens (symbol_kind_type yyarg[], int yyargn) const
  {
    // Actual number of expected tokens
    int yycount = 0;

    const int yyn = yypact_[+yyparser_.yystack_[0].state];
    if (!yy_pact_value_is_default_ (yyn))
      {
        /* Start YYX at -YYN if negative to avoid negative indexes in
           YYCHECK.  In other words, skip the first -YYN actions for
           this state because they are default actions.  */
        const int yyxbegin = yyn < 0 ? -yyn : 0;
        // Stay within bounds of both yycheck and yytname.
        const int yychecklim = yylast_ - yyn + 1;
        const int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
        for (int yyx = yyxbegin; yyx < yyxend; ++yyx)
          if (yycheck_[yyx + yyn] == yyx && yyx != symbol_kind::S_YYerror
              && !yy_table_value_is_error_ (yytable_[yyx + yyn]))
            {
              if (!yyarg)
                ++yycount;
              else if (yycount == yyargn)
                return 0;
              else
                yyarg[yycount++] = YY_CAST (symbol_kind_type, yyx);
            }
      }

    if (yyarg && yycount == 0 && 0 < yyargn)
      yyarg[0] = symbol_kind::S_YYEMPTY;
    return yycount;
  }






  int
  parser::yy_syntax_error_arguments_ (const context& yyctx,
                                                 symbol_kind_type yyarg[], int yyargn) const
  {
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
         scanner and before detecting a syntax error.  Thus, state merging
         (from LALR or IELR) and default reductions corrupt the expected
         token list.  However, the list is correct for canonical LR with
         one exception: it will still contain any token that will not be
         accepted due to an error action in a later state.
    */

    if (!yyctx.lookahead ().empty ())
      {
        if (yyarg)
          yyarg[0] = yyctx.token ();
        int yyn = yyctx.expected_tokens (yyarg ? yyarg + 1 : yyarg, yyargn - 1);
        return yyn + 1;
      }
    return 0;
  }

  // Generate an error message.
  std::string
  parser::yysyntax_error_ (const context& yyctx) const
  {
    // Its maximum.
    enum { YYARGS_MAX = 5 };
    // Arguments of yyformat.
    symbol_kind_type yyarg[YYARGS_MAX];
    int yycount = yy_syntax_error_arguments_ (yyctx, yyarg, YYARGS_MAX);

    char const* yyformat = YY_NULLPTR;
    switch (yycount)
      {
#define YYCASE_(N, S)                         \
        case N:                               \
          yyformat = S;                       \
        break
      default: // Avoid compiler warnings.
        YYCASE_ (0, YY_("syntax error"));
        YYCASE_ (1, YY_("syntax error, unexpected %s"));
        YYCASE_ (2, YY_("syntax error, unexpected %s, expecting %s"));
        YYCASE_ (3, YY_("syntax error, unexpected %s, expecting %s or %s"));
        YYCASE_ (4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
        YYCASE_ (5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
#undef YYCASE_
      }

    std::string yyres;
    // Argument number.
    std::ptrdiff_t yyi = 0;
    for (char const* yyp = yyformat; *yyp; ++yyp)
      if (yyp[0] == '%' && yyp[1] == 's' && yyi < yycount)
        {
          yyres += symbol_name (yyarg[yyi++]);
          ++yyp;
        }
      else
        yyres += *yyp;
    return yyres;
  }


  const signed char parser::yypact_ninf_ = -124;

  const short parser::yytable_ninf_ = -140;

  const short
  parser::yypact_[] =
  {
    -124,   160,  -124,  -124,  -124,    13,    39,    68,    27,  -124,
    -124,  -124,  -124,  -124,  -124,  -124,  -124,  -124,  -124,  -124,
    -124,  -124,  -124,  -124,  -124,  -124,    88,   106,  -124,   139,
    -124,  -124,     9,   -72,  -124,  -124,  -124,  -124,  -124,  -124,
    -124,  -124,  -124,    58,  -124,    81,  -124,  -124,    86,   120,
     237,   205,   120,   120,  -124,   126,   129,    14,   116,   208,
     132,   127,  -124,   201,    58,   204,   141,   135,   130,   120,
     133,   133,   219,   217,  -124,  -124,   140,    19,    94,    -2,
      90,  -124,  -124,  -124,  -124,  -124,  -124,  -124,   221,    69,
     136,   146,  -124,  -124,   145,   226,  -124,     7,   138,   219,
    -124,  -124,   227,  -124,  -124,    14,   150,     1,   151,  -124,
    -124,   125,  -124,  -124,  -124,   237,   148,     4,  -124,  -124,
    -124,     3,  -124,  -124,   -19,   142,    23,   222,  -124,  -124,
    -124,    60,  -124,   204,  -124,    58,  -124,  -124,  -124,   137,
      21,  -124,  -124,  -124,   155,  -124,  -124,  -124,   237,   174,
    -124,   274,  -124,   -23,   193,     3,   276,  -124,  -124,   131,
    -124,  -124,  -124,  -124,  -124,  -124,   231,  -124,     8,    58,
     197,  -124,  -124,   196,  -124,  -124,   200,  -124,  -124,  -124,
    -124,    84,  -124,  -124,  -124,   199,   202,   203,   206,   207,
    -124,  -124,  -124,  -124,  -124,   197,    96,  -124,  -124,   125,
    -124,   167,   167,   167,   167,   167,  -124,  -124,  -124,  -124,
     194,  -124,    79,  -124,   209,   210,   211,   213,   214,    18,
    -124,  -124,  -124,   167,   167,   167,  -124,   215,   220,    19,
     218,   216,   223,  -124,  -124,   167,   167,   224,   225,  -124,
     167,   228,  -124
  };

  const unsigned char
  parser::yydefact_[] =
  {
       2,     0,     1,    12,    21,   142,   117,     0,     0,    59,
      60,    61,    62,    71,    72,    73,    74,    75,    67,    68,
      69,    70,    63,    64,    65,    66,     0,     0,    25,    35,
       3,     8,     0,     9,    10,    42,    11,     7,    43,    44,
      22,     6,     4,   139,     5,     0,   141,   119,     0,    29,
       0,     0,    29,    29,    34,     0,    89,     0,    97,     0,
       0,     0,   144,     0,   139,     0,     0,     0,     0,    29,
      45,    45,     0,    38,    23,    89,     0,    84,     0,     0,
     106,    96,    19,   126,   125,   124,   123,   135,     0,     0,
       0,   137,   128,   143,     0,   110,   114,     0,     0,     0,
      31,    32,     0,    47,    48,     0,     0,     0,     0,    37,
      33,    15,    90,    87,    95,    86,     0,     0,    76,    78,
      77,     0,    92,    80,    85,     0,     0,     0,   134,   140,
     127,     0,   138,     0,   112,   139,   116,   118,   115,     0,
       0,    46,    41,    36,     0,    40,    13,    14,     0,     0,
      93,     0,    94,     0,     0,     0,     0,    91,   109,     0,
      20,   133,   129,   130,   121,   132,   131,   136,     0,   139,
       0,    27,    26,     0,    30,    39,    16,    24,    88,    79,
      82,     0,    83,    99,    98,     0,     0,     0,     0,     0,
     108,   105,   107,   122,   120,     0,    57,   111,    28,    15,
      81,     0,     0,     0,     0,     0,   113,    49,    51,    50,
       0,    55,     0,    17,     0,     0,     0,     0,     0,     0,
      58,    56,   100,     0,     0,     0,   101,    89,     0,    52,
       0,     0,     0,    54,   102,     0,     0,     0,     0,   103,
       0,     0,   104
  };

  const short
  parser::yypgoto_[] =
  {
    -124,  -124,  -124,    87,  -124,  -124,    15,  -124,  -124,  -124,
    -124,    97,  -124,  -124,   288,  -124,  -124,  -124,   -55,  -124,
     230,  -124,  -124,  -124,  -124,    78,  -124,   108,  -124,  -124,
     -79,  -124,  -124,  -124,  -124,   -57,   247,  -123,  -124,  -124,
       0,   -83,  -124,  -124,   176,  -124,  -124,  -124,  -124,  -124,
    -124,  -124,  -124,  -124,   229,   -61,  -124,  -124,  -124
  };

  const unsigned char
  parser::yydefgoto_[] =
  {
       0,     1,   148,   149,    31,   127,   105,    33,   111,    34,
     173,    67,    35,    51,   106,    55,    73,   110,   107,    37,
     103,    38,    39,   210,   228,   211,   212,   197,    40,   123,
     124,    76,   125,   116,   117,    58,    60,   190,   191,   192,
     108,    96,   135,   169,    97,    42,    48,    64,   166,    88,
      89,   167,    90,    91,    92,    62,    43,    44,    45
  };

  const short
  parser::yytable_[] =
  {
      77,    41,   118,    94,     4,   119,   120,   118,   136,   136,
     119,   120,   -97,    56,   138,    59,    32,    46,    75,   126,
      95,    95,   227,   115,     4,     9,    10,    11,    12,    13,
      14,    15,    16,    17,    18,    19,    20,    21,    22,    23,
      24,    25,   153,    47,   140,     9,    10,    11,    12,    13,
      14,    15,    16,    17,    18,    19,    20,    21,    22,    23,
      24,    25,   154,   179,   161,    68,   154,   162,   163,   155,
     164,   156,    49,   155,   170,   156,   181,   165,   214,   215,
     216,   217,   218,   121,    83,   138,    84,   151,   121,   143,
     152,   122,    52,    85,    57,   137,   194,     4,   113,    57,
     230,   231,   232,    57,    78,   158,    86,   144,   195,   174,
      53,    79,   237,   238,   159,   144,    50,   241,     9,    10,
      11,    12,    13,    14,    15,    16,    17,    18,    19,    20,
      21,    22,    23,    24,    25,   207,   208,   209,   183,   184,
     144,   171,    83,    54,    84,   172,    87,    61,   176,    70,
      71,    85,   207,   208,   209,   185,   186,   187,   188,   129,
       2,     3,   229,     4,    86,    63,   101,   220,   189,   154,
      65,     5,     6,    29,   183,   184,   155,   200,   156,    61,
     114,  -139,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    78,    26,    59,    87,   146,   147,    66,    79,    69,
      72,   -18,    81,    27,    74,    93,    82,    95,    98,    99,
     100,   109,     4,   139,   102,   128,   112,   131,   132,   133,
     134,   141,   142,   145,   150,   157,   160,   175,    28,    29,
       4,   193,    30,     9,    10,    11,    12,    13,    14,    15,
      16,    17,    18,    19,    20,    21,    22,    23,    24,    25,
     177,     9,    10,    11,    12,    13,    14,    15,    16,    17,
      18,    19,    20,    21,    22,    23,    24,    25,   178,   180,
     182,   196,   198,   199,   201,   219,   213,   202,   203,    36,
     221,   204,   205,   223,   224,   222,   225,   -53,    29,   235,
     226,   104,   233,   206,   234,    80,   236,     0,   240,   168,
     239,     0,     0,     0,   242,     0,     0,     0,   130
  };

  const short
  parser::yycheck_[] =
  {
      57,     1,     4,    64,     3,     7,     8,     4,     1,     1,
       7,     8,    84,     4,    97,    87,     1,     4,     4,    80,
      13,    13,     4,    78,     3,    24,    25,    26,    27,    28,
      29,    30,    31,    32,    33,    34,    35,    36,    37,    38,
      39,    40,   121,     4,    99,    24,    25,    26,    27,    28,
      29,    30,    31,    32,    33,    34,    35,    36,    37,    38,
      39,    40,    85,    86,     4,    50,    85,     7,     8,    92,
      10,    94,     4,    92,   135,    94,   155,    17,   201,   202,
     203,   204,   205,    85,    15,   168,    17,    83,    85,    88,
      86,    93,     4,    24,    85,    88,    88,     3,     4,    85,
     223,   224,   225,    85,    85,    82,    37,   107,   169,    88,
       4,    92,   235,   236,    91,   115,    89,   240,    24,    25,
      26,    27,    28,    29,    30,    31,    32,    33,    34,    35,
      36,    37,    38,    39,    40,    56,    57,    58,     7,     8,
     140,     4,    15,     4,    17,     8,    77,    89,   148,    52,
      53,    24,    56,    57,    58,    24,    25,    26,    27,    90,
       0,     1,   219,     3,    37,    84,    69,    88,    37,    85,
      84,    11,    12,    79,     7,     8,    92,    93,    94,    89,
      86,    91,    22,    23,    24,    25,    26,    27,    28,    29,
      30,    31,    32,    33,    34,    35,    36,    37,    38,    39,
      40,    85,    42,    87,    77,    80,    81,    87,    92,     4,
      84,    86,     4,    53,    85,    14,    84,    13,    77,    84,
      90,     4,     3,    85,    91,     4,    86,    91,    82,    84,
       4,     4,    82,    82,    86,    93,    14,    82,    78,    79,
       3,    10,    82,    24,    25,    26,    27,    28,    29,    30,
      31,    32,    33,    34,    35,    36,    37,    38,    39,    40,
      86,    24,    25,    26,    27,    28,    29,    30,    31,    32,
      33,    34,    35,    36,    37,    38,    39,    40,     4,    86,
       4,    84,    86,    83,    85,    91,   199,    85,    85,     1,
     212,    85,    85,    83,    83,    86,    83,    82,    79,    83,
      86,    71,    82,   195,    86,    58,    83,    -1,    83,   133,
      86,    -1,    -1,    -1,    86,    -1,    -1,    -1,    89
  };

  const unsigned char
  parser::yystos_[] =
  {
       0,    96,     0,     1,     3,    11,    12,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    42,    53,    78,    79,
      82,    99,   101,   102,   104,   107,   109,   114,   116,   117,
     123,   135,   140,   151,   152,   153,     4,     4,   141,     4,
      89,   108,     4,     4,     4,   110,     4,    85,   130,    87,
     131,    89,   150,    84,   142,    84,    87,   106,   101,     4,
     106,   106,    84,   111,    85,     4,   126,   130,    85,    92,
     131,     4,    84,    15,    17,    24,    37,    77,   144,   145,
     147,   148,   149,    14,   150,    13,   136,   139,    77,    84,
      90,   106,    91,   115,   115,   101,   109,   113,   135,     4,
     112,   103,    86,     4,    86,   113,   128,   129,     4,     7,
       8,    85,    93,   124,   125,   127,   150,   100,     4,    90,
     149,    91,    82,    84,     4,   137,     1,    88,   136,    85,
     113,     4,    82,    88,   135,    82,    80,    81,    97,    98,
      86,    83,    86,   125,    85,    92,    94,    93,    82,    91,
      14,     4,     7,     8,    10,    17,   143,   146,   139,   138,
     150,     4,     8,   105,    88,    82,   135,    86,     4,    86,
      86,   125,     4,     7,     8,    24,    25,    26,    27,    37,
     132,   133,   134,    10,    88,   150,    84,   122,    86,    83,
      93,    85,    85,    85,    85,    85,   122,    56,    57,    58,
     118,   120,   121,    98,   132,   132,   132,   132,   132,    91,
      88,   120,    86,    83,    83,    83,    86,     4,   119,   130,
     132,   132,   132,    82,    86,    83,    83,   132,   132,    86,
      83,   132,    86
  };

  const unsigned char
  parser::yyr1_[] =
  {
       0,    95,    96,    96,    96,    96,    96,    96,    96,    96,
      96,    96,    96,    97,    97,    97,    98,    98,    98,   100,
      99,   101,   101,   103,   102,   104,   105,   105,   106,   106,
     107,   108,   107,   109,   110,   110,   111,   112,   112,   113,
     113,   113,   114,   114,   114,   115,   115,   116,   117,   118,
     118,   118,   119,   119,   120,   121,   121,   121,   122,   123,
     123,   123,   123,   123,   123,   123,   123,   123,   123,   123,
     123,   123,   123,   123,   123,   123,   124,   124,   124,   124,
     125,   125,   125,   125,   126,   127,   128,   129,   129,   130,
     130,   130,   130,   130,   130,   130,   131,   131,   132,   132,
     133,   133,   133,   133,   133,   134,   135,   135,   135,   135,
     137,   136,   138,   136,   139,   139,   139,   141,   140,   142,
     140,   143,   143,   144,   144,   144,   144,   145,   145,   146,
     146,   146,   146,   146,   147,   147,   148,   149,   149,   150,
     150,   151,   151,   152,   153
  };

  const signed char
  parser::yyr2_[] =
  {
       0,     2,     0,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     1,     1,     0,     2,     4,     0,     0,
       5,     1,     1,     0,     6,     1,     1,     1,     5,     0,
       6,     3,     4,     4,     1,     0,     3,     1,     0,     3,
       2,     2,     1,     1,     1,     0,     2,     4,     4,     1,
       1,     1,     1,     1,     4,     1,     2,     0,     3,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     3,
       1,     4,     3,     3,     1,     1,     1,     1,     3,     1,
       3,     4,     3,     4,     4,     3,     2,     0,     1,     1,
       4,     4,     6,     8,    10,     1,     3,     6,     6,     5,
       0,     4,     0,     5,     1,     2,     2,     0,     5,     0,
       7,     1,     2,     1,     1,     1,     1,     2,     1,     1,
       1,     1,     1,     1,     2,     1,     3,     1,     2,     0,
       3,     2,     1,     3,     2
  };


#if YYDEBUG || 1
  // YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
  // First, the terminals, then, starting at \a YYNTOKENS, nonterminals.
  const char*
  const parser::yytname_[] =
  {
  "\"end of file\"", "error", "\"invalid token\"", "TYPE_NAME",
  "IDENTIFIER", "TRUE", "FALSE", "FLOAT", "INT", "BOOL", "STR",
  "HLSL11SHADER", "TECHNIQUE", "PASS", "CODEBODY", "STRING_TYPE",
  "VOID_TYPE", "UNSIGNED", "HIGHP", "MEDIUMP", "LOWP", "UNIFORM",
  "CSTBUFFER", "CONSTANTBUFFER", "FLOAT_TYPE", "FLOAT2_TYPE",
  "FLOAT3_TYPE", "FLOAT4_TYPE", "MAT2_TYPE", "MAT2x4_TYPE", "MAT3_TYPE",
  "MAT34_TYPE", "MAT4_TYPE", "BOOL_TYPE", "BOOL2_TYPE", "BOOL3_TYPE",
  "BOOL4_TYPE", "INT_TYPE", "INT2_TYPE", "INT3_TYPE", "INT4_TYPE",
  "TEXTURE1D_TYPE", "TEXTURE2D_TYPE", "TEXTURE2DSHADOW_TYPE",
  "TEXTURE2DRECT_TYPE", "TEXTURE3D_TYPE", "TEXTURECUBE_TYPE",
  "SAMPLER1D_TYPE", "SAMPLER2D_TYPE", "SAMPLER2DSHADOW_TYPE",
  "SAMPLER2DRECT_TYPE", "SAMPLER3D_TYPE", "SAMPLERCUBE_TYPE",
  "SAMPLERSTATE", "EXTENSION", "SEPARATE_SHADER", "VERTEXPROGRAM",
  "FRAGMENTPROGRAM", "GEOMETRYPROGRAM", "HULLPROGRAM", "EVALPROGRAM",
  "SHDPROFILE", "SAMPLERRESOURCE", "SAMPLERTEXUNIT", "SETSAMPLERSTATE",
  "SETDSTSTATE", "SETRASTERIZATIONSTATE", "SETCOLORSAMPLESTATE",
  "IMAGERESOURCE", "IMAGEUNIT", "IMAGEACCESS", "IMAGELAYER",
  "IMAGELAYERED", "WRITE_ONLY", "READ_ONLY", "READ_WRITE", "VERTEXFORMAT",
  "REGISTER", "FATALERROR", "STRUCT", "INSPECYFIER", "OUTSPECYFIER", "';'",
  "','", "'{'", "'('", "')'", "':'", "'}'", "'<'", "'>'", "'='", "'['",
  "']'", "'.'", "$accept", "input", "var_spec", "arguments",
  "function_definition", "$@1", "object_type", "function_declaration",
  "$@2", "fatal_error", "register_value", "register_declaration",
  "cbuffer", "template_parameter", "struct", "struct_header",
  "struct_body", "struct_footer", "var_decls", "shader_resource",
  "resource_initializer", "texture2d", "sampler_state", "shader_type",
  "shader_assignment_shader", "shader_assignment", "shader_assignments",
  "passstates", "base_type", "primary_expression", "postfix_expression",
  "declarator", "constant_expression", "parameter_type_list",
  "identifier_list", "direct_declarator", "semantic", "value",
  "basic_type_constructor", "type_constructor", "var_decl", "pass", "$@3",
  "$@4", "passes", "tech", "$@5", "$@6", "STRING_LIST", "scalar_type",
  "annotation_list", "annotation_value", "annotation_header",
  "annotation_base", "annotation", "annotations", "hlsl_header", "hlsl",
  "shader_header", YY_NULLPTR
  };
#endif


#if YYDEBUG
  const short
  parser::yyrline_[] =
  {
       0,   274,   274,   275,   276,   277,   278,   279,   280,   281,
     282,   283,   284,   287,   287,   287,   289,   290,   291,   293,
     293,   301,   301,   303,   303,   308,   310,   310,   312,   312,
     314,   320,   322,   328,   333,   333,   334,   335,   336,   338,
     338,   338,   340,   340,   340,   342,   342,   343,   348,   354,
     355,   356,   359,   359,   361,   370,   371,   372,   379,   383,
     384,   385,   386,   387,   388,   389,   390,   391,   392,   393,
     394,   395,   396,   397,   398,   399,   406,   408,   409,   411,
     415,   416,   417,   419,   424,   426,   428,   431,   432,   436,
     437,   438,   439,   440,   441,   442,   445,   445,   448,   448,
     453,   454,   455,   456,   457,   458,   462,   466,   467,   468,
     481,   481,   496,   496,   524,   525,   526,   532,   532,   537,
     537,   555,   555,   556,   556,   556,   556,   559,   560,   562,
     562,   562,   562,   562,   564,   564,   565,   566,   566,   579,
     580,   585,   588,   594,   610
  };

  void
  parser::yy_stack_print_ () const
  {
    *yycdebug_ << "Stack now";
    for (stack_type::const_iterator
           i = yystack_.begin (),
           i_end = yystack_.end ();
         i != i_end; ++i)
      *yycdebug_ << ' ' << int (i->state);
    *yycdebug_ << '\n';
  }

  void
  parser::yy_reduce_print_ (int yyrule) const
  {
    int yylno = yyrline_[yyrule];
    int yynrhs = yyr2_[yyrule];
    // Print the symbols being reduced, and their result.
    *yycdebug_ << "Reducing stack by rule " << yyrule - 1
               << " (line " << yylno << "):\n";
    // The symbols being reduced.
    for (int yyi = 0; yyi < yynrhs; yyi++)
      YY_SYMBOL_PRINT ("   $" << yyi + 1 << " =",
                       yystack_[(yynrhs) - (yyi + 1)]);
  }
#endif // YYDEBUG


} // yy
#line 1860 "/cygdrive/c/Users/chiap/source/repos/fromasmtodisasm/TestEngine/Code/Engine/Renderer/Shaders/Parser.cpp"

#line 612 "/cygdrive/c/Users/chiap/source/repos/fromasmtodisasm/TestEngine/Code/Engine/Renderer/Shaders/Parser.yy"


#include <sstream>
void
yy::parser::error(const location_type& l, const std::string& m)
{
  std::stringstream ss;
  ss << l << ": " << m;
  CryError(ss.str().c_str());    
}
#pragma warning(pop)
