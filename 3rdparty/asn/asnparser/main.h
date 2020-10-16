/*
 * main.h
 *
 * PWLib application header file for asnparser
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
 * $Log: main.h,v $
 * Revision 1.3  2002/07/02 02:03:26  mangelo
 * Remove Pwlib dependency
 *
 * Revision 1.2  2001/09/07 22:39:17  mangelo
 * add Log keyword substitution
 *
 *
 * May 3, 2001 Huang-Ming Huang
 *   Fixed the problem with my wrong interpretation to varaible constraint.
 *
 * March, 2001 Huang-Ming Huang
 *            Add support for Information Object Class and generate code that follows
 *            X/Open ASN.1/C++ interface.
 *
 */

#ifndef _MAIN_H
#define _MAIN_H

#if defined(_MSC_VER) && (_MSC_VER <=1800)
#pragma warning(disable:4786)
#endif
#include <stdio.h>
#include <vector>
#include <list>
#include <map>
#include <functional>
#include <stack>
#include <string>
#include <iostream>
#include <boost/smart_ptr.hpp>
#include <boost/cstdint.hpp>

extern unsigned lineNumber;
extern std::string  fileName;
extern FILE* yyin;
extern FILE* idin;
extern int yyparse();
extern int idparse();

void yyerror(char* str);
void iderror(char* str);

/////////////////////////////////////////
//
//  standard error output from parser
//

enum StdErrorType { Warning, Fatal };

class StdError {
public:
    StdError(StdErrorType ne) : e(ne) { }
    //StdError(StdErrorType ne, unsigned ln) : e(ne), l(ln) { }
    friend std::ostream& operator<<(std::ostream& out, const StdError& e);

protected:
    StdErrorType e;
};


/////////////////////////////////////////
//
//  intermediate structures from parser
//

typedef std::vector<std::string> StringList;

class NamedNumber { ////: public PObject
public:
    NamedNumber(std::string* nam);
    NamedNumber(std::string* nam, int num);
    NamedNumber(std::string* nam, const std::string& ref);
    friend std::ostream& operator << (std::ostream&, const NamedNumber&);

    void SetAutoNumber(const NamedNumber& prev);
    const std::string& GetName() const { return name; }
    int GetNumber() const { return number; }

protected:
    std::string name;
    std::string reference;
    int number;
    bool autonumber;
};

class Printable {
public:
    virtual void PrintOn(std::ostream&) const = 0;
};

inline std::ostream& operator << (std::ostream& os, const Printable& obj) {
    obj.PrintOn(os);
    return os;
}

//PLIST(NamedNumberList, NamedNumber);
typedef boost::shared_ptr<NamedNumber> NamedNumberPtr;
typedef std::list<NamedNumberPtr> NamedNumberList;


// Types

class TypeBase;
typedef boost::shared_ptr<TypeBase> TypePtr;
typedef std::vector<TypePtr> TypesVector;

class Tag : public Printable {
public:
    enum Type {
        Universal,
        Application,
        ContextSpecific,
        Private,
        ImplicitType
    };
    enum UniversalTags {
        IllegalUniversalTag,
        UniversalBoolean,
        UniversalInteger,
        UniversalBitString,
        UniversalOctetString,
        UniversalNull,
        UniversalObjectId,
        UniversalObjectDescriptor,
        UniversalExternalType,
        UniversalReal,
        UniversalEnumeration,
        UniversalEmbeddedPDV,
        UniversalSequence = 16,
        UniversalSet,
        UniversalNumericString,
        UniversalPrintableString,
        UniversalTeletexString,
        UniversalVideotexString,
        UniversalIA5String,
        UniversalUTCTime,
        UniversalGeneralisedTime,
        UniversalGraphicString,
        UniversalVisibleString,
        UniversalGeneralString,
        UniversalUniversalString,
        UniversalBMPString = 30
    };
    enum Mode {
        Implicit,
        Explicit,
        Automatic
    };
    Tag(unsigned tagNum, Mode m);

    void PrintOn(std::ostream&) const;
    bool operator == (const Tag& other) const {
        return type == other.type && number == other.number && mode == other.mode;
    }

    bool operator != (const Tag& other) const {
        return !(*this == other);
    }

    Type type;
    unsigned number;
    Mode mode;

    static const char* classNames[];
    static const char* modeNames[];
};

class ValueSet;
class Constraint;
class Parameter;
class SizeConstraintElement;
class FromConstraintElement;
class SubTypeConstraintElement;

typedef boost::shared_ptr<ValueSet> ValueSetPtr;
typedef boost::shared_ptr<Constraint> ConstraintPtr;

class ConstraintElementBase : public Printable {
public:
    ConstraintElementBase();
    ~ConstraintElementBase();

    void SetExclusions(boost::shared_ptr<ConstraintElementBase> excl) { exclusions = excl; }

    virtual void GenerateCplusplus(const std::string& fn, std::ostream& hdr, std::ostream& cxx, std::ostream& inl) const;
    virtual void GetConstraint(std::string&, bool digital = false) const {digital;}
    virtual bool ReferencesType(const TypeBase& type) const;

    virtual ValueSetPtr GetValueSetFromValueField(const std::string& field) const;
    virtual ValueSetPtr GetValueSetFromValueSetField(const std::string& field) const;
    virtual ConstraintPtr GetObjectSetFromObjectField(const std::string& field) const;
    virtual ConstraintPtr GetObjectSetFromObjectSetField(const std::string& field) const;

    virtual bool HasPERInvisibleConstraint(const Parameter&) const { return false;}
    virtual void GenerateObjectSetInstanceCode(const std::string&, std::ostream&) const {}
    virtual void GenerateObjSetAccessCode(std::ostream&) {}
    virtual bool GetCharacterSet(std::string& characterSet) const;
    virtual const SizeConstraintElement* GetSizeConstraint() const;
    virtual const FromConstraintElement* GetFromConstraint() const;
    virtual const SubTypeConstraintElement* GetSubTypeConstraint() const;

    virtual void PrintOn(std::ostream&) {};

protected:
    boost::shared_ptr<ConstraintElementBase> exclusions;
};

typedef boost::shared_ptr<ConstraintElementBase> ConstraintElementPtr;
typedef std::vector<ConstraintElementPtr> ConstraintElementVector;

class Constraint : public Printable {
public:
    Constraint(bool extend) : extendable(extend) {};
    Constraint(ConstraintElementPtr& elmt);
    Constraint(std::auto_ptr<ConstraintElementVector> std,
               bool extend,
               std::auto_ptr<ConstraintElementVector> ext =
                   std::auto_ptr<ConstraintElementVector>());

    Constraint(const Constraint& other);

    void PrintOn(std::ostream&) const;
    void PrintElements(std::ostream&) const;

    bool IsExtendable() const { return extendable; }
    void GetConstraint(std::string& str, bool digital = false) const;
    void GenerateCplusplus(const std::string& fn, std::ostream& hdr, std::ostream& cxx, std::ostream& inl) const;
    bool ReferencesType(const TypeBase& type) const;

    const ConstraintElementVector& GetStandardElements() const { return standard; }
    const ConstraintElementVector& GetExtensionElements() const { return extensions; }
    ConstraintElementVector& GetStandardElements() { return standard; }
    ConstraintElementVector& GetExtensionElements() { return extensions; }

    ValueSetPtr GetValueSetFromValueField(const std::string& field) const;
    ValueSetPtr GetValueSetFromValueSetField(const std::string& field) const;
    ConstraintPtr GetObjectSetFromObjectField(const std::string& field) const;
    ConstraintPtr GetObjectSetFromObjectSetField(const std::string& field) const;
    const SizeConstraintElement* GetSizeConstraint() const;
    const FromConstraintElement* GetFromConstraint() const;
    const SubTypeConstraintElement* GetSubTypeConstraint() const;

    void GetCharacterSet(std::string& characterSet) const;

    virtual std::auto_ptr<Constraint> Clone() const;
    bool HasPERInvisibleConstraint(const Parameter&) const;
    void GenerateObjectSetInstanceCode(const std::string& prefix, std::ostream& cxx) const;
    void GenerateObjSetAccessCode(std::ostream&);

protected:
    ConstraintElementVector standard;
    bool                  extendable;
    ConstraintElementVector extensions;
};

typedef boost::shared_ptr<Constraint> ConstraintPtr;
typedef std::vector<ConstraintPtr> ConstraintList;

class ConstrainAllConstraintElement : public ConstraintElementBase {
public:
    ConstrainAllConstraintElement(ConstraintElementPtr excl);
    void PrintOn(std::ostream&) const {}
};



class ElementListConstraintElement : public ConstraintElementBase {
public:
    ElementListConstraintElement();
    ElementListConstraintElement(std::auto_ptr<ConstraintElementVector> list);
    void PrintOn(std::ostream&) const;

    virtual void GenerateCplusplus(const std::string& fn, std::ostream& hdr, std::ostream& cxx, std::ostream& inl) const;
    virtual void GetConstraint(std::string& str, bool digital = false) const;
    virtual bool ReferencesType(const TypeBase& type) const;
    const ConstraintElementVector& GetElements() const { return elements; }

    virtual ValueSetPtr GetValueSetFromValueField(const std::string& field) const;
    virtual ValueSetPtr GetValueSetFromValueSetField(const std::string& field) const;
    virtual ConstraintPtr GetObjectSetFromObjectField(const std::string& field) const;
    virtual ConstraintPtr GetObjectSetFromObjectSetField(const std::string& field) const;
    virtual bool HasPERInvisibleConstraint(const Parameter&) const;
    virtual void GenerateObjectSetInstanceCode(const std::string& prefix, std::ostream& cxx) const;
    virtual void GenerateObjSetAccessCode(std::ostream&);

    virtual const SizeConstraintElement* GetSizeConstraint() const;
    virtual const FromConstraintElement* GetFromConstraint() const;

    void AppendElements(
        ConstraintElementVector::const_iterator first,
        ConstraintElementVector::const_iterator last
    );
protected:
    ConstraintElementVector elements;
};


class ValueBase;
typedef boost::shared_ptr<ValueBase> ValuePtr;

class SingleValueConstraintElement : public ConstraintElementBase {
public:
    SingleValueConstraintElement(const ValuePtr& val);
    ~SingleValueConstraintElement();
    void PrintOn(std::ostream&) const;

    virtual void GenerateCplusplus(const std::string& fn, std::ostream& hdr, std::ostream& cxx, std::ostream& inl) const;
    virtual void GetConstraint(std::string& str, bool digital = false) const;

    const ValuePtr& GetValue() const { return value; }
    virtual bool HasPERInvisibleConstraint(const Parameter&) const;
    virtual bool GetCharacterSet(std::string& characterSet) const;

protected:
    const ValuePtr value;
private:
    SingleValueConstraintElement& operator = (const SingleValueConstraintElement&);
};


class ValueRangeConstraintElement : public ConstraintElementBase {
public:
    ValueRangeConstraintElement(ValuePtr lowerBound, ValuePtr upperBound);
    ~ValueRangeConstraintElement();
    void PrintOn(std::ostream&) const;
    void GenerateRange(std::ostream& strm);
    virtual void GetConstraint(std::string& str, bool digital = false) const;

    virtual void GenerateCplusplus(const std::string& fn, std::ostream& hdr, std::ostream& cxx, std::ostream& inl) const;
    virtual bool HasPERInvisibleConstraint(const Parameter&) const;
    virtual bool GetCharacterSet(std::string& characterSet) const;
protected:
    ValuePtr lower;
    ValuePtr upper;
};



class SubTypeConstraintElement : public ConstraintElementBase {
    //PCLASSINFO(SubTypeConstraintElement, ConstraintElementBase);
public:
    SubTypeConstraintElement(TypePtr typ);
    ~SubTypeConstraintElement();
    void PrintOn(std::ostream&) const;
    void GenerateCplusplus(const std::string&, std::ostream&, std::ostream&, std::ostream&) const;
    virtual bool ReferencesType(const TypeBase& type) const;
    virtual bool HasPERInvisibleConstraint(const Parameter&) const;
    virtual void GetConstraint(std::string& str, bool digital = false) const;
    virtual const SubTypeConstraintElement* GetSubTypeConstraint() const;
    std::string GetSubTypeName() const;
    const TypePtr GetSubType() const { return subtype;}
protected:
    TypePtr subtype;
};


class NestedConstraintConstraintElement : public ConstraintElementBase {
public:
    NestedConstraintConstraintElement(ConstraintPtr con);
    ~NestedConstraintConstraintElement();

    virtual bool ReferencesType(const TypeBase& type) const;
    virtual bool HasPERInvisibleConstraint(const Parameter&) const;

protected:
    ConstraintPtr constraint;
};


class SizeConstraintElement : public NestedConstraintConstraintElement {
public:
    SizeConstraintElement(ConstraintPtr constraint);
    virtual void GetConstraint(std::string& str, bool digital = false) const;
    void PrintOn(std::ostream&) const;
    virtual void GenerateCplusplus(const std::string& fn, std::ostream& hdr, std::ostream& cxx, std::ostream& inl) const;
    virtual const SizeConstraintElement* GetSizeConstraint() const;
};


class FromConstraintElement : public NestedConstraintConstraintElement {
public:
    FromConstraintElement(ConstraintPtr constraint);
    virtual void GetConstraint(std::string& str, bool digital = false) const;
    void PrintOn(std::ostream&) const;
    virtual void GenerateCplusplus(const std::string& fn, std::ostream& hdr, std::ostream& cxx, std::ostream& inl) const;
    virtual const FromConstraintElement* GetFromConstraint() const;
    std::string GetCharacterSet(const char* canonicalSet, int canonicalSetSize) const;
    int GetRange(std::ostream& cxx) const;
};


class WithComponentConstraintElement : public NestedConstraintConstraintElement {
public:
    WithComponentConstraintElement(std::string name, ConstraintPtr constraint, int presence);
    void PrintOn(std::ostream&) const;
    virtual void GenerateCplusplus(const std::string& fn, std::ostream& hdr, std::ostream& cxx, std::ostream& inl) const;
    virtual bool HasPERInvisibleConstraint(const Parameter&) const { return false; }

    enum {
        Present,
        Absent,
        Optional,
        Default
    };

protected:
    std::string name;
    int     presence;
};


class InnerTypeConstraintElement : public ElementListConstraintElement {
public:
    InnerTypeConstraintElement(std::auto_ptr<ConstraintElementVector> list, bool partial);

    void PrintOn(std::ostream&) const;
    virtual void GenerateCplusplus(const std::string& fn, std::ostream& hdr, std::ostream& cxx, std::ostream& inl) const;

protected:
    bool partial;
};

class ActualParameter;
typedef boost::shared_ptr<ActualParameter> ActualParameterPtr;
typedef std::vector<ActualParameterPtr> ActualParameterList;
typedef boost::shared_ptr<ActualParameterList> ActualParameterListPtr;

class UserDefinedConstraintElement : public ConstraintElementBase {
public:
    UserDefinedConstraintElement(ActualParameterListPtr list);
    void PrintOn(std::ostream&) const;
    virtual void GenerateCplusplus(const std::string& fn, std::ostream& hdr, std::ostream& cxx, std::ostream& inl) const;
    virtual bool HasPERInvisibleConstraint(const Parameter&) const { return true; }
protected:
    ActualParameterListPtr parameters;
};

class DefinedObjectSet;
typedef boost::shared_ptr<DefinedObjectSet> DefinedObjectSetPtr;
class TableConstraint {
public:
    TableConstraint(boost::shared_ptr<DefinedObjectSet> objSet,
                    std::auto_ptr<StringList> atNotations = std::auto_ptr<StringList>());
    ~TableConstraint();
    bool ReferenceType(const TypeBase& type);
    std::string GetObjectSetIdentifier() const;
    const StringList* GetAtNotations() const { return atNotations.get();}
private:
    boost::shared_ptr<DefinedObjectSet> objSet;
    std::auto_ptr<StringList> atNotations;
};

/////////////////////////////////////////////

class Parameter : public Printable {
public:
    Parameter(const std::string& name);
    Parameter(const std::string& name, int type);
    Parameter(const Parameter& other);
    int GetIdentifierType();
    const std::string& GetName() const;
    void PrintOn(std::ostream& strm) const;
    virtual bool IsTypeParameter() const { return true;}
    virtual bool ReferencedBy(const TypeBase&) const ;
    virtual ActualParameterPtr MakeActualParameter() const;
protected:
    std::string name;
    int identifierType;
};

class ValueParameter : public Parameter {
public:
    ValueParameter(TypePtr governor, const std::string& nam);
    ValueParameter(const ValueParameter& other);
    ~ValueParameter();
    TypeBase* GetGovernor() { return governor.get();}
    void PrintOn(std::ostream& strm) const;
    virtual bool IsTypeParameter() const { return false;}
    virtual bool ReferencedBy(const TypeBase&) const { return false;}
    virtual ActualParameterPtr MakeActualParameter() const;
protected:
    TypePtr governor;
};

class DefinedObjectClass;
typedef boost::shared_ptr<DefinedObjectClass> DefinedObjectClassPtr;
class ObjectParameter : public Parameter {
public:
    ObjectParameter(DefinedObjectClassPtr governor,
                    const std::string& name);
    ~ObjectParameter();
    DefinedObjectClass* GetGovernor() { return governor.get();}
    void PrintOn(std::ostream& strm) const;
    virtual bool IsTypeParameter() const { return false;}
    virtual bool ReferencedBy(const TypeBase&) const ;
    virtual ActualParameterPtr MakeActualParameter() const;
protected:
    DefinedObjectClassPtr governor;
};

typedef boost::shared_ptr<Parameter> ParameterPtr;
typedef std::vector<ParameterPtr> ParameterListRep;

class ParameterList : public Printable {
public:
    void Append(ParameterPtr param);
    bool IsEmpty() const { return rep.empty();}
    int GetIdentifierType(const char* identifier);
    Parameter* GetParameter(const char* identifier);
    void GenerateCplusplus(std::string& templatePrefix, std::string& classNameString);
    void PrintOn(std::ostream& strm) const;
    boost::shared_ptr<ParameterList> GetReferencedParameters(const TypeBase& type) const;
    ActualParameterListPtr MakeActualParameters() const;
    void swap(ParameterList& other) { rep.swap(other.rep); }
    ParameterListRep rep;
};

typedef boost::shared_ptr<ParameterList> ParameterListPtr;

////////////////////////////////////////////
class ModuleDefinition;

class TypeBase : public Printable {
public:
    void PrintOn(std::ostream&) const;

    void BeginParseValue() const;
    void EndParseValue() const;
    void BeginParseValueSet() const;
    void EndParseValueSet() const;

    const std::string& GetName() const { return name; }
    void SetName(const std::string& name);
    void SetAsValueSetType() { isValueSetType = true;}
    std::string GetIdentifier() const { return identifier; }
    void SetTag(Tag::Type cls, unsigned num, Tag::Mode mode);
    const Tag& GetTag() const { return tag; }
    bool HasNonStandardTag() const { return tag != defaultTag; }
    void SetParameters(ParameterList& list);
    void AddConstraint(ConstraintPtr constraint) { constraints.push_back(constraint); }
    bool HasConstraints() const ;
    void MoveConstraints(TypeBase& from);
    void CopyConstraints(const TypeBase& from);
    virtual bool HasParameters() const { return !parameters.IsEmpty(); }
    bool IsOptional() const { return isOptional; }
    void SetOptional() { isOptional = true; }
    void SetDefaultValue(ValuePtr value);
    const std::string& GetTemplatePrefix() const { return templatePrefix; }
    const std::string& GetClassNameString() const { return classNameString; }
    void SetOuterClassName(const std::string& name) { outerClassName = name; }
    void SetTemplatePrefix(const std::string& name) { templatePrefix = name; }
    bool IsValueSetType() const { return isValueSetType;}

    virtual void AdjustIdentifier(bool);
    virtual void FlattenUsedTypes();
    virtual TypePtr FlattenThisType(TypePtr& self, const TypeBase& parent);
    virtual bool IsChoice() const;
    virtual bool IsParameterizedType() const;
    virtual bool IsPrimitiveType() const;
    virtual bool IsSequenceOfType() const { return false;}
    virtual void GenerateCplusplus(std::ostream& hdr, std::ostream& cxx, std::ostream& inl);
    virtual void GenerateForwardDecls(std::ostream& hdr);
    virtual void GenerateOperators(std::ostream& hdr, std::ostream& cxx, const TypeBase& actualType);
    virtual const char* GetAncestorClass() const = 0;
    virtual std::string GetTypeName() const;
    virtual bool CanReferenceType() const;
    virtual bool ReferencesType(const TypeBase& type) const;
    virtual const std::string& GetCModuleName() const;
    virtual bool IsParameterisedImport() const;
    virtual bool CanBeFwdDeclared(bool isComponent = false) const;
    virtual bool FwdDeclareMe(std::ostream& hdr);
    virtual void GenerateInfo(const TypeBase* type, std::ostream& hdr, std::ostream& cxx);
    virtual void GenerateCreateMethod(std::ostream& hdr, std::ostream& cxx);

    bool IsGenerated() const { return isGenerated; }
    virtual void BeginGenerateCplusplus(std::ostream& hdr, std::ostream& cxx, std::ostream& inl);
    void EndGenerateCplusplus(std::ostream& hdr, std::ostream& cxx, std::ostream& inl);
    void GenerateTags(std::ostream& strm) const;
    void GenerateCplusplusConstraints(const std::string& prefix, std::ostream& hdr, std::ostream& cxx, std::ostream& inl) const;
    virtual void GenerateStaticInitializer(std::ostream& cxx);

    const ConstraintList& GetConstraints() const { return constraints;}
    virtual void BeginParseThisTypeValue() const {}
    virtual void EndParseThisTypeValue() const {}
    virtual void ResolveReference() const {}

    virtual std::string GetPrimitiveType() const;

    virtual void RemovePERInvisibleConstraint(const ParameterPtr&);
    void RemovePERInvisibleConstraints();
    virtual bool UseType(const TypeBase&) const { return false; }

    virtual void GenerateConstructors(std::ostream& hdr, std::ostream& cxx, std::ostream& inl);
    virtual bool NeedGenInfo() const ;
    TypePtr SeqOfFlattenThisType(const TypeBase& parent, TypePtr thisPtr);

    const ParameterList& GetParameters() const { return parameters; }
    virtual void GenerateDecoder(std::ostream&) {}

    enum RemoveReuslt {
        OK,
        MAY_NOT,
        FORBIDDEN
    };
    virtual RemoveReuslt CanRemoveType(const TypeBase&) { return OK; }
    virtual bool RemoveThisType(const TypeBase& type) { return GetName() == type.GetName();}
    virtual bool IsRemovedType() const { return false;}
    ModuleDefinition* GetModule() const { return module;}
protected:
    TypeBase(unsigned tagNum, ModuleDefinition* md);
    TypeBase(TypeBase& copy);

    void PrintStart(std::ostream&) const;
    void PrintFinish(std::ostream&) const;
    const char* GetClass() const;

    Tag            tag;
    Tag            defaultTag;
    std::string        name; // The ASN.1 Type name
    std::string        identifier; // The converted C Type name
    ConstraintList constraints;
    bool           isOptional;
    ValuePtr       defaultValue;
    bool           isGenerated;
    ParameterList  parameters;
    std::string        templatePrefix;
    std::string        classNameString;
    std::string        shortClassNameString;
    std::string        outerClassName;
    bool           isValueSetType;
    ModuleDefinition* module;
};



class DefinedType : public TypeBase {
public:
    DefinedType(const std::string& name);
    DefinedType(TypePtr refType);
    DefinedType(TypePtr refType, TypePtr& bType);
    DefinedType(TypePtr refType, const std::string& name);
    DefinedType(TypePtr refType, const TypeBase& parent);

    void PrintOn(std::ostream&) const;

    virtual bool IsChoice() const;
    virtual bool IsParameterizedType() const;
    virtual void GenerateOperators(std::ostream& hdr, std::ostream& cxx, const TypeBase& actualType);
    virtual const char* GetAncestorClass() const;
    virtual std::string GetTypeName() const;
    virtual bool CanReferenceType() const;
    virtual bool ReferencesType(const TypeBase& type) const;
    virtual void GenerateCplusplus(std::ostream& hdr, std::ostream& cxx, std::ostream& inl);
    virtual bool CanBeFwdDeclared(bool isComponent = false) const ;

    virtual void BeginParseThisTypeValue() const;
    virtual void EndParseThisTypeValue() const;
    virtual void ResolveReference() const;
    virtual const std::string& GetCModuleName() const;
    virtual bool UseType(const TypeBase& type) const ;
    virtual bool NeedGenInfo() const;
    virtual RemoveReuslt CanRemoveType(const TypeBase&) ;
    virtual bool RemoveThisType(const TypeBase&);
    virtual std::string GetPrimitiveType() const;
    virtual void GenerateInfo(const TypeBase* type, std::ostream& hdr, std::ostream& cxx);
    virtual TypePtr FlattenThisType(TypePtr& self, const TypeBase& parent);

protected:
    void ConstructFromType(TypePtr& refType, const std::string& name);
    std::string referenceName;
    mutable TypePtr baseType;
    mutable bool unresolved;
};


class ParameterizedType : public DefinedType {
public:
    ParameterizedType(const std::string& name, ActualParameterList& args);
    ParameterizedType(TypePtr& refType,
                      const TypeBase& parent,
                      ActualParameterList& args);

    void PrintOn(std::ostream&) const;

    virtual bool IsParameterizedType() const;
    virtual std::string GetTypeName() const;
    virtual bool ReferencesType(const TypeBase& type) const;
    virtual bool UseType(const TypeBase& type) const ;
    virtual RemoveReuslt CanRemoveType(const TypeBase&);
protected:
    ActualParameterList arguments;
};


class SelectionType : public TypeBase {
public:
    SelectionType(const std::string& name, TypePtr base);
    ~SelectionType();

    void PrintOn(std::ostream&) const;

    virtual void FlattenUsedTypes();
    virtual TypePtr FlattenThisType(TypePtr& self, const TypeBase& parent);
    virtual void GenerateCplusplus(std::ostream& hdr, std::ostream& cxx, std::ostream& inl);
    virtual const char* GetAncestorClass() const;
    virtual bool CanReferenceType() const;
    virtual bool ReferencesType(const TypeBase& type) const;
    virtual bool UseType(const TypeBase& type) const ;

protected:
    std::string selection;
    TypePtr baseType;
};


class BooleanType : public TypeBase {
public:
    BooleanType();
    virtual void GenerateOperators(std::ostream& hdr, std::ostream& cxx, const TypeBase& actualType);
    virtual const char* GetAncestorClass() const;
    virtual std::string GetPrimitiveType() const { return "bool";}
    virtual void GenerateConstructors(std::ostream& hdr, std::ostream& cxx, std::ostream& inl);
};


class IntegerType : public TypeBase {
public:
    IntegerType();
    IntegerType(NamedNumberList&);
    virtual const char* GetAncestorClass() const;
    virtual void GenerateCplusplus(std::ostream& hdr, std::ostream& cxx, std::ostream& inl);
    std::string GetTypeName() const;
    std::string GetTypeName(bool digital) const;
    virtual std::string GetPrimitiveType() const {
        return GetIdentifier() + "::value_type::int_type";
    }

    virtual bool CanReferenceType() const;
    virtual bool ReferencesType(const TypeBase& type) const;

    virtual void GenerateStaticInitializer(std::ostream& cxx) override;
    virtual void GenerateConstructors(std::ostream& hdr, std::ostream& cxx, std::ostream& inl);
    virtual bool NeedGenInfo() const;
    virtual void GenerateInfo(const TypeBase* type, std::ostream&, std::ostream&);
    virtual TypePtr FlattenThisType(TypePtr& self, const TypeBase& parent);
protected:
    NamedNumberList allowedValues;
};


class EnumeratedType : public TypeBase {
public:
    EnumeratedType(NamedNumberList& enums, bool extend, NamedNumberList* ext);
    void PrintOn(std::ostream&) const;
    virtual TypePtr FlattenThisType(TypePtr& self, const TypeBase& parent);
    virtual void GenerateCplusplus(std::ostream& hdr, std::ostream& cxx, std::ostream& inl);
    virtual void GenerateOperators(std::ostream& hdr, std::ostream& cxx, const TypeBase& actualType);
    virtual const char* GetAncestorClass() const;
    virtual std::string GetPrimitiveType() const { return GetIdentifier() + "::value_type::NamedNumber";}
    virtual void GenerateConstructors(std::ostream& hdr, std::ostream& cxx, std::ostream& inl);
    virtual bool NeedGenInfo() const { return true;}
    bool IsPrimitiveType() const;
    virtual void GenerateInfo(const TypeBase* type, std::ostream& hdr, std::ostream& cxx);
protected:
    NamedNumberList enumerations;
    size_t numEnums;
    bool extendable;
    int maxEnumValue;
};


class RealType : public TypeBase {
public:
    RealType();
    virtual const char* GetAncestorClass() const;
    virtual std::string GetPrimitiveType() const { return "double";}
    virtual void GenerateOperators(std::ostream& hdr, std::ostream& cxx, const TypeBase& actualType);
    virtual void GenerateConstructors(std::ostream& hdr, std::ostream& cxx, std::ostream& inl);
};


class BitStringType : public TypeBase {
public:
    BitStringType();
    BitStringType(NamedNumberList&);
    virtual const char* GetAncestorClass() const;
    std::string GetTypeName() const;
    virtual std::string GetPrimitiveType() const { return TypeBase::GetPrimitiveType();}
    virtual void GenerateInfo(const TypeBase* type, std::ostream& hdr, std::ostream& cxx);
    std::string GetTypeName(bool digital) const;
    virtual void GenerateStaticInitializer(std::ostream& cxx) override;
protected:

    NamedNumberList allowedBits;
};


class OctetStringType : public TypeBase {
public:
    OctetStringType();
    virtual const char* GetAncestorClass() const;
    std::string GetTypeName() const;
    virtual std::string GetPrimitiveType() const { return "const std::vector<char>&";}
    virtual const char* GetConstrainedType() const;
    virtual void GenerateConstructors(std::ostream& hdr, std::ostream& cxx, std::ostream& inl);
    virtual void GenerateInfo(const TypeBase* type, std::ostream& hdr, std::ostream& cxx);
    std::string GetTypeName(bool digital) const;
    virtual void GenerateStaticInitializer(std::ostream& cxx) override;
};


class NullType : public TypeBase {
public:
    NullType();
    virtual const char* GetAncestorClass() const;
    virtual void BeginParseThisTypeValue() const;
    virtual void EndParseThisTypeValue() const;
};


class SequenceType : public TypeBase {
    void PrintOn(std::ostream&) const;
public:
    SequenceType(TypesVector* std,
                 bool extendable,
                 TypesVector* extensions,
                 unsigned tagNum = Tag::UniversalSequence);
    virtual void FlattenUsedTypes();
    virtual TypePtr FlattenThisType(TypePtr& self, const TypeBase& parent);
    virtual bool IsPrimitiveType() const;
    virtual void BeginGenerateCplusplus(std::ostream& hdr, std::ostream& cxx, std::ostream& inl) override;
    virtual void GenerateConstructors(std::ostream& hdr, std::ostream& cxx, std::ostream& inl) override;
    virtual void GenerateCplusplus(std::ostream& hdr, std::ostream& cxx, std::ostream& inl);
    virtual const char* GetAncestorClass() const;
    virtual bool CanReferenceType() const;
    virtual bool ReferencesType(const TypeBase& type) const;

    virtual void GenerateStaticInitializer(std::ostream& cxx) override;
    void GenerateComponent(TypeBase& field, std::ostream& hdr, std::ostream& cxx, std::ostream& inl, int id);
    virtual bool CanBeFwdDeclared(bool isComponent) const ;
    virtual void RemovePERInvisibleConstraint(const ParameterPtr&);
    virtual bool UseType(const TypeBase& type) const ;
    virtual bool NeedGenInfo() const { return true; }
    virtual void GenerateForwardDecls(std::ostream& hdr);
    virtual RemoveReuslt CanRemoveType(const TypeBase&) ;
    virtual bool RemoveThisType(const TypeBase&);
    virtual void GenerateInfo(const TypeBase* type, std::ostream& hdr, std::ostream& cxx);
protected:
    TypesVector fields;
    size_t numFields;
    mutable std::vector<bool> needFwdDeclare;
    bool extendable;
    mutable bool detectingLoop;
};


class SequenceOfType : public TypeBase {
public:
    SequenceOfType(TypePtr base, ConstraintPtr constraint = ConstraintPtr(), unsigned tag = Tag::UniversalSequence);
    ~SequenceOfType();
    void PrintOn(std::ostream&) const;
    virtual void FlattenUsedTypes();
    virtual TypePtr FlattenThisType(TypePtr& self, const TypeBase& parent);
    virtual bool IsPrimitiveType() const;
    virtual void GenerateForwardDecls(std::ostream& hdr);
    virtual const char* GetAncestorClass() const;
    virtual bool CanReferenceType() const;
    virtual void GenerateCplusplus(std::ostream& hdr, std::ostream& cxx, std::ostream& inl);
    virtual void GenerateStaticInitializer(std::ostream& cxx) override;
    virtual bool ReferencesType(const TypeBase& type) const;
    virtual std::string GetTypeName() const;
    virtual bool FwdDeclareMe(std::ostream& hdr);
    virtual bool IsSequenceOfType() const { return true;}
    virtual void RemovePERInvisibleConstraint(const ParameterPtr&);
    virtual bool UseType(const TypeBase& type) const ;
    virtual void GenerateConstructors(std::ostream& hdr, std::ostream& cxx, std::ostream& inl);
    void SetNonTypedef(bool v) { nonTypedef = v;}
    virtual RemoveReuslt CanRemoveType(const TypeBase&) ;
    virtual void GenerateInfo(const TypeBase* type, std::ostream& hdr, std::ostream& cxx);
    inline void DoNotGenerateStaticInitializer() {do_not_generate_static_initializer_ = true;}
protected:
    TypePtr baseType;
    bool nonTypedef;
private:
    bool do_not_generate_static_initializer_;
};


class SetType : public SequenceType {
public:
    SetType();
    SetType(SequenceType& seq);
    virtual const char* GetAncestorClass() const;
};


class SetOfType : public SequenceOfType {
public:
    SetOfType(TypePtr base, ConstraintPtr constraint = ConstraintPtr());
    virtual std::string GetTypeName() const;
};


class ChoiceType : public SequenceType {
public:
    ChoiceType(TypesVector* std = NULL,
               bool extendable = false,
               TypesVector* extensions = NULL);
    virtual void GenerateCplusplus(std::ostream& hdr, std::ostream& cxx, std::ostream& inl);
    virtual void GenerateConstructors(std::ostream& hdr, std::ostream& cxx, std::ostream& inl) override;
    virtual void GenerateOperators(std::ostream& hdr, std::ostream& cxx, const TypeBase& actualType);
    virtual bool IsPrimitiveType() const;
    virtual bool IsChoice() const;
    virtual const char* GetAncestorClass() const;
    void GenerateComponent(TypeBase& field, std::ostream& hdr, std::ostream& cxx, std::ostream& inl, int id);
    virtual RemoveReuslt CanRemoveType(const TypeBase&) ;
    virtual void GenerateInfo(const TypeBase* type, std::ostream& hdr, std::ostream& cxx);
    virtual void GenerateStaticInitializer(std::ostream& cxx) override;
private:
    std::vector<TypeBase*> sortedFields;
};


class EmbeddedPDVType : public TypeBase {
public:
    EmbeddedPDVType();
    virtual const char* GetAncestorClass() const;
};


class ExternalType : public TypeBase {
public:
    ExternalType();
    virtual const char* GetAncestorClass() const;
};


class AnyType : public TypeBase {
public:
    AnyType(const std::string& ident);
    void PrintOn(std::ostream& strm) const;
    virtual const char* GetAncestorClass() const;
protected:
    std::string identifier;
};


class StringTypeBase : public TypeBase {
public:
    StringTypeBase(int tag);
    virtual std::string GetTypeName() const;
    virtual std::string GetPrimitiveType() const { return "const std::string&";}
    virtual void GenerateConstructors(std::ostream& hdr, std::ostream& cxx, std::ostream& inl);
    virtual void GenerateOperators(std::ostream& hdr, std::ostream& cxx, const TypeBase& actualType);
    virtual bool NeedGenInfo() const;
    virtual void GenerateInfo(const TypeBase* type, std::ostream& hdr, std::ostream& cxx);
    virtual const char* GetCanonicalSetString() const { return NULL;};
protected:
    const char* canonicalSet;
    const char* canonicalSetRep;
    int canonicalSetSize;
};


class BMPStringType : public StringTypeBase {
public:
    BMPStringType();
    virtual const char* GetAncestorClass() const;
    virtual std::string GetPrimitiveType() const { return "const std::wstring&";}
    virtual void GenerateOperators(std::ostream& hdr, std::ostream& cxx, const TypeBase& actualType);
    virtual void GenerateConstructors(std::ostream& hdr, std::ostream& cxx, std::ostream& inl);
    virtual void GenerateInfo(const TypeBase* type, std::ostream& hdr, std::ostream& cxx);
};


class GeneralStringType : public StringTypeBase {
public:
    GeneralStringType();
    virtual const char* GetAncestorClass() const;
};


class GraphicStringType : public StringTypeBase {
public:
    GraphicStringType();
    virtual const char* GetAncestorClass() const;
};


class IA5StringType : public StringTypeBase {
public:
    IA5StringType();
    virtual const char* GetAncestorClass() const;
};


class ISO646StringType : public StringTypeBase {
public:
    ISO646StringType();
    virtual const char* GetAncestorClass() const;
};


class NumericStringType : public StringTypeBase {
public:
    NumericStringType();
    virtual const char* GetAncestorClass() const;
};


class PrintableStringType : public StringTypeBase {
public:
    PrintableStringType();
    virtual const char* GetAncestorClass() const;
};


class TeletexStringType : public StringTypeBase {
public:
    TeletexStringType();
    virtual const char* GetAncestorClass() const;
};


class T61StringType : public StringTypeBase {
public:
    T61StringType();
    virtual const char* GetAncestorClass() const;
};


class UniversalStringType : public StringTypeBase {
public:
    UniversalStringType();
    virtual const char* GetAncestorClass() const;
};


class VideotexStringType : public StringTypeBase {
public:
    VideotexStringType();
    virtual const char* GetAncestorClass() const;
};


class VisibleStringType : public StringTypeBase {
public:
    VisibleStringType();
    virtual const char* GetAncestorClass() const;
};


class UnrestrictedCharacterStringType : public StringTypeBase {
public:
    UnrestrictedCharacterStringType();
    virtual const char* GetAncestorClass() const;
};


class GeneralizedTimeType : public TypeBase {
public:
    GeneralizedTimeType();
    virtual const char* GetAncestorClass() const;
    virtual std::string GetPrimitiveType() const { return "const char*";}
    virtual void GenerateConstructors(std::ostream& hdr, std::ostream& cxx, std::ostream& inl);
};


class UTCTimeType : public TypeBase {
public:
    UTCTimeType();
    virtual const char* GetAncestorClass() const;
};


class ObjectDescriptorType : public TypeBase {
public:
    ObjectDescriptorType();
    virtual const char* GetAncestorClass() const;
};


class ObjectIdentifierType : public TypeBase {
public:
    ObjectIdentifierType();
    virtual const char* GetAncestorClass() const;
    virtual void BeginParseThisTypeValue() const;
    virtual void EndParseThisTypeValue() const;
    virtual void GenerateConstructors(std::ostream& hdr, std::ostream& cxx, std::ostream& inl);
};



class ObjectClassBase;
typedef boost::shared_ptr<ObjectClassBase> ObjectClassBasePtr;

class ObjectClassFieldType : public TypeBase {
public:
    ObjectClassFieldType(ObjectClassBasePtr  objclass, const std::string& field);
    ~ObjectClassFieldType();
    virtual const char* GetAncestorClass() const;
    void PrintOn(std::ostream&) const;
    virtual bool CanReferenceType() const;
    virtual bool ReferencesType(const TypeBase& type) const;
    TypeBase* GetFieldType() ;
    const TypeBase* GetFieldType() const ;
    virtual std::string GetTypeName() const;
    void AddTableConstraint(boost::shared_ptr<TableConstraint> constraint);
    void GenerateDecoder(std::ostream&);
    virtual void GenerateInfo(const TypeBase* type, std::ostream& hdr, std::ostream& cxx);
    std::string GetConstrainedTypeName() const;
protected:
    ObjectClassBasePtr asnObjectClass;
    std::string asnObjectClassField;
    boost::shared_ptr<TableConstraint> tableConstraint;
};


class ImportedType : public TypeBase {
public:
    ImportedType(const TypePtr& ref);
    ImportedType(const std::string& name, bool parameterised);
    virtual const char* GetAncestorClass() const;
    virtual void AdjustIdentifier(bool usingNamespace);
    virtual void GenerateCplusplus(std::ostream& hdr, std::ostream& cxx, std::ostream& inl);
    virtual bool IsParameterisedImport() const;

    virtual void BeginParseThisTypeValue() const;
    virtual void EndParseThisTypeValue() const;

    void SetModuleName(const std::string& name) ;
    virtual const std::string& GetCModuleName() const { return cModuleName; }
    const std::string& GetModuleName() const { return moduleName; }
    bool IsPrimitiveType() const;
protected:
    std::string modulePrefix;
    bool    parameterised;
    const TypePtr reference;
    std::string moduleName;
    std::string cModuleName;
private:
    ImportedType& operator = (const ImportedType&);
};


class InformationObject;
typedef boost::shared_ptr<InformationObject> InformationObjectPtr;


class TypeFromObject : public TypeBase {
public:
    TypeFromObject(InformationObjectPtr  obj, const std::string& fld);
    ~TypeFromObject();
    virtual const char* GetAncestorClass() const;
    void PrintOn(std::ostream&) const;
    virtual void GenerateCplusplus(std::ostream& hdr, std::ostream& cxx, std::ostream& inl);
protected:
    InformationObjectPtr refObj;
    std::string field;
};

class RemovedType : public TypeBase {
public:
    RemovedType(const TypeBase& type);
    virtual bool IsRemovedType() const { return true;}
    virtual const char* GetAncestorClass() const;
};

// Values

class ValueBase : public Printable {
public:
    void SetValueName(const std::string& name);
    const std::string& GetName() const { return valueName; }

    virtual void GenerateCplusplus(std::ostream& hdr, std::ostream& cxx, std::ostream& inl) const;
    virtual bool IsPERInvisibleConstraint(const Parameter&) const { return false;}

protected:
    void PrintBase(std::ostream&) const;
    std::string valueName;
};

typedef std::vector<ValuePtr> ValuesList;

class DefinedValue : public ValueBase {
    //PCLASSINFO(DefinedValue, ValueBase);
public:
    DefinedValue(const std::string& name);
    DefinedValue(const ValuePtr&);
    DefinedValue(const std::string& name, const ValuePtr&);
    void PrintOn(std::ostream&) const;
    const std::string& GetReference() const { return referenceName; }
    virtual void GenerateCplusplus(std::ostream& hdr, std::ostream& cxx, std::ostream& inl) const;
    virtual bool IsPERInvisibleConstraint(const Parameter& param) const { return param.GetName() == referenceName;}
protected:
    std::string referenceName;
    mutable ValuePtr actualValue;
    mutable bool unresolved;
};

class ImportedValue : public DefinedValue {
public:
    ImportedValue(const std::string& modName, const std::string& name, const ValuePtr& v)
        : DefinedValue(name, v), moduleName(modName) {}
    const std::string& GetModuleName() const { return moduleName; }
private:
    std::string moduleName;
};


class BooleanValue : public ValueBase {
public:
    BooleanValue(bool newVal);
    void PrintOn(std::ostream&) const;
    virtual void GenerateCplusplus(std::ostream& hdr, std::ostream& cxx, std::ostream& inl) const;
protected:
    bool value;
};


class IntegerValue : public ValueBase {
public:
    IntegerValue(boost::int64_t newVal);
    void PrintOn(std::ostream&) const;
    virtual void GenerateCplusplus(std::ostream& hdr, std::ostream& cxx, std::ostream& inl) const;

    operator boost::int64_t() const { return value; }
    operator long() const { return (long)value; }

protected:
    boost::int64_t value;
};


class RealValue : public ValueBase {
public:
    RealValue(double newVal);
    void PrintOn(std::ostream&) const {}
protected:
    double value;
};


class OctetStringValue : public ValueBase {
public:
    OctetStringValue() { }
    OctetStringValue(const std::string& newVal);
    void PrintOn(std::ostream&) const {}
protected:
    std::vector<char> value;
};


class BitStringValue : public ValueBase {
public:
    BitStringValue() { }
    BitStringValue(const std::string& newVal);
    BitStringValue(StringList* newVal);
    void PrintOn(std::ostream&) const {}
protected:
    std::vector<char> value;
};


class NullValue : public ValueBase {
public:
    void PrintOn(std::ostream&) const {}
};


class CharacterValue : public ValueBase {
public:
    CharacterValue(char c);
    CharacterValue(char t1, char t2);
    CharacterValue(char q1, char q2, char q3, char q4);
    void PrintOn(std::ostream&) const;
    virtual void GenerateCplusplus(std::ostream& hdr, std::ostream& cxx, std::ostream& inl) const;
    unsigned GetValue() const { return value;}
protected:
    unsigned value;
};


class CharacterStringValue : public ValueBase {
public:
    CharacterStringValue() { }
    CharacterStringValue(const std::string& newVal);
    CharacterStringValue(StringList& newVal);
    void PrintOn(std::ostream&) const;
    virtual void GenerateCplusplus(std::ostream& hdr, std::ostream& cxx, std::ostream& inl) const;
    void GetValue(std::string& v) const;
protected:
    std::string value;
};


class ObjectIdentifierValue : public ValueBase {
public:
    ObjectIdentifierValue(const std::string& newVal);
    ObjectIdentifierValue(StringList& newVal);
    void PrintOn(std::ostream&) const;
protected:
    StringList value;
};


class MinValue : public ValueBase {
public:
    void PrintOn(std::ostream&) const;
    virtual void GenerateCplusplus(std::ostream& hdr, std::ostream& cxx, std::ostream& inl) const;
};


class MaxValue : public ValueBase {
public:
    void PrintOn(std::ostream&) const;
    virtual void GenerateCplusplus(std::ostream& hdr, std::ostream& cxx, std::ostream& inl) const;
};


class SequenceValue : public ValueBase {
public:
    SequenceValue(ValuesList* list = NULL);
    void PrintOn(std::ostream&) const;
protected:
    ValuesList values;
};

class ChoiceValue : public ValueBase {
public:
    ChoiceValue(const TypePtr& typ, const std::string& fieldName, ValuePtr val)
        : type(typ), fieldname(fieldName), value(val) { }
    void PrintOn(std::ostream&) const;
    virtual void GenerateCplusplus(std::ostream& hdr, std::ostream& cxx, std::ostream& inl) const;
protected:
    const TypePtr type;
    std::string fieldname;
    ValuePtr value;
private:
    ChoiceValue& operator = (const ChoiceValue&);
};

// ValueSet
class ValueSet : public Printable {
public:
    virtual void Union(ValueSetPtr&) = 0;
    virtual void Intersect(ValueSetPtr&) = 0;
    virtual TypePtr MakeValueSetType() = 0;
    virtual TypePtr GetType() = 0;
    virtual ConstraintElementVector* GetElements() = 0;
    virtual void ResolveReference() const {}
};

class ValueSetDefn : public ValueSet {
public:
    ValueSetDefn();
    ValueSetDefn(TypePtr type, ConstraintPtr cons);
    ~ValueSetDefn();

    void Union(ValueSetPtr&);
    void Intersect(ValueSetPtr&);
    TypePtr MakeValueSetType();
    void PrintOn(std::ostream&) const;

    TypePtr GetType() { return type;}
    ConstraintElementVector* GetElements() { return elements.get();}
    void ResolveReference() const;
protected:
    TypePtr type;
    std::auto_ptr<ConstraintElementVector> elements;
    bool extendable;
};



class ObjectSetConstraintElement;
class ValueSetFromObject : public ValueSet {
public:
    ValueSetFromObject(InformationObjectPtr obj, const std::string& fld);
    ~ValueSetFromObject();
    void Union(ValueSetPtr&);
    void Intersect(ValueSetPtr&);
    TypePtr MakeValueSetType();
    void PrintOn(std::ostream&) const;

    TypePtr GetType() ;
    ConstraintElementVector* GetElements() ;
protected:
    ValueSetPtr GetRepresentation();

    InformationObjectPtr object;
    std::string field;
    ValueSetPtr rep;
};

typedef boost::shared_ptr<ObjectSetConstraintElement> ObjectSetConstraintElementPtr;

class ValueSetFromObjects : public ValueSet {
public:
    ValueSetFromObjects(ObjectSetConstraintElementPtr objSet,
                        const std::string& fld);
    ~ValueSetFromObjects();
    void Union(ValueSetPtr&);
    void Intersect(ValueSetPtr&);
    TypePtr MakeValueSetType();
    void PrintOn(std::ostream&) const;

    TypePtr GetType();
    ConstraintElementVector* GetElements();
protected:
    ValueSetPtr GetRepresentation();

    ObjectSetConstraintElementPtr objectSet;
    std::string field;
    ValueSetPtr rep;
};

// object class
class FieldSetting;

typedef boost::shared_ptr<FieldSetting> FieldSettingPtr;
typedef std::vector<FieldSettingPtr> FieldSettingList;

class FieldSpec;
//PLIST(FieldSpecsList, FieldSpec);
typedef boost::shared_ptr<FieldSpec> FieldSpecPtr;
typedef std::vector<FieldSpecPtr> FieldSpecsList;

class FieldSpec : public Printable {
public:
    FieldSpec(const std::string& nam, bool optional = false);
    virtual ~FieldSpec();

    bool IsOptional() const { return isOptional; }
    virtual bool HasDefault() const = 0;
    const std::string& GetName() const { return name; }
    const std::string& GetIdentifier() const { return identifier;}
    virtual std::string GetField() const = 0;

    virtual void EstablishFieldRelation(FieldSpecsList*) {}
    virtual void BeginParseSetting(FieldSettingList*) const {}
    virtual void EndParseSetting() const {}

    virtual int GetToken() const = 0;
    void PrintOn(std::ostream&) const;

    virtual void ResolveReference() const {};

    // used only for FixedTypeValueField and FixedTypeValueSetField
    virtual TypeBase* GetFieldType() { return NULL;}
    virtual const TypeBase* GetFieldType() const { return NULL; }

    //

    virtual bool GetKey(TypePtr&, std::string&) { return false;}
    virtual void FwdDeclare(std::ostream&) const {}
    virtual void Generate_info_type_constructor(std::ostream&) const {}
    virtual void Generate_info_type_memfun(std::ostream&) const {}
    virtual void Generate_info_type_mem(std::ostream&) const {}

    virtual void Generate_value_type(std::ostream&) const {}
    virtual void GenerateTypeField(const std::string& templatePrefix,
                                   const std::string& classNameString,
                                   const TypeBase* keyType,
                                   const std::string& objClassName,
                                   std::ostream& hdr, std::ostream& cxx, std::ostream& inl) const;

protected:

    std::string name;
    std::string identifier;
    bool isOptional;
};

class TypeFieldSpec : public FieldSpec {
public:
    TypeFieldSpec(const std::string& nam, bool optional = false, TypePtr defaultType = TypePtr());
    ~TypeFieldSpec();
    virtual bool HasDefault() const;
    std::string GetField() const;
    std::string GetDefault() const ;
    TypePtr GetDefaultType();
    virtual int GetToken() const;
    void PrintOn(std::ostream&) const;
    virtual void ResolveReference() const;
    virtual void Generate_info_type_constructor(std::ostream&) const;
    virtual void Generate_info_type_memfun(std::ostream& hdr) const;
    virtual void Generate_info_type_mem(std::ostream&) const;
    virtual void Generate_value_type(std::ostream& hdr) const;
    virtual void GenerateTypeField(const std::string& templatePrefix,
                                   const std::string& classNameString,
                                   const TypeBase* keyType,
                                   const std::string& objClassName,
                                   std::ostream& hdr, std::ostream& cxx, std::ostream& inl) const;
protected:
    TypePtr type;
};

class FixedTypeValueFieldSpec : public FieldSpec {
public:
    FixedTypeValueFieldSpec(const std::string& nam, TypePtr t, bool optional = false,
                            bool unique = false);
    ~FixedTypeValueFieldSpec();

    virtual bool HasDefault() const;
    void SetDefault(ValuePtr value);
    std::string GetField() const;
    virtual void BeginParseSetting(FieldSettingList*)  const ;
    virtual void EndParseSetting() const ;
    virtual int GetToken() const;

    void PrintOn(std::ostream&) const;
    virtual void ResolveReference() const;

    virtual bool GetKey(TypePtr& keyType, std::string& keyName);
    virtual TypeBase* GetFieldType() ;
    virtual const TypeBase* GetFieldType() const;

protected:
    bool isUnique;
    TypePtr  type;
    ValuePtr defaultValue;
};

class FixedTypeValueSetFieldSpec : public FieldSpec {
public:
    FixedTypeValueSetFieldSpec(const std::string& nam, TypePtr t, bool optional = false);
    ~FixedTypeValueSetFieldSpec();

    virtual bool HasDefault() const;
    void SetDefault(ValueSetPtr valueSet) { defaultValueSet = valueSet; }
    std::string GetField() const { return type->GetName();}
    virtual void BeginParseSetting(FieldSettingList*) const ;
    virtual void EndParseSetting() const ;
    virtual int GetToken() const;

    void PrintOn(std::ostream&) const;
    virtual void ResolveReference() const;

    virtual TypeBase* GetFieldType() ;
    virtual const TypeBase* GetFieldType() const ;

protected:
    TypePtr  type;
    ValueSetPtr defaultValueSet;
};

class VariableTypeValueFieldSpec : public FieldSpec {
public:
    VariableTypeValueFieldSpec(const std::string& nam,
                               const std::string& fieldname,
                               bool optional = false);
    ~VariableTypeValueFieldSpec();

    virtual bool HasDefault() const;
    std::string GetField() const { return fieldName;}
    void SetDefault(ValuePtr value) { defaultValue = value;}
    virtual void EstablishFieldRelation(FieldSpecsList* specs);

    virtual void BeginParseSetting(FieldSettingList*) const ;
    virtual void EndParseSetting() const ;
    virtual int GetToken() const;

    void PrintOn(std::ostream&) const;
    virtual void ResolveReference() const;

protected:
    std::string fieldName;
    ValuePtr defaultValue;
    TypePtr defaultType;
};

class VariableTypeValueSetFieldSpec : public FieldSpec {
public:
    VariableTypeValueSetFieldSpec(const std::string& nam,
                                  const std::string& fieldname,
                                  bool optional = false);
    ~VariableTypeValueSetFieldSpec();

    virtual bool HasDefault() const;
    std::string GetField() const { return fieldName;}
    void SetDefault(ValueSetPtr valueSet) { defaultValueSet = valueSet; }
    virtual void EstablishFieldRelation(FieldSpecsList* specs);

    virtual void BeginParseSetting(FieldSettingList*) const ;
    virtual void EndParseSetting() const ;
    virtual int GetToken() const;

    void PrintOn(std::ostream&) const;
    virtual void ResolveReference() const;

protected:
    std::string fieldName;
    ValueSetPtr defaultValueSet;
};

class DefinedObjectClass;
typedef boost::shared_ptr<DefinedObjectClass> DefinedObjectClassPtr;
class ObjectFieldSpec : public FieldSpec {
public:
    ObjectFieldSpec(const std::string& nam,
                    DefinedObjectClass* oclass,
                    bool optional = false);
    ~ObjectFieldSpec() ;

    virtual bool HasDefault() const;
    std::string GetField() const;
    void SetDefault(InformationObjectPtr dftObj);
    virtual void BeginParseSetting(FieldSettingList*) const ;
    virtual void EndParseSetting() const ;
    virtual int GetToken() const;

    void PrintOn(std::ostream&) const;
    virtual void ResolveReference() const;
protected:
    DefinedObjectClass* objectClass;
    InformationObjectPtr obj;
};

class ObjectSetFieldSpec : public FieldSpec {
public:
    ObjectSetFieldSpec(const std::string& nam,
                       DefinedObjectClassPtr oclass,
                       bool optional = false);
    ~ObjectSetFieldSpec();

    virtual bool HasDefault() const;
    void SetDefault(ConstraintPtr dftObjSet) { objSet = dftObjSet;}
    std::string GetField() const;

    virtual void BeginParseSetting(FieldSettingList*) const ;
    virtual void EndParseSetting() const ;
    virtual int GetToken() const;

    void PrintOn(std::ostream&) const;
    virtual void ResolveReference() const;
    virtual void FwdDeclare(std::ostream& hdr) const ;
    virtual void Generate_info_type_constructor(std::ostream&) const;
    virtual void Generate_info_type_memfun(std::ostream& hdr) const ;
    virtual void Generate_info_type_mem(std::ostream&) const;
    virtual void Generate_value_type(std::ostream& hdr) const ;
    virtual void GenerateTypeField(const std::string& templatePrefix,
                                   const std::string& classNameString,
                                   const TypeBase* keyType,
                                   const std::string& objClassName,
                                   std::ostream& hdr,
                                   std::ostream& cxx,
                                   std::ostream& inl) const;
protected:
    DefinedObjectClassPtr objectClass;
    ConstraintPtr objSet;
};


class DefinedSyntaxToken;

class TokenOrGroupSpec : public Printable {
public:
    virtual ~TokenOrGroupSpec() {};
    virtual bool ValidateField(FieldSpecsList*) = 0;
    virtual bool HasLiteral(const std::string& str) const = 0;
    enum MakeDefaultSyntaxResult {
        FAIL, // indicate the make process fail
        CONSUMED_AND_EXHAUSTED,
        CONSUMED_AND_NOT_EXHAUSTED,
        NOT_CONSUMED
    };
    virtual MakeDefaultSyntaxResult  MakeDefaultSyntax(DefinedSyntaxToken* token, FieldSettingList* setting) = 0;
    virtual void PreMakeDefaultSyntax(FieldSettingList* settings) = 0;
    virtual void CancelMakeDefaultSyntax() const = 0;
    virtual void Reset() {}
};

class Literal : public TokenOrGroupSpec {
public:
    Literal(const std::string& str)
        : name(str) { }
    Literal(const char* str): name(str) {}
    virtual ~Literal() {};
    void PrintOn(std::ostream& strm) const ;
    virtual bool ValidateField(FieldSpecsList*) { return true; }
    virtual MakeDefaultSyntaxResult  MakeDefaultSyntax(DefinedSyntaxToken* token, FieldSettingList* setting);
    virtual bool HasLiteral(const std::string& str) const { return str == name; }
    virtual void PreMakeDefaultSyntax(FieldSettingList*) {};
    virtual void CancelMakeDefaultSyntax() const {}

protected:
    std::string name;
};

class PrimitiveFieldName : public TokenOrGroupSpec {
public:
    PrimitiveFieldName(const std::string& fieldname)
        : name(fieldname)
    {}
    PrimitiveFieldName(const char* fieldname) : name(fieldname) {}
    PrimitiveFieldName(const PrimitiveFieldName& other) { name = other.name; field = other.field;}
    ~PrimitiveFieldName() {};
    void PrintOn(std::ostream&) const ;
    virtual bool ValidateField(FieldSpecsList* fields);
    virtual MakeDefaultSyntaxResult  MakeDefaultSyntax(DefinedSyntaxToken* token, FieldSettingList* setting) ;
    virtual bool HasLiteral(const std::string&) const { return false; }
    virtual void PreMakeDefaultSyntax(FieldSettingList* settings);
    virtual void CancelMakeDefaultSyntax() const;
private:
    std::string name;
    FieldSpec* field;
};

typedef boost::shared_ptr<TokenOrGroupSpec> TokenOrGroupSpecPtr;
typedef std::vector<TokenOrGroupSpecPtr> TokenOrGroupSpecList;

class TokenGroup : public TokenOrGroupSpec {
public:
    TokenGroup() : optional(false), cursor(0) {}
    TokenGroup(const TokenGroup& other);
    ~TokenGroup() {}
    void AddToken(TokenOrGroupSpecPtr token) { tokenOrGroupSpecList.push_back(token);}
    void SetOptional() { optional = true;}
    void PrintOn(std::ostream&) const;
    bool ValidateField(FieldSpecsList* fields);
    MakeDefaultSyntaxResult  MakeDefaultSyntax(DefinedSyntaxToken* token,
                                               FieldSettingList* setting);
    virtual bool HasLiteral(const std::string& str) const;
    virtual void PreMakeDefaultSyntax(FieldSettingList* settings);
    virtual void CancelMakeDefaultSyntax() const;

    size_t size() const { return tokenOrGroupSpecList.size(); }
    TokenOrGroupSpec& operator[](size_t i) { return *(tokenOrGroupSpecList[i]);}
    virtual void Reset();
private:
    TokenOrGroupSpecList tokenOrGroupSpecList;
    bool optional;
    size_t cursor;
};

typedef boost::shared_ptr<TokenGroup> TokenGroupPtr;

class DefaultSyntaxBuilder {
public:
    DefaultSyntaxBuilder(TokenGroupPtr tkGrp);
    ~DefaultSyntaxBuilder();
    void AddToken(DefinedSyntaxToken* token);
    std::auto_ptr<FieldSettingList> GetDefaultSyntax();
    void ResetTokenGroup();
private:
    TokenGroupPtr tokenGroup;
    std::auto_ptr<FieldSettingList> setting;
};

class ObjectClassBase : public Printable {
public:
    ObjectClassBase() {};
    ObjectClassBase(const std::string& nam) { SetName(nam); }
    virtual ~ObjectClassBase() {};

    void SetName(const std::string& nam);
    virtual const std::string& GetName() const { return name;}
    int GetFieldToken(const char* fieldname) const;
    virtual FieldSpec* GetField(const std::string& fieldName) = 0;
    virtual const FieldSpec* GetField(const std::string& fieldName) const = 0;
    virtual TypeBase* GetFieldType(const std::string& fieldName) { return GetField(fieldName)->GetFieldType(); }
    virtual const TypeBase* GetFieldType(const std::string& fieldName) const { return GetField(fieldName)->GetFieldType(); }
    virtual bool VerifyDefaultSyntax(FieldSettingList*) const = 0;
    virtual bool HasLiteral(const std::string& str) const = 0;
    virtual TokenGroupPtr GetWithSyntax() const = 0;
    virtual void PreParseObject() const = 0;
    virtual void BeginParseObject() const = 0;
    virtual void EndParseObject() const = 0 ;
    virtual void BeginParseObjectSet() const = 0;
    virtual void EndParseObjectSet() const = 0;
    virtual void ResolveReference() const = 0;
    virtual void GenerateCplusplus(std::ostream&, std::ostream&, std::ostream&) {}
    virtual const std::string& GetKeyName() const = 0;
protected:
    std::string name;
};

typedef std::vector<ObjectClassBasePtr> ObjectClassesList;
class ObjectClassDefn : public ObjectClassBase {
public:
    ObjectClassDefn();

    ~ObjectClassDefn();

    void SetFieldSpecs(std::auto_ptr<FieldSpecsList> list);
    void SetWithSyntaxSpec(TokenGroupPtr list);

    FieldSpec* GetField(const std::string& fieldName);
    const FieldSpec* GetField(const std::string& fieldName) const;

    virtual bool VerifyDefaultSyntax(FieldSettingList*) const;
    virtual bool HasLiteral(const std::string& str) const { return withSyntaxSpec->HasLiteral(str);}
    virtual TokenGroupPtr GetWithSyntax() const;
    virtual void PreParseObject() const ;
    virtual void BeginParseObject() const;
    virtual void EndParseObject() const;
    virtual void BeginParseObjectSet() const;
    virtual void EndParseObjectSet() const;
    void PrintOn(std::ostream&) const;
    virtual void ResolveReference() const;

    void ResolveKey();
    const std::string& GetKeyName() const { return keyName; }
    void GenerateCplusplus(std::ostream& hdr, std::ostream& cxx, std::ostream& inl);
protected:
    std::auto_ptr<FieldSpecsList> fieldSpecs;
    TokenGroupPtr withSyntaxSpec;
    TypePtr keyType;
    std::string keyName;
};


class DefinedObjectClass : public ObjectClassBase {
public:
    DefinedObjectClass(ObjectClassBase* ref);
    DefinedObjectClass(const std::string& nam, ObjectClassBase* ref = NULL);
    ~DefinedObjectClass() {}

    const std::string& GetName() const { return referenceName;}
    ObjectClassBase* GetReference();
    const ObjectClassBase* GetReference() const;
    FieldSpec* GetField(const std::string& fieldName);
    const FieldSpec* GetField(const std::string& fieldName) const;
    virtual bool VerifyDefaultSyntax(FieldSettingList*) const;
    bool HasLiteral(const std::string& str) const;
    virtual TokenGroupPtr GetWithSyntax() const;
    virtual void PreParseObject() const ;
    virtual void BeginParseObject() const;
    virtual void EndParseObject() const;
    virtual void BeginParseObjectSet() const;
    virtual void EndParseObjectSet() const;
    void PrintOn(std::ostream& strm) const;
    virtual void ResolveReference() const;
    virtual TypeBase* GetFieldType(const std::string& fieldName) ;
    virtual const TypeBase* GetFieldType(const std::string& fieldName) const;
    const std::string& GetKeyName() const { return reference->GetKeyName(); }
protected:
    std::string referenceName;
    mutable ObjectClassBase* reference;
};

class ImportedObjectClass : public DefinedObjectClass {
public:
    ImportedObjectClass(const std::string& modName,
                        const std::string& nam,
                        ObjectClassBase* ref)
        : DefinedObjectClass(nam, ref), moduleName(modName) {}
    const std::string& GetModuleName() const { return moduleName;}
private:
    std::string moduleName;
};

class Setting : public Printable {
public:

    enum {
        has_type_setting = 0x01,
        has_value_setting = 0x02,
        has_valueSet_setting = 0x04,
        has_object_setting = 0x08,
        has_objectSet_setting = 0x10
    };
    virtual ~Setting() {};
    virtual void GenerateInfo(const std::string&, std::ostream&) {}
    virtual void GenerateCplusplus(const std::string&, const std::string&, std::ostream&, std::ostream&, std::ostream&, unsigned& flag) = 0;
    virtual void GenerateInitializationList(std::ostream&, std::ostream&, std::ostream&) {}
    virtual bool IsExtendable() const { return false;}
    virtual void GenerateInstanceCode(const std::string&, std::ostream&) const {}
};

class TypeSetting : public Setting {
public:
    TypeSetting(TypePtr typeBase) { type = typeBase; }
    ~TypeSetting() {}

    TypePtr GetType() { return type;}
    const TypeBase* GetType() const { return type.get();}
    void PrintOn(std::ostream& strm) const;
    virtual void GenerateCplusplus(const std::string& prefix, const std::string& name, std::ostream& hdr, std::ostream& cxx, std::ostream& inl, unsigned& flag);
    virtual void GenerateInfo(const std::string& name, std::ostream& hdr);
protected:
    TypePtr type;
};


class ValueSetting : public Setting {
public:
    ValueSetting(TypePtr typeBase, ValuePtr valueBase);
    ~ValueSetting();

    ValuePtr GetValue() { return value;}
    const ValuePtr GetValue() const { return value;}
    TypePtr GetType() { return type;}
    const TypeBase* GetType() const { return type.get();}
    void PrintOn(std::ostream& strm) const;
    virtual void GenerateCplusplus(const std::string& prefix, const std::string& name, std::ostream& hdr, std::ostream& cxx, std::ostream& inl, unsigned& flag);
    virtual void GenerateInitializationList(std::ostream&, std::ostream&, std::ostream&);
protected:
    TypePtr  type;
    ValuePtr value;
};

class ValueSetSetting : public Setting {
public:
    ValueSetSetting(ValueSetPtr set);
    ~ValueSetSetting();

    ValueSetPtr GetValueSet() { return valueSet; }
    const ValueSetPtr GetValueSet() const { return valueSet; }
    void PrintOn(std::ostream& strm) const;
    virtual void GenerateCplusplus(const std::string& prefix, const std::string& name, std::ostream& hdr, std::ostream& cxx, std::ostream& inl, unsigned& flag);
protected:
    ValueSetPtr valueSet;
};

class InformationObject;
typedef boost::shared_ptr<InformationObject> InformationObjectPtr;
class ObjectSetting : public Setting {
public:
    ObjectSetting(InformationObjectPtr obj, ObjectClassBase* objClass);
    ~ObjectSetting();
    InformationObjectPtr GetObject() { return object; }
    const InformationObject* GetObject() const { return object.get(); }
    void PrintOn(std::ostream& strm) const ;
    virtual void GenerateCplusplus(const std::string& prefix, const std::string& name, std::ostream& hdr, std::ostream& cxx, std::ostream& inl, unsigned& flag);
protected:
    ObjectClassBase* objectClass;
    InformationObjectPtr object;
};

class ObjectSetSetting : public Setting {
public:
    ObjectSetSetting(ConstraintPtr objSet, ObjectClassBase* objClass)
        : objectClass(objClass), objectSet(objSet) { }
    ~ObjectSetSetting();
    ConstraintPtr GetObjectSet() { return objectSet;}
    const ConstraintPtr GetObjectSet() const { return objectSet; }
    void PrintOn(std::ostream&) const ;
    virtual void GenerateCplusplus(const std::string& prefix, const std::string& name, std::ostream& hdr, std::ostream& cxx, std::ostream& inl, unsigned& flag);
    virtual void GenerateInfo(const std::string& name, std::ostream& hdr);
    virtual bool IsExtendable() const { return objectSet->IsExtendable(); }
    void GenerateInstanceCode(const std::string& prefix, std::ostream& cxx) const;
protected:
    ObjectClassBase* objectClass;
    ConstraintPtr objectSet;
};

class FieldSetting : public Printable {
public:
    FieldSetting(const std::string& fieldname, std::auto_ptr<Setting> aSetting);
    ~FieldSetting();

    const std::string& GetName() const { return name;}
    Setting* GetSetting() { return setting.get();}
    const Setting* GetSetting() const { return setting.get(); }
    void PrintOn(std::ostream&) const;

    bool IsExtendable() const;
    void GenerateCplusplus(const std::string& prefix, std::ostream& hdr, std::ostream& cxx, std::ostream& inl, unsigned& flag);
    void GenerateInitializationList(std::ostream& hdr, std::ostream& cxx, std::ostream& inl);
    void GenerateInfo(std::ostream& hdr);
    void GenerateInstanceCode(const std::string& prefix, std::ostream& cxx) const;
protected:
    std::string name;
    std::string identifier;
    std::auto_ptr<Setting> setting;
};


class InformationObject : public Printable {
public:
    InformationObject() : parameters(NULL) {}
    virtual ~InformationObject();
    void SetName(const std::string& str) { name = str; }
    const std::string& GetName() const { return name; }
    const std::string& GetClassName() const;
    void PrintBase(std::ostream&) const;
    virtual bool SetObjectClass(const ObjectClassBase* definedClass) = 0;
    virtual const ObjectClassBase* GetObjectClass() const = 0;
    virtual const Setting* GetSetting(const std::string& fieldname) const = 0;
    void SetParameters(std::auto_ptr<ParameterList> list);
    virtual void GenerateCplusplus(std::ostream&, std::ostream&, std::ostream&) {}
    virtual bool IsExtendable() const = 0;
    virtual void GenerateInstanceCode(std::ostream& cxx) const = 0;
protected:
    virtual bool VerifyObjectDefinition() = 0 ;
    std::string name;
    std::auto_ptr<ParameterList> parameters;
};

typedef std::vector<InformationObjectPtr> InformationObjectList;

class DefinedObject : public InformationObject {
public:
    DefinedObject(const std::string& name, const InformationObject* ref = NULL);
    bool VerifyObjectDefinition();
    const InformationObject* GetReference() const ;
    bool SetObjectClass(const ObjectClassBase*) { return true;}
    const ObjectClassBase* GetObjectClass() const ;
    void PrintOn(std::ostream&) const;
    virtual const Setting* GetSetting(const std::string& fieldname) const;
    virtual bool IsExtendable() const { return reference->IsExtendable(); }
    virtual void GenerateInstanceCode(std::ostream& cxx) const;
protected:
    std::string referenceName;
    mutable const InformationObject* reference;
};

class ImportedObject : public DefinedObject {
public:
    ImportedObject(const std::string& modName, const std::string& name, const InformationObject* ref)
        : DefinedObject(name, ref), moduleName(modName) {}
    const std::string& GetModuleName() const { return moduleName; }
    virtual void GenerateInstanceCode(std::ostream& cxx) const;
private:
    std::string moduleName;
};

class DefaultObjectDefn : public InformationObject {
public:
    DefaultObjectDefn(std::auto_ptr<FieldSettingList> list) : settings(list) {}
    ~DefaultObjectDefn() { }
    bool VerifyObjectDefinition();

    bool SetObjectClass(const ObjectClassBase* definedClass);
    const ObjectClassBase* GetObjectClass() const { return referenceClass;}

    void PrintOn(std::ostream&) const;
    virtual const Setting* GetSetting(const std::string& fieldname) const;
    virtual void GenerateCplusplus(std::ostream& hdr , std::ostream& cxx, std::ostream& inl);
    virtual bool IsExtendable() const ;
    virtual void GenerateInstanceCode(std::ostream& cxx) const;
protected:
    std::auto_ptr<FieldSettingList> settings;
    const ObjectClassBase* referenceClass;
};

class ObjectFromObject : public InformationObject {
public:
    ObjectFromObject(InformationObjectPtr referenceObj, const std::string& fld);
    ~ObjectFromObject();
    void PrintOn(std::ostream&) const;
    virtual bool SetObjectClass(const ObjectClassBase*) {return true;}
    virtual const ObjectClassBase* GetObjectClass() const ;
    virtual const Setting* GetSetting(const std::string& fieldname) const;
    virtual bool IsExtendable() const { return refObj->IsExtendable(); }
    virtual void GenerateInstanceCode(std::ostream& cxx) const;
protected:
    virtual bool VerifyObjectDefinition();
    InformationObjectPtr refObj;
    std::string field;
};

class DefinedSyntaxToken {
public:
    virtual ~DefinedSyntaxToken() {};
    virtual bool MatchLiteral(const std::string&) { return false;}
    virtual FieldSettingPtr MatchSetting(const std::string&) { return FieldSettingPtr();}
    virtual bool IsEndSyntaxToken() { return false;}
};

class LiteralToken : public DefinedSyntaxToken {
public:
    LiteralToken(const std::string& tokenName) : name(tokenName) { }
    virtual bool MatchLiteral(const std::string& literal) { return literal == name;}
protected:
    std::string name;
};

class SettingToken : public DefinedSyntaxToken {
public:
    SettingToken(std::auto_ptr<Setting> set) : setting(set) {}
    ~SettingToken() { }
    virtual FieldSettingPtr MatchSetting(const std::string&) ;
protected:
    std::auto_ptr<Setting> setting;
};

class EndSyntaxToken : public DefinedSyntaxToken {
public:
    EndSyntaxToken() {}
    virtual bool IsEndSyntaxToken() { return true;}
};



class InformationObjectSet : public Printable {
public:
    virtual ~InformationObjectSet() {}

    virtual const std::string& GetName() const = 0 ;
    virtual const ObjectClassBase* GetObjectClass() const = 0 ;
    virtual bool IsExtendable() const = 0;

    virtual ValueSetPtr GetValueSetFromValueField(const std::string& field) const = 0;
    virtual ValueSetPtr GetValueSetFromValueSetField(const std::string& field) const = 0;
    virtual ConstraintPtr GetObjectSetFromObjectField(const std::string& field) const = 0;
    virtual ConstraintPtr GetObjectSetFromObjectSetField(const std::string& field) const = 0;
    virtual bool HasParameters() const = 0;
    virtual void GenerateInstanceCode(std::ostream& cxx) const = 0;
    virtual void GenerateType(std::ostream& hdr, std::ostream& inl, std::ostream& cxx) const = 0;
};

class InformationObjectSetDefn : public InformationObjectSet {
public:
    InformationObjectSetDefn(const std::string& nam,
                             ObjectClassBasePtr objClass,
                             ConstraintPtr set,
                             ParameterListPtr list = ParameterListPtr());
    ~InformationObjectSetDefn();

    const std::string& GetName() const { return name;}
    const ObjectClassBase* GetObjectClass() const ;

    virtual bool IsExtendable() const { return rep->IsExtendable(); }
    ValueSetPtr GetValueSetFromValueField(const std::string& field) const;
    ValueSetPtr GetValueSetFromValueSetField(const std::string& field) const;
    ConstraintPtr GetObjectSetFromObjectField(const std::string& field) const;
    ConstraintPtr GetObjectSetFromObjectSetField(const std::string& field) const;
    bool HasParameters() const { return parameters.get() != NULL; }
    void GenerateInstanceCode(std::ostream& cxx) const;
    void PrintOn(std::ostream&) const;
    virtual void GenerateType(std::ostream& hdr, std::ostream& inl, std::ostream& cxx) const;
    bool GenerateTypeConstructor(std::ostream& cxx) const;
protected:
    std::string name;
    ObjectClassBasePtr objectClass;
    ConstraintPtr rep;
    ParameterListPtr parameters;
};

class ImportedObjectSet :  public InformationObjectSet {
public:
    ImportedObjectSet(const std::string& modName, const InformationObjectSet* objSet) : reference(objSet), moduleName(modName) {}
    virtual const std::string& GetName() const { return reference->GetName(); }
    virtual const ObjectClassBase* GetObjectClass() const { return reference->GetObjectClass(); }
    const std::string& GetModuleName() const { return moduleName; }

    virtual bool IsExtendable() const { return reference->IsExtendable(); }
    virtual ValueSetPtr GetValueSetFromValueField(const std::string& field) const { return reference->GetValueSetFromValueField(field); }
    virtual ValueSetPtr GetValueSetFromValueSetField(const std::string& field) const { return reference->GetValueSetFromValueSetField(field); }
    virtual ConstraintPtr GetObjectSetFromObjectField(const std::string& field) const { return reference->GetObjectSetFromObjectField(field); }
    virtual ConstraintPtr GetObjectSetFromObjectSetField(const std::string& field) const { return reference->GetObjectSetFromObjectSetField(field); }
    virtual bool HasParameters() const { return reference->HasParameters(); }
    void GenerateInstanceCode(std::ostream&) const {}
    virtual void GenerateType(std::ostream&, std::ostream&, std::ostream&) const {}
    void PrintOn(std::ostream&) const {}
private:
    const InformationObjectSet* reference;
    std::string moduleName;
};

typedef boost::shared_ptr<InformationObjectSet> InformationObjectSetPtr;
typedef std::vector<InformationObjectSetPtr> InformationObjectSetList;
class ObjectSetConstraintElement : public ConstraintElementBase {
public:
    virtual const ObjectClassBase* GetObjectClass() const = 0 ;
    virtual std::string GetName() const = 0;
    virtual void GenerateObjSetAccessCode(std::ostream&) {}
};

class DefinedObjectSet : public ObjectSetConstraintElement {
public:
    DefinedObjectSet(const std::string& ref) ;

    virtual const ObjectClassBase* GetObjectClass() const ;
    virtual std::string GetName() const { return referenceName;}

    virtual ValueSetPtr GetValueSetFromValueField(const std::string& field) const;
    virtual ValueSetPtr GetValueSetFromValueSetField(const std::string& field) const;
    virtual ConstraintPtr GetObjectSetFromObjectField(const std::string& field) const;
    virtual ConstraintPtr GetObjectSetFromObjectSetField(const std::string& field) const;
    virtual bool HasPERInvisibleConstraint(const Parameter& param) const;

    virtual const InformationObjectSet* GetReference() const;

    void PrintOn(std::ostream&) const;
protected:
    std::string referenceName;
    mutable const InformationObjectSet* reference;
};

class ParameterizedObjectSet : public DefinedObjectSet {
public:
    ParameterizedObjectSet(const std::string& ref, ActualParameterListPtr args) ;
    ~ParameterizedObjectSet();
    virtual std::string GetName() const ;

    void PrintOn(std::ostream&) const;
protected:
    ActualParameterListPtr arguments;
};


class ObjectSetFromObject : public ObjectSetConstraintElement {
public:
    ObjectSetFromObject(InformationObjectPtr obj, const std::string& fld);
    ~ObjectSetFromObject();

    virtual const ObjectClassBase* GetObjectClass() const ;
    virtual std::string GetName() const;

    virtual ValueSetPtr GetValueSetFromValueField(const std::string& field) const;
    virtual ValueSetPtr GetValueSetFromValueSetField(const std::string& field) const;
    virtual ConstraintPtr GetObjectSetFromObjectField(const std::string& field) const;
    virtual ConstraintPtr GetObjectSetFromObjectSetField(const std::string& field) const;
    virtual bool HasPERInvisibleConstraint(const Parameter& param) const;

    void PrintOn(std::ostream&) const;
protected:
    Constraint* GetRepresentation() const;
    InformationObjectPtr refObj;
    std::string field;
    mutable ConstraintPtr rep;
};

class ObjectSetFromObjects : public ObjectSetConstraintElement {
public:
    ObjectSetFromObjects(ObjectSetConstraintElementPtr objSet, const std::string& fld);
    ~ObjectSetFromObjects();

    virtual const ObjectClassBase* GetObjectClass() const ;
    virtual std::string GetName() const;

    virtual ValueSetPtr GetValueSetFromValueField(const std::string& field) const;
    virtual ValueSetPtr GetValueSetFromValueSetField(const std::string& field) const;
    virtual ConstraintPtr GetObjectSetFromObjectField(const std::string& field) const;
    virtual ConstraintPtr GetObjectSetFromObjectSetField(const std::string& field) const;

    virtual bool HasPERInvisibleConstraint(const Parameter& param) const;
    void PrintOn(std::ostream&) const;
    virtual void GenerateObjSetAccessCode(std::ostream&);
protected:
    ConstraintPtr GetRepresentation() const;
    ObjectSetConstraintElementPtr refObjSet;
    std::string field;
    mutable ConstraintPtr rep;
};

class SingleObjectConstraintElement : public ConstraintElementBase {
public:
    SingleObjectConstraintElement(InformationObjectPtr obj);
    ~SingleObjectConstraintElement();

    virtual ValueSetPtr GetValueSetFromValueField(const std::string& field) const;
    virtual ValueSetPtr GetValueSetFromValueSetField(const std::string& field) const;
    virtual ConstraintPtr GetObjectSetFromObjectField(const std::string& field) const;
    virtual ConstraintPtr GetObjectSetFromObjectSetField(const std::string& field) const;
    virtual bool HasPERInvisibleConstraint(const Parameter& param) const;
    virtual void GenerateObjectSetInstanceCode(const std::string& prefix, std::ostream& cxx) const;
    void PrintOn(std::ostream&) const;
protected:
    InformationObjectPtr object;
};

class ObjectSetType : public TypeBase {
public:
    ObjectSetType(InformationObjectSetPtr objSet);
    virtual const char* GetAncestorClass() const;
    virtual void GenerateCplusplus(std::ostream& hdr, std::ostream& cxx, std::ostream& inl);
    virtual bool HasParameters() const;
private:
    InformationObjectSetPtr objSet;
};

// ImportedSymbol
class ModuleDefinition;
class Symbol : public Printable {
public:
    Symbol(const std::string& sym, bool param);
    bool IsParameterisedImport() const { return parameterized;}
    virtual void AppendToModule(ModuleDefinition* from, ModuleDefinition* to) = 0;
    virtual void AppendToModule(const std::string&, ModuleDefinition*) {}
    virtual void GenerateUsingDirective(const std::string&, std::ostream&) const {}
    virtual bool IsType() const { return false; }
    virtual bool IsValuesOrObjects() const { return false;}
    const std::string& GetName() const { return name; }

    void PrintOn(std::ostream& strm) const;
protected:
    std::string name;
    bool parameterized;
};

class TypeReference : public Symbol {
public:
    TypeReference(const std::string& sym, bool param) : Symbol(sym, param) {}
    virtual void AppendToModule(ModuleDefinition* from, ModuleDefinition* to);
    virtual void AppendToModule(const std::string& fromName, ModuleDefinition* to);
    virtual void GenerateUsingDirective(const std::string& moduleName, std::ostream& strm) const;
    virtual bool IsType() const { return true; }
};

class ValueReference : public Symbol {
public:
    ValueReference(const std::string& sym, bool param) : Symbol(sym, param) {}
    virtual bool IsValuesOrObjects() const { return true;}
    virtual void AppendToModule(ModuleDefinition* from, ModuleDefinition* to);
};

class ObjectClassReference : public Symbol {
public:
    ObjectClassReference(const std::string& sym, bool param) : Symbol(sym, param) {}
    virtual void GenerateUsingDirective(const std::string& moduleName, std::ostream& strm) const ;
    virtual void AppendToModule(ModuleDefinition* from, ModuleDefinition* to);
};

class ObjectReference : public Symbol {
public:
    ObjectReference(const std::string& sym, bool param) : Symbol(sym, param) {}
    virtual void AppendToModule(ModuleDefinition* from, ModuleDefinition* to);
    virtual bool IsValuesOrObjects() const { return true;}
};

class ObjectSetReference : public Symbol {
public:
    ObjectSetReference(const std::string& sym, bool param) : Symbol(sym, param) {}
    virtual void AppendToModule(ModuleDefinition* from, ModuleDefinition* to);
    virtual bool IsValuesOrObjects() const { return true;}
};
// ActualParameter

class ActualParameter : public Printable {
public:
    virtual bool ReferencesType(const TypeBase&) const { return false;}
    virtual bool UseType(const TypeBase&) const { return false;}
    virtual bool GenerateTemplateArgument(std::string&) const { return false;}
};

class ActualTypeParameter : public ActualParameter {
public:
    ActualTypeParameter(TypePtr type);
    ~ActualTypeParameter();

    bool ReferencesType(const TypeBase& type) const;
    bool UseType(const TypeBase&) const;
    bool GenerateTemplateArgument(std::string& name) const;
    void PrintOn(std::ostream& strm) const;
protected:
    TypePtr param;
};

class ActualValueParameter : public ActualParameter {
public:
    ActualValueParameter(ValuePtr value);
    ~ActualValueParameter();

    void PrintOn(std::ostream& strm) const;
protected:
    ValuePtr param;
};

class ActualValueSetParameter : public ActualParameter {
public:
    ActualValueSetParameter(TypePtr valueSetType);
    ~ActualValueSetParameter();

    bool ReferencesType(const TypeBase& type) const;
    bool UseType(const TypeBase&) const;
    bool GenerateTemplateArgument(std::string&) const;

    void PrintOn(std::ostream& strm) const;
protected:
    TypePtr param;
};

class ActualObjectParameter : public ActualParameter {
public:
    ActualObjectParameter(InformationObjectPtr obj);
    ~ActualObjectParameter();
    bool GenerateTemplateArgument(std::string& name) const;
    virtual bool UseType(const TypeBase&) const ;
    virtual bool ReferencesType(const TypeBase& type) const;

    void PrintOn(std::ostream& strm) const;
protected:
    InformationObjectPtr param;
};

class ActualObjectSetParameter : public ActualParameter {
public:
    ActualObjectSetParameter(boost::shared_ptr<ObjectSetConstraintElement> objectSet);
    ~ActualObjectSetParameter();
    bool GenerateTemplateArgument(std::string& name) const;
    virtual bool UseType(const TypeBase&) const ;
    virtual bool ReferencesType(const TypeBase& type) const;

    void PrintOn(std::ostream& strm) const;
    virtual bool IsTemplateArgument() const { return true;}
protected:
    boost::shared_ptr<ObjectSetConstraintElement> param;
};



typedef boost::shared_ptr<Symbol> SymbolPtr;
typedef std::vector<SymbolPtr> SymbolList;

class ImportModule : public Printable {
    //PCLASSINFO(ImportModule, PObject);
public:
    ImportModule(std::string* name, SymbolList* syms);
    void SetFileName(const std::string& name) { filename = name; }
    const std::string& GetFileName() const { return filename;}

    void PrintOn(std::ostream&) const;

    void GenerateCplusplus(std::ostream& hdr, std::ostream& cxx, std::ostream& inl);
    void GenerateUsingDirectives(std::ostream& strm) const;
    void Adjust();

    const std::string& GetName() const { return fullModuleName; }
    bool HasValuesOrObjects() const;
    std::string GetCModuleName() const;
    const std::string& GetLowerCaseName() const { return filename; }

protected:
    std::string   fullModuleName;
    std::string   shortModuleName;
    SymbolList symbols;
    std::string   filename;
};

typedef boost::shared_ptr<ImportModule> ImportModulePtr;
typedef std::vector<ImportModulePtr> ImportsList;
typedef boost::shared_ptr<ModuleDefinition> ModuleDefinitionPtr;
typedef std::vector<ModuleDefinitionPtr> ModuleList;


typedef std::map<std::string, TypePtr> TypeMap;

class ModuleDefinition : public Printable {
public:
    ModuleDefinition(const std::string& name, Tag::Mode defTagMode);
    ~ModuleDefinition();

    void PrintOn(std::ostream&) const;

    Tag::Mode GetDefaultTagMode() const { return defaultTagMode; }

    void AddIdentifier(std::string* name, int idType);
    void AddImportedIdentifiers(StringList& imports, const std::string& moduleName);
    int  GetIdentifierType(const std::string& id);

    void SetDefinitiveObjId(StringList& id);
    void SetExportAll();
    void SetExports(SymbolList& syms);

    void AddImport(ImportModulePtr mod)  { imports.push_back(mod); }
    void AddType(TypePtr type)       ;
    void AddValue(ValuePtr val)      { values.push_back(val); }
    void AddObjectClass(ObjectClassBasePtr oclass) { objectClasses.push_back(oclass); }
    void AddInformationObject(InformationObjectPtr obj) { informationObjects.push_back(obj); }
    void AddInformationObjectSet(InformationObjectSetPtr set) { informationObjectSets.push_back(set); }

    TypePtr FindType(const std::string& name);
    bool  HasType(const std::string& name);
    const ValuesList& GetValues() const { return values; }

    ValuePtr FindValue(const std::string& name);
    ObjectClassBasePtr FindObjectClass(const std::string& name);
    const InformationObject* FindInformationObject(const std::string& name);
    const InformationObjectSet* FindInformationObjectSet(const std::string& name);

    const std::string& GetName() const { return moduleName; }
    const std::string& GetCModuleName() const { return cModuleName; }
    const std::string& GetPrefix()     const { return classNamePrefix; }

    std::string GetImportModuleName(const std::string& moduleName);

    int GetIndentLevel() const { return indentLevel; }
    void SetIndentLevel(int delta) { indentLevel += delta; }

    bool UsingInlines() const { return usingInlines; }

    void GenerateCplusplus(const std::string& modName,
                           unsigned numFiles,
                           //bool useNamespaces,
                           //bool useInlines,
                           bool verbose);

    void ResolveObjectClassReferences() const;

    void AdjustModuleName(const std::string& sourcePath, bool isSubModule = false);
    bool ReorderTypes();
    std::string CreateSubModules(SymbolList& exportedSymbols);
    std::string GetFileName();
    void AdjustImportedModules();

    void GenerateClassModule(std::ostream& hdr, std::ostream& cxx, std::ostream& inl);
    void CreateObjectSetTypes();

    void AddToRemoveList(const std::string& reference);
    void RemoveReferences(bool verbose);
    ImportModule* FindImportedModule(const std::string& theModuleName);

private:
    ModuleDefinition& operator = (const ModuleDefinition&);
    typedef std::map<std::string, std::string> PStringMap;
    const std::string            moduleName;
    std::string                  classNamePrefix;
    bool                     separateClassFiles;
    StringList              definitiveId;
    Tag::Mode                defaultTagMode;
    SymbolList               exports;
    bool                     exportAll;
    ImportsList              imports;
    PStringMap               importNames;
    TypesVector              types;
    TypeMap                  typeMap;
    ValuesList               values;
//    MibList         mibs;
    int                      indentLevel;
    bool                     usingInlines;
    ObjectClassesList        objectClasses;
    InformationObjectList    informationObjects;
    InformationObjectSetList informationObjectSets;
    std::map<std::string, int>    identifiers;
    std::string                  shortModuleName;
    std::string                  cModuleName;
    std::string              path;
    ModuleList               subModules;
    std::vector<std::string>     removeList;
};

template <class T>
boost::shared_ptr<T> FindWithName(const std::vector<boost::shared_ptr<T> >& cont, const std::string& name) {
    typedef std::vector<boost::shared_ptr<T> > Cont;

    typename Cont::const_iterator itr = cont.begin(), last = cont.end();
    for (; itr != last; ++itr)
        if ((*itr)->GetName() == name)
            return *itr;
    return boost::shared_ptr<T>();

}

extern ModuleDefinition* Module;
extern ModuleList Modules;

typedef std::stack<ObjectClassBase*> ClassStack;
extern ClassStack* classStack;
extern ParameterList* DummyParameters;

ModuleDefinition* FindModule(const char* name);
void AddRemoveItem(const char* item);
#endif
