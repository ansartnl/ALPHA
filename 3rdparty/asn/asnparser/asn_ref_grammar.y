%expect 12

%{

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
#pragma warning(disable:4701)
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

%}

%token MODULEREFERENCE 
%token TYPEREFERENCE  
%token OBJECTCLASSREFERENCE 
%token IDENTIFIER
%token WOULDBE_OBJECTCLASSREFERENCE
%token FIELDREFERENCE
%token INTEGER

%token CSTRING
%token BSTRING
%token HSTRING

%token ABSENT           
%token ABSTRACT_SYNTAX  
%token ALL              
%token ANY
%token APPLICATION      
%token ASSIGNMENT
%token AUTOMATIC        
%token BEGIN_t
%token BIT              
%token BMPString
%token BOOLEAN_t
%token BY
%token CHARACTER        
%token CHOICE           
%token CLASS            
%token COMPONENT        
%token COMPONENTS       
%token CONSTRAINED      
%token DEFAULT          
%token DEFINED
%token DEFINITIONS      
%token EMBEDDED         
%token END
%token ENUMERATED       
%token EXCEPT           
%token EXPLICIT         
%token EXPORTS          
%token EXTERNAL         
%token FALSE_t           
%token FROM             
%token GeneralString    
%token GraphicString    
%token IA5String        
%token TYPE_IDENTIFIER 
%token IDENTIFIER_t
%token IMPLICIT         
%token IMPORTS          
%token INCLUDES         
%token INSTANCE         
%token INTEGER_t        
%token INTERSECTION     
%token ISO646String
%token MACRO
%token MAX              
%token MIN              
%token MINUS_INFINITY
%token NOTATION
%token NULL_t
%token NumericString    
%token OBJECT           
%token OCTET            
%token OF_t              
%token OPTIONAL_t
%token PDV              
%token PLUS_INFINITY    
%token PRESENT          
%token PrintableString  
%token PRIVATE          
%token REAL             
%token SEQUENCE         
%token SET              
%token SIZE_t            
%token STRING           
%token SYNTAX           
%token T61String        
%token TAGS             
%token TeletexString    
%token TRUE_t
%token TYPE_t
%token UNION            
%token UNIQUE           
%token UNIVERSAL        
%token UniversalString  
%token VideotexString   
%token VisibleString    
%token GeneralizedTime
%token UTCTime
%token VALUE
%token WITH

%token ObjectDescriptor_t


%type <ival> INTEGER
%type <ival> TagDefault

%type <sval> IDENTIFIER
%type <sval> MODULEREFERENCE
%type <sval> TYPEREFERENCE
%type <sval> TypeReference
%type <sval> OBJECTCLASSREFERENCE
%type <sval> WOULDBE_OBJECTCLASSREFERENCE
%type <sval> FIELDREFERENCE
%type <sval> GlobalModuleReference
%type <sval> Reference
%type <sval> FieldName
%type <sval> Symbol
%type <sval> ParameterizedReference
%type <sval> ParameterizedTypeReference
%type <sval> ParameterizedWouldbeObjectClassReference
%type <sval> ParameterizedIdentifier

%type <slst> SymbolList

%union {
  boost::int64_t              ival;
  std::string				* sval;
  StringList				* slst;
  struct {
    Tag::Type tagClass;
    unsigned tagNumber;
  } tagv;
}


%%

ModuleDefinitionList
  : ModuleDefinitionList ModuleDefinition
    {  }
  | ModuleDefinition
    {  }

ModuleDefinition
  : TypeReference '{' '}' DEFINITIONS TagDefault ASSIGNMENT BEGIN_t
      {
	Module = new ModuleDefinition(*$1, (Tag::Mode)$5);
	Modules.push_back(ModuleDefinitionPtr(Module));
	delete $1;
      }
    ModuleBody END
	  {
    Module = NULL;
	  }
  | TypeReference DEFINITIONS TagDefault ASSIGNMENT BEGIN_t
      {
	Module = new ModuleDefinition(*$1, (Tag::Mode)$3);
	Modules.push_back(ModuleDefinitionPtr(Module));
	delete $1;
      }
    ModuleBody END
	  {
    Module = NULL;
	  }
  ;


TagDefault
  : EXPLICIT TAGS
      {
	$$ = Tag::Explicit;
      }
  | IMPLICIT TAGS 
      {
	$$ = Tag::Implicit;
      }
  | AUTOMATIC TAGS 
      {
	$$ = Tag::Automatic;
      }
  | /* empty */
      {
	$$ = Tag::Explicit;
      }
  ;


/*************************************/

ModuleBody
  : Exports Imports AssignmentList
  | /* empty */
  ;


Exports
  : EXPORTS SymbolsExported ';'
  | /* empty */
  ;


SymbolsExported
  : SymbolList
      {  }
  | /* empty */
      {  }
  ;


Imports
  : IMPORTS SymbolsImported ';' 
  | /* empty */
  ;


SymbolsImported
  : SymbolsFromModuleList
  | /* empty */
  ;

SymbolsFromModuleList
  : SymbolsFromModule
  | SymbolsFromModuleList SymbolsFromModule
  ;


SymbolsFromModule
  : SymbolList FROM GlobalModuleReference
      {
    Module->AddImportedIdentifiers(*$1, *$3);
    delete $1;
    delete $3;
	  }
  ;


GlobalModuleReference
  : TypeReference AssignedIdentifier
      {
	$$ = $1;
      }
  ;


AssignedIdentifier
  : DefinedValue
  | ObjectIdentifierValue
  | /* empty */
  ;

SymbolList
  : Symbol
      {
	$$ = new StringList;
	$$->push_back(*$1);
	delete $1;
      }
  | Symbol ',' SymbolList
      {
	$3->push_back(*$1);
	delete $1;
	$$ = $3;
      }
  ;



Symbol
  : Reference
  | ParameterizedReference  
  ;

ParameterizedReference 
  : Reference '{' '}'
      {
	*$1 += "{}";
	$$ = $1;
	  }
  ;
/*************************************/

AssignmentList: Assignment 
  | AssignmentList Assignment
  ;


Assignment
  : TypeAssignment
  | ValueAssignment
  | ValueSetTypeAssignment 
  | ObjectClassAssignment
  | ObjectAssignment
  | ObjectSetAssignment
  | ParameterizedAssignment 
  ;


ValueSetTypeAssignment
  : TYPEREFERENCE Type ASSIGNMENT '{' '}'
      {
	Module->AddIdentifier($1, TYPEREFERENCE);
      }
   | WOULDBE_OBJECTCLASSREFERENCE Type ASSIGNMENT '{' '}'
      {
	Module->AddIdentifier($1, TYPEREFERENCE);
      }
    
   ;




/********/

TypeAssignment
  : TYPEREFERENCE ASSIGNMENT Type
      {
	Module->AddIdentifier($1, TYPEREFERENCE);
      }
  | WOULDBE_OBJECTCLASSREFERENCE ASSIGNMENT Type
      {
	Module->AddIdentifier($1, TYPEREFERENCE);
      } 
  ;


Type
  : ConstrainedType
  | ReferencedType
  | BuiltinType
  ;


BuiltinType
  : BitStringType
  | BooleanType 
  | CharacterStringType 
  | ChoiceType 
  | EmbeddedPDVType 
  | EnumeratedType 
  | ExternalType 
  | AnyType 
  | InstanceOfType
  | IntegerType 
  | NullType 
  | ObjectClassFieldType
  | ObjectIdentifierType 
  | OctetStringType 
  | RealType 
  | SequenceType 
  | SequenceOfType 
  | SetType 
  | SetOfType 
  | TaggedType
  ;


ReferencedType
  : DefinedType
  | UsefulType
  | SelectionType
  | InformationFromObjects
  ;


DefinedType
  : ExternalTypeReference
  | TypeReference
      {
  delete $1;
	  }
  | ParameterizedType
/*| ParameterizedValueSetType	synonym for ParameterizedType */
  ;


ExternalTypeReference
  : MODULEREFERENCE '.' TypeReference
      {
   delete $1;
   delete $3;
      }
  ;

ParameterizedType
  : SimpleDefinedType '{' '}'
  ;

SimpleDefinedType 
  : ExternalTypeReference
  | TypeReference
      {
  delete $1;
	  }
  ;

BitStringType
  : BIT STRING 
  | BIT STRING '{' '}'
  ;


BooleanType
  : BOOLEAN_t
  ;


CharacterStringType
  : RestrictedCharacterStringType
  | UnrestrictedCharacterStringType
  ;

RestrictedCharacterStringType
  : BMPString
  | GeneralString
  | GraphicString
  | IA5String
  | ISO646String
  | NumericString
  | PrintableString
  | TeletexString
  | T61String
  | UniversalString
  | VideotexString
  | VisibleString
  ;


UnrestrictedCharacterStringType
  : CHARACTER STRING
  ;


ChoiceType
  : CHOICE '{' '}'
  ;




EmbeddedPDVType
  : EMBEDDED PDV
  ;


EnumeratedType
  : ENUMERATED '{' '}'
  ;

ExternalType
  : EXTERNAL
  ;


AnyType
  : ANY
  | ANY DEFINED BY IDENTIFIER
  ;


InstanceOfType
  : INSTANCE OF_t DefinedObjectClass
  ;


IntegerType
  : INTEGER_t
  | INTEGER_t '{' '}'
  ;


NullType
  : NULL_t
  ;


ObjectClassFieldType
  : DefinedObjectClass '.' FieldName
  ;


ObjectIdentifierType
  : OBJECT IDENTIFIER_t
  ;

OctetStringType
  : OCTET STRING
  ;


RealType
  : REAL
  ;


SequenceType
  : SEQUENCE '{'  '}'
  ;

SequenceOfType
  : SEQUENCE OF_t Type
  ;


SetType
  : SET '{'  '}'
  ;


SetOfType
  : SET OF_t Type
  ;


TaggedType
  : Tag Type
  | Tag IMPLICIT Type  
  | Tag EXPLICIT Type
  ;

Tag
  : '[' ']'
  ;


SelectionType
  : IDENTIFIER '<' Type
      {
  delete $1;
      }
  ;


UsefulType
  : GeneralizedTime
  | UTCTime
  | ObjectDescriptor_t
  ;


InformationFromObjects
  : ReferencedObjects '.' FieldName
      {
	  }
  ;

ReferencedObjects
  : IDENTIFIER
      {
  delete $1;
      }
  | IDENTIFIER '{' '}'
      {
  delete $1;
      }
  | TypeReference
     {
  delete $1;
	 }
  | ParameterizedType
  ;


/********/

ConstrainedType
  : Type Constraint
  | TypeWithConstraint
  ;

TypeWithConstraint
  : SET Constraint OF_t Type
  | SET SizeConstraint OF_t Type
  | SEQUENCE Constraint OF_t Type
  | SEQUENCE SizeConstraint OF_t Type
  ;

Constraint
  : '('  ')'
  ;



SizeConstraint
  : SIZE_t Constraint
  ;

/********/

ObjectClassAssignment
  : WOULDBE_OBJECTCLASSREFERENCE ASSIGNMENT ObjectClass
    {
  Module->AddIdentifier($1, OBJECTCLASSREFERENCE);
    }
  ;

ObjectAssignment
  : IDENTIFIER DefinedObjectClass ASSIGNMENT Object
    {
  Module->AddIdentifier($1, OBJECTREFERENCE);
	}
  ;

ObjectSetAssignment
  : TYPEREFERENCE DefinedObjectClass ASSIGNMENT ObjectSet
    {
  Module->AddIdentifier($1, OBJECTSETREFERENCE);
	}
  | WOULDBE_OBJECTCLASSREFERENCE DefinedObjectClass ASSIGNMENT ObjectSet
    {
  Module->AddIdentifier($1, OBJECTSETREFERENCE);
	}
  ;


ObjectClass
  : DefinedObjectClass
  | ObjectClassDefn 
  | ParameterizedObjectClass
  ;

DefinedObjectClass
  : ExternalObjectClassReference
  | OBJECTCLASSREFERENCE
    {
  delete $1;
    }
  | UsefulObjectClassReference
  ;


ExternalObjectClassReference
  : MODULEREFERENCE '.' OBJECTCLASSREFERENCE
	{
  delete $1;
  delete $3;	
	}
  ;

UsefulObjectClassReference
  : TYPE_IDENTIFIER
  | ABSTRACT_SYNTAX
  ;


ObjectClassDefn
  : CLASS  '{'  '}'  WithSyntaxSpec
  ;

WithSyntaxSpec
  : WITH SYNTAX '{' '}'
  | /* empty */
  ;

DefinedObject
  : ExternalObjectReference
  | IDENTIFIER
    {
  delete $1;
	}
  ;

ExternalObjectReference
  : MODULEREFERENCE '.' IDENTIFIER
    {
  delete $1;
  delete $3;
	}
  ;


ParameterizedObjectClass
  : DefinedObjectClass  '{' '}'
  ;



FieldName
  : FieldName '.' FIELDREFERENCE
    {
  delete $3;
	}
  | FIELDREFERENCE
    {
  delete $1;
	}
  ;


Object
  : DefinedObject
  | '{' '}'
  | InformationFromObjects
/*  | ParameterizedObject */
  ;



ObjectSet
  : '{' '}'
  ;




/********/

ParameterizedAssignment
  : ParameterizedTypeAssignment
  | ParameterizedValueAssignment
  | ParameterizedValueSetTypeAssignment
  | ParameterizedObjectClassAssignment
  | ParameterizedObjectAssignment
  | ParameterizedObjectSetAssignment 
  ;

ParameterizedTypeAssignment
  : ParameterizedTypeReference ASSIGNMENT Type
      {
    Module->AddIdentifier($1, PARAMETERIZEDTYPEREFERENCE);
      }
  | ParameterizedWouldbeObjectClassReference ASSIGNMENT Type
      {
    Module->AddIdentifier($1, PARAMETERIZEDTYPEREFERENCE);
      }
     
  ;

ParameterizedValueAssignment
  : ParameterizedIdentifier Type	ASSIGNMENT Value
      {
    Module->AddIdentifier($1, PARAMETERIZEDVALUEREFERENCE); 
	  }
  ;

ParameterizedValueSetTypeAssignment
  : ParameterizedTypeReference Type ASSIGNMENT '{' '}'
      { 
    Module->AddIdentifier($1, PARAMETERIZEDTYPEREFERENCE);
	  }
  | ParameterizedWouldbeObjectClassReference Type ASSIGNMENT '{' '}'
      { 
    Module->AddIdentifier($1, PARAMETERIZEDTYPEREFERENCE);
	  }
  ;

ParameterizedObjectClassAssignment
  : ParameterizedWouldbeObjectClassReference ASSIGNMENT ObjectClass
      { 
    Module->AddIdentifier($1, PARAMETERIZEDOBJECTCLASSREFERENCE);
	  }
  ;

ParameterizedObjectAssignment
  : ParameterizedIdentifier DefinedObjectClass ASSIGNMENT Object
      { 
    Module->AddIdentifier($1, PARAMETERIZEDOBJECTREFERENCE);
	  }
  ;

ParameterizedObjectSetAssignment
  : ParameterizedTypeReference DefinedObjectClass ASSIGNMENT ObjectSet
      { 
    Module->AddIdentifier($1, PARAMETERIZEDOBJECTSETREFERENCE);
	  }
  | ParameterizedWouldbeObjectClassReference DefinedObjectClass ASSIGNMENT ObjectSet
      { 
    Module->AddIdentifier($1, PARAMETERIZEDOBJECTSETREFERENCE);
	  }
  ;

ParameterizedWouldbeObjectClassReference
  : WOULDBE_OBJECTCLASSREFERENCE ParameterList
  ;
	    
ParameterizedTypeReference
  : TYPEREFERENCE ParameterList
  ;

ParameterizedIdentifier
  : IDENTIFIER ParameterList
  ;

ParameterList 
  : '{' '}'
  ;


/********/

ValueAssignment 
  : IDENTIFIER Type	ASSIGNMENT Value
      {
  Module->AddIdentifier($1, VALUEREFERENCE);
	  }
  ;

Value
  : NonOpenTypeValue
  | ObjectClassFieldValue
  ;

NonOpenTypeValue
  : BuiltinValue
  | ReferencedValue
  ;

BuiltinValue
  : BitStringValue 
  | BooleanValue 
  | CharacterStringValue 
  | ChoiceValue 
/*| EmbeddedPDVValue  synonym to SequenceValue */
/*| EnumeratedValue   synonym to IDENTIFIER    */
/*| ExternalValue     synonym to SequenceValue */
/*| InstanceOfValue   synonym to Value */
/*| SignedNumber       */ 
  | NullValue 
  | ObjectIdentifierValue
  | RealValue 
/*!!!! 
  | SequenceValue 
  | SequenceOfValue
*/
/*| SetValue	      synonym to SequenceValue */
/*| SetOfValue	      synonym to SequenceOfValue */
/*| TaggedValue	      synonym to Value */
  ;


DefinedValue
  : ExternalValueReference
  | IDENTIFIER
      {
  delete $1;
	  }
  | ParameterizedValue
  ;


ExternalValueReference
  : MODULEREFERENCE '.' IDENTIFIER
      {
  delete $1;
  delete $3;
      }
  ;


ObjectIdentifierValue
  : '{' '}'
  ;


BitStringValue
  : BSTRING
  | HSTRING
  ;

BooleanValue
  : TRUE_t
  | FALSE_t
  ;


CharacterStringValue
  : RestrictedCharacterStringValue
/*!!!
  | UnrestrictedCharacterStringValue
*/
  ;

RestrictedCharacterStringValue
  : CSTRING
  ;

ChoiceValue
  : IDENTIFIER ':' Value
      {  
  delete $1;
	  }
  ;


NullValue
  : NULL_t
  ;


RealValue
  : NumericRealValue 
  | SpecialRealValue
  ;

NumericRealValue
  :  '0'
  ;

SpecialRealValue
  : PLUS_INFINITY
  | MINUS_INFINITY
  ;

ObjectClassFieldValue
  : OpenTypeFieldVal
  ;

OpenTypeFieldVal
  : Type ':' Value
  ;


ReferencedValue
  : DefinedValue
  | InformationFromObjects
  ;


ParameterizedValue
  : SimpleDefinedValue  '{' '}'
  ;

SimpleDefinedValue
  : ExternalValueReference
  | IDENTIFIER
      {
  delete $1;
	  }
  ;


TypeReference
  : TYPEREFERENCE
  | WOULDBE_OBJECTCLASSREFERENCE
  ;

Reference
  : TypeReference
  | IDENTIFIER
  ;

/** End of File ****/
