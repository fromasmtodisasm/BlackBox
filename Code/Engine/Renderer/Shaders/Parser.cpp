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
#line 120 "/cygdrive/c/Users/chiap_000/source/repos/fromasmtodisasm/TestEngine/Code/Engine/Renderer/Shaders/Parser.yy"

	#pragma warning(push, 0)
    #include "Driver.hpp"
    #include "Scanner.hpp"
    #include "location.hh"

    #define GreenLog(...) gEnv->pLog->Log(__VA_ARGS__)

    #include "Effect.hpp"

    #ifdef S_FALSE
    #undef S_FALSE
    #endif

    bool is_common;
    std::vector<string> annotations;

    static yy::parser::symbol_type yylex(Scanner &scanner, Driver& driver) {
        return scanner.ScanToken();
    }

    using Type = std::string;

#line 63 "/cygdrive/c/Users/chiap_000/source/repos/fromasmtodisasm/TestEngine/Code/Engine/Renderer/Shaders/Parser.cpp"




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
#line 162 "/cygdrive/c/Users/chiap_000/source/repos/fromasmtodisasm/TestEngine/Code/Engine/Renderer/Shaders/Parser.cpp"

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
      case symbol_kind::S_shader_assignment_shader: // shader_assignment_shader
      case symbol_kind::S_declarator: // declarator
      case symbol_kind::S_direct_declarator: // direct_declarator
        value.YY_MOVE_OR_COPY< DirectDeclarator > (YY_MOVE (that.value));
        break;

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
      case symbol_kind::S_shader_assignment_shader: // shader_assignment_shader
      case symbol_kind::S_declarator: // declarator
      case symbol_kind::S_direct_declarator: // direct_declarator
        value.move< DirectDeclarator > (YY_MOVE (that.value));
        break;

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
      case symbol_kind::S_shader_assignment_shader: // shader_assignment_shader
      case symbol_kind::S_declarator: // declarator
      case symbol_kind::S_direct_declarator: // direct_declarator
        value.copy< DirectDeclarator > (that.value);
        break;

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
      case symbol_kind::S_shader_assignment_shader: // shader_assignment_shader
      case symbol_kind::S_declarator: // declarator
      case symbol_kind::S_direct_declarator: // direct_declarator
        value.move< DirectDeclarator > (that.value);
        break;

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
#line 111 "/cygdrive/c/Users/chiap_000/source/repos/fromasmtodisasm/TestEngine/Code/Engine/Renderer/Shaders/Parser.yy"
{
    is_common = false;
}

#line 600 "/cygdrive/c/Users/chiap_000/source/repos/fromasmtodisasm/TestEngine/Code/Engine/Renderer/Shaders/Parser.cpp"


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
      case symbol_kind::S_shader_assignment_shader: // shader_assignment_shader
      case symbol_kind::S_declarator: // declarator
      case symbol_kind::S_direct_declarator: // direct_declarator
        yylhs.value.emplace< DirectDeclarator > ();
        break;

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
#line 280 "/cygdrive/c/Users/chiap_000/source/repos/fromasmtodisasm/TestEngine/Code/Engine/Renderer/Shaders/Parser.yy"
              { CryLog("Empty effect"); }
#line 781 "/cygdrive/c/Users/chiap_000/source/repos/fromasmtodisasm/TestEngine/Code/Engine/Renderer/Shaders/Parser.cpp"
    break;

  case 8: // input: input function_declaration
#line 286 "/cygdrive/c/Users/chiap_000/source/repos/fromasmtodisasm/TestEngine/Code/Engine/Renderer/Shaders/Parser.yy"
                             {CryLog("Pop lex state from declaration"); lex_pop_state();}
#line 787 "/cygdrive/c/Users/chiap_000/source/repos/fromasmtodisasm/TestEngine/Code/Engine/Renderer/Shaders/Parser.cpp"
    break;

  case 18: // $@1: %empty
#line 298 "/cygdrive/c/Users/chiap_000/source/repos/fromasmtodisasm/TestEngine/Code/Engine/Renderer/Shaders/Parser.yy"
                                                       { 
    //CryLog("Open function scope");
    }
#line 795 "/cygdrive/c/Users/chiap_000/source/repos/fromasmtodisasm/TestEngine/Code/Engine/Renderer/Shaders/Parser.cpp"
    break;

  case 19: // function_definition: function_declaration semantic '{' $@1 CODEBODY
#line 300 "/cygdrive/c/Users/chiap_000/source/repos/fromasmtodisasm/TestEngine/Code/Engine/Renderer/Shaders/Parser.yy"
               {
    //CryLog("Close function scope"); 
    auto body = yystack_[0].value.as < std::string > ();
    //CryLog("FuncBody: %s", body.data());
}
#line 805 "/cygdrive/c/Users/chiap_000/source/repos/fromasmtodisasm/TestEngine/Code/Engine/Renderer/Shaders/Parser.cpp"
    break;

  case 22: // $@2: %empty
#line 308 "/cygdrive/c/Users/chiap_000/source/repos/fromasmtodisasm/TestEngine/Code/Engine/Renderer/Shaders/Parser.yy"
                                                       {}
#line 811 "/cygdrive/c/Users/chiap_000/source/repos/fromasmtodisasm/TestEngine/Code/Engine/Renderer/Shaders/Parser.cpp"
    break;

  case 23: // function_declaration: object_type IDENTIFIER '(' $@2 arguments ')'
#line 308 "/cygdrive/c/Users/chiap_000/source/repos/fromasmtodisasm/TestEngine/Code/Engine/Renderer/Shaders/Parser.yy"
                                                                      {
    CryLog("Parsed function declaration for: [%s]", yystack_[4].value.as < std::string > ().data());
    lex_print_state();
}
#line 820 "/cygdrive/c/Users/chiap_000/source/repos/fromasmtodisasm/TestEngine/Code/Engine/Renderer/Shaders/Parser.cpp"
    break;

  case 24: // fatal_error: FATALERROR
#line 313 "/cygdrive/c/Users/chiap_000/source/repos/fromasmtodisasm/TestEngine/Code/Engine/Renderer/Shaders/Parser.yy"
                        { CryFatalError("Stopping paring!!!"); }
#line 826 "/cygdrive/c/Users/chiap_000/source/repos/fromasmtodisasm/TestEngine/Code/Engine/Renderer/Shaders/Parser.cpp"
    break;

  case 29: // cbuffer: CSTBUFFER IDENTIFIER register_declaration '{' var_decls '}'
#line 320 "/cygdrive/c/Users/chiap_000/source/repos/fromasmtodisasm/TestEngine/Code/Engine/Renderer/Shaders/Parser.yy"
{
    CryLog("New CBuffer %s", yystack_[4].value.as < std::string > ().data());
    lex_pop_state();
}
#line 835 "/cygdrive/c/Users/chiap_000/source/repos/fromasmtodisasm/TestEngine/Code/Engine/Renderer/Shaders/Parser.cpp"
    break;

  case 30: // cbuffer: CONSTANTBUFFER template_parameter IDENTIFIER register_declaration
#line 325 "/cygdrive/c/Users/chiap_000/source/repos/fromasmtodisasm/TestEngine/Code/Engine/Renderer/Shaders/Parser.yy"
{
    CryLog("New CBuffer %s", yystack_[1].value.as < std::string > ().data());
    lex_pop_state();
}
#line 844 "/cygdrive/c/Users/chiap_000/source/repos/fromasmtodisasm/TestEngine/Code/Engine/Renderer/Shaders/Parser.cpp"
    break;

  case 32: // struct: STRUCT struct_header struct_body struct_footer
#line 334 "/cygdrive/c/Users/chiap_000/source/repos/fromasmtodisasm/TestEngine/Code/Engine/Renderer/Shaders/Parser.yy"
{
    //CryLog("New Struct");
}
#line 852 "/cygdrive/c/Users/chiap_000/source/repos/fromasmtodisasm/TestEngine/Code/Engine/Renderer/Shaders/Parser.cpp"
    break;

  case 33: // struct_header: IDENTIFIER
#line 338 "/cygdrive/c/Users/chiap_000/source/repos/fromasmtodisasm/TestEngine/Code/Engine/Renderer/Shaders/Parser.yy"
                         {CryLog("StructName: %s", yystack_[0].value.as < std::string > ().data()); scanner.register_type(yystack_[0].value.as < std::string > ().data());}
#line 858 "/cygdrive/c/Users/chiap_000/source/repos/fromasmtodisasm/TestEngine/Code/Engine/Renderer/Shaders/Parser.cpp"
    break;

  case 34: // struct_header: %empty
#line 338 "/cygdrive/c/Users/chiap_000/source/repos/fromasmtodisasm/TestEngine/Code/Engine/Renderer/Shaders/Parser.yy"
                                                                                                           {yylhs.value.as < std::string > ()="";}
#line 864 "/cygdrive/c/Users/chiap_000/source/repos/fromasmtodisasm/TestEngine/Code/Engine/Renderer/Shaders/Parser.cpp"
    break;

  case 36: // struct_footer: IDENTIFIER
#line 340 "/cygdrive/c/Users/chiap_000/source/repos/fromasmtodisasm/TestEngine/Code/Engine/Renderer/Shaders/Parser.yy"
                          {CryLog("Declared and defined struct with name: %s", yystack_[0].value.as < std::string > ().data());}
#line 870 "/cygdrive/c/Users/chiap_000/source/repos/fromasmtodisasm/TestEngine/Code/Engine/Renderer/Shaders/Parser.cpp"
    break;

  case 37: // struct_footer: %empty
#line 341 "/cygdrive/c/Users/chiap_000/source/repos/fromasmtodisasm/TestEngine/Code/Engine/Renderer/Shaders/Parser.yy"
         {yylhs.value.as < std::string > ()="";}
#line 876 "/cygdrive/c/Users/chiap_000/source/repos/fromasmtodisasm/TestEngine/Code/Engine/Renderer/Shaders/Parser.cpp"
    break;

  case 46: // texture2d: TEXTURE2D_TYPE IDENTIFIER register_declaration resource_initializer
#line 349 "/cygdrive/c/Users/chiap_000/source/repos/fromasmtodisasm/TestEngine/Code/Engine/Renderer/Shaders/Parser.yy"
{
    CryLog("texture2d");
}
#line 884 "/cygdrive/c/Users/chiap_000/source/repos/fromasmtodisasm/TestEngine/Code/Engine/Renderer/Shaders/Parser.cpp"
    break;

  case 47: // sampler_state: SAMPLERSTATE IDENTIFIER register_declaration resource_initializer
#line 354 "/cygdrive/c/Users/chiap_000/source/repos/fromasmtodisasm/TestEngine/Code/Engine/Renderer/Shaders/Parser.yy"
{
    CryLog("sampler");
}
#line 892 "/cygdrive/c/Users/chiap_000/source/repos/fromasmtodisasm/TestEngine/Code/Engine/Renderer/Shaders/Parser.cpp"
    break;

  case 48: // shader_type: VERTEXPROGRAM
#line 359 "/cygdrive/c/Users/chiap_000/source/repos/fromasmtodisasm/TestEngine/Code/Engine/Renderer/Shaders/Parser.yy"
                {yylhs.value.as < IShader::Type > () = yystack_[0].value.as < IShader::Type > ();}
#line 898 "/cygdrive/c/Users/chiap_000/source/repos/fromasmtodisasm/TestEngine/Code/Engine/Renderer/Shaders/Parser.cpp"
    break;

  case 49: // shader_type: GEOMETRYPROGRAM
#line 360 "/cygdrive/c/Users/chiap_000/source/repos/fromasmtodisasm/TestEngine/Code/Engine/Renderer/Shaders/Parser.yy"
                  {yylhs.value.as < IShader::Type > () = yystack_[0].value.as < IShader::Type > ();}
#line 904 "/cygdrive/c/Users/chiap_000/source/repos/fromasmtodisasm/TestEngine/Code/Engine/Renderer/Shaders/Parser.cpp"
    break;

  case 50: // shader_type: FRAGMENTPROGRAM
#line 361 "/cygdrive/c/Users/chiap_000/source/repos/fromasmtodisasm/TestEngine/Code/Engine/Renderer/Shaders/Parser.yy"
                  {yylhs.value.as < IShader::Type > () = yystack_[0].value.as < IShader::Type > ();}
#line 910 "/cygdrive/c/Users/chiap_000/source/repos/fromasmtodisasm/TestEngine/Code/Engine/Renderer/Shaders/Parser.cpp"
    break;

  case 51: // shader_assignment_shader: direct_declarator
#line 364 "/cygdrive/c/Users/chiap_000/source/repos/fromasmtodisasm/TestEngine/Code/Engine/Renderer/Shaders/Parser.yy"
                                            {
    yylhs.value.as < DirectDeclarator > () = yystack_[0].value.as < DirectDeclarator > ();
}
#line 918 "/cygdrive/c/Users/chiap_000/source/repos/fromasmtodisasm/TestEngine/Code/Engine/Renderer/Shaders/Parser.cpp"
    break;

  case 52: // shader_assignment_shader: IDENTIFIER
#line 367 "/cygdrive/c/Users/chiap_000/source/repos/fromasmtodisasm/TestEngine/Code/Engine/Renderer/Shaders/Parser.yy"
             {
    yylhs.value.as < DirectDeclarator > () = DirectDeclarator{yystack_[0].value.as < std::string > ()};
}
#line 926 "/cygdrive/c/Users/chiap_000/source/repos/fromasmtodisasm/TestEngine/Code/Engine/Renderer/Shaders/Parser.cpp"
    break;

  case 53: // shader_assignment: shader_type '=' shader_assignment_shader ';'
#line 371 "/cygdrive/c/Users/chiap_000/source/repos/fromasmtodisasm/TestEngine/Code/Engine/Renderer/Shaders/Parser.yy"
                                                                {
    //$$ = std::make_pair($1, $3);
	driver.currentEffect->shader_assignment(yystack_[3].value.as < IShader::Type > (),yystack_[1].value.as < DirectDeclarator > ().Name);
}
#line 935 "/cygdrive/c/Users/chiap_000/source/repos/fromasmtodisasm/TestEngine/Code/Engine/Renderer/Shaders/Parser.cpp"
    break;

  case 58: // base_type: FLOAT_TYPE
#line 390 "/cygdrive/c/Users/chiap_000/source/repos/fromasmtodisasm/TestEngine/Code/Engine/Renderer/Shaders/Parser.yy"
              { yylhs.value.as < nvFX::IUniform::Type > () = nvFX::IUniform::TFloat; }
#line 941 "/cygdrive/c/Users/chiap_000/source/repos/fromasmtodisasm/TestEngine/Code/Engine/Renderer/Shaders/Parser.cpp"
    break;

  case 59: // base_type: FLOAT2_TYPE
#line 391 "/cygdrive/c/Users/chiap_000/source/repos/fromasmtodisasm/TestEngine/Code/Engine/Renderer/Shaders/Parser.yy"
               { yylhs.value.as < nvFX::IUniform::Type > () = nvFX::IUniform::TVec2; }
#line 947 "/cygdrive/c/Users/chiap_000/source/repos/fromasmtodisasm/TestEngine/Code/Engine/Renderer/Shaders/Parser.cpp"
    break;

  case 60: // base_type: FLOAT3_TYPE
#line 392 "/cygdrive/c/Users/chiap_000/source/repos/fromasmtodisasm/TestEngine/Code/Engine/Renderer/Shaders/Parser.yy"
               { yylhs.value.as < nvFX::IUniform::Type > () = nvFX::IUniform::TVec3; }
#line 953 "/cygdrive/c/Users/chiap_000/source/repos/fromasmtodisasm/TestEngine/Code/Engine/Renderer/Shaders/Parser.cpp"
    break;

  case 61: // base_type: FLOAT4_TYPE
#line 393 "/cygdrive/c/Users/chiap_000/source/repos/fromasmtodisasm/TestEngine/Code/Engine/Renderer/Shaders/Parser.yy"
               { yylhs.value.as < nvFX::IUniform::Type > () = nvFX::IUniform::TVec4; }
#line 959 "/cygdrive/c/Users/chiap_000/source/repos/fromasmtodisasm/TestEngine/Code/Engine/Renderer/Shaders/Parser.cpp"
    break;

  case 62: // base_type: INT_TYPE
#line 394 "/cygdrive/c/Users/chiap_000/source/repos/fromasmtodisasm/TestEngine/Code/Engine/Renderer/Shaders/Parser.yy"
               { yylhs.value.as < nvFX::IUniform::Type > () = nvFX::IUniform::TInt; }
#line 965 "/cygdrive/c/Users/chiap_000/source/repos/fromasmtodisasm/TestEngine/Code/Engine/Renderer/Shaders/Parser.cpp"
    break;

  case 63: // base_type: INT2_TYPE
#line 395 "/cygdrive/c/Users/chiap_000/source/repos/fromasmtodisasm/TestEngine/Code/Engine/Renderer/Shaders/Parser.yy"
             { yylhs.value.as < nvFX::IUniform::Type > () = nvFX::IUniform::TInt2; }
#line 971 "/cygdrive/c/Users/chiap_000/source/repos/fromasmtodisasm/TestEngine/Code/Engine/Renderer/Shaders/Parser.cpp"
    break;

  case 64: // base_type: INT3_TYPE
#line 396 "/cygdrive/c/Users/chiap_000/source/repos/fromasmtodisasm/TestEngine/Code/Engine/Renderer/Shaders/Parser.yy"
             { yylhs.value.as < nvFX::IUniform::Type > () = nvFX::IUniform::TInt3; }
#line 977 "/cygdrive/c/Users/chiap_000/source/repos/fromasmtodisasm/TestEngine/Code/Engine/Renderer/Shaders/Parser.cpp"
    break;

  case 65: // base_type: INT4_TYPE
#line 397 "/cygdrive/c/Users/chiap_000/source/repos/fromasmtodisasm/TestEngine/Code/Engine/Renderer/Shaders/Parser.yy"
             { yylhs.value.as < nvFX::IUniform::Type > () = nvFX::IUniform::TInt4; }
#line 983 "/cygdrive/c/Users/chiap_000/source/repos/fromasmtodisasm/TestEngine/Code/Engine/Renderer/Shaders/Parser.cpp"
    break;

  case 66: // base_type: BOOL_TYPE
#line 398 "/cygdrive/c/Users/chiap_000/source/repos/fromasmtodisasm/TestEngine/Code/Engine/Renderer/Shaders/Parser.yy"
             { yylhs.value.as < nvFX::IUniform::Type > () = nvFX::IUniform::TBool; }
#line 989 "/cygdrive/c/Users/chiap_000/source/repos/fromasmtodisasm/TestEngine/Code/Engine/Renderer/Shaders/Parser.cpp"
    break;

  case 67: // base_type: BOOL2_TYPE
#line 399 "/cygdrive/c/Users/chiap_000/source/repos/fromasmtodisasm/TestEngine/Code/Engine/Renderer/Shaders/Parser.yy"
              { yylhs.value.as < nvFX::IUniform::Type > () = nvFX::IUniform::TBool2; }
#line 995 "/cygdrive/c/Users/chiap_000/source/repos/fromasmtodisasm/TestEngine/Code/Engine/Renderer/Shaders/Parser.cpp"
    break;

  case 68: // base_type: BOOL3_TYPE
#line 400 "/cygdrive/c/Users/chiap_000/source/repos/fromasmtodisasm/TestEngine/Code/Engine/Renderer/Shaders/Parser.yy"
              { yylhs.value.as < nvFX::IUniform::Type > () = nvFX::IUniform::TBool3; }
#line 1001 "/cygdrive/c/Users/chiap_000/source/repos/fromasmtodisasm/TestEngine/Code/Engine/Renderer/Shaders/Parser.cpp"
    break;

  case 69: // base_type: BOOL4_TYPE
#line 401 "/cygdrive/c/Users/chiap_000/source/repos/fromasmtodisasm/TestEngine/Code/Engine/Renderer/Shaders/Parser.yy"
              { yylhs.value.as < nvFX::IUniform::Type > () = nvFX::IUniform::TBool4; }
#line 1007 "/cygdrive/c/Users/chiap_000/source/repos/fromasmtodisasm/TestEngine/Code/Engine/Renderer/Shaders/Parser.cpp"
    break;

  case 70: // base_type: MAT2_TYPE
#line 402 "/cygdrive/c/Users/chiap_000/source/repos/fromasmtodisasm/TestEngine/Code/Engine/Renderer/Shaders/Parser.yy"
             { yylhs.value.as < nvFX::IUniform::Type > () = nvFX::IUniform::TMat2; }
#line 1013 "/cygdrive/c/Users/chiap_000/source/repos/fromasmtodisasm/TestEngine/Code/Engine/Renderer/Shaders/Parser.cpp"
    break;

  case 71: // base_type: MAT2x4_TYPE
#line 403 "/cygdrive/c/Users/chiap_000/source/repos/fromasmtodisasm/TestEngine/Code/Engine/Renderer/Shaders/Parser.yy"
               { yylhs.value.as < nvFX::IUniform::Type > () = nvFX::IUniform::TMat2x4; }
#line 1019 "/cygdrive/c/Users/chiap_000/source/repos/fromasmtodisasm/TestEngine/Code/Engine/Renderer/Shaders/Parser.cpp"
    break;

  case 72: // base_type: MAT3_TYPE
#line 404 "/cygdrive/c/Users/chiap_000/source/repos/fromasmtodisasm/TestEngine/Code/Engine/Renderer/Shaders/Parser.yy"
             { yylhs.value.as < nvFX::IUniform::Type > () = nvFX::IUniform::TMat3; }
#line 1025 "/cygdrive/c/Users/chiap_000/source/repos/fromasmtodisasm/TestEngine/Code/Engine/Renderer/Shaders/Parser.cpp"
    break;

  case 73: // base_type: MAT34_TYPE
#line 405 "/cygdrive/c/Users/chiap_000/source/repos/fromasmtodisasm/TestEngine/Code/Engine/Renderer/Shaders/Parser.yy"
              { yylhs.value.as < nvFX::IUniform::Type > () = nvFX::IUniform::TMat3; }
#line 1031 "/cygdrive/c/Users/chiap_000/source/repos/fromasmtodisasm/TestEngine/Code/Engine/Renderer/Shaders/Parser.cpp"
    break;

  case 74: // base_type: MAT4_TYPE
#line 406 "/cygdrive/c/Users/chiap_000/source/repos/fromasmtodisasm/TestEngine/Code/Engine/Renderer/Shaders/Parser.yy"
             { yylhs.value.as < nvFX::IUniform::Type > () = nvFX::IUniform::TMat4; }
#line 1037 "/cygdrive/c/Users/chiap_000/source/repos/fromasmtodisasm/TestEngine/Code/Engine/Renderer/Shaders/Parser.cpp"
    break;

  case 83: // declarator: direct_declarator
#line 431 "/cygdrive/c/Users/chiap_000/source/repos/fromasmtodisasm/TestEngine/Code/Engine/Renderer/Shaders/Parser.yy"
            { yylhs.value.as < DirectDeclarator > () = yystack_[0].value.as < DirectDeclarator > (); }
#line 1043 "/cygdrive/c/Users/chiap_000/source/repos/fromasmtodisasm/TestEngine/Code/Engine/Renderer/Shaders/Parser.cpp"
    break;

  case 88: // direct_declarator: IDENTIFIER
#line 443 "/cygdrive/c/Users/chiap_000/source/repos/fromasmtodisasm/TestEngine/Code/Engine/Renderer/Shaders/Parser.yy"
                     {
        yylhs.value.as < DirectDeclarator > () = DirectDeclarator{yystack_[0].value.as < std::string > ()};
    }
#line 1051 "/cygdrive/c/Users/chiap_000/source/repos/fromasmtodisasm/TestEngine/Code/Engine/Renderer/Shaders/Parser.cpp"
    break;

  case 89: // direct_declarator: '(' declarator ')'
#line 446 "/cygdrive/c/Users/chiap_000/source/repos/fromasmtodisasm/TestEngine/Code/Engine/Renderer/Shaders/Parser.yy"
                             {yylhs.value.as < DirectDeclarator > () = yystack_[1].value.as < DirectDeclarator > ();}
#line 1057 "/cygdrive/c/Users/chiap_000/source/repos/fromasmtodisasm/TestEngine/Code/Engine/Renderer/Shaders/Parser.cpp"
    break;

  case 90: // direct_declarator: direct_declarator '[' constant_expression ']'
#line 447 "/cygdrive/c/Users/chiap_000/source/repos/fromasmtodisasm/TestEngine/Code/Engine/Renderer/Shaders/Parser.yy"
          { yylhs.value.as < DirectDeclarator > () = yystack_[3].value.as < DirectDeclarator > (); }
#line 1063 "/cygdrive/c/Users/chiap_000/source/repos/fromasmtodisasm/TestEngine/Code/Engine/Renderer/Shaders/Parser.cpp"
    break;

  case 91: // direct_declarator: direct_declarator '[' ']'
#line 448 "/cygdrive/c/Users/chiap_000/source/repos/fromasmtodisasm/TestEngine/Code/Engine/Renderer/Shaders/Parser.yy"
          { yylhs.value.as < DirectDeclarator > () = yystack_[2].value.as < DirectDeclarator > (); }
#line 1069 "/cygdrive/c/Users/chiap_000/source/repos/fromasmtodisasm/TestEngine/Code/Engine/Renderer/Shaders/Parser.cpp"
    break;

  case 92: // direct_declarator: direct_declarator '(' parameter_type_list ')'
#line 449 "/cygdrive/c/Users/chiap_000/source/repos/fromasmtodisasm/TestEngine/Code/Engine/Renderer/Shaders/Parser.yy"
          { yylhs.value.as < DirectDeclarator > () = yystack_[3].value.as < DirectDeclarator > (); }
#line 1075 "/cygdrive/c/Users/chiap_000/source/repos/fromasmtodisasm/TestEngine/Code/Engine/Renderer/Shaders/Parser.cpp"
    break;

  case 93: // direct_declarator: direct_declarator '(' identifier_list ')'
#line 450 "/cygdrive/c/Users/chiap_000/source/repos/fromasmtodisasm/TestEngine/Code/Engine/Renderer/Shaders/Parser.yy"
          { yylhs.value.as < DirectDeclarator > () = yystack_[3].value.as < DirectDeclarator > (); }
#line 1081 "/cygdrive/c/Users/chiap_000/source/repos/fromasmtodisasm/TestEngine/Code/Engine/Renderer/Shaders/Parser.cpp"
    break;

  case 94: // direct_declarator: direct_declarator '(' ')'
#line 451 "/cygdrive/c/Users/chiap_000/source/repos/fromasmtodisasm/TestEngine/Code/Engine/Renderer/Shaders/Parser.yy"
                                   {
        yylhs.value.as < DirectDeclarator > () = yystack_[2].value.as < DirectDeclarator > ();
    }
#line 1089 "/cygdrive/c/Users/chiap_000/source/repos/fromasmtodisasm/TestEngine/Code/Engine/Renderer/Shaders/Parser.cpp"
    break;

  case 105: // var_decl: object_type direct_declarator semantic
#line 472 "/cygdrive/c/Users/chiap_000/source/repos/fromasmtodisasm/TestEngine/Code/Engine/Renderer/Shaders/Parser.yy"
{
    //CryLog("TryParseVarDecl");
}
#line 1097 "/cygdrive/c/Users/chiap_000/source/repos/fromasmtodisasm/TestEngine/Code/Engine/Renderer/Shaders/Parser.cpp"
    break;

  case 109: // $@3: %empty
#line 490 "/cygdrive/c/Users/chiap_000/source/repos/fromasmtodisasm/TestEngine/Code/Engine/Renderer/Shaders/Parser.yy"
     { 
    CryLog("Creating PASS");
    }
#line 1105 "/cygdrive/c/Users/chiap_000/source/repos/fromasmtodisasm/TestEngine/Code/Engine/Renderer/Shaders/Parser.cpp"
    break;

  case 110: // pass: PASS $@3 annotations passstates
#line 494 "/cygdrive/c/Users/chiap_000/source/repos/fromasmtodisasm/TestEngine/Code/Engine/Renderer/Shaders/Parser.yy"
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
#line 1121 "/cygdrive/c/Users/chiap_000/source/repos/fromasmtodisasm/TestEngine/Code/Engine/Renderer/Shaders/Parser.cpp"
    break;

  case 111: // $@4: %empty
#line 505 "/cygdrive/c/Users/chiap_000/source/repos/fromasmtodisasm/TestEngine/Code/Engine/Renderer/Shaders/Parser.yy"
                  {
    SPass pass;
    pass.Name = yystack_[0].value.as < std::string > ().c_str();
    driver.currentEffect->m_Techniques.back().Passes.push_back(pass);
    //driver.currentEffect->m_shaders.push_back(IEffect::ShaderInfo{$1, $3});
    //driver.currentEffect->m_shaders.push_back(IEffect::ShaderInfo{$1, $3});

    CryLog("Creating PASS %s\n", pass.Name.data());
    //curPass = curTechnique->addPass($2->c_str())->getExInterface();
    //curAnnotations = curPass->annotations()->getExInterface();
    }
#line 1137 "/cygdrive/c/Users/chiap_000/source/repos/fromasmtodisasm/TestEngine/Code/Engine/Renderer/Shaders/Parser.cpp"
    break;

  case 112: // pass: PASS IDENTIFIER $@4 annotations passstates
#line 516 "/cygdrive/c/Users/chiap_000/source/repos/fromasmtodisasm/TestEngine/Code/Engine/Renderer/Shaders/Parser.yy"
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
#line 1152 "/cygdrive/c/Users/chiap_000/source/repos/fromasmtodisasm/TestEngine/Code/Engine/Renderer/Shaders/Parser.cpp"
    break;

  case 115: // passes: passes error
#line 534 "/cygdrive/c/Users/chiap_000/source/repos/fromasmtodisasm/TestEngine/Code/Engine/Renderer/Shaders/Parser.yy"
               { error(yystack_[1].location, "Error in Pass list\n");}
#line 1158 "/cygdrive/c/Users/chiap_000/source/repos/fromasmtodisasm/TestEngine/Code/Engine/Renderer/Shaders/Parser.cpp"
    break;

  case 116: // $@5: %empty
#line 540 "/cygdrive/c/Users/chiap_000/source/repos/fromasmtodisasm/TestEngine/Code/Engine/Renderer/Shaders/Parser.yy"
          {
    CryLog("Creation of Technique for NO name\n");
    //curTechnique = curContainer->createTechnique()->getExInterface();
    //curAnnotations = curTechnique->annotations()->getExInterface();
}
#line 1168 "/cygdrive/c/Users/chiap_000/source/repos/fromasmtodisasm/TestEngine/Code/Engine/Renderer/Shaders/Parser.cpp"
    break;

  case 118: // $@6: %empty
#line 545 "/cygdrive/c/Users/chiap_000/source/repos/fromasmtodisasm/TestEngine/Code/Engine/Renderer/Shaders/Parser.yy"
                       {
	auto techs = driver.currentEffect->m_Techniques;
    CTechnique tech(techs.size(), yystack_[0].value.as < std::string > ().c_str());
    driver.currentEffect->m_Techniques.push_back(tech);
    CryLog("creation of Technique %s...\n", tech.Name.data());
    //curTechnique = curContainer->createTechnique($2->c_str())->getExInterface();
    //curAnnotations = curTechnique->annotations()->getExInterface();
    //delete $2;
}
#line 1182 "/cygdrive/c/Users/chiap_000/source/repos/fromasmtodisasm/TestEngine/Code/Engine/Renderer/Shaders/Parser.cpp"
    break;

  case 119: // tech: TECHNIQUE IDENTIFIER $@6 annotations '{' passes '}'
#line 553 "/cygdrive/c/Users/chiap_000/source/repos/fromasmtodisasm/TestEngine/Code/Engine/Renderer/Shaders/Parser.yy"
                             { 
    lex_pop_state();
    scanner.canNowAddFragment = true;
    //curAnnotations = NULL;
}
#line 1192 "/cygdrive/c/Users/chiap_000/source/repos/fromasmtodisasm/TestEngine/Code/Engine/Renderer/Shaders/Parser.cpp"
    break;

  case 132: // annotation_value: IDENTIFIER
#line 571 "/cygdrive/c/Users/chiap_000/source/repos/fromasmtodisasm/TestEngine/Code/Engine/Renderer/Shaders/Parser.yy"
                                                                   {CryLog("annotation IDENTIFIER");}
#line 1198 "/cygdrive/c/Users/chiap_000/source/repos/fromasmtodisasm/TestEngine/Code/Engine/Renderer/Shaders/Parser.cpp"
    break;

  case 137: // annotation: annotation_base ';'
#line 575 "/cygdrive/c/Users/chiap_000/source/repos/fromasmtodisasm/TestEngine/Code/Engine/Renderer/Shaders/Parser.yy"
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
#line 1213 "/cygdrive/c/Users/chiap_000/source/repos/fromasmtodisasm/TestEngine/Code/Engine/Renderer/Shaders/Parser.cpp"
    break;

  case 138: // annotations: %empty
#line 588 "/cygdrive/c/Users/chiap_000/source/repos/fromasmtodisasm/TestEngine/Code/Engine/Renderer/Shaders/Parser.yy"
        {annotations = std::vector<string>();}
#line 1219 "/cygdrive/c/Users/chiap_000/source/repos/fromasmtodisasm/TestEngine/Code/Engine/Renderer/Shaders/Parser.cpp"
    break;


#line 1223 "/cygdrive/c/Users/chiap_000/source/repos/fromasmtodisasm/TestEngine/Code/Engine/Renderer/Shaders/Parser.cpp"

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


  const signed char parser::yypact_ninf_ = -78;

  const short parser::yytable_ninf_ = -139;

  const short
  parser::yypact_[] =
  {
     -78,   177,   -78,   -78,   -78,     8,    10,   -61,   -78,   -78,
     -78,   -78,   -78,   -78,   -78,   -78,   -78,   -78,   -78,   -78,
     -78,   -78,   -78,   -78,   -78,    83,    92,   -78,   101,   -78,
     -78,     7,    17,   -78,   -78,   -78,   -78,   -78,   -78,   -78,
     -78,   -78,   -78,    33,    38,   254,   156,    38,    38,   -78,
      77,    78,    14,   -18,   165,    90,    75,   158,    99,   109,
     105,    38,    91,    91,   236,   184,   -78,   -78,   108,    21,
     112,    -2,    29,   -78,   -78,    49,   134,   192,   -78,     2,
     135,   236,   -78,   -78,   217,   -78,   -78,    14,   140,     5,
     141,   -78,   -78,    43,   -78,   -78,   -78,   254,   138,    -1,
     -78,   -78,   -78,    13,   -78,   -78,    98,   132,    37,   212,
     -78,   -78,   -78,   -78,   -78,   223,    85,   137,   147,   -78,
     158,   -78,    75,   -78,   -78,   -78,    80,    22,   -78,   -78,
     -78,   149,   -78,   -78,   -78,   254,   146,   -78,   229,   -78,
      87,   148,    13,   231,   -78,   -78,   160,   -78,   -78,   -78,
     -78,   104,   -78,     6,    75,   152,   -78,   -78,   151,   -78,
     -78,   155,   -78,   -78,   -78,   -78,   -13,   -78,   -78,   -78,
     157,   159,   161,   162,   163,   -78,   -78,   -78,   -78,   -78,
     -78,   -78,   -78,   230,   -78,   -78,   152,   100,   -78,   -78,
      43,   -78,    63,    63,    63,    63,    63,   -78,   -78,   -78,
     -78,   -78,   150,   -78,    19,   -78,   164,   166,   168,   169,
     167,    18,   -78,   -78,   -78,    63,    63,    63,   -78,   172,
     176,    21,   191,   213,   214,   -78,   -78,    63,    63,   209,
     215,   -78,    63,   216,   -78
  };

  const unsigned char
  parser::yydefact_[] =
  {
       2,     0,     1,    11,    20,   116,     0,     0,    58,    59,
      60,    61,    70,    71,    72,    73,    74,    66,    67,    68,
      69,    62,    63,    64,    65,     0,     0,    24,    34,     3,
       7,     0,     8,     9,    41,    10,     6,    42,    43,    21,
       5,     4,   118,     0,    28,     0,     0,    28,    28,    33,
       0,    88,     0,    96,     0,     0,   138,     0,     0,     0,
       0,    28,    44,    44,     0,    37,    22,    88,     0,    83,
       0,     0,   105,    95,    18,     0,     0,   109,   113,     0,
       0,     0,    31,    30,     0,    46,    47,     0,     0,     0,
       0,    36,    32,    14,    89,    86,    94,    85,     0,     0,
      75,    77,    76,     0,    91,    79,    84,     0,     0,     0,
     125,   124,   123,   122,   134,     0,     0,     0,   136,   127,
       0,   111,   138,   115,   117,   114,     0,     0,    45,    40,
      35,     0,    39,    12,    13,     0,     0,    92,     0,    93,
       0,     0,     0,     0,    90,   108,     0,    19,   133,   139,
     126,     0,   137,     0,   138,     0,    26,    25,     0,    29,
      38,    15,    23,    87,    78,    81,     0,    82,    98,    97,
       0,     0,     0,     0,     0,   107,   104,   106,   132,   128,
     129,   120,   131,   130,   135,   119,     0,    56,   110,    27,
      14,    80,     0,     0,     0,     0,     0,   121,   112,    48,
      50,    49,     0,    54,     0,    16,     0,     0,     0,     0,
       0,     0,    57,    55,    99,     0,     0,     0,   100,    88,
       0,    51,     0,     0,     0,    53,   101,     0,     0,     0,
       0,   102,     0,     0,   103
  };

  const short
  parser::yypgoto_[] =
  {
     -78,   -78,   -78,    53,   -78,   -78,    23,   -78,   -78,   -78,
     -78,   -38,   -78,   -78,   244,   -78,   -78,   -78,   -54,   -78,
     237,   -78,   -78,   -78,   -78,    95,   -78,   115,   -78,   -78,
     -77,   -78,   -78,   -78,   -78,   -52,   250,   -62,   -78,   -78,
       0,   -75,   -78,   -78,   185,   -78,   -78,   -78,   -78,   -78,
     -78,   -78,   -78,   -78,   188,   -59
  };

  const unsigned char
  parser::yydefgoto_[] =
  {
       0,     1,   135,   136,    30,   109,    87,    32,    93,    33,
     158,    59,    34,    46,    88,    50,    65,    92,    89,    36,
      85,    37,    38,   202,   220,   203,   204,   188,    39,   105,
     106,    68,   107,    98,    99,    53,    55,   175,   176,   177,
      90,    78,   122,   154,    79,    41,    43,    56,   183,   115,
     116,   184,   117,   118,   119,    76
  };

  const short
  parser::yytable_[] =
  {
      69,    40,   100,   123,   125,   101,   102,   123,     4,    62,
      63,    51,    42,   108,    44,    77,    97,   100,    67,    77,
     101,   102,   219,    83,    31,     4,   140,   127,    45,     8,
       9,    10,    11,    12,    13,    14,    15,    16,    17,    18,
      19,    20,    21,    22,    23,    24,     8,     9,    10,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    20,    21,
      22,    23,    24,   155,   110,   166,   111,    70,    60,    54,
     168,   169,   141,   112,    71,   199,   200,   201,   125,   142,
     191,   143,   138,   103,   156,   139,   113,    47,   157,   131,
     124,   104,    52,   130,   185,   186,    48,   131,   103,    52,
     110,   -96,   111,    52,    54,    49,    70,   212,   178,   112,
     159,   179,   180,    71,   181,     4,    95,    57,    75,   145,
    -138,   182,   113,   133,   134,    58,   114,   131,   146,   -17,
     206,   207,   208,   209,   210,   161,     8,     9,    10,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    20,    21,
      22,    23,    24,   222,   223,   224,   199,   200,   201,   221,
      61,    64,   114,    66,    75,   229,   230,   168,   169,    73,
     233,    77,   141,   164,    74,   149,    80,     2,     3,   142,
       4,   143,    84,   141,   170,   171,   172,   173,    91,     5,
     142,    28,   143,    81,    94,    82,   121,   174,    96,     6,
       7,     8,     9,    10,    11,    12,    13,    14,    15,    16,
      17,    18,    19,    20,    21,    22,    23,    24,   120,    25,
     126,   128,   129,   132,   137,   144,   147,   148,   151,   152,
      26,   160,   162,   163,   165,   167,   187,   189,   190,     4,
     197,   211,   192,   205,   193,    35,   194,   195,   196,   215,
     214,   216,   217,   218,   -52,    27,    28,     4,   225,    29,
       8,     9,    10,    11,    12,    13,    14,    15,    16,    17,
      18,    19,    20,    21,    22,    23,    24,   226,     8,     9,
      10,    11,    12,    13,    14,    15,    16,    17,    18,    19,
      20,    21,    22,    23,    24,   231,   227,   228,   232,   213,
      86,   198,   234,    72,   150,   153,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    28
  };

  const short
  parser::yycheck_[] =
  {
      52,     1,     4,     1,    79,     7,     8,     1,     3,    47,
      48,     4,     4,    72,     4,    13,    70,     4,     4,    13,
       7,     8,     4,    61,     1,     3,   103,    81,    89,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    24,    25,    26,    27,
      28,    29,    30,    31,    32,    33,    34,    35,    36,    37,
      38,    39,    40,   122,    15,   142,    17,    85,    45,    87,
       7,     8,    85,    24,    92,    56,    57,    58,   153,    92,
      93,    94,    83,    85,     4,    86,    37,     4,     8,    89,
      88,    93,    85,    88,    88,   154,     4,    97,    85,    85,
      15,    84,    17,    85,    87,     4,    85,    88,     4,    24,
      88,     7,     8,    92,    10,     3,     4,    84,    89,    82,
      91,    17,    37,    80,    81,    87,    77,   127,    91,    86,
     192,   193,   194,   195,   196,   135,    24,    25,    26,    27,
      28,    29,    30,    31,    32,    33,    34,    35,    36,    37,
      38,    39,    40,   215,   216,   217,    56,    57,    58,   211,
       4,    84,    77,    85,    89,   227,   228,     7,     8,     4,
     232,    13,    85,    86,    84,    90,    77,     0,     1,    92,
       3,    94,    91,    85,    24,    25,    26,    27,     4,    12,
      92,    79,    94,    84,    86,    90,     4,    37,    86,    22,
      23,    24,    25,    26,    27,    28,    29,    30,    31,    32,
      33,    34,    35,    36,    37,    38,    39,    40,    84,    42,
      85,     4,    82,    82,    86,    93,    14,     4,    91,    82,
      53,    82,    86,     4,    86,     4,    84,    86,    83,     3,
      10,    91,    85,   190,    85,     1,    85,    85,    85,    83,
      86,    83,    83,    86,    82,    78,    79,     3,    82,    82,
      24,    25,    26,    27,    28,    29,    30,    31,    32,    33,
      34,    35,    36,    37,    38,    39,    40,    86,    24,    25,
      26,    27,    28,    29,    30,    31,    32,    33,    34,    35,
      36,    37,    38,    39,    40,    86,    83,    83,    83,   204,
      63,   186,    86,    53,   116,   120,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    79
  };

  const unsigned char
  parser::yystos_[] =
  {
       0,    96,     0,     1,     3,    12,    22,    23,    24,    25,
      26,    27,    28,    29,    30,    31,    32,    33,    34,    35,
      36,    37,    38,    39,    40,    42,    53,    78,    79,    82,
      99,   101,   102,   104,   107,   109,   114,   116,   117,   123,
     135,   140,     4,   141,     4,    89,   108,     4,     4,     4,
     110,     4,    85,   130,    87,   131,   142,    84,    87,   106,
     101,     4,   106,   106,    84,   111,    85,     4,   126,   130,
      85,    92,   131,     4,    84,    89,   150,    13,   136,   139,
      77,    84,    90,   106,    91,   115,   115,   101,   109,   113,
     135,     4,   112,   103,    86,     4,    86,   113,   128,   129,
       4,     7,     8,    85,    93,   124,   125,   127,   150,   100,
      15,    17,    24,    37,    77,   144,   145,   147,   148,   149,
      84,     4,   137,     1,    88,   136,    85,   113,     4,    82,
      88,   135,    82,    80,    81,    97,    98,    86,    83,    86,
     125,    85,    92,    94,    93,    82,    91,    14,     4,    90,
     149,    91,    82,   139,   138,   150,     4,     8,   105,    88,
      82,   135,    86,     4,    86,    86,   125,     4,     7,     8,
      24,    25,    26,    27,    37,   132,   133,   134,     4,     7,
       8,    10,    17,   143,   146,    88,   150,    84,   122,    86,
      83,    93,    85,    85,    85,    85,    85,    10,   122,    56,
      57,    58,   118,   120,   121,    98,   132,   132,   132,   132,
     132,    91,    88,   120,    86,    83,    83,    83,    86,     4,
     119,   130,   132,   132,   132,    82,    86,    83,    83,   132,
     132,    86,    83,   132,    86
  };

  const unsigned char
  parser::yyr1_[] =
  {
       0,    95,    96,    96,    96,    96,    96,    96,    96,    96,
      96,    96,    97,    97,    97,    98,    98,    98,   100,    99,
     101,   101,   103,   102,   104,   105,   105,   106,   106,   107,
     107,   108,   109,   110,   110,   111,   112,   112,   113,   113,
     113,   114,   114,   114,   115,   115,   116,   117,   118,   118,
     118,   119,   119,   120,   121,   121,   121,   122,   123,   123,
     123,   123,   123,   123,   123,   123,   123,   123,   123,   123,
     123,   123,   123,   123,   123,   124,   124,   124,   124,   125,
     125,   125,   125,   126,   127,   128,   129,   129,   130,   130,
     130,   130,   130,   130,   130,   131,   131,   132,   132,   133,
     133,   133,   133,   133,   134,   135,   135,   135,   135,   137,
     136,   138,   136,   139,   139,   139,   141,   140,   142,   140,
     143,   143,   144,   144,   144,   144,   145,   145,   146,   146,
     146,   146,   146,   147,   147,   148,   149,   149,   150,   150
  };

  const signed char
  parser::yyr2_[] =
  {
       0,     2,     0,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     1,     1,     0,     2,     4,     0,     0,     5,
       1,     1,     0,     6,     1,     1,     1,     5,     0,     6,
       4,     3,     4,     1,     0,     3,     1,     0,     3,     2,
       2,     1,     1,     1,     0,     2,     4,     4,     1,     1,
       1,     1,     1,     4,     1,     2,     0,     3,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     3,     1,
       4,     3,     3,     1,     1,     1,     1,     3,     1,     3,
       4,     3,     4,     4,     3,     2,     0,     1,     1,     4,
       4,     6,     8,    10,     1,     3,     6,     6,     5,     0,
       4,     0,     5,     1,     2,     2,     0,     5,     0,     7,
       1,     2,     1,     1,     1,     1,     2,     1,     1,     1,
       1,     1,     1,     2,     1,     3,     1,     2,     0,     3
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
  "annotation_base", "annotation", "annotations", YY_NULLPTR
  };
#endif


#if YYDEBUG
  const short
  parser::yyrline_[] =
  {
       0,   280,   280,   281,   282,   283,   284,   285,   286,   287,
     288,   289,   292,   292,   292,   294,   295,   296,   298,   298,
     306,   306,   308,   308,   313,   315,   315,   317,   317,   319,
     324,   330,   333,   338,   338,   339,   340,   341,   343,   343,
     343,   345,   345,   345,   347,   347,   348,   353,   359,   360,
     361,   364,   367,   371,   377,   378,   379,   386,   390,   391,
     392,   393,   394,   395,   396,   397,   398,   399,   400,   401,
     402,   403,   404,   405,   406,   413,   415,   416,   418,   422,
     423,   424,   426,   431,   433,   435,   438,   439,   443,   446,
     447,   448,   449,   450,   451,   456,   456,   459,   459,   462,
     463,   464,   465,   466,   467,   471,   475,   476,   477,   490,
     490,   505,   505,   532,   533,   534,   540,   540,   545,   545,
     564,   564,   565,   565,   565,   565,   568,   569,   571,   571,
     571,   571,   571,   573,   573,   574,   575,   575,   588,   589
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
#line 1894 "/cygdrive/c/Users/chiap_000/source/repos/fromasmtodisasm/TestEngine/Code/Engine/Renderer/Shaders/Parser.cpp"

#line 592 "/cygdrive/c/Users/chiap_000/source/repos/fromasmtodisasm/TestEngine/Code/Engine/Renderer/Shaders/Parser.yy"


#include <sstream>
void
yy::parser::error(const location_type& l, const std::string& m)
{
  std::stringstream ss;
  ss << l << ": " << m;
  CryError(ss.str().c_str());    
}
#pragma warning(pop)
