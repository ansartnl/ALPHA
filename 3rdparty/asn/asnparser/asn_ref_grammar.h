
/* A Bison parser, made by GNU Bison 2.4.1.  */

/* Skeleton interface for Bison's Yacc-like parsers in C
   
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
typedef union YYSTYPE
{

/* Line 1676 of yacc.c  */
#line 187 "asn_ref_grammar.y"

  boost::int64_t              ival;
  std::string				* sval;
  StringList				* slst;
  struct {
    Tag::Type tagClass;
    unsigned tagNumber;
  } tagv;



/* Line 1676 of yacc.c  */
#line 156 "asn_ref_grammar.tab.h"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif

extern YYSTYPE idlval;


