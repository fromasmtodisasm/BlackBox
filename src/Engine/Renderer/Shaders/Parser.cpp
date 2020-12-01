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
#line 22 "Parser.yy" // lalr1.cc:397

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

    static yy::parser::symbol_type yylex(Scanner &scanner, Driver& driver) {
        return scanner.ScanToken();
    }

#line 59 "Parser.cpp" // lalr1.cc:397


// First part of user declarations.

#line 64 "Parser.cpp" // lalr1.cc:404

# ifndef YY_NULLPTR
#  if defined __cplusplus && 201103L <= __cplusplus
#   define YY_NULLPTR nullptr
#  else
#   define YY_NULLPTR 0
#  endif
# endif

#include "Parser.hpp"

// User implementation prologue.

#line 78 "Parser.cpp" // lalr1.cc:412


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
#line 164 "Parser.cpp" // lalr1.cc:479

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
        value.move< VkFormat > (that.value);
        break;

      case 15: // TRUE
      case 16: // FALSE
      case 19: // BOOL
        value.move< bool > (that.value);
        break;

      case 17: // FLOAT
        value.move< float > (that.value);
        break;

      case 18: // INT
        value.move< int > (that.value);
        break;

      case 14: // IDENTIFIER
      case 20: // STR
      case 32: // CODEBODY
      case 33: // VARNAME
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
        value.copy< VkFormat > (that.value);
        break;

      case 15: // TRUE
      case 16: // FALSE
      case 19: // BOOL
        value.copy< bool > (that.value);
        break;

      case 17: // FLOAT
        value.copy< float > (that.value);
        break;

      case 18: // INT
        value.copy< int > (that.value);
        break;

      case 14: // IDENTIFIER
      case 20: // STR
      case 32: // CODEBODY
      case 33: // VARNAME
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
        yylhs.value.build< VkFormat > ();
        break;

      case 15: // TRUE
      case 16: // FALSE
      case 19: // BOOL
        yylhs.value.build< bool > ();
        break;

      case 17: // FLOAT
        yylhs.value.build< float > ();
        break;

      case 18: // INT
        yylhs.value.build< int > ();
        break;

      case 14: // IDENTIFIER
      case 20: // STR
      case 32: // CODEBODY
      case 33: // VARNAME
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
#line 382 "Parser.yy" // lalr1.cc:859
    { gEnv->pLog->LogWarning("Empty effect"); }
#line 1170 "Parser.cpp" // lalr1.cc:859
    break;

  case 20:
#line 405 "Parser.yy" // lalr1.cc:859
    { 
		gEnv->pLog->Log("$3 Shader $1%s $3parsed", yystack_[2].value.as< std::string > ().data()); 
	}
#line 1178 "Parser.cpp" // lalr1.cc:859
    break;

  case 21:
#line 412 "Parser.yy" // lalr1.cc:859
    {
    gEnv->pLog->Log(
    "$3 New vertex format <%s> with field %s (%s = %d)", 
    yystack_[6].value.as< std::string > ().data(), yystack_[4].value.as< std::string > ().data(), "vkFormat", yystack_[2].value.as< VkFormat > ());
}
#line 1188 "Parser.cpp" // lalr1.cc:859
    break;

  case 22:
#line 420 "Parser.yy" // lalr1.cc:859
    { yylhs.value.as< VkFormat > () = yystack_[0].value.as< VkFormat > (); }
#line 1194 "Parser.cpp" // lalr1.cc:859
    break;

  case 23:
#line 421 "Parser.yy" // lalr1.cc:859
    { yylhs.value.as< VkFormat > () = yystack_[0].value.as< VkFormat > (); }
#line 1200 "Parser.cpp" // lalr1.cc:859
    break;

  case 24:
#line 422 "Parser.yy" // lalr1.cc:859
    { yylhs.value.as< VkFormat > () = yystack_[0].value.as< VkFormat > (); }
#line 1206 "Parser.cpp" // lalr1.cc:859
    break;

  case 25:
#line 423 "Parser.yy" // lalr1.cc:859
    { yylhs.value.as< VkFormat > () = yystack_[0].value.as< VkFormat > (); }
#line 1212 "Parser.cpp" // lalr1.cc:859
    break;

  case 26:
#line 424 "Parser.yy" // lalr1.cc:859
    { yylhs.value.as< VkFormat > () = yystack_[0].value.as< VkFormat > (); }
#line 1218 "Parser.cpp" // lalr1.cc:859
    break;

  case 27:
#line 425 "Parser.yy" // lalr1.cc:859
    { yylhs.value.as< VkFormat > () = yystack_[0].value.as< VkFormat > (); }
#line 1224 "Parser.cpp" // lalr1.cc:859
    break;

  case 28:
#line 426 "Parser.yy" // lalr1.cc:859
    { yylhs.value.as< VkFormat > () = yystack_[0].value.as< VkFormat > (); }
#line 1230 "Parser.cpp" // lalr1.cc:859
    break;

  case 29:
#line 427 "Parser.yy" // lalr1.cc:859
    { yylhs.value.as< VkFormat > () = yystack_[0].value.as< VkFormat > (); }
#line 1236 "Parser.cpp" // lalr1.cc:859
    break;

  case 30:
#line 428 "Parser.yy" // lalr1.cc:859
    { yylhs.value.as< VkFormat > () = yystack_[0].value.as< VkFormat > (); }
#line 1242 "Parser.cpp" // lalr1.cc:859
    break;

  case 31:
#line 429 "Parser.yy" // lalr1.cc:859
    { yylhs.value.as< VkFormat > () = yystack_[0].value.as< VkFormat > (); }
#line 1248 "Parser.cpp" // lalr1.cc:859
    break;

  case 32:
#line 430 "Parser.yy" // lalr1.cc:859
    { yylhs.value.as< VkFormat > () = yystack_[0].value.as< VkFormat > (); }
#line 1254 "Parser.cpp" // lalr1.cc:859
    break;

  case 33:
#line 431 "Parser.yy" // lalr1.cc:859
    { yylhs.value.as< VkFormat > () = yystack_[0].value.as< VkFormat > (); }
#line 1260 "Parser.cpp" // lalr1.cc:859
    break;

  case 34:
#line 432 "Parser.yy" // lalr1.cc:859
    { yylhs.value.as< VkFormat > () = yystack_[0].value.as< VkFormat > (); }
#line 1266 "Parser.cpp" // lalr1.cc:859
    break;

  case 35:
#line 433 "Parser.yy" // lalr1.cc:859
    { yylhs.value.as< VkFormat > () = yystack_[0].value.as< VkFormat > (); }
#line 1272 "Parser.cpp" // lalr1.cc:859
    break;

  case 36:
#line 434 "Parser.yy" // lalr1.cc:859
    { yylhs.value.as< VkFormat > () = yystack_[0].value.as< VkFormat > (); }
#line 1278 "Parser.cpp" // lalr1.cc:859
    break;

  case 37:
#line 435 "Parser.yy" // lalr1.cc:859
    { yylhs.value.as< VkFormat > () = yystack_[0].value.as< VkFormat > (); }
#line 1284 "Parser.cpp" // lalr1.cc:859
    break;

  case 38:
#line 436 "Parser.yy" // lalr1.cc:859
    { yylhs.value.as< VkFormat > () = yystack_[0].value.as< VkFormat > (); }
#line 1290 "Parser.cpp" // lalr1.cc:859
    break;

  case 39:
#line 437 "Parser.yy" // lalr1.cc:859
    { yylhs.value.as< VkFormat > () = yystack_[0].value.as< VkFormat > (); }
#line 1296 "Parser.cpp" // lalr1.cc:859
    break;

  case 40:
#line 438 "Parser.yy" // lalr1.cc:859
    { yylhs.value.as< VkFormat > () = yystack_[0].value.as< VkFormat > (); }
#line 1302 "Parser.cpp" // lalr1.cc:859
    break;

  case 41:
#line 439 "Parser.yy" // lalr1.cc:859
    { yylhs.value.as< VkFormat > () = yystack_[0].value.as< VkFormat > (); }
#line 1308 "Parser.cpp" // lalr1.cc:859
    break;

  case 42:
#line 440 "Parser.yy" // lalr1.cc:859
    { yylhs.value.as< VkFormat > () = yystack_[0].value.as< VkFormat > (); }
#line 1314 "Parser.cpp" // lalr1.cc:859
    break;

  case 43:
#line 441 "Parser.yy" // lalr1.cc:859
    { yylhs.value.as< VkFormat > () = yystack_[0].value.as< VkFormat > (); }
#line 1320 "Parser.cpp" // lalr1.cc:859
    break;

  case 44:
#line 442 "Parser.yy" // lalr1.cc:859
    { yylhs.value.as< VkFormat > () = yystack_[0].value.as< VkFormat > (); }
#line 1326 "Parser.cpp" // lalr1.cc:859
    break;

  case 45:
#line 443 "Parser.yy" // lalr1.cc:859
    { yylhs.value.as< VkFormat > () = yystack_[0].value.as< VkFormat > (); }
#line 1332 "Parser.cpp" // lalr1.cc:859
    break;

  case 46:
#line 444 "Parser.yy" // lalr1.cc:859
    { yylhs.value.as< VkFormat > () = yystack_[0].value.as< VkFormat > (); }
#line 1338 "Parser.cpp" // lalr1.cc:859
    break;

  case 47:
#line 445 "Parser.yy" // lalr1.cc:859
    { yylhs.value.as< VkFormat > () = yystack_[0].value.as< VkFormat > (); }
#line 1344 "Parser.cpp" // lalr1.cc:859
    break;

  case 48:
#line 446 "Parser.yy" // lalr1.cc:859
    { yylhs.value.as< VkFormat > () = yystack_[0].value.as< VkFormat > (); }
#line 1350 "Parser.cpp" // lalr1.cc:859
    break;

  case 49:
#line 447 "Parser.yy" // lalr1.cc:859
    { yylhs.value.as< VkFormat > () = yystack_[0].value.as< VkFormat > (); }
#line 1356 "Parser.cpp" // lalr1.cc:859
    break;

  case 50:
#line 448 "Parser.yy" // lalr1.cc:859
    { yylhs.value.as< VkFormat > () = yystack_[0].value.as< VkFormat > (); }
#line 1362 "Parser.cpp" // lalr1.cc:859
    break;

  case 51:
#line 449 "Parser.yy" // lalr1.cc:859
    { yylhs.value.as< VkFormat > () = yystack_[0].value.as< VkFormat > (); }
#line 1368 "Parser.cpp" // lalr1.cc:859
    break;

  case 52:
#line 450 "Parser.yy" // lalr1.cc:859
    { yylhs.value.as< VkFormat > () = yystack_[0].value.as< VkFormat > (); }
#line 1374 "Parser.cpp" // lalr1.cc:859
    break;

  case 53:
#line 451 "Parser.yy" // lalr1.cc:859
    { yylhs.value.as< VkFormat > () = yystack_[0].value.as< VkFormat > (); }
#line 1380 "Parser.cpp" // lalr1.cc:859
    break;

  case 54:
#line 452 "Parser.yy" // lalr1.cc:859
    { yylhs.value.as< VkFormat > () = yystack_[0].value.as< VkFormat > (); }
#line 1386 "Parser.cpp" // lalr1.cc:859
    break;

  case 55:
#line 453 "Parser.yy" // lalr1.cc:859
    { yylhs.value.as< VkFormat > () = yystack_[0].value.as< VkFormat > (); }
#line 1392 "Parser.cpp" // lalr1.cc:859
    break;

  case 56:
#line 454 "Parser.yy" // lalr1.cc:859
    { yylhs.value.as< VkFormat > () = yystack_[0].value.as< VkFormat > (); }
#line 1398 "Parser.cpp" // lalr1.cc:859
    break;

  case 57:
#line 455 "Parser.yy" // lalr1.cc:859
    { yylhs.value.as< VkFormat > () = yystack_[0].value.as< VkFormat > (); }
#line 1404 "Parser.cpp" // lalr1.cc:859
    break;

  case 58:
#line 456 "Parser.yy" // lalr1.cc:859
    { yylhs.value.as< VkFormat > () = yystack_[0].value.as< VkFormat > (); }
#line 1410 "Parser.cpp" // lalr1.cc:859
    break;

  case 59:
#line 457 "Parser.yy" // lalr1.cc:859
    { yylhs.value.as< VkFormat > () = yystack_[0].value.as< VkFormat > (); }
#line 1416 "Parser.cpp" // lalr1.cc:859
    break;

  case 60:
#line 458 "Parser.yy" // lalr1.cc:859
    { yylhs.value.as< VkFormat > () = yystack_[0].value.as< VkFormat > (); }
#line 1422 "Parser.cpp" // lalr1.cc:859
    break;

  case 61:
#line 459 "Parser.yy" // lalr1.cc:859
    { yylhs.value.as< VkFormat > () = yystack_[0].value.as< VkFormat > (); }
#line 1428 "Parser.cpp" // lalr1.cc:859
    break;

  case 62:
#line 460 "Parser.yy" // lalr1.cc:859
    { yylhs.value.as< VkFormat > () = yystack_[0].value.as< VkFormat > (); }
#line 1434 "Parser.cpp" // lalr1.cc:859
    break;

  case 63:
#line 461 "Parser.yy" // lalr1.cc:859
    { yylhs.value.as< VkFormat > () = yystack_[0].value.as< VkFormat > (); }
#line 1440 "Parser.cpp" // lalr1.cc:859
    break;

  case 64:
#line 462 "Parser.yy" // lalr1.cc:859
    { yylhs.value.as< VkFormat > () = yystack_[0].value.as< VkFormat > (); }
#line 1446 "Parser.cpp" // lalr1.cc:859
    break;

  case 65:
#line 463 "Parser.yy" // lalr1.cc:859
    { yylhs.value.as< VkFormat > () = yystack_[0].value.as< VkFormat > (); }
#line 1452 "Parser.cpp" // lalr1.cc:859
    break;

  case 66:
#line 464 "Parser.yy" // lalr1.cc:859
    { yylhs.value.as< VkFormat > () = yystack_[0].value.as< VkFormat > (); }
#line 1458 "Parser.cpp" // lalr1.cc:859
    break;

  case 67:
#line 465 "Parser.yy" // lalr1.cc:859
    { yylhs.value.as< VkFormat > () = yystack_[0].value.as< VkFormat > (); }
#line 1464 "Parser.cpp" // lalr1.cc:859
    break;

  case 68:
#line 466 "Parser.yy" // lalr1.cc:859
    { yylhs.value.as< VkFormat > () = yystack_[0].value.as< VkFormat > (); }
#line 1470 "Parser.cpp" // lalr1.cc:859
    break;

  case 69:
#line 467 "Parser.yy" // lalr1.cc:859
    { yylhs.value.as< VkFormat > () = yystack_[0].value.as< VkFormat > (); }
#line 1476 "Parser.cpp" // lalr1.cc:859
    break;

  case 70:
#line 468 "Parser.yy" // lalr1.cc:859
    { yylhs.value.as< VkFormat > () = yystack_[0].value.as< VkFormat > (); }
#line 1482 "Parser.cpp" // lalr1.cc:859
    break;

  case 71:
#line 469 "Parser.yy" // lalr1.cc:859
    { yylhs.value.as< VkFormat > () = yystack_[0].value.as< VkFormat > (); }
#line 1488 "Parser.cpp" // lalr1.cc:859
    break;

  case 72:
#line 470 "Parser.yy" // lalr1.cc:859
    { yylhs.value.as< VkFormat > () = yystack_[0].value.as< VkFormat > (); }
#line 1494 "Parser.cpp" // lalr1.cc:859
    break;

  case 73:
#line 471 "Parser.yy" // lalr1.cc:859
    { yylhs.value.as< VkFormat > () = yystack_[0].value.as< VkFormat > (); }
#line 1500 "Parser.cpp" // lalr1.cc:859
    break;

  case 74:
#line 472 "Parser.yy" // lalr1.cc:859
    { yylhs.value.as< VkFormat > () = yystack_[0].value.as< VkFormat > (); }
#line 1506 "Parser.cpp" // lalr1.cc:859
    break;

  case 75:
#line 473 "Parser.yy" // lalr1.cc:859
    { yylhs.value.as< VkFormat > () = yystack_[0].value.as< VkFormat > (); }
#line 1512 "Parser.cpp" // lalr1.cc:859
    break;

  case 76:
#line 474 "Parser.yy" // lalr1.cc:859
    { yylhs.value.as< VkFormat > () = yystack_[0].value.as< VkFormat > (); }
#line 1518 "Parser.cpp" // lalr1.cc:859
    break;

  case 77:
#line 475 "Parser.yy" // lalr1.cc:859
    { yylhs.value.as< VkFormat > () = yystack_[0].value.as< VkFormat > (); }
#line 1524 "Parser.cpp" // lalr1.cc:859
    break;

  case 78:
#line 476 "Parser.yy" // lalr1.cc:859
    { yylhs.value.as< VkFormat > () = yystack_[0].value.as< VkFormat > (); }
#line 1530 "Parser.cpp" // lalr1.cc:859
    break;

  case 79:
#line 477 "Parser.yy" // lalr1.cc:859
    { yylhs.value.as< VkFormat > () = yystack_[0].value.as< VkFormat > (); }
#line 1536 "Parser.cpp" // lalr1.cc:859
    break;

  case 80:
#line 478 "Parser.yy" // lalr1.cc:859
    { yylhs.value.as< VkFormat > () = yystack_[0].value.as< VkFormat > (); }
#line 1542 "Parser.cpp" // lalr1.cc:859
    break;

  case 81:
#line 479 "Parser.yy" // lalr1.cc:859
    { yylhs.value.as< VkFormat > () = yystack_[0].value.as< VkFormat > (); }
#line 1548 "Parser.cpp" // lalr1.cc:859
    break;

  case 82:
#line 480 "Parser.yy" // lalr1.cc:859
    { yylhs.value.as< VkFormat > () = yystack_[0].value.as< VkFormat > (); }
#line 1554 "Parser.cpp" // lalr1.cc:859
    break;

  case 83:
#line 481 "Parser.yy" // lalr1.cc:859
    { yylhs.value.as< VkFormat > () = yystack_[0].value.as< VkFormat > (); }
#line 1560 "Parser.cpp" // lalr1.cc:859
    break;

  case 84:
#line 482 "Parser.yy" // lalr1.cc:859
    { yylhs.value.as< VkFormat > () = yystack_[0].value.as< VkFormat > (); }
#line 1566 "Parser.cpp" // lalr1.cc:859
    break;

  case 85:
#line 483 "Parser.yy" // lalr1.cc:859
    { yylhs.value.as< VkFormat > () = yystack_[0].value.as< VkFormat > (); }
#line 1572 "Parser.cpp" // lalr1.cc:859
    break;

  case 86:
#line 484 "Parser.yy" // lalr1.cc:859
    { yylhs.value.as< VkFormat > () = yystack_[0].value.as< VkFormat > (); }
#line 1578 "Parser.cpp" // lalr1.cc:859
    break;

  case 87:
#line 485 "Parser.yy" // lalr1.cc:859
    { yylhs.value.as< VkFormat > () = yystack_[0].value.as< VkFormat > (); }
#line 1584 "Parser.cpp" // lalr1.cc:859
    break;

  case 88:
#line 486 "Parser.yy" // lalr1.cc:859
    { yylhs.value.as< VkFormat > () = yystack_[0].value.as< VkFormat > (); }
#line 1590 "Parser.cpp" // lalr1.cc:859
    break;

  case 89:
#line 487 "Parser.yy" // lalr1.cc:859
    { yylhs.value.as< VkFormat > () = yystack_[0].value.as< VkFormat > (); }
#line 1596 "Parser.cpp" // lalr1.cc:859
    break;

  case 90:
#line 488 "Parser.yy" // lalr1.cc:859
    { yylhs.value.as< VkFormat > () = yystack_[0].value.as< VkFormat > (); }
#line 1602 "Parser.cpp" // lalr1.cc:859
    break;

  case 91:
#line 489 "Parser.yy" // lalr1.cc:859
    { yylhs.value.as< VkFormat > () = yystack_[0].value.as< VkFormat > (); }
#line 1608 "Parser.cpp" // lalr1.cc:859
    break;

  case 92:
#line 490 "Parser.yy" // lalr1.cc:859
    { yylhs.value.as< VkFormat > () = yystack_[0].value.as< VkFormat > (); }
#line 1614 "Parser.cpp" // lalr1.cc:859
    break;

  case 93:
#line 491 "Parser.yy" // lalr1.cc:859
    { yylhs.value.as< VkFormat > () = yystack_[0].value.as< VkFormat > (); }
#line 1620 "Parser.cpp" // lalr1.cc:859
    break;

  case 94:
#line 492 "Parser.yy" // lalr1.cc:859
    { yylhs.value.as< VkFormat > () = yystack_[0].value.as< VkFormat > (); }
#line 1626 "Parser.cpp" // lalr1.cc:859
    break;

  case 95:
#line 493 "Parser.yy" // lalr1.cc:859
    { yylhs.value.as< VkFormat > () = yystack_[0].value.as< VkFormat > (); }
#line 1632 "Parser.cpp" // lalr1.cc:859
    break;

  case 96:
#line 494 "Parser.yy" // lalr1.cc:859
    { yylhs.value.as< VkFormat > () = yystack_[0].value.as< VkFormat > (); }
#line 1638 "Parser.cpp" // lalr1.cc:859
    break;

  case 97:
#line 495 "Parser.yy" // lalr1.cc:859
    { yylhs.value.as< VkFormat > () = yystack_[0].value.as< VkFormat > (); }
#line 1644 "Parser.cpp" // lalr1.cc:859
    break;

  case 98:
#line 496 "Parser.yy" // lalr1.cc:859
    { yylhs.value.as< VkFormat > () = yystack_[0].value.as< VkFormat > (); }
#line 1650 "Parser.cpp" // lalr1.cc:859
    break;

  case 99:
#line 497 "Parser.yy" // lalr1.cc:859
    { yylhs.value.as< VkFormat > () = yystack_[0].value.as< VkFormat > (); }
#line 1656 "Parser.cpp" // lalr1.cc:859
    break;

  case 100:
#line 498 "Parser.yy" // lalr1.cc:859
    { yylhs.value.as< VkFormat > () = yystack_[0].value.as< VkFormat > (); }
#line 1662 "Parser.cpp" // lalr1.cc:859
    break;

  case 101:
#line 499 "Parser.yy" // lalr1.cc:859
    { yylhs.value.as< VkFormat > () = yystack_[0].value.as< VkFormat > (); }
#line 1668 "Parser.cpp" // lalr1.cc:859
    break;

  case 102:
#line 500 "Parser.yy" // lalr1.cc:859
    { yylhs.value.as< VkFormat > () = yystack_[0].value.as< VkFormat > (); }
#line 1674 "Parser.cpp" // lalr1.cc:859
    break;

  case 103:
#line 501 "Parser.yy" // lalr1.cc:859
    { yylhs.value.as< VkFormat > () = yystack_[0].value.as< VkFormat > (); }
#line 1680 "Parser.cpp" // lalr1.cc:859
    break;

  case 104:
#line 502 "Parser.yy" // lalr1.cc:859
    { yylhs.value.as< VkFormat > () = yystack_[0].value.as< VkFormat > (); }
#line 1686 "Parser.cpp" // lalr1.cc:859
    break;

  case 105:
#line 503 "Parser.yy" // lalr1.cc:859
    { yylhs.value.as< VkFormat > () = yystack_[0].value.as< VkFormat > (); }
#line 1692 "Parser.cpp" // lalr1.cc:859
    break;

  case 106:
#line 504 "Parser.yy" // lalr1.cc:859
    { yylhs.value.as< VkFormat > () = yystack_[0].value.as< VkFormat > (); }
#line 1698 "Parser.cpp" // lalr1.cc:859
    break;

  case 107:
#line 505 "Parser.yy" // lalr1.cc:859
    { yylhs.value.as< VkFormat > () = yystack_[0].value.as< VkFormat > (); }
#line 1704 "Parser.cpp" // lalr1.cc:859
    break;

  case 108:
#line 506 "Parser.yy" // lalr1.cc:859
    { yylhs.value.as< VkFormat > () = yystack_[0].value.as< VkFormat > (); }
#line 1710 "Parser.cpp" // lalr1.cc:859
    break;

  case 109:
#line 507 "Parser.yy" // lalr1.cc:859
    { yylhs.value.as< VkFormat > () = yystack_[0].value.as< VkFormat > (); }
#line 1716 "Parser.cpp" // lalr1.cc:859
    break;

  case 110:
#line 508 "Parser.yy" // lalr1.cc:859
    { yylhs.value.as< VkFormat > () = yystack_[0].value.as< VkFormat > (); }
#line 1722 "Parser.cpp" // lalr1.cc:859
    break;

  case 111:
#line 509 "Parser.yy" // lalr1.cc:859
    { yylhs.value.as< VkFormat > () = yystack_[0].value.as< VkFormat > (); }
#line 1728 "Parser.cpp" // lalr1.cc:859
    break;

  case 112:
#line 510 "Parser.yy" // lalr1.cc:859
    { yylhs.value.as< VkFormat > () = yystack_[0].value.as< VkFormat > (); }
#line 1734 "Parser.cpp" // lalr1.cc:859
    break;

  case 113:
#line 511 "Parser.yy" // lalr1.cc:859
    { yylhs.value.as< VkFormat > () = yystack_[0].value.as< VkFormat > (); }
#line 1740 "Parser.cpp" // lalr1.cc:859
    break;

  case 114:
#line 512 "Parser.yy" // lalr1.cc:859
    { yylhs.value.as< VkFormat > () = yystack_[0].value.as< VkFormat > (); }
#line 1746 "Parser.cpp" // lalr1.cc:859
    break;

  case 115:
#line 513 "Parser.yy" // lalr1.cc:859
    { yylhs.value.as< VkFormat > () = yystack_[0].value.as< VkFormat > (); }
#line 1752 "Parser.cpp" // lalr1.cc:859
    break;

  case 116:
#line 514 "Parser.yy" // lalr1.cc:859
    { yylhs.value.as< VkFormat > () = yystack_[0].value.as< VkFormat > (); }
#line 1758 "Parser.cpp" // lalr1.cc:859
    break;

  case 117:
#line 515 "Parser.yy" // lalr1.cc:859
    { yylhs.value.as< VkFormat > () = yystack_[0].value.as< VkFormat > (); }
#line 1764 "Parser.cpp" // lalr1.cc:859
    break;

  case 118:
#line 516 "Parser.yy" // lalr1.cc:859
    { yylhs.value.as< VkFormat > () = yystack_[0].value.as< VkFormat > (); }
#line 1770 "Parser.cpp" // lalr1.cc:859
    break;

  case 119:
#line 517 "Parser.yy" // lalr1.cc:859
    { yylhs.value.as< VkFormat > () = yystack_[0].value.as< VkFormat > (); }
#line 1776 "Parser.cpp" // lalr1.cc:859
    break;

  case 120:
#line 518 "Parser.yy" // lalr1.cc:859
    { yylhs.value.as< VkFormat > () = yystack_[0].value.as< VkFormat > (); }
#line 1782 "Parser.cpp" // lalr1.cc:859
    break;

  case 121:
#line 519 "Parser.yy" // lalr1.cc:859
    { yylhs.value.as< VkFormat > () = yystack_[0].value.as< VkFormat > (); }
#line 1788 "Parser.cpp" // lalr1.cc:859
    break;

  case 122:
#line 520 "Parser.yy" // lalr1.cc:859
    { yylhs.value.as< VkFormat > () = yystack_[0].value.as< VkFormat > (); }
#line 1794 "Parser.cpp" // lalr1.cc:859
    break;

  case 123:
#line 521 "Parser.yy" // lalr1.cc:859
    { yylhs.value.as< VkFormat > () = yystack_[0].value.as< VkFormat > (); }
#line 1800 "Parser.cpp" // lalr1.cc:859
    break;

  case 124:
#line 522 "Parser.yy" // lalr1.cc:859
    { yylhs.value.as< VkFormat > () = yystack_[0].value.as< VkFormat > (); }
#line 1806 "Parser.cpp" // lalr1.cc:859
    break;

  case 125:
#line 523 "Parser.yy" // lalr1.cc:859
    { yylhs.value.as< VkFormat > () = yystack_[0].value.as< VkFormat > (); }
#line 1812 "Parser.cpp" // lalr1.cc:859
    break;

  case 126:
#line 524 "Parser.yy" // lalr1.cc:859
    { yylhs.value.as< VkFormat > () = yystack_[0].value.as< VkFormat > (); }
#line 1818 "Parser.cpp" // lalr1.cc:859
    break;

  case 127:
#line 525 "Parser.yy" // lalr1.cc:859
    { yylhs.value.as< VkFormat > () = yystack_[0].value.as< VkFormat > (); }
#line 1824 "Parser.cpp" // lalr1.cc:859
    break;

  case 128:
#line 526 "Parser.yy" // lalr1.cc:859
    { yylhs.value.as< VkFormat > () = yystack_[0].value.as< VkFormat > (); }
#line 1830 "Parser.cpp" // lalr1.cc:859
    break;

  case 129:
#line 527 "Parser.yy" // lalr1.cc:859
    { yylhs.value.as< VkFormat > () = yystack_[0].value.as< VkFormat > (); }
#line 1836 "Parser.cpp" // lalr1.cc:859
    break;

  case 130:
#line 528 "Parser.yy" // lalr1.cc:859
    { yylhs.value.as< VkFormat > () = yystack_[0].value.as< VkFormat > (); }
#line 1842 "Parser.cpp" // lalr1.cc:859
    break;

  case 131:
#line 529 "Parser.yy" // lalr1.cc:859
    { yylhs.value.as< VkFormat > () = yystack_[0].value.as< VkFormat > (); }
#line 1848 "Parser.cpp" // lalr1.cc:859
    break;

  case 132:
#line 530 "Parser.yy" // lalr1.cc:859
    { yylhs.value.as< VkFormat > () = yystack_[0].value.as< VkFormat > (); }
#line 1854 "Parser.cpp" // lalr1.cc:859
    break;

  case 133:
#line 531 "Parser.yy" // lalr1.cc:859
    { yylhs.value.as< VkFormat > () = yystack_[0].value.as< VkFormat > (); }
#line 1860 "Parser.cpp" // lalr1.cc:859
    break;

  case 134:
#line 532 "Parser.yy" // lalr1.cc:859
    { yylhs.value.as< VkFormat > () = yystack_[0].value.as< VkFormat > (); }
#line 1866 "Parser.cpp" // lalr1.cc:859
    break;

  case 135:
#line 533 "Parser.yy" // lalr1.cc:859
    { yylhs.value.as< VkFormat > () = yystack_[0].value.as< VkFormat > (); }
#line 1872 "Parser.cpp" // lalr1.cc:859
    break;

  case 136:
#line 534 "Parser.yy" // lalr1.cc:859
    { yylhs.value.as< VkFormat > () = yystack_[0].value.as< VkFormat > (); }
#line 1878 "Parser.cpp" // lalr1.cc:859
    break;

  case 137:
#line 535 "Parser.yy" // lalr1.cc:859
    { yylhs.value.as< VkFormat > () = yystack_[0].value.as< VkFormat > (); }
#line 1884 "Parser.cpp" // lalr1.cc:859
    break;

  case 138:
#line 536 "Parser.yy" // lalr1.cc:859
    { yylhs.value.as< VkFormat > () = yystack_[0].value.as< VkFormat > (); }
#line 1890 "Parser.cpp" // lalr1.cc:859
    break;

  case 139:
#line 537 "Parser.yy" // lalr1.cc:859
    { yylhs.value.as< VkFormat > () = yystack_[0].value.as< VkFormat > (); }
#line 1896 "Parser.cpp" // lalr1.cc:859
    break;

  case 140:
#line 538 "Parser.yy" // lalr1.cc:859
    { yylhs.value.as< VkFormat > () = yystack_[0].value.as< VkFormat > (); }
#line 1902 "Parser.cpp" // lalr1.cc:859
    break;

  case 141:
#line 539 "Parser.yy" // lalr1.cc:859
    { yylhs.value.as< VkFormat > () = yystack_[0].value.as< VkFormat > (); }
#line 1908 "Parser.cpp" // lalr1.cc:859
    break;

  case 142:
#line 540 "Parser.yy" // lalr1.cc:859
    { yylhs.value.as< VkFormat > () = yystack_[0].value.as< VkFormat > (); }
#line 1914 "Parser.cpp" // lalr1.cc:859
    break;

  case 143:
#line 541 "Parser.yy" // lalr1.cc:859
    { yylhs.value.as< VkFormat > () = yystack_[0].value.as< VkFormat > (); }
#line 1920 "Parser.cpp" // lalr1.cc:859
    break;

  case 144:
#line 542 "Parser.yy" // lalr1.cc:859
    { yylhs.value.as< VkFormat > () = yystack_[0].value.as< VkFormat > (); }
#line 1926 "Parser.cpp" // lalr1.cc:859
    break;

  case 145:
#line 543 "Parser.yy" // lalr1.cc:859
    { yylhs.value.as< VkFormat > () = yystack_[0].value.as< VkFormat > (); }
#line 1932 "Parser.cpp" // lalr1.cc:859
    break;

  case 146:
#line 544 "Parser.yy" // lalr1.cc:859
    { yylhs.value.as< VkFormat > () = yystack_[0].value.as< VkFormat > (); }
#line 1938 "Parser.cpp" // lalr1.cc:859
    break;

  case 147:
#line 545 "Parser.yy" // lalr1.cc:859
    { yylhs.value.as< VkFormat > () = yystack_[0].value.as< VkFormat > (); }
#line 1944 "Parser.cpp" // lalr1.cc:859
    break;

  case 148:
#line 546 "Parser.yy" // lalr1.cc:859
    { yylhs.value.as< VkFormat > () = yystack_[0].value.as< VkFormat > (); }
#line 1950 "Parser.cpp" // lalr1.cc:859
    break;

  case 149:
#line 547 "Parser.yy" // lalr1.cc:859
    { yylhs.value.as< VkFormat > () = yystack_[0].value.as< VkFormat > (); }
#line 1956 "Parser.cpp" // lalr1.cc:859
    break;

  case 150:
#line 548 "Parser.yy" // lalr1.cc:859
    { yylhs.value.as< VkFormat > () = yystack_[0].value.as< VkFormat > (); }
#line 1962 "Parser.cpp" // lalr1.cc:859
    break;

  case 151:
#line 549 "Parser.yy" // lalr1.cc:859
    { yylhs.value.as< VkFormat > () = yystack_[0].value.as< VkFormat > (); }
#line 1968 "Parser.cpp" // lalr1.cc:859
    break;

  case 152:
#line 550 "Parser.yy" // lalr1.cc:859
    { yylhs.value.as< VkFormat > () = yystack_[0].value.as< VkFormat > (); }
#line 1974 "Parser.cpp" // lalr1.cc:859
    break;

  case 153:
#line 551 "Parser.yy" // lalr1.cc:859
    { yylhs.value.as< VkFormat > () = yystack_[0].value.as< VkFormat > (); }
#line 1980 "Parser.cpp" // lalr1.cc:859
    break;

  case 154:
#line 552 "Parser.yy" // lalr1.cc:859
    { yylhs.value.as< VkFormat > () = yystack_[0].value.as< VkFormat > (); }
#line 1986 "Parser.cpp" // lalr1.cc:859
    break;

  case 155:
#line 553 "Parser.yy" // lalr1.cc:859
    { yylhs.value.as< VkFormat > () = yystack_[0].value.as< VkFormat > (); }
#line 1992 "Parser.cpp" // lalr1.cc:859
    break;

  case 156:
#line 554 "Parser.yy" // lalr1.cc:859
    { yylhs.value.as< VkFormat > () = yystack_[0].value.as< VkFormat > (); }
#line 1998 "Parser.cpp" // lalr1.cc:859
    break;

  case 157:
#line 555 "Parser.yy" // lalr1.cc:859
    { yylhs.value.as< VkFormat > () = yystack_[0].value.as< VkFormat > (); }
#line 2004 "Parser.cpp" // lalr1.cc:859
    break;

  case 158:
#line 556 "Parser.yy" // lalr1.cc:859
    { yylhs.value.as< VkFormat > () = yystack_[0].value.as< VkFormat > (); }
#line 2010 "Parser.cpp" // lalr1.cc:859
    break;

  case 159:
#line 557 "Parser.yy" // lalr1.cc:859
    { yylhs.value.as< VkFormat > () = yystack_[0].value.as< VkFormat > (); }
#line 2016 "Parser.cpp" // lalr1.cc:859
    break;

  case 160:
#line 558 "Parser.yy" // lalr1.cc:859
    { yylhs.value.as< VkFormat > () = yystack_[0].value.as< VkFormat > (); }
#line 2022 "Parser.cpp" // lalr1.cc:859
    break;

  case 161:
#line 559 "Parser.yy" // lalr1.cc:859
    { yylhs.value.as< VkFormat > () = yystack_[0].value.as< VkFormat > (); }
#line 2028 "Parser.cpp" // lalr1.cc:859
    break;

  case 162:
#line 560 "Parser.yy" // lalr1.cc:859
    { yylhs.value.as< VkFormat > () = yystack_[0].value.as< VkFormat > (); }
#line 2034 "Parser.cpp" // lalr1.cc:859
    break;

  case 163:
#line 561 "Parser.yy" // lalr1.cc:859
    { yylhs.value.as< VkFormat > () = yystack_[0].value.as< VkFormat > (); }
#line 2040 "Parser.cpp" // lalr1.cc:859
    break;

  case 164:
#line 562 "Parser.yy" // lalr1.cc:859
    { yylhs.value.as< VkFormat > () = yystack_[0].value.as< VkFormat > (); }
#line 2046 "Parser.cpp" // lalr1.cc:859
    break;

  case 165:
#line 563 "Parser.yy" // lalr1.cc:859
    { yylhs.value.as< VkFormat > () = yystack_[0].value.as< VkFormat > (); }
#line 2052 "Parser.cpp" // lalr1.cc:859
    break;

  case 166:
#line 564 "Parser.yy" // lalr1.cc:859
    { yylhs.value.as< VkFormat > () = yystack_[0].value.as< VkFormat > (); }
#line 2058 "Parser.cpp" // lalr1.cc:859
    break;

  case 167:
#line 565 "Parser.yy" // lalr1.cc:859
    { yylhs.value.as< VkFormat > () = yystack_[0].value.as< VkFormat > (); }
#line 2064 "Parser.cpp" // lalr1.cc:859
    break;

  case 168:
#line 566 "Parser.yy" // lalr1.cc:859
    { yylhs.value.as< VkFormat > () = yystack_[0].value.as< VkFormat > (); }
#line 2070 "Parser.cpp" // lalr1.cc:859
    break;

  case 169:
#line 567 "Parser.yy" // lalr1.cc:859
    { yylhs.value.as< VkFormat > () = yystack_[0].value.as< VkFormat > (); }
#line 2076 "Parser.cpp" // lalr1.cc:859
    break;

  case 170:
#line 568 "Parser.yy" // lalr1.cc:859
    { yylhs.value.as< VkFormat > () = yystack_[0].value.as< VkFormat > (); }
#line 2082 "Parser.cpp" // lalr1.cc:859
    break;

  case 171:
#line 569 "Parser.yy" // lalr1.cc:859
    { yylhs.value.as< VkFormat > () = yystack_[0].value.as< VkFormat > (); }
#line 2088 "Parser.cpp" // lalr1.cc:859
    break;

  case 172:
#line 570 "Parser.yy" // lalr1.cc:859
    { yylhs.value.as< VkFormat > () = yystack_[0].value.as< VkFormat > (); }
#line 2094 "Parser.cpp" // lalr1.cc:859
    break;

  case 173:
#line 571 "Parser.yy" // lalr1.cc:859
    { yylhs.value.as< VkFormat > () = yystack_[0].value.as< VkFormat > (); }
#line 2100 "Parser.cpp" // lalr1.cc:859
    break;

  case 174:
#line 572 "Parser.yy" // lalr1.cc:859
    { yylhs.value.as< VkFormat > () = yystack_[0].value.as< VkFormat > (); }
#line 2106 "Parser.cpp" // lalr1.cc:859
    break;

  case 175:
#line 573 "Parser.yy" // lalr1.cc:859
    { yylhs.value.as< VkFormat > () = yystack_[0].value.as< VkFormat > (); }
#line 2112 "Parser.cpp" // lalr1.cc:859
    break;

  case 176:
#line 574 "Parser.yy" // lalr1.cc:859
    { yylhs.value.as< VkFormat > () = yystack_[0].value.as< VkFormat > (); }
#line 2118 "Parser.cpp" // lalr1.cc:859
    break;

  case 177:
#line 575 "Parser.yy" // lalr1.cc:859
    { yylhs.value.as< VkFormat > () = yystack_[0].value.as< VkFormat > (); }
#line 2124 "Parser.cpp" // lalr1.cc:859
    break;

  case 178:
#line 576 "Parser.yy" // lalr1.cc:859
    { yylhs.value.as< VkFormat > () = yystack_[0].value.as< VkFormat > (); }
#line 2130 "Parser.cpp" // lalr1.cc:859
    break;

  case 179:
#line 577 "Parser.yy" // lalr1.cc:859
    { yylhs.value.as< VkFormat > () = yystack_[0].value.as< VkFormat > (); }
#line 2136 "Parser.cpp" // lalr1.cc:859
    break;

  case 180:
#line 578 "Parser.yy" // lalr1.cc:859
    { yylhs.value.as< VkFormat > () = yystack_[0].value.as< VkFormat > (); }
#line 2142 "Parser.cpp" // lalr1.cc:859
    break;

  case 181:
#line 579 "Parser.yy" // lalr1.cc:859
    { yylhs.value.as< VkFormat > () = yystack_[0].value.as< VkFormat > (); }
#line 2148 "Parser.cpp" // lalr1.cc:859
    break;

  case 182:
#line 580 "Parser.yy" // lalr1.cc:859
    { yylhs.value.as< VkFormat > () = yystack_[0].value.as< VkFormat > (); }
#line 2154 "Parser.cpp" // lalr1.cc:859
    break;

  case 183:
#line 581 "Parser.yy" // lalr1.cc:859
    { yylhs.value.as< VkFormat > () = yystack_[0].value.as< VkFormat > (); }
#line 2160 "Parser.cpp" // lalr1.cc:859
    break;

  case 184:
#line 582 "Parser.yy" // lalr1.cc:859
    { yylhs.value.as< VkFormat > () = yystack_[0].value.as< VkFormat > (); }
#line 2166 "Parser.cpp" // lalr1.cc:859
    break;

  case 185:
#line 583 "Parser.yy" // lalr1.cc:859
    { yylhs.value.as< VkFormat > () = yystack_[0].value.as< VkFormat > (); }
#line 2172 "Parser.cpp" // lalr1.cc:859
    break;

  case 186:
#line 584 "Parser.yy" // lalr1.cc:859
    { yylhs.value.as< VkFormat > () = yystack_[0].value.as< VkFormat > (); }
#line 2178 "Parser.cpp" // lalr1.cc:859
    break;

  case 187:
#line 585 "Parser.yy" // lalr1.cc:859
    { yylhs.value.as< VkFormat > () = yystack_[0].value.as< VkFormat > (); }
#line 2184 "Parser.cpp" // lalr1.cc:859
    break;

  case 188:
#line 586 "Parser.yy" // lalr1.cc:859
    { yylhs.value.as< VkFormat > () = yystack_[0].value.as< VkFormat > (); }
#line 2190 "Parser.cpp" // lalr1.cc:859
    break;

  case 189:
#line 587 "Parser.yy" // lalr1.cc:859
    { yylhs.value.as< VkFormat > () = yystack_[0].value.as< VkFormat > (); }
#line 2196 "Parser.cpp" // lalr1.cc:859
    break;

  case 190:
#line 588 "Parser.yy" // lalr1.cc:859
    { yylhs.value.as< VkFormat > () = yystack_[0].value.as< VkFormat > (); }
#line 2202 "Parser.cpp" // lalr1.cc:859
    break;

  case 191:
#line 589 "Parser.yy" // lalr1.cc:859
    { yylhs.value.as< VkFormat > () = yystack_[0].value.as< VkFormat > (); }
#line 2208 "Parser.cpp" // lalr1.cc:859
    break;

  case 192:
#line 590 "Parser.yy" // lalr1.cc:859
    { yylhs.value.as< VkFormat > () = yystack_[0].value.as< VkFormat > (); }
#line 2214 "Parser.cpp" // lalr1.cc:859
    break;

  case 193:
#line 591 "Parser.yy" // lalr1.cc:859
    { yylhs.value.as< VkFormat > () = yystack_[0].value.as< VkFormat > (); }
#line 2220 "Parser.cpp" // lalr1.cc:859
    break;

  case 194:
#line 592 "Parser.yy" // lalr1.cc:859
    { yylhs.value.as< VkFormat > () = yystack_[0].value.as< VkFormat > (); }
#line 2226 "Parser.cpp" // lalr1.cc:859
    break;

  case 195:
#line 593 "Parser.yy" // lalr1.cc:859
    { yylhs.value.as< VkFormat > () = yystack_[0].value.as< VkFormat > (); }
#line 2232 "Parser.cpp" // lalr1.cc:859
    break;

  case 196:
#line 594 "Parser.yy" // lalr1.cc:859
    { yylhs.value.as< VkFormat > () = yystack_[0].value.as< VkFormat > (); }
#line 2238 "Parser.cpp" // lalr1.cc:859
    break;

  case 197:
#line 595 "Parser.yy" // lalr1.cc:859
    { yylhs.value.as< VkFormat > () = yystack_[0].value.as< VkFormat > (); }
#line 2244 "Parser.cpp" // lalr1.cc:859
    break;

  case 198:
#line 596 "Parser.yy" // lalr1.cc:859
    { yylhs.value.as< VkFormat > () = yystack_[0].value.as< VkFormat > (); }
#line 2250 "Parser.cpp" // lalr1.cc:859
    break;

  case 199:
#line 597 "Parser.yy" // lalr1.cc:859
    { yylhs.value.as< VkFormat > () = yystack_[0].value.as< VkFormat > (); }
#line 2256 "Parser.cpp" // lalr1.cc:859
    break;

  case 200:
#line 598 "Parser.yy" // lalr1.cc:859
    { yylhs.value.as< VkFormat > () = yystack_[0].value.as< VkFormat > (); }
#line 2262 "Parser.cpp" // lalr1.cc:859
    break;

  case 201:
#line 599 "Parser.yy" // lalr1.cc:859
    { yylhs.value.as< VkFormat > () = yystack_[0].value.as< VkFormat > (); }
#line 2268 "Parser.cpp" // lalr1.cc:859
    break;

  case 202:
#line 600 "Parser.yy" // lalr1.cc:859
    { yylhs.value.as< VkFormat > () = yystack_[0].value.as< VkFormat > (); }
#line 2274 "Parser.cpp" // lalr1.cc:859
    break;

  case 203:
#line 601 "Parser.yy" // lalr1.cc:859
    { yylhs.value.as< VkFormat > () = yystack_[0].value.as< VkFormat > (); }
#line 2280 "Parser.cpp" // lalr1.cc:859
    break;

  case 204:
#line 602 "Parser.yy" // lalr1.cc:859
    { yylhs.value.as< VkFormat > () = yystack_[0].value.as< VkFormat > (); }
#line 2286 "Parser.cpp" // lalr1.cc:859
    break;

  case 205:
#line 603 "Parser.yy" // lalr1.cc:859
    { yylhs.value.as< VkFormat > () = yystack_[0].value.as< VkFormat > (); }
#line 2292 "Parser.cpp" // lalr1.cc:859
    break;

  case 206:
#line 604 "Parser.yy" // lalr1.cc:859
    { yylhs.value.as< VkFormat > () = yystack_[0].value.as< VkFormat > (); }
#line 2298 "Parser.cpp" // lalr1.cc:859
    break;


#line 2302 "Parser.cpp" // lalr1.cc:859
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


  const signed char parser::yypact_ninf_ = -61;

  const signed char parser::yytable_ninf_ = -1;

  const signed char
  parser::yypact_[] =
  {
     -11,   -61,     0,   -61,   -12,   -10,   -61,   -61,   -61,   -61,
     -61,   -61,   -61,   -61,   -61,   -61,   -61,   -61,   -61,    14,
     -61,   -61,   -61,   -61,   -61,   -61,   -61,   -61,   -61,   -61,
     -61,   -61,   -61,   -61,   -61,    -7,    -5,   -61,   -26,    -6,
     -61,     4,   -60,   -61,   -61,   -61,   -61,   -61,   -61,   -61,
     -61,   -61,   -61,   -61,   -61,   -61,   -61,   -61,   -61,   -61,
     -61,   -61,   -61,   -61,   -61,   -61,   -61,   -61,   -61,   -61,
     -61,   -61,   -61,   -61,   -61,   -61,   -61,   -61,   -61,   -61,
     -61,   -61,   -61,   -61,   -61,   -61,   -61,   -61,   -61,   -61,
     -61,   -61,   -61,   -61,   -61,   -61,   -61,   -61,   -61,   -61,
     -61,   -61,   -61,   -61,   -61,   -61,   -61,   -61,   -61,   -61,
     -61,   -61,   -61,   -61,   -61,   -61,   -61,   -61,   -61,   -61,
     -61,   -61,   -61,   -61,   -61,   -61,   -61,   -61,   -61,   -61,
     -61,   -61,   -61,   -61,   -61,   -61,   -61,   -61,   -61,   -61,
     -61,   -61,   -61,   -61,   -61,   -61,   -61,   -61,   -61,   -61,
     -61,   -61,   -61,   -61,   -61,   -61,   -61,   -61,   -61,   -61,
     -61,   -61,   -61,   -61,   -61,   -61,   -61,   -61,   -61,   -61,
     -61,   -61,   -61,   -61,   -61,   -61,   -61,   -61,   -61,   -61,
     -61,   -61,   -61,   -61,   -61,   -61,   -61,   -61,   -61,   -61,
     -61,   -61,   -61,   -61,   -61,   -61,   -61,   -61,   -61,   -61,
     -61,   -61,   -61,   -61,   -61,   -61,   -61,   -61,   -61,   -61,
     -61,   -61,   -61,   -61,   -61,   -61,   -61,   -61,   -61,   -61,
     -61,   -61,   -61,   -61,   -61,   -61,   -61,   -61,    -3,    -1,
     -61
  };

  const unsigned char
  parser::yydefact_[] =
  {
       2,     3,     2,     1,     0,     0,    19,   207,   208,   209,
     210,   211,   212,   213,   214,   215,   216,   217,   218,     0,
       4,     5,    18,     6,     7,     8,     9,    10,    11,    12,
      13,    14,    15,    16,    17,     0,     0,   219,     0,     0,
      20,     0,     0,    22,    23,    24,    25,    26,    27,    28,
      29,    30,    31,    32,    33,    34,    35,    36,    37,    38,
      39,    40,    41,    42,    43,    44,    45,    46,    47,    48,
      49,    50,    51,    52,    53,    54,    55,    56,    57,    58,
      59,    60,    61,    62,    63,    64,    65,    66,    67,    68,
      69,    70,    71,    72,    73,    74,    75,    76,    77,    78,
      79,    80,    81,    82,    83,    84,    85,    86,    87,    88,
      89,    90,    91,    92,    93,    94,    95,    96,    97,    98,
      99,   100,   101,   102,   103,   104,   105,   106,   107,   108,
     109,   110,   111,   112,   113,   114,   115,   116,   117,   118,
     119,   120,   121,   122,   123,   124,   125,   126,   127,   128,
     129,   130,   131,   132,   133,   134,   135,   136,   137,   138,
     139,   140,   141,   142,   143,   144,   145,   146,   147,   148,
     149,   150,   151,   152,   153,   154,   155,   156,   157,   158,
     159,   160,   161,   162,   163,   164,   165,   166,   167,   168,
     169,   170,   171,   172,   173,   174,   175,   176,   177,   178,
     179,   180,   181,   182,   183,   184,   185,   186,   187,   188,
     189,   190,   191,   192,   193,   194,   195,   196,   197,   198,
     199,   200,   201,   202,   203,   204,   205,   206,     0,     0,
      21
  };

  const signed char
  parser::yypgoto_[] =
  {
     -61,   -61,    11,   -61,   -61,   -61,   -61,   -61,   -61,   -61,
     -61,   -61,   -61,   -61,   -61,   -61,   -61,   -61
  };

  const short int
  parser::yydefgoto_[] =
  {
      -1,   228,    19,    20,    21,    22,    23,    24,    25,    26,
      27,    28,    29,    30,    31,    32,    33,    34
  };

  const unsigned char
  parser::yytable_[] =
  {
       3,     1,    35,    38,    36,    39,    40,    42,    41,   229,
     230,     2,     1,     0,     0,    37,     0,     0,     0,     0,
       0,     4,     0,     0,     0,     0,     1,     0,     0,     0,
       0,     0,     0,     0,     0,     4,    43,    44,    45,    46,
      47,    48,    49,    50,    51,    52,    53,    54,    55,    56,
      57,    58,    59,    60,    61,    62,    63,    64,    65,    66,
      67,    68,    69,    70,    71,    72,    73,    74,    75,    76,
      77,    78,    79,    80,    81,    82,    83,    84,    85,    86,
      87,    88,    89,    90,    91,    92,    93,    94,    95,    96,
      97,    98,    99,   100,   101,   102,   103,   104,   105,   106,
     107,   108,   109,   110,   111,   112,   113,   114,   115,   116,
     117,   118,   119,   120,   121,   122,   123,   124,   125,   126,
     127,   128,   129,   130,   131,   132,   133,   134,   135,   136,
     137,   138,   139,   140,   141,   142,   143,   144,   145,   146,
     147,   148,   149,   150,   151,   152,   153,   154,   155,   156,
     157,   158,   159,   160,   161,   162,   163,   164,   165,   166,
     167,   168,   169,   170,   171,   172,   173,   174,   175,   176,
     177,   178,   179,   180,   181,   182,   183,   184,   185,   186,
     187,   188,   189,   190,   191,   192,   193,   194,   195,   196,
     197,   198,   199,   200,   201,   202,   203,   204,   205,   206,
     207,   208,   209,   210,   211,   212,   213,   214,   215,   216,
     217,   218,   219,   220,   221,   222,   223,   224,   225,   226,
     227,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     5,     6,     7,     8,     9,    10,    11,    12,    13,
      14,    15,    16,    17,    18,     5,     6,     7,     8,     9,
      10,    11,    12,    13,    14,    15,    16,    17,    18
  };

  const short int
  parser::yycheck_[] =
  {
       0,    12,    14,    10,    14,    10,    32,     3,    14,    12,
      11,     0,    12,    -1,    -1,     1,    -1,    -1,    -1,    -1,
      -1,    21,    -1,    -1,    -1,    -1,    12,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    21,    96,    97,    98,    99,
     100,   101,   102,   103,   104,   105,   106,   107,   108,   109,
     110,   111,   112,   113,   114,   115,   116,   117,   118,   119,
     120,   121,   122,   123,   124,   125,   126,   127,   128,   129,
     130,   131,   132,   133,   134,   135,   136,   137,   138,   139,
     140,   141,   142,   143,   144,   145,   146,   147,   148,   149,
     150,   151,   152,   153,   154,   155,   156,   157,   158,   159,
     160,   161,   162,   163,   164,   165,   166,   167,   168,   169,
     170,   171,   172,   173,   174,   175,   176,   177,   178,   179,
     180,   181,   182,   183,   184,   185,   186,   187,   188,   189,
     190,   191,   192,   193,   194,   195,   196,   197,   198,   199,
     200,   201,   202,   203,   204,   205,   206,   207,   208,   209,
     210,   211,   212,   213,   214,   215,   216,   217,   218,   219,
     220,   221,   222,   223,   224,   225,   226,   227,   228,   229,
     230,   231,   232,   233,   234,   235,   236,   237,   238,   239,
     240,   241,   242,   243,   244,   245,   246,   247,   248,   249,
     250,   251,   252,   253,   254,   255,   256,   257,   258,   259,
     260,   261,   262,   263,   264,   265,   266,   267,   268,   269,
     270,   271,   272,   273,   274,   275,   276,   277,   278,   279,
     280,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   281,   282,   283,   284,   285,   286,   287,   288,   289,
     290,   291,   292,   293,   294,   281,   282,   283,   284,   285,
     286,   287,   288,   289,   290,   291,   292,   293,   294
  };

  const unsigned short int
  parser::yystos_[] =
  {
       0,    12,   297,     0,    21,   281,   282,   283,   284,   285,
     286,   287,   288,   289,   290,   291,   292,   293,   294,   297,
     298,   299,   300,   301,   302,   303,   304,   305,   306,   307,
     308,   309,   310,   311,   312,    14,    14,     1,    10,    10,
      32,    14,     3,    96,    97,    98,    99,   100,   101,   102,
     103,   104,   105,   106,   107,   108,   109,   110,   111,   112,
     113,   114,   115,   116,   117,   118,   119,   120,   121,   122,
     123,   124,   125,   126,   127,   128,   129,   130,   131,   132,
     133,   134,   135,   136,   137,   138,   139,   140,   141,   142,
     143,   144,   145,   146,   147,   148,   149,   150,   151,   152,
     153,   154,   155,   156,   157,   158,   159,   160,   161,   162,
     163,   164,   165,   166,   167,   168,   169,   170,   171,   172,
     173,   174,   175,   176,   177,   178,   179,   180,   181,   182,
     183,   184,   185,   186,   187,   188,   189,   190,   191,   192,
     193,   194,   195,   196,   197,   198,   199,   200,   201,   202,
     203,   204,   205,   206,   207,   208,   209,   210,   211,   212,
     213,   214,   215,   216,   217,   218,   219,   220,   221,   222,
     223,   224,   225,   226,   227,   228,   229,   230,   231,   232,
     233,   234,   235,   236,   237,   238,   239,   240,   241,   242,
     243,   244,   245,   246,   247,   248,   249,   250,   251,   252,
     253,   254,   255,   256,   257,   258,   259,   260,   261,   262,
     263,   264,   265,   266,   267,   268,   269,   270,   271,   272,
     273,   274,   275,   276,   277,   278,   279,   280,   296,    12,
      11
  };

  const unsigned short int
  parser::yyr1_[] =
  {
       0,   295,   297,   297,   297,   297,   297,   297,   297,   297,
     297,   297,   297,   297,   297,   297,   297,   297,   297,   298,
     299,   300,   296,   296,   296,   296,   296,   296,   296,   296,
     296,   296,   296,   296,   296,   296,   296,   296,   296,   296,
     296,   296,   296,   296,   296,   296,   296,   296,   296,   296,
     296,   296,   296,   296,   296,   296,   296,   296,   296,   296,
     296,   296,   296,   296,   296,   296,   296,   296,   296,   296,
     296,   296,   296,   296,   296,   296,   296,   296,   296,   296,
     296,   296,   296,   296,   296,   296,   296,   296,   296,   296,
     296,   296,   296,   296,   296,   296,   296,   296,   296,   296,
     296,   296,   296,   296,   296,   296,   296,   296,   296,   296,
     296,   296,   296,   296,   296,   296,   296,   296,   296,   296,
     296,   296,   296,   296,   296,   296,   296,   296,   296,   296,
     296,   296,   296,   296,   296,   296,   296,   296,   296,   296,
     296,   296,   296,   296,   296,   296,   296,   296,   296,   296,
     296,   296,   296,   296,   296,   296,   296,   296,   296,   296,
     296,   296,   296,   296,   296,   296,   296,   296,   296,   296,
     296,   296,   296,   296,   296,   296,   296,   296,   296,   296,
     296,   296,   296,   296,   296,   296,   296,   296,   296,   296,
     296,   296,   296,   296,   296,   296,   296,   296,   296,   296,
     296,   296,   296,   296,   296,   296,   296,   301,   302,   303,
     304,   305,   306,   307,   308,   309,   310,   311,   312,   312
  };

  const unsigned char
  parser::yyr2_[] =
  {
       0,     2,     0,     1,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     1,
       4,     8,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     2
  };



  // YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
  // First, the terminals, then, starting at \a yyntokens_, nonterminals.
  const char*
  const parser::yytname_[] =
  {
  "\"end of file\"", "error", "$undefined", "\"=\"", "\"-\"", "\"+\"",
  "\"*\"", "\"/\"", "\"(\"", "\")\"", "\"{\"", "\"}\"", "\";\"", "\",\"",
  "IDENTIFIER", "TRUE", "FALSE", "FLOAT", "INT", "BOOL", "STR",
  "GLSLSHADER", "HLSL10SHADER", "HLSL11SHADER", "CGSHADER",
  "SAMPLER_STATE", "DST_STATE", "PR_STATE", "COLOR_SAMPLE_STATE",
  "RASTERIZATION_STATE", "TECHNIQUE", "PASS", "CODEBODY", "VARNAME",
  "TEXTURERESOURCE", "TEXTURERESOURCE1D", "TEXTURERESOURCE2D",
  "TEXTURERESOURCE3D", "TEXTURERESOURCERECT", "TEXTURERESOURCECUBE",
  "VOID_TYPE", "UNSIGNED", "HIGHP", "MEDIUMP", "LOWP", "UNIFORM",
  "CSTBUFFER", "FLOAT_TYPE", "FLOAT2_TYPE", "FLOAT3_TYPE", "FLOAT4_TYPE",
  "MAT2_TYPE", "MAT3_TYPE", "MAT4_TYPE", "BOOL_TYPE", "BOOL2_TYPE",
  "BOOL3_TYPE", "BOOL4_TYPE", "INT_TYPE", "INT2_TYPE", "INT3_TYPE",
  "INT4_TYPE", "TEXTURE1D_TYPE", "TEXTURE2D_TYPE", "TEXTURE2DSHADOW_TYPE",
  "TEXTURE2DRECT_TYPE", "TEXTURE3D_TYPE", "TEXTURECUBE_TYPE",
  "SAMPLER1D_TYPE", "SAMPLER2D_TYPE", "SAMPLER2DSHADOW_TYPE",
  "SAMPLER2DRECT_TYPE", "SAMPLER3D_TYPE", "SAMPLERCUBE_TYPE", "EXTENSION",
  "SEPARATE_SHADER", "VERTEXPROGRAM", "FRAGMENTPROGRAM", "GEOMETRYPROGRAM",
  "HULLPROGRAM", "EVALPROGRAM", "SHDPROFILE", "SAMPLERRESOURCE",
  "SAMPLERTEXUNIT", "SETSAMPLERSTATE", "SETDSTSTATE",
  "SETRASTERIZATIONSTATE", "SETCOLORSAMPLESTATE", "IMAGERESOURCE",
  "IMAGEUNIT", "IMAGEACCESS", "IMAGELAYER", "IMAGELAYERED", "WRITE_ONLY",
  "READ_ONLY", "READ_WRITE", "FORMAT_UNDEFINED", "FORMAT_R4G4_UNORM_PACK8",
  "FORMAT_R4G4B4A4_UNORM_PACK16", "FORMAT_B4G4R4A4_UNORM_PACK16",
  "FORMAT_R5G6B5_UNORM_PACK16", "FORMAT_B5G6R5_UNORM_PACK16",
  "FORMAT_R5G5B5A1_UNORM_PACK16", "FORMAT_B5G5R5A1_UNORM_PACK16",
  "FORMAT_A1R5G5B5_UNORM_PACK16", "FORMAT_R8_UNORM", "FORMAT_R8_SNORM",
  "FORMAT_R8_USCALED", "FORMAT_R8_SSCALED", "FORMAT_R8_UINT",
  "FORMAT_R8_SINT", "FORMAT_R8_SRGB", "FORMAT_R8G8_UNORM",
  "FORMAT_R8G8_SNORM", "FORMAT_R8G8_USCALED", "FORMAT_R8G8_SSCALED",
  "FORMAT_R8G8_UINT", "FORMAT_R8G8_SINT", "FORMAT_R8G8_SRGB",
  "FORMAT_R8G8B8_UNORM", "FORMAT_R8G8B8_SNORM", "FORMAT_R8G8B8_USCALED",
  "FORMAT_R8G8B8_SSCALED", "FORMAT_R8G8B8_UINT", "FORMAT_R8G8B8_SINT",
  "FORMAT_R8G8B8_SRGB", "FORMAT_B8G8R8_UNORM", "FORMAT_B8G8R8_SNORM",
  "FORMAT_B8G8R8_USCALED", "FORMAT_B8G8R8_SSCALED", "FORMAT_B8G8R8_UINT",
  "FORMAT_B8G8R8_SINT", "FORMAT_B8G8R8_SRGB", "FORMAT_R8G8B8A8_UNORM",
  "FORMAT_R8G8B8A8_SNORM", "FORMAT_R8G8B8A8_USCALED",
  "FORMAT_R8G8B8A8_SSCALED", "FORMAT_R8G8B8A8_UINT",
  "FORMAT_R8G8B8A8_SINT", "FORMAT_R8G8B8A8_SRGB", "FORMAT_B8G8R8A8_UNORM",
  "FORMAT_B8G8R8A8_SNORM", "FORMAT_B8G8R8A8_USCALED",
  "FORMAT_B8G8R8A8_SSCALED", "FORMAT_B8G8R8A8_UINT",
  "FORMAT_B8G8R8A8_SINT", "FORMAT_B8G8R8A8_SRGB",
  "FORMAT_A8B8G8R8_UNORM_PACK32", "FORMAT_A8B8G8R8_SNORM_PACK32",
  "FORMAT_A8B8G8R8_USCALED_PACK32", "FORMAT_A8B8G8R8_SSCALED_PACK32",
  "FORMAT_A8B8G8R8_UINT_PACK32", "FORMAT_A8B8G8R8_SINT_PACK32",
  "FORMAT_A8B8G8R8_SRGB_PACK32", "FORMAT_A2R10G10B10_UNORM_PACK32",
  "FORMAT_A2R10G10B10_SNORM_PACK32", "FORMAT_A2R10G10B10_USCALED_PACK32",
  "FORMAT_A2R10G10B10_SSCALED_PACK32", "FORMAT_A2R10G10B10_UINT_PACK32",
  "FORMAT_A2R10G10B10_SINT_PACK32", "FORMAT_A2B10G10R10_UNORM_PACK32",
  "FORMAT_A2B10G10R10_SNORM_PACK32", "FORMAT_A2B10G10R10_USCALED_PACK32",
  "FORMAT_A2B10G10R10_SSCALED_PACK32", "FORMAT_A2B10G10R10_UINT_PACK32",
  "FORMAT_A2B10G10R10_SINT_PACK32", "FORMAT_R16_UNORM", "FORMAT_R16_SNORM",
  "FORMAT_R16_USCALED", "FORMAT_R16_SSCALED", "FORMAT_R16_UINT",
  "FORMAT_R16_SINT", "FORMAT_R16_SFLOAT", "FORMAT_R16G16_UNORM",
  "FORMAT_R16G16_SNORM", "FORMAT_R16G16_USCALED", "FORMAT_R16G16_SSCALED",
  "FORMAT_R16G16_UINT", "FORMAT_R16G16_SINT", "FORMAT_R16G16_SFLOAT",
  "FORMAT_R16G16B16_UNORM", "FORMAT_R16G16B16_SNORM",
  "FORMAT_R16G16B16_USCALED", "FORMAT_R16G16B16_SSCALED",
  "FORMAT_R16G16B16_UINT", "FORMAT_R16G16B16_SINT",
  "FORMAT_R16G16B16_SFLOAT", "FORMAT_R16G16B16A16_UNORM",
  "FORMAT_R16G16B16A16_SNORM", "FORMAT_R16G16B16A16_USCALED",
  "FORMAT_R16G16B16A16_SSCALED", "FORMAT_R16G16B16A16_UINT",
  "FORMAT_R16G16B16A16_SINT", "FORMAT_R16G16B16A16_SFLOAT",
  "FORMAT_R32_UINT", "FORMAT_R32_SINT", "FORMAT_R32_SFLOAT",
  "FORMAT_R32G32_UINT", "FORMAT_R32G32_SINT", "FORMAT_R32G32_SFLOAT",
  "FORMAT_R32G32B32_UINT", "FORMAT_R32G32B32_SINT",
  "FORMAT_R32G32B32_SFLOAT", "FORMAT_R32G32B32A32_UINT",
  "FORMAT_R32G32B32A32_SINT", "FORMAT_R32G32B32A32_SFLOAT",
  "FORMAT_R64_UINT", "FORMAT_R64_SINT", "FORMAT_R64_SFLOAT",
  "FORMAT_R64G64_UINT", "FORMAT_R64G64_SINT", "FORMAT_R64G64_SFLOAT",
  "FORMAT_R64G64B64_UINT", "FORMAT_R64G64B64_SINT",
  "FORMAT_R64G64B64_SFLOAT", "FORMAT_R64G64B64A64_UINT",
  "FORMAT_R64G64B64A64_SINT", "FORMAT_R64G64B64A64_SFLOAT",
  "FORMAT_B10G11R11_UFLOAT_PACK32", "FORMAT_E5B9G9R9_UFLOAT_PACK32",
  "FORMAT_D16_UNORM", "FORMAT_X8_D24_UNORM_PACK32", "FORMAT_D32_SFLOAT",
  "FORMAT_S8_UINT", "FORMAT_D16_UNORM_S8_UINT", "FORMAT_D24_UNORM_S8_UINT",
  "FORMAT_D32_SFLOAT_S8_UINT", "FORMAT_BC1_RGB_UNORM_BLOCK",
  "FORMAT_BC1_RGB_SRGB_BLOCK", "FORMAT_BC1_RGBA_UNORM_BLOCK",
  "FORMAT_BC1_RGBA_SRGB_BLOCK", "FORMAT_BC2_UNORM_BLOCK",
  "FORMAT_BC2_SRGB_BLOCK", "FORMAT_BC3_UNORM_BLOCK",
  "FORMAT_BC3_SRGB_BLOCK", "FORMAT_BC4_UNORM_BLOCK",
  "FORMAT_BC4_SNORM_BLOCK", "FORMAT_BC5_UNORM_BLOCK",
  "FORMAT_BC5_SNORM_BLOCK", "FORMAT_BC6H_UFLOAT_BLOCK",
  "FORMAT_BC6H_SFLOAT_BLOCK", "FORMAT_BC7_UNORM_BLOCK",
  "FORMAT_BC7_SRGB_BLOCK", "FORMAT_ETC2_R8G8B8_UNORM_BLOCK",
  "FORMAT_ETC2_R8G8B8_SRGB_BLOCK", "FORMAT_ETC2_R8G8B8A1_UNORM_BLOCK",
  "FORMAT_ETC2_R8G8B8A1_SRGB_BLOCK", "FORMAT_ETC2_R8G8B8A8_UNORM_BLOCK",
  "FORMAT_ETC2_R8G8B8A8_SRGB_BLOCK", "FORMAT_EAC_R11_UNORM_BLOCK",
  "FORMAT_EAC_R11_SNORM_BLOCK", "FORMAT_EAC_R11G11_UNORM_BLOCK",
  "FORMAT_EAC_R11G11_SNORM_BLOCK", "FORMAT_ASTC_4x4_UNORM_BLOCK",
  "FORMAT_ASTC_4x4_SRGB_BLOCK", "FORMAT_ASTC_5x4_UNORM_BLOCK",
  "FORMAT_ASTC_5x4_SRGB_BLOCK", "FORMAT_ASTC_5x5_UNORM_BLOCK",
  "FORMAT_ASTC_5x5_SRGB_BLOCK", "FORMAT_ASTC_6x5_UNORM_BLOCK",
  "FORMAT_ASTC_6x5_SRGB_BLOCK", "FORMAT_ASTC_6x6_UNORM_BLOCK",
  "FORMAT_ASTC_6x6_SRGB_BLOCK", "FORMAT_ASTC_8x5_UNORM_BLOCK",
  "FORMAT_ASTC_8x5_SRGB_BLOCK", "FORMAT_ASTC_8x6_UNORM_BLOCK",
  "FORMAT_ASTC_8x6_SRGB_BLOCK", "FORMAT_ASTC_8x8_UNORM_BLOCK",
  "FORMAT_ASTC_8x8_SRGB_BLOCK", "FORMAT_ASTC_10x5_UNORM_BLOCK",
  "FORMAT_ASTC_10x5_SRGB_BLOCK", "FORMAT_ASTC_10x6_UNORM_BLOCK",
  "FORMAT_ASTC_10x6_SRGB_BLOCK", "FORMAT_ASTC_10x8_UNORM_BLOCK",
  "FORMAT_ASTC_10x8_SRGB_BLOCK", "FORMAT_ASTC_10x10_UNORM_BLOCK",
  "FORMAT_ASTC_10x10_SRGB_BLOCK", "FORMAT_ASTC_12x10_UNORM_BLOCK",
  "FORMAT_ASTC_12x10_SRGB_BLOCK", "FORMAT_ASTC_12x12_UNORM_BLOCK",
  "FORMAT_ASTC_12x12_SRGB_BLOCK", "VERTEXFORMAT", "\"tech\"", "\"hlsl\"",
  "\"texture\"", "\"resource\"", "\"fbo\"", "\"sstate\"", "\"dststate\"",
  "\"prstate\"", "\"csstate\"", "\"rstate\"", "\"namespace\"",
  "\"cstbuffer\"", "\"uniforms\"", "$accept", "format", "input", "tech",
  "glsl", "vertexformat", "hlsl", "texture", "resource", "fbo", "sstate",
  "dststate", "prstate", "csstate", "rstate", "namespace", "cstbuffer",
  "uniforms", YY_NULLPTR
  };

#if YYDEBUG
  const unsigned short int
  parser::yyrline_[] =
  {
       0,   382,   382,   383,   384,   385,   386,   387,   388,   389,
     390,   391,   392,   393,   394,   395,   396,   397,   398,   403,
     405,   411,   420,   421,   422,   423,   424,   425,   426,   427,
     428,   429,   430,   431,   432,   433,   434,   435,   436,   437,
     438,   439,   440,   441,   442,   443,   444,   445,   446,   447,
     448,   449,   450,   451,   452,   453,   454,   455,   456,   457,
     458,   459,   460,   461,   462,   463,   464,   465,   466,   467,
     468,   469,   470,   471,   472,   473,   474,   475,   476,   477,
     478,   479,   480,   481,   482,   483,   484,   485,   486,   487,
     488,   489,   490,   491,   492,   493,   494,   495,   496,   497,
     498,   499,   500,   501,   502,   503,   504,   505,   506,   507,
     508,   509,   510,   511,   512,   513,   514,   515,   516,   517,
     518,   519,   520,   521,   522,   523,   524,   525,   526,   527,
     528,   529,   530,   531,   532,   533,   534,   535,   536,   537,
     538,   539,   540,   541,   542,   543,   544,   545,   546,   547,
     548,   549,   550,   551,   552,   553,   554,   555,   556,   557,
     558,   559,   560,   561,   562,   563,   564,   565,   566,   567,
     568,   569,   570,   571,   572,   573,   574,   575,   576,   577,
     578,   579,   580,   581,   582,   583,   584,   585,   586,   587,
     588,   589,   590,   591,   592,   593,   594,   595,   596,   597,
     598,   599,   600,   601,   602,   603,   604,   607,   608,   609,
     610,   611,   612,   613,   614,   615,   616,   617,   618,   619
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
#line 2966 "Parser.cpp" // lalr1.cc:1167
#line 622 "Parser.yy" // lalr1.cc:1168


#include <sstream>
void
yy::parser::error(const location_type& l, const std::string& m)
{
  std::stringstream ss;
  ss << l << ": " << m;
  gEnv->pLog->LogError(ss.str().c_str());    
}
#pragma warning(pop)
