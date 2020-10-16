/*
 * InvalidTracer.cxx
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
 * The Original Code is III ASN.1 Tool
 *
 * The Initial Developer of the Original Code is Institute for Information Industry.
 *
 * Portions are Copyright (C) 1993 Free Software Foundation, Inc.
 * All Rights Reserved.
 *
 * Contributor(s): Huang-Ming Huang
 *
 * $Log: InvalidTracer.cxx,v $
 * Revision 1.3  2002/07/02 04:45:13  mangelo
 * Modify for VC.Net and GCC 3.1
 *
 * Revision 1.2  2001/08/03 07:42:00  mangelo
 * Add InvalidTracer
 *
 */
#ifdef ASN1_HAS_IOSTREAM
#include "asn1.h"

namespace ASN1 {

/**
 * The InvalidTracer class defines the opertions to find the first component of a constructed
 * ASN1::AbstactData object which doesn't meet its own constraint.
 *
 * @see trace_invalid()
 */

class InvalidTracer : public ConstVisitor {
public:
    friend std::ostream& operator << (std::ostream& os, const InvalidTracer& tracer) {
        return os << tracer.strm.str();
    }
private:
    bool do_visit(const Null& value);
    bool do_visit(const BOOLEAN& value);
    bool do_visit(const INTEGER& value);
    bool do_visit(const ENUMERATED& value);
    bool do_visit(const OBJECT_IDENTIFIER& value);
    bool do_visit(const BIT_STRING& value) ;
    bool do_visit(const OCTET_STRING& value);
    bool do_visit(const AbstractString& value);
    bool do_visit(const BMPString& value);
    bool do_visit(const CHOICE& value);
    bool do_visit(const OpenData& value);
    bool do_visit(const GeneralizedTime& value);
    bool do_visit(const SEQUENCE_OF_Base& value);

    bool visitExtensionRoot(const SEQUENCE& value, int index);
    bool visitKnownExtension(const SEQUENCE& value, int index);

    std::stringstream strm;
};


bool InvalidTracer::do_visit(const Null&) {
    return true;
}

bool InvalidTracer::do_visit(const BOOLEAN&) {
    return true;
}


bool InvalidTracer::do_visit(const INTEGER& value) {
    if (value.getLowerLimit() >= 0) {
        unsigned v = static_cast<unsigned>(value.getValue());
        if (value.getValue() < value.getLowerLimit())
            strm << " The INTEGER value " << v << " is smaller than lower bound " << value.getLowerLimit();
        else if (v > value.getUpperLimit() && !value.extendable())
            strm << " The INTEGER value " << v << " is greater than upper bound " << value.getUpperLimit();
        else
            return true;
    } else {
        if (value.getValue() < value.getLowerLimit())
            strm << " The INTEGER value " << value.getValue() << " is smaller than lower bound " << value.getLowerLimit();
        else if (static_cast<unsigned>(value.getValue()) > value.getUpperLimit() && !value.extendable())
            strm << " The INTEGER value " << value.getValue() << " is greater than upper bound " << value.getUpperLimit();
        else
            return true;
    }
    return false;
}

bool InvalidTracer::do_visit(const ENUMERATED& value) {
    if (value.asInt() > value.getMaximum()) {
        strm << " This ENUMERATED has invalid value " << value.asInt();
        return false;
    }
    return true;
}

bool InvalidTracer::do_visit(const OBJECT_IDENTIFIER& value) {
    if (value.levels() == 0) {
        strm << " This OBJECT IDENTIFIER is not assigned";
        return false;
    }
    return true;
}

bool InvalidTracer::do_visit(const BIT_STRING& value) {
    if (value.size() < static_cast<unsigned>(value.getLowerLimit())) {
        strm << " This BIT STRING has size " << value.size() << " smaller than its lower bound " << value.getLowerLimit();
    } else if (value.getConstraintType() == FixedConstraint && value.size() > value.getUpperLimit()) {
        strm << " This BIT STRING has size " << value.size() << " greater than its upper bound " << value.getUpperLimit();
    } else
        return true;

    return false;
}

bool InvalidTracer::do_visit(const OCTET_STRING& value) {
    if (value.size() < static_cast<unsigned>(value.getLowerLimit())) {
        strm << " This OCTET STRING has size " << value.size() << " smaller than its lower bound " << value.getLowerLimit();
    } else if (value.getConstraintType() == FixedConstraint && value.size() > value.getUpperLimit()) {
        strm << " This OCTET STRING has size " << value.size() << " greater than its upper bound " << value.getUpperLimit();
    } else
        return true;

    return false;
}

bool InvalidTracer::do_visit(const AbstractString& value) {
    size_t pos;
    if (value.size() < static_cast<unsigned>(value.getLowerLimit())) {
        strm << " This AbstractString has size " << value.size() << " smaller than its lower bound " << value.getLowerLimit();
    } else if (value.getConstraintType() == FixedConstraint && value.size() > value.getUpperLimit()) {
        strm << " This AbstractString has size " << value.size() << " greater than its upper bound " << value.getUpperLimit();
    } else if ((pos = value.find_first_invalid()) != std::string::npos) {
        strm << " The character '" << value[pos]  << "' is not valid for the string";
    } else
        return true;

    return false;
}

bool InvalidTracer::do_visit(const BMPString& value) {
    unsigned pos;
    if (value.size() < static_cast<unsigned>(value.getLowerLimit())) {
        strm << " This BMPString has size " << value.size() << " smaller than its lower bound " << value.getLowerLimit();
    } else if (value.getConstraintType() == FixedConstraint && value.size() > value.getUpperLimit()) {
        strm << " This BMPString has size " << value.size() << " greater than its upper bound " << value.getUpperLimit();
    } else if ((pos = value.first_illegal_at()) < value.size()) {
        strm << " The character '" << value[pos]  << "' is not valid for the string";
    } else
        return true;

    return false;
}

bool InvalidTracer::do_visit(const CHOICE& value) {
    if (value.currentSelection() == CHOICE::unselected_)
        strm << " This CHOICE is not selected";
    else if (value.currentSelection() == CHOICE::unknownSelection_)
        strm << " This selection is not understood by this decoder";
    else {
        strm << "." << value.getSelectionName();
        return value.getSelection()->accept(*this);
    }
    return false;
}

bool InvalidTracer::do_visit(const OpenData& value) {
    if (value.has_data())
        return value.get_data().accept(*this);

    if (!value.has_buf()) {
        strm << " This Open Type does not contain any valid data";
        return false;
    }

    return true;
}

bool InvalidTracer::do_visit(const GeneralizedTime& value) {
    if (value.isStrictlyValid()) {
        strm << " This GeneralizedTime is not valid";
        return false;
    }
    return true;
}

bool InvalidTracer::do_visit(const SEQUENCE_OF_Base& value) {
    if (value.size() < static_cast<unsigned>(value.getLowerLimit())) {
        strm << " This SEQUENCE OF has size " << value.size() << " smaller than its lower bound " << value.getLowerLimit();
        return false;
    } else if (value.getConstraintType() == FixedConstraint && value.size() > value.getUpperLimit()) {
        strm << " This SEQUENCE OF has size " << value.size() << " greater than its upper bound " << value.getUpperLimit();
        return false;
    }

    SEQUENCE_OF_Base::const_iterator first = value.begin(), last = value.end();

    for (; first != last; ++first) {
        InvalidTracer tracer;
        if (!(*first)->accept(tracer)) {
            strm << "[" << first - value.begin() << "]" << tracer;
            return false;
        }
    }
    return true;
}

bool InvalidTracer::visitExtensionRoot(const SEQUENCE& value, int index) {
    InvalidTracer tracer;
    if (!value.getField(index)->accept(tracer)) {
        strm <<  "." << value.getFieldName(index) << tracer;
        return false;
    }
    return true;
}

bool InvalidTracer::visitKnownExtension(const SEQUENCE& value, int index) {
    return visitExtensionRoot(value, index);
}

bool trace_invalid(std::ostream& os, const char* str, const AbstractData& data) {
    InvalidTracer tracer;
    if (!data.accept(tracer)) {
        os << str << tracer << std::endl;
        return false;
    }
    return true;
}

}
#endif
