
/* A Bison parser, made by GNU Bison 2.4.1.  */

/* Skeleton implementation for Bison's Yacc-like parsers in C

      Copyright (C) 1984, 1989, 1990, 2000, 2001, 2002, 2003, 2004, 2005, 2006
   Free Software Foundation, Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "2.4.1"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1

/* Using locations.  */
#define YYLSP_NEEDED 0

/* Substitute the variable and function names.  */
#define yyparse         idparse
#define yylex           idlex
#define yyerror         iderror
#define yylval          idlval
#define yychar          idchar
#define yydebug         iddebug
#define yynerrs         idnerrs


/* Copy the first part of user declarations.  */

/* Line 189 of yacc.c  */
#line 3 "asn_ref_grammar.y"


/*
 * asn_ref_grammer.y
 *
 * ASN grammar file for typereference, objectclassreference, valuereference, ...
 *
 * Copyright (c) 2001 Institute for Information Industry
 * (http://www.iii.org.tw/iiia/ewelcome.htm)
 *
 * The contents of this file are subject to the Mozilla Public License
 * Version 1.0 (the "License"); you may not use this file except in
 * compliance with the License. You may obtain a copy of the License at
 * http://www.mozilla.org/MPL/
 *
 * Software distributed under the License is distributed on an "AS IS"
 * basis, WITHOUT WARRANTY OF ANY KIND, either express or implied. See
 * the License for the specific language governing rights and limitations
 * under the License.
 *
 * The Original Code is ASN Parser.
 *
 * The Initial Developer of the Original Code is Institute for Information Industry
 *
 * Contributor(s): ___________________.
 *
 * $Log: asn_ref_grammar.y,v $
 * Revision 1.3  2002/07/02 02:03:25  mangelo
 * Remove Pwlib dependency
 *
 * Revision 1.2  2001/09/07 22:38:28  mangelo
 * add Log keyword substitution
 *
 *
 */

#undef malloc
#undef calloc
#undef realloc
#undef free

#include "main.h"

extern int idlex();

typedef std::vector<std::string> StringList;

#ifdef _MSC_VER
#pragma warning(disable:4701 4065 4244 4702)
#define MSDOS
#endif

#define VALUEREFERENCE  (TYPEREFERENCE+2)
#define OBJECTREFERENCE (TYPEREFERENCE+3)
#define OBJECTSETREFERENCE (TYPEREFERENCE+4)

#define PARAMETERIZEDTYPEREFERENCE (TYPEREFERENCE+5)
#define PARAMETERIZEDOBJECTCLASSREFERENCE (TYPEREFERENCE+6)
#define PARAMETERIZEDVALUEREFERENCE  (TYPEREFERENCE+7)
#define PARAMETERIZEDOBJECTREFERENCE (TYPEREFERENCE+8)
#define PARAMETERIZEDOBJECTSETREFERENCE (TYPEREFERENCE+9)

#define VALUESET_BRACE (TYPEREFERENCE+10)
#define OBJECT_BRACE (TYPEREFERENCE+11)
#define OBJECTSET_BRACE (TYPEREFERENCE+12)



/* Line 189 of yacc.c  */
#line 150 "asn_ref_grammar.tab.c"

/* Enabling traces.  */
#ifndef YYDEBUG
# define YYDEBUG 1
#endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

/* Enabling the token table.  */
#ifndef YYTOKEN_TABLE
# define YYTOKEN_TABLE 0
#endif


/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
/* Put the tokens into the symbol table, so that GDB and other debuggers
   know about them.  */
enum yytokentype {
    MODULEREFERENCE = 258,
    TYPEREFERENCE = 259,
    OBJECTCLASSREFERENCE = 260,
    IDENTIFIER = 261,
    WOULDBE_OBJECTCLASSREFERENCE = 262,
    FIELDREFERENCE = 263,
    INTEGER = 264,
    CSTRING = 265,
    BSTRING = 266,
    HSTRING = 267,
    ABSENT = 268,
    ABSTRACT_SYNTAX = 269,
    ALL = 270,
    ANY = 271,
    APPLICATION = 272,
    ASSIGNMENT = 273,
    AUTOMATIC = 274,
    BEGIN_t = 275,
    BIT = 276,
    BMPString = 277,
    BOOLEAN_t = 278,
    BY = 279,
    CHARACTER = 280,
    CHOICE = 281,
    CLASS = 282,
    COMPONENT = 283,
    COMPONENTS = 284,
    CONSTRAINED = 285,
    DEFAULT = 286,
    DEFINED = 287,
    DEFINITIONS = 288,
    EMBEDDED = 289,
    END = 290,
    ENUMERATED = 291,
    EXCEPT = 292,
    EXPLICIT = 293,
    EXPORTS = 294,
    EXTERNAL = 295,
    FALSE_t = 296,
    FROM = 297,
    GeneralString = 298,
    GraphicString = 299,
    IA5String = 300,
    TYPE_IDENTIFIER = 301,
    IDENTIFIER_t = 302,
    IMPLICIT = 303,
    IMPORTS = 304,
    INCLUDES = 305,
    INSTANCE = 306,
    INTEGER_t = 307,
    INTERSECTION = 308,
    ISO646String = 309,
    MACRO = 310,
    MAX = 311,
    MIN = 312,
    MINUS_INFINITY = 313,
    NOTATION = 314,
    NULL_t = 315,
    NumericString = 316,
    OBJECT = 317,
    OCTET = 318,
    OF_t = 319,
    OPTIONAL_t = 320,
    PDV = 321,
    PLUS_INFINITY = 322,
    PRESENT = 323,
    PrintableString = 324,
    PRIVATE = 325,
    REAL = 326,
    SEQUENCE = 327,
    SET = 328,
    SIZE_t = 329,
    STRING = 330,
    SYNTAX = 331,
    T61String = 332,
    TAGS = 333,
    TeletexString = 334,
    TRUE_t = 335,
    TYPE_t = 336,
    UNION = 337,
    UNIQUE = 338,
    UNIVERSAL = 339,
    UniversalString = 340,
    VideotexString = 341,
    VisibleString = 342,
    GeneralizedTime = 343,
    UTCTime = 344,
    VALUE = 345,
    WITH = 346,
    ObjectDescriptor_t = 347
};
#endif



#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE {

    /* Line 214 of yacc.c  */
#line 187 "asn_ref_grammar.y"

    boost::int64_t              ival;
    std::string*                sval;
    StringList*                 slst;
    struct {
        Tag::Type tagClass;
        unsigned tagNumber;
    } tagv;



    /* Line 214 of yacc.c  */
#line 290 "asn_ref_grammar.tab.c"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif


/* Copy the second part of user declarations.  */


/* Line 264 of yacc.c  */
#line 302 "asn_ref_grammar.tab.c"

#ifdef short
# undef short
#endif

#ifdef YYTYPE_UINT8
typedef YYTYPE_UINT8 yytype_uint8;
#else
typedef unsigned char yytype_uint8;
#endif

#ifdef YYTYPE_INT8
typedef YYTYPE_INT8 yytype_int8;
#elif (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
typedef signed char yytype_int8;
#else
typedef short int yytype_int8;
#endif

#ifdef YYTYPE_UINT16
typedef YYTYPE_UINT16 yytype_uint16;
#else
typedef unsigned short int yytype_uint16;
#endif

#ifdef YYTYPE_INT16
typedef YYTYPE_INT16 yytype_int16;
#else
typedef short int yytype_int16;
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif ! defined YYSIZE_T && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned int
# endif
#endif

#define YYSIZE_MAXIMUM ((YYSIZE_T) -1)

#ifndef YY_
# if YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(msgid) dgettext ("bison-runtime", msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(msgid) msgid
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(e) ((void) (e))
#else
# define YYUSE(e) /* empty */
#endif

/* Identity function, used to suppress warnings about constant conditions.  */
#ifndef lint
# define YYID(n) (n)
#else
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static int
YYID(int yyi)
#else
static int
YYID(yyi)
int yyi;
#endif
{
    return yyi;
}
#endif

#if ! defined yyoverflow || YYERROR_VERBOSE

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#     ifndef _STDLIB_H
#      define _STDLIB_H 1
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
/* Pacify GCC's `empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (YYID (0))
#  ifndef YYSTACK_ALLOC_MAXIMUM
/* The OS might guarantee only one guard page at the bottom of the stack,
   and a page size can be as small as 4096 bytes.  So we cannot safely
   invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
   to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined _STDLIB_H \
       && ! ((defined YYMALLOC || defined malloc) \
         && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef _STDLIB_H
#    define _STDLIB_H 1
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void* malloc(YYSIZE_T);  /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void free(void*);   /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* ! defined yyoverflow || YYERROR_VERBOSE */


#if (! defined yyoverflow \
     && (! defined __cplusplus \
     || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc {
    yytype_int16 yyss_alloc;
    YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
    ((N) * (sizeof (yytype_int16) + sizeof (YYSTYPE)) \
     + YYSTACK_GAP_MAXIMUM)

/* Copy COUNT objects from FROM to TO.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(To, From, Count) \
    __builtin_memcpy (To, From, (Count) * sizeof (*(From)))
#  else
#   define YYCOPY(To, From, Count)      \
    do                    \
    {                   \
        YYSIZE_T yyi;             \
        for (yyi = 0; yyi < (Count); yyi++)   \
            (To)[yyi] = (From)[yyi];        \
    }                   \
    while (YYID (0))
#  endif
# endif

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)               \
    do                                  \
    {                                 \
        YYSIZE_T yynewbytes;                        \
        YYCOPY (&yyptr->Stack_alloc, Stack, yysize);            \
        Stack = &yyptr->Stack_alloc;                    \
        yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
        yyptr += yynewbytes / sizeof (*yyptr);              \
    }                                 \
    while (YYID (0))

#endif

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  6
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   1065

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  105
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  109
/* YYNRULES -- Number of rules.  */
#define YYNRULES  223
/* YYNRULES -- Number of states.  */
#define YYNSTATES  350

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   347

#define YYTRANSLATE(YYX)                        \
    ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const yytype_uint8 yytranslate[] = {
    0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
    2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
    2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
    2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
    101,   102,     2,     2,    96,     2,    97,     2,   104,     2,
    2,     2,     2,     2,     2,     2,     2,     2,   103,    95,
    100,     2,     2,     2,     2,     2,     2,     2,     2,     2,
    2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
    2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
    2,    98,     2,    99,     2,     2,     2,     2,     2,     2,
    2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
    2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
    2,     2,     2,    93,     2,    94,     2,     2,     2,     2,
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
    85,    86,    87,    88,    89,    90,    91,    92
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const yytype_uint16 yyprhs[] = {
    0,     0,     3,     6,     8,     9,    20,    21,    30,    33,
    36,    39,    40,    44,    45,    49,    50,    52,    53,    57,
    58,    60,    61,    63,    66,    70,    73,    75,    77,    78,
    80,    84,    86,    88,    92,    94,    97,    99,   101,   103,
    105,   107,   109,   111,   117,   123,   127,   131,   133,   135,
    137,   139,   141,   143,   145,   147,   149,   151,   153,   155,
    157,   159,   161,   163,   165,   167,   169,   171,   173,   175,
    177,   179,   181,   183,   185,   187,   189,   191,   195,   199,
    201,   203,   206,   211,   213,   215,   217,   219,   221,   223,
    225,   227,   229,   231,   233,   235,   237,   239,   241,   244,
    248,   251,   255,   257,   259,   264,   268,   270,   274,   276,
    280,   283,   286,   288,   292,   296,   300,   304,   307,   311,
    315,   318,   322,   324,   326,   328,   332,   334,   338,   340,
    342,   345,   347,   352,   357,   362,   367,   370,   373,   377,
    382,   387,   392,   394,   396,   398,   400,   402,   404,   408,
    410,   412,   417,   422,   423,   425,   427,   431,   435,   439,
    441,   443,   446,   448,   451,   453,   455,   457,   459,   461,
    463,   467,   471,   476,   482,   488,   492,   497,   502,   507,
    510,   513,   516,   519,   524,   526,   528,   530,   532,   534,
    536,   538,   540,   542,   544,   546,   548,   550,   552,   556,
    559,   561,   563,   565,   567,   569,   571,   575,   577,   579,
    581,   583,   585,   587,   589,   593,   595,   597,   601,   603,
    605,   607,   609,   611
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int16 yyrhs[] = {
    106,     0,    -1,   106,   107,    -1,   107,    -1,    -1,   212,
    93,    94,    33,   110,    18,    20,   108,   111,    35,    -1,
    -1,   212,    33,   110,    18,    20,   109,   111,    35,    -1,
    38,    78,    -1,    48,    78,    -1,    19,    78,    -1,    -1,
    112,   114,   123,    -1,    -1,    39,   113,    95,    -1,    -1,
    120,    -1,    -1,    49,   115,    95,    -1,    -1,   116,    -1,
    -1,   117,    -1,   116,   117,    -1,   120,    42,   118,    -1,
    212,   119,    -1,   195,    -1,   197,    -1,    -1,   121,    -1,
    121,    96,   120,    -1,   213,    -1,   122,    -1,   213,    93,
    94,    -1,   124,    -1,   123,   124,    -1,   126,    -1,   191,
    -1,   125,    -1,   165,    -1,   166,    -1,   167,    -1,   180,
    -1,     4,   127,    18,    93,    94,    -1,     7,   127,    18,
    93,    94,    -1,     4,    18,   127,    -1,     7,    18,   127,
    -1,   161,    -1,   129,    -1,   128,    -1,   134,    -1,   135,
    -1,   136,    -1,   139,    -1,   140,    -1,   141,    -1,   142,
    -1,   143,    -1,   144,    -1,   145,    -1,   146,    -1,   147,
    -1,   148,    -1,   149,    -1,   150,    -1,   151,    -1,   152,
    -1,   153,    -1,   154,    -1,   155,    -1,   130,    -1,   158,
    -1,   157,    -1,   159,    -1,   131,    -1,   212,    -1,   132,
    -1,     3,    97,   212,    -1,   133,    93,    94,    -1,   131,
    -1,   212,    -1,    21,    75,    -1,    21,    75,    93,    94,
    -1,    23,    -1,   137,    -1,   138,    -1,    22,    -1,    43,
    -1,    44,    -1,    45,    -1,    54,    -1,    61,    -1,    69,
    -1,    79,    -1,    77,    -1,    85,    -1,    86,    -1,    87,
    -1,    25,    75,    -1,    26,    93,    94,    -1,    34,    66,
    -1,    36,    93,    94,    -1,    40,    -1,    16,    -1,    16,
    32,    24,     6,    -1,    51,    64,   169,    -1,    52,    -1,
    52,    93,    94,    -1,    60,    -1,   169,    97,   177,    -1,
    62,    47,    -1,    63,    75,    -1,    71,    -1,    72,    93,
    94,    -1,    72,    64,   127,    -1,    73,    93,    94,    -1,
    73,    64,   127,    -1,   156,   127,    -1,   156,    48,   127,
    -1,   156,    38,   127,    -1,    98,    99,    -1,     6,   100,
    127,    -1,    88,    -1,    89,    -1,    92,    -1,   160,    97,
    177,    -1,     6,    -1,     6,    93,    94,    -1,   212,    -1,
    132,    -1,   127,   163,    -1,   162,    -1,    73,   163,    64,
    127,    -1,    73,   164,    64,   127,    -1,    72,   163,    64,
    127,    -1,    72,   164,    64,   127,    -1,   101,   102,    -1,
    74,   163,    -1,     7,    18,   168,    -1,     6,   169,    18,
    178,    -1,     4,   169,    18,   179,    -1,     7,   169,    18,
    179,    -1,   169,    -1,   172,    -1,   176,    -1,   170,    -1,
    5,    -1,   171,    -1,     3,    97,     5,    -1,    46,    -1,
    14,    -1,    27,    93,    94,   173,    -1,    91,    76,    93,
    94,    -1,    -1,   175,    -1,     6,    -1,     3,    97,     6,
    -1,   169,    93,    94,    -1,   177,    97,     8,    -1,     8,
    -1,   174,    -1,    93,    94,    -1,   159,    -1,    93,    94,
    -1,   181,    -1,   182,    -1,   183,    -1,   184,    -1,   185,
    -1,   186,    -1,   188,    18,   127,    -1,   187,    18,   127,
    -1,   189,   127,    18,   192,    -1,   188,   127,    18,    93,
    94,    -1,   187,   127,    18,    93,    94,    -1,   187,    18,
    168,    -1,   189,   169,    18,   178,    -1,   188,   169,    18,
    179,    -1,   187,   169,    18,   179,    -1,     7,   190,    -1,
    4,   190,    -1,     6,   190,    -1,    93,    94,    -1,     6,
    127,    18,   192,    -1,   193,    -1,   207,    -1,   194,    -1,
    209,    -1,   198,    -1,   199,    -1,   200,    -1,   202,    -1,
    203,    -1,   197,    -1,   204,    -1,   196,    -1,     6,    -1,
    210,    -1,     3,    97,     6,    -1,    93,    94,    -1,    11,
    -1,    12,    -1,    80,    -1,    41,    -1,   201,    -1,    10,
    -1,     6,   103,   192,    -1,    60,    -1,   205,    -1,   206,
    -1,   104,    -1,    67,    -1,    58,    -1,   208,    -1,   127,
    103,   192,    -1,   195,    -1,   159,    -1,   211,    93,    94,
    -1,   196,    -1,     6,    -1,     4,    -1,     7,    -1,   212,
    -1,     6,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] = {
    0,   201,   201,   203,   208,   207,   218,   217,   231,   235,
    239,   244,   253,   254,   259,   260,   265,   268,   273,   274,
    279,   280,   284,   285,   290,   300,   308,   309,   310,   314,
    320,   331,   332,   336,   344,   345,   350,   351,   352,   353,
    354,   355,   356,   361,   365,   378,   382,   390,   391,   392,
    397,   398,   399,   400,   401,   402,   403,   404,   405,   406,
    407,   408,   409,   410,   411,   412,   413,   414,   415,   416,
    421,   422,   423,   424,   429,   430,   434,   440,   448,   452,
    453,   460,   461,   466,   471,   472,   476,   477,   478,   479,
    480,   481,   482,   483,   484,   485,   486,   487,   492,   497,
    504,   509,   513,   518,   519,   524,   529,   530,   535,   540,
    545,   549,   554,   559,   563,   568,   573,   578,   579,   580,
    584,   589,   597,   598,   599,   604,   610,   614,   618,   622,
    629,   630,   634,   635,   636,   637,   641,   647,   653,   660,
    667,   671,   679,   680,   681,   685,   686,   690,   695,   703,
    704,   709,   713,   714,   718,   719,   726,   735,   741,   745,
    753,   754,   755,   762,   771,   772,   773,   774,   775,   776,
    780,   784,   792,   799,   803,   810,   817,   824,   828,   835,
    839,   843,   847,   854,   861,   862,   866,   867,   871,   872,
    873,   874,   880,   881,   882,   894,   895,   899,   904,   913,
    918,   919,   923,   924,   929,   936,   940,   948,   953,   954,
    958,   962,   963,   967,   971,   976,   977,   982,   986,   987,
    995,   996,  1000,  1001
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || YYTOKEN_TABLE
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char* const yytname[] = {
    "$end", "error", "$undefined", "MODULEREFERENCE", "TYPEREFERENCE",
    "OBJECTCLASSREFERENCE", "IDENTIFIER", "WOULDBE_OBJECTCLASSREFERENCE",
    "FIELDREFERENCE", "INTEGER", "CSTRING", "BSTRING", "HSTRING", "ABSENT",
    "ABSTRACT_SYNTAX", "ALL", "ANY", "APPLICATION", "ASSIGNMENT",
    "AUTOMATIC", "BEGIN_t", "BIT", "BMPString", "BOOLEAN_t", "BY",
    "CHARACTER", "CHOICE", "CLASS", "COMPONENT", "COMPONENTS", "CONSTRAINED",
    "DEFAULT", "DEFINED", "DEFINITIONS", "EMBEDDED", "END", "ENUMERATED",
    "EXCEPT", "EXPLICIT", "EXPORTS", "EXTERNAL", "FALSE_t", "FROM",
    "GeneralString", "GraphicString", "IA5String", "TYPE_IDENTIFIER",
    "IDENTIFIER_t", "IMPLICIT", "IMPORTS", "INCLUDES", "INSTANCE",
    "INTEGER_t", "INTERSECTION", "ISO646String", "MACRO", "MAX", "MIN",
    "MINUS_INFINITY", "NOTATION", "NULL_t", "NumericString", "OBJECT",
    "OCTET", "OF_t", "OPTIONAL_t", "PDV", "PLUS_INFINITY", "PRESENT",
    "PrintableString", "PRIVATE", "REAL", "SEQUENCE", "SET", "SIZE_t",
    "STRING", "SYNTAX", "T61String", "TAGS", "TeletexString", "TRUE_t",
    "TYPE_t", "UNION", "UNIQUE", "UNIVERSAL", "UniversalString",
    "VideotexString", "VisibleString", "GeneralizedTime", "UTCTime", "VALUE",
    "WITH", "ObjectDescriptor_t", "'{'", "'}'", "';'", "','", "'.'", "'['",
    "']'", "'<'", "'('", "')'", "':'", "'0'", "$accept",
    "ModuleDefinitionList", "ModuleDefinition", "$@1", "$@2", "TagDefault",
    "ModuleBody", "Exports", "SymbolsExported", "Imports", "SymbolsImported",
    "SymbolsFromModuleList", "SymbolsFromModule", "GlobalModuleReference",
    "AssignedIdentifier", "SymbolList", "Symbol", "ParameterizedReference",
    "AssignmentList", "Assignment", "ValueSetTypeAssignment",
    "TypeAssignment", "Type", "BuiltinType", "ReferencedType", "DefinedType",
    "ExternalTypeReference", "ParameterizedType", "SimpleDefinedType",
    "BitStringType", "BooleanType", "CharacterStringType",
    "RestrictedCharacterStringType", "UnrestrictedCharacterStringType",
    "ChoiceType", "EmbeddedPDVType", "EnumeratedType", "ExternalType",
    "AnyType", "InstanceOfType", "IntegerType", "NullType",
    "ObjectClassFieldType", "ObjectIdentifierType", "OctetStringType",
    "RealType", "SequenceType", "SequenceOfType", "SetType", "SetOfType",
    "TaggedType", "Tag", "SelectionType", "UsefulType",
    "InformationFromObjects", "ReferencedObjects", "ConstrainedType",
    "TypeWithConstraint", "Constraint", "SizeConstraint",
    "ObjectClassAssignment", "ObjectAssignment", "ObjectSetAssignment",
    "ObjectClass", "DefinedObjectClass", "ExternalObjectClassReference",
    "UsefulObjectClassReference", "ObjectClassDefn", "WithSyntaxSpec",
    "DefinedObject", "ExternalObjectReference", "ParameterizedObjectClass",
    "FieldName", "Object", "ObjectSet", "ParameterizedAssignment",
    "ParameterizedTypeAssignment", "ParameterizedValueAssignment",
    "ParameterizedValueSetTypeAssignment",
    "ParameterizedObjectClassAssignment", "ParameterizedObjectAssignment",
    "ParameterizedObjectSetAssignment",
    "ParameterizedWouldbeObjectClassReference", "ParameterizedTypeReference",
    "ParameterizedIdentifier", "ParameterList", "ValueAssignment", "Value",
    "NonOpenTypeValue", "BuiltinValue", "DefinedValue",
    "ExternalValueReference", "ObjectIdentifierValue", "BitStringValue",
    "BooleanValue", "CharacterStringValue", "RestrictedCharacterStringValue",
    "ChoiceValue", "NullValue", "RealValue", "NumericRealValue",
    "SpecialRealValue", "ObjectClassFieldValue", "OpenTypeFieldVal",
    "ReferencedValue", "ParameterizedValue", "SimpleDefinedValue",
    "TypeReference", "Reference", 0
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[YYLEX-NUM] -- Internal token number corresponding to
   token YYLEX-NUM.  */
static const yytype_uint16 yytoknum[] = {
    0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
    265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
    275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
    285,   286,   287,   288,   289,   290,   291,   292,   293,   294,
    295,   296,   297,   298,   299,   300,   301,   302,   303,   304,
    305,   306,   307,   308,   309,   310,   311,   312,   313,   314,
    315,   316,   317,   318,   319,   320,   321,   322,   323,   324,
    325,   326,   327,   328,   329,   330,   331,   332,   333,   334,
    335,   336,   337,   338,   339,   340,   341,   342,   343,   344,
    345,   346,   347,   123,   125,    59,    44,    46,    91,    93,
    60,    40,    41,    58,    48
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] = {
    0,   105,   106,   106,   108,   107,   109,   107,   110,   110,
    110,   110,   111,   111,   112,   112,   113,   113,   114,   114,
    115,   115,   116,   116,   117,   118,   119,   119,   119,   120,
    120,   121,   121,   122,   123,   123,   124,   124,   124,   124,
    124,   124,   124,   125,   125,   126,   126,   127,   127,   127,
    128,   128,   128,   128,   128,   128,   128,   128,   128,   128,
    128,   128,   128,   128,   128,   128,   128,   128,   128,   128,
    129,   129,   129,   129,   130,   130,   130,   131,   132,   133,
    133,   134,   134,   135,   136,   136,   137,   137,   137,   137,
    137,   137,   137,   137,   137,   137,   137,   137,   138,   139,
    140,   141,   142,   143,   143,   144,   145,   145,   146,   147,
    148,   149,   150,   151,   152,   153,   154,   155,   155,   155,
    156,   157,   158,   158,   158,   159,   160,   160,   160,   160,
    161,   161,   162,   162,   162,   162,   163,   164,   165,   166,
    167,   167,   168,   168,   168,   169,   169,   169,   170,   171,
    171,   172,   173,   173,   174,   174,   175,   176,   177,   177,
    178,   178,   178,   179,   180,   180,   180,   180,   180,   180,
    181,   181,   182,   183,   183,   184,   185,   186,   186,   187,
    188,   189,   190,   191,   192,   192,   193,   193,   194,   194,
    194,   194,   194,   194,   194,   195,   195,   195,   196,   197,
    198,   198,   199,   199,   200,   201,   202,   203,   204,   204,
    205,   206,   206,   207,   208,   209,   209,   210,   211,   211,
    212,   212,   213,   213
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] = {
    0,     2,     2,     1,     0,    10,     0,     8,     2,     2,
    2,     0,     3,     0,     3,     0,     1,     0,     3,     0,
    1,     0,     1,     2,     3,     2,     1,     1,     0,     1,
    3,     1,     1,     3,     1,     2,     1,     1,     1,     1,
    1,     1,     1,     5,     5,     3,     3,     1,     1,     1,
    1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
    1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
    1,     1,     1,     1,     1,     1,     1,     3,     3,     1,
    1,     2,     4,     1,     1,     1,     1,     1,     1,     1,
    1,     1,     1,     1,     1,     1,     1,     1,     2,     3,
    2,     3,     1,     1,     4,     3,     1,     3,     1,     3,
    2,     2,     1,     3,     3,     3,     3,     2,     3,     3,
    2,     3,     1,     1,     1,     3,     1,     3,     1,     1,
    2,     1,     4,     4,     4,     4,     2,     2,     3,     4,
    4,     4,     1,     1,     1,     1,     1,     1,     3,     1,
    1,     4,     4,     0,     1,     1,     3,     3,     3,     1,
    1,     2,     1,     2,     1,     1,     1,     1,     1,     1,
    3,     3,     4,     5,     5,     3,     4,     4,     4,     2,
    2,     2,     2,     4,     1,     1,     1,     1,     1,     1,
    1,     1,     1,     1,     1,     1,     1,     1,     3,     2,
    1,     1,     1,     1,     1,     1,     3,     1,     1,     1,
    1,     1,     1,     1,     3,     1,     1,     3,     1,     1,
    1,     1,     1,     1
};

/* YYDEFACT[STATE-NAME] -- Default rule to reduce with in state
   STATE-NUM when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint8 yydefact[] = {
    0,   220,   221,     0,     3,     0,     1,     2,    11,     0,
    0,     0,     0,     0,     0,    10,     8,     9,     0,    11,
    6,     0,    15,     0,    17,     0,    19,     4,   223,     0,
    16,    29,    32,   222,    31,     7,    21,     0,    15,    14,
    0,     0,     0,    20,    22,     0,     0,     0,     0,    12,
    34,    38,    36,    39,    40,    41,    42,   164,   165,   166,
    167,   168,   169,     0,     0,     0,    37,     0,    30,    33,
    18,    23,     0,     0,   146,   126,   150,   103,     0,     0,
    86,    83,     0,     0,     0,     0,   102,    87,    88,    89,
    149,     0,   106,    90,   108,    91,     0,     0,    92,   112,
    0,     0,    94,    93,    95,    96,    97,   122,   123,   124,
    0,     0,     0,    49,    48,    70,    74,    76,     0,    50,
    51,    52,    84,    85,    53,    54,    55,    56,    57,    58,
    59,    60,    61,    62,    63,    64,    65,    66,    67,    68,
    69,     0,    72,    71,    73,     0,    47,   131,     0,   145,
    147,   180,    75,     0,     0,   181,     0,     0,     0,   179,
    35,     0,     0,     0,     0,     0,     0,     0,     0,     5,
    24,    28,     0,     0,     0,     0,    45,     0,    81,    98,
    0,   100,     0,     0,     0,   110,   111,     0,     0,     0,
    0,     0,     0,     0,     0,     0,     0,   182,   120,     0,
    130,     0,     0,     0,   117,     0,     0,     0,     0,     0,
    0,    46,   138,   142,   143,   144,     0,     0,   171,   175,
    0,     0,   170,     0,     0,     0,     0,     0,   196,     0,
    25,    26,   195,    27,   197,     0,   148,    77,   127,   121,
    0,     0,    99,   101,     0,   105,   107,   114,   137,   113,
    136,     0,     0,   116,   115,     0,     0,     0,    78,   119,
    118,   159,   125,     0,   140,   109,     0,   196,   205,   200,
    201,   203,   212,   207,   211,   202,   210,     0,   216,   183,
    184,   186,   215,   193,   188,   189,   190,   204,   191,   192,
    194,   208,   209,   185,   213,   187,     0,   155,     0,    79,
    129,   162,   160,   154,   139,    80,     0,     0,     0,   141,
    0,   178,     0,   177,   172,   176,     0,   199,     0,   104,
    82,     0,   134,   135,   132,   133,    43,     0,   163,     0,
    0,     0,     0,   161,   153,   157,    44,   174,   173,   198,
    217,   158,   206,   214,   156,     0,   151,     0,     0,   152
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] = {
    -1,     3,     4,    38,    22,    13,    25,    26,    29,    37,
    42,    43,    44,   170,   230,    45,    31,    32,    49,    50,
    51,    52,   277,   113,   114,   115,   116,   117,   118,   119,
    120,   121,   122,   123,   124,   125,   126,   127,   128,   129,
    130,   131,   132,   133,   134,   135,   136,   137,   138,   139,
    140,   141,   142,   143,   144,   145,   146,   147,   200,   192,
    53,    54,    55,   212,   177,   149,   150,   214,   346,   302,
    303,   215,   262,   304,   264,    56,    57,    58,    59,    60,
    61,    62,    63,    64,    65,   151,    66,   279,   280,   281,
    282,   232,   283,   284,   285,   286,   287,   288,   289,   290,
    291,   292,   293,   294,   295,   234,   235,   152,    34
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -225
static const yytype_int16 yypact[] = {
    59,  -225,  -225,    60,  -225,   -14,  -225,  -225,    32,   -69,
    -39,   -17,     9,    35,    57,  -225,  -225,  -225,    75,    32,
    -225,    81,    53,   100,   105,    87,    84,  -225,  -225,    41,
    -225,    52,  -225,  -225,    62,  -225,   105,   125,    53,  -225,
    105,    63,    64,   105,  -225,   116,   319,   601,   415,   125,
    -225,  -225,  -225,  -225,  -225,  -225,  -225,  -225,  -225,  -225,
    -225,  -225,  -225,   697,   787,   967,  -225,   126,  -225,  -225,
    -225,  -225,    59,    65,  -225,    -7,  -225,   128,   967,    88,
    -225,  -225,    89,    74,   102,    76,  -225,  -225,  -225,  -225,
    -225,   107,    77,  -225,  -225,  -225,   127,    98,  -225,  -225,
    -19,   -18,  -225,  -225,  -225,  -225,  -225,  -225,  -225,  -225,
    82,    78,   -16,  -225,  -225,  -225,    85,    83,    86,  -225,
    -225,  -225,  -225,  -225,  -225,  -225,  -225,  -225,  -225,  -225,
    -225,  -225,  -225,  -225,  -225,  -225,  -225,  -225,  -225,  -225,
    -225,   511,  -225,  -225,  -225,    90,  -225,  -225,    19,  -225,
    -225,  -225,     3,   -12,    20,  -225,   877,    -4,    24,  -225,
    -225,   877,     2,    26,   967,    13,    29,    17,    31,  -225,
    -225,    15,   138,    91,   967,   151,    80,    92,    93,  -225,
    94,  -225,    96,    45,    97,  -225,  -225,   967,    80,    99,
    95,   118,   119,   967,   101,   120,   130,  -225,  -225,   103,
    -225,   104,   967,   967,    80,   184,   108,   184,   217,     1,
    109,    80,  -225,     5,  -225,  -225,   110,   108,    80,  -225,
    111,   108,    80,   112,   108,   217,     1,   113,   114,   117,
    -225,  -225,   115,  -225,  -225,   123,  -225,  -225,  -225,    80,
    193,   131,  -225,  -225,   133,  -225,  -225,    80,  -225,  -225,
    -225,   967,   967,    80,  -225,   967,   967,   141,  -225,    80,
    80,  -225,   135,   142,  -225,   135,   137,   -35,  -225,  -225,
    -225,  -225,  -225,   -25,  -225,  -225,  -225,    12,    48,  -225,
    -225,  -225,  -225,  -225,  -225,  -225,  -225,  -225,  -225,  -225,
    -225,  -225,  -225,  -225,  -225,  -225,   144,     8,   143,  -225,
    -225,  -225,  -225,  -225,  -225,   147,   152,   153,   154,  -225,
    155,  -225,   156,  -225,  -225,  -225,   194,  -225,   158,  -225,
    -225,   201,    80,    80,    80,    80,  -225,   204,  -225,   134,
    217,   217,   140,  -225,   122,  -225,  -225,  -225,  -225,  -225,
    -225,  -225,  -225,  -225,  -225,   169,  -225,   124,   160,  -225
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] = {
    -225,  -225,   252,  -225,  -225,   237,   221,  -225,  -225,  -225,
    -225,  -225,   222,  -225,  -225,    33,  -225,  -225,  -225,   215,
    -225,  -225,   -37,  -225,  -225,  -225,  -157,  -155,  -225,  -225,
    -225,  -225,  -225,  -225,  -225,  -225,  -225,  -225,  -225,  -225,
    -225,  -225,  -225,  -225,  -225,  -225,  -225,  -225,  -225,  -225,
    -225,  -225,  -225,  -225,  -196,  -225,  -225,  -225,   -78,   165,
    -225,  -225,  -225,   121,   -31,  -225,  -225,  -225,  -225,  -225,
    -225,  -225,    66,    44,  -140,  -225,  -225,  -225,  -225,  -225,
    -225,  -225,  -225,  -225,  -225,   106,  -225,  -224,  -225,  -225,
    129,  -225,   136,  -225,  -225,  -225,  -225,  -225,  -225,  -225,
    -225,  -225,  -225,  -225,  -225,  -225,  -225,     0,  -225
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -220
static const yytype_int16 yytable[] = {
    5,   314,   199,     5,   296,     1,   208,   297,     2,   112,
    153,   157,   278,   301,   216,   148,   154,   158,   227,     8,
    220,   228,   191,   195,    33,    14,   162,   165,   167,   278,
    301,   223,   163,   166,   168,   225,    33,   206,   209,    15,
    33,   176,   217,    33,   221,   187,   193,   224,   244,   226,
    74,    10,   299,    18,   300,   188,   188,    30,   173,    76,
    6,    16,  -126,     1,     1,   174,     2,     2,   330,   299,
    11,   300,   171,    68,   189,   194,  -108,   309,  -108,     9,
    12,   311,   190,   190,   313,   190,   173,    17,   -13,   190,
    19,    90,    24,   174,   298,    20,   -80,   190,   307,    23,
    -128,   173,   207,   190,   204,  -126,   342,   343,   229,     1,
    248,    28,     2,   190,   190,   331,   207,   207,   190,   211,
    27,   207,    35,   207,   218,   213,   207,   222,   207,    46,
    213,    47,    48,    36,   278,   278,    39,   239,     1,   236,
    339,     2,     1,   236,     1,     2,   344,     2,    40,   -73,
    247,   -73,   245,   155,   159,    41,   253,    69,    72,    70,
    175,   169,   172,   178,   179,   259,   260,   180,   181,   182,
    184,   183,   237,   186,   185,   240,   197,   198,   -79,   201,
    -129,   190,   251,   252,   255,   238,   241,   205,   242,   207,
    243,   246,   261,   249,   256,   254,   257,   250,   258,   319,
    339,   263,   306,   308,   310,   312,   236,  -219,  -218,   305,
    316,   317,   341,   345,   322,   323,   318,   348,   324,   325,
    266,     1,    74,   267,     2,   320,   305,   268,   269,   270,
    321,    76,   327,    77,   329,   326,   328,   333,    79,    80,
    81,   332,    82,    83,  -128,   347,   334,   335,   336,   337,
    338,    84,   340,    85,   349,     7,    21,    86,   271,    67,
    87,    88,    89,    90,   160,    71,   196,     0,    91,    92,
    315,    93,     0,   265,     0,   272,     0,   273,    95,    96,
    97,     0,   219,     0,   274,     0,    98,     0,    99,   100,
    101,     0,     0,     0,   102,     0,   103,   275,     0,     0,
    231,     0,   104,   105,   106,   107,   108,   233,     0,   109,
    229,     0,     0,     0,     0,   111,     0,     0,     0,     0,
    0,   276,    73,     1,    74,    75,     2,     0,     0,   237,
    0,     0,   237,    76,     0,    77,     0,    78,     0,     0,
    79,    80,    81,     0,    82,    83,     0,     0,     0,     0,
    0,     0,     0,    84,     0,    85,     0,     0,     0,    86,
    0,     0,    87,    88,    89,    90,     0,     0,     0,     0,
    91,    92,     0,    93,     0,     0,     0,     0,     0,    94,
    95,    96,    97,     0,     0,     0,     0,     0,    98,     0,
    99,   100,   101,     0,     0,     0,   102,     0,   103,     0,
    0,     0,     0,     0,   104,   105,   106,   107,   108,     0,
    0,   109,   110,     0,     0,     0,     0,   111,    73,     1,
    74,    75,     2,     0,     0,     0,     0,     0,     0,    76,
    0,    77,     0,   156,     0,     0,    79,    80,    81,     0,
    82,    83,     0,     0,     0,     0,     0,     0,     0,    84,
    0,    85,     0,     0,     0,    86,     0,     0,    87,    88,
    89,    90,     0,     0,     0,     0,    91,    92,     0,    93,
    0,     0,     0,     0,     0,    94,    95,    96,    97,     0,
    0,     0,     0,     0,    98,     0,    99,   100,   101,     0,
    0,     0,   102,     0,   103,     0,     0,     0,     0,     0,
    104,   105,   106,   107,   108,     0,     0,   109,   110,     0,
    0,     0,     0,   111,    73,     1,    74,    75,     2,     0,
    0,     0,     0,     0,     0,    76,     0,    77,     0,     0,
    0,     0,    79,    80,    81,     0,    82,    83,     0,     0,
    0,     0,     0,     0,     0,    84,     0,    85,     0,   202,
    0,    86,     0,     0,    87,    88,    89,    90,     0,   203,
    0,     0,    91,    92,     0,    93,     0,     0,     0,     0,
    0,    94,    95,    96,    97,     0,     0,     0,     0,     0,
    98,     0,    99,   100,   101,     0,     0,     0,   102,     0,
    103,     0,     0,     0,     0,     0,   104,   105,   106,   107,
    108,     0,     0,   109,    73,     1,    74,    75,     2,   111,
    0,     0,     0,     0,     0,    76,     0,    77,     0,     0,
    0,     0,    79,    80,    81,     0,    82,    83,     0,     0,
    0,     0,     0,     0,     0,    84,     0,    85,     0,     0,
    0,    86,     0,     0,    87,    88,    89,    90,     0,     0,
    0,     0,    91,    92,     0,    93,     0,     0,     0,     0,
    0,    94,    95,    96,    97,     0,     0,     0,     0,     0,
    98,     0,    99,   100,   101,     0,     0,     0,   102,     0,
    103,     0,     0,     0,     0,     0,   104,   105,   106,   107,
    108,     0,     0,   109,   110,     0,     0,     0,     0,   111,
    73,     1,    74,    75,     2,     0,     0,     0,     0,     0,
    0,    76,     0,    77,     0,   161,     0,     0,    79,    80,
    81,     0,    82,    83,     0,     0,     0,     0,     0,     0,
    0,    84,     0,    85,     0,     0,     0,    86,     0,     0,
    87,    88,    89,    90,     0,     0,     0,     0,    91,    92,
    0,    93,     0,     0,     0,     0,     0,    94,    95,    96,
    97,     0,     0,     0,     0,     0,    98,     0,    99,   100,
    101,     0,     0,     0,   102,     0,   103,     0,     0,     0,
    0,     0,   104,   105,   106,   107,   108,     0,     0,   109,
    73,     1,    74,    75,     2,   111,     0,     0,     0,     0,
    0,    76,     0,    77,     0,   164,     0,     0,    79,    80,
    81,     0,    82,    83,     0,     0,     0,     0,     0,     0,
    0,    84,     0,    85,     0,     0,     0,    86,     0,     0,
    87,    88,    89,    90,     0,     0,     0,     0,    91,    92,
    0,    93,     0,     0,     0,     0,     0,    94,    95,    96,
    97,     0,     0,     0,     0,     0,    98,     0,    99,   100,
    101,     0,     0,     0,   102,     0,   103,     0,     0,     0,
    0,     0,   104,   105,   106,   107,   108,     0,     0,   109,
    73,     1,    74,    75,     2,   111,     0,     0,     0,     0,
    0,    76,     0,    77,     0,     0,     0,     0,    79,    80,
    81,     0,    82,    83,   210,     0,     0,     0,     0,     0,
    0,    84,     0,    85,     0,     0,     0,    86,     0,     0,
    87,    88,    89,    90,     0,     0,     0,     0,    91,    92,
    0,    93,     0,     0,     0,     0,     0,    94,    95,    96,
    97,     0,     0,     0,     0,     0,    98,     0,    99,   100,
    101,     0,     0,     0,   102,     0,   103,     0,     0,     0,
    0,     0,   104,   105,   106,   107,   108,     0,     0,   109,
    73,     1,    74,    75,     2,   111,     0,     0,     0,     0,
    0,    76,     0,    77,     0,     0,     0,     0,    79,    80,
    81,     0,    82,    83,     0,     0,     0,     0,     0,     0,
    0,    84,     0,    85,     0,     0,     0,    86,     0,     0,
    87,    88,    89,    90,     0,     0,     0,     0,    91,    92,
    0,    93,     0,     0,     0,     0,     0,    94,    95,    96,
    97,     0,     0,     0,     0,     0,    98,     0,    99,   100,
    101,     0,     0,     0,   102,     0,   103,     0,     0,     0,
    0,     0,   104,   105,   106,   107,   108,     0,     0,   109,
    0,     0,     0,     0,     0,   111
};

static const yytype_int16 yycheck[] = {
    0,   225,    18,     3,     3,     4,    18,     6,     7,    46,
    47,    48,   208,   209,    18,    46,    47,    48,     3,    33,
    18,     6,   100,   101,    24,    94,    63,    64,    65,   225,
    226,    18,    63,    64,    65,    18,    36,    18,    18,    78,
    40,    78,    18,    43,    18,    64,    64,    18,     3,    18,
    5,    19,   209,    18,   209,    74,    74,    24,    93,    14,
    0,    78,    97,     4,     4,   100,     7,     7,   103,   226,
    38,   226,    72,    40,    93,    93,   101,   217,   103,    93,
    48,   221,   101,   101,   224,   101,    93,    78,    35,   101,
    33,    46,    39,   100,    93,    20,    93,   101,    93,    18,
    97,    93,    97,   101,   141,    97,   330,   331,    93,     4,
    188,     6,     7,   101,   101,   103,    97,    97,   101,   156,
    20,    97,    35,    97,   161,   156,    97,   164,    97,     4,
    161,     6,     7,    49,   330,   331,    95,   174,     4,     5,
    6,     7,     4,     5,     4,     7,     6,     7,    96,   101,
    187,   103,   183,    47,    48,    93,   193,    94,    42,    95,
    32,    35,    97,    75,    75,   202,   203,    93,    66,    93,
    93,    64,   172,    75,    47,    24,    94,    99,    93,    93,
    97,   101,    64,    64,    64,    94,    93,    97,    94,    97,
    94,    94,     8,    94,    64,    94,    93,   102,    94,     6,
    6,    93,    93,    93,    93,    93,     5,    93,    93,   209,
    97,    94,     8,    91,   251,   252,    93,    93,   255,   256,
    3,     4,     5,     6,     7,    94,   226,    10,    11,    12,
    97,    14,    97,    16,    97,    94,    94,    94,    21,    22,
    23,    97,    25,    26,    97,    76,    94,    94,    94,    94,
    94,    34,    94,    36,    94,     3,    19,    40,    41,    38,
    43,    44,    45,    46,    49,    43,   101,    -1,    51,    52,
    226,    54,    -1,   207,    -1,    58,    -1,    60,    61,    62,
    63,    -1,   161,    -1,    67,    -1,    69,    -1,    71,    72,
    73,    -1,    -1,    -1,    77,    -1,    79,    80,    -1,    -1,
    171,    -1,    85,    86,    87,    88,    89,   171,    -1,    92,
    93,    -1,    -1,    -1,    -1,    98,    -1,    -1,    -1,    -1,
    -1,   104,     3,     4,     5,     6,     7,    -1,    -1,   329,
    -1,    -1,   332,    14,    -1,    16,    -1,    18,    -1,    -1,
    21,    22,    23,    -1,    25,    26,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    34,    -1,    36,    -1,    -1,    -1,    40,
    -1,    -1,    43,    44,    45,    46,    -1,    -1,    -1,    -1,
    51,    52,    -1,    54,    -1,    -1,    -1,    -1,    -1,    60,
    61,    62,    63,    -1,    -1,    -1,    -1,    -1,    69,    -1,
    71,    72,    73,    -1,    -1,    -1,    77,    -1,    79,    -1,
    -1,    -1,    -1,    -1,    85,    86,    87,    88,    89,    -1,
    -1,    92,    93,    -1,    -1,    -1,    -1,    98,     3,     4,
    5,     6,     7,    -1,    -1,    -1,    -1,    -1,    -1,    14,
    -1,    16,    -1,    18,    -1,    -1,    21,    22,    23,    -1,
    25,    26,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    34,
    -1,    36,    -1,    -1,    -1,    40,    -1,    -1,    43,    44,
    45,    46,    -1,    -1,    -1,    -1,    51,    52,    -1,    54,
    -1,    -1,    -1,    -1,    -1,    60,    61,    62,    63,    -1,
    -1,    -1,    -1,    -1,    69,    -1,    71,    72,    73,    -1,
    -1,    -1,    77,    -1,    79,    -1,    -1,    -1,    -1,    -1,
    85,    86,    87,    88,    89,    -1,    -1,    92,    93,    -1,
    -1,    -1,    -1,    98,     3,     4,     5,     6,     7,    -1,
    -1,    -1,    -1,    -1,    -1,    14,    -1,    16,    -1,    -1,
    -1,    -1,    21,    22,    23,    -1,    25,    26,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    34,    -1,    36,    -1,    38,
    -1,    40,    -1,    -1,    43,    44,    45,    46,    -1,    48,
    -1,    -1,    51,    52,    -1,    54,    -1,    -1,    -1,    -1,
    -1,    60,    61,    62,    63,    -1,    -1,    -1,    -1,    -1,
    69,    -1,    71,    72,    73,    -1,    -1,    -1,    77,    -1,
    79,    -1,    -1,    -1,    -1,    -1,    85,    86,    87,    88,
    89,    -1,    -1,    92,     3,     4,     5,     6,     7,    98,
    -1,    -1,    -1,    -1,    -1,    14,    -1,    16,    -1,    -1,
    -1,    -1,    21,    22,    23,    -1,    25,    26,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    34,    -1,    36,    -1,    -1,
    -1,    40,    -1,    -1,    43,    44,    45,    46,    -1,    -1,
    -1,    -1,    51,    52,    -1,    54,    -1,    -1,    -1,    -1,
    -1,    60,    61,    62,    63,    -1,    -1,    -1,    -1,    -1,
    69,    -1,    71,    72,    73,    -1,    -1,    -1,    77,    -1,
    79,    -1,    -1,    -1,    -1,    -1,    85,    86,    87,    88,
    89,    -1,    -1,    92,    93,    -1,    -1,    -1,    -1,    98,
    3,     4,     5,     6,     7,    -1,    -1,    -1,    -1,    -1,
    -1,    14,    -1,    16,    -1,    18,    -1,    -1,    21,    22,
    23,    -1,    25,    26,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    34,    -1,    36,    -1,    -1,    -1,    40,    -1,    -1,
    43,    44,    45,    46,    -1,    -1,    -1,    -1,    51,    52,
    -1,    54,    -1,    -1,    -1,    -1,    -1,    60,    61,    62,
    63,    -1,    -1,    -1,    -1,    -1,    69,    -1,    71,    72,
    73,    -1,    -1,    -1,    77,    -1,    79,    -1,    -1,    -1,
    -1,    -1,    85,    86,    87,    88,    89,    -1,    -1,    92,
    3,     4,     5,     6,     7,    98,    -1,    -1,    -1,    -1,
    -1,    14,    -1,    16,    -1,    18,    -1,    -1,    21,    22,
    23,    -1,    25,    26,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    34,    -1,    36,    -1,    -1,    -1,    40,    -1,    -1,
    43,    44,    45,    46,    -1,    -1,    -1,    -1,    51,    52,
    -1,    54,    -1,    -1,    -1,    -1,    -1,    60,    61,    62,
    63,    -1,    -1,    -1,    -1,    -1,    69,    -1,    71,    72,
    73,    -1,    -1,    -1,    77,    -1,    79,    -1,    -1,    -1,
    -1,    -1,    85,    86,    87,    88,    89,    -1,    -1,    92,
    3,     4,     5,     6,     7,    98,    -1,    -1,    -1,    -1,
    -1,    14,    -1,    16,    -1,    -1,    -1,    -1,    21,    22,
    23,    -1,    25,    26,    27,    -1,    -1,    -1,    -1,    -1,
    -1,    34,    -1,    36,    -1,    -1,    -1,    40,    -1,    -1,
    43,    44,    45,    46,    -1,    -1,    -1,    -1,    51,    52,
    -1,    54,    -1,    -1,    -1,    -1,    -1,    60,    61,    62,
    63,    -1,    -1,    -1,    -1,    -1,    69,    -1,    71,    72,
    73,    -1,    -1,    -1,    77,    -1,    79,    -1,    -1,    -1,
    -1,    -1,    85,    86,    87,    88,    89,    -1,    -1,    92,
    3,     4,     5,     6,     7,    98,    -1,    -1,    -1,    -1,
    -1,    14,    -1,    16,    -1,    -1,    -1,    -1,    21,    22,
    23,    -1,    25,    26,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    34,    -1,    36,    -1,    -1,    -1,    40,    -1,    -1,
    43,    44,    45,    46,    -1,    -1,    -1,    -1,    51,    52,
    -1,    54,    -1,    -1,    -1,    -1,    -1,    60,    61,    62,
    63,    -1,    -1,    -1,    -1,    -1,    69,    -1,    71,    72,
    73,    -1,    -1,    -1,    77,    -1,    79,    -1,    -1,    -1,
    -1,    -1,    85,    86,    87,    88,    89,    -1,    -1,    92,
    -1,    -1,    -1,    -1,    -1,    98
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] = {
    0,     4,     7,   106,   107,   212,     0,   107,    33,    93,
    19,    38,    48,   110,    94,    78,    78,    78,    18,    33,
    20,   110,   109,    18,    39,   111,   112,    20,     6,   113,
    120,   121,   122,   212,   213,    35,    49,   114,   108,    95,
    96,    93,   115,   116,   117,   120,     4,     6,     7,   123,
    124,   125,   126,   165,   166,   167,   180,   181,   182,   183,
    184,   185,   186,   187,   188,   189,   191,   111,   120,    94,
    95,   117,    42,     3,     5,     6,    14,    16,    18,    21,
    22,    23,    25,    26,    34,    36,    40,    43,    44,    45,
    46,    51,    52,    54,    60,    61,    62,    63,    69,    71,
    72,    73,    77,    79,    85,    86,    87,    88,    89,    92,
    93,    98,   127,   128,   129,   130,   131,   132,   133,   134,
    135,   136,   137,   138,   139,   140,   141,   142,   143,   144,
    145,   146,   147,   148,   149,   150,   151,   152,   153,   154,
    155,   156,   157,   158,   159,   160,   161,   162,   169,   170,
    171,   190,   212,   127,   169,   190,    18,   127,   169,   190,
    124,    18,   127,   169,    18,   127,   169,   127,   169,    35,
    118,   212,    97,    93,   100,    32,   127,   169,    75,    75,
    93,    66,    93,    64,    93,    47,    75,    64,    74,    93,
    101,   163,   164,    64,    93,   163,   164,    94,    99,    18,
    163,    93,    38,    48,   127,    97,    18,    97,    18,    18,
    27,   127,   168,   169,   172,   176,    18,    18,   127,   168,
    18,    18,   127,    18,    18,    18,    18,     3,     6,    93,
    119,   195,   196,   197,   210,   211,     5,   212,    94,   127,
    24,    93,    94,    94,     3,   169,    94,   127,   163,    94,
    102,    64,    64,   127,    94,    64,    64,    93,    94,   127,
    127,     8,   177,    93,   179,   177,     3,     6,    10,    11,
    12,    41,    58,    60,    67,    80,   104,   127,   159,   192,
    193,   194,   195,   197,   198,   199,   200,   201,   202,   203,
    204,   205,   206,   207,   208,   209,     3,     6,    93,   131,
    132,   159,   174,   175,   178,   212,    93,    93,    93,   179,
    93,   179,    93,   179,   192,   178,    97,    94,    93,     6,
    94,    97,   127,   127,   127,   127,    94,    97,    94,    97,
    103,   103,    97,    94,    94,    94,    94,    94,    94,     6,
    94,     8,   192,   192,     6,    91,   173,    76,    93,    94
};

#define yyerrok     (yyerrstatus = 0)
#define yyclearin   (yychar = YYEMPTY)
#define YYEMPTY     (-2)
#define YYEOF       0

#define YYACCEPT    goto yyacceptlab
#define YYABORT     goto yyabortlab
#define YYERROR     goto yyerrorlab


/* Like YYERROR except do call yyerror.  This remains here temporarily
   to ease the transition to the new meaning of YYERROR, for GCC.
   Once GCC version 2 has supplanted version 1, this can go.  */

#define YYFAIL      goto yyerrlab

#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                  \
    do                              \
        if (yychar == YYEMPTY && yylen == 1)              \
        {                               \
            yychar = (Token);                     \
            yylval = (Value);                     \
            yytoken = YYTRANSLATE (yychar);               \
            YYPOPSTACK (1);                       \
            goto yybackup;                        \
        }                               \
        else                              \
        {                               \
            yyerror (YY_("syntax error: cannot back up")); \
            YYERROR;                          \
        }                               \
    while (YYID (0))


#define YYTERROR    1
#define YYERRCODE   256


/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

#define YYRHSLOC(Rhs, K) ((Rhs)[K])
#ifndef YYLLOC_DEFAULT
# define YYLLOC_DEFAULT(Current, Rhs, N)                \
    do                                  \
        if (YYID (N))                                                    \
        {                               \
            (Current).first_line   = YYRHSLOC (Rhs, 1).first_line;    \
            (Current).first_column = YYRHSLOC (Rhs, 1).first_column;  \
            (Current).last_line    = YYRHSLOC (Rhs, N).last_line;     \
            (Current).last_column  = YYRHSLOC (Rhs, N).last_column;   \
        }                               \
        else                              \
        {                               \
            (Current).first_line   = (Current).last_line   =      \
                                                                  YYRHSLOC (Rhs, 0).last_line;                \
            (Current).first_column = (Current).last_column =      \
                                                                  YYRHSLOC (Rhs, 0).last_column;              \
        }                               \
    while (YYID (0))
#endif


/* YY_LOCATION_PRINT -- Print the location on the stream.
   This macro was not mandated originally: define only if we know
   we won't break user code: when these are the locations we know.  */

#ifndef YY_LOCATION_PRINT
# if YYLTYPE_IS_TRIVIAL
#  define YY_LOCATION_PRINT(File, Loc)          \
    fprintf (File, "%d.%d-%d.%d",          \
             (Loc).first_line, (Loc).first_column, \
             (Loc).last_line,  (Loc).last_column)
# else
#  define YY_LOCATION_PRINT(File, Loc) ((void) 0)
# endif
#endif


/* YYLEX -- calling `yylex' with the right arguments.  */

#ifdef YYLEX_PARAM
# define YYLEX yylex (YYLEX_PARAM)
#else
# define YYLEX yylex ()
#endif

/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)            \
    do {                        \
        if (yydebug)                  \
            YYFPRINTF Args;             \
    } while (YYID (0))

# define YY_SYMBOL_PRINT(Title, Type, Value, Location)            \
    do {                                      \
        if (yydebug)                                \
        {                                     \
            YYFPRINTF (stderr, "%s ", Title);                   \
            yy_symbol_print (stderr,                        \
                             Type, Value); \
            YYFPRINTF (stderr, "\n");                       \
        }                                     \
    } while (YYID (0))


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_value_print(FILE* yyoutput, int yytype, YYSTYPE const* const yyvaluep)
#else
static void
yy_symbol_value_print(yyoutput, yytype, yyvaluep)
FILE* yyoutput;
int yytype;
YYSTYPE const* const yyvaluep;
#endif
{
    if (!yyvaluep)
        return;
# ifdef YYPRINT
    if (yytype < YYNTOKENS)
        YYPRINT(yyoutput, yytoknum[yytype], *yyvaluep);
# else
    YYUSE(yyoutput);
# endif
    switch (yytype) {
    default:
        break;
    }
}


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_print(FILE* yyoutput, int yytype, YYSTYPE const* const yyvaluep)
#else
static void
yy_symbol_print(yyoutput, yytype, yyvaluep)
FILE* yyoutput;
int yytype;
YYSTYPE const* const yyvaluep;
#endif
{
    if (yytype < YYNTOKENS)
        YYFPRINTF(yyoutput, "token %s (", yytname[yytype]);
    else
        YYFPRINTF(yyoutput, "nterm %s (", yytname[yytype]);

    yy_symbol_value_print(yyoutput, yytype, yyvaluep);
    YYFPRINTF(yyoutput, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_stack_print(yytype_int16* yybottom, yytype_int16* yytop)
#else
static void
yy_stack_print(yybottom, yytop)
yytype_int16* yybottom;
yytype_int16* yytop;
#endif
{
    YYFPRINTF(stderr, "Stack now");
    for (; yybottom <= yytop; yybottom++) {
        int yybot = *yybottom;
        YYFPRINTF(stderr, " %d", yybot);
    }
    YYFPRINTF(stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)                \
    do {                                \
        if (yydebug)                          \
            yy_stack_print ((Bottom), (Top));               \
    } while (YYID (0))


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_reduce_print(YYSTYPE* yyvsp, int yyrule)
#else
static void
yy_reduce_print(yyvsp, yyrule)
YYSTYPE* yyvsp;
int yyrule;
#endif
{
    int yynrhs = yyr2[yyrule];
    int yyi;
    unsigned long int yylno = yyrline[yyrule];
    YYFPRINTF(stderr, "Reducing stack by rule %d (line %lu):\n",
              yyrule - 1, yylno);
    /* The symbols being reduced.  */
    for (yyi = 0; yyi < yynrhs; yyi++) {
        YYFPRINTF(stderr, "   $%d = ", yyi + 1);
        yy_symbol_print(stderr, yyrhs[yyprhs[yyrule] + yyi],
                        &(yyvsp[(yyi + 1) - (yynrhs)])
                       );
        YYFPRINTF(stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)      \
    do {                    \
        if (yydebug)              \
            yy_reduce_print (yyvsp, Rule); \
    } while (YYID (0))

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args)
# define YY_SYMBOL_PRINT(Title, Type, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif



#if YYERROR_VERBOSE

# ifndef yystrlen
#  if defined __GLIBC__ && defined _STRING_H
#   define yystrlen strlen
#  else
/* Return the length of YYSTR.  */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static YYSIZE_T
yystrlen(const char* yystr)
#else
static YYSIZE_T
yystrlen(yystr)
const char* yystr;
#endif
{
    YYSIZE_T yylen;
    for (yylen = 0; yystr[yylen]; yylen++)
        continue;
    return yylen;
}
#  endif
# endif

# ifndef yystpcpy
#  if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static char*
yystpcpy(char* yydest, const char* yysrc)
#else
static char*
yystpcpy(yydest, yysrc)
char* yydest;
const char* yysrc;
#endif
{
    char* yyd = yydest;
    const char* yys = yysrc;

    while ((*yyd++ = *yys++) != '\0')
        continue;

    return yyd - 1;
}
#  endif
# endif

# ifndef yytnamerr
/* Copy to YYRES the contents of YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for yyerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  YYSTR is taken from yytname.  If YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static YYSIZE_T
yytnamerr(char* yyres, const char* yystr) {
    if (*yystr == '"') {
        YYSIZE_T yyn = 0;
        char const* yyp = yystr;

        for (;;)
            switch (*++yyp) {
            case '\'':
            case ',':
                goto do_not_strip_quotes;

            case '\\':
                if (*++yyp != '\\')
                    goto do_not_strip_quotes;
            /* Fall through.  */
            default:
                if (yyres)
                    yyres[yyn] = *yyp;
                yyn++;
                break;

            case '"':
                if (yyres)
                    yyres[yyn] = '\0';
                return yyn;
            }
do_not_strip_quotes: ;
    }

    if (! yyres)
        return yystrlen(yystr);

    return yystpcpy(yyres, yystr) - yyres;
}
# endif

/* Copy into YYRESULT an error message about the unexpected token
   YYCHAR while in state YYSTATE.  Return the number of bytes copied,
   including the terminating null byte.  If YYRESULT is null, do not
   copy anything; just return the number of bytes that would be
   copied.  As a special case, return 0 if an ordinary "syntax error"
   message will do.  Return YYSIZE_MAXIMUM if overflow occurs during
   size calculation.  */
static YYSIZE_T
yysyntax_error(char* yyresult, int yystate, int yychar) {
    int yyn = yypact[yystate];

    if (!(YYPACT_NINF < yyn && yyn <= YYLAST))
        return 0;
    else {
        int yytype = YYTRANSLATE(yychar);
        YYSIZE_T yysize0 = yytnamerr(0, yytname[yytype]);
        YYSIZE_T yysize = yysize0;
        YYSIZE_T yysize1;
        int yysize_overflow = 0;
        enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
        char const* yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
        int yyx;

# if 0
        /* This is so xgettext sees the translatable formats that are
        constructed on the fly.  */
        YY_("syntax error, unexpected %s");
        YY_("syntax error, unexpected %s, expecting %s");
        YY_("syntax error, unexpected %s, expecting %s or %s");
        YY_("syntax error, unexpected %s, expecting %s or %s or %s");
        YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s");
# endif
        char* yyfmt;
        char const* yyf;
        static char const yyunexpected[] = "syntax error, unexpected %s";
        static char const yyexpecting[] = ", expecting %s";
        static char const yyor[] = " or %s";
        char yyformat[sizeof yyunexpected
                      + sizeof yyexpecting - 1
                      + ((YYERROR_VERBOSE_ARGS_MAXIMUM - 2)
                         * (sizeof yyor - 1))];
        char const* yyprefix = yyexpecting;

        /* Start YYX at -YYN if negative to avoid negative indexes in
        YYCHECK.  */
        int yyxbegin = yyn < 0 ? -yyn : 0;

        /* Stay within bounds of both yycheck and yytname.  */
        int yychecklim = YYLAST - yyn + 1;
        int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
        int yycount = 1;

        yyarg[0] = yytname[yytype];
        yyfmt = yystpcpy(yyformat, yyunexpected);

        for (yyx = yyxbegin; yyx < yyxend; ++yyx)
            if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR) {
                if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM) {
                    yycount = 1;
                    yysize = yysize0;
                    yyformat[sizeof yyunexpected - 1] = '\0';
                    break;
                }
                yyarg[yycount++] = yytname[yyx];
                yysize1 = yysize + yytnamerr(0, yytname[yyx]);
                yysize_overflow |= (yysize1 < yysize);
                yysize = yysize1;
                yyfmt = yystpcpy(yyfmt, yyprefix);
                yyprefix = yyor;
            }

        yyf = YY_(yyformat);
        yysize1 = yysize + yystrlen(yyf);
        yysize_overflow |= (yysize1 < yysize);
        yysize = yysize1;

        if (yysize_overflow)
            return YYSIZE_MAXIMUM;

        if (yyresult) {
            /* Avoid sprintf, as that infringes on the user's name space.
               Don't have undefined behavior even if the translation
               produced a string with the wrong number of "%s"s.  */
            char* yyp = yyresult;
            int yyi = 0;
            while ((*yyp = *yyf) != '\0') {
                if (*yyp == '%' && yyf[1] == 's' && yyi < yycount) {
                    yyp += yytnamerr(yyp, yyarg[yyi++]);
                    yyf += 2;
                } else {
                    yyp++;
                    yyf++;
                }
            }
        }
        return yysize;
    }
}
#endif /* YYERROR_VERBOSE */


/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yydestruct(const char* yymsg, int yytype, YYSTYPE* yyvaluep)
#else
static void
yydestruct(yymsg, yytype, yyvaluep)
const char* yymsg;
int yytype;
YYSTYPE* yyvaluep;
#endif
{
    YYUSE(yyvaluep);

    if (!yymsg)
        yymsg = "Deleting";
    YY_SYMBOL_PRINT(yymsg, yytype, yyvaluep, yylocationp);

    switch (yytype) {

    default:
        break;
    }
}

/* Prevent warnings from -Wmissing-prototypes.  */
#ifdef YYPARSE_PARAM
#if defined __STDC__ || defined __cplusplus
int yyparse(void* YYPARSE_PARAM);
#else
int yyparse();
#endif
#else /* ! YYPARSE_PARAM */
#if defined __STDC__ || defined __cplusplus
int yyparse(void);
#else
int yyparse();
#endif
#endif /* ! YYPARSE_PARAM */


/* The lookahead symbol.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;

/* Number of syntax errors so far.  */
int yynerrs;



/*-------------------------.
| yyparse or yypush_parse.  |
`-------------------------*/

#ifdef YYPARSE_PARAM
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse(void* YYPARSE_PARAM)
#else
int
yyparse(YYPARSE_PARAM)
void* YYPARSE_PARAM;
#endif
#else /* ! YYPARSE_PARAM */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse(void)
#else
int
yyparse()

#endif
#endif
{


    int yystate;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus;

    /* The stacks and their tools:
       `yyss': related to states.
       `yyvs': related to semantic values.

       Refer to the stacks thru separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* The state stack.  */
    yytype_int16 yyssa[YYINITDEPTH];
    yytype_int16* yyss;
    yytype_int16* yyssp;

    /* The semantic value stack.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE* yyvs;
    YYSTYPE* yyvsp;

    YYSIZE_T yystacksize;

    int yyn;
    int yyresult;
    /* Lookahead token as an internal (translated) token number.  */
    int yytoken;
    /* The variables used to return semantic value and location from the
       action routines.  */
    YYSTYPE yyval;

#if YYERROR_VERBOSE
    /* Buffer for error messages, and its allocated size.  */
    char yymsgbuf[128];
    char* yymsg = yymsgbuf;
    YYSIZE_T yymsg_alloc = sizeof yymsgbuf;
#endif

#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

    /* The number of symbols on the RHS of the reduced rule.
       Keep to zero when no symbol should be popped.  */
    int yylen = 0;

    yytoken = 0;
    yyss = yyssa;
    yyvs = yyvsa;
    yystacksize = YYINITDEPTH;

    YYDPRINTF((stderr, "Starting parse\n"));

    yystate = 0;
    yyerrstatus = 0;
    yynerrs = 0;
    yychar = YYEMPTY; /* Cause a token to be read.  */

    /* Initialize stack pointers.
       Waste one element of value and location stack
       so that they stay on the same level as the state stack.
       The wasted elements are never initialized.  */
    yyssp = yyss;
    yyvsp = yyvs;

    goto yysetstate;

    /*------------------------------------------------------------.
    | yynewstate -- Push a new state, which is found in yystate.  |
    `------------------------------------------------------------*/
yynewstate:
    /* In all cases, when you get here, the value and location stacks
       have just been pushed.  So pushing a state here evens the stacks.  */
    yyssp++;

yysetstate:
    *yyssp = yystate;

    if (yyss + yystacksize - 1 <= yyssp) {
        /* Get the current used size of the three stacks, in elements.  */
        YYSIZE_T yysize = yyssp - yyss + 1;

#ifdef yyoverflow
        {
            /* Give user a chance to reallocate the stack.  Use copies of
               these so that the &'s don't force the real ones into
               memory.  */
            YYSTYPE* yyvs1 = yyvs;
            yytype_int16* yyss1 = yyss;

            /* Each stack pointer address is followed by the size of the
               data in use in that stack, in bytes.  This used to be a
               conditional around just the two extra args, but that might
               be undefined if yyoverflow is a macro.  */
            yyoverflow(YY_("memory exhausted"),
                       &yyss1, yysize * sizeof(*yyssp),
                       &yyvs1, yysize * sizeof(*yyvsp),
                       &yystacksize);

            yyss = yyss1;
            yyvs = yyvs1;
        }
#else /* no yyoverflow */
# ifndef YYSTACK_RELOCATE
        goto yyexhaustedlab;
# else
        /* Extend the stack our own way.  */
        if (YYMAXDEPTH <= yystacksize)
            goto yyexhaustedlab;
        yystacksize *= 2;
        if (YYMAXDEPTH < yystacksize)
            yystacksize = YYMAXDEPTH;

        {
            yytype_int16* yyss1 = yyss;
            union yyalloc* yyptr =
                        (union yyalloc*) YYSTACK_ALLOC(YYSTACK_BYTES(yystacksize));
            if (! yyptr)
                goto yyexhaustedlab;
            YYSTACK_RELOCATE(yyss_alloc, yyss);
            YYSTACK_RELOCATE(yyvs_alloc, yyvs);
#  undef YYSTACK_RELOCATE
            if (yyss1 != yyssa)
                YYSTACK_FREE(yyss1);
        }
# endif
#endif /* no yyoverflow */

        yyssp = yyss + yysize - 1;
        yyvsp = yyvs + yysize - 1;

        YYDPRINTF((stderr, "Stack size increased to %lu\n",
                   (unsigned long int) yystacksize));

        if (yyss + yystacksize - 1 <= yyssp)
            YYABORT;
    }

    YYDPRINTF((stderr, "Entering state %d\n", yystate));

    if (yystate == YYFINAL)
        YYACCEPT;

    goto yybackup;

    /*-----------.
    | yybackup.  |
    `-----------*/
yybackup:

    /* Do appropriate processing given the current state.  Read a
       lookahead token if we need one and don't already have one.  */

    /* First try to decide what to do without reference to lookahead token.  */
    yyn = yypact[yystate];
    if (yyn == YYPACT_NINF)
        goto yydefault;

    /* Not known => get a lookahead token if don't already have one.  */

    /* YYCHAR is either YYEMPTY or YYEOF or a valid lookahead symbol.  */
    if (yychar == YYEMPTY) {
        YYDPRINTF((stderr, "Reading a token: "));
        yychar = YYLEX;
    }

    if (yychar <= YYEOF) {
        yychar = yytoken = YYEOF;
        YYDPRINTF((stderr, "Now at end of input.\n"));
    } else {
        yytoken = YYTRANSLATE(yychar);
        YY_SYMBOL_PRINT("Next token is", yytoken, &yylval, &yylloc);
    }

    /* If the proper action on seeing token YYTOKEN is to reduce or to
       detect an error, take that action.  */
    yyn += yytoken;
    if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
        goto yydefault;
    yyn = yytable[yyn];
    if (yyn <= 0) {
        if (yyn == 0 || yyn == YYTABLE_NINF)
            goto yyerrlab;
        yyn = -yyn;
        goto yyreduce;
    }

    /* Count tokens shifted since error; after three, turn off error
       status.  */
    if (yyerrstatus)
        yyerrstatus--;

    /* Shift the lookahead token.  */
    YY_SYMBOL_PRINT("Shifting", yytoken, &yylval, &yylloc);

    /* Discard the shifted token.  */
    yychar = YYEMPTY;

    yystate = yyn;
    *++yyvsp = yylval;

    goto yynewstate;


    /*-----------------------------------------------------------.
    | yydefault -- do the default action for the current state.  |
    `-----------------------------------------------------------*/
yydefault:
    yyn = yydefact[yystate];
    if (yyn == 0)
        goto yyerrlab;
    goto yyreduce;


    /*-----------------------------.
    | yyreduce -- Do a reduction.  |
    `-----------------------------*/
yyreduce:
    /* yyn is the number of a rule to reduce with.  */
    yylen = yyr2[yyn];

    /* If YYLEN is nonzero, implement the default value of the action:
       `$$ = $1'.

       Otherwise, the following line sets YYVAL to garbage.
       This behavior is undocumented and Bison
       users should not rely upon it.  Assigning to YYVAL
       unconditionally makes the parser a bit smaller, and it avoids a
       GCC warning that YYVAL may be used uninitialized.  */
    yyval = yyvsp[1 - yylen];


    YY_REDUCE_PRINT(yyn);
    switch (yyn) {
    case 2:

        /* Line 1455 of yacc.c  */
#line 202 "asn_ref_grammar.y"
        {  ;}
        break;

    case 3:

        /* Line 1455 of yacc.c  */
#line 204 "asn_ref_grammar.y"
        {  ;}
        break;

    case 4:

        /* Line 1455 of yacc.c  */
#line 208 "asn_ref_grammar.y"
        {
            Module = new ModuleDefinition(*(yyvsp[(1) - (7)].sval), (Tag::Mode)(yyvsp[(5) - (7)].ival));
            Modules.push_back(ModuleDefinitionPtr(Module));
            delete(yyvsp[(1) - (7)].sval);
            ;
        }
        break;

    case 5:

        /* Line 1455 of yacc.c  */
#line 214 "asn_ref_grammar.y"
        {
            Module = NULL;
            ;
        }
        break;

    case 6:

        /* Line 1455 of yacc.c  */
#line 218 "asn_ref_grammar.y"
        {
            Module = new ModuleDefinition(*(yyvsp[(1) - (5)].sval), (Tag::Mode)(yyvsp[(3) - (5)].ival));
            Modules.push_back(ModuleDefinitionPtr(Module));
            delete(yyvsp[(1) - (5)].sval);
            ;
        }
        break;

    case 7:

        /* Line 1455 of yacc.c  */
#line 224 "asn_ref_grammar.y"
        {
            Module = NULL;
            ;
        }
        break;

    case 8:

        /* Line 1455 of yacc.c  */
#line 232 "asn_ref_grammar.y"
        {
            (yyval.ival) = Tag::Explicit;
            ;
        }
        break;

    case 9:

        /* Line 1455 of yacc.c  */
#line 236 "asn_ref_grammar.y"
        {
            (yyval.ival) = Tag::Implicit;
            ;
        }
        break;

    case 10:

        /* Line 1455 of yacc.c  */
#line 240 "asn_ref_grammar.y"
        {
            (yyval.ival) = Tag::Automatic;
            ;
        }
        break;

    case 11:

        /* Line 1455 of yacc.c  */
#line 244 "asn_ref_grammar.y"
        {
            (yyval.ival) = Tag::Explicit;
            ;
        }
        break;

    case 16:

        /* Line 1455 of yacc.c  */
#line 266 "asn_ref_grammar.y"
        {  ;}
        break;

    case 17:

        /* Line 1455 of yacc.c  */
#line 268 "asn_ref_grammar.y"
        {  ;}
        break;

    case 24:

        /* Line 1455 of yacc.c  */
#line 291 "asn_ref_grammar.y"
        {
            Module->AddImportedIdentifiers(*(yyvsp[(1) - (3)].slst), *(yyvsp[(3) - (3)].sval));
            delete(yyvsp[(1) - (3)].slst);
            delete(yyvsp[(3) - (3)].sval);
            ;
        }
        break;

    case 25:

        /* Line 1455 of yacc.c  */
#line 301 "asn_ref_grammar.y"
        {
            (yyval.sval) = (yyvsp[(1) - (2)].sval);
            ;
        }
        break;

    case 29:

        /* Line 1455 of yacc.c  */
#line 315 "asn_ref_grammar.y"
        {
            (yyval.slst) = new StringList;
            (yyval.slst)->push_back(*(yyvsp[(1) - (1)].sval));
            delete(yyvsp[(1) - (1)].sval);
            ;
        }
        break;

    case 30:

        /* Line 1455 of yacc.c  */
#line 321 "asn_ref_grammar.y"
        {
            (yyvsp[(3) - (3)].slst)->push_back(*(yyvsp[(1) - (3)].sval));
            delete(yyvsp[(1) - (3)].sval);
            (yyval.slst) = (yyvsp[(3) - (3)].slst);
            ;
        }
        break;

    case 33:

        /* Line 1455 of yacc.c  */
#line 337 "asn_ref_grammar.y"
        {
            *(yyvsp[(1) - (3)].sval) += "{}";
            (yyval.sval) = (yyvsp[(1) - (3)].sval);
            ;
        }
        break;

    case 43:

        /* Line 1455 of yacc.c  */
#line 362 "asn_ref_grammar.y"
        {
            Module->AddIdentifier((yyvsp[(1) - (5)].sval), TYPEREFERENCE);
            ;
        }
        break;

    case 44:

        /* Line 1455 of yacc.c  */
#line 366 "asn_ref_grammar.y"
        {
            Module->AddIdentifier((yyvsp[(1) - (5)].sval), TYPEREFERENCE);
            ;
        }
        break;

    case 45:

        /* Line 1455 of yacc.c  */
#line 379 "asn_ref_grammar.y"
        {
            Module->AddIdentifier((yyvsp[(1) - (3)].sval), TYPEREFERENCE);
            ;
        }
        break;

    case 46:

        /* Line 1455 of yacc.c  */
#line 383 "asn_ref_grammar.y"
        {
            Module->AddIdentifier((yyvsp[(1) - (3)].sval), TYPEREFERENCE);
            ;
        }
        break;

    case 75:

        /* Line 1455 of yacc.c  */
#line 431 "asn_ref_grammar.y"
        {
            delete(yyvsp[(1) - (1)].sval);
            ;
        }
        break;

    case 77:

        /* Line 1455 of yacc.c  */
#line 441 "asn_ref_grammar.y"
        {
            delete(yyvsp[(1) - (3)].sval);
            delete(yyvsp[(3) - (3)].sval);
            ;
        }
        break;

    case 80:

        /* Line 1455 of yacc.c  */
#line 454 "asn_ref_grammar.y"
        {
            delete(yyvsp[(1) - (1)].sval);
            ;
        }
        break;

    case 121:

        /* Line 1455 of yacc.c  */
#line 590 "asn_ref_grammar.y"
        {
            delete(yyvsp[(1) - (3)].sval);
            ;
        }
        break;

    case 125:

        /* Line 1455 of yacc.c  */
#line 605 "asn_ref_grammar.y"
        {
            ;
        }
        break;

    case 126:

        /* Line 1455 of yacc.c  */
#line 611 "asn_ref_grammar.y"
        {
            delete(yyvsp[(1) - (1)].sval);
            ;
        }
        break;

    case 127:

        /* Line 1455 of yacc.c  */
#line 615 "asn_ref_grammar.y"
        {
            delete(yyvsp[(1) - (3)].sval);
            ;
        }
        break;

    case 128:

        /* Line 1455 of yacc.c  */
#line 619 "asn_ref_grammar.y"
        {
            delete(yyvsp[(1) - (1)].sval);
            ;
        }
        break;

    case 138:

        /* Line 1455 of yacc.c  */
#line 654 "asn_ref_grammar.y"
        {
            Module->AddIdentifier((yyvsp[(1) - (3)].sval), OBJECTCLASSREFERENCE);
            ;
        }
        break;

    case 139:

        /* Line 1455 of yacc.c  */
#line 661 "asn_ref_grammar.y"
        {
            Module->AddIdentifier((yyvsp[(1) - (4)].sval), OBJECTREFERENCE);
            ;
        }
        break;

    case 140:

        /* Line 1455 of yacc.c  */
#line 668 "asn_ref_grammar.y"
        {
            Module->AddIdentifier((yyvsp[(1) - (4)].sval), OBJECTSETREFERENCE);
            ;
        }
        break;

    case 141:

        /* Line 1455 of yacc.c  */
#line 672 "asn_ref_grammar.y"
        {
            Module->AddIdentifier((yyvsp[(1) - (4)].sval), OBJECTSETREFERENCE);
            ;
        }
        break;

    case 146:

        /* Line 1455 of yacc.c  */
#line 687 "asn_ref_grammar.y"
        {
            delete(yyvsp[(1) - (1)].sval);
            ;
        }
        break;

    case 148:

        /* Line 1455 of yacc.c  */
#line 696 "asn_ref_grammar.y"
        {
            delete(yyvsp[(1) - (3)].sval);
            delete(yyvsp[(3) - (3)].sval);
            ;
        }
        break;

    case 155:

        /* Line 1455 of yacc.c  */
#line 720 "asn_ref_grammar.y"
        {
            delete(yyvsp[(1) - (1)].sval);
            ;
        }
        break;

    case 156:

        /* Line 1455 of yacc.c  */
#line 727 "asn_ref_grammar.y"
        {
            delete(yyvsp[(1) - (3)].sval);
            delete(yyvsp[(3) - (3)].sval);
            ;
        }
        break;

    case 158:

        /* Line 1455 of yacc.c  */
#line 742 "asn_ref_grammar.y"
        {
            delete(yyvsp[(3) - (3)].sval);
            ;
        }
        break;

    case 159:

        /* Line 1455 of yacc.c  */
#line 746 "asn_ref_grammar.y"
        {
            delete(yyvsp[(1) - (1)].sval);
            ;
        }
        break;

    case 170:

        /* Line 1455 of yacc.c  */
#line 781 "asn_ref_grammar.y"
        {
            Module->AddIdentifier((yyvsp[(1) - (3)].sval), PARAMETERIZEDTYPEREFERENCE);
            ;
        }
        break;

    case 171:

        /* Line 1455 of yacc.c  */
#line 785 "asn_ref_grammar.y"
        {
            Module->AddIdentifier((yyvsp[(1) - (3)].sval), PARAMETERIZEDTYPEREFERENCE);
            ;
        }
        break;

    case 172:

        /* Line 1455 of yacc.c  */
#line 793 "asn_ref_grammar.y"
        {
            Module->AddIdentifier((yyvsp[(1) - (4)].sval), PARAMETERIZEDVALUEREFERENCE);
            ;
        }
        break;

    case 173:

        /* Line 1455 of yacc.c  */
#line 800 "asn_ref_grammar.y"
        {
            Module->AddIdentifier((yyvsp[(1) - (5)].sval), PARAMETERIZEDTYPEREFERENCE);
            ;
        }
        break;

    case 174:

        /* Line 1455 of yacc.c  */
#line 804 "asn_ref_grammar.y"
        {
            Module->AddIdentifier((yyvsp[(1) - (5)].sval), PARAMETERIZEDTYPEREFERENCE);
            ;
        }
        break;

    case 175:

        /* Line 1455 of yacc.c  */
#line 811 "asn_ref_grammar.y"
        {
            Module->AddIdentifier((yyvsp[(1) - (3)].sval), PARAMETERIZEDOBJECTCLASSREFERENCE);
            ;
        }
        break;

    case 176:

        /* Line 1455 of yacc.c  */
#line 818 "asn_ref_grammar.y"
        {
            Module->AddIdentifier((yyvsp[(1) - (4)].sval), PARAMETERIZEDOBJECTREFERENCE);
            ;
        }
        break;

    case 177:

        /* Line 1455 of yacc.c  */
#line 825 "asn_ref_grammar.y"
        {
            Module->AddIdentifier((yyvsp[(1) - (4)].sval), PARAMETERIZEDOBJECTSETREFERENCE);
            ;
        }
        break;

    case 178:

        /* Line 1455 of yacc.c  */
#line 829 "asn_ref_grammar.y"
        {
            Module->AddIdentifier((yyvsp[(1) - (4)].sval), PARAMETERIZEDOBJECTSETREFERENCE);
            ;
        }
        break;

    case 183:

        /* Line 1455 of yacc.c  */
#line 855 "asn_ref_grammar.y"
        {
            Module->AddIdentifier((yyvsp[(1) - (4)].sval), VALUEREFERENCE);
            ;
        }
        break;

    case 196:

        /* Line 1455 of yacc.c  */
#line 896 "asn_ref_grammar.y"
        {
            delete(yyvsp[(1) - (1)].sval);
            ;
        }
        break;

    case 198:

        /* Line 1455 of yacc.c  */
#line 905 "asn_ref_grammar.y"
        {
            delete(yyvsp[(1) - (3)].sval);
            delete(yyvsp[(3) - (3)].sval);
            ;
        }
        break;

    case 206:

        /* Line 1455 of yacc.c  */
#line 941 "asn_ref_grammar.y"
        {
            delete(yyvsp[(1) - (3)].sval);
            ;
        }
        break;

    case 219:

        /* Line 1455 of yacc.c  */
#line 988 "asn_ref_grammar.y"
        {
            delete(yyvsp[(1) - (1)].sval);
            ;
        }
        break;



        /* Line 1455 of yacc.c  */
#line 2515 "asn_ref_grammar.tab.c"
    default: break;
    }
    YY_SYMBOL_PRINT("-> $$ =", yyr1[yyn], &yyval, &yyloc);

    YYPOPSTACK(yylen);
    yylen = 0;
    YY_STACK_PRINT(yyss, yyssp);

    *++yyvsp = yyval;

    /* Now `shift' the result of the reduction.  Determine what state
       that goes to, based on the state we popped back to and the rule
       number reduced by.  */

    yyn = yyr1[yyn];

    yystate = yypgoto[yyn - YYNTOKENS] + *yyssp;
    if (0 <= yystate && yystate <= YYLAST && yycheck[yystate] == *yyssp)
        yystate = yytable[yystate];
    else
        yystate = yydefgoto[yyn - YYNTOKENS];

    goto yynewstate;


    /*------------------------------------.
    | yyerrlab -- here on detecting error |
    `------------------------------------*/
yyerrlab:
    /* If not already recovering from an error, report this error.  */
    if (!yyerrstatus) {
        ++yynerrs;
#if ! YYERROR_VERBOSE
        yyerror(YY_("syntax error"));
#else
        {
            YYSIZE_T yysize = yysyntax_error(0, yystate, yychar);
            if (yymsg_alloc < yysize && yymsg_alloc < YYSTACK_ALLOC_MAXIMUM) {
                YYSIZE_T yyalloc = 2 * yysize;
                if (!(yysize <= yyalloc && yyalloc <= YYSTACK_ALLOC_MAXIMUM))
                    yyalloc = YYSTACK_ALLOC_MAXIMUM;
                if (yymsg != yymsgbuf)
                    YYSTACK_FREE(yymsg);
                yymsg = (char*) YYSTACK_ALLOC(yyalloc);
                if (yymsg)
                    yymsg_alloc = yyalloc;
                else {
                    yymsg = yymsgbuf;
                    yymsg_alloc = sizeof yymsgbuf;
                }
            }

            if (0 < yysize && yysize <= yymsg_alloc) {
                (void) yysyntax_error(yymsg, yystate, yychar);
                yyerror(yymsg);
            } else {
                yyerror(YY_("syntax error"));
                if (yysize != 0)
                    goto yyexhaustedlab;
            }
        }
#endif
    }



    if (yyerrstatus == 3) {
        /* If just tried and failed to reuse lookahead token after an
        error, discard it.  */

        if (yychar <= YYEOF) {
            /* Return failure if at end of input.  */
            if (yychar == YYEOF)
                YYABORT;
        } else {
            yydestruct("Error: discarding",
                       yytoken, &yylval);
            yychar = YYEMPTY;
        }
    }

    /* Else will try to reuse lookahead token after shifting the error
       token.  */
    goto yyerrlab1;


    /*---------------------------------------------------.
    | yyerrorlab -- error raised explicitly by YYERROR.  |
    `---------------------------------------------------*/
yyerrorlab:

    /* Pacify compilers like GCC when the user code never invokes
       YYERROR and the label yyerrorlab therefore never appears in user
       code.  */
    if (/*CONSTCOND*/ 0)
        goto yyerrorlab;

    /* Do not reclaim the symbols of the rule which action triggered
       this YYERROR.  */
    YYPOPSTACK(yylen);
    yylen = 0;
    YY_STACK_PRINT(yyss, yyssp);
    yystate = *yyssp;
    goto yyerrlab1;


    /*-------------------------------------------------------------.
    | yyerrlab1 -- common code for both syntax error and YYERROR.  |
    `-------------------------------------------------------------*/
yyerrlab1:
    yyerrstatus = 3;  /* Each real token shifted decrements this.  */

    for (;;) {
        yyn = yypact[yystate];
        if (yyn != YYPACT_NINF) {
            yyn += YYTERROR;
            if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYTERROR) {
                yyn = yytable[yyn];
                if (0 < yyn)
                    break;
            }
        }

        /* Pop the current state because it cannot handle the error token.  */
        if (yyssp == yyss)
            YYABORT;


        yydestruct("Error: popping",
                   yystos[yystate], yyvsp);
        YYPOPSTACK(1);
        yystate = *yyssp;
        YY_STACK_PRINT(yyss, yyssp);
    }

    *++yyvsp = yylval;


    /* Shift the error token.  */
    YY_SYMBOL_PRINT("Shifting", yystos[yyn], yyvsp, yylsp);

    yystate = yyn;
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

#if !defined(yyoverflow) || YYERROR_VERBOSE
    /*-------------------------------------------------.
    | yyexhaustedlab -- memory exhaustion comes here.  |
    `-------------------------------------------------*/
yyexhaustedlab:
    yyerror(YY_("memory exhausted"));
    yyresult = 2;
    /* Fall through.  */
#endif

yyreturn:
    if (yychar != YYEMPTY)
        yydestruct("Cleanup: discarding lookahead",
                   yytoken, &yylval);
    /* Do not reclaim the symbols of the rule which action triggered
       this YYABORT or YYACCEPT.  */
    YYPOPSTACK(yylen);
    YY_STACK_PRINT(yyss, yyssp);
    while (yyssp != yyss) {
        yydestruct("Cleanup: popping",
                   yystos[*yyssp], yyvsp);
        YYPOPSTACK(1);
    }
#ifndef yyoverflow
    if (yyss != yyssa)
        YYSTACK_FREE(yyss);
#endif
#if YYERROR_VERBOSE
    if (yymsg != yymsgbuf)
        YYSTACK_FREE(yymsg);
#endif
    /* Make sure YYID is used.  */
    return YYID(yyresult);
}



