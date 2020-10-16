/*
 * asn1.h
 *
 * Abstract Syntax Notation 1 Encoding Rules
 *
 * Copyright (c) 1993-1998 Equivalence Pty. Ltd.
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
 * The Original Code is Portable Windows Library.
 *
 * The Initial Developer of the Original Code is Equivalence Pty. Ltd.
 *
 * Portions are Copyright (C) 1993 Free Software Foundation, Inc.
 * All Rights Reserved.
 *
 * Contributor(s): Huang-Ming Huang
 *
 * The code is adapted from asner.h of PWLib, but the dependancy on PWLib has
 * been removed.
 *
 * $Log: asn1.h,v $
 * Revision 1.15  2002/08/20 22:35:54  mangelo
 * Add MSVC DLL support
 *
 * Revision 1.14  2002/07/18 06:53:33  btrummer
 * Added range checks for PEREncoder's encodeLength(), encodeUnsigned()
 * and encodeConstrainedLength(). Encoding an object which violates its
 * length constraint will fail now.
 *
 * Revision 1.13  2002/07/17 11:17:51  btrummer
 * Check for the macro __IBMCPP__ rather than _AIX for the last AiX-fix.
 *
 * Revision 1.12  2002/07/11 08:34:08  btrummer
 * Added AiX-fix for the initialization of Constrained_INTEGER::theInfo.
 *
 * Revision 1.11  2002/07/11 07:51:07  btrummer
 * Added some InfoType typedefs needed for AiX.
 *
 * Revision 1.10  2002/07/11 06:22:19  btrummer
 * Fixed the constructor
 * OBJECT_IDENTIFIER(InputIterator, InputIterator, const void*),
 * which raised an internal compiler error on Windows.
 *
 * Revision 1.9  2002/07/04 03:47:22  mangelo
 * added explicit wstring typedef for GCC 2.95.x
 *
 * Revision 1.8  2002/07/03 06:15:26  btrummer
 * Added a range-check for PERDecoder::rollback(), so beginPosition will
 * never be set outside of endPosition.
 *
 * Revision 1.7  2002/07/02 04:45:13  mangelo
 * Modify for VC.Net and GCC 3.1
 *
 * Revision 1.6  2001/11/13 16:40:12  mangelo
 * Fixed the missing retrun in OCTET_STRING::operator=(const char*)
 *
 * Revision 1.5  2001/10/25 02:47:56  mangelo
 * fixed the wrong declaration of SEQUNCE_OF::InfoType.type
 *
 * Revision 1.4  2001/10/05 18:51:18  mangelo
 * Fixed wrong declarations of AbstractString.TpyeInfo.type and BMPString.TypeInfo.type. Added tagMode() to SEQUENCE
 *
 * Revision 1.3  2001/08/29 15:47:34  mangelo
 * Fixed the declaration of SizeConstraint class template
 *
 * Revision 1.2  2001/08/03 07:43:10  mangelo
 * Fixed isValid() bugs
 *
 *
 * 2001/07/26 Huang-Ming Huang
 * Fixed INTEGER::isStrictValid() when the lowerLimit() >=0 and value > INT_MAX.
 *
 * 2001/07/26 Huang-Ming Huang
 * Added custom memory manager using boost/pool.
 *
 * 2001/07/16 Huang-Ming Huang
 * Optional components of SEQUENCE is now created on demand.
 *
 * 2001/06/26 Huang-Ming Huang
 * Version 2.1 Reimplemented to minimize the code size.
 *
 */
#ifndef _ASN1_H
#define _ASN1_H

#if defined(__GNUC__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Wignored-qualifiers"
#pragma GCC diagnostic ignored "-fpermissive"
#endif

#if defined(_MSC_VER) && (_MSC_VER <= 1800)
#pragma warning(disable: 4786 4100 4189 4018 4701)
#endif

#if defined(_MSC_VER) && !defined(NDEBUG) && !defined(_CPPRTTI)
#error Please Use /GR compiler option for debug version. In Visual Stdio, check the Enable RTTI box under C++ Language of C/C++ Tab in Project/Settings
#endif

#ifdef ASN1_ALLOCATOR
#include <boost/pool/singleton_pool.hpp>
#endif

#ifdef min
#undef minCPDL
#endif

#ifdef max
#undef max
#endif

#include <assert.h>
#include <string>
#include <memory>
#include <time.h>
#include <sstream>
#include <iomanip>
#include <memory>
#include <boost/iterator.hpp>
#include "AssocVector.h"


#if !defined(ASN1_STATIC) && defined(_MSC_VER)
#define ASN1_EXPORT __declspec(dllimport)
#else
#define ASN1_EXPORT
#endif


#ifdef ASN1_HAS_IOSTREAM
#include <sstream>
#define AVN_ONLY(x) x
#else
#define AVN_ONLY(x)
#endif

#ifdef NEED_WSTRING_TYPEDEF
namespace std {
typedef basic_string<wchar_t> wstring;
}
#endif

#include <stdexcept>

#include <QVariant>
#include <QString>

/////////////////////////////////////////////////////////////////////////////

namespace ASN1 {

class Visitor;
class ConstVistor;

enum ConstraintType {
    Unconstrained,
    PartiallyConstrained,
    FixedConstraint,
    ExtendableConstraint
};

class Visitor;
class ConstVisitor;
class AbstractData;

namespace detail {

template <unsigned v>
struct int_to_type
{};


template <class T>
struct Allocator {
#ifdef ASN1_ALLOCATOR
    static void* operator new(std::size_t sz) {
        assert(sizeof(T) == sz);
        return boost::singleton_pool<int_to_type<sizeof(T)>, sizeof(T)>::malloc();
    }
    static void  operator delete(void* p) {
        boost::singleton_pool<int_to_type<sizeof(T)> , sizeof(T)>::free(p);
    }
#endif
};

}


class ASN1BadVariantCast : public std::runtime_error {
public:
    ASN1BadVariantCast(const QString& what_arg) : std::runtime_error(what_arg.toStdString()) {}
};

class ASN1ValueIsOutOfRange : public std::runtime_error {
public:
    ASN1ValueIsOutOfRange(const QString& what_arg) : std::runtime_error(what_arg.toStdString()) {}
};


/**
 * Base class for ASN.1 types.
 *
 * The ASN1::AbstractData class provides an abstract base class from which all ASN.1
 * builtin and user-defined type classes are derived. This class offers polymorphic
 * functionality including copying/cloning, comparison, validity check, and output.
 * The class is abstract and can only be instantiated in derived forms.
 *
 * The ASN1::AbstractData class also plays the role \a Element in \a Visitor Design Pattern, described
 * by Gamma, et al. It has the accept() operations which dispatch the request according to their exact
 * subtype (ConcreteElement). In this implementation, there are two kinds of \a Visitors, \c Visitor
 * and \c ConstVistor. See those two classes for more detail explanation.
 *
 * @warning For all the AbstractData subclass, the assignment operator do not involve the exchange of
 *  constraints and tags. That is to say, the type been assigned will
 *  retain the same constraints and tags as it hasn't been assigned.
 *  However all the copy constructors do copy the  constraints and tags from the object been copied.
 *  It is the programmer's responsibility to prevent using copy constructor on different types.
 *
 */
class AbstractData {
public:
    virtual ~AbstractData() {};

    /**
     *  Allocate a copy of this object on the freestore.
     *
     *  The object allocated is of
     *  the same actual subclass as the object on which this method is invoked, and
     *  has a copy of all of its values. The allocated object is owned by the caller,
     *  who is responsible for deleting it.
     */
    AbstractData* clone() const { return do_clone(); }

    /**
     * Verify that the object conforms to any type constraints on it.
     *
     * These constraints
     * consist of those inherent to the ASN.1 type (such as NumericString), and any subtype
     * constraints. The isValid() function accepts a value as an acceptable extended value.
     * That is, if the ASN.1 type is defined as being extensible (using the ... notation),
     * then a value that could fit within such an extension is considered valid.
     */
    bool isValid() const ;

    /**
     * Verify that the object conforms to any type constraints on it,
     * without allowing for any extended values.
     *
     * For types that do not have
     * extensibility defined, isStrictlyValid() is equivalent to isValid().
     * For constructed types, isStrictlyValid() returns true when isStrictlyValid()
     * is true for all the components.
     */
    bool isStrictlyValid() const ;

    /**
     * Compare two AbstractData Objects.
     *
     * @return The return value is 0 if this object equals to \c other;
     *  the return value > 0 if this object is greater than \c other.
     *
     * This implementation allows only the comparison between compatible types, e.g.
     * IA5String and VisibleString. The behavior of comparing between incompatible
     * types is undefined.
     */
    int compare(const AbstractData& other) const { return do_compare(other); }

    /*
     * Most operators are defined as member functions rather than friend functions because
     * GCC 2.95.x have problem to resolve template operator functions when  any non-template
     * operator function exists.
     */
    bool operator == (const AbstractData& rhs) const { return compare(rhs) == 0; }
    bool operator != (const AbstractData& rhs) const { return compare(rhs) != 0; }
    bool operator < (const AbstractData& rhs) const { return compare(rhs) <  0; }
    bool operator > (const AbstractData& rhs) const { return compare(rhs) >  0; }
    bool operator <= (const AbstractData& rhs) const { return compare(rhs) <= 0; }
    bool operator >= (const AbstractData& rhs) const { return compare(rhs) >= 0; }

    /**
     * Returns the ASN.1 Tag.
     *
     * An ASN.1 tag has two parts, tag class and tag number.
     * The return value of getTag() is (tagClass << 16 | tagNumber).
     */
    unsigned getTag() const { return info()->tag; }

    /**
     * Accept a \c Visitor which implement a fragment of algorithm or request for
     * each corresponding class of ASN.1 object structure.
     *
     * @param v The \c Visitor.
     * @return true if the operation has been successfully executed.
     */
    bool accept(Visitor& v) { return do_accept(v) ;}
    /**
     * Accept a \c Visitor which implement a fragment of algorithm or request for
     * each corresponding class of ASN.1 object structure.
     *
     * @param v The \c ConstVisitor.
     * @return true if the operation has been successfully executed.
     */
    bool accept(ConstVisitor& v) const { return do_accept(v); }

    /**
     * Create a AbstractData object based on the \c info structure.
     *
     * This is a factory method for creating an instance of ASN1::AbstractData
     * object. Each concrete ASN1::AbstractData subclass contains a structure which describe
     * the information of the type, constraints, and how it can be instantiate. Using this information,
     * an object can be created by the static member function.
     *
     * @return NULL if the object creation is fail; otherwise a new object is successfully
     *  created and should be deleted by its caller.
     */
    inline static AbstractData* create(const void* info)
    { return info == NULL ? NULL : static_cast<const InfoType*>(info)->create(info); }

    virtual QVariant toVariant() const = 0;
    virtual void fromVariant(const QVariant& variant) = 0;

private:
    virtual int do_compare(const AbstractData& other) const = 0;
    virtual AbstractData* do_clone() const = 0;
    virtual bool do_accept(Visitor&) = 0;
    virtual bool do_accept(ConstVisitor&) const = 0;

protected:
    AbstractData(const void* info);

    typedef AbstractData* (*CreateFun)(const void*);

    struct InfoType {
        CreateFun create;
        unsigned tag; /* the tag is represented using the formula
                           (tagClass << 16 | tagNumber) */
    };
    const void* info_;

public:
    static const std::string& typeName() { static std::string tn = "AbstractData"; return tn; }
    const InfoType* info() const { return static_cast<const InfoType*>(info_);}

#ifdef ASN1_HAS_IOSTREAM
public:
    friend std::ostream& operator<<(
        std::ostream& strm,       // Stream to print the object into.
        const AbstractData& obj   // Object to print to the stream.
    );

    friend std::istream& operator >>(
        std::istream& strm,       // Stream which stores the object into.
        AbstractData& obj         // Object to retrive from the stream.
    );

    std::ios_base::iostate get_from(std::istream&);
    std::ios_base::iostate print_on(std::ostream&) const ;

    /**
     *  Set the AbstractData object from a string containing ASN.1 value notation.
     *
     *  This supports full ASN.1 except for the following limitations:
     *  \li Value references are not allowed. That is, a value can not contain the names of other values.    *  Note that values are commonly used to provide the high order part of an Object Identifier.
     *  \li Open type is not currently supported.
     */
    bool setFromValueNotation(const std::string& valueString);
    /**
     *  Format the AbstractData object using normal ASN.1 value notation into the string.
     */
    std::string asValueNotation() const;
#endif
};




/** Base class for constrained ASN encoding/decoding.
*/
class ConstrainedObject : public AbstractData {
public:
    ConstrainedObject(const void* info) : AbstractData(info) {}
    bool extendable() const { return info()->type == ExtendableConstraint; }
    bool constrained() const { return info()->type != Unconstrained; }
    unsigned getConstraintType() const { return info()->type; }
    int getLowerLimit() const { return info()->lowerLimit; }
    unsigned getUpperLimit() const { return info()->upperLimit; }

protected:
    struct InfoType {
        CreateFun create;
        unsigned tag;
        unsigned type;
        int lowerLimit;
        unsigned upperLimit;
    };
    const InfoType* info() const  { return static_cast<const InfoType*>(info_);}
};

/** Class for ASN Null type.
*/
class Null : public AbstractData , public detail::Allocator<Null> {
    Null(const void* info) : AbstractData(info) {}
public:
    Null() : AbstractData(&theInfo) {}

    bool isValid() const { return true;}
    bool isStrictlyValid() const { return true;}

    Null* clone() const { return static_cast<Null*>(do_clone()); }
    static AbstractData* create(const void* info);
    void swap(Null& that) { /* do nothing */}

    bool operator == (const Null&) const { return true;  }
    bool operator != (const Null&) const { return false; }
    bool operator < (const Null&) const { return false; }
    bool operator > (const Null&) const { return false; }
    bool operator <= (const Null&) const { return false; }
    bool operator >= (const Null&) const { return false; }
    ASN1_EXPORT static const InfoType theInfo;
    static bool equal_type(const ASN1::AbstractData& type)
    {return type.info() == reinterpret_cast<const ASN1::AbstractData::InfoType*>(&theInfo);}

    virtual QVariant toVariant() const override;
    virtual void fromVariant(const QVariant& variant) override;

    static const std::string& typeName() { static std::string tn = "Null"; return tn; }

private:
    virtual int do_compare(const AbstractData& other) const;
    virtual AbstractData* do_clone() const ;
    virtual bool do_accept(Visitor&);
    virtual bool do_accept(ConstVisitor&) const;
};


/** Class for ASN Boolean type.
*/
class BOOLEAN : public AbstractData, public detail::Allocator<BOOLEAN> {
protected:
    BOOLEAN(const void* info);
public:
    typedef bool value_type;
    typedef bool& reference;
    typedef bool const_reference;
    BOOLEAN() : AbstractData(&theInfo), value(false) {}
    BOOLEAN(bool b , const void* info = &theInfo);

    BOOLEAN(const BOOLEAN& that);
    BOOLEAN& operator = (const BOOLEAN& that) { value = that.value; return *this; }
    BOOLEAN& operator = (bool b) { value = b; return *this; }
    operator bool() const { return value; }

    bool isValid() const { return true;}
    bool isStrictlyValid() const { return true;}

    BOOLEAN* clone() const { return static_cast<BOOLEAN*>(do_clone()); }
    static AbstractData* create(const void*);
    void swap(BOOLEAN& that) { std::swap(value, that.value); }

    bool operator == (const BOOLEAN& rhs) const { return value == rhs.value ; }
    bool operator != (const BOOLEAN& rhs) const { return value != rhs.value ; }
    bool operator < (const BOOLEAN& rhs) const { return value <  rhs.value ; }
    bool operator > (const BOOLEAN& rhs) const { return value >  rhs.value ; }
    bool operator <= (const BOOLEAN& rhs) const { return value <= rhs.value ; }
    bool operator >= (const BOOLEAN& rhs) const { return value >= rhs.value ; }

#if __GNUC__> 2 || __GNUC_MINOR__ > 95
    friend bool operator == (bool lhs, const BOOLEAN& rhs) { return lhs == rhs.value ; }
    friend bool operator != (bool lhs, const BOOLEAN& rhs) { return lhs != rhs.value ; }
    friend bool operator < (bool lhs, const BOOLEAN& rhs) { return lhs <  rhs.value ; }
    friend bool operator > (bool lhs, const BOOLEAN& rhs) { return lhs >  rhs.value ; }
    friend bool operator <= (bool lhs, const BOOLEAN& rhs) { return lhs <= rhs.value ; }
    friend bool operator >= (bool lhs, const BOOLEAN& rhs) { return lhs >= rhs.value ; }
#endif

    bool operator == (bool rhs) const { return value == rhs ; }
    bool operator != (bool rhs) const { return value != rhs ; }
    bool operator < (bool rhs) const { return value <  rhs ; }
    bool operator > (bool rhs) const { return value >  rhs ; }
    bool operator <= (bool rhs) const { return value <= rhs ; }
    bool operator >= (bool rhs) const { return value >= rhs ; }
    ASN1_EXPORT static const InfoType theInfo;
    static bool equal_type(const ASN1::AbstractData& type)
    {return type.info() == reinterpret_cast<const ASN1::AbstractData::InfoType*>(&theInfo);}

    virtual QVariant toVariant() const override;
    virtual void fromVariant(const QVariant& variant) override;

    static const std::string& typeName() { static std::string tn = "BOOLEAN"; return tn; }

private:
    virtual int do_compare(const AbstractData& other) const;
    virtual AbstractData* do_clone() const ;
    virtual bool do_accept(Visitor&);
    virtual bool do_accept(ConstVisitor&) const;
    bool value;
};

/** Class for ASN Integer type.
*/
class INTEGER : public ConstrainedObject, public detail::Allocator<INTEGER> {
protected:
    INTEGER(const void* info);
public:
    typedef int int_type;
    typedef int value_type;
    typedef int_type& reference;
    typedef int_type const_reference;

    INTEGER() : ConstrainedObject(&theInfo), value(0) {}
    INTEGER(int_type val, const void* info = &theInfo);
    INTEGER(const INTEGER& other);

    INTEGER& operator = (const INTEGER& val) { setValue(val.value) ; return *this;}
    INTEGER& operator = (int_type val) { setValue(val); return *this;}
    int_type getValue() const { return static_cast<int_type>(value);}

    bool isValid() const { return isStrictValid() || getConstraintType() == ExtendableConstraint;  }
    bool isStrictValid() const;

    INTEGER* clone() const { return static_cast<INTEGER*>(do_clone()); }
    static AbstractData* create(const void*);
    void swap(INTEGER& that) { std::swap(value, that.value); }

    bool operator == (const INTEGER& rhs) const { return getValue() == rhs.getValue(); }
    bool operator != (const INTEGER& rhs) const { return getValue() != rhs.getValue(); }
    bool operator < (const INTEGER& rhs) const { return getValue() <  rhs.getValue(); }
    bool operator > (const INTEGER& rhs) const { return getValue() >  rhs.getValue(); }
    bool operator <= (const INTEGER& rhs) const { return getValue() <= rhs.getValue(); }
    bool operator >= (const INTEGER& rhs) const { return getValue() >= rhs.getValue(); }

#if __GNUC__> 2 || __GNUC_MINOR__ > 95
    friend bool operator == (int_type lhs, const INTEGER& rhs) { return lhs == rhs.getValue(); }
    friend bool operator != (int_type lhs, const INTEGER& rhs) { return lhs != rhs.getValue(); }
    friend bool operator < (int_type lhs, const INTEGER& rhs) { return lhs <  rhs.getValue(); }
    friend bool operator > (int_type lhs, const INTEGER& rhs) { return lhs >  rhs.getValue(); }
    friend bool operator <= (int_type lhs, const INTEGER& rhs) { return lhs <= rhs.getValue(); }
    friend bool operator >= (int_type lhs, const INTEGER& rhs) { return lhs >= rhs.getValue(); }
#endif

    bool operator == (int_type rhs) const { return getValue() == rhs; }
    bool operator != (int_type rhs) const { return getValue() != rhs; }
    bool operator < (int_type rhs) const { return getValue() <  rhs; }
    bool operator > (int_type rhs) const { return getValue() >  rhs; }
    bool operator <= (int_type rhs) const { return getValue() <= rhs; }
    bool operator >= (int_type rhs) const { return getValue() >= rhs; }

    INTEGER& operator += (int_type val) { value += val; return *this; }
    INTEGER& operator -= (int_type val) { value -= val; return *this; }
    INTEGER& operator *= (int_type val) { value *= val; return *this; }
    INTEGER& operator /= (int_type val) { value /= val; return *this; }
    INTEGER& operator %= (int_type val) { value %= val; return *this; }

    INTEGER& operator += (const INTEGER& val) { value += val.getValue(); return *this; }
    INTEGER& operator -= (const INTEGER& val) { value -= val.getValue(); return *this; }
    INTEGER& operator *= (const INTEGER& val) { value *= val.getValue(); return *this; }
    INTEGER& operator /= (const INTEGER& val) { value /= val.getValue(); return *this; }
    INTEGER& operator %= (const INTEGER& val) { value %= val.getValue(); return *this; }

    INTEGER& operator ++ () { ++ value  ; return *this;}
    INTEGER operator ++ (int) { INTEGER result(*this); ++(*this); return result;}
    INTEGER& operator -- () { -- value  ; return *this;}
    INTEGER operator -- (int) { INTEGER result(*this); --(*this); return result;}

    int_type operator + (const INTEGER& rhs) const { int_type t(getValue()); return t += rhs.getValue();}
    int_type operator - (const INTEGER& rhs) const { int_type t(getValue()); return t -= rhs.getValue();}
    int_type operator * (const INTEGER& rhs) const { int_type t(getValue()); return t *= rhs.getValue();}
    int_type operator / (const INTEGER& rhs) const { int_type t(getValue()); return t /= rhs.getValue();}

    friend int_type operator + (int_type lhs, const INTEGER& rhs) { int_type t(lhs); return t += rhs.getValue();}
    friend int_type operator - (int_type lhs, const INTEGER& rhs) { int_type t(lhs); return t -= rhs.getValue();}
    friend int_type operator * (int_type lhs, const INTEGER& rhs) { int_type t(lhs); return t *= rhs.getValue();}
    friend int_type operator / (int_type lhs, const INTEGER& rhs) { int_type t(lhs); return t /= rhs.getValue();}

    int_type operator + (int_type rhs) { int_type t(getValue()); return t += rhs;}
    int_type operator - (int_type rhs) { int_type t(getValue()); return t -= rhs;}
    int_type operator * (int_type rhs) { int_type t(getValue()); return t *= rhs;}
    int_type operator / (int_type rhs) { int_type t(getValue()); return t /= rhs;}

    ASN1_EXPORT static const InfoType theInfo;
    static bool equal_type(const ASN1::AbstractData& type)
    {return type.info() == reinterpret_cast<const ASN1::AbstractData::InfoType*>(&theInfo);}

    virtual QVariant toVariant() const override;
    virtual void fromVariant(const QVariant& variant) override;

    static const std::string& typeName() { static std::string tn = "INTEGER"; return tn; }

protected:
    void setValue(int_type val) { value = static_cast<unsigned>(val); }
    unsigned value;
private:
    virtual int do_compare(const AbstractData& other) const;
    virtual AbstractData* do_clone() const ;
    virtual bool do_accept(Visitor&);
    virtual bool do_accept(ConstVisitor&) const;

};



class IntegerWithNamedNumber : public INTEGER {
protected:
    IntegerWithNamedNumber(const void* info);
    IntegerWithNamedNumber(int val, const void* info);
    static AbstractData* create(const void* info);

    struct NameEntry {
        int value;
        const char* name;
    };

    struct NameEntryCmp {
        bool operator()(const NameEntry& lhs, const NameEntry& rhs) const
        { return lhs.value < rhs.value; }
        bool operator()(int lhs, const NameEntry& rhs) const
        { return lhs < rhs.value; }
        bool operator()(const NameEntry& lhs, int rhs) const
        { return lhs.value < rhs; }
    };

    struct InfoType {
        CreateFun create;
        unsigned tag;
        unsigned type;
        int lowerLimit;
        unsigned upperLimit;
        AVN_ONLY(const NameEntry* nameEntries;)
        AVN_ONLY(unsigned entryCount;)
    };
    const InfoType* info() const { return static_cast<const InfoType*>(info_); }
private:
    virtual bool do_accept(Visitor&);
    virtual bool do_accept(ConstVisitor&) const;
#ifdef ASN1_HAS_IOSTREAM
public:
    bool getName(std::string&) const;
    bool setFromName(const std::string&);
#endif
};

namespace detail {
template <int i>
struct is_negtive {
    enum { yes = (i < 0) };
};

template <bool isNegtive>
struct select_integer_type;


template <>
struct select_integer_type<false> {
    typedef unsigned value_type;
};

template <>
struct select_integer_type<true> {
    typedef int value_type;
};
}

template <ConstraintType contraint, int lower, unsigned upper >
class Constrained_INTEGER : public INTEGER {
    typedef Constrained_INTEGER<contraint, lower, upper> ThisType;
    typedef INTEGER Inherited;
protected:
    typedef Inherited::InfoType InfoType;
public:
    enum {
        UpperLimit = upper,
        LowerLimit = lower
    };
    // select value_type base on lower bound, if lower is negtive then value_type is int, otherwise value_type is unsigned.
    typedef typename detail::select_integer_type<detail::is_negtive<lower>::yes >::value_type value_type;
    typedef value_type& reference;
    typedef value_type const_reference;
    typedef value_type int_type;

    Constrained_INTEGER(value_type val = 0)
        : INTEGER(&theInfo)
    {   setValue(val);    }

    // Constrained_INTEGER(const ThisType& other) ; //use default copy constructor
    ThisType& operator = (const INTEGER& val) { value = val.value; assert_borders(isValid()); return *this; }
    ThisType& operator = (int_type val) { value = val; assert_borders(isValid()); return *this; }
    int_type getValue() const { return (value_type) value; }

    bool operator == (const ThisType& rhs) const { return getValue() == rhs.getValue(); }
    bool operator != (const ThisType& rhs) const { return getValue() != rhs.getValue(); }
    bool operator < (const ThisType& rhs) const { return getValue() <  rhs.getValue(); }
    bool operator > (const ThisType& rhs) const { return getValue() >  rhs.getValue(); }
    bool operator <= (const ThisType& rhs) const { return getValue() <= rhs.getValue(); }
    bool operator >= (const ThisType& rhs) const { return getValue() >= rhs.getValue(); }

#if __GNUC__> 2 || __GNUC_MINOR__ > 95
    friend bool operator == (int_type lhs, const ThisType& rhs) { return lhs == rhs.getValue(); }
    friend bool operator != (int_type lhs, const ThisType& rhs) { return lhs != rhs.getValue(); }
    friend bool operator < (int_type lhs, const ThisType& rhs) { return lhs <  rhs.getValue(); }
    friend bool operator > (int_type lhs, const ThisType& rhs) { return lhs >  rhs.getValue(); }
    friend bool operator <= (int_type lhs, const ThisType& rhs) { return lhs <= rhs.getValue(); }
    friend bool operator >= (int_type lhs, const ThisType& rhs) { return lhs >= rhs.getValue(); }
#endif

    bool operator == (int_type rhs) const { return getValue() == rhs; }
    bool operator != (int_type rhs) const { return getValue() != rhs; }
    bool operator < (int_type rhs) const { return getValue() <  rhs; }
    bool operator > (int_type rhs) const { return getValue() >  rhs; }
    bool operator <= (int_type rhs) const { return getValue() <= rhs; }
    bool operator >= (int_type rhs) const { return getValue() >= rhs; }

    ThisType& operator += (int_type val) { value += val; assert_borders(isValid()); return *this; }
    ThisType& operator -= (int_type val) { value -= val; assert_borders(isValid()); return *this; }
    ThisType& operator *= (int_type val) { value *= val; assert_borders(isValid()); return *this; }
    ThisType& operator /= (int_type val) { value /= val; assert_borders(isValid()); return *this; }
    ThisType& operator %= (int_type val) { value %= val; assert_borders(isValid()); return *this; }

    ThisType& operator += (const ThisType& val) { value += val.getValue(); assert_borders(isValid()); return *this; }
    ThisType& operator -= (const ThisType& val) { value -= val.getValue(); assert_borders(isValid()); return *this; }
    ThisType& operator *= (const ThisType& val) { value *= val.getValue(); assert_borders(isValid()); return *this; }
    ThisType& operator /= (const ThisType& val) { value /= val.getValue(); assert_borders(isValid()); return *this; }
    ThisType& operator %= (const ThisType& val) { value %= val.getValue(); assert_borders(isValid()); return *this; }

    ThisType& operator ++ () { ++ value  ; assert_borders(isValid()); return *this;}
    ThisType  operator ++ (int) { ThisType result(getValue()); ++value; assert_borders(isValid()); return result;}
    ThisType& operator -- () { -- value  ; assert_borders(isValid()); return *this;}
    ThisType  operator -- (int) { ThisType result(getValue()); --value; assert_borders(isValid()); return result;}

    ThisType* clone() const { return static_cast<ThisType*>(INTEGER::clone()); }
    void swap(ThisType& that) { std::swap(value, that.value); }

    int_type operator + (const ThisType& rhs) const { int_type t(getValue()); return t += rhs.getValue();}
    int_type operator - (const ThisType& rhs) const { int_type t(getValue()); return t -= rhs.getValue();}
    int_type operator * (const ThisType& rhs) const { int_type t(getValue()); return t *= rhs.getValue();}
    int_type operator / (const ThisType& rhs) const { int_type t(getValue()); return t /= rhs.getValue();}

    friend int_type operator + (int_type lhs, const ThisType& rhs) { int_type t(lhs); return t += rhs.getValue();}
    friend int_type operator - (int_type lhs, const ThisType& rhs) { int_type t(lhs); return t -= rhs.getValue();}
    friend int_type operator * (int_type lhs, const ThisType& rhs) { int_type t(lhs); return t *= rhs.getValue();}
    friend int_type operator / (int_type lhs, const ThisType& rhs) { int_type t(lhs); return t /= rhs.getValue();}

    int_type operator + (int_type rhs) const { int_type t(getValue()); return t += rhs;}
    int_type operator - (int_type rhs) const { int_type t(getValue()); return t -= rhs;}
    int_type operator * (int_type rhs) const { int_type t(getValue()); return t *= rhs;}
    int_type operator / (int_type rhs) const { int_type t(getValue()); return t /= rhs;}

    static const InfoType theInfo;
    static bool equal_type(const ASN1::AbstractData& type)
    {return type.info() == reinterpret_cast<const ASN1::AbstractData::InfoType*>(&theInfo);}

    static const std::string& typeName() {
        static std::string tn = std::string("Constrained_INTEGER<") + std::to_string(contraint) + ", " +
                                std::to_string(lower) + ", " + std::to_string(upper) + ">";
        return tn;
    }

private:
    void assert_borders(bool valid) {
        if (!valid)
            throw ASN1ValueIsOutOfRange(QString("Value %1 is out of range [%2, %3]").arg(value).arg(LowerLimit).arg(UpperLimit));
    }
};

template <ConstraintType contraint, int lower, unsigned upper >
#if defined (__IBMCPP__)
// I don't know, why the else-part doesn't work on VAC++5/AiX...
const typename ConstrainedObject::InfoType Constrained_INTEGER<contraint, lower, upper>::theInfo = {
#else
const typename Constrained_INTEGER<contraint, lower, upper>::InfoType Constrained_INTEGER<contraint, lower, upper>::theInfo = {
#endif //!defined (__IBMCPP__)
    &INTEGER::create,
    2,
    contraint,
    lower,
    upper
};


/** Class for ASN Enumerated type.
*/
class ENUMERATED : public AbstractData, public detail::Allocator<ENUMERATED> {
protected:
    ENUMERATED(const void* info);
public:
    ENUMERATED(const ENUMERATED& that);
    ENUMERATED& operator = (const ENUMERATED& that);

    // ENUMERATED specific methods
    int asInt() const { return value; }
    void setFromInt(int val) { value = val; }

    bool isValid() const { return extendable() || isStrictlyValid() ;}
    bool isStrictlyValid() const { return value <= getMaximum();}

    int getMaximum() const { return info()->maxEnumValue; }

    bool operator == (int rhs) const { return value == rhs; }
    bool operator != (int rhs) const { return value != rhs; }
    bool operator < (int rhs) const { return value <  rhs; }
    bool operator > (int rhs) const { return value >  rhs; }
    bool operator <= (int rhs) const { return value <= rhs; }
    bool operator >= (int rhs) const { return value >= rhs; }

    static AbstractData* create(const void* info);
    bool extendable() const { return info()->extendableFlag; }

    virtual QVariant toVariant() const override;
    virtual void fromVariant(const QVariant& variant) override;

    static const std::string& typeName() { static std::string tn = "ENUMERATED"; return tn; }

protected:
    struct InfoType {
        CreateFun create;
        unsigned tag;
        bool extendableFlag;
        unsigned maxEnumValue;
        AVN_ONLY(const char** names;)
    };

    void swap(ENUMERATED& other) { std::swap(value, other.value); }
    int value;
private:
    virtual int do_compare(const AbstractData& other) const;
    virtual AbstractData* do_clone() const ;
    virtual bool do_accept(Visitor&);
    virtual bool do_accept(ConstVisitor&) const;
    const InfoType* info() const { return static_cast<const InfoType*>(info_); }

#ifdef ASN1_HAS_IOSTREAM
public:
    const char* getName() const;
    bool setFromName(const std::string&);
    const char** names() { return info()->names; }
#endif
};



/** Class for ASN Object Identifier type.
*/
class OBJECT_IDENTIFIER : public AbstractData, public detail::Allocator<OBJECT_IDENTIFIER> {
protected:
    OBJECT_IDENTIFIER(const void* info) : AbstractData(info) {}
public:
    OBJECT_IDENTIFIER() : AbstractData(&theInfo) {}
#if defined(_MSC_VER) && (_MSC_VER <= 1200)
    typedef unsigned* InputIterator;
#else
    template <class InputIterator>
#endif
    OBJECT_IDENTIFIER(InputIterator first, InputIterator last, const void* info = &theInfo)
        : AbstractData(info), value(first, last)
    { }

    OBJECT_IDENTIFIER(const OBJECT_IDENTIFIER& other);
    OBJECT_IDENTIFIER& operator=(const OBJECT_IDENTIFIER& other) {
        // extensibility and tags are not to be assigned,
        // therefore the parent assignment operator is not called
        value = other.value;
        return *this;
    }


    OBJECT_IDENTIFIER* clone() const { return static_cast<OBJECT_IDENTIFIER*>(do_clone());}
    static AbstractData* create(const void* info);
    void swap(OBJECT_IDENTIFIER& that) { value.swap(that.value); }

    bool isValid() const { return value.size() != 0;}
    bool isStrictlyValid() const { return value.size() != 0;}

    unsigned levels() const { return value.size(); }
    const unsigned operator[](unsigned idx) const { return value[idx]; }
    void append(unsigned arcValue) { value.push_back(arcValue); }
    void trim(unsigned levels = 1) { value.erase(value.end() - levels, value.end()); }

    template <class InputIterator>
    void assign(InputIterator first, InputIterator last)
    {   value.assign(first, last); }

    bool decodeCommon(const char* data, unsigned dataLen);
    void encodeCommon(std::vector<char>& eObjId) const;

    // comparison operators
    bool operator == (const OBJECT_IDENTIFIER& rhs) const { return value == rhs.value; }
    bool operator != (const OBJECT_IDENTIFIER& rhs) const { return value != rhs.value; }
    bool operator < (const OBJECT_IDENTIFIER& rhs) const { return value <  rhs.value; }
    bool operator > (const OBJECT_IDENTIFIER& rhs) const { return value >  rhs.value; }
    bool operator <= (const OBJECT_IDENTIFIER& rhs) const { return value <= rhs.value; }
    bool operator >= (const OBJECT_IDENTIFIER& rhs) const { return value >= rhs.value; }

    ASN1_EXPORT static const InfoType theInfo;
    static bool equal_type(const ASN1::AbstractData& type)
    {return type.info() == reinterpret_cast<const ASN1::AbstractData::InfoType*>(&theInfo);}

    virtual QVariant toVariant() const override;
    virtual void fromVariant(const QVariant& variant) override;

    static const std::string& typeName() { static std::string tn = "OBJECT_IDENTIFIER"; return tn; }

private:
    virtual int do_compare(const AbstractData& other) const;
    virtual AbstractData* do_clone() const;
    virtual bool do_accept(Visitor&);
    virtual bool do_accept(ConstVisitor&) const;
    std::vector<unsigned> value;
};


/** Class for ASN Bit String type.
*/
class BIT_STRING : public ConstrainedObject, public detail::Allocator<BIT_STRING> {
protected:
    BIT_STRING(const void* info);
public:
    BIT_STRING() : ConstrainedObject(&theInfo) {}
    BIT_STRING(const BIT_STRING& other);
    BIT_STRING& operator=(const BIT_STRING& other) {
        bitData = other.bitData;
        totalBits = other.totalBits;
        return *this;
    }

    bool isValid() const { return size() >= (unsigned)getLowerLimit() && (getConstraintType() != FixedConstraint || size() <= getUpperLimit()); }
    bool isStrictlyValid() const { return size() >= (unsigned)getLowerLimit() && size() <= getUpperLimit(); }

    BIT_STRING* clone() const { return static_cast<BIT_STRING*>(do_clone());}
    static AbstractData* create(const void* info);
    void swap(BIT_STRING& other) {
        bitData.swap(other.bitData);
        std::swap(totalBits, other.totalBits);
    }


    unsigned size() const { return totalBits; }
    void resize(unsigned nBits) {
        bitData.resize((nBits + 7) / 8);
        totalBits = nBits;
    }


    bool operator[](unsigned bit) const {
        if ((unsigned)bit < totalBits)
            return (bitData[bit >> 3] & (1 << (7 - (bit & 7)))) != 0;
        return false;
    }


    void setData(unsigned nBits, const char* buf) {
        int size = (nBits - 1) / 8 + 1;
        bitData.assign(buf, buf + size);
        totalBits = nBits;
    }

    const std::vector<char>& getData() const {
        return bitData;
    }

    void set(unsigned bit) {
        if (bit < totalBits)
            bitData[(unsigned)(bit >> 3)] |= 1 << (7 - (bit & 7));
    }
    void clear(unsigned bit) {
        if (bit < totalBits)
            bitData[(unsigned)(bit >> 3)] &= ~(1 << (7 - (bit & 7)));
    }
    void invert(unsigned bit) {
        if (bit < totalBits)
            bitData[(unsigned)(bit >> 3)] ^= 1 << (7 - (bit & 7));
    }

    bool operator == (const BIT_STRING& rhs) const { return compare(rhs) == 0; }
    bool operator != (const BIT_STRING& rhs) const { return compare(rhs) != 0; }
    bool operator < (const BIT_STRING& rhs) const { return compare(rhs) <  0; }
    bool operator > (const BIT_STRING& rhs) const { return compare(rhs) >  0; }
    bool operator <= (const BIT_STRING& rhs) const { return compare(rhs) <= 0; }
    bool operator >= (const BIT_STRING& rhs) const { return compare(rhs) >= 0; }

    static bool equal_type(const ASN1::AbstractData& type)
    {return type.info() == reinterpret_cast<const ASN1::AbstractData::InfoType*>(&theInfo);}
    ASN1_EXPORT static const InfoType theInfo;

    virtual QVariant toVariant() const override;
    virtual void fromVariant(const QVariant& variant) override;

    static const std::string& typeName() { static std::string tn = "BIT_STRING"; return tn; }

private:
    friend class BERDecoder;
    friend class PERDecoder;
    friend class AVNDecoder;
    virtual int do_compare(const AbstractData& other) const;
    virtual AbstractData* do_clone() const ;
    virtual bool do_accept(Visitor&);
    virtual bool do_accept(ConstVisitor&) const;

    unsigned totalBits;
    std::vector<char> bitData;
};


struct EmptyConstraint {
    enum {
        constraint_type = Unconstrained,
        lower_bound = 0,
        upper_bound = UINT_MAX
    };
};

template <unsigned TYPE, int LOWERBOUND, unsigned UPPERBOUND>
struct SizeConstraint {
    enum {
        constraint_type = TYPE,
        lower_bound = LOWERBOUND,
        upper_bound = UPPERBOUND
    };
};


template <class Constraint>
class Constrained_BIT_STRING : public BIT_STRING {
    typedef BIT_STRING Inherited;
protected:
    typedef Inherited::InfoType InfoType;
    Constrained_BIT_STRING(const void* info) : BIT_STRING(info) {}
public:
    Constrained_BIT_STRING() : BIT_STRING(&theInfo) {}
    // Constrained_BIT_STRING(const Constrained_BIT_STRING & other);  // use default copy constructor

    Constrained_BIT_STRING& operator=(const BIT_STRING& other) {
        BIT_STRING::operator = (other);
        return *this;
    }

    Constrained_BIT_STRING* clone() const { return static_cast<Constrained_BIT_STRING*>(BIT_STRING::clone());}
    static const InfoType theInfo;
    static bool equal_type(const ASN1::AbstractData& type)
    {return type.info() == reinterpret_cast<const ASN1::AbstractData::InfoType*>(&theInfo);}

    static const std::string& typeName() {
        static std::string tn = std::string("Constrained_BIT_STRING<") + std::to_string(Constraint::constraint_type) + ", " +
                                std::to_string(Constraint::lower_bound) + ", " + std::to_string(Constraint::upper_bound) + ">";
        return tn;
    }
};

template <class Constraint>
const typename Constrained_BIT_STRING<Constraint>::InfoType Constrained_BIT_STRING<Constraint>::theInfo = {
    BIT_STRING::create,
    3,
    Constraint::constraint_type,
    Constraint::lower_bound,
    Constraint::upper_bound
};


/** Class for ASN Octet String type.
*/
class OCTET_STRING : public ConstrainedObject, public std::vector<char>, public detail::Allocator<OCTET_STRING> {
    typedef std::vector<char> ContainerType;
protected:
    OCTET_STRING(const void* info);
public:
    typedef ContainerType::value_type value_type;
    typedef ContainerType::size_type size_type;
    typedef ContainerType::difference_type difference_type;
    typedef ContainerType::reference reference;
    typedef ContainerType::const_reference const_reference;

    OCTET_STRING() : ConstrainedObject(&theInfo) {}
    OCTET_STRING(size_type n, char v, const void* info = &theInfo) ;

    template <class Itr>
    OCTET_STRING(Itr first, Itr last)
        : ConstrainedObject(&theInfo), ContainerType(first, last)
    { }

    OCTET_STRING(const std::vector<char>& other, const void* info = &theInfo);

    OCTET_STRING(const OCTET_STRING& other) ;

    OCTET_STRING& operator=(const OCTET_STRING& other)    {
        assign(other.begin(), other.end());
        return *this;
    }

    OCTET_STRING& operator = (const std::vector<char>& other) {
        assign(other.begin(), other.end());
        return *this;
    }

    OCTET_STRING& operator=(const std::string& str) {
        assign(str.begin(), str.end());
        return *this;
    }

    OCTET_STRING& operator=(const char* str) {
        assign(str, str + strlen(str));
        return *this;
    }

    bool isValid() const { return size() >= (unsigned)getLowerLimit() && (getConstraintType() != FixedConstraint || size() <= getUpperLimit());}
    bool isStrictlyValid() const { return size() >= (unsigned)getLowerLimit() && size() <= getUpperLimit();}

    OCTET_STRING* clone() const { return static_cast<OCTET_STRING*>(do_clone());}
    static AbstractData* create(const void* info);
    void swap(OCTET_STRING& other) { ContainerType::swap(other); }

    operator std::string() const { return std::string(begin(), end()); }

    ASN1_EXPORT static const InfoType theInfo;
    static bool equal_type(const ASN1::AbstractData& type)
    {return type.info() == reinterpret_cast<const ASN1::AbstractData::InfoType*>(&theInfo);}

    virtual QVariant toVariant() const override;
    virtual void fromVariant(const QVariant& variant) override;

    static const std::string& typeName() { static std::string tn = "OCTET_STRING"; return tn; }

private:
    virtual int do_compare(const AbstractData& other) const;
    virtual AbstractData* do_clone() const ;
    virtual bool do_accept(Visitor&);
    virtual bool do_accept(ConstVisitor&) const;
};

template <class Constraint>
class Constrained_OCTET_STRING : public OCTET_STRING {
    typedef OCTET_STRING Inherited;
protected:
    typedef Inherited::InfoType InfoType;
public:
    Constrained_OCTET_STRING(const void* info = &theInfo): OCTET_STRING(info)   {}

    Constrained_OCTET_STRING(size_type n, char v) : OCTET_STRING(n, v, &theInfo)    { }

    template <class Itr>
    Constrained_OCTET_STRING(Itr first, Itr last) :
        OCTET_STRING(&theInfo)  { assign(first, last); }

    Constrained_OCTET_STRING(const std::vector<char>& other) : OCTET_STRING(other, &theInfo) {}

    // Constrained_OCTET_STRING(const Constrained_OCTET_STRING & other) ;

    Constrained_OCTET_STRING& operator = (const std::vector<char>& other) {
        assign(other.begin(), other.end());
        return *this;
    }

    Constrained_OCTET_STRING& operator=(const std::string& str) {
        assign((const char*)(str.begin()), (const char*)(str.end()));
        return *this;
    }

    Constrained_OCTET_STRING& operator=(const char* str) {
        assign(str, str + strlen(str));
        return *this;
    }

    Constrained_OCTET_STRING* clone() const { return static_cast<Constrained_OCTET_STRING*>(OCTET_STRING::clone());}
    static AbstractData* create();
    void swap(Constrained_OCTET_STRING& other) { OCTET_STRING::swap(other); }

    static const InfoType theInfo;
    static bool equal_type(const ASN1::AbstractData& type)
    {return type.info() == reinterpret_cast<const ASN1::AbstractData::InfoType*>(&theInfo);}

    static const std::string& typeName() {
        static std::string tn = std::string("Constrained_OCTET_STRING<") + std::to_string(Constraint::constraint_type) + ", " +
                                std::to_string(Constraint::lower_bound) + ", " + std::to_string(Constraint::upper_bound) + ">";
        return tn;
    }
};

template <class Constraint>
const typename Constrained_OCTET_STRING<Constraint>::InfoType Constrained_OCTET_STRING<Constraint>::theInfo = {
    OCTET_STRING::create,
    4,
    Constraint::constraint_type,
    Constraint::lower_bound,
    Constraint::upper_bound
};



/** Base class for ASN String types.
*/
class AbstractString : public ConstrainedObject, public std::string, public detail::Allocator<AbstractString> {
protected:
    typedef std::string base_string;
public:
    AbstractString(const AbstractString& other);
    typedef base_string::value_type value_type;
    typedef base_string::size_type size_type;
    typedef base_string::difference_type difference_type;
    typedef base_string::reference reference;
    typedef base_string::const_reference const_reference;


    AbstractString& operator=(const char* str) { assign(str);  return *this;}
    AbstractString& operator=(const std::string& str) { assign(str);  return *this;}
    AbstractString& operator=(char c) { assign(1, c); return *this;}
    AbstractString& operator+=(const std::string& rhs) { append(rhs);  return *this;}
    AbstractString& operator+=(const char* s) { append(s);  return *this;}
    AbstractString& operator+=(char c) { append(1, c);  return *this;}
    AbstractString& append(const std::string& str) { base_string::append(str); assert_borders(); return *this;}
    AbstractString& append(const std::string& str, size_type pos, size_type n) { base_string::append(str, pos, n); assert_borders(); return *this;}
    AbstractString& append(const char* s, size_type n) { base_string::append(s, n); assert_borders(); return *this;}
    AbstractString& append(const char* s) { base_string::append(s); assert_borders(); return *this;}
    AbstractString& append(size_type n, char c) { base_string::append(n, c); assert_borders(); return *this;}
    AbstractString& append(const_iterator first, const_iterator last) { base_string::append(first, last); assert_borders(); return *this;}
    AbstractString& assign(const base_string& str) { base_string::assign(str); assert_borders(); return *this;}
    AbstractString& assign(const base_string& str, size_type pos, size_type n) { base_string::assign(str, pos, n); assert_borders(); return *this;}
    AbstractString& assign(const char* s, size_type n) { base_string::assign(s, n); assert_borders(); return *this;}
    AbstractString& assign(const char* s) { base_string::assign(s); assert_borders(); return *this;}
    AbstractString& assign(size_type n, char c) { base_string::assign(n, c); assert_borders(); return *this;}
    AbstractString& assign(const_iterator first, const_iterator last) { base_string::assign(first, last); assert_borders(); return *this;}
    AbstractString& insert(size_type p0, const base_string& str) { base_string::insert(p0, str); assert_borders(); return *this;}
    AbstractString& insert(size_type p0, const base_string& str, size_type pos, size_type n) { base_string::insert(p0, str, pos, n); assert_borders(); return *this;}
    AbstractString& insert(size_type p0, const char* s, size_type n) { base_string::insert(p0, s, n); assert_borders(); return *this;}
    AbstractString& insert(size_type p0, const char* s) { base_string::insert(p0, s); assert_borders(); return *this;}
    AbstractString& insert(size_type p0, size_type n, char c) { base_string::insert(p0, n, c); assert_borders(); return *this;}

    int compare(const AbstractString& other) const { return base_string::compare(other); }

    bool isValid() const;
    bool isStrictlyValid() const;
    size_type find_first_invalid() const {
        return info()->characterSetSize ? find_first_not_of(info()->characterSet, 0, info()->characterSetSize) : std::string::npos;
    }

#ifdef ASN1_HAS_IOSTREAM
    friend std::ostream& operator << (std::ostream& os, const AbstractString& data)
    { return os << static_cast<const std::string&>(data); }
#endif

    const char* getCharacterSet() const  { return info()->characterSet; }
    unsigned getCharacterSetSize() const { return info()->characterSetSize; }
    unsigned getCanonicalSetBits() const { return info()->canonicalSetBits; }
    unsigned getNumBits(bool align) const { return align ? info()->charSetAlignedBits : info()->charSetUnalignedBits; }

    static AbstractData* create(const void*);

    virtual QVariant toVariant() const override;
    virtual void fromVariant(const QVariant& variant) override;

    static const std::string& typeName() { static std::string tn = "AbstractString"; return tn; }

private:
    virtual int do_compare(const AbstractData& other) const;
    virtual AbstractData* do_clone() const ;
    virtual bool do_accept(Visitor&);
    virtual bool do_accept(ConstVisitor&) const;

    void assert_borders() {
        if (!isValid())
            throw ASN1ValueIsOutOfRange(QString("Length of '%1' is out of range [%2, %3]").arg(data()).arg(getLowerLimit()).arg(getUpperLimit()));
    }

protected:
    AbstractString(const void* info);
    AbstractString(const void* info, const std::string& str);
    AbstractString(const void* info, const char* str);

    struct InfoType {
        CreateFun create;
        unsigned tag;
        unsigned type;
        int lowerLimit;
        unsigned upperLimit;
        const char* characterSet;
        unsigned characterSetSize;
        unsigned canonicalSetBits;
        unsigned charSetUnalignedBits;
        unsigned charSetAlignedBits;
    };
    const InfoType* info() const { return static_cast<const InfoType*>(info_);}
};

class NumericString : public AbstractString {
protected:
    NumericString(const void* info) : AbstractString(info) { }
public:
    NumericString() : AbstractString(&theInfo) { }
    NumericString(const std::string& str, const void* info = &theInfo) : AbstractString(info, str) { }
    NumericString(const char* str, const void* info = &theInfo) : AbstractString(info, str) { }

    NumericString& operator = (const NumericString& other) { assign(other); return *this;}
    NumericString& operator=(const char* str) { assign(str);  return *this;}
    NumericString& operator=(const std::string& str) { assign(str);  return *this;}
    NumericString& operator=(char c) { assign(1, c); return *this;}

    NumericString* clone() const { return static_cast<NumericString*>(AbstractString::clone()); }
    static AbstractData* create();
    void swap(NumericString& other) { base_string::swap(other); }

    ASN1_EXPORT static const InfoType theInfo;
    static bool equal_type(const ASN1::AbstractData& type)
    {return type.info() == reinterpret_cast<const ASN1::AbstractData::InfoType*>(&theInfo);}
};

class PrintableString : public AbstractString {
protected:
    PrintableString(const void* info) : AbstractString(info) { }
public:
    PrintableString() : AbstractString(&theInfo) { }
    PrintableString(const std::string& str, const void* info = &theInfo) : AbstractString(info, str) { }
    PrintableString(const char* str, const void* info = &theInfo) : AbstractString(info, str) { }

    PrintableString& operator = (const PrintableString& other) { assign(other); return *this;}
    PrintableString& operator=(const char* str) { assign(str);  return *this;}
    PrintableString& operator=(const std::string& str) { assign(str);  return *this;}
    PrintableString& operator=(char c) { assign(1, c); return *this;}
    PrintableString* clone() const { return static_cast<PrintableString*>(AbstractString::clone()); }
    static AbstractData* create();
    void swap(PrintableString& other) { base_string::swap(other); }
    ASN1_EXPORT static const InfoType theInfo;
    static bool equal_type(const ASN1::AbstractData& type)
    {return type.info() == reinterpret_cast<const ASN1::AbstractData::InfoType*>(&theInfo);}
};

class VisibleString : public AbstractString {
protected:
    VisibleString(const void* info) : AbstractString(info) { }
public:
    VisibleString() : AbstractString(&theInfo) { }
    VisibleString(const std::string& str, const void* info = &theInfo) : AbstractString(info, str) { }
    VisibleString(const char* str, const void* info = &theInfo) : AbstractString(info, str) { }

    VisibleString& operator = (const VisibleString& other) { assign(other); return *this;}
    VisibleString& operator=(const char* str) { assign(str);  return *this;}
    VisibleString& operator=(const std::string& str) { assign(str);  return *this;}
    VisibleString& operator=(char c) { assign(1, c); return *this;}
    VisibleString* clone() const { return static_cast<VisibleString*>(AbstractString::clone()); }
    static AbstractData* create();
    void swap(VisibleString& other) { base_string::swap(other); }

    ASN1_EXPORT static const InfoType theInfo;
    static bool equal_type(const ASN1::AbstractData& type)
    {return type.info() == reinterpret_cast<const ASN1::AbstractData::InfoType*>(&theInfo);}
};

class IA5String : public AbstractString {
protected:
    IA5String(const void* info) : AbstractString(info) { }
public:
    IA5String() : AbstractString(&theInfo) { }
    IA5String(const std::string& str, const void* info = &theInfo) : AbstractString(info, str) { }
    IA5String(const char* str, const void* info = &theInfo) : AbstractString(info, str) { }

    IA5String& operator = (const IA5String& other) { assign(other); return *this;}
    IA5String& operator=(const char* str) { assign(str);  return *this;}
    IA5String& operator=(const std::string& str) { assign(str);  return *this;}
    IA5String& operator=(char c) { assign(1, c); return *this;}
    IA5String* clone() const { return static_cast<IA5String*>(AbstractString::clone()); }
    static AbstractData* create();
    void swap(IA5String& other) { base_string::swap(other); }

    ASN1_EXPORT static const InfoType theInfo;
    static bool equal_type(const ASN1::AbstractData& type)
    {return type.info() == reinterpret_cast<const ASN1::AbstractData::InfoType*>(&theInfo);}
};

class GeneralString : public AbstractString {
protected:
    GeneralString(const void* info) : AbstractString(info) { }
public:
    GeneralString() : AbstractString(&theInfo) { }
    GeneralString(const std::string& str, const void* info = &theInfo) : AbstractString(info, str) { }
    GeneralString(const char* str, const void* info = &theInfo) : AbstractString(info, str) { }

    GeneralString& operator = (const GeneralString& other) { assign(other); return *this;}
    GeneralString& operator=(const char* str) { assign(str);  return *this;}
    GeneralString& operator=(const std::string& str) { assign(str);  return *this;}
    GeneralString& operator=(char c) { assign(1, c); return *this;}
    GeneralString* clone() const { return static_cast<GeneralString*>(AbstractString::clone()); }
    static AbstractData* create();
    void swap(GeneralString& other) { base_string::swap(other); }

    ASN1_EXPORT static const InfoType theInfo;
    static bool equal_type(const ASN1::AbstractData& type)
    {return type.info() == reinterpret_cast<const ASN1::AbstractData::InfoType*>(&theInfo);}
};


/** Class for ASN BMP (16 bit) String type.*/

class BMPString : public ConstrainedObject, public std::wstring {
protected:
    typedef std::wstring base_string;
    BMPString(const void* info);
    struct InfoType {
        CreateFun create;
        unsigned tag;
        unsigned type;
        int lowerLimit;
        unsigned upperLimit;
        wchar_t firstChar, lastChar;
        unsigned charSetUnalignedBits;
        unsigned charSetAlignedBits;
    };
public:
    typedef base_string::value_type value_type;
    typedef base_string::size_type size_type;
    typedef base_string::difference_type difference_type;
    typedef base_string::reference reference;
    typedef base_string::const_reference const_reference;

    BMPString();
    BMPString(const base_string& str, const void* info = &theInfo);
    BMPString(const value_type* str, const void* info = &theInfo);
    BMPString(const BMPString& other);
    BMPString& operator=(const value_type* str) { return assign(str);}
    BMPString& operator=(const base_string& str) { return  assign(str);}
    BMPString& operator=(value_type c) { return  assign(1, c);}
    BMPString& operator+=(const base_string& rhs) { return append(rhs);}
    BMPString& operator+=(const value_type* s) { return append(s);}
    BMPString& operator+=(value_type c) { return append(1, c);}
    BMPString& append(const base_string& str) { base_string::append(str); return *this;}
    BMPString& append(const base_string& str, size_type pos, size_type n) { base_string::append(str, pos, n); return *this;}
    BMPString& append(const value_type* s, size_type n) { base_string::append(s, n); return *this;}
    BMPString& append(const value_type* s) { base_string::append(s); return *this;}
    BMPString& append(size_type n, value_type c) { base_string::append(n, c); return *this;}
    BMPString& append(const_iterator first, const_iterator last) { base_string::append(first, last); return *this;}
    BMPString& assign(const base_string& str) { base_string::assign(str); return *this;}
    BMPString& assign(const base_string& str, size_type pos, size_type n) { base_string::assign(str, pos, n); return *this;}
    BMPString& assign(const value_type* s, size_type n) { base_string::assign(s, n); return *this;}
    BMPString& assign(const value_type* s) { base_string::assign(s); return *this;}
    BMPString& assign(size_type n, value_type c) { base_string::assign(n, c); return *this;}
    BMPString& assign(const_iterator first, const_iterator last) { base_string::assign(first, last); return *this;}
    BMPString& insert(size_type p0, const base_string& str) { base_string::insert(p0, str); return *this;}
    BMPString& insert(size_type p0, const base_string& str, size_type pos, size_type n) { base_string::insert(p0, str, pos, n); return *this;}
    BMPString& insert(size_type p0, const value_type* s, size_type n) { base_string::insert(p0, s, n); return *this;}
    BMPString& insert(size_type p0, const value_type* s) { base_string::insert(p0, s); return *this;}
    BMPString& insert(size_type p0, size_type n, value_type c) { base_string::insert(p0, n, c); return *this;}

    int compare(const BMPString& other) const { return base_string::compare(other); }

    BMPString* clone() const { return static_cast<BMPString*>(do_clone()); }
    static AbstractData* create(const void* info);
    void swap(BMPString& other) { base_string::swap(other); }

    bool isValid() const;
    bool isStrictlyValid() const;

#ifdef ASN1_HAS_IOSTREAM
    friend std::ostream& operator << (std::ostream& os, const BMPString& data)
    { return os << static_cast<const AbstractData&>(data); }
#endif

    wchar_t getFirstChar() const { return info()->firstChar; }
    wchar_t getLastChar() const { return info()->lastChar; }
    unsigned getNumBits(bool align) const {
        return align ? info()->charSetAlignedBits : info()->charSetUnalignedBits;
    }

    ASN1_EXPORT static const InfoType theInfo;
    static bool equal_type(const ASN1::AbstractData& type)
    {return type.info() == reinterpret_cast<const ASN1::AbstractData::InfoType*>(&theInfo);}
    size_type first_illegal_at() const;

    virtual QVariant toVariant() const override;
    virtual void fromVariant(const QVariant& variant) override;

    static const std::string& typeName() { static std::string tn = "BMPString"; return tn; }

private:
    bool legalCharacter(wchar_t ch) const;
    const InfoType* info() const { return static_cast<const InfoType*>(info_);}

    int do_compare(const AbstractData& other) const;
    virtual AbstractData* do_clone() const;
    virtual bool do_accept(Visitor&);
    virtual bool do_accept(ConstVisitor&) const;
};


class GeneralizedTime : public AbstractData, public detail::Allocator<GeneralizedTime> {
protected:
    GeneralizedTime(const void* info);
public:
    typedef const char* const_reference;
    GeneralizedTime();
    GeneralizedTime(const char* value);
    GeneralizedTime(int year, int month, int day,
                    int hour = 0, int minute = 0, int second = 0,
                    int millisec = 0, int mindiff = 0, bool utc = false);

    GeneralizedTime(const GeneralizedTime& other) ;
    GeneralizedTime& operator = (const GeneralizedTime& other) ;

    /**
     * Returns the character string format of this object. Unlike asValueNototation(),
     * the string returned by get() does not contain double quote marks (").
     */
    std::string get() const;
    /**
     * Set the value of this object using character string format. Unlike fromValueNototation(),
     * the string used by set() shall not contain double quote marks (").
     */
    void set(const char*);

    time_t get_time_t();
    void set_time_t(time_t gmt);

    int get_year() const { return year; }
    int get_month() const { return month; }
    int get_day() const { return day; }
    int get_hour() const { return hour; }
    int get_minute() const { return minute;}
    int get_second() const { return second; }
    int get_millisec() const { return millisec; }
    int get_mindiff() const { return mindiff; }
    bool get_utc() const { return utc;}

    void set_year(int yr) { year = yr; }
    void set_month(int mon) { month = mon; }
    void set_day(int dy) { day = dy; }
    void set_hour(int hr) { hour = hr; }
    void set_minute(int min) { minute = min;}
    void set_second(int sec) { second = sec; }
    void set_millisec(int milsec) { millisec = milsec ;}
    void set_mindiff(int mdiff) { utc = false; mindiff = mdiff; }
    void set_utc(bool tc) { mindiff = 0; utc = tc; }

    void swap(GeneralizedTime& other) ;
    GeneralizedTime* clone() const { return static_cast<GeneralizedTime*>(do_clone()); }

    bool isValid() const { return isStrictlyValid();}
    static AbstractData* create(const void* info);
    bool isStrictlyValid() const;

    ASN1_EXPORT static const InfoType theInfo;
    static bool equal_type(const ASN1::AbstractData& type)
    {return type.info() == reinterpret_cast<const ASN1::AbstractData::InfoType*>(&theInfo);}

    virtual QVariant toVariant() const override;
    virtual void fromVariant(const QVariant& variant) override;

    static const std::string& typeName() { static std::string tn = "GeneralizedTime"; return tn; }

private:
    virtual int do_compare(const AbstractData& other) const;
    virtual AbstractData* do_clone() const ;
    virtual bool do_accept(Visitor&);
    virtual bool do_accept(ConstVisitor&) const;

    int year, month, day, hour, minute, second, millisec, mindiff;
    bool utc;

};




/** Class for ASN Choice type.
*/
class CHOICE : public AbstractData, public detail::Allocator<CHOICE> {
public:
    enum Id {
        unknownSelection_ = -2,
        unselected_ = -1
    };

    /**
     * Returns the index of the currently selected alternatives.
     */
    int currentSelection() const { return choiceID; }
    bool isSelected(int sel) const { return choiceID == sel; }
    bool isUnknownSelection() const { return choiceID == unknownSelection_; }

    bool isValid() const;
    bool isStrictlyValid() const;

    /**
     * Returns the object of the currently selected alternatives.
     */
    AbstractData* getSelection() { return choice.get(); }
    const AbstractData* getSelection() const { return choice.get(); }
    /**
     * Set the value by alternative index number.
     */
    bool select(int selection);
    /**
     * Set the value by index number and the pointer to the selected alternative.
     *
     * This member funtion will take the ownership of the object pointed by \c obj.
     *
     * @param id The index number of the selected alternative.
     * @param obj The pointer to the selected alternative.
     */
    AbstractData* setSelection(int id, AbstractData* obj);

    /**
     * Returns the number of alternatives in the extension root.
     */
    unsigned getNumChoices() const { return info()->numChoices; }
    bool extendable() const { return info()->extendableFlag; }
    unsigned getSelectionTag() const {
        assert(choiceID >= 0);
        return info()->tags == 0 ? 0x800000 | choiceID
               : info()->tags[choiceID];
    }

    const QString& getChoiceKey() const {return choiceKey;}
    void setChoiceWrapperId(const QString& choice_wrapper_id) { choiceWrapperId = choice_wrapper_id; }

    static AbstractData* create(const void* info);

    virtual QVariant toVariant() const override;
    virtual void fromVariant(const QVariant& variant) override;

    static const std::string& typeName() { static std::string tn = "CHOICE"; return tn; }

protected:
    CHOICE(const void* info , int id = -1, AbstractData* value = NULL);
    ~CHOICE();


    CHOICE(const CHOICE& other);
    CHOICE& operator=(const CHOICE& other);
    void swap(CHOICE& other);

    void setChoiceKey(const QString& choice_key) {choiceKey = choice_key;}

    std::unique_ptr<AbstractData> choice;
    QString choiceKey;
    QString choiceWrapperId;
    int choiceID;

    struct InfoType {
        CreateFun create;
        unsigned tag;
        bool extendableFlag;
        const void** selectionInfos;
        unsigned numChoices;
        unsigned totalChoices;
        unsigned* tags;
        AVN_ONLY(const char** names;)
    };
private:
    virtual int do_compare(const AbstractData& other) const;
    virtual bool do_accept(Visitor&);
    virtual bool do_accept(ConstVisitor&) const;
    virtual AbstractData* do_clone() const;
    bool createSelection();
    const InfoType* info() const { return static_cast<const InfoType*>(info_);}
public:
    /**
     * Set the value by \c tag number and \c tag class.
     */
    bool setID(unsigned tagNum, unsigned tagClass);

#ifdef ASN1_HAS_IOSTREAM
public:
    const char* getSelectionName() const { assert(choiceID < static_cast<int>(info()->totalChoices)); return info()->names[choiceID]; }
    friend class AVNDecoder;
#endif
};

class PEREncoder;
class PERDecoder;



/** Class for ASN Sequence type.
*/
class SEQUENCE : public AbstractData, public detail::Allocator<SEQUENCE> {
public:
    enum {
        AUTOMATIC_TAG,
        EXPLICIT_TAG,
        IMPLICIT_TAG
    };

    SEQUENCE(const SEQUENCE& other);
    ~SEQUENCE();
    SEQUENCE& operator=(const SEQUENCE& other);
    SEQUENCE* clone() const { return static_cast<SEQUENCE*>(do_clone()); }
    void swap(SEQUENCE& other);

    /**
     *  Returns the pointer to the component of the SEQUENCE at position \c pos.
     */
    AbstractData* getField(unsigned pos) {
        assert(pos < fields.size());
        return fields[pos];
    }
    const AbstractData* getField(unsigned pos) const {
        assert(pos < fields.size());
        return fields[pos];
    }

    unsigned tagMode() const {
        if (info()->tags == 0)
            return AUTOMATIC_TAG;
        else if (info()->tags != static_cast<const void*>(&defaultTag))
            return EXPLICIT_TAG;
        else
            return IMPLICIT_TAG;
    }
    unsigned getFieldTag(int pos) const {
        if (info()->tags == 0) // Automatic Tags
            return 0x800000 |  pos;
        else if (info()->tags != static_cast<const void*>(&defaultTag)) // EXPLICIT Tag
            return info()->tags[pos];
        else
            return static_cast<const AbstractData::InfoType*>(info()->fieldInfos[pos])->tag; // IMPLICIT Tag
    }
    bool extendable() const { return info()->extendableFlag; }
    /**
     * Makes an OPTIONAL field present.
     *
     * @param opt The index to the OPTIONAL field
     * @param pos The position of the OPTIONAL field
     */
    void includeOptionalField(unsigned opt, unsigned pos);
    /**
     * Tests the presence of an OPTIONAL field.
     *
     * @param opt The index to the OPTIONAL field
     */
    bool hasOptionalField(unsigned opt) const;
    /**
     * Makes an OPTIONAL field absence.
     *
     * @param opt The index to the OPTIONAL field
     */
    void removeOptionalField(unsigned opt);

    static AbstractData* create(const void*);

    const QString& getFieldKey(size_t pos) const {
        assert(pos < fieldKeys.size());
        return fieldKeys[pos];
    }

    virtual QVariant toVariant() const override;
    virtual void fromVariant(const QVariant& variant) override;

    static const std::string& typeName() { static std::string tn = "SEQUENCE"; return tn; }

protected:

    SEQUENCE(const void* info);

    virtual void setFieldKeys() {}

    void setFieldKey(size_t pos, const QString& name) {
        assert(pos < fieldKeys.size());
        fieldKeys[pos] = name;
    }

    enum {
        mandatory_ = -1
    };
    class FieldVector : public std::vector<AbstractData*> {
    public:
        FieldVector() {};
        ~FieldVector();
        FieldVector(const FieldVector& other);
    private:
        FieldVector& operator = (const FieldVector& other);
    };

    struct BitMap {
        BitMap() : totalBits(0) {}
        unsigned size() const { return totalBits;}
        void resize(unsigned nBits);
        bool operator [](unsigned bit) const;
        void set(unsigned bit);
        void clear(unsigned bit);
        void swap(BitMap& other);
        unsigned totalBits;
        std::vector<char> bitData;
    };

    FieldVector fields;
    std::vector<QString> fieldKeys;
    BitMap optionMap;
    BitMap extensionMap;

    ASN1_EXPORT static const unsigned defaultTag;

    struct InfoType {
        CreateFun create;
        unsigned tag;
        bool extendableFlag;
        const void** fieldInfos;
        int* ids;
        unsigned numFields;
        unsigned knownExtensions;
        unsigned numOptional;
        const char* nonOptionalExtensions;
        const unsigned* tags;
        AVN_ONLY(const char** names;)
    };
private:
    friend class Visitor;
    friend class ConstVisitor;
    friend class PEREncoder;
    friend class PERDecoder;

    virtual int do_compare(const AbstractData& other) const;
    virtual AbstractData* do_clone() const ;
    virtual bool do_accept(ConstVisitor&) const;

    const InfoType* info() const { return static_cast<const InfoType*>(info_);}
protected:
    virtual bool do_accept(Visitor&);
#ifdef ASN1_HAS_IOSTREAM
public:
    const char* getFieldName(int i) const { return info()->names[i]; }
#endif

};


/** Class for ASN set type.
*/
class SET : public SEQUENCE {
public:
    SET* clone() const { return static_cast<SET*>(SEQUENCE::clone()); }
protected:
    SET(const void* info)
        : SEQUENCE(info)
    {  }
};



/** Class for ASN SEQUENCE type.
*/
class SEQUENCE_OF_Base : public ConstrainedObject, public detail::Allocator<SEQUENCE_OF_Base> {
protected:
    typedef std::vector<AbstractData*> Container;
    SEQUENCE_OF_Base(const void*);
public:
    ~SEQUENCE_OF_Base() { clear();}
    SEQUENCE_OF_Base(const SEQUENCE_OF_Base& other);
    SEQUENCE_OF_Base& operator=(const SEQUENCE_OF_Base& other);

    typedef Container::iterator iterator;
    typedef Container::const_iterator const_iterator;

    iterator begin()  { return container.begin(); }
    iterator end()  { return container.end(); }
    const_iterator begin() const { return container.begin(); }
    const_iterator end() const { return container.end(); }
    void resize(Container::size_type size);
    void push_back(AbstractData* obj) { container.push_back(obj);}
    void erase(iterator first, iterator last);
    Container::size_type    size() const { return container.size(); }
    Container::size_type    max_size() const { return container.max_size(); }
    Container::size_type    capacity() const { return container.capacity(); }
    void reserve(Container::size_type n) { container.reserve(n); }
    bool empty() const { return container.empty(); }
    void clear();

    bool isValid() const;
    bool isStrictlyValid() const;

    AbstractData* createElement() const {
        const AbstractData::InfoType* elementInfo =
            static_cast<const AbstractData::InfoType*>(static_cast<const InfoType*>(info_)->elementInfo);
        return elementInfo->create(elementInfo);
    }

    static AbstractData* create(const void* info);

    virtual QVariant toVariant() const override;
    virtual void fromVariant(const QVariant& variant) override;

    static const std::string& typeName() { static std::string tn = "SEQUENCE_OF_Base"; return tn; }

protected:
    void insert(iterator position, Container::size_type n, const AbstractData& x);
    void insert(iterator position, const_iterator first, const_iterator last);

    Container container;
    virtual AbstractData* do_clone() const;

    struct create_from_ptr {
        AbstractData* operator()(const AbstractData* obj) { return obj->clone(); }
    };

    struct InfoType {
        CreateFun create;
        unsigned tag;
        unsigned type;
        int lowerLimit;
        unsigned upperLimit;
        const void* elementInfo;
    };

private:
    virtual int do_compare(const AbstractData& other) const;
    virtual bool do_accept(Visitor&);
    virtual bool do_accept(ConstVisitor&) const;

    struct create_from0 {
        const AbstractData& object;
        create_from0(const AbstractData& obj) : object(obj) {}
        AbstractData* operator()() const { return object.clone(); }
    };
};


template <class T, class Constraint = EmptyConstraint>
class SEQUENCE_OF : public SEQUENCE_OF_Base {
    typedef SEQUENCE_OF_Base Inherited;
protected:
    typedef Inherited::InfoType InfoType;
public:
    typedef T&                                  reference;
    typedef const T&                            const_reference;
    typedef T                                   value_type;
    typedef value_type*                         pointer;
    typedef const value_type*                   const_pointer;
    typedef typename Container::size_type       size_type;
    typedef typename Container::difference_type difference_type;

private:
    typedef boost::iterator<std::random_access_iterator_tag, value_type> my_iterator_traits;
    typedef boost::iterator<std::random_access_iterator_tag, const value_type> my_const_iterator_traits;
public:
    class iterator : public my_iterator_traits {
    public:
        typedef T                           value_type;
        typedef T&                          reference;
        typedef value_type*                 pointer;
        typedef typename Container::difference_type difference_type;
        iterator() {}
        iterator(typename Container::iterator i) : itsIter(i) {}
        typename Container::iterator base() const {return itsIter;}

        reference operator*() const {return *static_cast<pointer>(*itsIter);}
        pointer operator->() const {return static_cast<pointer>(*itsIter);}
        reference operator[](difference_type n) const {return *static_cast<pointer>(*itsIter[n]);}

        iterator& operator++()          {++itsIter; return *this;}
        iterator& operator--()          {--itsIter; return *this;}
        iterator operator++(int)        {iterator t(*this); ++itsIter; return t;}
        iterator operator--(int)        {iterator t(*this); --itsIter; return t;}
        iterator& operator+=(difference_type n)     {itsIter += n; return *this;}
        iterator& operator-=(difference_type n)     {itsIter -= n; return *this;}
        iterator operator+(difference_type n) const {return iterator(itsIter + n);}
        iterator operator-(difference_type n) const {return iterator(itsIter - n);}
        //friend iterator operator + (difference_type n, const iterator& it) { return it+n; }
        difference_type operator - (const iterator& rhs) const { return base() - rhs.base(); }

        bool operator==(const iterator& r) const    {return itsIter == r.itsIter;}
        bool operator!=(const iterator& r) const    {return itsIter != r.itsIter;}
        bool operator<(const iterator& r) const     {return itsIter < r.itsIter;}
    private:
        typename Container::iterator itsIter;
    };

    class const_iterator : public my_const_iterator_traits {
    public:
        typedef T                                   value_type;
        typedef const T&                            reference;
        typedef const value_type*                   pointer;
        typedef typename Container::difference_type difference_type;

        const_iterator() {}
        const_iterator(typename Container::const_iterator i) : itsIter(i) {}
        const_iterator(typename Container::iterator i) : itsIter(i) {}
        const_iterator(iterator i) : itsIter(i.base()) {}
        typename Container::const_iterator base() const {return itsIter;}

        reference operator*() const {return *static_cast<pointer>(*itsIter);}
        pointer operator->() const {return static_cast<pointer>(*itsIter);}
        reference operator[](difference_type n) const {return *static_cast<pointer>(*itsIter[n]);}

        const_iterator& operator++()          {++itsIter; return *this;}
        const_iterator& operator--()          {--itsIter; return *this;}
        const_iterator  operator++(int)        {const_iterator t(*this); ++itsIter; return t;}
        const_iterator  operator--(int)        {const_iterator t(*this); --itsIter; return t;}
        const_iterator& operator+=(difference_type n)     {itsIter += n; return *this;}
        const_iterator& operator-=(difference_type n)     {itsIter -= n; return *this;}
        const_iterator  operator+(difference_type n) const {return const_iterator(itsIter + n);}
        const_iterator  operator-(difference_type n) const {return const_iterator(itsIter - n);}

        //friend const_iterator  operator + (difference_type n, const const_iterator& it) { return it+n; }
        difference_type operator - (const const_iterator& rhs) const { return base() - rhs.base(); }

        bool operator==(const const_iterator& r) const    {return itsIter == r.itsIter;}
        bool operator!=(const const_iterator& r) const    {return itsIter != r.itsIter;}
        bool operator< (const const_iterator& r) const     {return itsIter < r.itsIter;}
    private:
        Container::const_iterator itsIter;
    };

#if 1 //defined(BOOST_NO_STD_ITERATOR) || defined (BOOST_MSVC_STD_ITERATOR)
    class reverse_iterator : public my_iterator_traits {
    public:
        typedef T                                   value_type;
        typedef T&                                  reference;
        typedef value_type*                         pointer;
        typedef typename Container::difference_type difference_type;

        reverse_iterator() {}
        reverse_iterator(iterator i) : itsIter(i) {}
        iterator base() const {return itsIter;}

        reference operator*() const { return *(itsIter - 1); }
        pointer operator->() const  { return &(**this); }
        reference operator[](difference_type n) const {return *((*this) + n);}

        reverse_iterator& operator++()          {--itsIter; return *this;}
        reverse_iterator& operator--()          {++itsIter; return *this;}
        reverse_iterator operator++(int)        {reverse_iterator t(*this); --itsIter; return t;}
        reverse_iterator operator--(int)        {reverse_iterator t(*this); ++itsIter; return t;}
        reverse_iterator& operator+=(difference_type n)     {itsIter - n; return *this;}
        reverse_iterator& operator-=(difference_type n)     {itsIter += n; return *this;}
        reverse_iterator operator+(difference_type n) const {return reverse_iterator(itsIter - n);}
        reverse_iterator operator-(difference_type n) const {return reverse_iterator(itsIter + n);}
        //friend reverse_iterator operator + (difference_type n, const reverse_iterator& it) { return it-n; }
        difference_type operator - (const reverse_iterator& rhs) const { return rhs.base() - base(); }

        bool operator==(const reverse_iterator& r) const    {return itsIter == r.itsIter;}
        bool operator!=(const reverse_iterator& r) const    {return itsIter != r.itsIter;}
        bool operator<(const reverse_iterator& r) const     {return itsIter > r.itsIter;}
    private:
        iterator itsIter;
    };

    class const_reverse_iterator : public my_const_iterator_traits {
    public:
        typedef const T                             value_type;
        typedef const T&                            reference;
        typedef const value_type*                   pointer;
        typedef typename Container::difference_type difference_type;

        const_reverse_iterator() {}
        const_reverse_iterator(reverse_iterator i) : itsIter(i.base()) {}
        const_reverse_iterator(const_iterator i) : itsIter(i) {}
        const_iterator base() const {return itsIter;}

        reference operator*() const { return *(itsIter - 1); }
        pointer operator->() const  { return &(**this); }
        reference operator[](difference_type n) const {return *((*this) + n);}

        const_reverse_iterator& operator++()          {--itsIter; return *this;}
        const_reverse_iterator& operator--()          {++itsIter; return *this;}
        const_reverse_iterator operator++(int)        {const_reverse_iterator t(*this); --itsIter; return t;}
        const_reverse_iterator operator--(int)        {const_reverse_iterator t(*this); ++itsIter; return t;}
        const_reverse_iterator& operator+=(difference_type n)     {itsIter - n; return *this;}
        const_reverse_iterator& operator-=(difference_type n)     {itsIter += n; return *this;}
        const_reverse_iterator operator+(difference_type n) const {return const_reverse_iterator(itsIter - n);}
        const_reverse_iterator operator-(difference_type n) const {return const_reverse_iterator(itsIter + n);}
        //friend const_reverse_iterator operator + (difference_type n, const const_reverse_iterator& it) { return it-n; }
        difference_type operator - (const const_reverse_iterator& rhs) const { return rhs.base() - base(); }

        bool operator==(const const_reverse_iterator& r) const    {return itsIter == r.itsIter;}
        bool operator!=(const const_reverse_iterator& r) const    {return itsIter != r.itsIter;}
        bool operator< (const const_reverse_iterator& r) const     {return itsIter > r.itsIter;}
    private:
        const_iterator itsIter;
    };
#else
    typedef std::reverse_iterator<iterator> reverse_iterator;
    typedef std::reverse_iterator<const_iterator> const_reverse_iterator;
#endif

    SEQUENCE_OF(const void* info = &theInfo) : SEQUENCE_OF_Base(info) {  }
    SEQUENCE_OF(size_type n, const_reference val = T())
        : SEQUENCE_OF_Base(&theInfo) {
        insert(begin(), n, val);
    }
    template <class InputIterator>
    SEQUENCE_OF(InputIterator first, InputIterator last)
        : SEQUENCE_OF_Base(&theInfo) {
        insert(begin(), first, last);
    }


    SEQUENCE_OF(const SEQUENCE_OF<T, Constraint>& other) : SEQUENCE_OF_Base(other) {}


    SEQUENCE_OF<T, Constraint>& operator = (const SEQUENCE_OF<T, Constraint>& x) {
        SEQUENCE_OF<T, Constraint> temp(x.begin(), x.end());
        swap(temp);
        return *this;
    }
    void assign(size_type n) {
        SEQUENCE_OF<T, Constraint> temp(n);
        swap(temp);
    }
    void assign(size_type n, const T& value) {
        SEQUENCE_OF<T, Constraint> temp(n, value);
        swap(temp);
    }
    template <class InputIterator>
    void assign(InputIterator first, InputIterator last) {
        clear();
        insert(begin(), first, last);
    }
    iterator                begin() { return iterator(container.begin());   }
    const_iterator          begin() const { return const_iterator(container.begin());}
    iterator                end() { return iterator(container.end());}
    const_iterator          end() const { return const_iterator(container.end());}

    reverse_iterator        rbegin() { return reverse_iterator(end());}
    const_reverse_iterator  rbegin() const { return const_reverse_iterator(end());}
    reverse_iterator        rend() { return reverse_iterator(begin());}
    const_reverse_iterator  rend() const { return const_reverse_iterator(begin());}

    void resize(size_type sz) { Inherited::resize(sz); }
    void        resize(size_type sz, const T& c) {
        if (sz < size()) container.resize(sz, c);
        else insert(end(), sz - size(), c);
    }
    reference           operator[](size_type n) { return *static_cast<T*>(container.operator[](n));}
    const_reference     operator[](size_type n)  const { return *static_cast<const T*>(container.operator[](n));}
    reference           at(size_type n) { return *static_cast<T*>(container.at(n));}
    const_reference     at(size_type n) const { return *static_cast<const T*>(container.at(n));}
    reference           front() { return *static_cast<T*>(container.front());}
    const_reference     front() const { return *static_cast<const T*>(container.front());}
    reference           back() { return *static_cast<T*>(container.back());}
    const_reference     back() const { return *static_cast<const T*>(container.back());}
    void push_back(const T& x) { container.push_back(x.clone());}
    /**
     * Takes the ownership of the object pointed by \c x,
     * and insert it to the back of this object.
     */
    void push_back(pointer x) { container.push_back(x);}
    void pop_back() { clean(--end()); container.pop_back();}
    void push_front(const T& x) { container.insert(0, x.clone());}
    /**
     * Takes the ownership of the object pointed by \c x,
     * and insert it to the front of this object.
     */
    void push_front(pointer x) { container.insert(0, x);}
    void pop_front() { clean(begin()); container.erase(container.begin(), container.begin() + 1);}
    iterator    insert(iterator position, const T& x) { return iterator(container.insert(position.base(), x.clone()));}
    /**
     * Takes the ownership of the object pointed by \c x,
     * and insert it before the element pointed by \c position.
     */
    iterator    insert(iterator position, pointer x) { return iterator(container.insert(position.base(), x));}
    void insert(iterator position, size_type n, const T& x) { SEQUENCE_OF_Base::insert(position.base(), n, x);}
#if !defined(_MSC_VER)
    void insert(iterator position, const_iterator first, const_iterator last) {
        SEQUENCE_OF_Base::insert(position.base(), first.base(), last.base());
    }
#endif
    template <class InputIterator>
    void insert(iterator position, InputIterator first, InputIterator last) {
#if 1//defined(__GNUC__) && (__GNUC__ <= 2) 
        //Both VC6.0 SP4 and GNU C 2.95.2 has problem with finding the  operator - (const_iterator, const_iterator)
        // used by std::distance()
        std::transform(first, last, std::inserter(container, position.base()), create_from1());
        //for (; first != last; ++first) {
        //  container.push_back((*first)->clone());
        //}
#else
        difference_type dist = std::distance(container.begin(), position.base());
        reserve(size() + std::distance(first, last));
        std::transform(first, last, std::inserter(container, container.begin() + dist), create_from1());
#endif
    }
    iterator    erase(iterator position) { clean(position); return iterator(container.erase(position.base()));}
    iterator    erase(iterator first, iterator last) { clean(first, last); return iterator(container.erase(first.base(), last.base()));}
    void        swap(SEQUENCE_OF<T, Constraint>& x) { container.swap(x.container);}
    /**
     * Removes the last element from this object, and returns the ownership of the removed element
     * to the caller.
     */
    pointer release_back() { pointer x = static_cast<pointer>(container.back()); container.pop_back(); return x;}
    /**
     * Removes the first element from this object, and returns the ownership of the removed element
     * to the caller.
     */
    pointer release_front() { pointer x = static_cast<pointer>(container.front()); container.erase(container.begin(), container.begin() + 1); return x;}
    /**
     * Removes the element pointed by \c position from this object, and returns the ownership of the removed element
     * to the caller.
     */
    pointer release(iterator position) {
        pointer result = static_cast<pointer>(*(position.base()));
        container.erase(position.base());
        return result;
    }
    /**
     * Removes the elements in [first, last) from this object; furthermore, the elements been removed would not be
     * deleted. It is the caller's responsibility to obtain the ownerships of those elements before this operation
     * is executed.
     */
    void release(iterator first, iterator last) {
        container.erase(first.base(), last.base());
    }
    SEQUENCE_OF<T, Constraint>* clone() const { return static_cast<SEQUENCE_OF<T, Constraint>*>(do_clone()); }

    void remove(const_reference x)
    { remove_if(std::bind2nd(std::equal_to<T>(), x));    }

    template <class Predicate>  void remove_if(Predicate pred) {
        Container::iterator k = std::stable_partition(container.begin(), container.end(), std::not1(UnaryPredicate<Predicate>(pred)));
        erase(iterator(k), end());
    }

    void unique()
    { unique_if(std::equal_to<T>()); }

    template <class BinPred> void unique_if(BinPred pred) {
        if (container.size() < 2) return;
        BinaryPredicate<BinPred> pr(pred);
        Container::iterator k, first, last = container.end();
        k = first = container.begin();
        while (++first != last)
            if (!pr(*k, *first))
                if (++k != first)
                    std::swap(*first, *k);
        erase(iterator(++k), end());
    }


    void sort() { sort(std::less<T>());}

    template <class Compare> void sort(Compare comp)
    { std::sort(container.begin(), container.end(), ComparePredicate<Compare>(comp)); }

    void reverse() { std::reverse(container.begin(), container.end());}

    static const InfoType theInfo;
    static bool equal_type(const ASN1::AbstractData& type)
    {return type.info() == reinterpret_cast<const ASN1::AbstractData::InfoType*>(&theInfo);}

    static const std::string& typeName() {
        static std::string tn = std::string("SEQUENCE_OF<") + T::typeName() + ", ASN1::SizeConstraint<" +
                                std::to_string(Constraint::constraint_type) + ", " + std::to_string(Constraint::lower_bound) +
                                ", " + std::to_string(Constraint::upper_bound) + "> >";
        return tn;
    }

    virtual void fromVariant(const QVariant& variant) override {
        QVariantList list;
        if (variant.canConvert<QVariantList>())
            list = variant.toList();
        else
            throw ASN1BadVariantCast("bad cast to SEQUENCE_OF");

        resize(list.size());
        auto it = begin();
        for (auto list_it = list.begin(); list_it != list.end(); ++it, ++list_it)
            it->fromVariant(*list_it);
    }

private:
    void clean(iterator i)  {   delete &*i; }
    void clean(iterator first, iterator last)      { while (first != last) clean(first++); }


    struct create_from1 {
        AbstractData* operator()(const T& obj) const { return new T(obj); }
    };


    template <class OriginalPredicate>
    struct UnaryPredicate : public std::unary_function<AbstractData*, bool> {
        UnaryPredicate(OriginalPredicate& o) : pred(o) {}
        bool operator()(AbstractData* x) const { return pred(*static_cast<pointer>(x));}
        OriginalPredicate& pred;
    };

    template <class OriginalPredicate>
    struct BinaryPredicate : public std::binary_function<AbstractData*, AbstractData*, bool> {
        BinaryPredicate(OriginalPredicate& o) : pred(o) {}
        bool operator()(AbstractData* x, AbstractData* y) const { return pred(*static_cast<pointer>(x), *static_cast<pointer>(y));}
        OriginalPredicate& pred;
    };

    template <class OriginalPredicate>
    struct ComparePredicate : public std::binary_function<AbstractData*, AbstractData*, int> {
        ComparePredicate(OriginalPredicate& o) : pred(o) {}
        int operator()(AbstractData* x, AbstractData* y) const { return pred(*static_cast<pointer>(x), *static_cast<pointer>(y));}
        OriginalPredicate& pred;
    };

};


template <class T, class Constraint>
const typename SEQUENCE_OF<T, Constraint>::InfoType SEQUENCE_OF<T, Constraint>::theInfo = {
    SEQUENCE_OF_Base::create,
    0x10,
    Constraint::constraint_type,
    Constraint::lower_bound,
    Constraint::upper_bound,
    &T::theInfo
};

template <class T, class Constraint>
inline typename SEQUENCE_OF<T, Constraint>::iterator operator +
(typename SEQUENCE_OF<T, Constraint>::difference_type i,
 const typename SEQUENCE_OF<T, Constraint>::iterator& it) {
    return it + i;
}

template <class T, class Constraint>
inline typename SEQUENCE_OF<T, Constraint>::const_iterator operator +
(typename SEQUENCE_OF<T, Constraint>::difference_type i,
 const typename SEQUENCE_OF<T, Constraint>::const_iterator& it) {
    return it + i;
}

template <class T, class Constraint>
inline typename SEQUENCE_OF<T, Constraint>::reverse_iterator operator +
(typename SEQUENCE_OF<T, Constraint>::difference_type i,
 const typename SEQUENCE_OF<T, Constraint>::reverse_iterator& it) {
    return it + i;
}

template <class T, class Constraint>
inline typename SEQUENCE_OF<T, Constraint>::const_reverse_iterator operator +
(typename SEQUENCE_OF<T, Constraint>::difference_type i,
 const typename SEQUENCE_OF<T, Constraint>::const_reverse_iterator& it) {
    return it + i;
}

template <class T, class Constraint = EmptyConstraint>
class SET_OF : public SEQUENCE_OF<T, Constraint> {
    typedef SEQUENCE_OF<T, Constraint>  Inherited;
    SET_OF(const void* info) : Inherited(info) {  }
public:
    typedef typename Inherited::size_type size_type;
    typedef typename Inherited::const_reference const_reference;
protected:
    typedef typename Inherited::InfoType InfoType;
public:
    SET_OF() {  }
    SET_OF(size_type n, const_reference val = T())
        : Inherited(&theInfo)
    {   insert(Inherited::begin(), n, val);     }

    template <class InputIterator>
    SET_OF(InputIterator first, InputIterator last)
        : Inherited(&theInfo)
    {   insert(Inherited::begin(), first, last);  }


    SET_OF(const SET_OF<T, Constraint>& other) : Inherited(other) {}
    SET_OF<T, Constraint>& operator = (const SET_OF<T, Constraint>& x)  {
        SET_OF<T, Constraint> temp(x.begin(), x.end());
        swap(temp);
        return *this;
    }
    SET_OF<T, Constraint>* clone() const { return static_cast<SET_OF<T, Constraint>*>(Inherited::clone()); }

    static const InfoType theInfo;
    static bool equal_type(const ASN1::AbstractData& type)
    {return type.info() == reinterpret_cast<const ASN1::AbstractData::InfoType*>(&theInfo);}

};

template <class T, class Constraint>
const typename SET_OF<T, Constraint>::InfoType SET_OF<T, Constraint>::theInfo = {
    SEQUENCE_OF_Base::create,
    0x11,
    Constraint::constraint_type,
    Constraint::lower_bound,
    Constraint::upper_bound,
    &T::theInfo
};


typedef std::vector<char> OpenBuf;

class OpenData : public AbstractData, public detail::Allocator<OpenData> {
public:
    typedef AbstractData data_type;

    OpenData(const void* info = &theInfo) : AbstractData(info) {}
    OpenData(AbstractData* pData, const void* info = &theInfo)
        : AbstractData(info), data(pData) {}
    OpenData(OpenBuf* pBuf)
        :  AbstractData(&theInfo), buf(pBuf) {}

    OpenData(const AbstractData& aData, const void* info = &theInfo)
        : AbstractData(info), data(aData.clone()) {}
    OpenData(const OpenBuf& aBuf)
        :  AbstractData(&theInfo), buf(new OpenBuf(aBuf)) {}

    OpenData(const OpenData& that);

    OpenData& operator = (const OpenData& that) { OpenData tmp(that);   swap(tmp); return *this; }
    OpenData& operator = (const AbstractData& aData) { data.reset(aData.clone()); return *this; }
    OpenData& operator = (const OpenBuf& aBuf) { buf.reset(new OpenBuf(aBuf)); return *this; }

    void grab(AbstractData* aData) { data.reset(aData); }
    void grab(OpenBuf* aBuf) { buf.reset(aBuf); }

    AbstractData* release_data() { return data.release();}
    OpenBuf* release_buf() { return buf.release();}

    bool isEmpty() const { return !has_data() && !has_buf(); }
    bool has_data() const {return data.get() != NULL ;}
    bool has_buf() const { return buf.get() != NULL; }

    AbstractData& get_data() { return *data;}
    const AbstractData& get_data() const { return *data;}
    OpenBuf& get_buf() { return *buf; }
    const OpenBuf& get_buf() const { return *buf; }

    bool isValid() const;
    bool isStrictlyValid() const;

    OpenData* clone() const { return static_cast<OpenData*>(do_clone()); }
    static AbstractData* create(const void* info);
    void swap(OpenData& other);

    bool operator == (const OpenData& rhs) const { return do_compare(rhs) == 0; }
    bool operator != (const OpenData& rhs) const { return do_compare(rhs) != 0; }
    bool operator < (const OpenData& rhs) const { return do_compare(rhs) <  0; }
    bool operator > (const OpenData& rhs) const { return do_compare(rhs) >  0; }
    bool operator <= (const OpenData& rhs) const { return do_compare(rhs) <= 0; }
    bool operator >= (const OpenData& rhs) const { return do_compare(rhs) >= 0; }

    ASN1_EXPORT static const InfoType theInfo;
    static bool equal_type(const AbstractData& type)
    {return type.info() == reinterpret_cast<const AbstractData::InfoType*>(&theInfo);}

    virtual QVariant toVariant() const override;
    virtual void fromVariant(const QVariant& variant) override;

    static const std::string& typeName() { static std::string tn = "OpenData"; return tn; }

protected:
    std::unique_ptr<AbstractData> data;
    std::unique_ptr<OpenBuf> buf;
private:
    virtual int do_compare(const AbstractData& other) const;
    virtual AbstractData* do_clone() const ;
    virtual bool do_accept(ConstVisitor&) const;
    virtual bool do_accept(Visitor&);
};

class TypeConstrainedOpenData : public OpenData {
private:
    virtual bool do_accept(Visitor& v);
protected:
    TypeConstrainedOpenData(AbstractData* pData, const void* info) :  OpenData(pData, info) {}
    TypeConstrainedOpenData(const AbstractData& aData, const void* info) : OpenData(aData, info) {}
    static AbstractData* create(const void* info);

    struct InfoType {
        CreateFun create;
        unsigned tag;
        const void* typeInfo;
    };
};

template <class T>
class Constrained_OpenData : public TypeConstrainedOpenData {
    typedef T data_type;
    typedef TypeConstrainedOpenData Inherited;
protected:
    typedef Inherited::InfoType InfoType;
public:
    Constrained_OpenData(const void* info = &TypeConstrainedOpenData::theInfo)
        : TypeConstrainedOpenData(T::create(&T::theInfo), info) {

        assert(static_cast<const InfoType*>(info_)->typeInfo != info);
    }
    Constrained_OpenData(const T& t)
        : TypeConstrainedOpenData(t, &TypeConstrainedOpenData::theInfo) {}

    Constrained_OpenData(const Constrained_OpenData<T>& that): TypeConstrainedOpenData(that) {}

    Constrained_OpenData<T>& operator = (const data_type& aData) { data.reset(aData.clone()); return *this; }
    Constrained_OpenData<T>& operator = (const Constrained_OpenData<T>& that) { Constrained_OpenData<T> tmp(that);  swap(tmp); return *this; }
    Constrained_OpenData<T>& operator = (const OpenBuf& aBuf) { buf.reset(new OpenBuf(aBuf)); return *this;}

    void grab(T* aData) { data.reset(aData); }
    T& get_data() { return static_cast<T&>(OpenData::get_data()); }
    const T& get_data() const { return static_cast<const T&>(OpenData::get_data());}

    Constrained_OpenData<T>* clone() const { return static_cast<Constrained_OpenData<T>*>(OpenData::clone()); }
    void swap(Constrained_OpenData<T>& other) { OpenData::swap(other); }

    static const InfoType theInfo;
    static bool equal_type(const ASN1::AbstractData& type)
    {return type.info() == reinterpret_cast<const ASN1::AbstractData::InfoType*>(&theInfo);}
private:
    const InfoType* info() const { return static_cast<const InfoType*>(info_); }
};

template <class T>
const typename Constrained_OpenData<T>::InfoType Constrained_OpenData<T>::theInfo = {
    TypeConstrainedOpenData::create,
    0,
    &T::theInfo
};


//////////////////////////////////////////////////////////////////////////////
class CoderEnv;

/**
 * Visitor is base class for defining a new mutable operation for ASN.1 types without
 * changing the classes of the ASN.1 types it operates.
 *
 * This class is the role "Visitor" in the Visitor design pattern (Gamma et al. 1995).
 * It allows us to add a new operation to the ASN.1 types without changing the classes
 * of the ASN.1 types; furthermore, it avoids linking the object code of the new operation
 * when it is not used, which is why this library can be so small and lean.
 *
 * This class \c Visitor differs from the class \c ConstVistor in that a \c Visitor operates on mutable
 * objects; in other words, it will change the value of the ASN.1 objects it operates on. For
 * example, \c BERDecoder and \c PERDecoder ,which both inherit from \c Visitor, are used to decode
 * a byte stream into a ASN.1 value; that is to say, they modify the ASN.1 objects they visit to
 * some specific values.
 *
 * All classes inherit from shall override the do_visit() member functions, which are called
 * by their corresponding visit() member functions. The do_visit() member functions are all
 * declared private for they should not be called by their derivatives.
 *
 * @see BERDecoder, PERDecoder, AVNDecoder
 */

class Visitor {
public:
    virtual ~Visitor() {}
    bool visit(Null& value) { return do_visit(value); }
    bool visit(BOOLEAN& value) { return do_visit(value); }
    bool visit(INTEGER& value) { return do_visit(value); }
    bool visit(IntegerWithNamedNumber& value) { return do_visit(value); }
    bool visit(ENUMERATED& value) { return do_visit(value); }
    bool visit(OBJECT_IDENTIFIER& value) { return do_visit(value); }
    bool visit(OCTET_STRING& value)  { return do_visit(value); }
    bool visit(BIT_STRING& value) { return do_visit(value); }
    bool visit(AbstractString& value) { return do_visit(value); }
    bool visit(BMPString& value) { return do_visit(value); }
    bool visit(CHOICE& value) { return do_visit(value); }
    bool visit(SEQUENCE_OF_Base& value) { return do_visit(value); }
    bool visit(OpenData& value) { return do_visit(value); }
    bool revisit(OpenData& value) { return do_revisit(value); }
    bool visit(TypeConstrainedOpenData& value) { return do_visit(value); }
    bool visit(GeneralizedTime& value) { return do_visit(value); }
    bool visit(SEQUENCE& value) ;

    CoderEnv* get_env() { return env;}

    enum VISIT_SEQ_RESULT {
        FAIL,
        STOP,
        NO_EXTENSION,
        CONTINUE
    };
protected:
    Visitor(CoderEnv* coder) : env(coder) {}

private:
    virtual bool do_visit(Null& value) = 0;
    virtual bool do_visit(BOOLEAN& value) = 0;
    virtual bool do_visit(INTEGER& value) = 0;
    virtual bool do_visit(IntegerWithNamedNumber& value) { return visit(static_cast<INTEGER&>(value));}
    virtual bool do_visit(ENUMERATED& value) = 0;
    virtual bool do_visit(OBJECT_IDENTIFIER& value) = 0;
    virtual bool do_visit(OCTET_STRING& value) = 0;
    virtual bool do_visit(BIT_STRING& value) = 0;
    virtual bool do_visit(AbstractString& value) = 0;
    virtual bool do_visit(BMPString& value) = 0;
    virtual bool do_visit(CHOICE& value) = 0;
    virtual bool do_visit(SEQUENCE_OF_Base& value) = 0;
    virtual bool do_visit(OpenData& value) = 0;
    virtual bool do_revisit(OpenData& value) = 0;
    virtual bool do_visit(TypeConstrainedOpenData& value) = 0;
    virtual bool do_visit(GeneralizedTime& value) = 0;

    /**
     * Called by \c visit() before visiting any component in the \c SEQUENCE.
     *
     * @param value The \c SEQUENCE been visited.
     * @return If the return value is FAIL, it indicates the operation is fail and stop visiting
     *         subsequent components; otherwise, it means the operation is successful. However,
     *         different return values indicate how the sebsequent components are traversed. STOP means
     *         no sebsequent components should be visited; NO_EXTENSION means only extension roots
     *         should be visited; CONTINUE means all components should be visited.
     */
    virtual VISIT_SEQ_RESULT preVisitExtensionRoots(SEQUENCE& value) { return CONTINUE; }
    /**
     * Called by \c visit() while visiting a component in the extension root of
     * the \c SEQUENCE.
     *
     * @param value The \c SEQUENCE been visited.
     * @param pos The position of the component in the SEQUENCE.
     * @param optional_id The optional index of the component in the SEQUENCE, the value is
     *        -1 when it is a mandatory component.
     * @return If the return value is FAIL, it indicates the operation is fail and stop visiting
     *         subsequent components; otherwise, it means the operation is successful. However,
     *         different return values indicate how the sebsequent components are traversed. STOP means
     *         no sebsequent components should be visited; CONTINUE means all components should be visited.
     */
    virtual VISIT_SEQ_RESULT visitExtensionRoot(SEQUENCE& value, int pos, int optional_id) { return FAIL;}
    /**
     * Called by \c visit() before visiting any extension component of the \c SEQUENCE.
     *
     * @param value The \c SEQUENCE been visited.
     * @return If the return value is FAIL, it indicates the operation is fail and stop visiting
     *         subsequent components; otherwise, it means the operation is successful. However,
     *         different return values indicate how the sebsequent components are traversed. STOP means
     *         no sebsequent components should be visited; CONTINUE means all components should be visited.
     */
    virtual VISIT_SEQ_RESULT preVisitExtensions(SEQUENCE& value) { return CONTINUE;}
    /**
     * Called by \c visit() when visiting a component which locates after the extension mark of
     * a \c SEQUENCE.
     *
     * @param value The \c SEQUENCE been visited.
     * @param pos The position of the component in the SEQUENCE.
     * @param optional_id The optional index of the component in the SEQUENCE, the value is
     *        -1 when it is a madatory component.
     * @return If the return value is FAIL, it indicates the operation is fail and stop visiting
     *         subsequent components; otherwise, it means the operation is successful. However,
     *         different return values indicate how the sebsequent components are traversed. STOP means
     *         no sebsequent components should be visited; CONTINUE means all components should be visited.
     */
    virtual VISIT_SEQ_RESULT visitKnownExtension(SEQUENCE& value, int pos, int optional_id) { return FAIL;}
    /**
     * Called by \c visit() after visiting all known extension components of the \c SEQUENCE.
     *
     * @param value The \c SEQUENCE been visited.
     * @return true if the operation is successful.
     */
    virtual bool visitUnknownExtensions(SEQUENCE& value) { return true;}
    CoderEnv* env;
};

/**
 * ConstVisitor is base class for defining a new non-mutable operation for ASN.1 types without
 * changing the classes of the ASN.1 types it operates.
 *
 * This class is the role "Visitor" in the Visitor design pattern (Gamma et al. 1995).
 * It allows us to add a new operation to the ASN.1 types without changing the classes
 * of the ASN.1 types; furthermore, it avoids linking the object code of the new operation
 * when it is not used, which is why this library can be so small and lean.
 *
 * This class \c ConstVistor differs from the class \c Visitor in that a \c ConstVistor operates
 * on non-mutable objects; in other words, it cannot change the value of the ASN.1 objects it
 * operates on. For example, \c BEREncoder and \c PEREncoder ,which both inherit from
 * \c ConstVisitor, are used to encode an ASN.1 value into a byte stream; that is to say, the
 * \c BEREncoder and PEREncoder do not modify the ASN.1 objects they visit.
 *
 * All classes inherit from shall override the do_visit() member functions, which are called
 * by their corresponding visit() member functions. The do_visit() member functions are all
 * declared private for they should not be called by their derivatives.
 *
 *@see BEREncoder, PEREncoder, AVNEncoder
 */
class ConstVisitor {
public:
    virtual ~ConstVisitor() {}
    bool visit(const Null& value) { return do_visit(value); }
    bool visit(const BOOLEAN& value) { return do_visit(value);}
    bool visit(const INTEGER& value) { return do_visit(value); }
    bool visit(const IntegerWithNamedNumber& value) { return do_visit(value);}
    bool visit(const ENUMERATED& value) { return do_visit(value); }
    bool visit(const OBJECT_IDENTIFIER& value) { return do_visit(value); }
    bool visit(const BIT_STRING& value) { return do_visit(value); }
    bool visit(const OCTET_STRING& value) { return do_visit(value); }
    bool visit(const AbstractString& value) { return do_visit(value); }
    bool visit(const BMPString& value) { return do_visit(value); }
    bool visit(const CHOICE& value) { return do_visit(value); }
    bool visit(const OpenData& value) { return do_visit(value); }
    bool visit(const GeneralizedTime& value) { return do_visit(value); }
    bool visit(const SEQUENCE_OF_Base& value) {return do_visit(value); }
    bool visit(const SEQUENCE& value) ;
private:
    virtual bool do_visit(const AbstractData& value) { return true; }
    virtual bool do_visit(const Null& value) { return do_visit(static_cast<const AbstractData&>(value)); }
    virtual bool do_visit(const BOOLEAN& value) { return do_visit(static_cast<const AbstractData&>(value)); }
    virtual bool do_visit(const INTEGER& value) { return do_visit(static_cast<const AbstractData&>(value)); }
    virtual bool do_visit(const IntegerWithNamedNumber& value) { return do_visit(static_cast<const INTEGER&>(value));}
    virtual bool do_visit(const ENUMERATED& value) { return do_visit(static_cast<const AbstractData&>(value)); }
    virtual bool do_visit(const OBJECT_IDENTIFIER& value) { return do_visit(static_cast<const AbstractData&>(value)); }
    virtual bool do_visit(const BIT_STRING& value) { return do_visit(static_cast<const AbstractData&>(value)); }
    virtual bool do_visit(const OCTET_STRING& value) { return do_visit(static_cast<const AbstractData&>(value)); }
    virtual bool do_visit(const AbstractString& value) { return do_visit(static_cast<const AbstractData&>(value)); }
    virtual bool do_visit(const BMPString& value) { return do_visit(static_cast<const AbstractData&>(value)); }
    virtual bool do_visit(const CHOICE& value) { return do_visit(static_cast<const AbstractData&>(value)); }
    virtual bool do_visit(const OpenData& value) { return do_visit(static_cast<const AbstractData&>(value)); }
    virtual bool do_visit(const GeneralizedTime& value) { return do_visit(static_cast<const AbstractData&>(value)); }
    virtual bool do_visit(const SEQUENCE_OF_Base& value) { return do_visit(static_cast<const AbstractData&>(value)); }

    virtual bool preVisitExtensionRoots(const SEQUENCE& value) { return do_visit(static_cast<const AbstractData&>(value)); }
    virtual bool visitExtensionRoot(const SEQUENCE& value, int index) { return false; }
    virtual bool preVisitExtensions(const SEQUENCE&) { return true;}
    virtual bool visitKnownExtension(const SEQUENCE& value, int index) { return false; }
    virtual bool afterVisitSequence(const SEQUENCE&) { return true;}
};

/////////////////////////////////////////////////////////////////////////////////////

/**
 * This class is used to encode an ASN.1 object into a byte stream using Basic Encoding Rule (BER),
 * defined in ITU X.690.
 *
 * The following example shows how to use this class.
 * \code
 *    ASN1::INGEGER obj(5);
 *    ASN1::OpenBuf buf; // the encoded buffer
 *    ASN1::BEREncoder encoder(buf);
 *
 *    obj.accept(encoder); // now buf contains the encoded BER stream.
 * \endcode
 *
 * @see BERDecoder
 */
class BEREncoder : public ConstVisitor {
public:
    /**
     * Constructor.
     *
     * @param buf The buffer used to holds the encoded BER stream.
     */
    BEREncoder(OpenBuf& buf)
        : encodedBuffer(buf), tag(0xffffffff) {
        encodedBuffer.clear();
        encodedBuffer.reserve(256);
    }

    void encodeHeader(const AbstractData& obj);

private:
    virtual bool do_visit(const Null& value);
    virtual bool do_visit(const BOOLEAN& value);
    virtual bool do_visit(const INTEGER& value);
    virtual bool do_visit(const ENUMERATED& value);
    virtual bool do_visit(const OBJECT_IDENTIFIER& value);
    virtual bool do_visit(const BIT_STRING& value);
    virtual bool do_visit(const OCTET_STRING& value);
    virtual bool do_visit(const AbstractString& value);
    virtual bool do_visit(const BMPString& value);
    virtual bool do_visit(const CHOICE& value);
    virtual bool do_visit(const SEQUENCE_OF_Base& value);
    virtual bool do_visit(const OpenData& value);
    virtual bool do_visit(const GeneralizedTime& value);

    virtual bool preVisitExtensionRoots(const SEQUENCE& value) ;
    virtual bool visitExtensionRoot(const SEQUENCE& value, int index);
    virtual bool visitKnownExtension(const SEQUENCE& value, int index);

    void encodeByte(unsigned value);
    void encodeBlock(const char* bufptr, unsigned nBytes);
    OpenBuf& encodedBuffer;
    unsigned tag;
};

/**
 * This class is used to decode a byte stream encoded using Basic Encoding Rule (BER) back
 * to an ASN.1 object.
 *
 * The following example shows how to use this class.
 * \code
 *    ASN1::INGEGER obj;
 *    ASN1::BERDecoder decoder(buf, buf+5);
 *
 *    obj.accept(decoder); // now obj holds the value which the buf represents.
 * \endcode
 *
 * @see BEREncoder
 */

class BERDecoder  : public Visitor {
public:
    /**
     * Constructor
     *
     * @param first The start position of the encoded BER stream.
     * @param last  The end position of the encoded BER stream.
     * @param coder The CoderEnv object which the decoder used to find the information objects
     *  it needs to decode the ASN.1 object. If this parameter is not NULL, the decoder will
     *  decode the open type based on the information objects which are inserted to the CoderEnv
     *  objects.
     */
    BERDecoder(const char* first, const char* last, CoderEnv* coder = NULL)
        : Visitor(coder)
        , beginPosition(first)
        , endPosition(last)
        , dontCheckTag(0)
    {}

    typedef const char* memento_type;
    memento_type get_memento() const { return beginPosition; }
    void rollback(memento_type memento) { beginPosition = memento;}
    bool decodeChoicePreamle(CHOICE& value, memento_type& nextPostion);

    bool decodeHeader(unsigned& tag,
                      bool& primitive,
                      unsigned& len);
    bool decodeHeader(AbstractData& obj, unsigned& len);

    virtual VISIT_SEQ_RESULT preVisitExtensionRoots(SEQUENCE& value);
    virtual VISIT_SEQ_RESULT visitExtensionRoot(SEQUENCE& value, int index, int optional_id);
    virtual VISIT_SEQ_RESULT visitKnownExtension(SEQUENCE& value, int index, int optional_id);
    virtual bool visitUnknownExtensions(SEQUENCE& value);

private:
    virtual bool do_visit(Null& value);
    virtual bool do_visit(BOOLEAN& value);
    virtual bool do_visit(INTEGER& value);
    virtual bool do_visit(ENUMERATED& value);
    virtual bool do_visit(OBJECT_IDENTIFIER& value);
    virtual bool do_visit(BIT_STRING& value);
    virtual bool do_visit(OCTET_STRING& value);
    virtual bool do_visit(AbstractString& value);
    virtual bool do_visit(BMPString& value);
    virtual bool do_visit(CHOICE& value);
    virtual bool do_visit(SEQUENCE_OF_Base& value);
    virtual bool do_visit(OpenData& value);
    virtual bool do_revisit(OpenData& value);
    virtual bool do_visit(TypeConstrainedOpenData& value);
    virtual bool do_visit(GeneralizedTime& value);

    bool atEnd();
    unsigned char decodeByte();
    unsigned decodeBlock(char* bufptr, unsigned nBytes);

    const char* beginPosition;
    const char* endPosition;
    std::vector<const char*> endSEQUENCEPositions;
    int dontCheckTag;
};

/**
 * This class is used to encode an ASN.1 object into a byte stream using Packed Encoding Rule (PER),
 * defined in ITU X.691.
 *
 * The following example shows how to use this class.
 * \code
 *    ASN1::INGEGER obj(5);
 *    ASN1::OpenBuf buf; // the encoded buffer
 *    ASN1::PEREncoder encoder(buf);
 *
 *    obj.accept(encoder); // now buf contains the encoded PER stream.
 * \endcode
 *
 * @see PERDecoder
 */

class PEREncoder : public ConstVisitor {
public:
    /**
     * Constructor
     *
     * @param buf The buffer used to holds the encoded PER stream.
     * @param isAligned Indicates whether using the aligned PER.
     *
     * @warning The unaligned PER has never been fully tested in current version.
     */
    PEREncoder(OpenBuf& buf, bool isAligned = true)
        : encodedBuffer(buf)
        , bitOffset(8)
        , alignedFlag(isAligned) {
        encodedBuffer.clear();
        encodedBuffer.reserve(256);
    }


    /**
     * Returns true if using the aligned PER.
     */
    bool aligned() const { return alignedFlag; }

private:
    virtual bool do_visit(const Null& value);
    virtual bool do_visit(const BOOLEAN& value);
    virtual bool do_visit(const INTEGER& value);
    virtual bool do_visit(const ENUMERATED& value);
    virtual bool do_visit(const OBJECT_IDENTIFIER& value);
    virtual bool do_visit(const BIT_STRING& value);
    virtual bool do_visit(const OCTET_STRING& value);
    virtual bool do_visit(const AbstractString& value);
    virtual bool do_visit(const BMPString& value);
    virtual bool do_visit(const CHOICE& value);
    virtual bool do_visit(const OpenData& value);
    virtual bool do_visit(const GeneralizedTime& value);
    virtual bool do_visit(const SEQUENCE_OF_Base& value);

    virtual bool preVisitExtensionRoots(const SEQUENCE& value) ;
    virtual bool visitExtensionRoot(const SEQUENCE& value, int index);
    virtual bool preVisitExtensions(const SEQUENCE& value) ;
    virtual bool visitKnownExtension(const SEQUENCE& value, int index);

    void encodeBitMap(const std::vector<char>& bitData, unsigned nBits);
    void encodeMultiBit(unsigned value, unsigned nBits);
    bool encodeConstrainedLength(const ConstrainedObject& obj, unsigned length) ;
    bool encodeConstraint(const ConstrainedObject& obj, unsigned value) ;
    void encodeSingleBit(bool value);
    void encodeSmallUnsigned(unsigned value);
    bool encodeLength(unsigned len, unsigned lower, unsigned upper);
    bool encodeUnsigned(unsigned value, unsigned lower, unsigned upper);
    bool encodeAnyType(const AbstractData*);

    void byteAlign();
    void encodeByte(unsigned value);
    void encodeBlock(const char* bufptr, unsigned nBytes);
    OpenBuf& encodedBuffer;
    unsigned short bitOffset;
    bool alignedFlag;
};

/**
 * This class is used to decode a byte stream encoded using Packed Encoding Rule (PER) back
 * to an ASN.1 object.
 *
 * The following example shows how to use this class.
 * \code
 *    ASN1::INGEGER obj;
 *    ASN1::PERDecoder encoder(buf, buf+5);
 *
 *    obj.accept(encoder); // now obj holds the value which the buf represents.
 * \endcode
 *
 * @see PEREncoder
 */

class PERDecoder  : public Visitor {
public:
    /**
     * Constructor
     *
     * @param first The start position of the encoded BER stream.
     * @param last  The end position of the encoded BER stream.
     * @param coder The CoderEnv object which the decoder used to find the information objects
     *  it needs to decode the ASN.1 object. If this parameter is not NULL, the decoder will
     *  decode the open type based on the information objects which are inserted to the CoderEnv
     *  objects.
     * @param isAligned Indicates whether using the aligned PER.
     *
     * @warning The unaligned PER has never been fully tested in current version.
     */
    PERDecoder(const char* first, const char* last, CoderEnv* coder = NULL, bool isAligned = true)
        : Visitor(coder)
        , beginPosition(first)
        , endPosition(last)
        , bitOffset(8)
        , alignedFlag(isAligned) {}

    struct memento_type {
        memento_type(const char* bytePos = 0, unsigned bitPos = 0)
            : bytePosition(bytePos), bitPosition(bitPos) {}
        const char* bytePosition;
        unsigned bitPosition;
    };

    memento_type get_memento() const { return memento_type(beginPosition, bitOffset); }
    void rollback(memento_type memento) {
        if (memento.bytePosition != 0) {
            beginPosition = (memento.bytePosition > endPosition ?
                             endPosition : memento.bytePosition);
            bitOffset   = memento.bitPosition;
        }
    }

    bool aligned() const { return alignedFlag; }

    bool decodeChoicePreamle(CHOICE& value, memento_type& nextPostion);
    const char* getPosition() const { return beginPosition; }
    const char* getNextPosition() const { return beginPosition + (bitOffset != 8 ? 1 : 0); }
    void setPosition(const char* newPos);
    int decodeConstrainedLength(ConstrainedObject& obj, unsigned& length);
    int decodeLength(unsigned lower, unsigned upper, unsigned& len);

    virtual VISIT_SEQ_RESULT preVisitExtensionRoots(SEQUENCE& value);
    virtual VISIT_SEQ_RESULT visitExtensionRoot(SEQUENCE& value, int index, int optional_id);
    virtual VISIT_SEQ_RESULT preVisitExtensions(SEQUENCE& value);
    virtual VISIT_SEQ_RESULT visitKnownExtension(SEQUENCE& value, int index, int optional_id);
    virtual bool visitUnknownExtensions(SEQUENCE& value);

private:

    virtual bool do_visit(Null& value);
    virtual bool do_visit(BOOLEAN& value);
    virtual bool do_visit(INTEGER& value);
    virtual bool do_visit(ENUMERATED& value);
    virtual bool do_visit(OBJECT_IDENTIFIER& value);
    virtual bool do_visit(BIT_STRING& value);
    virtual bool do_visit(OCTET_STRING& value);
    virtual bool do_visit(AbstractString& value);
    virtual bool do_visit(BMPString& value);
    virtual bool do_visit(CHOICE& value);
    virtual bool do_visit(SEQUENCE_OF_Base& value);
    virtual bool do_visit(OpenData& value);
    virtual bool do_revisit(OpenData& value);
    virtual bool do_visit(TypeConstrainedOpenData& value);
    virtual bool do_visit(GeneralizedTime& value);

    void byteAlign();
    bool atEnd() ;
    unsigned getBitsLeft() const;
    bool decodeSingleBit();
    bool decodeMultiBit(unsigned nBits, unsigned& value);
    bool decodeSmallUnsigned(unsigned& value);
    int decodeUnsigned(unsigned lower, unsigned upper, unsigned& value);
    unsigned decodeBlock(char* bufptr, unsigned nBytes);
    bool decodeBitMap(std::vector<char>& bitData, unsigned nBit);

    const char* beginPosition;
    const char* endPosition;
    unsigned short bitOffset;
    bool alignedFlag;
};

#ifdef ASN1_HAS_IOSTREAM

/**
 * This class is used to encode an ASN.1 object into a byte stream using Abstract Value Notation,
 * defined in ITU X.680.
 *
 * The following example shows how to use this class.
 * \code
 *    ASN1::INGEGER obj(5);
 *    ASN1::AVNEncoder encoder(std::cout);
 *
 *    obj.accept(encoder); // now the object is printed to std::cout.
 * \endcode
 *
 * @see AVNDecoder
 */

class AVNEncoder : public ConstVisitor {
public:
    AVNEncoder(std::ostream& os)
        : strm(os), indent(0) {}
private:

    virtual bool do_visit(const Null& value);
    virtual bool do_visit(const BOOLEAN& value);
    virtual bool do_visit(const INTEGER& value);
    virtual bool do_visit(const IntegerWithNamedNumber& value);
    virtual bool do_visit(const ENUMERATED& value);
    virtual bool do_visit(const OBJECT_IDENTIFIER& value);
    virtual bool do_visit(const BIT_STRING& value);
    virtual bool do_visit(const OCTET_STRING& value);
    virtual bool do_visit(const AbstractString& value);
    virtual bool do_visit(const BMPString& value);
    virtual bool do_visit(const CHOICE& value);
    virtual bool do_visit(const OpenData& value);
    virtual bool do_visit(const GeneralizedTime& value);
    virtual bool do_visit(const SEQUENCE_OF_Base& value);

    virtual bool preVisitExtensionRoots(const SEQUENCE& value) ;
    virtual bool visitExtensionRoot(const SEQUENCE& value, int index);
    virtual bool visitKnownExtension(const SEQUENCE& value, int index);
    virtual bool afterVisitSequence(const SEQUENCE& value);

    std::ostream& strm;
    int indent;
    std::vector<bool> outputSeparators; // used to indicate whether to output separator while parsing SEQUENCE
};

/**
 * This class is used to decode an Abstract Value Notation encoded byte stream
 * to an ASN.1 object.
 *
 * The following example shows how to use this class.
 * \code
 *    ASN1::INGEGER obj;
 *    ASN1::AVNDecoder encoder(std::cin);
 *
 *    obj.accept(encoder); // now obj holds the value which it read from std::cin.
 * \endcode
 *
 * @see PEREncoder
 */
class AVNDecoder  : public Visitor {
public:
    AVNDecoder(std::istream& is, CoderEnv* coder = NULL)
        : Visitor(coder), strm(is) {}

private:
    virtual bool do_visit(Null& value);
    virtual bool do_visit(BOOLEAN& value);
    virtual bool do_visit(INTEGER& value);
    virtual bool do_visit(IntegerWithNamedNumber& value);
    virtual bool do_visit(ENUMERATED& value);
    virtual bool do_visit(OBJECT_IDENTIFIER& value);
    virtual bool do_visit(BIT_STRING& value);
    virtual bool do_visit(OCTET_STRING& value);
    virtual bool do_visit(AbstractString& value);
    virtual bool do_visit(BMPString& value);
    virtual bool do_visit(CHOICE& value);
    virtual bool do_visit(SEQUENCE_OF_Base& value);
    virtual bool do_visit(OpenData& value);
    virtual bool do_revisit(OpenData& value);
    virtual bool do_visit(TypeConstrainedOpenData& value);
    virtual bool do_visit(GeneralizedTime& value);

    virtual VISIT_SEQ_RESULT preVisitExtensionRoots(SEQUENCE& value);
    virtual VISIT_SEQ_RESULT visitExtensionRoot(SEQUENCE& value, int index, int optional_id);
    virtual VISIT_SEQ_RESULT visitKnownExtension(SEQUENCE& value, int index, int optional_id);
    virtual bool visitUnknownExtensions(SEQUENCE& value);

    std::istream& strm;
    std::vector<std::string> identifiers; // used to indicate the last parsed idetifier while parsing SEQUENCE.
};


/**
 * The trace_invalid() is used to find the first component of a complex
 * ASN1::AbstactData object which doesn't satisfy its own constraint. It
 * output a string which describe where the AbstractData object is invalid
 * to the \c os.
 *
 * If the ASN1::AbstactData object is entirely valid, no string is outputted to \c os.
 *
 * @param os The stream where the result is outputted.
 * @param str The string to be inserted in the front of the result when the first
 *       invalid component is found.
 * @param data The ASN1::AbstactData object to be check for validity.
 * @return true if the ASN1::AbstactData object is valid.
 */

bool trace_invalid(std::ostream& os, const char* str, const AbstractData& data);

#endif // ASN1_HAS_IOSTREAM
///////////////////////////////////////////////////////////////////////////

class Module {
public:
    virtual ~Module() {};
    const char* name() { return moduleName; }
protected:
    const char* moduleName;
};

/////////////////////////////////////////////////////////////////////////////

class CoderEnv {
public:

    Module* find(const char* moduleName) {
        Modules::iterator i = modules.find(moduleName);
        return i != modules.end() ? i->second : NULL;
    }
    void insert(Module* module) { assert(module); modules[module->name()] = module;}
    void erase(Module* module) { assert(module); modules.erase(module->name()); }
    void erase(const char* moduleName) { modules.erase(moduleName); }

    enum EncodingRules { avn,
                         ber,
                         per_Basic_Aligned
                       };

    EncodingRules get_encodingRule() const { return encodingRule;}
    void set_encodingRule(EncodingRules rule) { encodingRule = rule; }
    void set_avn() {set_encodingRule(avn);}
    bool is_avn() const { return encodingRule == avn;}
    void set_ber() {set_encodingRule(ber);}
    bool is_ber() const { return encodingRule == ber;}
    void set_per_Basic_Aligned() {set_encodingRule(per_Basic_Aligned);}
    bool is_per_Basic_Aligned() const { return encodingRule == per_Basic_Aligned;}

    template <class OutputIterator>
    bool encode(const AbstractData& val, OutputIterator begin) {
        if (get_encodingRule() == per_Basic_Aligned)
            return encodePER(val, begin);
        if (get_encodingRule() == ber)
            return encodeBER(val, begin);
#ifdef ASN1_HAS_IOSTREAM
        if (get_encodingRule() == avn)
            return encodeAVN(val, begin);
#endif
        return false;
    }

    bool decode(const char* first, const char* last, AbstractData& val, bool defered);
    bool decode(const unsigned char* first, const unsigned char* last , AbstractData& val, bool defered) {
        return decode(reinterpret_cast<const char*>(first), reinterpret_cast<const char*>(last), val, defered);
    }

#if !defined(_MSC_VER) || (_MSC_VER > 1800)
    template <class InputIterator>
    bool decode(InputIterator first, InputIterator last, AbstractData& val, bool defered) {
#ifdef ASN1_HAS_IOSTREAM
        if (get_encodingRule() == avn) {
            std::istringstream strm(std::string(first, last));
            AVNDecoder decoder(strm);
            return val.accept(decoder);
        }
#endif // ASN1_HAS_IOSTREAM
        OpenBuf buf(first, last);
        return decode((const char*)&buf[0], (const char*)&*buf.end(), val, defered);
    }
#endif

protected:
    EncodingRules encodingRule;
    struct StringListeralCmp : public std::binary_function<const char*, const char*, bool> {
        bool operator()(const char* lhs, const char* rhs) const
        { return strcmp(lhs, rhs) < 0 ; }
    };
    typedef Loki::AssocVector<const char*, Module*, StringListeralCmp> Modules;
    Modules modules;
    template <class OutputIterator>
    bool encodePER(const AbstractData& val, OutputIterator begin) {
        OpenBuf buf;
        PEREncoder encoder(buf);
        if (val.accept(encoder)) {
            std::copy(buf.begin(), buf.end(), begin);
            return true;
        }
        return false;
    }

    template <class OutputIterator>
    bool encodeBER(const AbstractData& val, OutputIterator begin) {
        OpenBuf buf;
        BEREncoder encoder(buf);
        if (val.accept(encoder)) {
            std::copy(buf.begin() , buf.end(), begin);
            return true;
        }
        return false;
    }

#ifdef ASN1_HAS_IOSTREAM
    template <class OutputIterator>
    bool encodeAVN(const AbstractData& val, OutputIterator begin) {
        std::ostringstream strm;
        AVNEncoder encoder(strm);
        if (val.accept(encoder)) {
            const std::string& str = strm.str();
            std::copy(str.begin(), str.end(), begin);
            return true;
        }
        return false;
    }
#endif
};


template <class OutputIterator>
bool encode(const AbstractData& val, CoderEnv* env, OutputIterator begin) {
    return env->encode(val, begin);
}


template <class InputIterator>
bool decode(InputIterator first, InputIterator last, CoderEnv* env, AbstractData& val, bool defered = false) {
    return env->decode(first, last, val, defered);
}

////////////////////////////////////////////////////////////////////////////


} // namespace ASN1


#if defined(__GNUC__)
#pragma GCC diagnostic pop
#endif

#endif // _ASNER_H
