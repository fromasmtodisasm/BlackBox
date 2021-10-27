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
#line 114 "/cygdrive/c/Users/chiap/source/repos/fromasmtodisasm/TestEngine/Code/Engine/Renderer/Shaders/Parser.yy"

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
#line 104 "/cygdrive/c/Users/chiap/source/repos/fromasmtodisasm/TestEngine/Code/Engine/Renderer/Shaders/Parser.yy"
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
#line 270 "/cygdrive/c/Users/chiap/source/repos/fromasmtodisasm/TestEngine/Code/Engine/Renderer/Shaders/Parser.yy"
              { CryLog("Empty effect"); }
#line 763 "/cygdrive/c/Users/chiap/source/repos/fromasmtodisasm/TestEngine/Code/Engine/Renderer/Shaders/Parser.cpp"
    break;

  case 19: // $@1: %empty
#line 289 "/cygdrive/c/Users/chiap/source/repos/fromasmtodisasm/TestEngine/Code/Engine/Renderer/Shaders/Parser.yy"
                                                       { 
    CryLog("Open function scope"); scanner.goto_codebody(); 
    }
#line 771 "/cygdrive/c/Users/chiap/source/repos/fromasmtodisasm/TestEngine/Code/Engine/Renderer/Shaders/Parser.cpp"
    break;

  case 20: // function_definition: function_declaration semantic '{' $@1 CODEBODY
#line 291 "/cygdrive/c/Users/chiap/source/repos/fromasmtodisasm/TestEngine/Code/Engine/Renderer/Shaders/Parser.yy"
               {
    CryLog("Close function scope"); 
    CryLog("Funcbody: \n%s", yystack_[0].value.as < std::string > ().data());
}
#line 780 "/cygdrive/c/Users/chiap/source/repos/fromasmtodisasm/TestEngine/Code/Engine/Renderer/Shaders/Parser.cpp"
    break;

  case 23: // $@2: %empty
#line 298 "/cygdrive/c/Users/chiap/source/repos/fromasmtodisasm/TestEngine/Code/Engine/Renderer/Shaders/Parser.yy"
                                                       {CryLog("TryParseFunc");}
#line 786 "/cygdrive/c/Users/chiap/source/repos/fromasmtodisasm/TestEngine/Code/Engine/Renderer/Shaders/Parser.cpp"
    break;

  case 24: // function_declaration: object_type IDENTIFIER '(' $@2 arguments ')'
#line 298 "/cygdrive/c/Users/chiap/source/repos/fromasmtodisasm/TestEngine/Code/Engine/Renderer/Shaders/Parser.yy"
                                                                                             {
    CryLog("Parsed function declaration for: [%s]", yystack_[4].value.as < std::string > ().data());
}
#line 794 "/cygdrive/c/Users/chiap/source/repos/fromasmtodisasm/TestEngine/Code/Engine/Renderer/Shaders/Parser.cpp"
    break;

  case 25: // fatal_error: FATALERROR
#line 302 "/cygdrive/c/Users/chiap/source/repos/fromasmtodisasm/TestEngine/Code/Engine/Renderer/Shaders/Parser.yy"
                        { CryFatalError("Stopping paring!!!"); }
#line 800 "/cygdrive/c/Users/chiap/source/repos/fromasmtodisasm/TestEngine/Code/Engine/Renderer/Shaders/Parser.cpp"
    break;

  case 30: // cbuffer: CSTBUFFER IDENTIFIER register_declaration '{' var_decls '}'
#line 309 "/cygdrive/c/Users/chiap/source/repos/fromasmtodisasm/TestEngine/Code/Engine/Renderer/Shaders/Parser.yy"
{
    CryLog("New CBuffer %s", yystack_[4].value.as < std::string > ().data());
    lex_pop_state();
}
#line 809 "/cygdrive/c/Users/chiap/source/repos/fromasmtodisasm/TestEngine/Code/Engine/Renderer/Shaders/Parser.cpp"
    break;

  case 31: // struct: STRUCT struct_header struct_body struct_footer
#line 315 "/cygdrive/c/Users/chiap/source/repos/fromasmtodisasm/TestEngine/Code/Engine/Renderer/Shaders/Parser.yy"
{
    CryLog("New Struct");
}
#line 817 "/cygdrive/c/Users/chiap/source/repos/fromasmtodisasm/TestEngine/Code/Engine/Renderer/Shaders/Parser.cpp"
    break;

  case 32: // struct_header: IDENTIFIER
#line 319 "/cygdrive/c/Users/chiap/source/repos/fromasmtodisasm/TestEngine/Code/Engine/Renderer/Shaders/Parser.yy"
                         {CryLog("StructName: %s", yystack_[0].value.as < std::string > ().data()); scanner.register_type(yystack_[0].value.as < std::string > ().data());}
#line 823 "/cygdrive/c/Users/chiap/source/repos/fromasmtodisasm/TestEngine/Code/Engine/Renderer/Shaders/Parser.cpp"
    break;

  case 33: // struct_header: %empty
#line 319 "/cygdrive/c/Users/chiap/source/repos/fromasmtodisasm/TestEngine/Code/Engine/Renderer/Shaders/Parser.yy"
                                                                                                           {yylhs.value.as < std::string > ()="";}
#line 829 "/cygdrive/c/Users/chiap/source/repos/fromasmtodisasm/TestEngine/Code/Engine/Renderer/Shaders/Parser.cpp"
    break;

  case 35: // struct_footer: IDENTIFIER
#line 321 "/cygdrive/c/Users/chiap/source/repos/fromasmtodisasm/TestEngine/Code/Engine/Renderer/Shaders/Parser.yy"
                          {CryLog("Declared and defined struct with name: %s", yystack_[0].value.as < std::string > ().data());}
#line 835 "/cygdrive/c/Users/chiap/source/repos/fromasmtodisasm/TestEngine/Code/Engine/Renderer/Shaders/Parser.cpp"
    break;

  case 36: // struct_footer: %empty
#line 322 "/cygdrive/c/Users/chiap/source/repos/fromasmtodisasm/TestEngine/Code/Engine/Renderer/Shaders/Parser.yy"
         {yylhs.value.as < std::string > ()="";}
#line 841 "/cygdrive/c/Users/chiap/source/repos/fromasmtodisasm/TestEngine/Code/Engine/Renderer/Shaders/Parser.cpp"
    break;

  case 43: // texture2d: TEXTURE2D_TYPE IDENTIFIER register_declaration
#line 329 "/cygdrive/c/Users/chiap/source/repos/fromasmtodisasm/TestEngine/Code/Engine/Renderer/Shaders/Parser.yy"
{
    CryLog("texture2d");
}
#line 849 "/cygdrive/c/Users/chiap/source/repos/fromasmtodisasm/TestEngine/Code/Engine/Renderer/Shaders/Parser.cpp"
    break;

  case 44: // sampler_state: SAMPLERSTATE IDENTIFIER register_declaration
#line 334 "/cygdrive/c/Users/chiap/source/repos/fromasmtodisasm/TestEngine/Code/Engine/Renderer/Shaders/Parser.yy"
{
    CryLog("sampler");
}
#line 857 "/cygdrive/c/Users/chiap/source/repos/fromasmtodisasm/TestEngine/Code/Engine/Renderer/Shaders/Parser.cpp"
    break;

  case 45: // shader_type: VERTEXPROGRAM
#line 339 "/cygdrive/c/Users/chiap/source/repos/fromasmtodisasm/TestEngine/Code/Engine/Renderer/Shaders/Parser.yy"
                {yylhs.value.as < IShader::Type > () = yystack_[0].value.as < IShader::Type > ();}
#line 863 "/cygdrive/c/Users/chiap/source/repos/fromasmtodisasm/TestEngine/Code/Engine/Renderer/Shaders/Parser.cpp"
    break;

  case 46: // shader_type: GEOMETRYPROGRAM
#line 340 "/cygdrive/c/Users/chiap/source/repos/fromasmtodisasm/TestEngine/Code/Engine/Renderer/Shaders/Parser.yy"
                  {yylhs.value.as < IShader::Type > () = yystack_[0].value.as < IShader::Type > ();}
#line 869 "/cygdrive/c/Users/chiap/source/repos/fromasmtodisasm/TestEngine/Code/Engine/Renderer/Shaders/Parser.cpp"
    break;

  case 47: // shader_type: FRAGMENTPROGRAM
#line 341 "/cygdrive/c/Users/chiap/source/repos/fromasmtodisasm/TestEngine/Code/Engine/Renderer/Shaders/Parser.yy"
                  {yylhs.value.as < IShader::Type > () = yystack_[0].value.as < IShader::Type > ();}
#line 875 "/cygdrive/c/Users/chiap/source/repos/fromasmtodisasm/TestEngine/Code/Engine/Renderer/Shaders/Parser.cpp"
    break;

  case 48: // shader_assignment: shader_type '=' IDENTIFIER
#line 344 "/cygdrive/c/Users/chiap/source/repos/fromasmtodisasm/TestEngine/Code/Engine/Renderer/Shaders/Parser.yy"
                                              {
    //$$ = std::make_pair($1, $3);
	driver.currentEffect->shader_assignment(yystack_[2].value.as < IShader::Type > (),yystack_[0].value.as < std::string > ());
}
#line 884 "/cygdrive/c/Users/chiap/source/repos/fromasmtodisasm/TestEngine/Code/Engine/Renderer/Shaders/Parser.cpp"
    break;

  case 52: // base_type: FLOAT_TYPE
#line 363 "/cygdrive/c/Users/chiap/source/repos/fromasmtodisasm/TestEngine/Code/Engine/Renderer/Shaders/Parser.yy"
              { yylhs.value.as < nvFX::IUniform::Type > () = nvFX::IUniform::TFloat; }
#line 890 "/cygdrive/c/Users/chiap/source/repos/fromasmtodisasm/TestEngine/Code/Engine/Renderer/Shaders/Parser.cpp"
    break;

  case 53: // base_type: FLOAT2_TYPE
#line 364 "/cygdrive/c/Users/chiap/source/repos/fromasmtodisasm/TestEngine/Code/Engine/Renderer/Shaders/Parser.yy"
               { yylhs.value.as < nvFX::IUniform::Type > () = nvFX::IUniform::TVec2; }
#line 896 "/cygdrive/c/Users/chiap/source/repos/fromasmtodisasm/TestEngine/Code/Engine/Renderer/Shaders/Parser.cpp"
    break;

  case 54: // base_type: FLOAT3_TYPE
#line 365 "/cygdrive/c/Users/chiap/source/repos/fromasmtodisasm/TestEngine/Code/Engine/Renderer/Shaders/Parser.yy"
               { yylhs.value.as < nvFX::IUniform::Type > () = nvFX::IUniform::TVec3; }
#line 902 "/cygdrive/c/Users/chiap/source/repos/fromasmtodisasm/TestEngine/Code/Engine/Renderer/Shaders/Parser.cpp"
    break;

  case 55: // base_type: FLOAT4_TYPE
#line 366 "/cygdrive/c/Users/chiap/source/repos/fromasmtodisasm/TestEngine/Code/Engine/Renderer/Shaders/Parser.yy"
               { yylhs.value.as < nvFX::IUniform::Type > () = nvFX::IUniform::TVec4; }
#line 908 "/cygdrive/c/Users/chiap/source/repos/fromasmtodisasm/TestEngine/Code/Engine/Renderer/Shaders/Parser.cpp"
    break;

  case 56: // base_type: INT_TYPE
#line 367 "/cygdrive/c/Users/chiap/source/repos/fromasmtodisasm/TestEngine/Code/Engine/Renderer/Shaders/Parser.yy"
               { yylhs.value.as < nvFX::IUniform::Type > () = nvFX::IUniform::TInt; }
#line 914 "/cygdrive/c/Users/chiap/source/repos/fromasmtodisasm/TestEngine/Code/Engine/Renderer/Shaders/Parser.cpp"
    break;

  case 57: // base_type: INT2_TYPE
#line 368 "/cygdrive/c/Users/chiap/source/repos/fromasmtodisasm/TestEngine/Code/Engine/Renderer/Shaders/Parser.yy"
             { yylhs.value.as < nvFX::IUniform::Type > () = nvFX::IUniform::TInt2; }
#line 920 "/cygdrive/c/Users/chiap/source/repos/fromasmtodisasm/TestEngine/Code/Engine/Renderer/Shaders/Parser.cpp"
    break;

  case 58: // base_type: INT3_TYPE
#line 369 "/cygdrive/c/Users/chiap/source/repos/fromasmtodisasm/TestEngine/Code/Engine/Renderer/Shaders/Parser.yy"
             { yylhs.value.as < nvFX::IUniform::Type > () = nvFX::IUniform::TInt3; }
#line 926 "/cygdrive/c/Users/chiap/source/repos/fromasmtodisasm/TestEngine/Code/Engine/Renderer/Shaders/Parser.cpp"
    break;

  case 59: // base_type: INT4_TYPE
#line 370 "/cygdrive/c/Users/chiap/source/repos/fromasmtodisasm/TestEngine/Code/Engine/Renderer/Shaders/Parser.yy"
             { yylhs.value.as < nvFX::IUniform::Type > () = nvFX::IUniform::TInt4; }
#line 932 "/cygdrive/c/Users/chiap/source/repos/fromasmtodisasm/TestEngine/Code/Engine/Renderer/Shaders/Parser.cpp"
    break;

  case 60: // base_type: BOOL_TYPE
#line 371 "/cygdrive/c/Users/chiap/source/repos/fromasmtodisasm/TestEngine/Code/Engine/Renderer/Shaders/Parser.yy"
             { yylhs.value.as < nvFX::IUniform::Type > () = nvFX::IUniform::TBool; }
#line 938 "/cygdrive/c/Users/chiap/source/repos/fromasmtodisasm/TestEngine/Code/Engine/Renderer/Shaders/Parser.cpp"
    break;

  case 61: // base_type: BOOL2_TYPE
#line 372 "/cygdrive/c/Users/chiap/source/repos/fromasmtodisasm/TestEngine/Code/Engine/Renderer/Shaders/Parser.yy"
              { yylhs.value.as < nvFX::IUniform::Type > () = nvFX::IUniform::TBool2; }
#line 944 "/cygdrive/c/Users/chiap/source/repos/fromasmtodisasm/TestEngine/Code/Engine/Renderer/Shaders/Parser.cpp"
    break;

  case 62: // base_type: BOOL3_TYPE
#line 373 "/cygdrive/c/Users/chiap/source/repos/fromasmtodisasm/TestEngine/Code/Engine/Renderer/Shaders/Parser.yy"
              { yylhs.value.as < nvFX::IUniform::Type > () = nvFX::IUniform::TBool3; }
#line 950 "/cygdrive/c/Users/chiap/source/repos/fromasmtodisasm/TestEngine/Code/Engine/Renderer/Shaders/Parser.cpp"
    break;

  case 63: // base_type: BOOL4_TYPE
#line 374 "/cygdrive/c/Users/chiap/source/repos/fromasmtodisasm/TestEngine/Code/Engine/Renderer/Shaders/Parser.yy"
              { yylhs.value.as < nvFX::IUniform::Type > () = nvFX::IUniform::TBool4; }
#line 956 "/cygdrive/c/Users/chiap/source/repos/fromasmtodisasm/TestEngine/Code/Engine/Renderer/Shaders/Parser.cpp"
    break;

  case 64: // base_type: MAT2_TYPE
#line 375 "/cygdrive/c/Users/chiap/source/repos/fromasmtodisasm/TestEngine/Code/Engine/Renderer/Shaders/Parser.yy"
             { yylhs.value.as < nvFX::IUniform::Type > () = nvFX::IUniform::TMat2; }
#line 962 "/cygdrive/c/Users/chiap/source/repos/fromasmtodisasm/TestEngine/Code/Engine/Renderer/Shaders/Parser.cpp"
    break;

  case 65: // base_type: MAT3_TYPE
#line 376 "/cygdrive/c/Users/chiap/source/repos/fromasmtodisasm/TestEngine/Code/Engine/Renderer/Shaders/Parser.yy"
             { yylhs.value.as < nvFX::IUniform::Type > () = nvFX::IUniform::TMat3; }
#line 968 "/cygdrive/c/Users/chiap/source/repos/fromasmtodisasm/TestEngine/Code/Engine/Renderer/Shaders/Parser.cpp"
    break;

  case 66: // base_type: MAT4_TYPE
#line 377 "/cygdrive/c/Users/chiap/source/repos/fromasmtodisasm/TestEngine/Code/Engine/Renderer/Shaders/Parser.yy"
             { yylhs.value.as < nvFX::IUniform::Type > () = nvFX::IUniform::TMat4; }
#line 974 "/cygdrive/c/Users/chiap/source/repos/fromasmtodisasm/TestEngine/Code/Engine/Renderer/Shaders/Parser.cpp"
    break;

  case 69: // var_decl: object_type IDENTIFIER semantic
#line 385 "/cygdrive/c/Users/chiap/source/repos/fromasmtodisasm/TestEngine/Code/Engine/Renderer/Shaders/Parser.yy"
{
    CryLog("TryParseVarDecl");
}
#line 982 "/cygdrive/c/Users/chiap/source/repos/fromasmtodisasm/TestEngine/Code/Engine/Renderer/Shaders/Parser.cpp"
    break;

  case 72: // $@3: %empty
#line 402 "/cygdrive/c/Users/chiap/source/repos/fromasmtodisasm/TestEngine/Code/Engine/Renderer/Shaders/Parser.yy"
     { 
    CryLog("Creating PASS");
    }
#line 990 "/cygdrive/c/Users/chiap/source/repos/fromasmtodisasm/TestEngine/Code/Engine/Renderer/Shaders/Parser.cpp"
    break;

  case 73: // pass: PASS $@3 annotations passstates
#line 406 "/cygdrive/c/Users/chiap/source/repos/fromasmtodisasm/TestEngine/Code/Engine/Renderer/Shaders/Parser.yy"
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
#line 1006 "/cygdrive/c/Users/chiap/source/repos/fromasmtodisasm/TestEngine/Code/Engine/Renderer/Shaders/Parser.cpp"
    break;

  case 74: // $@4: %empty
#line 417 "/cygdrive/c/Users/chiap/source/repos/fromasmtodisasm/TestEngine/Code/Engine/Renderer/Shaders/Parser.yy"
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
#line 1023 "/cygdrive/c/Users/chiap/source/repos/fromasmtodisasm/TestEngine/Code/Engine/Renderer/Shaders/Parser.cpp"
    break;

  case 75: // pass: PASS IDENTIFIER $@4 annotations passstates
#line 429 "/cygdrive/c/Users/chiap/source/repos/fromasmtodisasm/TestEngine/Code/Engine/Renderer/Shaders/Parser.yy"
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
#line 1038 "/cygdrive/c/Users/chiap/source/repos/fromasmtodisasm/TestEngine/Code/Engine/Renderer/Shaders/Parser.cpp"
    break;

  case 78: // passes: passes error
#line 447 "/cygdrive/c/Users/chiap/source/repos/fromasmtodisasm/TestEngine/Code/Engine/Renderer/Shaders/Parser.yy"
               { error(yystack_[1].location, "Error in Pass list\n");}
#line 1044 "/cygdrive/c/Users/chiap/source/repos/fromasmtodisasm/TestEngine/Code/Engine/Renderer/Shaders/Parser.cpp"
    break;

  case 79: // $@5: %empty
#line 453 "/cygdrive/c/Users/chiap/source/repos/fromasmtodisasm/TestEngine/Code/Engine/Renderer/Shaders/Parser.yy"
          {
    CryLog("Creation of Technique for NO name\n");
    //curTechnique = curContainer->createTechnique()->getExInterface();
    //curAnnotations = curTechnique->annotations()->getExInterface();
}
#line 1054 "/cygdrive/c/Users/chiap/source/repos/fromasmtodisasm/TestEngine/Code/Engine/Renderer/Shaders/Parser.cpp"
    break;

  case 81: // $@6: %empty
#line 458 "/cygdrive/c/Users/chiap/source/repos/fromasmtodisasm/TestEngine/Code/Engine/Renderer/Shaders/Parser.yy"
                       {
    CTechnique tech;
    tech.Name =  yystack_[0].value.as < std::string > ().c_str();
    driver.currentEffect->m_Techniques.push_back(tech);
    CryLog("creation of Technique %s...\n", tech.Name.data());
    //curTechnique = curContainer->createTechnique($2->c_str())->getExInterface();
    //curAnnotations = curTechnique->annotations()->getExInterface();
    //delete $2;
}
#line 1068 "/cygdrive/c/Users/chiap/source/repos/fromasmtodisasm/TestEngine/Code/Engine/Renderer/Shaders/Parser.cpp"
    break;

  case 82: // tech: TECHNIQUE IDENTIFIER $@6 annotations '{' passes '}'
#line 466 "/cygdrive/c/Users/chiap/source/repos/fromasmtodisasm/TestEngine/Code/Engine/Renderer/Shaders/Parser.yy"
                             { 
    lex_pop_state();
    //curAnnotations = NULL;
}
#line 1077 "/cygdrive/c/Users/chiap/source/repos/fromasmtodisasm/TestEngine/Code/Engine/Renderer/Shaders/Parser.cpp"
    break;

  case 93: // annotation: scalar_type IDENTIFIER '=' annotation_value ';'
#line 483 "/cygdrive/c/Users/chiap/source/repos/fromasmtodisasm/TestEngine/Code/Engine/Renderer/Shaders/Parser.yy"
                                                            {
    CryLog("annotation: %s = ", yystack_[3].value.as < std::string > ().c_str());
/*
    if(!curAnnotations)
        curAnnotations = IAnnotationEx::getAnnotationSingleton(2); // need a temporary place since nothing was initialized
    if(!curAnnotations->addAnnotation($2->c_str(), $4->c_str()))
        yyerror("err\n");
    delete $4;
*/
    }
#line 1092 "/cygdrive/c/Users/chiap/source/repos/fromasmtodisasm/TestEngine/Code/Engine/Renderer/Shaders/Parser.cpp"
    break;

  case 94: // annotations: %empty
#line 496 "/cygdrive/c/Users/chiap/source/repos/fromasmtodisasm/TestEngine/Code/Engine/Renderer/Shaders/Parser.yy"
        {annotations = std::vector<string>();}
#line 1098 "/cygdrive/c/Users/chiap/source/repos/fromasmtodisasm/TestEngine/Code/Engine/Renderer/Shaders/Parser.cpp"
    break;

  case 96: // hlsl_header: HLSL11SHADER IDENTIFIER
#line 502 "/cygdrive/c/Users/chiap/source/repos/fromasmtodisasm/TestEngine/Code/Engine/Renderer/Shaders/Parser.yy"
                                {
        yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); 
    }
#line 1106 "/cygdrive/c/Users/chiap/source/repos/fromasmtodisasm/TestEngine/Code/Engine/Renderer/Shaders/Parser.cpp"
    break;

  case 97: // hlsl_header: HLSL11SHADER
#line 505 "/cygdrive/c/Users/chiap/source/repos/fromasmtodisasm/TestEngine/Code/Engine/Renderer/Shaders/Parser.yy"
                       {
        yylhs.value.as < std::string > () = "Common";
        is_common = true;
    }
#line 1115 "/cygdrive/c/Users/chiap/source/repos/fromasmtodisasm/TestEngine/Code/Engine/Renderer/Shaders/Parser.cpp"
    break;

  case 98: // hlsl: shader_header '{' CODEBODY
#line 511 "/cygdrive/c/Users/chiap/source/repos/fromasmtodisasm/TestEngine/Code/Engine/Renderer/Shaders/Parser.yy"
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
#line 1134 "/cygdrive/c/Users/chiap/source/repos/fromasmtodisasm/TestEngine/Code/Engine/Renderer/Shaders/Parser.cpp"
    break;

  case 99: // shader_header: hlsl_header annotations
#line 527 "/cygdrive/c/Users/chiap/source/repos/fromasmtodisasm/TestEngine/Code/Engine/Renderer/Shaders/Parser.yy"
                                        { yylhs.value.as < std::string > () = yystack_[1].value.as < std::string > (); }
#line 1140 "/cygdrive/c/Users/chiap/source/repos/fromasmtodisasm/TestEngine/Code/Engine/Renderer/Shaders/Parser.cpp"
    break;


#line 1144 "/cygdrive/c/Users/chiap/source/repos/fromasmtodisasm/TestEngine/Code/Engine/Renderer/Shaders/Parser.cpp"

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


  const signed char parser::yypact_ninf_ = -85;

  const signed char parser::yytable_ninf_ = -95;

  const short
  parser::yypact_[] =
  {
     -85,    88,   -85,   -85,   -85,    53,    72,    77,   -85,   -85,
     -85,   -85,   -85,   -85,   -85,   -85,   -85,   -85,   -85,   -85,
     -85,   -85,   -85,    78,    79,   -85,    89,   -85,   -85,    90,
     -17,   -85,   -85,   -85,   -85,   -85,   -85,   -85,   -85,   -85,
     -84,   -85,   -19,   -85,   -85,    11,    12,    12,    12,   -85,
      16,   -65,    91,    22,    46,   -85,    84,   -84,    92,    30,
      25,   -85,   -85,   123,   103,   -85,   -16,   -85,   -85,   -85,
     -85,   -85,   -85,   104,   -11,   -85,   -85,    47,   125,   -85,
       0,    48,   123,   127,    54,     2,    55,   -85,   -85,   -67,
      49,   118,    50,   -85,   -85,    92,   -85,   -84,   -85,   -85,
     -85,    14,    17,    56,   -85,   -85,    60,   -85,   -85,   -85,
     158,    59,    65,   -85,    58,     1,   -84,    62,   -85,   -85,
      61,   -85,   -85,    57,   -85,   -85,   -85,   -85,   -85,   -85,
     -85,    66,   -85,    62,   -46,   -85,   -85,   -67,   -85,   -85,
     -85,   -85,   -85,    76,   -85,     5,   -85,   137,   -85,   -85,
     -85
  };

  const signed char
  parser::yydefact_[] =
  {
       2,     0,     1,    12,    21,    97,    79,     0,    52,    53,
      54,    55,    64,    65,    66,    60,    61,    62,    63,    56,
      57,    58,    59,     0,     0,    25,    33,     3,     8,     0,
       9,    10,    40,    11,     7,    41,    42,    22,     6,     4,
      94,     5,     0,    96,    81,     0,    29,    29,    29,    32,
       0,    68,     0,     0,     0,    99,     0,    94,     0,     0,
       0,    43,    44,     0,    36,    23,    69,    67,    19,    86,
      85,    84,    83,     0,     0,    88,    98,     0,    72,    76,
       0,     0,     0,     0,     0,     0,     0,    35,    31,    15,
       0,     0,     0,    95,    87,     0,    74,    94,    78,    80,
      77,     0,     0,    68,    39,    34,     0,    38,    13,    14,
       0,     0,     0,    20,     0,     0,    94,     0,    27,    26,
       0,    30,    37,    16,    24,    71,    70,    89,    90,    91,
      92,     0,    82,     0,     0,    73,    28,    15,    93,    75,
      45,    47,    46,     0,    49,     0,    17,     0,    51,    50,
      48
  };

  const short
  parser::yypgoto_[] =
  {
     -85,   -85,   -85,    28,   -85,   -85,   165,   -85,   -85,   -85,
     -85,    31,   -85,   167,   -85,   -85,   -85,    87,   -85,   -85,
     -85,   -85,    26,   -85,    37,   -85,   142,    -1,   -59,   -85,
     -85,    80,   -85,   -85,   -85,   -85,   -85,   -85,    99,   -42,
     -85,   -85,   -85
  };

  const unsigned char
  parser::yydefgoto_[] =
  {
       0,     1,   110,   111,    28,    91,    83,    30,    89,    31,
     120,    60,    32,    84,    50,    64,    88,    85,    34,    35,
      36,   143,   144,   145,   135,    37,    66,    86,    79,    97,
     116,    80,    39,    45,    57,    73,    74,   131,    75,    55,
      40,    41,    42
  };

  const short
  parser::yytable_[] =
  {
      38,    98,    98,    54,    69,     4,    70,   140,   141,   142,
     108,   109,    71,    78,    78,    77,   -18,    65,   118,    52,
       4,   100,   119,    72,    90,     8,     9,    10,    11,    12,
      13,    14,    15,    16,    17,    18,    19,    20,    21,    22,
       8,     9,    10,    11,    12,    13,    14,    15,    16,    17,
      18,    19,    20,    21,    22,   117,   100,    43,   140,   141,
     142,    69,    56,    70,   -68,   127,   128,    52,   129,    71,
     -94,    54,   125,   126,   133,   130,    44,    93,    61,    62,
      72,    46,    47,    48,   106,    99,   132,   105,     2,     3,
     148,     4,    58,    49,    51,    67,    59,    63,    76,     5,
       6,   106,   121,    68,    81,    78,    82,    87,    92,   123,
       7,     8,     9,    10,    11,    12,    13,    14,    15,    16,
      17,    18,    19,    20,    21,    22,     4,    23,    95,    96,
     101,   103,   113,   104,   107,   112,   114,   137,    24,   122,
      52,   150,   124,   134,   136,   138,     8,     9,    10,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    20,    21,
      22,     4,   147,    25,    26,   146,    29,    27,    33,   102,
     139,   149,    53,    94,     0,   115,     0,     0,     0,     0,
       0,     8,     9,    10,    11,    12,    13,    14,    15,    16,
      17,    18,    19,    20,    21,    22,     0,     0,     0,    26
  };

  const short
  parser::yycheck_[] =
  {
       1,     1,     1,    87,    15,     3,    17,    53,    54,    55,
      77,    78,    23,    13,    13,    57,    83,    82,     4,    84,
       3,    80,     8,    34,    66,    23,    24,    25,    26,    27,
      28,    29,    30,    31,    32,    33,    34,    35,    36,    37,
      23,    24,    25,    26,    27,    28,    29,    30,    31,    32,
      33,    34,    35,    36,    37,    97,   115,     4,    53,    54,
      55,    15,    81,    17,    81,     7,     8,    84,    10,    23,
      86,    87,     7,     8,   116,    17,     4,    88,    47,    48,
      34,     4,     4,     4,    85,    85,    85,    85,     0,     1,
      85,     3,    81,     4,     4,     4,    84,    81,    14,    11,
      12,   102,    85,    81,    74,    13,    81,     4,     4,   110,
      22,    23,    24,    25,    26,    27,    28,    29,    30,    31,
      32,    33,    34,    35,    36,    37,     3,    39,    81,     4,
      82,     4,    14,    79,    79,    86,    86,    80,    50,    79,
      84,     4,    83,    81,    83,    79,    23,    24,    25,    26,
      27,    28,    29,    30,    31,    32,    33,    34,    35,    36,
      37,     3,    86,    75,    76,   137,     1,    79,     1,    82,
     133,   145,    30,    74,    -1,    95,    -1,    -1,    -1,    -1,
      -1,    23,    24,    25,    26,    27,    28,    29,    30,    31,
      32,    33,    34,    35,    36,    37,    -1,    -1,    -1,    76
  };

  const unsigned char
  parser::yystos_[] =
  {
       0,    90,     0,     1,     3,    11,    12,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    39,    50,    75,    76,    79,    93,    95,
      96,    98,   101,   102,   107,   108,   109,   114,   116,   121,
     129,   130,   131,     4,     4,   122,     4,     4,     4,     4,
     103,     4,    84,   115,    87,   128,    81,   123,    81,    84,
     100,   100,   100,    81,   104,    82,   115,     4,    81,    15,
      17,    23,    34,   124,   125,   127,    14,   128,    13,   117,
     120,    74,    81,    95,   102,   106,   116,     4,   105,    97,
     128,    94,     4,    88,   127,    81,     4,   118,     1,    85,
     117,    82,   106,     4,    79,    85,   116,    79,    77,    78,
      91,    92,    86,    14,    86,   120,   119,   128,     4,     8,
      99,    85,    79,   116,    83,     7,     8,     7,     8,    10,
      17,   126,    85,   128,    81,   113,    83,    80,    79,   113,
      53,    54,    55,   110,   111,   112,    92,    86,    85,   111,
       4
  };

  const unsigned char
  parser::yyr1_[] =
  {
       0,    89,    90,    90,    90,    90,    90,    90,    90,    90,
      90,    90,    90,    91,    91,    91,    92,    92,    92,    94,
      93,    95,    95,    97,    96,    98,    99,    99,   100,   100,
     101,   102,   103,   103,   104,   105,   105,   106,   106,   106,
     107,   107,   107,   108,   109,   110,   110,   110,   111,   112,
     112,   113,   114,   114,   114,   114,   114,   114,   114,   114,
     114,   114,   114,   114,   114,   114,   114,   115,   115,   116,
     116,   116,   118,   117,   119,   117,   120,   120,   120,   122,
     121,   123,   121,   124,   124,   124,   124,   125,   125,   126,
     126,   126,   126,   127,   128,   128,   129,   129,   130,   131
  };

  const signed char
  parser::yyr2_[] =
  {
       0,     2,     0,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     1,     1,     0,     2,     4,     0,     0,
       5,     1,     1,     0,     6,     1,     1,     1,     5,     0,
       6,     4,     1,     0,     3,     1,     0,     3,     2,     2,
       1,     1,     1,     3,     3,     1,     1,     1,     3,     1,
       2,     3,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     2,     0,     3,
       6,     6,     0,     4,     0,     5,     1,     2,     2,     0,
       5,     0,     7,     1,     1,     1,     1,     2,     1,     1,
       1,     1,     1,     5,     0,     3,     2,     1,     3,     2
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
  "CSTBUFFER", "FLOAT_TYPE", "FLOAT2_TYPE", "FLOAT3_TYPE", "FLOAT4_TYPE",
  "MAT2_TYPE", "MAT3_TYPE", "MAT4_TYPE", "BOOL_TYPE", "BOOL2_TYPE",
  "BOOL3_TYPE", "BOOL4_TYPE", "INT_TYPE", "INT2_TYPE", "INT3_TYPE",
  "INT4_TYPE", "TEXTURE1D_TYPE", "TEXTURE2D_TYPE", "TEXTURE2DSHADOW_TYPE",
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
  "','", "'{'", "'('", "')'", "':'", "'}'", "'='", "'<'", "'>'", "$accept",
  "input", "var_spec", "arguments", "function_definition", "$@1",
  "object_type", "function_declaration", "$@2", "fatal_error",
  "register_value", "register_declaration", "cbuffer", "struct",
  "struct_header", "struct_body", "struct_footer", "var_decls",
  "shader_resource", "texture2d", "sampler_state", "shader_type",
  "shader_assignment", "shader_assignments", "passstates", "base_type",
  "semantic", "var_decl", "pass", "$@3", "$@4", "passes", "tech", "$@5",
  "$@6", "scalar_type", "annotation_list", "annotation_value",
  "annotation", "annotations", "hlsl_header", "hlsl", "shader_header", YY_NULLPTR
  };
#endif


#if YYDEBUG
  const short
  parser::yyrline_[] =
  {
       0,   270,   270,   271,   272,   273,   274,   275,   276,   277,
     278,   279,   280,   283,   283,   283,   285,   286,   287,   289,
     289,   296,   296,   298,   298,   302,   304,   304,   306,   306,
     308,   314,   319,   319,   320,   321,   322,   324,   324,   324,
     326,   326,   326,   328,   333,   339,   340,   341,   344,   350,
     351,   359,   363,   364,   365,   366,   367,   368,   369,   370,
     371,   372,   373,   374,   375,   376,   377,   381,   381,   384,
     388,   389,   402,   402,   417,   417,   445,   446,   447,   453,
     453,   458,   458,   475,   475,   475,   475,   478,   479,   481,
     481,   481,   481,   483,   496,   497,   502,   505,   511,   527
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
#line 1747 "/cygdrive/c/Users/chiap/source/repos/fromasmtodisasm/TestEngine/Code/Engine/Renderer/Shaders/Parser.cpp"

#line 529 "/cygdrive/c/Users/chiap/source/repos/fromasmtodisasm/TestEngine/Code/Engine/Renderer/Shaders/Parser.yy"


#include <sstream>
void
yy::parser::error(const location_type& l, const std::string& m)
{
  std::stringstream ss;
  ss << l << ": " << m;
  gEnv->pLog->LogError(ss.str().c_str());    
}
#pragma warning(pop)
