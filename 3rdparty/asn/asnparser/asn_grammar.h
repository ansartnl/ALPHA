
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
typedef union YYSTYPE
{

/* Line 1676 of yacc.c  */
#line 428 "asn_grammar.y"

  boost::int64_t       ival;
  std::string	        * sval;
  StringList	    * slst;
  TypeBase	        * tval;
  TypePtr           * tptr;
  TypesVector       * tlst;
  ValueBase	        * vval;
  ValuesList            * vlst;
  NamedNumber	        * nval;
  NamedNumberList       * nlst;
  Constraint            * cons;
  ConstraintElementVector * elst;
  ConstraintElementBase * elmt;
  FieldSpec             * fspc;
  FieldSpecsList		* flst;
  ObjectClassBase		* objc;
  DefinedObjectClass    * dobj;
  TokenGroup            * tgrp;
  TokenOrGroupSpec      * togs;
  Setting				* sett;
  InformationObject     * objt;
  InformationObjectSet  * objs;
  FieldSettingList      * fldl;
  DefaultSyntaxBuilder  * bldr;
  DefinedSyntaxToken    * tken;
  ValueSet              * vset;
  ObjectSetConstraintElement* osce;
  Symbol                * symb;
  SymbolList            * syml;
  Parameter             * para;
  ParameterList         * plst;
  ActualParameter       * apar;
  ActualParameterList   * aplt;
  TableConstraint       * tcons;
  ObjectClassFieldType  * ocft;
  DefinedObjectSet      * dos;

  struct {
    Tag::Type tagClass;
    unsigned tagNumber;
  } tagv;



/* Line 1676 of yacc.c  */
#line 216 "asn_grammar.tab.h"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif

extern YYSTYPE yylval;


