
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



/* Copy the first part of user declarations.  */

/* Line 189 of yacc.c  */
#line 3 "asn_grammar.y"

#ifdef _MSC_VER
#pragma warning(disable:4244 4065 4702)
#endif

/*
 * asn_grammar.y
 *
 * ASN grammar file
 *
 * ASN.1 compiler to produce C++ classes.
 *
 * Copyright (c) 1997-1999 Equivalence Pty. Ltd.
 *
 * Copyright (c) 2001 Institute for Information Industry, Taiwan, Republic of China
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
 * The Initial Developer of the Original Code is Equivalence Pty. Ltd.
 *
 * Portions of this code were written with the assisance of funding from
 * Vovida Networks, Inc. http://www.vovida.com.
 *
 * Portions are Copyright (C) 1993 Free Software Foundation, Inc.
 * All Rights Reserved.
 *
 * Contributor(s): ___________________.
 *
 * $Log: asn_grammar.y,v $
 * Revision 1.3  2002/07/02 02:03:25  mangelo
 * Remove Pwlib dependency
 *
 * Revision 1.2  2001/09/07 22:38:10  mangelo
 * add Log keyword substitution
 *
 *
 * March, 2001. Huang-Ming Huang
 *            Add support for Information Object Class.
 *
 */

#undef malloc
#undef calloc
#undef realloc
#undef free

#include "main.h"
#include <sstream>


extern int yylex();

extern int ReferenceTokenContext;
extern int IdentifierTokenContext;
extern int BraceTokenContext;
extern int NullTokenContext;
extern int InMacroContext;
extern int HasObjectTypeMacro;
extern int InMIBContext;
extern int InOIDContext ;

static int UnnamedFieldCount = 1;
extern int InWithSyntaxContext;
extern int InObjectSetContext;
extern const ObjectClassBase* InformationFromObjectContext;
extern TypePtr ValueTypeContext;
extern int ParsingConstructedType;


static std::string* ConcatNames(std::string* s1, char c, std::string* s2) {
    *s1 += c;
    *s1 += *s2;
    delete s2;
    return s1;
}

#ifdef _MSC_VER
#pragma warning(disable:4701)
#endif



/* Line 189 of yacc.c  */
#line 165 "asn_grammar.tab.c"

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
    VALUEREFERENCE = 261,
    OBJECTREFERENCE = 262,
    OBJECTSETREFERENCE = 263,
    PARAMETERIZEDTYPEREFERENCE = 264,
    PARAMETERIZEDOBJECTCLASSREFERENCE = 265,
    PARAMETERIZEDVALUEREFERENCE = 266,
    PARAMETERIZEDOBJECTREFERENCE = 267,
    PARAMETERIZEDOBJECTSETREFERENCE = 268,
    VALUESET_BRACE = 269,
    OBJECT_BRACE = 270,
    OBJECTSET_BRACE = 271,
    IDENTIFIER = 272,
    BIT_IDENTIFIER = 273,
    OID_IDENTIFIER = 274,
    IMPORT_IDENTIFIER = 275,
    fieldreference = 276,
    FieldReference = 277,
    TYPEFIELDREFERENCE = 278,
    FIXEDTYPEVALUEFIELDREFERENCE = 279,
    VARIABLETYPEVALUEFIELDREFERENCE = 280,
    FIXEDTYPEVALUESETFIELDREFERENCE = 281,
    VARIABLETYPEVALUESETFIELDREFERENCE = 282,
    OBJECTFIELDREFERENCE = 283,
    OBJECTSETFIELDREFERENCE = 284,
    INTEGER = 285,
    CSTRING = 286,
    OS_BSTRING = 287,
    BS_BSTRING = 288,
    OS_HSTRING = 289,
    BS_HSTRING = 290,
    ABSENT = 291,
    ABSTRACT_SYNTAX = 292,
    ALL = 293,
    ANY = 294,
    APPLICATION = 295,
    ASSIGNMENT = 296,
    AUTOMATIC = 297,
    BEGIN_t = 298,
    BIT = 299,
    BMPString = 300,
    BOOLEAN_t = 301,
    BY = 302,
    CHARACTER = 303,
    CHOICE = 304,
    CLASS = 305,
    COMPONENT = 306,
    COMPONENTS = 307,
    CONSTRAINED = 308,
    DEFAULT = 309,
    DEFINED = 310,
    DEFINITIONS = 311,
    EMBEDDED = 312,
    END = 313,
    ENUMERATED = 314,
    EXCEPT = 315,
    EXPLICIT = 316,
    EXPORTS = 317,
    EXTERNAL = 318,
    FALSE_t = 319,
    FROM = 320,
    GeneralString = 321,
    GraphicString = 322,
    IA5String = 323,
    TYPE_IDENTIFIER = 324,
    IDENTIFIER_t = 325,
    IMPLICIT = 326,
    IMPORTS = 327,
    INCLUDES = 328,
    INSTANCE = 329,
    INTEGER_t = 330,
    INTERSECTION = 331,
    ISO646String = 332,
    MACRO = 333,
    MAX = 334,
    MIN = 335,
    MINUS_INFINITY = 336,
    NOTATION = 337,
    NULL_VALUE = 338,
    NULL_TYPE = 339,
    NumericString = 340,
    OBJECT = 341,
    OCTET = 342,
    OF_t = 343,
    OPTIONAL_t = 344,
    PDV = 345,
    PLUS_INFINITY = 346,
    PRESENT = 347,
    PrintableString = 348,
    PRIVATE = 349,
    REAL = 350,
    SEQUENCE = 351,
    SET = 352,
    SIZE_t = 353,
    STRING = 354,
    SYNTAX = 355,
    T61String = 356,
    TAGS = 357,
    TeletexString = 358,
    TRUE_t = 359,
    TYPE_t = 360,
    UNION = 361,
    UNIQUE = 362,
    UNIVERSAL = 363,
    UniversalString = 364,
    VideotexString = 365,
    VisibleString = 366,
    GeneralizedTime = 367,
    UTCTime = 368,
    VALUE = 369,
    WITH = 370,
    ObjectDescriptor_t = 371,
    WORD_t = 372,
    OID_INTEGER = 373
};
#endif



#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE {

    /* Line 214 of yacc.c  */
#line 428 "asn_grammar.y"

    boost::int64_t       ival;
    std::string*            sval;
    StringList*         slst;
    TypeBase*           tval;
    TypePtr*            tptr;
    TypesVector*        tlst;
    ValueBase*          vval;
    ValuesList*             vlst;
    NamedNumber*            nval;
    NamedNumberList*        nlst;
    Constraint*             cons;
    ConstraintElementVector* elst;
    ConstraintElementBase* elmt;
    FieldSpec*              fspc;
    FieldSpecsList*         flst;
    ObjectClassBase*        objc;
    DefinedObjectClass*     dobj;
    TokenGroup*             tgrp;
    TokenOrGroupSpec*       togs;
    Setting*                sett;
    InformationObject*      objt;
    InformationObjectSet*   objs;
    FieldSettingList*       fldl;
    DefaultSyntaxBuilder*   bldr;
    DefinedSyntaxToken*     tken;
    ValueSet*               vset;
    ObjectSetConstraintElement* osce;
    Symbol*                 symb;
    SymbolList*             syml;
    Parameter*              para;
    ParameterList*          plst;
    ActualParameter*        apar;
    ActualParameterList*    aplt;
    TableConstraint*        tcons;
    ObjectClassFieldType*   ocft;
    DefinedObjectSet*       dos;

    struct {
        Tag::Type tagClass;
        unsigned tagNumber;
    } tagv;



    /* Line 214 of yacc.c  */
#line 365 "asn_grammar.tab.c"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif


/* Copy the second part of user declarations.  */


/* Line 264 of yacc.c  */
#line 377 "asn_grammar.tab.c"

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
#define YYLAST   3611

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  137
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  243
/* YYNRULES -- Number of rules.  */
#define YYNRULES  519
/* YYNRULES -- Number of states.  */
#define YYNSTATES  813

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   373

#define YYTRANSLATE(YYX)                        \
    ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const yytype_uint8 yytranslate[] = {
    0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
    2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
    2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
    2,     2,     2,   129,     2,     2,     2,     2,     2,     2,
    121,   122,     2,     2,   124,   136,   125,     2,   134,     2,
    2,     2,     2,     2,     2,     2,     2,     2,   130,   123,
    128,     2,   135,     2,   133,     2,     2,     2,     2,     2,
    2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
    2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
    2,   126,     2,   127,   132,     2,     2,     2,     2,     2,
    2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
    2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
    2,     2,     2,   119,   131,   120,     2,     2,     2,     2,
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
    115,   116,   117,   118
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const yytype_uint16 yyprhs[] = {
    0,     0,     3,     6,     8,     9,    19,    23,    24,    26,
    29,    31,    33,    35,    40,    43,    46,    49,    50,    54,
    55,    59,    60,    62,    63,    67,    68,    70,    71,    73,
    76,    80,    81,    85,    87,    89,    90,    92,    96,    98,
    100,   102,   104,   106,   110,   114,   118,   122,   126,   128,
    131,   133,   135,   137,   139,   141,   143,   145,   146,   152,
    156,   158,   160,   162,   164,   166,   168,   170,   172,   174,
    176,   178,   180,   182,   184,   186,   188,   190,   192,   194,
    196,   198,   200,   202,   204,   206,   208,   210,   212,   214,
    216,   220,   223,   229,   231,   235,   240,   245,   247,   249,
    251,   253,   255,   257,   259,   261,   263,   265,   267,   269,
    271,   273,   275,   278,   279,   285,   287,   291,   297,   299,
    303,   308,   311,   313,   316,   321,   323,   329,   337,   339,
    343,   345,   347,   349,   351,   356,   360,   362,   367,   369,
    371,   377,   383,   384,   389,   392,   395,   397,   401,   404,
    408,   409,   413,   415,   419,   425,   429,   431,   435,   437,
    440,   441,   446,   450,   454,   458,   461,   462,   466,   470,
    473,   477,   481,   486,   488,   490,   492,   494,   496,   497,
    501,   503,   505,   507,   510,   513,   516,   518,   521,   524,
    526,   528,   530,   532,   534,   536,   538,   541,   546,   549,
    551,   556,   561,   566,   571,   576,   578,   580,   583,   584,
    586,   588,   589,   594,   596,   602,   608,   616,   618,   621,
    623,   627,   629,   633,   635,   638,   641,   643,   645,   647,
    649,   651,   653,   657,   659,   661,   663,   665,   667,   669,
    674,   676,   679,   681,   684,   686,   688,   690,   692,   695,
    698,   700,   702,   704,   707,   711,   715,   719,   727,   729,
    733,   736,   740,   742,   744,   746,   747,   753,   754,   756,
    760,   762,   766,   768,   770,   772,   774,   776,   780,   787,
    791,   793,   796,   800,   804,   806,   810,   811,   817,   818,
    824,   826,   828,   830,   832,   834,   838,   840,   842,   848,
    852,   854,   856,   858,   860,   862,   864,   866,   868,   870,
    873,   877,   880,   884,   888,   889,   895,   900,   901,   908,
    911,   915,   918,   922,   923,   929,   932,   936,   939,   943,
    944,   950,   953,   957,   958,   964,   965,   970,   971,   975,
    978,   981,   983,   985,   987,   991,   993,   995,   997,   999,
    1001,  1003,  1005,  1009,  1011,  1013,  1017,  1020,  1025,  1029,
    1031,  1033,  1035,  1037,  1039,  1041,  1043,  1045,  1047,  1049,
    1051,  1055,  1056,  1062,  1063,  1067,  1071,  1074,  1075,  1077,
    1079,  1081,  1083,  1085,  1087,  1089,  1093,  1095,  1099,  1101,
    1103,  1105,  1107,  1108,  1112,  1113,  1117,  1119,  1121,  1123,
    1125,  1127,  1128,  1134,  1135,  1142,  1143,  1150,  1151,  1158,
    1159,  1166,  1170,  1174,  1176,  1180,  1184,  1186,  1189,  1194,
    1196,  1198,  1202,  1206,  1208,  1210,  1212,  1216,  1218,  1222,
    1224,  1226,  1228,  1230,  1231,  1237,  1239,  1241,  1243,  1245,
    1247,  1249,  1251,  1253,  1255,  1257,  1259,  1261,  1263,  1265,
    1269,  1273,  1275,  1278,  1280,  1282,  1287,  1289,  1291,  1293,
    1295,  1297,  1299,  1301,  1305,  1307,  1311,  1313,  1315,  1317,
    1319,  1321,  1323,  1325,  1329,  1331,  1335,  1337,  1339,  1349,
    1355,  1359,  1361,  1363,  1365,  1367,  1369,  1371,  1375,  1378,
    1380,  1384,  1387,  1389,  1391,  1394,  1396,  1398,  1401,  1403,
    1405,  1408,  1411,  1415,  1417,  1419,  1421,  1423,  1425,  1427,
    1429,  1431,  1433,  1435,  1437,  1439,  1443,  1448,  1453,  1455
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int16 yyrhs[] = {
    138,     0,    -1,   138,   139,    -1,   139,    -1,    -1,     3,
    141,    56,   145,    41,    43,   140,   146,    58,    -1,   119,
    142,   120,    -1,    -1,   143,    -1,   143,   142,    -1,    17,
    -1,    30,    -1,   144,    -1,    17,   121,    30,   122,    -1,
    61,   102,    -1,    71,   102,    -1,    42,   102,    -1,    -1,
    147,   149,   158,    -1,    -1,    62,   148,   123,    -1,    -1,
    156,    -1,    -1,    72,   150,   123,    -1,    -1,   151,    -1,
    -1,   152,    -1,   151,   152,    -1,   156,    65,   153,    -1,
    -1,     3,   154,   155,    -1,   343,    -1,   345,    -1,    -1,
    157,    -1,   157,   124,   156,    -1,     4,    -1,     6,    -1,
    5,    -1,     7,    -1,     8,    -1,     9,   119,   120,    -1,
    11,   119,   120,    -1,    10,   119,   120,    -1,    12,   119,
    120,    -1,    13,   119,   120,    -1,   159,    -1,   158,   159,
    -1,   162,    -1,   339,    -1,   160,    -1,   262,    -1,   263,
    -1,   265,    -1,   318,    -1,    -1,     4,   163,   161,    41,
    375,    -1,     4,    41,   163,    -1,   221,    -1,   165,    -1,
    164,    -1,   168,    -1,   171,    -1,   172,    -1,   175,    -1,
    181,    -1,   182,    -1,   186,    -1,   187,    -1,   188,    -1,
    189,    -1,   190,    -1,   191,    -1,   194,    -1,   195,    -1,
    196,    -1,   197,    -1,   204,    -1,   205,    -1,   208,    -1,
    209,    -1,   166,    -1,   214,    -1,   213,    -1,   215,    -1,
    216,    -1,   333,    -1,   332,    -1,     3,   125,     4,    -1,
    44,    99,    -1,    44,    99,   119,   169,   120,    -1,   170,
    -1,   169,   124,   170,    -1,    17,   121,    30,   122,    -1,
    17,   121,   343,   122,    -1,    46,    -1,   173,    -1,   174,
    -1,    45,    -1,    66,    -1,    67,    -1,    68,    -1,    77,
    -1,    85,    -1,    93,    -1,   103,    -1,   101,    -1,   109,
    -1,   110,    -1,   111,    -1,    48,    99,    -1,    -1,    49,
    119,   176,   177,   120,    -1,   178,    -1,   178,   124,   179,
    -1,   178,   124,   179,   124,   178,    -1,   180,    -1,   178,
    124,   180,    -1,   125,   125,   125,   225,    -1,    17,   163,
    -1,   163,    -1,    57,    90,    -1,    59,   119,   183,   120,
    -1,   184,    -1,   184,   124,   125,   125,   125,    -1,   184,
    124,   125,   125,   125,   124,   184,    -1,   185,    -1,   184,
    124,   185,    -1,    17,    -1,   378,    -1,    63,    -1,    39,
    -1,    39,    55,    47,    17,    -1,    74,    88,   268,    -1,
    75,    -1,    75,   119,   377,   120,    -1,    84,    -1,   192,
    -1,   192,   121,   256,   225,   122,    -1,   192,   121,   224,
    225,   122,    -1,    -1,   268,   193,   125,   299,    -1,    86,
    70,    -1,    87,    99,    -1,    95,    -1,   198,   200,   120,
    -1,   198,   120,    -1,   198,   179,   120,    -1,    -1,    96,
    199,   119,    -1,   201,    -1,   201,   124,   179,    -1,   201,
    124,   179,   124,   201,    -1,   179,   124,   201,    -1,   202,
    -1,   201,   124,   202,    -1,   180,    -1,   180,    89,    -1,
    -1,   180,   203,    54,   341,    -1,    52,    88,   163,    -1,
    96,    88,   163,    -1,   206,   200,   120,    -1,   206,   120,
    -1,    -1,    97,   207,   119,    -1,    97,    88,   163,    -1,
    210,   163,    -1,   210,    71,   163,    -1,   210,    61,   163,
    -1,   126,   212,   211,   127,    -1,    30,    -1,   343,    -1,
    108,    -1,    40,    -1,    94,    -1,    -1,    17,   128,   163,
    -1,   112,    -1,   113,    -1,   116,    -1,   316,    23,    -1,
    314,    24,    -1,   314,    26,    -1,   371,    -1,   314,    28,
    -1,   314,    29,    -1,   374,    -1,   296,    -1,   217,    -1,
    298,    -1,   294,    -1,   373,    -1,   220,    -1,    12,   334,
    -1,     3,   125,    12,   334,    -1,   163,   223,    -1,   222,
    -1,    97,   223,    88,   163,    -1,    97,   245,    88,   163,
    -1,    96,   223,    88,   163,    -1,    96,   245,    88,   163,
    -1,   121,   224,   225,   122,    -1,   228,    -1,   251,    -1,
    129,   226,    -1,    -1,   379,    -1,   343,    -1,    -1,   163,
    227,   130,   341,    -1,   229,    -1,   229,   124,   125,   125,
    125,    -1,   125,   125,   125,   124,   229,    -1,   229,   124,
    125,   125,   125,   124,   229,    -1,   230,    -1,    38,   233,
    -1,   231,    -1,   230,   234,   231,    -1,   232,    -1,   231,
    235,   232,    -1,   236,    -1,   236,   233,    -1,    60,   236,
    -1,   131,    -1,   106,    -1,   132,    -1,    76,    -1,   237,
    -1,   313,    -1,   121,   229,   122,    -1,   341,    -1,   244,
    -1,   238,    -1,   243,    -1,   245,    -1,   246,    -1,   239,
    125,   125,   240,    -1,   241,    -1,   241,   128,    -1,   242,
    -1,   128,   242,    -1,   341,    -1,    80,    -1,   341,    -1,
    79,    -1,    65,   223,    -1,    73,   163,    -1,   221,    -1,
    164,    -1,   165,    -1,    98,   223,    -1,   115,    51,   223,
    -1,   115,    52,   247,    -1,   119,   248,   120,    -1,   119,
    125,   125,   125,   124,   248,   120,    -1,   249,    -1,   249,
    124,   248,    -1,    17,   250,    -1,    17,   223,   250,    -1,
    92,    -1,    36,    -1,    89,    -1,    -1,    53,    47,   119,
    252,   120,    -1,    -1,   253,    -1,   254,   124,   253,    -1,
    254,    -1,   255,   130,   336,    -1,   336,    -1,   163,    -1,
    268,    -1,   257,    -1,   258,    -1,   119,   296,   120,    -1,
    119,   296,   120,   119,   259,   120,    -1,   259,   124,   260,
    -1,   260,    -1,   133,   261,    -1,   133,   125,   261,    -1,
    261,   125,    17,    -1,    17,    -1,     5,    41,   267,    -1,
    -1,     7,   268,   264,    41,   301,    -1,    -1,     8,   268,
    266,    41,   311,    -1,   268,    -1,   271,    -1,     5,    -1,
    269,    -1,   270,    -1,     3,   125,     5,    -1,    69,    -1,
    37,    -1,    50,   119,   272,   120,   286,    -1,   272,   124,
    273,    -1,   273,    -1,   274,    -1,   275,    -1,   278,    -1,
    279,    -1,   281,    -1,   282,    -1,   284,    -1,    22,    -1,
    22,    89,    -1,    22,    54,   163,    -1,    21,   163,    -1,
    21,   163,   107,    -1,    21,   163,    89,    -1,    -1,    21,
    163,   276,    54,   341,    -1,    21,   163,   107,    89,    -1,
    -1,    21,   163,   107,   277,    54,   341,    -1,    21,    22,
    -1,    21,    22,    89,    -1,    22,   163,    -1,    22,   163,
    89,    -1,    -1,    22,   163,   280,    54,   375,    -1,    22,
    22,    -1,    22,    22,    89,    -1,    21,   268,    -1,    21,
    268,    89,    -1,    -1,    21,   268,   283,    54,   301,    -1,
    22,   268,    -1,    22,   268,    89,    -1,    -1,    22,   268,
    285,    54,   311,    -1,    -1,   115,   100,   287,   288,    -1,
    -1,   119,   289,   120,    -1,   119,   120,    -1,   289,   290,
    -1,   290,    -1,   292,    -1,   291,    -1,   126,   289,   127,
    -1,   293,    -1,    22,    -1,    21,    -1,   117,    -1,   124,
    -1,     7,    -1,   295,    -1,     3,   125,     7,    -1,     8,
    -1,   297,    -1,     3,   125,     8,    -1,    13,   334,    -1,
    3,   125,    13,   334,    -1,   299,   125,   300,    -1,   300,
    -1,    23,    -1,   370,    -1,   372,    -1,    28,    -1,    29,
    -1,   294,    -1,   302,    -1,   373,    -1,   307,    -1,   303,
    -1,    15,   304,   120,    -1,    -1,   304,   124,   300,   305,
    310,    -1,    -1,   300,   306,   310,    -1,    15,   308,   120,
    -1,   308,   309,    -1,    -1,   293,    -1,   310,    -1,   163,
    -1,   341,    -1,   375,    -1,   301,    -1,   311,    -1,    16,
    312,   120,    -1,   228,    -1,   125,   125,   125,    -1,   301,
    -1,   296,    -1,   217,    -1,   298,    -1,    -1,   218,   315,
    125,    -1,    -1,   219,   317,   125,    -1,   319,    -1,   321,
    -1,   323,    -1,   325,    -1,   327,    -1,    -1,     9,   329,
    320,    41,   163,    -1,    -1,    11,   329,   163,   322,    41,
    341,    -1,    -1,     9,   329,   163,   324,    41,   375,    -1,
    -1,    12,   329,   268,   326,    41,   301,    -1,    -1,    13,
    329,   268,   328,    41,   311,    -1,   119,   330,   120,    -1,
    330,   124,   331,    -1,   331,    -1,   163,   130,   376,    -1,
    268,   130,   376,    -1,   376,    -1,     9,   334,    -1,     3,
    125,     9,   334,    -1,     4,    -1,   167,    -1,   119,   335,
    120,    -1,   335,   124,   336,    -1,   336,    -1,   163,    -1,
    341,    -1,   119,   166,   120,    -1,   337,    -1,   119,   338,
    120,    -1,   294,    -1,   220,    -1,   296,    -1,   298,    -1,
    -1,     6,   163,   340,    41,   341,    -1,   342,    -1,   368,
    -1,   350,    -1,   352,    -1,   353,    -1,   360,    -1,   379,
    -1,   361,    -1,   345,    -1,   349,    -1,   362,    -1,   365,
    -1,     6,    -1,   344,    -1,     3,   125,     6,    -1,   119,
    346,   120,    -1,   347,    -1,   346,   347,    -1,    19,    -1,
    118,    -1,    19,   121,   348,   122,    -1,   118,    -1,   344,
    -1,     6,    -1,    32,    -1,    34,    -1,    33,    -1,    35,
    -1,   119,   351,   120,    -1,    18,    -1,   351,   124,    18,
    -1,   104,    -1,    64,    -1,   354,    -1,    31,    -1,   355,
    -1,   358,    -1,   359,    -1,   119,   356,   120,    -1,   357,
    -1,   356,   124,   357,    -1,    31,    -1,   343,    -1,   119,
    30,   124,    30,   124,    30,   124,    30,   120,    -1,   119,
    30,   124,    30,   120,    -1,    17,   130,   341,    -1,    83,
    -1,   363,    -1,   364,    -1,   134,    -1,    91,    -1,    81,
    -1,   119,   366,   120,    -1,   119,   120,    -1,   367,    -1,
    366,   124,   367,    -1,    17,   341,    -1,   343,    -1,   369,
    -1,   316,   370,    -1,    24,    -1,    25,    -1,   316,   372,
    -1,    26,    -1,    27,    -1,   316,    28,    -1,   316,    29,
    -1,    14,   228,   120,    -1,     4,    -1,     6,    -1,     5,
    -1,     7,    -1,     8,    -1,     9,    -1,    11,    -1,    10,
    -1,    12,    -1,    13,    -1,    17,    -1,   378,    -1,   377,
    124,   378,    -1,    17,   121,   379,   122,    -1,    17,   121,
    343,   122,    -1,    30,    -1,   136,    30,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] = {
    0,   476,   476,   478,   483,   482,   497,   502,   508,   514,
    522,   523,   529,   533,   543,   547,   551,   556,   565,   566,
    571,   572,   577,   583,   590,   591,   596,   597,   601,   602,
    607,   621,   620,   633,   634,   636,   643,   648,   657,   661,
    665,   669,   673,   677,   681,   685,   689,   693,   702,   703,
    708,   709,   710,   711,   712,   713,   714,   720,   719,   738,
    747,   748,   749,   754,   755,   756,   757,   758,   759,   760,
    761,   762,   764,   765,   766,   767,   768,   769,   770,   771,
    772,   773,   774,   779,   780,   781,   782,   783,   792,   797,
    803,   811,   815,   824,   829,   836,   840,   849,   857,   858,
    862,   866,   870,   874,   878,   882,   886,   890,   894,   898,
    902,   906,   914,   923,   922,   934,   938,   942,   949,   954,
    962,   967,   972,   984,   992,   999,  1004,  1009,  1017,  1022,
    1031,  1035,  1039,  1046,  1050,  1059,  1064,  1068,  1076,  1084,
    1088,  1093,  1101,  1100,  1113,  1120,  1128,  1136,  1141,  1146,
    1155,  1154,  1162,  1166,  1170,  1174,  1181,  1187,  1195,  1199,
    1205,  1204,  1215,  1223,  1231,  1236,  1245,  1244,  1253,  1261,
    1266,  1271,  1279,  1287,  1288,  1301,  1305,  1309,  1314,  1321,
    1329,  1333,  1337,  1345,  1354,  1359,  1364,  1368,  1373,  1378,
    1383,  1387,  1391,  1398,  1399,  1400,  1404,  1407,  1416,  1420,
    1425,  1429,  1434,  1438,  1446,  1453,  1454,  1459,  1464,  1471,
    1475,  1477,  1476,  1490,  1494,  1498,  1504,  1514,  1515,  1526,
    1537,  1551,  1557,  1564,  1565,  1572,  1579,  1580,  1584,  1585,
    1589,  1590,  1591,  1599,  1603,  1607,  1608,  1609,  1611,  1615,
    1622,  1623,  1627,  1628,  1635,  1636,  1643,  1644,  1651,  1658,
    1665,  1666,  1667,  1676,  1684,  1688,  1695,  1699,  1706,  1711,
    1719,  1723,  1730,  1734,  1738,  1743,  1757,  1766,  1769,  1776,
    1781,  1789,  1793,  1797,  1801,  1807,  1808,  1812,  1819,  1826,
    1831,  1840,  1844,  1852,  1859,  1865,  1875,  1874,  1890,  1889,
    1906,  1910,  1918,  1923,  1928,  1936,  1943,  1947,  1954,  1965,
    1970,  1978,  1979,  1980,  1981,  1982,  1983,  1984,  1988,  1992,
    1997,  2005,  2010,  2015,  2021,  2020,  2033,  2039,  2038,  2057,
    2061,  2069,  2074,  2080,  2079,  2095,  2099,  2107,  2111,  2116,
    2115,  2131,  2135,  2140,  2139,  2156,  2155,  2165,  2171,  2175,
    2182,  2188,  2196,  2197,  2201,  2209,  2213,  2217,  2224,  2228,
    2236,  2241,  2250,  2264,  2269,  2277,  2284,  2288,  2297,  2301,
    2306,  2307,  2308,  2309,  2310,  2316,  2317,  2318,  2327,  2332,
    2339,  2349,  2348,  2360,  2359,  2374,  2384,  2390,  2396,  2400,
    2408,  2412,  2418,  2422,  2426,  2435,  2443,  2444,  2452,  2456,
    2460,  2464,  2478,  2477,  2489,  2488,  2501,  2502,  2503,  2505,
    2506,  2511,  2510,  2525,  2524,  2536,  2535,  2554,  2553,  2572,
    2571,  2590,  2597,  2602,  2610,  2614,  2618,  2625,  2630,  2639,
    2640,  2644,  2651,  2656,  2664,  2668,  2672,  2680,  2684,  2691,
    2692,  2696,  2700,  2709,  2708,  2723,  2724,  2729,  2730,  2731,
    2732,  2737,  2741,  2745,  2746,  2747,  2748,  2759,  2763,  2773,
    2781,  2795,  2807,  2816,  2817,  2818,  2826,  2827,  2828,  2833,
    2837,  2844,  2848,  2852,  2860,  2865,  2874,  2878,  2886,  2893,
    2897,  2898,  2899,  2903,  2910,  2916,  2924,  2925,  2932,  2941,
    2951,  2959,  2967,  2968,  2972,  2982,  2986,  2994,  2998,  3005,
    3010,  3017,  3059,  3060,  3064,  3075,  3076,  3080,  3088,  3089,
    3093,  3101,  3125,  3265,  3266,  3267,  3268,  3269,  3270,  3271,
    3272,  3273,  3274,  3275,  3280,  3285,  3292,  3296,  3305,  3306
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || YYTOKEN_TABLE
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char* const yytname[] = {
    "$end", "error", "$undefined", "MODULEREFERENCE", "TYPEREFERENCE",
    "OBJECTCLASSREFERENCE", "VALUEREFERENCE", "OBJECTREFERENCE",
    "OBJECTSETREFERENCE", "PARAMETERIZEDTYPEREFERENCE",
    "PARAMETERIZEDOBJECTCLASSREFERENCE", "PARAMETERIZEDVALUEREFERENCE",
    "PARAMETERIZEDOBJECTREFERENCE", "PARAMETERIZEDOBJECTSETREFERENCE",
    "VALUESET_BRACE", "OBJECT_BRACE", "OBJECTSET_BRACE", "IDENTIFIER",
    "BIT_IDENTIFIER", "OID_IDENTIFIER", "IMPORT_IDENTIFIER",
    "fieldreference", "FieldReference", "TYPEFIELDREFERENCE",
    "FIXEDTYPEVALUEFIELDREFERENCE", "VARIABLETYPEVALUEFIELDREFERENCE",
    "FIXEDTYPEVALUESETFIELDREFERENCE", "VARIABLETYPEVALUESETFIELDREFERENCE",
    "OBJECTFIELDREFERENCE", "OBJECTSETFIELDREFERENCE", "INTEGER", "CSTRING",
    "OS_BSTRING", "BS_BSTRING", "OS_HSTRING", "BS_HSTRING", "ABSENT",
    "ABSTRACT_SYNTAX", "ALL", "ANY", "APPLICATION", "ASSIGNMENT",
    "AUTOMATIC", "BEGIN_t", "BIT", "BMPString", "BOOLEAN_t", "BY",
    "CHARACTER", "CHOICE", "CLASS", "COMPONENT", "COMPONENTS", "CONSTRAINED",
    "DEFAULT", "DEFINED", "DEFINITIONS", "EMBEDDED", "END", "ENUMERATED",
    "EXCEPT", "EXPLICIT", "EXPORTS", "EXTERNAL", "FALSE_t", "FROM",
    "GeneralString", "GraphicString", "IA5String", "TYPE_IDENTIFIER",
    "IDENTIFIER_t", "IMPLICIT", "IMPORTS", "INCLUDES", "INSTANCE",
    "INTEGER_t", "INTERSECTION", "ISO646String", "MACRO", "MAX", "MIN",
    "MINUS_INFINITY", "NOTATION", "NULL_VALUE", "NULL_TYPE", "NumericString",
    "OBJECT", "OCTET", "OF_t", "OPTIONAL_t", "PDV", "PLUS_INFINITY",
    "PRESENT", "PrintableString", "PRIVATE", "REAL", "SEQUENCE", "SET",
    "SIZE_t", "STRING", "SYNTAX", "T61String", "TAGS", "TeletexString",
    "TRUE_t", "TYPE_t", "UNION", "UNIQUE", "UNIVERSAL", "UniversalString",
    "VideotexString", "VisibleString", "GeneralizedTime", "UTCTime", "VALUE",
    "WITH", "ObjectDescriptor_t", "WORD_t", "OID_INTEGER", "'{'", "'}'",
    "'('", "')'", "';'", "','", "'.'", "'['", "']'", "'<'", "'!'", "':'",
    "'|'", "'^'", "'@'", "'0'", "'>'", "'-'", "$accept",
    "ModuleDefinitionList", "ModuleDefinition", "$@1",
    "DefinitiveIdentifier", "DefinitiveObjIdComponentList",
    "DefinitiveObjIdComponent", "DefinitiveNameAndNumberForm", "TagDefault",
    "ModuleBody", "Exports", "SymbolsExported", "Imports", "SymbolsImported",
    "SymbolsFromModuleList", "SymbolsFromModule", "GlobalModuleReference",
    "$@2", "AssignedIdentifier", "SymbolList", "Symbol", "AssignmentList",
    "Assignment", "ValueSetTypeAssignment", "$@3", "TypeAssignment", "Type",
    "BuiltinType", "ReferencedType", "DefinedType", "ExternalTypeReference",
    "BitStringType", "NamedBitList", "NamedBit", "BooleanType",
    "CharacterStringType", "RestrictedCharacterStringType",
    "UnrestrictedCharacterStringType", "ChoiceType", "$@4",
    "AlternativeTypeLists", "AlternativeTypeList", "ExtensionAndException",
    "NamedType", "EmbeddedPDVType", "EnumeratedType", "Enumerations",
    "Enumeration", "EnumerationItem", "ExternalType", "AnyType",
    "InstanceOfType", "IntegerType", "NullType", "ObjectClassFieldType",
    "SimpleObjectClassFieldType", "$@5", "ObjectIdentifierType",
    "OctetStringType", "RealType", "SequenceType", "SequenceAndBrace", "$@6",
    "ComponentTypeLists", "ComponentTypeList", "ComponentType", "$@7",
    "SequenceOfType", "SetType", "SetAndBrace", "$@8", "SetOfType",
    "TaggedType", "Tag", "ClassNumber", "Class", "SelectionType",
    "UsefulType", "TypeFromObject", "ValueSetFromObjects",
    "ObjectSetFromObjects", "ReferencedObjects", "ReferencedObject",
    "ParameterizedObject", "ConstrainedType", "TypeWithConstraint",
    "Constraint", "ConstraintSpec", "ExceptionSpec",
    "ExceptionIdentification", "$@9", "ElementSetSpecs", "ElementSetSpec",
    "Unions", "Intersections", "IntersectionElements", "Exclusions",
    "UnionMark", "IntersectionMark", "Elements", "SubtypeElements",
    "ValueRange", "LowerEndpoint", "UpperEndpoint", "LowerEndValue",
    "UpperEndValue", "PermittedAlphabet", "ContainedSubtype",
    "SizeConstraint", "InnerTypeConstraints", "MultipleTypeConstraints",
    "TypeConstraints", "NamedConstraint", "PresenceConstraint",
    "UserDefinedConstraint", "UserDefinedConstraintParameterList",
    "UserDefinedConstraintParameters", "UserDefinedConstraintParameter",
    "Governor", "TableConstraint", "SimpleTableConstraint",
    "ComponentRelationConstraint", "AtNotations", "AtNotation",
    "ComponentIdList", "ObjectClassAssignment", "ObjectAssignment", "$@10",
    "ObjectSetAssignment", "$@11", "ObjectClass", "DefinedObjectClass",
    "ExternalObjectClassReference", "UsefulObjectClassReference",
    "ObjectClassDefn", "FieldSpecs", "FieldSpec", "TypeFieldSpec",
    "FixedTypeValueFieldSpec", "$@12", "$@13", "VariableTypeValueFieldSpec",
    "FixedTypeValueSetFieldSpec", "$@14", "VariableTypeValueSetFieldSpec",
    "ObjectFieldSpec", "$@15", "ObjectSetFieldSpec", "$@16",
    "WithSyntaxSpec", "$@17", "SyntaxList", "TokenOrGroupSpecs",
    "TokenOrGroupSpec", "OptionalGroup", "RequiredToken", "Literal",
    "DefinedObject", "ExternalObjectReference", "DefinedObjectSet",
    "ExternalObjectSetReference", "ParameterizedObjectSet", "FieldName",
    "PrimitiveFieldName", "Object", "ObjectDefn", "DefaultSyntax",
    "FieldSettings", "$@18", "$@19", "DefinedSyntax", "DefinedSyntaxTokens",
    "DefinedSyntaxToken", "Setting", "ObjectSet", "ObjectSetSpec",
    "ObjectSetElements", "ReferencedObjectsDot", "$@20",
    "ReferencedObjectDot", "$@21", "ParameterizedAssignment",
    "ParameterizedTypeAssignment", "$@22", "ParameterizedValueAssignment",
    "$@23", "ParameterizedValueSetTypeAssignment", "$@24",
    "ParameterizedObjectAssignment", "$@25",
    "ParameterizedObjectSetAssignment", "$@26", "ParameterList",
    "Parameters", "Parameter", "ParameterizedType", "SimpleDefinedType",
    "ActualParameterList", "ActualParameters", "ActualParameter",
    "ObjectParameter", "ObjectSetParameter", "ValueAssignment", "$@27",
    "Value", "BuiltinValue", "DefinedValue", "ExternalValueReference",
    "ObjectIdentifierValue", "ObjIdComponentList", "ObjIdComponent",
    "NumberForm", "OctetStringValue", "BitStringValue", "BitIdentifierList",
    "BooleanValue", "CharacterStringValue", "RestrictedCharacterStringValue",
    "CharacterStringList", "CharSyms", "CharsDefn", "Quadruple", "Tuple",
    "ChoiceValue", "NullValue", "RealValue", "NumericRealValue",
    "SpecialRealValue", "SequenceValue", "ComponentValueList", "NamedValue",
    "ReferencedValue", "ValueFromObject", "VALUEFIELDREFERENCE",
    "ValueSetFromObject", "VALUESETFIELDREFERENCE", "ObjectFromObject",
    "ObjectSetFromObject", "ValueSet", "Reference", "NamedNumberList",
    "NamedNumber", "SignedNumber", 0
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
    345,   346,   347,   348,   349,   350,   351,   352,   353,   354,
    355,   356,   357,   358,   359,   360,   361,   362,   363,   364,
    365,   366,   367,   368,   369,   370,   371,   372,   373,   123,
    125,    40,    41,    59,    44,    46,    91,    93,    60,    33,
    58,   124,    94,    64,    48,    62,    45
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint16 yyr1[] = {
    0,   137,   138,   138,   140,   139,   141,   141,   142,   142,
    143,   143,   143,   144,   145,   145,   145,   145,   146,   146,
    147,   147,   148,   148,   149,   149,   150,   150,   151,   151,
    152,   154,   153,   155,   155,   155,   156,   156,   157,   157,
    157,   157,   157,   157,   157,   157,   157,   157,   158,   158,
    159,   159,   159,   159,   159,   159,   159,   161,   160,   162,
    163,   163,   163,   164,   164,   164,   164,   164,   164,   164,
    164,   164,   164,   164,   164,   164,   164,   164,   164,   164,
    164,   164,   164,   165,   165,   165,   165,   165,   166,   166,
    167,   168,   168,   169,   169,   170,   170,   171,   172,   172,
    173,   173,   173,   173,   173,   173,   173,   173,   173,   173,
    173,   173,   174,   176,   175,   177,   177,   177,   178,   178,
    179,   180,   180,   181,   182,   183,   183,   183,   184,   184,
    185,   185,   186,   187,   187,   188,   189,   189,   190,   191,
    191,   191,   193,   192,   194,   195,   196,   197,   197,   197,
    199,   198,   200,   200,   200,   200,   201,   201,   202,   202,
    203,   202,   202,   204,   205,   205,   207,   206,   208,   209,
    209,   209,   210,   211,   211,   212,   212,   212,   212,   213,
    214,   214,   214,   215,   216,   216,   216,   217,   217,   217,
    218,   218,   218,   219,   219,   219,   220,   220,   221,   221,
    222,   222,   222,   222,   223,   224,   224,   225,   225,   226,
    226,   227,   226,   228,   228,   228,   228,   229,   229,   230,
    230,   231,   231,   232,   232,   233,   234,   234,   235,   235,
    236,   236,   236,   237,   237,   237,   237,   237,   237,   238,
    239,   239,   240,   240,   241,   241,   242,   242,   243,   244,
    244,   244,   244,   245,   246,   246,   247,   247,   248,   248,
    249,   249,   250,   250,   250,   250,   251,   252,   252,   253,
    253,   254,   254,   255,   255,   256,   256,   257,   258,   259,
    259,   260,   260,   261,   261,   262,   264,   263,   266,   265,
    267,   267,   268,   268,   268,   269,   270,   270,   271,   272,
    272,   273,   273,   273,   273,   273,   273,   273,   274,   274,
    274,   275,   275,   275,   276,   275,   275,   277,   275,   278,
    278,   279,   279,   280,   279,   281,   281,   282,   282,   283,
    282,   284,   284,   285,   284,   287,   286,   286,   288,   288,
    289,   289,   290,   290,   291,   292,   292,   292,   293,   293,
    294,   294,   295,   296,   296,   297,   298,   298,   299,   299,
    300,   300,   300,   300,   300,   301,   301,   301,   302,   302,
    303,   305,   304,   306,   304,   307,   308,   308,   309,   309,
    310,   310,   310,   310,   310,   311,   312,   312,   313,   313,
    313,   313,   315,   314,   317,   316,   318,   318,   318,   318,
    318,   320,   319,   322,   321,   324,   323,   326,   325,   328,
    327,   329,   330,   330,   331,   331,   331,   332,   332,   333,
    333,   334,   335,   335,   336,   336,   336,   336,   336,   337,
    337,   338,   338,   340,   339,   341,   341,   342,   342,   342,
    342,   342,   342,   342,   342,   342,   342,   343,   343,   344,
    345,   346,   346,   347,   347,   347,   348,   348,   348,   349,
    349,   350,   350,   350,   351,   351,   352,   352,   353,   354,
    354,   354,   354,   355,   356,   356,   357,   357,   358,   359,
    360,   361,   362,   362,   363,   364,   364,   365,   365,   366,
    366,   367,   368,   368,   369,   370,   370,   371,   372,   372,
    373,   374,   375,   376,   376,   376,   376,   376,   376,   376,
    376,   376,   376,   376,   377,   377,   378,   378,   379,   379
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] = {
    0,     2,     2,     1,     0,     9,     3,     0,     1,     2,
    1,     1,     1,     4,     2,     2,     2,     0,     3,     0,
    3,     0,     1,     0,     3,     0,     1,     0,     1,     2,
    3,     0,     3,     1,     1,     0,     1,     3,     1,     1,
    1,     1,     1,     3,     3,     3,     3,     3,     1,     2,
    1,     1,     1,     1,     1,     1,     1,     0,     5,     3,
    1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
    1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
    1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
    3,     2,     5,     1,     3,     4,     4,     1,     1,     1,
    1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
    1,     1,     2,     0,     5,     1,     3,     5,     1,     3,
    4,     2,     1,     2,     4,     1,     5,     7,     1,     3,
    1,     1,     1,     1,     4,     3,     1,     4,     1,     1,
    5,     5,     0,     4,     2,     2,     1,     3,     2,     3,
    0,     3,     1,     3,     5,     3,     1,     3,     1,     2,
    0,     4,     3,     3,     3,     2,     0,     3,     3,     2,
    3,     3,     4,     1,     1,     1,     1,     1,     0,     3,
    1,     1,     1,     2,     2,     2,     1,     2,     2,     1,
    1,     1,     1,     1,     1,     1,     2,     4,     2,     1,
    4,     4,     4,     4,     4,     1,     1,     2,     0,     1,
    1,     0,     4,     1,     5,     5,     7,     1,     2,     1,
    3,     1,     3,     1,     2,     2,     1,     1,     1,     1,
    1,     1,     3,     1,     1,     1,     1,     1,     1,     4,
    1,     2,     1,     2,     1,     1,     1,     1,     2,     2,
    1,     1,     1,     2,     3,     3,     3,     7,     1,     3,
    2,     3,     1,     1,     1,     0,     5,     0,     1,     3,
    1,     3,     1,     1,     1,     1,     1,     3,     6,     3,
    1,     2,     3,     3,     1,     3,     0,     5,     0,     5,
    1,     1,     1,     1,     1,     3,     1,     1,     5,     3,
    1,     1,     1,     1,     1,     1,     1,     1,     1,     2,
    3,     2,     3,     3,     0,     5,     4,     0,     6,     2,
    3,     2,     3,     0,     5,     2,     3,     2,     3,     0,
    5,     2,     3,     0,     5,     0,     4,     0,     3,     2,
    2,     1,     1,     1,     3,     1,     1,     1,     1,     1,
    1,     1,     3,     1,     1,     3,     2,     4,     3,     1,
    1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
    3,     0,     5,     0,     3,     3,     2,     0,     1,     1,
    1,     1,     1,     1,     1,     3,     1,     3,     1,     1,
    1,     1,     0,     3,     0,     3,     1,     1,     1,     1,
    1,     0,     5,     0,     6,     0,     6,     0,     6,     0,
    6,     3,     3,     1,     3,     3,     1,     2,     4,     1,
    1,     3,     3,     1,     1,     1,     3,     1,     3,     1,
    1,     1,     1,     0,     5,     1,     1,     1,     1,     1,
    1,     1,     1,     1,     1,     1,     1,     1,     1,     3,
    3,     1,     2,     1,     1,     4,     1,     1,     1,     1,
    1,     1,     1,     3,     1,     3,     1,     1,     1,     1,
    1,     1,     1,     3,     1,     3,     1,     1,     9,     5,
    3,     1,     1,     1,     1,     1,     1,     3,     2,     1,
    3,     2,     1,     1,     2,     1,     1,     2,     1,     1,
    2,     2,     3,     1,     1,     1,     1,     1,     1,     1,
    1,     1,     1,     1,     1,     3,     4,     4,     1,     2
};

/* YYDEFACT[STATE-NAME] -- Default rule to reduce with in state
   STATE-NUM when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint16 yydefact[] = {
    0,     7,     0,     3,     0,     0,     1,     2,    10,    11,
    0,     8,    12,    17,     0,     6,     9,     0,     0,     0,
    0,     0,    16,    14,    15,     0,    13,     4,    21,    23,
    0,    25,    38,    40,    39,    41,    42,     0,     0,     0,
    0,     0,     0,    22,    36,     5,    27,     0,     0,     0,
    0,     0,     0,    20,     0,     0,    26,    28,     0,     0,
    0,     0,     0,     0,     0,     0,     0,     0,    18,    48,
    52,    50,    53,    54,    55,    56,   396,   397,   398,   399,
    400,    51,    43,    45,    44,    46,    47,    37,    24,    29,
    0,     0,   419,   292,   350,   353,     0,     0,     0,     0,
    297,   133,     0,     0,   100,    97,     0,     0,     0,     0,
    132,   101,   102,   103,   296,     0,   136,   104,   138,   105,
    0,     0,   106,   146,   150,   166,   108,   107,   109,   110,
    111,   180,   181,   182,   178,    57,    62,    61,    83,   420,
    63,    64,    65,    98,    99,    66,    67,    68,    69,    70,
    71,    72,    73,    74,   139,    75,    76,    77,    78,     0,
    79,    80,     0,    81,    82,     0,    85,    84,    86,    87,
    191,   392,   394,   195,    60,   199,   142,   293,   294,   193,
    351,   190,   354,   192,     0,     0,    89,    88,   186,   194,
    189,     0,   433,     0,   286,   288,     0,   401,     0,     0,
    0,    49,    31,    30,     0,     0,   417,   196,   356,     0,
    0,    59,    91,   112,   113,   123,     0,     0,     0,   144,
    145,     0,     0,     0,     0,     0,     0,     0,     0,     0,
    0,   176,   177,   175,     0,     0,   198,     0,     0,     0,
    148,     0,   122,     0,   158,     0,   152,   156,   165,     0,
    0,     0,     0,   169,     0,     0,     0,   184,   185,   187,
    188,   183,   498,   499,   500,   501,   497,     0,   285,   290,
    291,     0,     0,     0,     0,   419,   292,   504,   506,   507,
    508,   510,   509,   511,   512,   513,     0,   142,     0,   413,
    416,   405,     0,   403,   407,   409,    35,    90,   295,   352,
    355,     0,     0,     0,     0,   447,     0,   518,   469,   459,
    461,   460,   462,   467,   486,   481,   485,   466,     0,   484,
    0,   424,   430,   429,     0,     0,   423,   427,   425,   435,
    492,   448,   443,   444,   437,   438,   439,   468,   470,   471,
    472,   440,   442,   445,   482,   483,   446,   436,   493,   441,
    179,     0,     0,     0,   130,     0,   125,   128,   131,   135,
    0,     0,   514,   163,   253,   377,     0,     0,     0,     0,
    245,     0,     0,     0,     0,     0,   251,   252,   390,   250,
    208,   205,   213,   217,   219,   221,   223,   230,   235,     0,
    240,   236,   234,   237,   238,   206,   365,   389,   391,   388,
    366,   369,   368,   231,   233,   367,   151,     0,     0,   168,
    167,     0,     0,     0,   173,     0,   174,     0,     0,   208,
    208,   275,   276,   121,     0,     0,   149,     0,   159,     0,
    147,     0,   164,   171,   170,   393,   395,     0,     0,     0,
    0,     0,     0,     0,   411,     0,     0,     0,     0,     0,
    0,     0,    32,    33,    34,   418,   197,   357,     0,     0,
    0,     0,   464,   453,     0,   476,   454,   488,     0,   431,
    432,     0,   477,     0,   451,     0,     0,   474,     0,   489,
    519,   495,   496,   494,   421,     0,   134,     0,     0,    93,
    0,   115,   118,     0,   124,     0,   137,     0,   360,   363,
    364,   373,     0,     0,   361,   362,     0,   218,     0,   248,
    249,     0,     0,     0,     0,     0,     0,     0,   227,   226,
    0,   229,   228,     0,   224,     0,   241,   202,   203,   200,
    201,     0,   172,     0,    58,     0,     0,     0,     0,   162,
    208,   155,     0,   153,   157,   143,   359,     0,   308,     0,
    300,   301,   302,   303,   304,   305,   306,   307,     0,     0,
    0,   434,     0,   287,     0,     0,   289,   503,   505,   506,
    507,   508,   511,   512,   513,   414,   415,   412,     0,   402,
    0,     0,     0,   449,   480,     0,   491,     0,     0,   426,
    428,   450,   452,   463,     0,   473,     0,   487,     0,   422,
    0,    92,     0,   114,     0,     0,     0,     0,   129,   515,
    0,   370,     0,   348,   375,   349,   380,   378,   383,   376,
    379,   384,   381,   382,   225,   267,   254,     0,   255,   232,
    0,   211,   207,   210,   209,   204,     0,   220,   222,     0,
    0,     0,   277,   141,   140,   120,     0,   161,     0,     0,
    319,   311,   327,   325,     0,   309,   321,   331,   337,     0,
    0,     0,     0,   386,     0,   406,   404,   408,   410,   458,
    456,   457,     0,     0,   465,   475,   490,     0,     0,    94,
    116,   119,   517,   516,     0,   374,   371,   424,     0,   268,
    270,     0,   142,   272,   265,     0,     0,   258,     0,     0,
    0,   247,     0,   239,   242,   246,   502,     0,   154,   358,
    320,   313,   312,     0,   328,     0,   326,   310,   322,     0,
    332,     0,     0,   298,   299,     0,   385,   455,   479,     0,
    95,    96,     0,   126,     0,   266,     0,     0,   263,   264,
    262,   265,   260,     0,   256,     0,   215,     0,   214,   243,
    0,     0,   280,   316,     0,     0,     0,     0,     0,   335,
    387,     0,   117,     0,   372,   269,   271,   261,     0,   259,
    212,     0,   284,     0,   281,   278,     0,     0,   315,   330,
    324,   334,     0,     0,     0,   127,     0,   216,   282,     0,
    279,   318,     0,   336,     0,     0,     0,   283,   347,   346,
    339,     0,     0,   341,   343,   342,   345,   478,   257,     0,
    338,   340,   344
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] = {
    -1,     2,     3,    28,     5,    10,    11,    12,    20,    30,
    31,    42,    47,    55,    56,    57,   203,   296,   452,    58,
    44,    68,    69,    70,   235,    71,   242,   136,   137,   138,
    139,   140,   488,   489,   141,   142,   143,   144,   145,   353,
    490,   491,   243,   244,   146,   147,   355,   356,   357,   148,
    149,   150,   151,   152,   153,   154,   256,   155,   156,   157,
    158,   159,   224,   245,   246,   247,   429,   160,   161,   162,
    228,   163,   164,   165,   415,   234,   166,   167,   168,   169,
    170,   171,   172,   173,   174,   175,   236,   380,   516,   632,
    699,   381,   382,   383,   384,   385,   507,   520,   523,   386,
    387,   388,   389,   703,   390,   704,   391,   392,   393,   394,
    628,   696,   697,   742,   395,   688,   689,   690,   691,   420,
    421,   422,   751,   752,   774,    72,    73,   273,    74,   274,
    268,   176,   177,   178,   270,   549,   550,   551,   552,   713,
    754,   553,   554,   719,   555,   556,   715,   557,   721,   723,
    782,   793,   802,   803,   804,   805,   806,   179,   180,   181,
    182,   183,   545,   501,   399,   400,   401,   502,   734,   610,
    402,   503,   619,   620,   621,   664,   403,   184,   254,   185,
    255,    75,    76,   292,    77,   448,    78,   446,    79,   449,
    80,   450,   197,   288,   289,   186,   187,   206,   325,   693,
    327,   471,    81,   271,   404,   329,   330,   331,   332,   473,
    474,   672,   333,   334,   475,   335,   336,   337,   338,   476,
    477,   339,   340,   341,   342,   343,   344,   345,   346,   478,
    479,   347,   348,   504,   188,   505,   189,   190,   623,   290,
    361,   358,   349
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -731
static const yytype_int16 yypact[] = {
    52,   -61,   203,  -731,    93,   107,  -731,  -731,   -43,  -731,
    49,    93,  -731,   214,    92,  -731,  -731,    84,   103,   120,
    202,   167,  -731,  -731,  -731,   256,  -731,  -731,    66,   580,
    238,   248,  -731,  -731,  -731,  -731,  -731,   227,   239,   244,
    247,   272,   249,  -731,   274,  -731,   580,   605,   292,   309,
    314,   331,   334,  -731,   580,   297,   580,  -731,   390,  2910,
    419,  3370,   209,   209,   348,   348,   348,   348,   605,  -731,
    -731,  -731,  -731,  -731,  -731,  -731,  -731,  -731,  -731,  -731,
    -731,  -731,  -731,  -731,  -731,  -731,  -731,  -731,  -731,  -731,
    467,   346,  -731,  -731,  -731,  -731,   353,   353,   353,   345,
    -731,   421,  3370,   380,  -731,  -731,   385,   366,   396,   371,
    -731,  -731,  -731,  -731,  -731,   405,   375,  -731,  -731,  -731,
    426,   399,  -731,  -731,    83,   136,  -731,  -731,  -731,  -731,
    -731,  -731,  -731,  -731,    11,   400,  -731,  -731,  -731,  -731,
    -731,  -731,  -731,  -731,  -731,  -731,  -731,  -731,  -731,  -731,
    -731,  -731,  -731,  -731,   401,  -731,  -731,  -731,  -731,  2202,
    -731,  -731,  2326,  -731,  -731,  2680,  -731,  -731,  -731,  -731,
    -731,  -731,  -731,  -731,  -731,  -731,  -731,  -731,  -731,  -731,
    -731,  -731,  -731,  -731,   378,   253,  -731,  -731,  -731,  -731,
    -731,   412,   400,   395,  -731,  -731,  2450,  3370,  3370,   209,
    209,  -731,  -731,  -731,   495,  1726,  -731,  -731,  -731,  3370,
    477,   400,   407,  -731,  -731,  -731,   510,   209,   511,  -731,
    -731,  3370,   400,   654,   410,   442,   443,  3370,   422,   450,
    457,  -731,  -731,  -731,   241,   507,  -731,   788,  2076,   459,
    -731,   424,   400,    70,   -12,   432,   429,  -731,  -731,   433,
    436,  3370,  3370,   400,   434,   437,   446,  -731,  -731,  -731,
    -731,  -731,  -731,  -731,  -731,  -731,  -731,   448,  -731,  -731,
    -731,   527,   567,   532,   533,   131,   191,  -731,   452,   453,
    353,  -731,  -731,   353,   353,   345,   -81,   445,   219,  -731,
    -731,   400,   540,   400,  -731,  -731,    35,  -731,  -731,  -731,
    -731,   353,   353,   353,   469,  -731,   275,  -731,  -731,  -731,
    -731,  -731,  -731,  -731,  -731,  -731,  -731,  -731,    57,  -731,
    566,   400,   472,   475,   300,   231,  -731,  -731,  -731,  -731,
    -731,  -731,  -731,  -731,  -731,  -731,  -731,  -731,  -731,  -731,
    -731,  -731,  -731,  -731,  -731,  -731,  -731,  -731,  -731,  -731,
    400,   582,   585,  3485,   482,   484,   491,  -731,  -731,  -731,
    482,   251,  -731,   400,  -731,   627,   546,   572,   400,  3370,
    -731,    97,    95,  1190,   496,   400,   499,   501,   500,   503,
    497,  -731,   505,   -37,   -30,  -731,   546,  -731,  -731,   502,
    504,  -731,  -731,  -731,  -731,  -731,   475,   509,   512,  -731,
    -731,  -731,  -731,  -731,   140,   521,  -731,  3370,  3370,   400,
    -731,  3370,  3370,   522,  -731,   508,  -731,   616,    78,   497,
    497,  -731,  -731,   400,  3370,   539,  -731,  3025,  -731,   577,
    -731,  2795,  -731,   400,   400,  -731,  -731,   627,   287,  2013,
    283,   620,   506,   506,  -731,  2450,   624,  3370,   629,   631,
    632,    16,  -731,  -731,  -731,  -731,  -731,  -731,   636,  2013,
    543,  2013,  -731,   553,   552,  -731,  -731,  -731,   559,  -731,
    -731,   562,  -731,    12,  -731,   254,   257,  -731,   273,  -731,
    -731,  -731,  -731,  -731,  -731,  1726,  -731,   569,   290,  -731,
    563,   570,  -731,    22,  -731,     6,  -731,   511,  -731,  -731,
    -731,  -731,   298,  1458,  -731,  -731,  1324,  -731,   576,  -731,
    400,   400,   578,   574,   579,  1860,   583,   581,  -731,  -731,
    1324,  -731,  -731,  1324,  -731,   584,  -731,   400,   400,   400,
    400,   702,  -731,   922,  -731,   589,   590,   593,   594,   400,
    497,   600,  2013,   601,  -731,   607,  -731,  3140,  2565,   299,
    -731,  -731,  -731,  -731,  -731,  -731,  -731,  -731,   608,   596,
    276,  -731,   611,  -731,   714,  1056,  -731,  -731,  -731,  -731,
    -731,  -731,  -731,  -731,  -731,  -731,  -731,  -731,   616,   400,
    2013,   283,   620,  -731,  -731,   474,  -731,    23,   700,  -731,
    -731,  -731,  -731,  -731,   725,  -731,   177,  -731,   727,  -731,
    291,  -731,   585,  -731,  3255,   626,   634,   621,  -731,  -731,
    1592,  -731,   627,  -731,  -731,  -731,   400,  -731,  -731,  -731,
    -731,  -731,  -731,  -731,  -731,  1726,  -731,    20,  -731,  -731,
    630,   400,  -731,  -731,  -731,  -731,   628,   -30,  -731,  1971,
    639,   460,   641,  -731,  -731,  -731,  3025,  -731,  3025,   627,
    672,   156,     2,   673,  3370,  -731,   -21,   112,   653,   287,
    221,   262,   646,  -731,   652,  -731,  -731,  -731,  -731,  -731,
    -731,  -731,   655,   301,  -731,  -731,  -731,   656,   659,  -731,
    650,  -731,  -731,  -731,   651,  -731,  -731,    55,   662,  -731,
    660,   657,   668,  -731,    81,   658,   665,   675,  1190,   674,
    661,  -731,  1978,  -731,  -731,  -731,  -731,   669,   600,  -731,
    -731,  -731,    29,   735,  -731,   752,  -731,   400,  -731,   753,
    -731,   754,   709,  -731,  -731,   685,  -731,  -731,  -731,   781,
    -731,  -731,  3485,   688,  1592,  -731,  1726,  1726,  -731,  -731,
    -731,   181,  -731,   689,  -731,   796,  -731,  2013,   691,  -731,
    30,   315,  -731,  -731,   762,  2013,   283,   616,   620,  -731,
    630,   693,   704,   510,  -731,  -731,  -731,  -731,   705,  -731,
    -731,  1190,  -731,   807,   706,  -731,   669,  2013,  -731,  -731,
    -731,  -731,   711,   805,  3485,   715,   796,  -731,   706,   821,
    -731,  -731,    71,  -731,   720,   510,   722,  -731,  -731,  -731,
    -731,   135,    99,  -731,  -731,  -731,  -731,  -731,  -731,    94,
    -731,  -731,  -731
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] = {
    -731,  -731,   841,  -731,  -731,   833,  -731,  -731,  -731,  -731,
    -731,  -731,  -731,  -731,  -731,   790,  -731,  -731,  -731,    82,
    -731,  -731,   780,  -731,  -731,  -731,   -59,  -164,  -138,   531,
    -731,  -731,  -731,   258,  -731,  -731,  -731,  -731,  -731,  -731,
    -731,   118,  -159,  -338,  -731,  -731,  -731,    96,  -479,  -731,
    -731,  -731,  -731,  -731,  -731,  -731,  -731,  -731,  -731,  -731,
    -731,  -731,  -731,   696,  -422,  -414,  -731,  -731,  -731,  -731,
    -731,  -731,  -731,  -731,  -731,  -731,  -731,  -731,  -731,  -731,
    -90,  -731,  -731,  -194,   -70,  -731,  -115,   633,  -366,  -731,
    -731,  -503,  -360,  -731,   344,   343,   481,  -731,  -731,   370,
    -731,  -731,  -731,  -731,  -731,   175,  -731,  -731,   212,  -731,
    -731,  -706,  -731,   137,  -731,  -731,   144,  -731,  -731,  -731,
    -731,  -731,  -731,   106,   114,  -731,  -731,  -731,  -731,  -731,
    -731,   -56,  -731,  -731,  -731,  -731,   229,  -731,  -731,  -731,
    -731,  -731,  -731,  -731,  -731,  -731,  -731,  -731,  -731,  -731,
    -731,  -731,    89,  -730,  -731,  -731,   391,  -173,  -731,  -189,
    -731,  -166,  -731,  -413,  -421,  -731,  -731,  -731,  -731,  -731,
    -731,  -731,  -731,  -583,  -440,  -731,  -731,  -731,  -731,  -201,
    -731,  -731,  -731,  -731,  -731,  -731,  -731,  -731,  -731,  -731,
    -731,  -731,   392,  -731,   451,  -731,  -731,   -53,  -731,  -193,
    -731,  -731,  -731,  -731,  -197,  -731,  -131,   306,   599,  -731,
    435,  -731,  -731,  -731,  -731,  -731,  -731,  -731,  -731,  -731,
    310,  -731,  -731,  -731,  -731,  -731,  -731,  -731,  -731,  -731,
    304,  -731,  -731,  -306,  -731,  -165,  -133,  -731,  -396,    -2,
    -731,  -204,  -426
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -506
static const yytype_int16 yytable[] = {
    135,   566,   192,   249,   324,   541,   194,   195,   328,   225,
    229,   322,   326,   513,   362,   492,   608,   544,   483,   563,
    266,   534,   324,   354,   546,   413,   413,   685,   305,   669,
    640,   463,   323,  -323,   397,   463,   324,   694,   413,   769,
    223,   305,  -160,   211,   207,   208,   521,   772,   397,   442,
    396,   231,   307,   537,   538,     1,  -329,   398,     4,   376,
    460,    92,   663,   305,   396,    95,    96,   606,   718,   518,
    98,   398,   811,   376,   461,   462,   463,   428,    14,   811,
    796,   535,   618,  -317,   305,   377,    95,   464,   465,   634,
    405,   714,   798,   799,   519,   461,   462,   463,   413,   377,
    223,   305,   522,   416,   405,   232,   253,   364,   464,   465,
    8,    43,   461,   462,   463,   798,   799,   738,   753,   233,
    798,   799,    21,     9,   -19,   464,   465,  -142,    29,   469,
    466,   607,   591,   378,   466,   269,    87,   286,   291,   293,
    287,   670,   668,   294,   295,   695,   321,   378,   511,   512,
    350,   764,   470,   379,   451,   773,   798,   799,   320,   266,
    667,   359,   363,    13,   375,   453,  -333,   379,   409,    15,
    739,   221,   324,   740,   645,   466,   223,   467,   375,   423,
    413,   222,   665,   305,   397,  -273,    22,   472,   613,   618,
    426,   800,   433,   434,   427,   615,   466,   801,   467,   686,
    396,   720,   223,     6,   223,    23,     1,   398,   465,   376,
    -314,   613,   193,   466,    93,   467,   613,   738,   615,   810,
    801,   812,    24,   615,   227,   801,   708,   583,   299,   536,
    207,   208,   544,   302,   222,   377,   709,  -142,   560,   564,
    405,   472,   561,    25,   413,   711,   100,   305,   455,   456,
    457,  -503,   613,   509,   483,  -503,    17,   223,   560,   615,
    560,   801,   584,   712,   586,  -244,   681,   396,  -244,   299,
    739,   414,   543,   740,   302,    18,   261,   223,   114,   262,
    263,   264,   265,   378,   324,    19,   562,   472,   328,    26,
    94,   322,   599,   609,   413,    97,    45,   305,   365,    27,
    481,   482,   324,   379,   264,   324,   622,   405,   547,   548,
    510,  -505,   323,   618,   375,  -505,   608,   397,   781,   324,
    46,   677,   324,   261,   481,   482,   262,   263,   264,   265,
    396,   397,   324,   396,   397,   779,   226,   230,   746,   444,
    398,   560,   376,   445,   397,   647,    48,   396,   527,   528,
    396,   484,   529,   530,   398,   485,   376,   398,    49,   376,
    396,   780,   605,    50,   324,   539,    51,   398,   377,   376,
    405,   496,    53,   405,   593,   497,   397,   595,   594,   560,
    564,   596,   377,   666,   633,   377,   286,   405,   579,   287,
    405,    52,   396,   597,   492,   377,   626,   598,    54,   398,
    405,   376,   257,   209,   258,   459,   259,   260,   396,   324,
    601,   787,    82,   622,   602,   193,   378,    93,   611,   658,
    88,   728,   612,   659,   324,   729,   321,   377,   328,    83,
    378,   322,   405,   378,    84,   775,   379,   396,   560,   776,
    575,   576,   705,   378,   616,   680,   681,   375,   405,   100,
    379,    85,   323,   379,    86,    90,   631,   198,   199,   200,
    191,   375,   267,   379,   375,   472,   583,   196,   300,   678,
    202,   204,   205,   209,   375,   378,   210,   405,   297,   212,
    583,   114,   300,   301,   213,   214,   215,   303,   651,   656,
    216,   652,   657,   217,   218,   379,   219,   324,   220,   297,
    298,   560,   299,   300,   301,   705,   375,   302,   303,   397,
    567,   568,   277,   569,   570,   571,   281,   282,   572,   573,
    272,   223,   237,   574,   351,   396,   352,   354,   360,   406,
    407,   408,   398,   324,   376,   324,   324,   622,   411,   328,
    328,   410,   322,   322,   766,   412,   560,   424,   417,   425,
    770,   616,   430,   431,   560,   564,   432,   427,   778,   435,
    377,   396,   436,   323,   323,   405,   687,   438,   439,   692,
    324,   437,   298,   440,   441,   443,   560,  -350,  -353,   741,
    791,   447,   397,   396,    32,    33,    34,    35,    36,    37,
    38,    39,    40,    41,   458,   717,   480,  -195,   396,   486,
    -193,   405,   487,   493,   494,   398,   506,   376,   378,    59,
    60,    61,    62,    63,    64,   495,    65,    66,    67,   508,
    -62,   514,   -61,   405,   -60,  -191,   515,   525,   379,   517,
    533,   542,   526,   377,  -190,   532,   565,  -192,   405,   375,
    297,   298,   583,   299,   300,   301,  -194,   531,   302,   303,
    498,   481,   482,   262,   263,   499,   500,   304,    92,    93,
    305,    94,    95,    96,   540,   578,    97,    98,   585,   365,
    580,   306,   581,   582,   587,   616,   588,   687,   321,   589,
    692,   378,   590,   603,   307,   308,   309,   310,   311,   312,
    600,   100,   366,   101,   604,   625,   629,   627,   103,   104,
    105,   379,   106,   107,   630,   635,   636,   367,   583,   639,
    642,   108,   375,   109,   641,   643,   644,   110,   313,   368,
    111,   112,   113,   114,   646,   648,   459,   369,   115,   116,
    673,   117,   649,   660,   370,   314,   661,   315,   118,   119,
    120,   121,   264,   674,   461,   316,   684,   122,   682,   123,
    124,   125,   222,   700,   698,   126,   683,   127,   317,   706,
    707,   710,   716,   128,   129,   130,   131,   132,   722,   371,
    133,   725,   726,   372,   732,   373,   733,   727,   730,   374,
    134,   731,   735,   743,   736,   744,   748,   737,   319,   755,
    320,   304,    92,    93,   305,    94,    95,    96,  -274,   745,
    97,    98,   750,   365,   747,   306,   756,   757,   758,   759,
    760,   761,   763,   694,   768,   771,   777,   783,   307,   308,
    309,   310,   311,   312,   772,   100,   366,   101,   784,   786,
    792,   789,   103,   104,   105,   794,   106,   107,   797,   795,
    807,   367,   808,     7,    16,   108,    89,   109,   201,   468,
    762,   110,   313,   368,   111,   112,   113,   114,   250,   785,
    679,   369,   115,   116,   637,   117,   638,   524,   370,   314,
    419,   315,   118,   119,   120,   121,   624,   749,   767,   316,
    765,   122,   790,   123,   124,   125,   222,   788,   724,   126,
    809,   127,   317,   671,   617,   454,   577,   128,   129,   130,
    131,   132,   676,   371,   133,     0,   675,   418,   592,   373,
    0,     0,     0,   374,   134,     0,     0,     0,     0,     0,
    0,     0,   319,     0,   320,   304,    92,    93,   305,    94,
    95,    96,     0,     0,    97,    98,     0,   365,     0,   306,
    0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
    0,     0,   307,   308,   309,   310,   311,   312,     0,   100,
    366,   101,     0,     0,     0,     0,   103,   104,   105,     0,
    106,   107,     0,     0,     0,     0,     0,     0,     0,   108,
    0,   109,     0,     0,     0,   110,   313,   368,   111,   112,
    113,   114,     0,     0,     0,   369,   115,   116,     0,   117,
    0,     0,   370,   314,     0,   315,   118,   119,   120,   121,
    0,     0,     0,   316,     0,   122,     0,   123,   124,   125,
    222,     0,     0,   126,     0,   127,   317,     0,     0,     0,
    0,   128,   129,   130,   131,   132,     0,   371,   133,     0,
    0,   372,     0,   373,     0,     0,     0,   374,   134,     0,
    0,     0,     0,     0,     0,     0,   319,     0,   320,   304,
    92,    93,   305,    94,    95,    96,     0,     0,    97,    98,
    0,   365,     0,   306,     0,     0,     0,     0,     0,     0,
    0,     0,     0,     0,     0,     0,   307,   308,   309,   310,
    311,   312,     0,   100,   366,   101,     0,     0,     0,     0,
    103,   104,   105,     0,   106,   107,     0,     0,     0,     0,
    0,     0,     0,   108,     0,   109,     0,     0,     0,   110,
    313,   368,   111,   112,   113,   114,     0,     0,     0,   369,
    115,   116,     0,   117,     0,     0,   370,   314,     0,   315,
    118,   119,   120,   121,     0,     0,     0,   316,     0,   122,
    0,   123,   124,   125,   222,     0,     0,   126,     0,   127,
    317,     0,     0,     0,     0,   128,   129,   130,   131,   132,
    0,   371,   133,     0,     0,   372,     0,   373,     0,     0,
    0,   662,   134,     0,     0,     0,     0,     0,     0,     0,
    319,     0,   320,   304,    92,    93,   305,    94,    95,    96,
    0,     0,    97,    98,     0,   365,     0,   306,     0,     0,
    0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
    307,   308,   309,   310,   311,   312,     0,   100,   366,   101,
    0,     0,     0,     0,   103,   104,   105,     0,   106,   107,
    0,     0,     0,     0,     0,     0,     0,   108,     0,   109,
    0,     0,     0,   110,   313,   368,   111,   112,   113,   114,
    0,     0,     0,   369,   115,   116,     0,   117,     0,     0,
    370,   314,     0,   315,   118,   119,   120,   121,     0,     0,
    0,   316,     0,   122,     0,   123,   124,   125,   222,     0,
    0,   126,     0,   127,   317,     0,     0,     0,     0,   128,
    129,   130,   131,   132,     0,   371,   133,     0,     0,   372,
    0,   373,     0,     0,     0,     0,   134,     0,     0,     0,
    0,     0,     0,     0,   319,     0,   320,   304,    92,    93,
    305,    94,    95,    96,     0,     0,    97,    98,     0,   365,
    0,   306,     0,     0,     0,     0,     0,     0,     0,     0,
    0,     0,     0,     0,   307,   308,   309,   310,   311,   312,
    0,   100,     0,   101,     0,     0,     0,     0,   103,   104,
    105,     0,   106,   107,     0,     0,     0,     0,     0,     0,
    0,   108,     0,   109,     0,     0,     0,   110,   313,   368,
    111,   112,   113,   114,     0,     0,     0,   369,   115,   116,
    0,   117,     0,     0,   370,   314,     0,   315,   118,   119,
    120,   121,     0,     0,     0,   316,     0,   122,     0,   123,
    124,   125,   222,     0,     0,   126,     0,   127,   317,     0,
    0,     0,     0,   128,   129,   130,   131,   132,     0,   371,
    133,     0,     0,   372,     0,   373,     0,     0,     0,     0,
    134,     0,     0,     0,     0,     0,     0,     0,   319,     0,
    320,   304,    92,    93,   305,    94,    95,    96,     0,     0,
    97,    98,   533,   365,   565,   306,     0,     0,     0,     0,
    0,     0,     0,     0,     0,     0,     0,     0,   307,   308,
    309,   310,   311,   312,     0,   100,     0,   101,     0,     0,
    0,     0,   103,   104,   105,     0,   106,   107,     0,     0,
    0,     0,     0,     0,     0,   108,     0,   109,     0,     0,
    0,   110,   313,     0,   111,   112,   113,   114,     0,     0,
    0,     0,   115,   116,     0,   117,     0,     0,     0,   314,
    0,   315,   118,   119,   120,   121,     0,     0,     0,   316,
    0,   122,     0,   123,   124,   125,     0,     0,     0,   126,
    0,   127,   317,     0,     0,     0,     0,   128,   129,   130,
    131,   132,     0,     0,   133,   613,     0,   372,   614,     0,
    0,     0,   615,     0,   134,     0,     0,     0,     0,     0,
    0,     0,   319,     0,   320,   304,    92,    93,   305,    94,
    95,    96,     0,     0,    97,    98,   533,   365,   565,   306,
    0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
    0,     0,   307,   308,   309,   310,   311,   312,     0,   100,
    0,   101,     0,     0,     0,     0,   103,   104,   105,     0,
    106,   107,     0,     0,     0,     0,     0,     0,     0,   108,
    0,   109,     0,     0,     0,   110,   313,     0,   111,   112,
    113,   114,     0,     0,     0,     0,   115,   116,     0,   117,
    0,     0,     0,   314,     0,   315,   118,   119,   120,   121,
    0,     0,     0,   316,     0,   122,     0,   123,   124,   125,
    0,     0,     0,   126,     0,   127,   317,     0,     0,     0,
    0,   128,   129,   130,   131,   132,     0,     0,   133,     0,
    0,   372,     0,     0,     0,     0,     0,     0,   134,     0,
    0,     0,     0,     0,     0,     0,   319,     0,   320,   304,
    92,    93,   305,    94,    95,    96,     0,     0,    97,    98,
    0,     0,     0,   306,     0,     0,     0,     0,     0,     0,
    0,     0,     0,     0,     0,     0,   307,   308,   309,   310,
    311,   312,     0,   100,     0,   101,     0,     0,     0,     0,
    103,   104,   105,     0,   106,   107,     0,     0,     0,     0,
    0,     0,     0,   108,     0,   109,     0,     0,     0,   110,
    313,     0,   111,   112,   113,   114,     0,     0,     0,     0,
    115,   116,     0,   117,     0,     0,     0,   314,     0,   315,
    118,   119,   120,   121,     0,     0,     0,   316,     0,   122,
    0,   123,   124,   125,     0,     0,     0,   126,     0,   127,
    317,     0,     0,     0,     0,   128,   129,   130,   131,   132,
    0,     0,   133,     0,     0,   318,     0,     0,     0,     0,
    0,     0,   134,     0,     0,     0,     0,     0,     0,     0,
    319,     0,   320,   304,    92,    93,   305,    94,    95,    96,
    0,     0,    97,    98,     0,     0,     0,    99,     0,     0,
    0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
    307,     0,     0,     0,     0,     0,     0,   100,     0,   101,
    0,     0,     0,     0,   103,   104,   105,     0,   106,   107,
    0,     0,     0,     0,     0,     0,     0,   108,     0,   109,
    0,     0,     0,   110,     0,     0,   111,   112,   113,   114,
    0,     0,     0,     0,   115,   116,     0,   117,     0,     0,
    0,     0,     0,     0,   118,   119,   120,   121,     0,     0,
    0,     0,     0,   122,     0,   123,   124,   125,     0,     0,
    0,   126,     0,   127,     0,     0,     0,     0,     0,   128,
    129,   130,   131,   132,   558,     0,   133,   305,    94,     0,
    0,   558,     0,    97,   305,    94,   134,     0,   559,     0,
    97,     0,     0,     0,     0,   559,   320,     0,     0,     0,
    0,   307,   308,   309,   310,   311,   312,     0,   307,   308,
    309,   310,   311,   312,     0,     0,   558,     0,     0,   305,
    94,     0,     0,     0,     0,    97,     0,     0,     0,     0,
    559,     0,     0,     0,     0,   313,     0,     0,     0,     0,
    0,     0,   313,   307,   308,   309,   310,   311,   312,     0,
    701,     0,   314,     0,   315,     0,     0,   701,     0,   314,
    0,   315,   316,     0,     0,     0,     0,     0,     0,   316,
    0,     0,     0,     0,     0,   317,     0,   313,     0,    91,
    92,    93,   317,    94,    95,    96,     0,     0,    97,    98,
    372,     0,     0,    99,   314,     0,   315,   372,     0,   702,
    0,     0,     0,     0,   316,   319,     0,   320,     0,     0,
    0,     0,   319,   100,   320,   101,     0,   317,     0,     0,
    103,   104,   105,     0,   106,   107,     0,     0,     0,     0,
    0,     0,   372,   108,     0,   109,     0,     0,     0,   110,
    0,     0,   111,   112,   113,   114,     0,   319,     0,   320,
    115,   116,     0,   117,     0,     0,     0,     0,     0,     0,
    118,   119,   120,   121,     0,     0,     0,     0,     0,   122,
    0,   123,   124,   125,     0,     0,     0,   126,     0,   127,
    0,     0,     0,     0,     0,   128,   129,   130,   131,   132,
    0,     0,   133,     0,     0,     0,     0,     0,     0,     0,
    0,     0,   134,     0,   209,    91,    92,    93,     0,    94,
    95,    96,     0,     0,    97,    98,     0,     0,     0,   238,
    0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
    0,     0,     0,     0,     0,     0,     0,     0,     0,   100,
    0,   101,     0,     0,     0,     0,   103,   104,   105,     0,
    106,   107,     0,     0,   239,     0,     0,     0,     0,   108,
    0,   109,     0,     0,     0,   110,     0,     0,   111,   112,
    113,   114,     0,     0,     0,     0,   115,   116,     0,   117,
    0,     0,     0,     0,     0,     0,   118,   119,   120,   121,
    0,     0,     0,     0,     0,   122,     0,   123,   124,   125,
    0,     0,     0,   126,     0,   127,     0,     0,     0,     0,
    0,   128,   129,   130,   131,   132,     0,     0,   133,     0,
    0,     0,   240,     0,     0,     0,     0,   241,   134,    91,
    92,    93,     0,    94,    95,    96,     0,     0,    97,    98,
    0,     0,     0,   238,     0,     0,     0,     0,     0,     0,
    0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
    0,     0,     0,   100,     0,   101,     0,     0,     0,     0,
    103,   104,   105,     0,   106,   107,     0,     0,   239,     0,
    0,     0,     0,   108,     0,   109,     0,     0,     0,   110,
    0,     0,   111,   112,   113,   114,     0,     0,     0,     0,
    115,   116,     0,   117,     0,     0,     0,     0,     0,     0,
    118,   119,   120,   121,     0,     0,     0,     0,     0,   122,
    0,   123,   124,   125,     0,     0,     0,   126,     0,   127,
    0,     0,     0,     0,     0,   128,   129,   130,   131,   132,
    0,     0,   133,     0,     0,     0,   248,     0,     0,     0,
    0,   241,   134,    91,   275,   276,   277,   278,   279,   280,
    281,   282,   283,   284,     0,     0,     0,   285,     0,     0,
    0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
    0,     0,     0,     0,     0,     0,     0,   100,     0,   101,
    0,     0,     0,     0,   103,   104,   105,     0,   106,   107,
    0,     0,     0,     0,     0,     0,     0,   108,     0,   109,
    0,     0,     0,   110,     0,     0,   111,   112,   113,   114,
    0,     0,     0,     0,   115,   116,     0,   117,     0,     0,
    0,     0,     0,     0,   118,   119,   120,   121,     0,     0,
    0,     0,     0,   122,     0,   123,   124,   125,     0,     0,
    0,   126,     0,   127,     0,     0,     0,     0,     0,   128,
    129,   130,   131,   132,     0,     0,   133,     0,    91,    92,
    93,     0,    94,    95,    96,     0,   134,    97,    98,     0,
    0,     0,    99,     0,     0,     0,     0,   653,     0,     0,
    0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
    0,     0,   100,     0,   101,     0,     0,     0,     0,   103,
    104,   105,     0,   106,   107,     0,     0,     0,     0,   654,
    0,     0,   108,     0,   109,     0,     0,     0,   110,     0,
    0,   111,   112,   113,   114,     0,     0,     0,     0,   115,
    116,     0,   117,     0,     0,     0,     0,     0,     0,   118,
    119,   120,   121,     0,   655,     0,     0,     0,   122,     0,
    123,   124,   125,     0,     0,     0,   126,     0,   127,     0,
    0,     0,     0,     0,   128,   129,   130,   131,   132,     0,
    0,   133,     0,    91,    92,    93,     0,    94,    95,    96,
    0,   134,    97,    98,     0,     0,     0,    99,     0,     0,
    0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
    0,     0,     0,     0,     0,     0,     0,   100,     0,   101,
    0,     0,     0,     0,   103,   104,   105,     0,   106,   107,
    0,     0,     0,     0,     0,     0,     0,   108,     0,   109,
    0,   251,     0,   110,     0,     0,   111,   112,   113,   114,
    0,   252,     0,     0,   115,   116,     0,   117,     0,     0,
    0,     0,     0,     0,   118,   119,   120,   121,     0,     0,
    0,     0,     0,   122,     0,   123,   124,   125,     0,     0,
    0,   126,     0,   127,     0,     0,     0,     0,     0,   128,
    129,   130,   131,   132,     0,     0,   133,     0,    91,    92,
    93,     0,    94,    95,    96,     0,   134,    97,    98,     0,
    0,     0,   238,     0,     0,     0,     0,     0,     0,     0,
    0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
    0,     0,   100,     0,   101,     0,     0,     0,     0,   103,
    104,   105,     0,   106,   107,     0,     0,   239,     0,     0,
    0,     0,   108,     0,   109,     0,     0,     0,   110,     0,
    0,   111,   112,   113,   114,     0,     0,     0,     0,   115,
    116,     0,   117,     0,     0,     0,     0,     0,     0,   118,
    119,   120,   121,     0,     0,     0,     0,     0,   122,     0,
    123,   124,   125,     0,     0,     0,   126,     0,   127,     0,
    0,     0,     0,     0,   128,   129,   130,   131,   132,     0,
    0,   133,     0,    91,    92,    93,     0,    94,    95,    96,
    241,   134,    97,    98,     0,     0,     0,    99,     0,     0,
    0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
    0,     0,     0,     0,     0,     0,     0,   100,     0,   101,
    0,   102,     0,     0,   103,   104,   105,     0,   106,   107,
    0,     0,     0,     0,     0,     0,     0,   108,     0,   109,
    0,     0,     0,   110,     0,     0,   111,   112,   113,   114,
    0,     0,     0,     0,   115,   116,     0,   117,     0,     0,
    0,     0,     0,     0,   118,   119,   120,   121,     0,     0,
    0,     0,     0,   122,     0,   123,   124,   125,     0,     0,
    0,   126,     0,   127,     0,     0,     0,     0,     0,   128,
    129,   130,   131,   132,     0,     0,   133,     0,    91,    92,
    93,     0,    94,    95,    96,     0,   134,    97,    98,     0,
    0,     0,   238,     0,     0,     0,     0,     0,     0,     0,
    0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
    0,     0,   100,     0,   101,     0,     0,     0,     0,   103,
    104,   105,     0,   106,   107,     0,     0,   239,     0,     0,
    0,     0,   108,     0,   109,     0,     0,     0,   110,     0,
    0,   111,   112,   113,   114,     0,     0,     0,     0,   115,
    116,     0,   117,     0,     0,     0,     0,     0,     0,   118,
    119,   120,   121,     0,     0,     0,     0,     0,   122,     0,
    123,   124,   125,     0,     0,     0,   126,     0,   127,     0,
    0,     0,     0,     0,   128,   129,   130,   131,   132,     0,
    0,   133,     0,    91,    92,    93,     0,    94,    95,    96,
    0,   134,    97,    98,     0,     0,     0,    99,     0,     0,
    0,     0,   650,     0,     0,     0,     0,     0,     0,     0,
    0,     0,     0,     0,     0,     0,     0,   100,     0,   101,
    0,     0,     0,     0,   103,   104,   105,     0,   106,   107,
    0,     0,     0,     0,     0,     0,     0,   108,     0,   109,
    0,     0,     0,   110,     0,     0,   111,   112,   113,   114,
    0,     0,     0,     0,   115,   116,     0,   117,     0,     0,
    0,     0,     0,     0,   118,   119,   120,   121,     0,     0,
    0,     0,     0,   122,     0,   123,   124,   125,     0,     0,
    0,   126,     0,   127,     0,     0,     0,     0,     0,   128,
    129,   130,   131,   132,     0,     0,   133,     0,    91,    92,
    93,     0,    94,    95,    96,     0,   134,    97,    98,     0,
    0,     0,   238,     0,     0,     0,     0,     0,     0,     0,
    0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
    0,     0,   100,     0,   101,     0,     0,     0,     0,   103,
    104,   105,     0,   106,   107,     0,     0,     0,     0,     0,
    0,     0,   108,     0,   109,     0,     0,     0,   110,     0,
    0,   111,   112,   113,   114,     0,     0,     0,     0,   115,
    116,     0,   117,     0,     0,     0,     0,     0,     0,   118,
    119,   120,   121,     0,     0,     0,     0,     0,   122,     0,
    123,   124,   125,     0,     0,     0,   126,     0,   127,     0,
    0,     0,     0,     0,   128,   129,   130,   131,   132,     0,
    0,   133,     0,    91,    92,    93,     0,    94,    95,    96,
    241,   134,    97,    98,     0,     0,     0,    99,     0,     0,
    0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
    0,     0,     0,     0,     0,     0,     0,   100,     0,   101,
    0,     0,     0,     0,   103,   104,   105,     0,   106,   107,
    0,     0,     0,     0,     0,     0,     0,   108,     0,   109,
    0,     0,     0,   110,     0,     0,   111,   112,   113,   114,
    0,     0,     0,     0,   115,   116,     0,   117,     0,     0,
    0,     0,     0,     0,   118,   119,   120,   121,     0,     0,
    0,     0,     0,   122,     0,   123,   124,   125,     0,     0,
    0,   126,     0,   127,     0,     0,     0,     0,     0,   128,
    129,   130,   131,   132,     0,     0,   133,     0,    91,    92,
    93,     0,    94,    95,    96,     0,   134,    97,    98,     0,
    0,     0,   238,     0,     0,     0,     0,     0,     0,     0,
    0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
    0,     0,   100,     0,   101,     0,     0,     0,     0,   103,
    104,   105,     0,   106,   107,     0,     0,     0,     0,     0,
    0,     0,   108,     0,   109,     0,     0,     0,   110,     0,
    0,   111,   112,   113,   114,     0,     0,     0,     0,   115,
    116,     0,   117,     0,     0,     0,     0,     0,     0,   118,
    119,   120,   121,     0,     0,     0,     0,     0,   122,     0,
    123,   124,   125,     0,     0,     0,   126,     0,   127,     0,
    0,     0,     0,     0,   128,   129,   130,   131,   132,     0,
    0,   133,     0,     0,     0,     0,     0,     0,     0,     0,
    0,   134
};

static const yytype_int16 yycheck[] = {
    59,   441,    61,   162,   205,   427,    62,    63,   205,   124,
    125,   205,   205,   373,   218,   353,   495,   431,   324,   440,
    185,   417,   223,    17,   437,     3,     3,   610,     6,     6,
    533,    19,   205,    54,   223,    19,   237,    17,     3,   745,
    121,     6,    54,   102,    97,    98,    76,    17,   237,   130,
    223,    40,    30,   419,   420,     3,    54,   223,   119,   223,
    3,     4,   565,     6,   237,     8,     9,   493,    89,   106,
    13,   237,   802,   237,    17,    18,    19,    89,   121,   809,
    786,     3,   503,    54,     6,   223,     8,    30,    31,   515,
    223,    89,    21,    22,   131,    17,    18,    19,     3,   237,
    121,     6,   132,   234,   237,    94,   165,   222,    30,    31,
    17,    29,    17,    18,    19,    21,    22,    36,    89,   108,
    21,    22,    30,    30,    58,    30,    31,   125,    62,   318,
    118,   125,   120,   223,   118,   191,    54,   196,   197,   198,
    196,   118,   582,   199,   200,   125,   205,   237,    51,    52,
    209,   734,   318,   223,   119,   125,    21,    22,   136,   324,
    581,   217,   221,    56,   223,   296,    54,   237,   227,   120,
    89,    88,   373,    92,   540,   118,   121,   120,   237,   238,
    3,    98,   578,     6,   373,   130,   102,   318,   117,   610,
    120,   120,   251,   252,   124,   124,   118,   126,   120,   612,
    373,    89,   121,     0,   121,   102,     3,   373,    31,   373,
    54,   117,     3,   118,     5,   120,   117,    36,   124,   120,
    126,   127,   102,   124,    88,   126,   648,     6,     7,   418,
    283,   284,   646,    12,    98,   373,   649,   125,   439,   440,
    373,   372,   439,    41,     3,    89,    37,     6,   301,   302,
    303,   120,   117,   368,   560,   124,    42,   121,   459,   124,
    461,   126,   459,   107,   461,   125,   604,   440,   128,     7,
    89,    30,   431,    92,    12,    61,    23,   121,    69,    26,
    27,    28,    29,   373,   485,    71,     3,   418,   485,   122,
    7,   485,   485,   497,     3,    12,    58,     6,    15,    43,
    24,    25,   503,   373,    28,   506,   503,   440,    21,    22,
    369,   120,   485,   734,   373,   124,   795,   506,   758,   520,
    72,    30,   523,    23,    24,    25,    26,    27,    28,    29,
    503,   520,   533,   506,   523,   756,   124,   125,   698,   120,
    506,   542,   506,   124,   533,   542,   119,   520,   407,   408,
    523,   120,   411,   412,   520,   124,   520,   523,   119,   523,
    533,   757,   493,   119,   565,   424,   119,   533,   506,   533,
    503,   120,   123,   506,   120,   124,   565,   120,   124,   580,
    581,   124,   520,   580,   515,   523,   445,   520,   447,   445,
    523,   119,   565,   120,   732,   533,   511,   124,   124,   565,
    533,   565,    24,   128,    26,   130,    28,    29,   581,   610,
    120,   771,   120,   610,   124,     3,   506,     5,   120,   120,
    123,   120,   124,   124,   625,   124,   485,   565,   625,   120,
    520,   625,   565,   523,   120,   120,   506,   610,   639,   124,
    442,   443,   639,   533,   503,   604,   784,   506,   581,    37,
    520,   120,   625,   523,   120,    65,   515,    65,    66,    67,
    41,   520,    50,   533,   523,   596,     6,   119,     8,   600,
    3,   125,   119,   128,   533,   565,    55,   610,     4,    99,
    6,    69,     8,     9,    99,   119,    90,    13,   547,   548,
    119,   547,   548,    88,   119,   565,    70,   698,    99,     4,
    5,   702,     7,     8,     9,   702,   565,    12,    13,   698,
    4,     5,     6,     7,     8,     9,    10,    11,    12,    13,
    125,   121,   121,    17,    47,   698,   119,    17,    17,   119,
    88,    88,   698,   734,   698,   736,   737,   734,    88,   736,
    737,   119,   736,   737,   737,    88,   747,    88,    41,   125,
    747,   610,   120,   124,   755,   756,   120,   124,   755,   125,
    698,   734,   125,   736,   737,   698,   625,   119,    41,   625,
    771,   125,     5,    41,    41,   130,   777,   125,   125,   694,
    777,    41,   771,   756,     4,     5,     6,     7,     8,     9,
    10,    11,    12,    13,   125,   654,    30,   125,   771,    17,
    125,   734,    17,   121,   120,   771,    60,   771,   698,     4,
    5,     6,     7,     8,     9,   124,    11,    12,    13,    47,
    121,   125,   121,   756,   121,   125,   129,   125,   698,   124,
    14,    54,   128,   771,   125,   127,    16,   125,   771,   698,
    4,     5,     6,     7,     8,     9,   125,   125,    12,    13,
    23,    24,    25,    26,    27,    28,    29,     3,     4,     5,
    6,     7,     8,     9,   125,    41,    12,    13,   125,    15,
    41,    17,    41,    41,   121,   734,   124,   736,   737,   120,
    736,   771,   120,   120,    30,    31,    32,    33,    34,    35,
    121,    37,    38,    39,   124,   119,   122,   119,    44,    45,
    46,   771,    48,    49,   125,   122,   125,    53,     6,   125,
    120,    57,   771,    59,   125,   122,   122,    63,    64,    65,
    66,    67,    68,    69,   124,   124,   130,    73,    74,    75,
    30,    77,   125,   125,    80,    81,   125,    83,    84,    85,
    86,    87,    28,    18,    17,    91,   125,    93,   122,    95,
    96,    97,    98,   125,   124,   101,   122,   103,   104,   120,
    119,    89,    89,   109,   110,   111,   112,   113,   115,   115,
    116,   125,   120,   119,   124,   121,   125,   122,   122,   125,
    126,   122,   120,   125,   124,   120,   125,   130,   134,    54,
    136,     3,     4,     5,     6,     7,     8,     9,   130,   124,
    12,    13,   133,    15,   130,    17,    54,    54,    54,   100,
    125,    30,   124,    17,   125,   124,    54,   124,    30,    31,
    32,    33,    34,    35,    17,    37,    38,    39,   124,   124,
    119,   125,    44,    45,    46,    30,    48,    49,    17,   124,
    120,    53,   120,     2,    11,    57,    56,    59,    68,   318,
    732,    63,    64,    65,    66,    67,    68,    69,   162,   763,
    602,    73,    74,    75,   520,    77,   523,   386,    80,    81,
    237,    83,    84,    85,    86,    87,   506,   702,   741,    91,
    736,    93,   776,    95,    96,    97,    98,   773,   659,   101,
    801,   103,   104,   587,   503,   296,   445,   109,   110,   111,
    112,   113,   598,   115,   116,    -1,   596,   119,   473,   121,
    -1,    -1,    -1,   125,   126,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,   134,    -1,   136,     3,     4,     5,     6,     7,
    8,     9,    -1,    -1,    12,    13,    -1,    15,    -1,    17,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    30,    31,    32,    33,    34,    35,    -1,    37,
    38,    39,    -1,    -1,    -1,    -1,    44,    45,    46,    -1,
    48,    49,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    57,
    -1,    59,    -1,    -1,    -1,    63,    64,    65,    66,    67,
    68,    69,    -1,    -1,    -1,    73,    74,    75,    -1,    77,
    -1,    -1,    80,    81,    -1,    83,    84,    85,    86,    87,
    -1,    -1,    -1,    91,    -1,    93,    -1,    95,    96,    97,
    98,    -1,    -1,   101,    -1,   103,   104,    -1,    -1,    -1,
    -1,   109,   110,   111,   112,   113,    -1,   115,   116,    -1,
    -1,   119,    -1,   121,    -1,    -1,    -1,   125,   126,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,   134,    -1,   136,     3,
    4,     5,     6,     7,     8,     9,    -1,    -1,    12,    13,
    -1,    15,    -1,    17,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    30,    31,    32,    33,
    34,    35,    -1,    37,    38,    39,    -1,    -1,    -1,    -1,
    44,    45,    46,    -1,    48,    49,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    57,    -1,    59,    -1,    -1,    -1,    63,
    64,    65,    66,    67,    68,    69,    -1,    -1,    -1,    73,
    74,    75,    -1,    77,    -1,    -1,    80,    81,    -1,    83,
    84,    85,    86,    87,    -1,    -1,    -1,    91,    -1,    93,
    -1,    95,    96,    97,    98,    -1,    -1,   101,    -1,   103,
    104,    -1,    -1,    -1,    -1,   109,   110,   111,   112,   113,
    -1,   115,   116,    -1,    -1,   119,    -1,   121,    -1,    -1,
    -1,   125,   126,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    134,    -1,   136,     3,     4,     5,     6,     7,     8,     9,
    -1,    -1,    12,    13,    -1,    15,    -1,    17,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    30,    31,    32,    33,    34,    35,    -1,    37,    38,    39,
    -1,    -1,    -1,    -1,    44,    45,    46,    -1,    48,    49,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    57,    -1,    59,
    -1,    -1,    -1,    63,    64,    65,    66,    67,    68,    69,
    -1,    -1,    -1,    73,    74,    75,    -1,    77,    -1,    -1,
    80,    81,    -1,    83,    84,    85,    86,    87,    -1,    -1,
    -1,    91,    -1,    93,    -1,    95,    96,    97,    98,    -1,
    -1,   101,    -1,   103,   104,    -1,    -1,    -1,    -1,   109,
    110,   111,   112,   113,    -1,   115,   116,    -1,    -1,   119,
    -1,   121,    -1,    -1,    -1,    -1,   126,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,   134,    -1,   136,     3,     4,     5,
    6,     7,     8,     9,    -1,    -1,    12,    13,    -1,    15,
    -1,    17,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    30,    31,    32,    33,    34,    35,
    -1,    37,    -1,    39,    -1,    -1,    -1,    -1,    44,    45,
    46,    -1,    48,    49,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    57,    -1,    59,    -1,    -1,    -1,    63,    64,    65,
    66,    67,    68,    69,    -1,    -1,    -1,    73,    74,    75,
    -1,    77,    -1,    -1,    80,    81,    -1,    83,    84,    85,
    86,    87,    -1,    -1,    -1,    91,    -1,    93,    -1,    95,
    96,    97,    98,    -1,    -1,   101,    -1,   103,   104,    -1,
    -1,    -1,    -1,   109,   110,   111,   112,   113,    -1,   115,
    116,    -1,    -1,   119,    -1,   121,    -1,    -1,    -1,    -1,
    126,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   134,    -1,
    136,     3,     4,     5,     6,     7,     8,     9,    -1,    -1,
    12,    13,    14,    15,    16,    17,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    30,    31,
    32,    33,    34,    35,    -1,    37,    -1,    39,    -1,    -1,
    -1,    -1,    44,    45,    46,    -1,    48,    49,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    57,    -1,    59,    -1,    -1,
    -1,    63,    64,    -1,    66,    67,    68,    69,    -1,    -1,
    -1,    -1,    74,    75,    -1,    77,    -1,    -1,    -1,    81,
    -1,    83,    84,    85,    86,    87,    -1,    -1,    -1,    91,
    -1,    93,    -1,    95,    96,    97,    -1,    -1,    -1,   101,
    -1,   103,   104,    -1,    -1,    -1,    -1,   109,   110,   111,
    112,   113,    -1,    -1,   116,   117,    -1,   119,   120,    -1,
    -1,    -1,   124,    -1,   126,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,   134,    -1,   136,     3,     4,     5,     6,     7,
    8,     9,    -1,    -1,    12,    13,    14,    15,    16,    17,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    30,    31,    32,    33,    34,    35,    -1,    37,
    -1,    39,    -1,    -1,    -1,    -1,    44,    45,    46,    -1,
    48,    49,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    57,
    -1,    59,    -1,    -1,    -1,    63,    64,    -1,    66,    67,
    68,    69,    -1,    -1,    -1,    -1,    74,    75,    -1,    77,
    -1,    -1,    -1,    81,    -1,    83,    84,    85,    86,    87,
    -1,    -1,    -1,    91,    -1,    93,    -1,    95,    96,    97,
    -1,    -1,    -1,   101,    -1,   103,   104,    -1,    -1,    -1,
    -1,   109,   110,   111,   112,   113,    -1,    -1,   116,    -1,
    -1,   119,    -1,    -1,    -1,    -1,    -1,    -1,   126,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,   134,    -1,   136,     3,
    4,     5,     6,     7,     8,     9,    -1,    -1,    12,    13,
    -1,    -1,    -1,    17,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    30,    31,    32,    33,
    34,    35,    -1,    37,    -1,    39,    -1,    -1,    -1,    -1,
    44,    45,    46,    -1,    48,    49,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    57,    -1,    59,    -1,    -1,    -1,    63,
    64,    -1,    66,    67,    68,    69,    -1,    -1,    -1,    -1,
    74,    75,    -1,    77,    -1,    -1,    -1,    81,    -1,    83,
    84,    85,    86,    87,    -1,    -1,    -1,    91,    -1,    93,
    -1,    95,    96,    97,    -1,    -1,    -1,   101,    -1,   103,
    104,    -1,    -1,    -1,    -1,   109,   110,   111,   112,   113,
    -1,    -1,   116,    -1,    -1,   119,    -1,    -1,    -1,    -1,
    -1,    -1,   126,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    134,    -1,   136,     3,     4,     5,     6,     7,     8,     9,
    -1,    -1,    12,    13,    -1,    -1,    -1,    17,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    30,    -1,    -1,    -1,    -1,    -1,    -1,    37,    -1,    39,
    -1,    -1,    -1,    -1,    44,    45,    46,    -1,    48,    49,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    57,    -1,    59,
    -1,    -1,    -1,    63,    -1,    -1,    66,    67,    68,    69,
    -1,    -1,    -1,    -1,    74,    75,    -1,    77,    -1,    -1,
    -1,    -1,    -1,    -1,    84,    85,    86,    87,    -1,    -1,
    -1,    -1,    -1,    93,    -1,    95,    96,    97,    -1,    -1,
    -1,   101,    -1,   103,    -1,    -1,    -1,    -1,    -1,   109,
    110,   111,   112,   113,     3,    -1,   116,     6,     7,    -1,
    -1,     3,    -1,    12,     6,     7,   126,    -1,    17,    -1,
    12,    -1,    -1,    -1,    -1,    17,   136,    -1,    -1,    -1,
    -1,    30,    31,    32,    33,    34,    35,    -1,    30,    31,
    32,    33,    34,    35,    -1,    -1,     3,    -1,    -1,     6,
    7,    -1,    -1,    -1,    -1,    12,    -1,    -1,    -1,    -1,
    17,    -1,    -1,    -1,    -1,    64,    -1,    -1,    -1,    -1,
    -1,    -1,    64,    30,    31,    32,    33,    34,    35,    -1,
    79,    -1,    81,    -1,    83,    -1,    -1,    79,    -1,    81,
    -1,    83,    91,    -1,    -1,    -1,    -1,    -1,    -1,    91,
    -1,    -1,    -1,    -1,    -1,   104,    -1,    64,    -1,     3,
    4,     5,   104,     7,     8,     9,    -1,    -1,    12,    13,
    119,    -1,    -1,    17,    81,    -1,    83,   119,    -1,   128,
    -1,    -1,    -1,    -1,    91,   134,    -1,   136,    -1,    -1,
    -1,    -1,   134,    37,   136,    39,    -1,   104,    -1,    -1,
    44,    45,    46,    -1,    48,    49,    -1,    -1,    -1,    -1,
    -1,    -1,   119,    57,    -1,    59,    -1,    -1,    -1,    63,
    -1,    -1,    66,    67,    68,    69,    -1,   134,    -1,   136,
    74,    75,    -1,    77,    -1,    -1,    -1,    -1,    -1,    -1,
    84,    85,    86,    87,    -1,    -1,    -1,    -1,    -1,    93,
    -1,    95,    96,    97,    -1,    -1,    -1,   101,    -1,   103,
    -1,    -1,    -1,    -1,    -1,   109,   110,   111,   112,   113,
    -1,    -1,   116,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,   126,    -1,   128,     3,     4,     5,    -1,     7,
    8,     9,    -1,    -1,    12,    13,    -1,    -1,    -1,    17,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    37,
    -1,    39,    -1,    -1,    -1,    -1,    44,    45,    46,    -1,
    48,    49,    -1,    -1,    52,    -1,    -1,    -1,    -1,    57,
    -1,    59,    -1,    -1,    -1,    63,    -1,    -1,    66,    67,
    68,    69,    -1,    -1,    -1,    -1,    74,    75,    -1,    77,
    -1,    -1,    -1,    -1,    -1,    -1,    84,    85,    86,    87,
    -1,    -1,    -1,    -1,    -1,    93,    -1,    95,    96,    97,
    -1,    -1,    -1,   101,    -1,   103,    -1,    -1,    -1,    -1,
    -1,   109,   110,   111,   112,   113,    -1,    -1,   116,    -1,
    -1,    -1,   120,    -1,    -1,    -1,    -1,   125,   126,     3,
    4,     5,    -1,     7,     8,     9,    -1,    -1,    12,    13,
    -1,    -1,    -1,    17,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    37,    -1,    39,    -1,    -1,    -1,    -1,
    44,    45,    46,    -1,    48,    49,    -1,    -1,    52,    -1,
    -1,    -1,    -1,    57,    -1,    59,    -1,    -1,    -1,    63,
    -1,    -1,    66,    67,    68,    69,    -1,    -1,    -1,    -1,
    74,    75,    -1,    77,    -1,    -1,    -1,    -1,    -1,    -1,
    84,    85,    86,    87,    -1,    -1,    -1,    -1,    -1,    93,
    -1,    95,    96,    97,    -1,    -1,    -1,   101,    -1,   103,
    -1,    -1,    -1,    -1,    -1,   109,   110,   111,   112,   113,
    -1,    -1,   116,    -1,    -1,    -1,   120,    -1,    -1,    -1,
    -1,   125,   126,     3,     4,     5,     6,     7,     8,     9,
    10,    11,    12,    13,    -1,    -1,    -1,    17,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    37,    -1,    39,
    -1,    -1,    -1,    -1,    44,    45,    46,    -1,    48,    49,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    57,    -1,    59,
    -1,    -1,    -1,    63,    -1,    -1,    66,    67,    68,    69,
    -1,    -1,    -1,    -1,    74,    75,    -1,    77,    -1,    -1,
    -1,    -1,    -1,    -1,    84,    85,    86,    87,    -1,    -1,
    -1,    -1,    -1,    93,    -1,    95,    96,    97,    -1,    -1,
    -1,   101,    -1,   103,    -1,    -1,    -1,    -1,    -1,   109,
    110,   111,   112,   113,    -1,    -1,   116,    -1,     3,     4,
    5,    -1,     7,     8,     9,    -1,   126,    12,    13,    -1,
    -1,    -1,    17,    -1,    -1,    -1,    -1,    22,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    37,    -1,    39,    -1,    -1,    -1,    -1,    44,
    45,    46,    -1,    48,    49,    -1,    -1,    -1,    -1,    54,
    -1,    -1,    57,    -1,    59,    -1,    -1,    -1,    63,    -1,
    -1,    66,    67,    68,    69,    -1,    -1,    -1,    -1,    74,
    75,    -1,    77,    -1,    -1,    -1,    -1,    -1,    -1,    84,
    85,    86,    87,    -1,    89,    -1,    -1,    -1,    93,    -1,
    95,    96,    97,    -1,    -1,    -1,   101,    -1,   103,    -1,
    -1,    -1,    -1,    -1,   109,   110,   111,   112,   113,    -1,
    -1,   116,    -1,     3,     4,     5,    -1,     7,     8,     9,
    -1,   126,    12,    13,    -1,    -1,    -1,    17,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    37,    -1,    39,
    -1,    -1,    -1,    -1,    44,    45,    46,    -1,    48,    49,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    57,    -1,    59,
    -1,    61,    -1,    63,    -1,    -1,    66,    67,    68,    69,
    -1,    71,    -1,    -1,    74,    75,    -1,    77,    -1,    -1,
    -1,    -1,    -1,    -1,    84,    85,    86,    87,    -1,    -1,
    -1,    -1,    -1,    93,    -1,    95,    96,    97,    -1,    -1,
    -1,   101,    -1,   103,    -1,    -1,    -1,    -1,    -1,   109,
    110,   111,   112,   113,    -1,    -1,   116,    -1,     3,     4,
    5,    -1,     7,     8,     9,    -1,   126,    12,    13,    -1,
    -1,    -1,    17,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    37,    -1,    39,    -1,    -1,    -1,    -1,    44,
    45,    46,    -1,    48,    49,    -1,    -1,    52,    -1,    -1,
    -1,    -1,    57,    -1,    59,    -1,    -1,    -1,    63,    -1,
    -1,    66,    67,    68,    69,    -1,    -1,    -1,    -1,    74,
    75,    -1,    77,    -1,    -1,    -1,    -1,    -1,    -1,    84,
    85,    86,    87,    -1,    -1,    -1,    -1,    -1,    93,    -1,
    95,    96,    97,    -1,    -1,    -1,   101,    -1,   103,    -1,
    -1,    -1,    -1,    -1,   109,   110,   111,   112,   113,    -1,
    -1,   116,    -1,     3,     4,     5,    -1,     7,     8,     9,
    125,   126,    12,    13,    -1,    -1,    -1,    17,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    37,    -1,    39,
    -1,    41,    -1,    -1,    44,    45,    46,    -1,    48,    49,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    57,    -1,    59,
    -1,    -1,    -1,    63,    -1,    -1,    66,    67,    68,    69,
    -1,    -1,    -1,    -1,    74,    75,    -1,    77,    -1,    -1,
    -1,    -1,    -1,    -1,    84,    85,    86,    87,    -1,    -1,
    -1,    -1,    -1,    93,    -1,    95,    96,    97,    -1,    -1,
    -1,   101,    -1,   103,    -1,    -1,    -1,    -1,    -1,   109,
    110,   111,   112,   113,    -1,    -1,   116,    -1,     3,     4,
    5,    -1,     7,     8,     9,    -1,   126,    12,    13,    -1,
    -1,    -1,    17,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    37,    -1,    39,    -1,    -1,    -1,    -1,    44,
    45,    46,    -1,    48,    49,    -1,    -1,    52,    -1,    -1,
    -1,    -1,    57,    -1,    59,    -1,    -1,    -1,    63,    -1,
    -1,    66,    67,    68,    69,    -1,    -1,    -1,    -1,    74,
    75,    -1,    77,    -1,    -1,    -1,    -1,    -1,    -1,    84,
    85,    86,    87,    -1,    -1,    -1,    -1,    -1,    93,    -1,
    95,    96,    97,    -1,    -1,    -1,   101,    -1,   103,    -1,
    -1,    -1,    -1,    -1,   109,   110,   111,   112,   113,    -1,
    -1,   116,    -1,     3,     4,     5,    -1,     7,     8,     9,
    -1,   126,    12,    13,    -1,    -1,    -1,    17,    -1,    -1,
    -1,    -1,    22,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    37,    -1,    39,
    -1,    -1,    -1,    -1,    44,    45,    46,    -1,    48,    49,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    57,    -1,    59,
    -1,    -1,    -1,    63,    -1,    -1,    66,    67,    68,    69,
    -1,    -1,    -1,    -1,    74,    75,    -1,    77,    -1,    -1,
    -1,    -1,    -1,    -1,    84,    85,    86,    87,    -1,    -1,
    -1,    -1,    -1,    93,    -1,    95,    96,    97,    -1,    -1,
    -1,   101,    -1,   103,    -1,    -1,    -1,    -1,    -1,   109,
    110,   111,   112,   113,    -1,    -1,   116,    -1,     3,     4,
    5,    -1,     7,     8,     9,    -1,   126,    12,    13,    -1,
    -1,    -1,    17,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    37,    -1,    39,    -1,    -1,    -1,    -1,    44,
    45,    46,    -1,    48,    49,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    57,    -1,    59,    -1,    -1,    -1,    63,    -1,
    -1,    66,    67,    68,    69,    -1,    -1,    -1,    -1,    74,
    75,    -1,    77,    -1,    -1,    -1,    -1,    -1,    -1,    84,
    85,    86,    87,    -1,    -1,    -1,    -1,    -1,    93,    -1,
    95,    96,    97,    -1,    -1,    -1,   101,    -1,   103,    -1,
    -1,    -1,    -1,    -1,   109,   110,   111,   112,   113,    -1,
    -1,   116,    -1,     3,     4,     5,    -1,     7,     8,     9,
    125,   126,    12,    13,    -1,    -1,    -1,    17,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    37,    -1,    39,
    -1,    -1,    -1,    -1,    44,    45,    46,    -1,    48,    49,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    57,    -1,    59,
    -1,    -1,    -1,    63,    -1,    -1,    66,    67,    68,    69,
    -1,    -1,    -1,    -1,    74,    75,    -1,    77,    -1,    -1,
    -1,    -1,    -1,    -1,    84,    85,    86,    87,    -1,    -1,
    -1,    -1,    -1,    93,    -1,    95,    96,    97,    -1,    -1,
    -1,   101,    -1,   103,    -1,    -1,    -1,    -1,    -1,   109,
    110,   111,   112,   113,    -1,    -1,   116,    -1,     3,     4,
    5,    -1,     7,     8,     9,    -1,   126,    12,    13,    -1,
    -1,    -1,    17,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    37,    -1,    39,    -1,    -1,    -1,    -1,    44,
    45,    46,    -1,    48,    49,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    57,    -1,    59,    -1,    -1,    -1,    63,    -1,
    -1,    66,    67,    68,    69,    -1,    -1,    -1,    -1,    74,
    75,    -1,    77,    -1,    -1,    -1,    -1,    -1,    -1,    84,
    85,    86,    87,    -1,    -1,    -1,    -1,    -1,    93,    -1,
    95,    96,    97,    -1,    -1,    -1,   101,    -1,   103,    -1,
    -1,    -1,    -1,    -1,   109,   110,   111,   112,   113,    -1,
    -1,   116,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,   126
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint16 yystos[] = {
    0,     3,   138,   139,   119,   141,     0,   139,    17,    30,
    142,   143,   144,    56,   121,   120,   142,    42,    61,    71,
    145,    30,   102,   102,   102,    41,   122,    43,   140,    62,
    146,   147,     4,     5,     6,     7,     8,     9,    10,    11,
    12,    13,   148,   156,   157,    58,    72,   149,   119,   119,
    119,   119,   119,   123,   124,   150,   151,   152,   156,     4,
    5,     6,     7,     8,     9,    11,    12,    13,   158,   159,
    160,   162,   262,   263,   265,   318,   319,   321,   323,   325,
    327,   339,   120,   120,   120,   120,   120,   156,   123,   152,
    65,     3,     4,     5,     7,     8,     9,    12,    13,    17,
    37,    39,    41,    44,    45,    46,    48,    49,    57,    59,
    63,    66,    67,    68,    69,    74,    75,    77,    84,    85,
    86,    87,    93,    95,    96,    97,   101,   103,   109,   110,
    111,   112,   113,   116,   126,   163,   164,   165,   166,   167,
    168,   171,   172,   173,   174,   175,   181,   182,   186,   187,
    188,   189,   190,   191,   192,   194,   195,   196,   197,   198,
    204,   205,   206,   208,   209,   210,   213,   214,   215,   216,
    217,   218,   219,   220,   221,   222,   268,   269,   270,   294,
    295,   296,   297,   298,   314,   316,   332,   333,   371,   373,
    374,    41,   163,     3,   268,   268,   119,   329,   329,   329,
    329,   159,     3,   153,   125,   119,   334,   334,   334,   128,
    55,   163,    99,    99,   119,    90,   119,    88,   119,    70,
    99,    88,    98,   121,   199,   223,   245,    88,   207,   223,
    245,    40,    94,   108,   212,   161,   223,   121,    17,    52,
    120,   125,   163,   179,   180,   200,   201,   202,   120,   179,
    200,    61,    71,   163,   315,   317,   193,    24,    26,    28,
    29,    23,    26,    27,    28,    29,   372,    50,   267,   268,
    271,   340,   125,   264,   266,     4,     5,     6,     7,     8,
    9,    10,    11,    12,    13,    17,   163,   268,   330,   331,
    376,   163,   320,   163,   268,   268,   154,     4,     5,     7,
    8,     9,    12,    13,     3,     6,    17,    30,    31,    32,
    33,    34,    35,    64,    81,    83,    91,   104,   119,   134,
    136,   163,   220,   294,   316,   335,   336,   337,   341,   342,
    343,   344,   345,   349,   350,   352,   353,   354,   355,   358,
    359,   360,   361,   362,   363,   364,   365,   368,   369,   379,
    163,    47,   119,   176,    17,   183,   184,   185,   378,   268,
    17,   377,   378,   163,   223,    15,    38,    53,    65,    73,
    80,   115,   119,   121,   125,   163,   164,   165,   217,   221,
    224,   228,   229,   230,   231,   232,   236,   237,   238,   239,
    241,   243,   244,   245,   246,   251,   294,   296,   298,   301,
    302,   303,   307,   313,   341,   373,   119,    88,    88,   163,
    119,    88,    88,     3,    30,   211,   343,    41,   119,   224,
    256,   257,   258,   163,    88,   125,   120,   124,    89,   203,
    120,   124,   120,   163,   163,   125,   125,   125,   119,    41,
    41,    41,   130,   130,   120,   124,   324,    41,   322,   326,
    328,   119,   155,   343,   345,   334,   334,   334,   125,   130,
    3,    17,    18,    19,    30,    31,   118,   120,   166,   296,
    298,   338,   343,   346,   347,   351,   356,   357,   366,   367,
    30,    24,    25,   370,   120,   124,    17,    17,   169,   170,
    177,   178,   180,   121,   120,   124,   120,   124,    23,    28,
    29,   300,   304,   308,   370,   372,    60,   233,    47,   223,
    163,    51,    52,   229,   125,   129,   225,   124,   106,   131,
    234,    76,   132,   235,   233,   125,   128,   163,   163,   163,
    163,   125,   127,    14,   375,     3,   296,   225,   225,   163,
    125,   201,    54,   179,   202,   299,   300,    21,    22,   272,
    273,   274,   275,   278,   279,   281,   282,   284,     3,    17,
    316,   341,     3,   301,   316,    16,   311,     4,     5,     7,
    8,     9,    12,    13,    17,   376,   376,   331,    41,   163,
    41,    41,    41,     6,   341,   125,   341,   121,   124,   120,
    120,   120,   347,   120,   124,   120,   124,   120,   124,   336,
    121,   120,   124,   120,   124,   343,   379,   125,   185,   378,
    306,   120,   124,   117,   120,   124,   163,   293,   301,   309,
    310,   311,   341,   375,   236,   119,   223,   119,   247,   122,
    125,   163,   226,   343,   379,   122,   125,   231,   232,   125,
    228,   125,   120,   122,   122,   225,   124,   341,   124,   125,
    22,   163,   268,    22,    54,    89,   163,   268,   120,   124,
    125,   125,   125,   228,   312,   375,   341,   301,   311,     6,
    118,   344,   348,    30,    18,   357,   367,    30,   343,   170,
    179,   180,   122,   122,   125,   310,   300,   163,   252,   253,
    254,   255,   268,   336,    17,   125,   248,   249,   124,   227,
    125,    79,   128,   240,   242,   341,   120,   119,   201,   300,
    89,    89,   107,   276,    89,   283,    89,   163,    89,   280,
    89,   285,   115,   286,   273,   125,   120,   122,   120,   124,
    122,   122,   124,   125,   305,   120,   124,   130,    36,    89,
    92,   223,   250,   125,   120,   124,   229,   130,   125,   242,
    133,   259,   260,    89,   277,    54,    54,    54,    54,   100,
    125,    30,   178,   124,   310,   253,   336,   250,   125,   248,
    341,   124,    17,   125,   261,   120,   124,    54,   341,   301,
    375,   311,   287,   124,   124,   184,   124,   229,   261,   125,
    260,   341,   119,   288,    30,   124,   248,    17,    21,    22,
    120,   126,   289,   290,   291,   292,   293,   120,   120,   289,
    120,   290,   127
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
#line 477 "asn_grammar.y"
        {  ;}
        break;

    case 3:

        /* Line 1455 of yacc.c  */
#line 479 "asn_grammar.y"
        {  ;}
        break;

    case 4:

        /* Line 1455 of yacc.c  */
#line 483 "asn_grammar.y"
        {
            Module = FindModule((yyvsp[(1) - (6)].sval)->c_str());
            if ((yyvsp[(2) - (6)].slst)) {
                Module->SetDefinitiveObjId(*(yyvsp[(2) - (6)].slst)); delete(yyvsp[(2) - (6)].slst);
            }
            ;
        }
        break;

    case 5:

        /* Line 1455 of yacc.c  */
#line 490 "asn_grammar.y"
        {
            Module->ResolveObjectClassReferences();
            Module = NULL;
            ;
        }
        break;

    case 6:

        /* Line 1455 of yacc.c  */
#line 498 "asn_grammar.y"
        {
            (yyval.slst) = (yyvsp[(2) - (3)].slst);
            ;
        }
        break;

    case 7:

        /* Line 1455 of yacc.c  */
#line 502 "asn_grammar.y"
        {
            (yyval.slst) = NULL;
            ;
        }
        break;

    case 8:

        /* Line 1455 of yacc.c  */
#line 509 "asn_grammar.y"
        {
            (yyval.slst) = new StringList;
            (yyval.slst)->push_back((yyvsp[(1) - (1)].sval)->c_str());
            delete(yyvsp[(1) - (1)].sval);
            ;
        }
        break;

    case 9:

        /* Line 1455 of yacc.c  */
#line 515 "asn_grammar.y"
        {
            (yyvsp[(2) - (2)].slst)->insert((yyvsp[(2) - (2)].slst)->begin(), std::string(*(yyvsp[(1) - (2)].sval)));
            (yyval.slst) = (yyvsp[(2) - (2)].slst);
            ;
        }
        break;

    case 11:

        /* Line 1455 of yacc.c  */
#line 524 "asn_grammar.y"
        {
            char buf[10];
            sprintf(buf, "%u", static_cast<unsigned>((yyvsp[(1) - (1)].ival)));
            (yyval.sval) = new std::string(buf);
            ;
        }
        break;

    case 13:

        /* Line 1455 of yacc.c  */
#line 534 "asn_grammar.y"
        {
            delete(yyvsp[(1) - (4)].sval);
            char buf[10];
            sprintf(buf, "%u", static_cast<unsigned>((yyvsp[(3) - (4)].ival)));
            (yyval.sval) = new std::string(buf);
            ;
        }
        break;

    case 14:

        /* Line 1455 of yacc.c  */
#line 544 "asn_grammar.y"
        {
            (yyval.ival) = Tag::Explicit;
            ;
        }
        break;

    case 15:

        /* Line 1455 of yacc.c  */
#line 548 "asn_grammar.y"
        {
            (yyval.ival) = Tag::Implicit;
            ;
        }
        break;

    case 16:

        /* Line 1455 of yacc.c  */
#line 552 "asn_grammar.y"
        {
            (yyval.ival) = Tag::Automatic;
            ;
        }
        break;

    case 17:

        /* Line 1455 of yacc.c  */
#line 556 "asn_grammar.y"
        {
            (yyval.ival) = Tag::Explicit;
            ;
        }
        break;

    case 22:

        /* Line 1455 of yacc.c  */
#line 578 "asn_grammar.y"
        {
            /*  Module->SetExports($1); */
            delete(yyvsp[(1) - (1)].syml);
            ;
        }
        break;

    case 23:

        /* Line 1455 of yacc.c  */
#line 583 "asn_grammar.y"
        {
            /*  Module->SetExportAll(); */
            ;
        }
        break;

    case 30:

        /* Line 1455 of yacc.c  */
#line 608 "asn_grammar.y"
        {
            if (!HasObjectTypeMacro) {
                HasObjectTypeMacro = FindWithName(*(yyvsp[(1) - (3)].syml), "OBJECT-TYPE").get() != NULL;
                if (HasObjectTypeMacro)
                    std::cerr << "Info: including OBJECT-TYPE macro" << std::endl;
            }
            Module->AddImport(ImportModulePtr(new ImportModule((yyvsp[(3) - (3)].sval), (yyvsp[(1) - (3)].syml))));
            ;
        }
        break;

    case 31:

        /* Line 1455 of yacc.c  */
#line 621 "asn_grammar.y"
        {
            InOIDContext = 1;
            ;
        }
        break;

    case 32:

        /* Line 1455 of yacc.c  */
#line 625 "asn_grammar.y"
        {
            InOIDContext = 0;
            delete(yyvsp[(3) - (3)].vval);
            ;
        }
        break;

    case 35:

        /* Line 1455 of yacc.c  */
#line 636 "asn_grammar.y"
        {
            (yyval.vval) = NULL;
            ;
        }
        break;

    case 36:

        /* Line 1455 of yacc.c  */
#line 644 "asn_grammar.y"
        {
            (yyval.syml) = new SymbolList;
            (yyval.syml)->push_back(SymbolPtr((yyvsp[(1) - (1)].symb)));
            ;
        }
        break;

    case 37:

        /* Line 1455 of yacc.c  */
#line 649 "asn_grammar.y"
        {
            (yyvsp[(3) - (3)].syml)->push_back(SymbolPtr((yyvsp[(1) - (3)].symb)));
            (yyval.syml) = (yyvsp[(3) - (3)].syml);
            ;
        }
        break;

    case 38:

        /* Line 1455 of yacc.c  */
#line 658 "asn_grammar.y"
        {
            (yyval.symb) = new TypeReference(*(yyvsp[(1) - (1)].sval), false); delete(yyvsp[(1) - (1)].sval);
            ;
        }
        break;

    case 39:

        /* Line 1455 of yacc.c  */
#line 662 "asn_grammar.y"
        {
            (yyval.symb) = new ValueReference(*(yyvsp[(1) - (1)].sval), false); delete(yyvsp[(1) - (1)].sval);
            ;
        }
        break;

    case 40:

        /* Line 1455 of yacc.c  */
#line 666 "asn_grammar.y"
        {
            (yyval.symb) = new ObjectClassReference(*(yyvsp[(1) - (1)].sval), false); delete(yyvsp[(1) - (1)].sval);
            ;
        }
        break;

    case 41:

        /* Line 1455 of yacc.c  */
#line 670 "asn_grammar.y"
        {
            (yyval.symb) = new ObjectReference(*(yyvsp[(1) - (1)].sval), false); delete(yyvsp[(1) - (1)].sval);
            ;
        }
        break;

    case 42:

        /* Line 1455 of yacc.c  */
#line 674 "asn_grammar.y"
        {
            (yyval.symb) = new ObjectSetReference(*(yyvsp[(1) - (1)].sval), false); delete(yyvsp[(1) - (1)].sval);
            ;
        }
        break;

    case 43:

        /* Line 1455 of yacc.c  */
#line 678 "asn_grammar.y"
        {
            (yyval.symb) = new TypeReference(*(yyvsp[(1) - (3)].sval), true); delete(yyvsp[(1) - (3)].sval);
            ;
        }
        break;

    case 44:

        /* Line 1455 of yacc.c  */
#line 682 "asn_grammar.y"
        {
            (yyval.symb) = new ValueReference(*(yyvsp[(1) - (3)].sval), true); delete(yyvsp[(1) - (3)].sval);
            ;
        }
        break;

    case 45:

        /* Line 1455 of yacc.c  */
#line 686 "asn_grammar.y"
        {
            (yyval.symb) = new ObjectClassReference(*(yyvsp[(1) - (3)].sval), true); delete(yyvsp[(1) - (3)].sval);
            ;
        }
        break;

    case 46:

        /* Line 1455 of yacc.c  */
#line 690 "asn_grammar.y"
        {
            (yyval.symb) = new ObjectReference(*(yyvsp[(1) - (3)].sval), true); delete(yyvsp[(1) - (3)].sval);
            ;
        }
        break;

    case 47:

        /* Line 1455 of yacc.c  */
#line 694 "asn_grammar.y"
        {
            (yyval.symb) = new ObjectSetReference(*(yyvsp[(1) - (3)].sval), true); delete(yyvsp[(1) - (3)].sval);
            ;
        }
        break;

    case 57:

        /* Line 1455 of yacc.c  */
#line 720 "asn_grammar.y"
        {
            (yyvsp[(2) - (2)].tval)->SetName(*(yyvsp[(1) - (2)].sval)); delete(yyvsp[(1) - (2)].sval);
            ValueTypeContext.reset((yyvsp[(2) - (2)].tval));
            (yyvsp[(2) - (2)].tval)->BeginParseValueSet();
            ;
        }
        break;

    case 58:

        /* Line 1455 of yacc.c  */
#line 726 "asn_grammar.y"
        {
            Module->AddType((yyvsp[(5) - (5)].vset)->MakeValueSetType());
            delete(yyvsp[(5) - (5)].vset);
            ;
        }
        break;

    case 59:

        /* Line 1455 of yacc.c  */
#line 739 "asn_grammar.y"
        {
            (yyvsp[(3) - (3)].tval)->SetName(*(yyvsp[(1) - (3)].sval)); delete(yyvsp[(1) - (3)].sval);
            Module->AddType(TypePtr((yyvsp[(3) - (3)].tval)));
            ;
        }
        break;

    case 71:

        /* Line 1455 of yacc.c  */
#line 763 "asn_grammar.y"
        { ;}
        break;

    case 87:

        /* Line 1455 of yacc.c  */
#line 784 "asn_grammar.y"
        {
            (yyval.tval) = (yyvsp[(1) - (1)].vset)->MakeValueSetType().get();
            delete(yyvsp[(1) - (1)].vset);
            ;
        }
        break;

    case 88:

        /* Line 1455 of yacc.c  */
#line 793 "asn_grammar.y"
        {
            (yyval.tval) = new DefinedType(*(yyvsp[(1) - (1)].sval));
            delete(yyvsp[(1) - (1)].sval);
            ;
        }
        break;

    case 90:

        /* Line 1455 of yacc.c  */
#line 804 "asn_grammar.y"
        {
            *(yyvsp[(1) - (3)].sval) += *(yyvsp[(3) - (3)].sval);
            delete(yyvsp[(3) - (3)].sval);
            ;
        }
        break;

    case 91:

        /* Line 1455 of yacc.c  */
#line 812 "asn_grammar.y"
        {
            (yyval.tval) = new BitStringType;
            ;
        }
        break;

    case 92:

        /* Line 1455 of yacc.c  */
#line 816 "asn_grammar.y"
        {
            (yyval.tval) = new BitStringType(*(yyvsp[(4) - (5)].nlst));
            delete(yyvsp[(4) - (5)].nlst);
            ;
        }
        break;

    case 93:

        /* Line 1455 of yacc.c  */
#line 825 "asn_grammar.y"
        {
            (yyval.nlst) = new NamedNumberList;
            (yyval.nlst)->push_back(NamedNumberPtr((yyvsp[(1) - (1)].nval)));
            ;
        }
        break;

    case 94:

        /* Line 1455 of yacc.c  */
#line 830 "asn_grammar.y"
        {
            (yyvsp[(1) - (3)].nlst)->insert((yyvsp[(1) - (3)].nlst)->begin(), NamedNumberPtr((yyvsp[(3) - (3)].nval)));
            ;
        }
        break;

    case 95:

        /* Line 1455 of yacc.c  */
#line 837 "asn_grammar.y"
        {
            (yyval.nval) = new NamedNumber((yyvsp[(1) - (4)].sval), (int)(yyvsp[(3) - (4)].ival));
            ;
        }
        break;

    case 96:

        /* Line 1455 of yacc.c  */
#line 841 "asn_grammar.y"
        {
            (yyval.nval) = new NamedNumber((yyvsp[(1) - (4)].sval), ((DefinedValue*)(yyvsp[(3) - (4)].vval))->GetReference());
            delete(yyvsp[(3) - (4)].vval);
            ;
        }
        break;

    case 97:

        /* Line 1455 of yacc.c  */
#line 850 "asn_grammar.y"
        {
            (yyval.tval) = new BooleanType;
            ;
        }
        break;

    case 100:

        /* Line 1455 of yacc.c  */
#line 863 "asn_grammar.y"
        {
            (yyval.tval) = new BMPStringType;
            ;
        }
        break;

    case 101:

        /* Line 1455 of yacc.c  */
#line 867 "asn_grammar.y"
        {
            (yyval.tval) = new GeneralStringType;
            ;
        }
        break;

    case 102:

        /* Line 1455 of yacc.c  */
#line 871 "asn_grammar.y"
        {
            (yyval.tval) = new GraphicStringType;
            ;
        }
        break;

    case 103:

        /* Line 1455 of yacc.c  */
#line 875 "asn_grammar.y"
        {
            (yyval.tval) = new IA5StringType;
            ;
        }
        break;

    case 104:

        /* Line 1455 of yacc.c  */
#line 879 "asn_grammar.y"
        {
            (yyval.tval) = new ISO646StringType;
            ;
        }
        break;

    case 105:

        /* Line 1455 of yacc.c  */
#line 883 "asn_grammar.y"
        {
            (yyval.tval) = new NumericStringType;
            ;
        }
        break;

    case 106:

        /* Line 1455 of yacc.c  */
#line 887 "asn_grammar.y"
        {
            (yyval.tval) = new PrintableStringType;
            ;
        }
        break;

    case 107:

        /* Line 1455 of yacc.c  */
#line 891 "asn_grammar.y"
        {
            (yyval.tval) = new TeletexStringType;
            ;
        }
        break;

    case 108:

        /* Line 1455 of yacc.c  */
#line 895 "asn_grammar.y"
        {
            (yyval.tval) = new T61StringType;
            ;
        }
        break;

    case 109:

        /* Line 1455 of yacc.c  */
#line 899 "asn_grammar.y"
        {
            (yyval.tval) = new UniversalStringType;
            ;
        }
        break;

    case 110:

        /* Line 1455 of yacc.c  */
#line 903 "asn_grammar.y"
        {
            (yyval.tval) = new VideotexStringType;
            ;
        }
        break;

    case 111:

        /* Line 1455 of yacc.c  */
#line 907 "asn_grammar.y"
        {
            (yyval.tval) = new VisibleStringType;
            ;
        }
        break;

    case 112:

        /* Line 1455 of yacc.c  */
#line 915 "asn_grammar.y"
        {
            (yyval.tval) = new UnrestrictedCharacterStringType;
            ;
        }
        break;

    case 113:

        /* Line 1455 of yacc.c  */
#line 923 "asn_grammar.y"
        {
            ParsingConstructedType++;
            ;
        }
        break;

    case 114:

        /* Line 1455 of yacc.c  */
#line 927 "asn_grammar.y"
        {
            (yyval.tval) = (yyvsp[(4) - (5)].tval);
            ParsingConstructedType--;
            ;
        }
        break;

    case 115:

        /* Line 1455 of yacc.c  */
#line 935 "asn_grammar.y"
        {
            (yyval.tval) = new ChoiceType((yyvsp[(1) - (1)].tlst));
            ;
        }
        break;

    case 116:

        /* Line 1455 of yacc.c  */
#line 939 "asn_grammar.y"
        {
            (yyval.tval) = new ChoiceType((yyvsp[(1) - (3)].tlst), true);
            ;
        }
        break;

    case 117:

        /* Line 1455 of yacc.c  */
#line 943 "asn_grammar.y"
        {
            (yyval.tval) = new ChoiceType((yyvsp[(1) - (5)].tlst), true, (yyvsp[(5) - (5)].tlst));
            ;
        }
        break;

    case 118:

        /* Line 1455 of yacc.c  */
#line 950 "asn_grammar.y"
        {
            (yyval.tlst) = new TypesVector;
            (yyval.tlst)->push_back(TypePtr((yyvsp[(1) - (1)].tval)));
            ;
        }
        break;

    case 119:

        /* Line 1455 of yacc.c  */
#line 955 "asn_grammar.y"
        {
            (yyvsp[(1) - (3)].tlst)->push_back(TypePtr((yyvsp[(3) - (3)].tval)));
            ;
        }
        break;

    case 121:

        /* Line 1455 of yacc.c  */
#line 968 "asn_grammar.y"
        {
            (yyvsp[(2) - (2)].tval)->SetName(*(yyvsp[(1) - (2)].sval)); delete(yyvsp[(1) - (2)].sval);
            (yyval.tval) = (yyvsp[(2) - (2)].tval);
            ;
        }
        break;

    case 122:

        /* Line 1455 of yacc.c  */
#line 973 "asn_grammar.y"
        {
            std::cerr << StdError(Warning) << "unnamed field." << std::endl;
            std::stringstream strm;
            strm << "_unnamed" << UnnamedFieldCount++ << std::ends;
            (yyvsp[(1) - (1)].tval)->SetName(strm.str());
            ;
        }
        break;

    case 123:

        /* Line 1455 of yacc.c  */
#line 985 "asn_grammar.y"
        {
            (yyval.tval) = new EmbeddedPDVType;
            ;
        }
        break;

    case 124:

        /* Line 1455 of yacc.c  */
#line 993 "asn_grammar.y"
        {
            (yyval.tval) = (yyvsp[(3) - (4)].tval);
            ;
        }
        break;

    case 125:

        /* Line 1455 of yacc.c  */
#line 1000 "asn_grammar.y"
        {
            (yyval.tval) = new EnumeratedType(*(yyvsp[(1) - (1)].nlst), false, NULL);
            delete(yyvsp[(1) - (1)].nlst);
            ;
        }
        break;

    case 126:

        /* Line 1455 of yacc.c  */
#line 1005 "asn_grammar.y"
        {
            (yyval.tval) = new EnumeratedType(*(yyvsp[(1) - (5)].nlst), true, NULL);
            delete(yyvsp[(1) - (5)].nlst);
            ;
        }
        break;

    case 127:

        /* Line 1455 of yacc.c  */
#line 1010 "asn_grammar.y"
        {
            (yyval.tval) = new EnumeratedType(*(yyvsp[(1) - (7)].nlst), true, (yyvsp[(7) - (7)].nlst));
            delete(yyvsp[(1) - (7)].nlst);
            ;
        }
        break;

    case 128:

        /* Line 1455 of yacc.c  */
#line 1018 "asn_grammar.y"
        {
            (yyval.nlst) = new NamedNumberList;
            (yyval.nlst)->push_back(NamedNumberPtr((yyvsp[(1) - (1)].nval)));
            ;
        }
        break;

    case 129:

        /* Line 1455 of yacc.c  */
#line 1023 "asn_grammar.y"
        {
            (yyvsp[(3) - (3)].nval)->SetAutoNumber(*((yyvsp[(1) - (3)].nlst)->back()));
            (yyvsp[(1) - (3)].nlst)->push_back(NamedNumberPtr((yyvsp[(3) - (3)].nval)));
            (yyval.nlst) = (yyvsp[(1) - (3)].nlst);
            ;
        }
        break;

    case 130:

        /* Line 1455 of yacc.c  */
#line 1032 "asn_grammar.y"
        {
            (yyval.nval) = new NamedNumber((yyvsp[(1) - (1)].sval));
            ;
        }
        break;

    case 132:

        /* Line 1455 of yacc.c  */
#line 1040 "asn_grammar.y"
        {
            (yyval.tval) = new ExternalType;
            ;
        }
        break;

    case 133:

        /* Line 1455 of yacc.c  */
#line 1047 "asn_grammar.y"
        {
            (yyval.tval) = new AnyType(NULL);
            ;
        }
        break;

    case 134:

        /* Line 1455 of yacc.c  */
#line 1051 "asn_grammar.y"
        {
            (yyval.tval) = new AnyType(*(yyvsp[(4) - (4)].sval));
            delete(yyvsp[(4) - (4)].sval);
            ;
        }
        break;

    case 136:

        /* Line 1455 of yacc.c  */
#line 1065 "asn_grammar.y"
        {
            (yyval.tval) = new IntegerType;
            ;
        }
        break;

    case 137:

        /* Line 1455 of yacc.c  */
#line 1069 "asn_grammar.y"
        {
            (yyval.tval) = new IntegerType(*(yyvsp[(3) - (4)].nlst)); delete(yyvsp[(3) - (4)].nlst);
            ;
        }
        break;

    case 138:

        /* Line 1455 of yacc.c  */
#line 1077 "asn_grammar.y"
        {
            (yyval.tval) = new NullType;
            ;
        }
        break;

    case 139:

        /* Line 1455 of yacc.c  */
#line 1085 "asn_grammar.y"
        {
            (yyval.tval)  = (yyvsp[(1) - (1)].ocft);
            ;
        }
        break;

    case 140:

        /* Line 1455 of yacc.c  */
#line 1089 "asn_grammar.y"
        {
            (yyvsp[(1) - (5)].ocft)->AddTableConstraint(boost::shared_ptr<TableConstraint>((yyvsp[(3) - (5)].tcons)));
            (yyval.tval) = (yyvsp[(1) - (5)].ocft);
            ;
        }
        break;

    case 141:

        /* Line 1455 of yacc.c  */
#line 1094 "asn_grammar.y"
        {
            (yyvsp[(1) - (5)].ocft)->AddConstraint(ConstraintPtr((yyvsp[(3) - (5)].cons)));
            (yyval.tval) = (yyvsp[(1) - (5)].ocft);
            ;
        }
        break;

    case 142:

        /* Line 1455 of yacc.c  */
#line 1101 "asn_grammar.y"
        {
            InformationFromObjectContext = (yyvsp[(1) - (1)].dobj);
            ;
        }
        break;

    case 143:

        /* Line 1455 of yacc.c  */
#line 1105 "asn_grammar.y"
        {
            (yyval.ocft) = new ObjectClassFieldType(ObjectClassBasePtr((yyvsp[(1) - (4)].dobj)), *(yyvsp[(4) - (4)].sval)); delete(yyvsp[(4) - (4)].sval);
            InformationFromObjectContext = NULL;
            ;
        }
        break;

    case 144:

        /* Line 1455 of yacc.c  */
#line 1114 "asn_grammar.y"
        {
            (yyval.tval) = new ObjectIdentifierType;
            ;
        }
        break;

    case 145:

        /* Line 1455 of yacc.c  */
#line 1121 "asn_grammar.y"
        {
            (yyval.tval) = new OctetStringType;
            ;
        }
        break;

    case 146:

        /* Line 1455 of yacc.c  */
#line 1129 "asn_grammar.y"
        {
            (yyval.tval) = new RealType;
            ;
        }
        break;

    case 147:

        /* Line 1455 of yacc.c  */
#line 1137 "asn_grammar.y"
        {
            (yyval.tval) = (yyvsp[(2) - (3)].tval);
            ParsingConstructedType--;
            ;
        }
        break;

    case 148:

        /* Line 1455 of yacc.c  */
#line 1142 "asn_grammar.y"
        {
            (yyval.tval) = new SequenceType(NULL, false, NULL);
            ParsingConstructedType--;
            ;
        }
        break;

    case 149:

        /* Line 1455 of yacc.c  */
#line 1147 "asn_grammar.y"
        {
            (yyval.tval) = new SequenceType(NULL, true, NULL);
            ParsingConstructedType--;
            ;
        }
        break;

    case 150:

        /* Line 1455 of yacc.c  */
#line 1155 "asn_grammar.y"
        {
            ParsingConstructedType++;
            ;
        }
        break;

    case 152:

        /* Line 1455 of yacc.c  */
#line 1163 "asn_grammar.y"
        {
            (yyval.tval) = new SequenceType((yyvsp[(1) - (1)].tlst), false, NULL);
            ;
        }
        break;

    case 153:

        /* Line 1455 of yacc.c  */
#line 1167 "asn_grammar.y"
        {
            (yyval.tval) = new SequenceType((yyvsp[(1) - (3)].tlst), true, NULL);
            ;
        }
        break;

    case 154:

        /* Line 1455 of yacc.c  */
#line 1171 "asn_grammar.y"
        {
            (yyval.tval) = new SequenceType((yyvsp[(1) - (5)].tlst), true, (yyvsp[(5) - (5)].tlst));
            ;
        }
        break;

    case 155:

        /* Line 1455 of yacc.c  */
#line 1175 "asn_grammar.y"
        {
            (yyval.tval) = new SequenceType(NULL, true, (yyvsp[(3) - (3)].tlst));
            ;
        }
        break;

    case 156:

        /* Line 1455 of yacc.c  */
#line 1182 "asn_grammar.y"
        {
            (yyval.tlst) = new TypesVector;
            (yyval.tlst)->push_back(*(yyvsp[(1) - (1)].tptr));
            delete(yyvsp[(1) - (1)].tptr);
            ;
        }
        break;

    case 157:

        /* Line 1455 of yacc.c  */
#line 1188 "asn_grammar.y"
        {
            (yyvsp[(1) - (3)].tlst)->push_back(*((yyvsp[(3) - (3)].tptr)));
            delete(yyvsp[(3) - (3)].tptr);
            ;
        }
        break;

    case 158:

        /* Line 1455 of yacc.c  */
#line 1196 "asn_grammar.y"
        {
            (yyval.tptr) = new TypePtr((yyvsp[(1) - (1)].tval));
            ;
        }
        break;

    case 159:

        /* Line 1455 of yacc.c  */
#line 1200 "asn_grammar.y"
        {
            (yyvsp[(1) - (2)].tval)->SetOptional();
            (yyval.tptr) = new TypePtr((yyvsp[(1) - (2)].tval));
            ;
        }
        break;

    case 160:

        /* Line 1455 of yacc.c  */
#line 1205 "asn_grammar.y"
        {
            ValueTypeContext.reset((yyvsp[(1) - (1)].tval));
            (yyvsp[(1) - (1)].tval)->BeginParseValue();
            ;
        }
        break;

    case 161:

        /* Line 1455 of yacc.c  */
#line 1210 "asn_grammar.y"
        {
            (yyvsp[(1) - (4)].tval)->SetDefaultValue(ValuePtr((yyvsp[(4) - (4)].vval)));
            (yyval.tptr) = new TypePtr(ValueTypeContext);
            (yyvsp[(1) - (4)].tval)->EndParseValue();
            ;
        }
        break;

    case 162:

        /* Line 1455 of yacc.c  */
#line 1216 "asn_grammar.y"
        {
            (yyval.tptr) = new TypePtr((yyvsp[(3) - (3)].tval));
            ;
        }
        break;

    case 163:

        /* Line 1455 of yacc.c  */
#line 1224 "asn_grammar.y"
        {
            (yyval.tval) = new SequenceOfType(TypePtr((yyvsp[(3) - (3)].tval)));
            ;
        }
        break;

    case 164:

        /* Line 1455 of yacc.c  */
#line 1232 "asn_grammar.y"
        {
            (yyval.tval) = new SetType(*(SequenceType*)(yyvsp[(2) - (3)].tval)); delete(yyvsp[(2) - (3)].tval);
            ParsingConstructedType--;
            ;
        }
        break;

    case 165:

        /* Line 1455 of yacc.c  */
#line 1237 "asn_grammar.y"
        {
            (yyval.tval) = new SetType;
            ParsingConstructedType--;
            ;
        }
        break;

    case 166:

        /* Line 1455 of yacc.c  */
#line 1245 "asn_grammar.y"
        {
            ParsingConstructedType++;
            ;
        }
        break;

    case 168:

        /* Line 1455 of yacc.c  */
#line 1254 "asn_grammar.y"
        {
            (yyval.tval) = new SetOfType(TypePtr((yyvsp[(3) - (3)].tval)));
            ;
        }
        break;

    case 169:

        /* Line 1455 of yacc.c  */
#line 1262 "asn_grammar.y"
        {
            (yyvsp[(2) - (2)].tval)->SetTag((yyvsp[(1) - (2)].tagv).tagClass, (yyvsp[(1) - (2)].tagv).tagNumber, Module->GetDefaultTagMode());
            (yyval.tval) = (yyvsp[(2) - (2)].tval);
            ;
        }
        break;

    case 170:

        /* Line 1455 of yacc.c  */
#line 1267 "asn_grammar.y"
        {
            (yyvsp[(3) - (3)].tval)->SetTag((yyvsp[(1) - (3)].tagv).tagClass, (yyvsp[(1) - (3)].tagv).tagNumber, Tag::Implicit);
            (yyval.tval) = (yyvsp[(3) - (3)].tval);
            ;
        }
        break;

    case 171:

        /* Line 1455 of yacc.c  */
#line 1272 "asn_grammar.y"
        {
            (yyvsp[(3) - (3)].tval)->SetTag((yyvsp[(1) - (3)].tagv).tagClass, (yyvsp[(1) - (3)].tagv).tagNumber, Tag::Explicit);
            (yyval.tval) = (yyvsp[(3) - (3)].tval);
            ;
        }
        break;

    case 172:

        /* Line 1455 of yacc.c  */
#line 1280 "asn_grammar.y"
        {
            (yyval.tagv).tagClass = (Tag::Type)(yyvsp[(2) - (4)].ival);
            (yyval.tagv).tagNumber = (int)(yyvsp[(3) - (4)].ival);
            ;
        }
        break;

    case 174:

        /* Line 1455 of yacc.c  */
#line 1289 "asn_grammar.y"
        {
            IntegerValue* val = dynamic_cast<IntegerValue*>((yyvsp[(1) - (1)].vval));
            if (val) {
                (yyval.ival) = *val;
                delete(yyvsp[(1) - (1)].vval);
            } else
                std::cerr << StdError(Fatal) << "incorrect value type." << std::endl;
            ;
        }
        break;

    case 175:

        /* Line 1455 of yacc.c  */
#line 1302 "asn_grammar.y"
        {
            (yyval.ival) = Tag::Universal;
            ;
        }
        break;

    case 176:

        /* Line 1455 of yacc.c  */
#line 1306 "asn_grammar.y"
        {
            (yyval.ival) = Tag::Application;
            ;
        }
        break;

    case 177:

        /* Line 1455 of yacc.c  */
#line 1310 "asn_grammar.y"
        {
            (yyval.ival) = Tag::Private;
            ;
        }
        break;

    case 178:

        /* Line 1455 of yacc.c  */
#line 1314 "asn_grammar.y"
        {
            (yyval.ival) = Tag::ContextSpecific;
            ;
        }
        break;

    case 179:

        /* Line 1455 of yacc.c  */
#line 1322 "asn_grammar.y"
        {
            (yyval.tval) = new SelectionType(*(yyvsp[(1) - (3)].sval), TypePtr((yyvsp[(3) - (3)].tval))); delete(yyvsp[(1) - (3)].sval);
            ;
        }
        break;

    case 180:

        /* Line 1455 of yacc.c  */
#line 1330 "asn_grammar.y"
        {
            (yyval.tval) = new GeneralizedTimeType;
            ;
        }
        break;

    case 181:

        /* Line 1455 of yacc.c  */
#line 1334 "asn_grammar.y"
        {
            (yyval.tval) = new UTCTimeType;
            ;
        }
        break;

    case 182:

        /* Line 1455 of yacc.c  */
#line 1338 "asn_grammar.y"
        {
            (yyval.tval) = new ObjectDescriptorType;
            ;
        }
        break;

    case 183:

        /* Line 1455 of yacc.c  */
#line 1346 "asn_grammar.y"
        {
            (yyval.tval) = new TypeFromObject(InformationObjectPtr((yyvsp[(1) - (2)].objt)), *(yyvsp[(2) - (2)].sval)); delete(yyvsp[(2) - (2)].sval);
            InformationFromObjectContext = NULL;
            ;
        }
        break;

    case 184:

        /* Line 1455 of yacc.c  */
#line 1355 "asn_grammar.y"
        {
            (yyval.vset) = new ValueSetFromObjects(ObjectSetConstraintElementPtr((yyvsp[(1) - (2)].osce)), *(yyvsp[(2) - (2)].sval)); delete(yyvsp[(2) - (2)].sval);
            InformationFromObjectContext = NULL;
            ;
        }
        break;

    case 185:

        /* Line 1455 of yacc.c  */
#line 1360 "asn_grammar.y"
        {
            (yyval.vset) = new ValueSetFromObjects(ObjectSetConstraintElementPtr((yyvsp[(1) - (2)].osce)), *(yyvsp[(2) - (2)].sval)); delete(yyvsp[(2) - (2)].sval);
            InformationFromObjectContext = NULL;
            ;
        }
        break;

    case 187:

        /* Line 1455 of yacc.c  */
#line 1369 "asn_grammar.y"
        {
            (yyval.osce) = new ObjectSetFromObjects(ObjectSetConstraintElementPtr((yyvsp[(1) - (2)].osce)), *(yyvsp[(2) - (2)].sval)); delete(yyvsp[(2) - (2)].sval);
            InformationFromObjectContext = NULL;
            ;
        }
        break;

    case 188:

        /* Line 1455 of yacc.c  */
#line 1374 "asn_grammar.y"
        {
            (yyval.osce) = new ObjectSetFromObjects(ObjectSetConstraintElementPtr((yyvsp[(1) - (2)].osce)), *(yyvsp[(2) - (2)].sval)); delete(yyvsp[(2) - (2)].sval);
            InformationFromObjectContext = NULL;
            ;
        }
        break;

    case 190:

        /* Line 1455 of yacc.c  */
#line 1384 "asn_grammar.y"
        {
            (yyval.osce) = (yyvsp[(1) - (1)].dos);
            ;
        }
        break;

    case 191:

        /* Line 1455 of yacc.c  */
#line 1388 "asn_grammar.y"
        {
            (yyval.osce) = (yyvsp[(1) - (1)].osce);
            ;
        }
        break;

    case 192:

        /* Line 1455 of yacc.c  */
#line 1392 "asn_grammar.y"
        {
            (yyval.osce) = (yyvsp[(1) - (1)].dos);
            ;
        }
        break;

    case 196:

        /* Line 1455 of yacc.c  */
#line 1405 "asn_grammar.y"
        {
            ;
        }
        break;

    case 197:

        /* Line 1455 of yacc.c  */
#line 1408 "asn_grammar.y"
        {
            ;
        }
        break;

    case 198:

        /* Line 1455 of yacc.c  */
#line 1417 "asn_grammar.y"
        {
            (yyvsp[(1) - (2)].tval)->AddConstraint(ConstraintPtr((yyvsp[(2) - (2)].cons)));
            ;
        }
        break;

    case 200:

        /* Line 1455 of yacc.c  */
#line 1426 "asn_grammar.y"
        {
            (yyval.tval) = new SetOfType(TypePtr((yyvsp[(4) - (4)].tval)), ConstraintPtr((yyvsp[(2) - (4)].cons)));
            ;
        }
        break;

    case 201:

        /* Line 1455 of yacc.c  */
#line 1430 "asn_grammar.y"
        {
            ConstraintElementPtr cons((yyvsp[(2) - (4)].elmt));
            (yyval.tval) = new SetOfType(TypePtr((yyvsp[(4) - (4)].tval)), ConstraintPtr(new Constraint(cons)));
            ;
        }
        break;

    case 202:

        /* Line 1455 of yacc.c  */
#line 1435 "asn_grammar.y"
        {
            (yyval.tval) = new SequenceOfType(TypePtr((yyvsp[(4) - (4)].tval)), ConstraintPtr((yyvsp[(2) - (4)].cons)));
            ;
        }
        break;

    case 203:

        /* Line 1455 of yacc.c  */
#line 1439 "asn_grammar.y"
        {
            ConstraintElementPtr cons((yyvsp[(2) - (4)].elmt));
            (yyval.tval) = new SequenceOfType(TypePtr((yyvsp[(4) - (4)].tval)), ConstraintPtr(new Constraint(cons)));
            ;
        }
        break;

    case 204:

        /* Line 1455 of yacc.c  */
#line 1447 "asn_grammar.y"
        {
            (yyval.cons) = (yyvsp[(2) - (4)].cons);
            ;
        }
        break;

    case 207:

        /* Line 1455 of yacc.c  */
#line 1460 "asn_grammar.y"
        {
            (yyval.vval) = (yyvsp[(2) - (2)].vval);
            ;
        }
        break;

    case 208:

        /* Line 1455 of yacc.c  */
#line 1464 "asn_grammar.y"
        {
            (yyval.vval) = NULL;
            ;
        }
        break;

    case 209:

        /* Line 1455 of yacc.c  */
#line 1472 "asn_grammar.y"
        {
            (yyval.vval) = new IntegerValue((yyvsp[(1) - (1)].ival));
            ;
        }
        break;

    case 211:

        /* Line 1455 of yacc.c  */
#line 1477 "asn_grammar.y"
        {
            ValueTypeContext.reset((yyvsp[(1) - (1)].tval));
            (yyvsp[(1) - (1)].tval)->BeginParseValue();
            ;
        }
        break;

    case 212:

        /* Line 1455 of yacc.c  */
#line 1482 "asn_grammar.y"
        {
            (yyvsp[(1) - (4)].tval)->EndParseValue();
            (yyval.vval) = (yyvsp[(4) - (4)].vval);
            ;
        }
        break;

    case 213:

        /* Line 1455 of yacc.c  */
#line 1491 "asn_grammar.y"
        {
            (yyval.cons) = new Constraint(std::auto_ptr<ConstraintElementVector>((yyvsp[(1) - (1)].elst)), false);
            ;
        }
        break;

    case 214:

        /* Line 1455 of yacc.c  */
#line 1495 "asn_grammar.y"
        {
            (yyval.cons) = new Constraint(std::auto_ptr<ConstraintElementVector>((yyvsp[(1) - (5)].elst)), true);
            ;
        }
        break;

    case 215:

        /* Line 1455 of yacc.c  */
#line 1499 "asn_grammar.y"
        {
            (yyval.cons) = new Constraint(std::auto_ptr<ConstraintElementVector>(),
                                          true,
                                          std::auto_ptr<ConstraintElementVector>((yyvsp[(5) - (5)].elst)));
            ;
        }
        break;

    case 216:

        /* Line 1455 of yacc.c  */
#line 1505 "asn_grammar.y"
        {
            (yyval.cons) = new Constraint(std::auto_ptr<ConstraintElementVector>((yyvsp[(1) - (7)].elst)),
                                          true,
                                          std::auto_ptr<ConstraintElementVector>((yyvsp[(7) - (7)].elst)));
            ;
        }
        break;

    case 218:

        /* Line 1455 of yacc.c  */
#line 1516 "asn_grammar.y"
        {
            (yyval.elst) = new ConstraintElementVector;
            (yyval.elst)->push_back(ConstraintElementPtr(
                                        new ConstrainAllConstraintElement(
                                            ConstraintElementPtr((yyvsp[(2) - (2)].elmt)))));
            ;
        }
        break;

    case 219:

        /* Line 1455 of yacc.c  */
#line 1527 "asn_grammar.y"
        {
            if ((yyvsp[(1) - (1)].elst)->size() == 1)
                (yyval.elst) = (yyvsp[(1) - (1)].elst);
            else {
                (yyval.elst) = new ConstraintElementVector;
                (yyval.elst)->push_back(ConstraintElementPtr(new ElementListConstraintElement(
                                                                 std::auto_ptr<ConstraintElementVector>((yyvsp[(1) - (1)].elst)))));
            }
            ;
        }
        break;

    case 220:

        /* Line 1455 of yacc.c  */
#line 1538 "asn_grammar.y"
        {
            if ((yyvsp[(3) - (3)].elst)->size() == 1) {
                (yyval.elst)->push_back((*(yyvsp[(3) - (3)].elst))[0]);
                delete(yyvsp[(3) - (3)].elst);
            } else
                (yyval.elst)->push_back(ConstraintElementPtr(new ElementListConstraintElement(
                                                                 std::auto_ptr<ConstraintElementVector>((yyvsp[(3) - (3)].elst)))));
            ;
        }
        break;

    case 221:

        /* Line 1455 of yacc.c  */
#line 1552 "asn_grammar.y"
        {
            (yyval.elst) = new ConstraintElementVector;
            (yyval.elst)->push_back(ConstraintElementPtr((yyvsp[(1) - (1)].elmt)));

            ;
        }
        break;

    case 222:

        /* Line 1455 of yacc.c  */
#line 1558 "asn_grammar.y"
        {
            (yyval.elst)->push_back(ConstraintElementPtr((yyvsp[(3) - (3)].elmt)));
            ;
        }
        break;

    case 224:

        /* Line 1455 of yacc.c  */
#line 1566 "asn_grammar.y"
        {
            (yyvsp[(1) - (2)].elmt)->SetExclusions(ConstraintElementPtr((yyvsp[(2) - (2)].elmt)));
            ;
        }
        break;

    case 225:

        /* Line 1455 of yacc.c  */
#line 1573 "asn_grammar.y"
        {
            (yyval.elmt) = (yyvsp[(2) - (2)].elmt);
            ;
        }
        break;

    case 232:

        /* Line 1455 of yacc.c  */
#line 1592 "asn_grammar.y"
        {
            (yyval.elmt) = new ElementListConstraintElement(std::auto_ptr<ConstraintElementVector>((yyvsp[(2) - (3)].elst)));
            ;
        }
        break;

    case 233:

        /* Line 1455 of yacc.c  */
#line 1600 "asn_grammar.y"
        {
            (yyval.elmt) = new SingleValueConstraintElement(ValuePtr((yyvsp[(1) - (1)].vval)));
            ;
        }
        break;

    case 234:

        /* Line 1455 of yacc.c  */
#line 1604 "asn_grammar.y"
        {
            (yyval.elmt) = new SubTypeConstraintElement(TypePtr((yyvsp[(1) - (1)].tval)));
            ;
        }
        break;

    case 239:

        /* Line 1455 of yacc.c  */
#line 1616 "asn_grammar.y"
        {
            (yyval.elmt) = new ValueRangeConstraintElement(ValuePtr((yyvsp[(1) - (4)].vval)), ValuePtr((yyvsp[(4) - (4)].vval)));
            ;
        }
        break;

    case 243:

        /* Line 1455 of yacc.c  */
#line 1629 "asn_grammar.y"
        {
            (yyval.vval) = (yyvsp[(2) - (2)].vval);
            ;
        }
        break;

    case 245:

        /* Line 1455 of yacc.c  */
#line 1637 "asn_grammar.y"
        {
            (yyval.vval) = new MinValue;
            ;
        }
        break;

    case 247:

        /* Line 1455 of yacc.c  */
#line 1645 "asn_grammar.y"
        {
            (yyval.vval) = new MaxValue;
            ;
        }
        break;

    case 248:

        /* Line 1455 of yacc.c  */
#line 1652 "asn_grammar.y"
        {
            (yyval.elmt) = new FromConstraintElement(ConstraintPtr((yyvsp[(2) - (2)].cons)));
            ;
        }
        break;

    case 249:

        /* Line 1455 of yacc.c  */
#line 1659 "asn_grammar.y"
        {
            (yyval.tval) = (yyvsp[(2) - (2)].tval);
            ;
        }
        break;

    case 253:

        /* Line 1455 of yacc.c  */
#line 1677 "asn_grammar.y"
        {
            (yyval.elmt) = new SizeConstraintElement(ConstraintPtr((yyvsp[(2) - (2)].cons)));
            ;
        }
        break;

    case 254:

        /* Line 1455 of yacc.c  */
#line 1685 "asn_grammar.y"
        {
            (yyval.elmt) = new WithComponentConstraintElement(NULL, ConstraintPtr((yyvsp[(3) - (3)].cons)), WithComponentConstraintElement::Default);
            ;
        }
        break;

    case 255:

        /* Line 1455 of yacc.c  */
#line 1689 "asn_grammar.y"
        {
            (yyval.elmt) = (yyvsp[(3) - (3)].elmt);
            ;
        }
        break;

    case 256:

        /* Line 1455 of yacc.c  */
#line 1696 "asn_grammar.y"
        {
            (yyval.elmt) = new InnerTypeConstraintElement(std::auto_ptr<ConstraintElementVector>((yyvsp[(2) - (3)].elst)), false);
            ;
        }
        break;

    case 257:

        /* Line 1455 of yacc.c  */
#line 1700 "asn_grammar.y"
        {
            (yyval.elmt) = new InnerTypeConstraintElement(std::auto_ptr<ConstraintElementVector>((yyvsp[(6) - (7)].elst)), true);
            ;
        }
        break;

    case 258:

        /* Line 1455 of yacc.c  */
#line 1707 "asn_grammar.y"
        {
            (yyval.elst) = new ConstraintElementVector;
            (yyval.elst)->push_back(ConstraintElementPtr((yyvsp[(1) - (1)].elmt)));
            ;
        }
        break;

    case 259:

        /* Line 1455 of yacc.c  */
#line 1712 "asn_grammar.y"
        {
            (yyvsp[(3) - (3)].elst)->push_back(ConstraintElementPtr((yyvsp[(1) - (3)].elmt)));
            (yyval.elst) = (yyvsp[(3) - (3)].elst);
            ;
        }
        break;

    case 260:

        /* Line 1455 of yacc.c  */
#line 1720 "asn_grammar.y"
        {
            (yyval.elmt) = new WithComponentConstraintElement(*(yyvsp[(1) - (2)].sval), ConstraintPtr(), (int)(yyvsp[(2) - (2)].ival)); delete(yyvsp[(1) - (2)].sval);
            ;
        }
        break;

    case 261:

        /* Line 1455 of yacc.c  */
#line 1724 "asn_grammar.y"
        {
            (yyval.elmt) = new WithComponentConstraintElement(*(yyvsp[(1) - (3)].sval), ConstraintPtr((yyvsp[(2) - (3)].cons)), (int)(yyvsp[(3) - (3)].ival)); delete(yyvsp[(1) - (3)].sval);
            ;
        }
        break;

    case 262:

        /* Line 1455 of yacc.c  */
#line 1731 "asn_grammar.y"
        {
            (yyval.ival) = WithComponentConstraintElement::Present;
            ;
        }
        break;

    case 263:

        /* Line 1455 of yacc.c  */
#line 1735 "asn_grammar.y"
        {
            (yyval.ival) = WithComponentConstraintElement::Absent;
            ;
        }
        break;

    case 264:

        /* Line 1455 of yacc.c  */
#line 1739 "asn_grammar.y"
        {
            (yyval.ival) = WithComponentConstraintElement::Optional;
            ;
        }
        break;

    case 265:

        /* Line 1455 of yacc.c  */
#line 1743 "asn_grammar.y"
        {
            (yyval.ival) = WithComponentConstraintElement::Default;
            ;
        }
        break;

    case 266:

        /* Line 1455 of yacc.c  */
#line 1758 "asn_grammar.y"
        {
            ConstraintElementPtr cons((yyvsp[(4) - (5)].elmt));
            (yyval.cons) = new Constraint(cons);
            ;
        }
        break;

    case 267:

        /* Line 1455 of yacc.c  */
#line 1766 "asn_grammar.y"
        {
            (yyval.elmt) = new UserDefinedConstraintElement(ActualParameterListPtr());
            ;
        }
        break;

    case 268:

        /* Line 1455 of yacc.c  */
#line 1770 "asn_grammar.y"
        {
            (yyval.elmt) = new UserDefinedConstraintElement(ActualParameterListPtr((yyvsp[(1) - (1)].aplt)));
            ;
        }
        break;

    case 269:

        /* Line 1455 of yacc.c  */
#line 1777 "asn_grammar.y"
        {
            (yyvsp[(3) - (3)].aplt)->push_back(ActualParameterPtr((yyvsp[(1) - (3)].apar)));
            (yyval.aplt) = (yyvsp[(3) - (3)].aplt);
            ;
        }
        break;

    case 270:

        /* Line 1455 of yacc.c  */
#line 1782 "asn_grammar.y"
        {
            (yyval.aplt) = new ActualParameterList;
            (yyval.aplt)->push_back(ActualParameterPtr((yyvsp[(1) - (1)].apar)));
            ;
        }
        break;

    case 271:

        /* Line 1455 of yacc.c  */
#line 1790 "asn_grammar.y"
        {
            (yyval.apar) = (yyvsp[(3) - (3)].apar);
            ;
        }
        break;

    case 273:

        /* Line 1455 of yacc.c  */
#line 1798 "asn_grammar.y"
        {
            delete(yyvsp[(1) - (1)].tval);
            ;
        }
        break;

    case 274:

        /* Line 1455 of yacc.c  */
#line 1802 "asn_grammar.y"
        {
            delete(yyvsp[(1) - (1)].dobj);
            ;
        }
        break;

    case 277:

        /* Line 1455 of yacc.c  */
#line 1813 "asn_grammar.y"
        {
            (yyval.tcons) = new TableConstraint(DefinedObjectSetPtr((yyvsp[(2) - (3)].dos)));
            ;
        }
        break;

    case 278:

        /* Line 1455 of yacc.c  */
#line 1820 "asn_grammar.y"
        {
            (yyval.tcons) = new TableConstraint(DefinedObjectSetPtr((yyvsp[(2) - (6)].dos)), std::auto_ptr<StringList>((yyvsp[(5) - (6)].slst)));
            ;
        }
        break;

    case 279:

        /* Line 1455 of yacc.c  */
#line 1827 "asn_grammar.y"
        {
            (yyvsp[(1) - (3)].slst)->push_back(*(yyvsp[(3) - (3)].sval));
            delete(yyvsp[(3) - (3)].sval);
            ;
        }
        break;

    case 280:

        /* Line 1455 of yacc.c  */
#line 1832 "asn_grammar.y"
        {
            (yyval.slst) = new StringList;
            (yyval.slst)->push_back(*(yyvsp[(1) - (1)].sval));
            delete(yyvsp[(1) - (1)].sval);
            ;
        }
        break;

    case 281:

        /* Line 1455 of yacc.c  */
#line 1841 "asn_grammar.y"
        {
            (yyval.sval) = (yyvsp[(2) - (2)].sval);
            ;
        }
        break;

    case 282:

        /* Line 1455 of yacc.c  */
#line 1845 "asn_grammar.y"
        {
            (yyvsp[(3) - (3)].sval)->insert(0, ".");
            (yyval.sval) = (yyvsp[(3) - (3)].sval);
            ;
        }
        break;

    case 283:

        /* Line 1455 of yacc.c  */
#line 1853 "asn_grammar.y"
        {
            *((yyvsp[(1) - (3)].sval)) += '.' ;
            *((yyvsp[(1) - (3)].sval)) += *((yyvsp[(3) - (3)].sval));
            (yyval.sval) = (yyvsp[(1) - (3)].sval);
            delete(yyvsp[(3) - (3)].sval);
            ;
        }
        break;

    case 285:

        /* Line 1455 of yacc.c  */
#line 1866 "asn_grammar.y"
        {
            (yyvsp[(3) - (3)].objc)->SetName(*(yyvsp[(1) - (3)].sval)); delete(yyvsp[(1) - (3)].sval);
            Module->AddObjectClass(ObjectClassBasePtr((yyvsp[(3) - (3)].objc)));
            ;
        }
        break;

    case 286:

        /* Line 1455 of yacc.c  */
#line 1875 "asn_grammar.y"
        {
            (yyvsp[(2) - (2)].dobj)->BeginParseObject();
            ;
        }
        break;

    case 287:

        /* Line 1455 of yacc.c  */
#line 1879 "asn_grammar.y"
        {
            (yyvsp[(2) - (5)].dobj)->EndParseObject();
            classStack->pop();
            (yyvsp[(5) - (5)].objt)->SetName(*(yyvsp[(1) - (5)].sval)); delete(yyvsp[(1) - (5)].sval);
            (yyvsp[(5) - (5)].objt)->SetObjectClass((yyvsp[(2) - (5)].dobj));
            Module->AddInformationObject(InformationObjectPtr((yyvsp[(5) - (5)].objt)));
            ;
        }
        break;

    case 288:

        /* Line 1455 of yacc.c  */
#line 1890 "asn_grammar.y"
        {
            (yyvsp[(2) - (2)].dobj)->BeginParseObjectSet();
            ;
        }
        break;

    case 289:

        /* Line 1455 of yacc.c  */
#line 1894 "asn_grammar.y"
        {
            Module->AddInformationObjectSet(InformationObjectSetPtr(
                                                new InformationObjectSetDefn(*(yyvsp[(1) - (5)].sval),
                                                                             ObjectClassBasePtr((yyvsp[(2) - (5)].dobj)),
                                                                             ConstraintPtr((yyvsp[(5) - (5)].cons)))));
            delete(yyvsp[(1) - (5)].sval);
            classStack->pop();
            ;
        }
        break;

    case 290:

        /* Line 1455 of yacc.c  */
#line 1907 "asn_grammar.y"
        {
            (yyval.objc) = (yyvsp[(1) - (1)].dobj);
            ;
        }
        break;

    case 291:

        /* Line 1455 of yacc.c  */
#line 1911 "asn_grammar.y"
        {
            (yyval.objc) = (yyvsp[(1) - (1)].objc);
            ;
        }
        break;

    case 292:

        /* Line 1455 of yacc.c  */
#line 1919 "asn_grammar.y"
        {
            (yyval.dobj) = new DefinedObjectClass(*(yyvsp[(1) - (1)].sval));
            delete(yyvsp[(1) - (1)].sval);
            ;
        }
        break;

    case 293:

        /* Line 1455 of yacc.c  */
#line 1924 "asn_grammar.y"
        {
            (yyval.dobj) = new DefinedObjectClass(*(yyvsp[(1) - (1)].sval));
            delete(yyvsp[(1) - (1)].sval);
            ;
        }
        break;

    case 294:

        /* Line 1455 of yacc.c  */
#line 1929 "asn_grammar.y"
        {
            (yyval.dobj) = new DefinedObjectClass(*(yyvsp[(1) - (1)].sval));
            delete(yyvsp[(1) - (1)].sval);
            ;
        }
        break;

    case 295:

        /* Line 1455 of yacc.c  */
#line 1937 "asn_grammar.y"
        {
            (yyval.sval) = ConcatNames((yyvsp[(1) - (3)].sval), '.', (yyvsp[(3) - (3)].sval));
            ;
        }
        break;

    case 296:

        /* Line 1455 of yacc.c  */
#line 1944 "asn_grammar.y"
        {
            (yyval.sval) = new std::string("TYPE-IDENTIFIER");
            ;
        }
        break;

    case 297:

        /* Line 1455 of yacc.c  */
#line 1948 "asn_grammar.y"
        {
            (yyval.sval) = new std::string("ABSTRACT-SYNTAX");
            ;
        }
        break;

    case 298:

        /* Line 1455 of yacc.c  */
#line 1955 "asn_grammar.y"
        {
            ObjectClassDefn* ocd = new ObjectClassDefn;
            (yyval.objc) = ocd;
            ocd->SetFieldSpecs(std::auto_ptr<FieldSpecsList>((yyvsp[(3) - (5)].flst)));
            ocd->SetWithSyntaxSpec(TokenGroupPtr((yyvsp[(5) - (5)].tgrp)));
            InWithSyntaxContext = false;
            ;
        }
        break;

    case 299:

        /* Line 1455 of yacc.c  */
#line 1966 "asn_grammar.y"
        {
            (yyvsp[(1) - (3)].flst)->push_back(FieldSpecPtr((yyvsp[(3) - (3)].fspc)));
            (yyval.flst) = (yyvsp[(1) - (3)].flst);
            ;
        }
        break;

    case 300:

        /* Line 1455 of yacc.c  */
#line 1971 "asn_grammar.y"
        {
            (yyval.flst) = new FieldSpecsList;
            (yyval.flst)->push_back(FieldSpecPtr((yyvsp[(1) - (1)].fspc)));
            ;
        }
        break;

    case 308:

        /* Line 1455 of yacc.c  */
#line 1989 "asn_grammar.y"
        {
            (yyval.fspc) = new TypeFieldSpec(*(yyvsp[(1) - (1)].sval)); delete(yyvsp[(1) - (1)].sval);
            ;
        }
        break;

    case 309:

        /* Line 1455 of yacc.c  */
#line 1993 "asn_grammar.y"
        {
            (yyval.fspc) = new TypeFieldSpec(*(yyvsp[(1) - (2)].sval), true); delete(yyvsp[(1) - (2)].sval);

            ;
        }
        break;

    case 310:

        /* Line 1455 of yacc.c  */
#line 1998 "asn_grammar.y"
        {
            (yyval.fspc) = new TypeFieldSpec(*(yyvsp[(1) - (3)].sval), false, TypePtr((yyvsp[(3) - (3)].tval))); delete(yyvsp[(1) - (3)].sval);
            ;
        }
        break;

    case 311:

        /* Line 1455 of yacc.c  */
#line 2006 "asn_grammar.y"
        {
            (yyval.fspc) = new FixedTypeValueFieldSpec(*(yyvsp[(1) - (2)].sval), TypePtr((yyvsp[(2) - (2)].tval)));
            delete(yyvsp[(1) - (2)].sval);
            ;
        }
        break;

    case 312:

        /* Line 1455 of yacc.c  */
#line 2011 "asn_grammar.y"
        {
            (yyval.fspc) = new FixedTypeValueFieldSpec(*(yyvsp[(1) - (3)].sval), TypePtr((yyvsp[(2) - (3)].tval)), false, true);
            delete(yyvsp[(1) - (3)].sval);
            ;
        }
        break;

    case 313:

        /* Line 1455 of yacc.c  */
#line 2016 "asn_grammar.y"
        {
            (yyval.fspc) = new FixedTypeValueFieldSpec(*(yyvsp[(1) - (3)].sval), TypePtr((yyvsp[(2) - (3)].tval)), true);
            delete(yyvsp[(1) - (3)].sval);
            ;
        }
        break;

    case 314:

        /* Line 1455 of yacc.c  */
#line 2021 "asn_grammar.y"
        {
            ValueTypeContext.reset((yyvsp[(2) - (2)].tval));
            (yyvsp[(2) - (2)].tval)->BeginParseValue();
            ;
        }
        break;

    case 315:

        /* Line 1455 of yacc.c  */
#line 2026 "asn_grammar.y"
        {
            FixedTypeValueFieldSpec* spec = new FixedTypeValueFieldSpec(*(yyvsp[(1) - (5)].sval), ValueTypeContext);
            (yyvsp[(2) - (5)].tval)->EndParseValue();
            spec->SetDefault(ValuePtr((yyvsp[(5) - (5)].vval)));
            (yyval.fspc) = spec;
            delete(yyvsp[(1) - (5)].sval);
            ;
        }
        break;

    case 316:

        /* Line 1455 of yacc.c  */
#line 2034 "asn_grammar.y"
        {
            (yyval.fspc) = new FixedTypeValueFieldSpec(*(yyvsp[(1) - (4)].sval), TypePtr((yyvsp[(2) - (4)].tval)), true, true);
            delete(yyvsp[(1) - (4)].sval);
            ;
        }
        break;

    case 317:

        /* Line 1455 of yacc.c  */
#line 2039 "asn_grammar.y"
        {
            ValueTypeContext.reset((yyvsp[(2) - (3)].tval));
            (yyvsp[(2) - (3)].tval)->BeginParseValue();
            ;
        }
        break;

    case 318:

        /* Line 1455 of yacc.c  */
#line 2044 "asn_grammar.y"
        {
            TypePtr t = ValueTypeContext;
            (yyvsp[(2) - (6)].tval)->EndParseValue();
            FixedTypeValueFieldSpec* spec = new FixedTypeValueFieldSpec(*(yyvsp[(1) - (6)].sval), t, false, true);
            spec->SetDefault(ValuePtr((yyvsp[(6) - (6)].vval)));
            (yyval.fspc) = spec;
            delete(yyvsp[(1) - (6)].sval);
            ;
        }
        break;

    case 319:

        /* Line 1455 of yacc.c  */
#line 2058 "asn_grammar.y"
        {
            (yyval.fspc) = new VariableTypeValueFieldSpec(*(yyvsp[(1) - (2)].sval), *(yyvsp[(2) - (2)].sval)); delete(yyvsp[(1) - (2)].sval); delete(yyvsp[(2) - (2)].sval);
            ;
        }
        break;

    case 320:

        /* Line 1455 of yacc.c  */
#line 2062 "asn_grammar.y"
        {
            (yyval.fspc) = new VariableTypeValueFieldSpec(*(yyvsp[(1) - (3)].sval), *(yyvsp[(2) - (3)].sval), true); delete(yyvsp[(1) - (3)].sval); delete(yyvsp[(2) - (3)].sval);
            ;
        }
        break;

    case 321:

        /* Line 1455 of yacc.c  */
#line 2070 "asn_grammar.y"
        {
            (yyval.fspc) = new FixedTypeValueSetFieldSpec(*(yyvsp[(1) - (2)].sval), TypePtr((yyvsp[(2) - (2)].tval)));
            delete(yyvsp[(1) - (2)].sval);
            ;
        }
        break;

    case 322:

        /* Line 1455 of yacc.c  */
#line 2075 "asn_grammar.y"
        {
            (yyval.fspc) = new FixedTypeValueSetFieldSpec(*(yyvsp[(1) - (3)].sval), TypePtr((yyvsp[(2) - (3)].tval)), true);
            delete(yyvsp[(1) - (3)].sval);
            ;
        }
        break;

    case 323:

        /* Line 1455 of yacc.c  */
#line 2080 "asn_grammar.y"
        {
            ValueTypeContext.reset((yyvsp[(2) - (2)].tval));
            (yyvsp[(2) - (2)].tval)->BeginParseValueSet();
            ;
        }
        break;

    case 324:

        /* Line 1455 of yacc.c  */
#line 2085 "asn_grammar.y"
        {
            FixedTypeValueSetFieldSpec* spec = new FixedTypeValueSetFieldSpec(*(yyvsp[(1) - (5)].sval), ValueTypeContext);
            (yyvsp[(2) - (5)].tval)->EndParseValueSet();
            spec->SetDefault(ValueSetPtr((yyvsp[(5) - (5)].vset)));
            (yyval.fspc) = spec;
            ;
        }
        break;

    case 325:

        /* Line 1455 of yacc.c  */
#line 2096 "asn_grammar.y"
        {
            (yyval.fspc) = new VariableTypeValueSetFieldSpec(*(yyvsp[(1) - (2)].sval), *(yyvsp[(2) - (2)].sval)); delete(yyvsp[(1) - (2)].sval); delete(yyvsp[(2) - (2)].sval);
            ;
        }
        break;

    case 326:

        /* Line 1455 of yacc.c  */
#line 2100 "asn_grammar.y"
        {
            (yyval.fspc) = new VariableTypeValueSetFieldSpec(*(yyvsp[(1) - (3)].sval), *(yyvsp[(2) - (3)].sval), true); delete(yyvsp[(1) - (3)].sval); delete(yyvsp[(2) - (3)].sval);
            ;
        }
        break;

    case 327:

        /* Line 1455 of yacc.c  */
#line 2108 "asn_grammar.y"
        {
            (yyval.fspc) = new  ObjectFieldSpec(*(yyvsp[(1) - (2)].sval), (yyvsp[(2) - (2)].dobj)); delete(yyvsp[(1) - (2)].sval);
            ;
        }
        break;

    case 328:

        /* Line 1455 of yacc.c  */
#line 2112 "asn_grammar.y"
        {
            (yyval.fspc) = new  ObjectFieldSpec(*(yyvsp[(1) - (3)].sval), (yyvsp[(2) - (3)].dobj), true); delete(yyvsp[(1) - (3)].sval);
            ;
        }
        break;

    case 329:

        /* Line 1455 of yacc.c  */
#line 2116 "asn_grammar.y"
        {
            (yyvsp[(2) - (2)].dobj)->BeginParseObject();
            ;
        }
        break;

    case 330:

        /* Line 1455 of yacc.c  */
#line 2120 "asn_grammar.y"
        {
            (yyvsp[(2) - (5)].dobj)->EndParseObject();
            classStack->pop();
            ObjectFieldSpec* spec = new  ObjectFieldSpec(*(yyvsp[(1) - (5)].sval), (yyvsp[(2) - (5)].dobj)); delete(yyvsp[(1) - (5)].sval);
            spec->SetDefault(InformationObjectPtr((yyvsp[(5) - (5)].objt)));
            (yyval.fspc) = spec;
            ;
        }
        break;

    case 331:

        /* Line 1455 of yacc.c  */
#line 2132 "asn_grammar.y"
        {
            (yyval.fspc) = new  ObjectSetFieldSpec(*(yyvsp[(1) - (2)].sval), DefinedObjectClassPtr((yyvsp[(2) - (2)].dobj))); delete(yyvsp[(1) - (2)].sval);
            ;
        }
        break;

    case 332:

        /* Line 1455 of yacc.c  */
#line 2136 "asn_grammar.y"
        {
            (yyval.fspc) = new  ObjectSetFieldSpec(*(yyvsp[(1) - (3)].sval), DefinedObjectClassPtr((yyvsp[(2) - (3)].dobj)), true); delete(yyvsp[(1) - (3)].sval);
            ;
        }
        break;

    case 333:

        /* Line 1455 of yacc.c  */
#line 2140 "asn_grammar.y"
        {
            (yyvsp[(2) - (2)].dobj)->BeginParseObjectSet();
            ;
        }
        break;

    case 334:

        /* Line 1455 of yacc.c  */
#line 2144 "asn_grammar.y"
        {
            ObjectSetFieldSpec* spec = new  ObjectSetFieldSpec(*(yyvsp[(1) - (5)].sval), DefinedObjectClassPtr((yyvsp[(2) - (5)].dobj)));
            delete(yyvsp[(1) - (5)].sval);
            spec->SetDefault(ConstraintPtr((yyvsp[(5) - (5)].cons)));
            (yyval.fspc) = spec;
            classStack->pop();
            ;
        }
        break;

    case 335:

        /* Line 1455 of yacc.c  */
#line 2156 "asn_grammar.y"
        {
            InWithSyntaxContext = true;
            ;
        }
        break;

    case 336:

        /* Line 1455 of yacc.c  */
#line 2160 "asn_grammar.y"
        {
            (yyval.tgrp) = (yyvsp[(4) - (4)].tgrp);
            InWithSyntaxContext = false;
            ;
        }
        break;

    case 337:

        /* Line 1455 of yacc.c  */
#line 2165 "asn_grammar.y"
        {
            (yyval.tgrp) = NULL;
            ;
        }
        break;

    case 338:

        /* Line 1455 of yacc.c  */
#line 2172 "asn_grammar.y"
        {
            (yyval.tgrp) = (yyvsp[(2) - (3)].tgrp);
            ;
        }
        break;

    case 339:

        /* Line 1455 of yacc.c  */
#line 2176 "asn_grammar.y"
        {
            (yyval.tgrp) = NULL;
            ;
        }
        break;

    case 340:

        /* Line 1455 of yacc.c  */
#line 2183 "asn_grammar.y"
        {
            (yyval.tgrp) = (yyvsp[(1) - (2)].tgrp);
            (yyval.tgrp)->AddToken(TokenOrGroupSpecPtr((yyvsp[(2) - (2)].togs)));

            ;
        }
        break;

    case 341:

        /* Line 1455 of yacc.c  */
#line 2189 "asn_grammar.y"
        {
            (yyval.tgrp) = new TokenGroup;
            (yyval.tgrp)->AddToken(TokenOrGroupSpecPtr((yyvsp[(1) - (1)].togs)));
            ;
        }
        break;

    case 344:

        /* Line 1455 of yacc.c  */
#line 2202 "asn_grammar.y"
        {
            (yyvsp[(2) - (3)].tgrp)->SetOptional();
            (yyval.togs) = (yyvsp[(2) - (3)].tgrp);
            ;
        }
        break;

    case 345:

        /* Line 1455 of yacc.c  */
#line 2210 "asn_grammar.y"
        {
            (yyval.togs) = new Literal(*(yyvsp[(1) - (1)].sval)); delete(yyvsp[(1) - (1)].sval);
            ;
        }
        break;

    case 346:

        /* Line 1455 of yacc.c  */
#line 2214 "asn_grammar.y"
        {
            (yyval.togs) = new PrimitiveFieldName(*(yyvsp[(1) - (1)].sval)); delete(yyvsp[(1) - (1)].sval);
            ;
        }
        break;

    case 347:

        /* Line 1455 of yacc.c  */
#line 2218 "asn_grammar.y"
        {
            (yyval.togs) = new PrimitiveFieldName(*(yyvsp[(1) - (1)].sval)); delete(yyvsp[(1) - (1)].sval);
            ;
        }
        break;

    case 348:

        /* Line 1455 of yacc.c  */
#line 2225 "asn_grammar.y"
        {
            (yyval.sval) = (yyvsp[(1) - (1)].sval);
            ;
        }
        break;

    case 349:

        /* Line 1455 of yacc.c  */
#line 2229 "asn_grammar.y"
        {
            (yyval.sval) = new std::string(",");
            ;
        }
        break;

    case 350:

        /* Line 1455 of yacc.c  */
#line 2237 "asn_grammar.y"
        {
            (yyval.objt) = new DefinedObject(*(yyvsp[(1) - (1)].sval));
            delete(yyvsp[(1) - (1)].sval);
            ;
        }
        break;

    case 351:

        /* Line 1455 of yacc.c  */
#line 2242 "asn_grammar.y"
        {
            (yyval.objt) = new DefinedObject(*(yyvsp[(1) - (1)].sval));
            delete(yyvsp[(1) - (1)].sval);
            ;
        }
        break;

    case 352:

        /* Line 1455 of yacc.c  */
#line 2251 "asn_grammar.y"
        {
            (yyval.sval) = ConcatNames((yyvsp[(1) - (3)].sval), '.', (yyvsp[(3) - (3)].sval));
            ;
        }
        break;

    case 353:

        /* Line 1455 of yacc.c  */
#line 2265 "asn_grammar.y"
        {
            (yyval.dos) = new DefinedObjectSet(*(yyvsp[(1) - (1)].sval));
            delete(yyvsp[(1) - (1)].sval);
            ;
        }
        break;

    case 354:

        /* Line 1455 of yacc.c  */
#line 2270 "asn_grammar.y"
        {
            (yyval.dos) = new DefinedObjectSet(*(yyvsp[(1) - (1)].sval));
            delete(yyvsp[(1) - (1)].sval);
            ;
        }
        break;

    case 355:

        /* Line 1455 of yacc.c  */
#line 2278 "asn_grammar.y"
        {
            (yyval.sval) = ConcatNames((yyvsp[(1) - (3)].sval), '.', (yyvsp[(3) - (3)].sval));
            ;
        }
        break;

    case 356:

        /* Line 1455 of yacc.c  */
#line 2285 "asn_grammar.y"
        {
            (yyval.dos) = new ParameterizedObjectSet(*(yyvsp[(1) - (2)].sval), ActualParameterListPtr((yyvsp[(2) - (2)].aplt)));  delete(yyvsp[(1) - (2)].sval);
            ;
        }
        break;

    case 357:

        /* Line 1455 of yacc.c  */
#line 2289 "asn_grammar.y"
        {
            std::string* str = ConcatNames((yyvsp[(1) - (4)].sval), '.', (yyvsp[(3) - (4)].sval));
            (yyval.dos) = new ParameterizedObjectSet(*str, ActualParameterListPtr((yyvsp[(4) - (4)].aplt)));
            delete str;
            ;
        }
        break;

    case 358:

        /* Line 1455 of yacc.c  */
#line 2298 "asn_grammar.y"
        {
            (yyval.sval) = ConcatNames((yyvsp[(1) - (3)].sval), '.', (yyvsp[(3) - (3)].sval));
            ;
        }
        break;

    case 368:

        /* Line 1455 of yacc.c  */
#line 2328 "asn_grammar.y"
        {
            (yyval.objt) = new DefaultObjectDefn((yyvsp[(1) - (1)].bldr)->GetDefaultSyntax());
            delete(yyvsp[(1) - (1)].bldr);
            ;
        }
        break;

    case 369:

        /* Line 1455 of yacc.c  */
#line 2333 "asn_grammar.y"
        {
            (yyval.objt) = new DefaultObjectDefn(std::auto_ptr<FieldSettingList>((yyvsp[(1) - (1)].fldl)));
            ;
        }
        break;

    case 370:

        /* Line 1455 of yacc.c  */
#line 2340 "asn_grammar.y"
        {
            (yyval.fldl) = (yyvsp[(2) - (3)].fldl);
            if (InObjectSetContext)
                classStack->top()->PreParseObject();
            ;
        }
        break;

    case 371:

        /* Line 1455 of yacc.c  */
#line 2349 "asn_grammar.y"
        {
            classStack->top()->GetField(*(yyvsp[(3) - (3)].sval))->BeginParseSetting((yyvsp[(1) - (3)].fldl));
            ;
        }
        break;

    case 372:

        /* Line 1455 of yacc.c  */
#line 2353 "asn_grammar.y"
        {
            (yyval.fldl) = (yyvsp[(1) - (5)].fldl);
            classStack->top()->GetField(*(yyvsp[(3) - (5)].sval))->EndParseSetting();
            (yyvsp[(1) - (5)].fldl)->push_back(FieldSettingPtr(new FieldSetting(*(yyvsp[(3) - (5)].sval), std::auto_ptr<Setting>((yyvsp[(5) - (5)].sett)))));
            delete(yyvsp[(3) - (5)].sval);
            ;
        }
        break;

    case 373:

        /* Line 1455 of yacc.c  */
#line 2360 "asn_grammar.y"
        {
            classStack->top()->GetField(*(yyvsp[(1) - (1)].sval))->BeginParseSetting(NULL);
            ;
        }
        break;

    case 374:

        /* Line 1455 of yacc.c  */
#line 2364 "asn_grammar.y"
        {
            (yyval.fldl) = new FieldSettingList;
            classStack->top()->GetField(*(yyvsp[(1) - (3)].sval))->EndParseSetting();
            (yyval.fldl)->push_back(FieldSettingPtr(new FieldSetting(*(yyvsp[(1) - (3)].sval), std::auto_ptr<Setting>((yyvsp[(3) - (3)].sett)))));
            delete(yyvsp[(1) - (3)].sval);
            ;
        }
        break;

    case 375:

        /* Line 1455 of yacc.c  */
#line 2375 "asn_grammar.y"
        {
            (yyval.bldr) = (yyvsp[(2) - (3)].bldr);
            (yyval.bldr)->ResetTokenGroup();
            if (InObjectSetContext)
                classStack->top()->PreParseObject();
            ;
        }
        break;

    case 376:

        /* Line 1455 of yacc.c  */
#line 2385 "asn_grammar.y"
        {
            (yyval.bldr) = (yyvsp[(1) - (2)].bldr);
            (yyval.bldr)->AddToken((yyvsp[(2) - (2)].tken));
            ;
        }
        break;

    case 377:

        /* Line 1455 of yacc.c  */
#line 2390 "asn_grammar.y"
        {
            (yyval.bldr) = new DefaultSyntaxBuilder(classStack->top()->GetWithSyntax());
            ;
        }
        break;

    case 378:

        /* Line 1455 of yacc.c  */
#line 2397 "asn_grammar.y"
        {
            (yyval.tken) = new LiteralToken(*(yyvsp[(1) - (1)].sval)); delete(yyvsp[(1) - (1)].sval);
            ;
        }
        break;

    case 379:

        /* Line 1455 of yacc.c  */
#line 2401 "asn_grammar.y"
        {
            (yyval.tken) = new SettingToken(std::auto_ptr<Setting>((yyvsp[(1) - (1)].sett)));
            ;
        }
        break;

    case 380:

        /* Line 1455 of yacc.c  */
#line 2409 "asn_grammar.y"
        {
            (yyval.sett) = new TypeSetting(TypePtr((yyvsp[(1) - (1)].tval)));
            ;
        }
        break;

    case 381:

        /* Line 1455 of yacc.c  */
#line 2413 "asn_grammar.y"
        {
            if (ValueTypeContext.get() == NULL)
                std::cerr << StdError(Fatal) << "Parsing Error\n";
            (yyval.sett) = new ValueSetting(ValueTypeContext, ValuePtr((yyvsp[(1) - (1)].vval)));
            ;
        }
        break;

    case 382:

        /* Line 1455 of yacc.c  */
#line 2419 "asn_grammar.y"
        {
            (yyval.sett) = new ValueSetSetting(ValueSetPtr((yyvsp[(1) - (1)].vset)));
            ;
        }
        break;

    case 383:

        /* Line 1455 of yacc.c  */
#line 2423 "asn_grammar.y"
        {
            (yyval.sett) = new ObjectSetting(InformationObjectPtr((yyvsp[(1) - (1)].objt)), classStack->top());
            ;
        }
        break;

    case 384:

        /* Line 1455 of yacc.c  */
#line 2427 "asn_grammar.y"
        {
            (yyval.sett) = new ObjectSetSetting(ConstraintPtr((yyvsp[(1) - (1)].cons)), classStack->top());
            classStack->pop();
            ;
        }
        break;

    case 385:

        /* Line 1455 of yacc.c  */
#line 2436 "asn_grammar.y"
        {
            (yyval.cons) = (yyvsp[(2) - (3)].cons);
            classStack->top()->EndParseObjectSet();
            ;
        }
        break;

    case 387:

        /* Line 1455 of yacc.c  */
#line 2445 "asn_grammar.y"
        {
            (yyval.cons) = new Constraint(std::auto_ptr<ConstraintElementVector>(), true);
            ;
        }
        break;

    case 388:

        /* Line 1455 of yacc.c  */
#line 2453 "asn_grammar.y"
        {
            (yyval.elmt) = new SingleObjectConstraintElement(InformationObjectPtr((yyvsp[(1) - (1)].objt)));
            ;
        }
        break;

    case 389:

        /* Line 1455 of yacc.c  */
#line 2457 "asn_grammar.y"
        {
            (yyval.elmt) = (yyvsp[(1) - (1)].dos);
            ;
        }
        break;

    case 390:

        /* Line 1455 of yacc.c  */
#line 2461 "asn_grammar.y"
        {
            (yyval.elmt) = (yyvsp[(1) - (1)].osce);
            ;
        }
        break;

    case 391:

        /* Line 1455 of yacc.c  */
#line 2465 "asn_grammar.y"
        {
            (yyval.elmt) = (yyvsp[(1) - (1)].dos);
            ;
        }
        break;

    case 392:

        /* Line 1455 of yacc.c  */
#line 2478 "asn_grammar.y"
        {
            InformationFromObjectContext = (yyvsp[(1) - (1)].osce)->GetObjectClass();
            ;
        }
        break;

    case 393:

        /* Line 1455 of yacc.c  */
#line 2482 "asn_grammar.y"
        {
            (yyval.osce) = (yyvsp[(1) - (3)].osce);
            ;
        }
        break;

    case 394:

        /* Line 1455 of yacc.c  */
#line 2489 "asn_grammar.y"
        {
            InformationFromObjectContext = (yyvsp[(1) - (1)].objt)->GetObjectClass();
            ;
        }
        break;

    case 395:

        /* Line 1455 of yacc.c  */
#line 2493 "asn_grammar.y"
        {
            (yyval.objt) = (yyvsp[(1) - (3)].objt);
            ;
        }
        break;

    case 401:

        /* Line 1455 of yacc.c  */
#line 2511 "asn_grammar.y"
        {
            DummyParameters = (yyvsp[(2) - (2)].plst);
            ;
        }
        break;

    case 402:

        /* Line 1455 of yacc.c  */
#line 2515 "asn_grammar.y"
        {
            DummyParameters = NULL;
            (yyvsp[(5) - (5)].tval)->SetName(*(yyvsp[(1) - (5)].sval)); delete(yyvsp[(1) - (5)].sval);
            (yyvsp[(5) - (5)].tval)->SetParameters(*(yyvsp[(2) - (5)].plst)); delete(yyvsp[(2) - (5)].plst);
            Module->AddType(TypePtr((yyvsp[(5) - (5)].tval)));
            ;
        }
        break;

    case 403:

        /* Line 1455 of yacc.c  */
#line 2525 "asn_grammar.y"
        {
            DummyParameters = (yyvsp[(2) - (3)].plst);
            ;
        }
        break;

    case 404:

        /* Line 1455 of yacc.c  */
#line 2529 "asn_grammar.y"
        {
            DummyParameters = NULL;
            ;
        }
        break;

    case 405:

        /* Line 1455 of yacc.c  */
#line 2536 "asn_grammar.y"
        {
            DummyParameters = (yyvsp[(2) - (3)].plst);
            ;
        }
        break;

    case 406:

        /* Line 1455 of yacc.c  */
#line 2540 "asn_grammar.y"
        {
            DummyParameters = NULL;
            ;
        }
        break;

    case 407:

        /* Line 1455 of yacc.c  */
#line 2554 "asn_grammar.y"
        {
            DummyParameters = (yyvsp[(2) - (3)].plst);
            (yyvsp[(3) - (3)].dobj)->BeginParseObject();
            ;
        }
        break;

    case 408:

        /* Line 1455 of yacc.c  */
#line 2559 "asn_grammar.y"
        {
            (yyvsp[(3) - (6)].dobj)->EndParseObject();
            classStack->pop();
            DummyParameters = NULL;
            (yyvsp[(6) - (6)].objt)->SetName(*(yyvsp[(1) - (6)].sval)); delete(yyvsp[(1) - (6)].sval);
            (yyvsp[(6) - (6)].objt)->SetObjectClass((yyvsp[(3) - (6)].dobj));
            (yyvsp[(6) - (6)].objt)->SetParameters(std::auto_ptr<ParameterList>((yyvsp[(2) - (6)].plst)));
            Module->AddInformationObject(InformationObjectPtr((yyvsp[(6) - (6)].objt)));
            ;
        }
        break;

    case 409:

        /* Line 1455 of yacc.c  */
#line 2572 "asn_grammar.y"
        {
            DummyParameters = (yyvsp[(2) - (3)].plst);
            (yyvsp[(3) - (3)].dobj)->BeginParseObjectSet();
            ;
        }
        break;

    case 410:

        /* Line 1455 of yacc.c  */
#line 2577 "asn_grammar.y"
        {
            DummyParameters = NULL;
            Module->AddInformationObjectSet(InformationObjectSetPtr(
                                                new InformationObjectSetDefn(*(yyvsp[(1) - (6)].sval),
                                                                             ObjectClassBasePtr((yyvsp[(3) - (6)].dobj)),
                                                                             ConstraintPtr((yyvsp[(6) - (6)].cons)),
                                                                             ParameterListPtr((yyvsp[(2) - (6)].plst)))));
            delete(yyvsp[(1) - (6)].sval);
            classStack->pop();
            ;
        }
        break;

    case 411:

        /* Line 1455 of yacc.c  */
#line 2591 "asn_grammar.y"
        {
            (yyval.plst) = (yyvsp[(2) - (3)].plst);
            ;
        }
        break;

    case 412:

        /* Line 1455 of yacc.c  */
#line 2598 "asn_grammar.y"
        {
            (yyval.plst) = (yyvsp[(1) - (3)].plst);
            (yyval.plst)->Append(ParameterPtr((yyvsp[(3) - (3)].para)));
            ;
        }
        break;

    case 413:

        /* Line 1455 of yacc.c  */
#line 2603 "asn_grammar.y"
        {
            (yyval.plst) = new ParameterList;
            (yyval.plst)->Append(ParameterPtr((yyvsp[(1) - (1)].para)));
            ;
        }
        break;

    case 414:

        /* Line 1455 of yacc.c  */
#line 2611 "asn_grammar.y"
        {
            (yyval.para) = new ValueParameter(TypePtr((yyvsp[(1) - (3)].tval)), *(yyvsp[(3) - (3)].sval)); delete(yyvsp[(1) - (3)].tval);
            ;
        }
        break;

    case 415:

        /* Line 1455 of yacc.c  */
#line 2615 "asn_grammar.y"
        {
            (yyval.para) = new ObjectParameter(DefinedObjectClassPtr((yyvsp[(1) - (3)].dobj)), *(yyvsp[(3) - (3)].sval)); delete(yyvsp[(3) - (3)].sval);
            ;
        }
        break;

    case 416:

        /* Line 1455 of yacc.c  */
#line 2619 "asn_grammar.y"
        {
            (yyval.para) = new Parameter(*(yyvsp[(1) - (1)].sval)); delete(yyvsp[(1) - (1)].sval);
            ;
        }
        break;

    case 417:

        /* Line 1455 of yacc.c  */
#line 2626 "asn_grammar.y"
        {
            (yyval.tval) = new ParameterizedType(*(yyvsp[(1) - (2)].sval), *(yyvsp[(2) - (2)].aplt));
            delete(yyvsp[(1) - (2)].sval); delete(yyvsp[(2) - (2)].aplt);
            ;
        }
        break;

    case 418:

        /* Line 1455 of yacc.c  */
#line 2631 "asn_grammar.y"
        {
            std::string* str = ConcatNames((yyvsp[(1) - (4)].sval), '.', (yyvsp[(3) - (4)].sval));
            (yyval.tval) = new ParameterizedType(*str, *(yyvsp[(4) - (4)].aplt));
            delete str; delete(yyvsp[(4) - (4)].aplt);
            ;
        }
        break;

    case 421:

        /* Line 1455 of yacc.c  */
#line 2645 "asn_grammar.y"
        {
            (yyval.aplt) = (yyvsp[(2) - (3)].aplt);
            ;
        }
        break;

    case 422:

        /* Line 1455 of yacc.c  */
#line 2652 "asn_grammar.y"
        {
            (yyvsp[(1) - (3)].aplt)->push_back(ActualParameterPtr((yyvsp[(3) - (3)].apar)));
            (yyval.aplt) = (yyvsp[(1) - (3)].aplt);
            ;
        }
        break;

    case 423:

        /* Line 1455 of yacc.c  */
#line 2657 "asn_grammar.y"
        {
            (yyval.aplt) = new ActualParameterList;
            (yyval.aplt)->push_back(ActualParameterPtr((yyvsp[(1) - (1)].apar)));
            ;
        }
        break;

    case 424:

        /* Line 1455 of yacc.c  */
#line 2665 "asn_grammar.y"
        {
            (yyval.apar) = new ActualTypeParameter(TypePtr((yyvsp[(1) - (1)].tval)));
            ;
        }
        break;

    case 425:

        /* Line 1455 of yacc.c  */
#line 2669 "asn_grammar.y"
        {
            (yyval.apar) = new ActualValueParameter(ValuePtr((yyvsp[(1) - (1)].vval)));
            ;
        }
        break;

    case 426:

        /* Line 1455 of yacc.c  */
#line 2673 "asn_grammar.y"
        {
            (yyval.apar) = new ActualValueSetParameter(TypePtr((yyvsp[(2) - (3)].tval)));
            ;
        }
        break;

    case 427:

        /* Line 1455 of yacc.c  */
#line 2681 "asn_grammar.y"
        {
            (yyval.apar) = new ActualObjectParameter(InformationObjectPtr((yyvsp[(1) - (1)].objt)));
            ;
        }
        break;

    case 428:

        /* Line 1455 of yacc.c  */
#line 2685 "asn_grammar.y"
        {
            (yyval.apar) = new ActualObjectSetParameter(boost::shared_ptr<ObjectSetConstraintElement>((yyvsp[(2) - (3)].osce)));
            ;
        }
        break;

    case 431:

        /* Line 1455 of yacc.c  */
#line 2697 "asn_grammar.y"
        {
            (yyval.osce) = (yyvsp[(1) - (1)].dos);
            ;
        }
        break;

    case 432:

        /* Line 1455 of yacc.c  */
#line 2701 "asn_grammar.y"
        {
            (yyval.osce) = (yyvsp[(1) - (1)].dos);
            ;
        }
        break;

    case 433:

        /* Line 1455 of yacc.c  */
#line 2709 "asn_grammar.y"
        {
            ValueTypeContext.reset((yyvsp[(2) - (2)].tval));
            (yyvsp[(2) - (2)].tval)->BeginParseValue();
            ;
        }
        break;

    case 434:

        /* Line 1455 of yacc.c  */
#line 2714 "asn_grammar.y"
        {
            (yyvsp[(2) - (5)].tval)->EndParseValue();
            (yyvsp[(5) - (5)].vval)->SetValueName(*(yyvsp[(1) - (5)].sval)); delete(yyvsp[(1) - (5)].sval);
            Module->AddValue(ValuePtr((yyvsp[(5) - (5)].vval)));
            ;
        }
        break;

    case 441:

        /* Line 1455 of yacc.c  */
#line 2738 "asn_grammar.y"
        {
            (yyval.vval) = new IntegerValue((yyvsp[(1) - (1)].ival));
            ;
        }
        break;

    case 447:

        /* Line 1455 of yacc.c  */
#line 2760 "asn_grammar.y"
        {
            (yyval.vval) = new DefinedValue(*(yyvsp[(1) - (1)].sval)); delete(yyvsp[(1) - (1)].sval);
            ;
        }
        break;

    case 448:

        /* Line 1455 of yacc.c  */
#line 2764 "asn_grammar.y"
        {
            (yyval.vval) = new DefinedValue(*(yyvsp[(1) - (1)].sval)); delete(yyvsp[(1) - (1)].sval);
            ;
        }
        break;

    case 449:

        /* Line 1455 of yacc.c  */
#line 2774 "asn_grammar.y"
        {
            *(yyvsp[(1) - (3)].sval) += *(yyvsp[(3) - (3)].sval);
            delete(yyvsp[(3) - (3)].sval);
            ;
        }
        break;

    case 450:

        /* Line 1455 of yacc.c  */
#line 2782 "asn_grammar.y"
        {
            (yyval.vval) = new ObjectIdentifierValue(*(yyvsp[(2) - (3)].slst)); delete(yyvsp[(2) - (3)].slst);
            ;
        }
        break;

    case 451:

        /* Line 1455 of yacc.c  */
#line 2796 "asn_grammar.y"
        {
            (yyval.slst) = new StringList;
            (yyval.slst)->push_back(*(yyvsp[(1) - (1)].sval));
            delete(yyvsp[(1) - (1)].sval);
            ;
        }
        break;

    case 452:

        /* Line 1455 of yacc.c  */
#line 2808 "asn_grammar.y"
        {
            (yyvsp[(1) - (2)].slst)->push_back(*(yyvsp[(2) - (2)].sval));
            (yyval.slst) = (yyvsp[(1) - (2)].slst);
            delete(yyvsp[(2) - (2)].sval)
            ;
        }
        break;

    case 455:

        /* Line 1455 of yacc.c  */
#line 2819 "asn_grammar.y"
        {
            delete(yyvsp[(1) - (4)].sval);
            (yyval.sval) = (yyvsp[(3) - (4)].sval);
            ;
        }
        break;

    case 459:

        /* Line 1455 of yacc.c  */
#line 2834 "asn_grammar.y"
        {
            (yyval.vval) = new OctetStringValue(*(yyvsp[(1) - (1)].sval)); delete(yyvsp[(1) - (1)].sval);
            ;
        }
        break;

    case 460:

        /* Line 1455 of yacc.c  */
#line 2838 "asn_grammar.y"
        {
            (yyval.vval) = new OctetStringValue(*(yyvsp[(1) - (1)].sval)); delete(yyvsp[(1) - (1)].sval);
            ;
        }
        break;

    case 461:

        /* Line 1455 of yacc.c  */
#line 2845 "asn_grammar.y"
        {
            (yyval.vval) = new BitStringValue(*(yyvsp[(1) - (1)].sval)); delete(yyvsp[(1) - (1)].sval);
            ;
        }
        break;

    case 462:

        /* Line 1455 of yacc.c  */
#line 2849 "asn_grammar.y"
        {
            (yyval.vval) = new BitStringValue(*(yyvsp[(1) - (1)].sval)); delete(yyvsp[(1) - (1)].sval);
            ;
        }
        break;

    case 463:

        /* Line 1455 of yacc.c  */
#line 2853 "asn_grammar.y"
        {
            (yyval.vval) = new BitStringValue((yyvsp[(2) - (3)].slst));
            ;
        }
        break;

    case 464:

        /* Line 1455 of yacc.c  */
#line 2861 "asn_grammar.y"
        {
            // Look up $1
            (yyval.slst) = new StringList;
            ;
        }
        break;

    case 465:

        /* Line 1455 of yacc.c  */
#line 2866 "asn_grammar.y"
        {
            // Look up $3
            (yyvsp[(1) - (3)].slst)->push_back(std::string("0"));
            ;
        }
        break;

    case 466:

        /* Line 1455 of yacc.c  */
#line 2875 "asn_grammar.y"
        {
            (yyval.vval) = new BooleanValue(true);
            ;
        }
        break;

    case 467:

        /* Line 1455 of yacc.c  */
#line 2879 "asn_grammar.y"
        {
            (yyval.vval) = new BooleanValue(false);
            ;
        }
        break;

    case 469:

        /* Line 1455 of yacc.c  */
#line 2894 "asn_grammar.y"
        {
            (yyval.vval) = new CharacterStringValue(*(yyvsp[(1) - (1)].sval)); delete(yyvsp[(1) - (1)].sval);
            ;
        }
        break;

    case 473:

        /* Line 1455 of yacc.c  */
#line 2904 "asn_grammar.y"
        {
            (yyval.vval) = new CharacterStringValue(*(yyvsp[(2) - (3)].slst)); delete(yyvsp[(2) - (3)].slst);
            ;
        }
        break;

    case 474:

        /* Line 1455 of yacc.c  */
#line 2911 "asn_grammar.y"
        {
            (yyval.slst) = new StringList;
            (yyval.slst)->push_back(*(yyvsp[(1) - (1)].sval));
            delete(yyvsp[(1) - (1)].sval);
            ;
        }
        break;

    case 475:

        /* Line 1455 of yacc.c  */
#line 2917 "asn_grammar.y"
        {
            (yyvsp[(1) - (3)].slst)->push_back(*(yyvsp[(3) - (3)].sval));
            delete(yyvsp[(3) - (3)].sval);
            ;
        }
        break;

    case 477:

        /* Line 1455 of yacc.c  */
#line 2926 "asn_grammar.y"
        {
            std::cerr << StdError(Warning) << "DefinedValue in string unsupported" << std::endl;
            ;
        }
        break;

    case 478:

        /* Line 1455 of yacc.c  */
#line 2933 "asn_grammar.y"
        {
            if ((yyvsp[(2) - (9)].ival) != 0 || (yyvsp[(4) - (9)].ival) != 0 || (yyvsp[(6) - (9)].ival) > 255 || (yyvsp[(8) - (9)].ival) > 255)
                std::cerr << StdError(Warning) << "Illegal value in Character Quadruple" << std::endl;
            (yyval.vval) = new CharacterValue((unsigned char)(yyvsp[(2) - (9)].ival), (unsigned char)(yyvsp[(4) - (9)].ival), (unsigned char)(yyvsp[(6) - (9)].ival), (unsigned char)(yyvsp[(8) - (9)].ival));
            ;
        }
        break;

    case 479:

        /* Line 1455 of yacc.c  */
#line 2942 "asn_grammar.y"
        {
            if ((yyvsp[(2) - (5)].ival) > 255 || (yyvsp[(4) - (5)].ival) > 255)
                std::cerr << StdError(Warning) << "Illegal value in Character Tuple" << std::endl;
            (yyval.vval) = new CharacterValue((unsigned char)(yyvsp[(2) - (5)].ival), (unsigned char)(yyvsp[(4) - (5)].ival));
            ;
        }
        break;

    case 480:

        /* Line 1455 of yacc.c  */
#line 2952 "asn_grammar.y"
        {
            (yyval.vval) = new ChoiceValue(ValueTypeContext, *(yyvsp[(1) - (3)].sval), ValuePtr((yyvsp[(3) - (3)].vval))); delete(yyvsp[(1) - (3)].sval);
            ;
        }
        break;

    case 481:

        /* Line 1455 of yacc.c  */
#line 2960 "asn_grammar.y"
        {
            (yyval.vval) = new NullValue;
            ;
        }
        break;

    case 484:

        /* Line 1455 of yacc.c  */
#line 2973 "asn_grammar.y"
        {
            (yyval.vval) = new RealValue(0);
            ;
        }
        break;

    case 485:

        /* Line 1455 of yacc.c  */
#line 2983 "asn_grammar.y"
        {
            (yyval.vval) = new RealValue(0);
            ;
        }
        break;

    case 486:

        /* Line 1455 of yacc.c  */
#line 2987 "asn_grammar.y"
        {
            (yyval.vval) = new RealValue(0);
            ;
        }
        break;

    case 487:

        /* Line 1455 of yacc.c  */
#line 2995 "asn_grammar.y"
        {
            (yyval.vval) = new SequenceValue((yyvsp[(2) - (3)].vlst));
            ;
        }
        break;

    case 488:

        /* Line 1455 of yacc.c  */
#line 2999 "asn_grammar.y"
        {
            (yyval.vval) = new SequenceValue;
            ;
        }
        break;

    case 489:

        /* Line 1455 of yacc.c  */
#line 3006 "asn_grammar.y"
        {
            (yyval.vlst) = new ValuesList;
            (yyval.vlst)->push_back(ValuePtr((yyvsp[(1) - (1)].vval)));
            ;
        }
        break;

    case 490:

        /* Line 1455 of yacc.c  */
#line 3011 "asn_grammar.y"
        {
            (yyvsp[(1) - (3)].vlst)->push_back(ValuePtr((yyvsp[(3) - (3)].vval)));
            ;
        }
        break;

    case 491:

        /* Line 1455 of yacc.c  */
#line 3018 "asn_grammar.y"
        {
            (yyvsp[(2) - (2)].vval)->SetValueName(*(yyvsp[(1) - (2)].sval)); delete(yyvsp[(1) - (2)].sval);
            (yyval.vval) = (yyvsp[(2) - (2)].vval);
            ;
        }
        break;

    case 494:

        /* Line 1455 of yacc.c  */
#line 3065 "asn_grammar.y"
        {
            ValueSetting* setting = (ValueSetting*)(yyvsp[(1) - (2)].objt)->GetSetting(*(yyvsp[(2) - (2)].sval));
            (yyval.vval) = new DefinedValue(setting->GetValue());
            delete(yyvsp[(1) - (2)].objt);
            delete(yyvsp[(2) - (2)].sval);
            InformationFromObjectContext = NULL;
            ;
        }
        break;

    case 497:

        /* Line 1455 of yacc.c  */
#line 3081 "asn_grammar.y"
        {
            (yyval.vset) = new ValueSetFromObject(InformationObjectPtr((yyvsp[(1) - (2)].objt)), *(yyvsp[(2) - (2)].sval)); delete(yyvsp[(2) - (2)].sval);
            InformationFromObjectContext = NULL;
            ;
        }
        break;

    case 500:

        /* Line 1455 of yacc.c  */
#line 3094 "asn_grammar.y"
        {
            (yyval.objt) = new ObjectFromObject(InformationObjectPtr((yyvsp[(1) - (2)].objt)), *(yyvsp[(2) - (2)].sval)); delete(yyvsp[(2) - (2)].sval);
            InformationFromObjectContext = NULL;
            ;
        }
        break;

    case 501:

        /* Line 1455 of yacc.c  */
#line 3102 "asn_grammar.y"
        {
            (yyval.osce) = new ObjectSetFromObject(InformationObjectPtr((yyvsp[(1) - (2)].objt)), *(yyvsp[(2) - (2)].sval)); delete(yyvsp[(2) - (2)].sval);
            InformationFromObjectContext = NULL;
            ;
        }
        break;

    case 502:

        /* Line 1455 of yacc.c  */
#line 3126 "asn_grammar.y"
        {
            if (ValueTypeContext.get() == NULL)
                std::cerr << StdError(Warning) << "";
            // $$ = new ValueSetDefn(TypePtr(new DefinedType(ValueTypeContext)), ConstraintPtr($2));
            (yyval.vset) = new ValueSetDefn(ValueTypeContext, ConstraintPtr((yyvsp[(2) - (3)].cons)));
            ValueTypeContext->EndParseValueSet();
            ;
        }
        break;

    case 514:

        /* Line 1455 of yacc.c  */
#line 3281 "asn_grammar.y"
        {
            (yyval.nlst) = new NamedNumberList;
            (yyval.nlst)->push_back(NamedNumberPtr((yyvsp[(1) - (1)].nval)));
            ;
        }
        break;

    case 515:

        /* Line 1455 of yacc.c  */
#line 3286 "asn_grammar.y"
        {
            (yyvsp[(1) - (3)].nlst)->push_back(NamedNumberPtr((yyvsp[(3) - (3)].nval)));
            ;
        }
        break;

    case 516:

        /* Line 1455 of yacc.c  */
#line 3293 "asn_grammar.y"
        {
            (yyval.nval) = new NamedNumber((yyvsp[(1) - (4)].sval), (int)(yyvsp[(3) - (4)].ival));
            ;
        }
        break;

    case 517:

        /* Line 1455 of yacc.c  */
#line 3297 "asn_grammar.y"
        {
            (yyval.nval) = new NamedNumber((yyvsp[(1) - (4)].sval), ((DefinedValue*)(yyvsp[(3) - (4)].vval))->GetReference());
            delete(yyvsp[(3) - (4)].vval);
            ;
        }
        break;

    case 519:

        /* Line 1455 of yacc.c  */
#line 3307 "asn_grammar.y"
        {
            (yyval.ival) = -(yyvsp[(2) - (2)].ival);
            ;
        }
        break;



        /* Line 1455 of yacc.c  */
#line 6499 "asn_grammar.tab.c"
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



