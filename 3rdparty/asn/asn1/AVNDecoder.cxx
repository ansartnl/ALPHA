/*
 * avndecoder.cxx
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
 * Contributor(s):
 *
 * $Log: AVNDecoder.cxx,v $
 * Revision 1.2  2002/07/02 04:45:13  mangelo
 * Modify for VC.Net and GCC 3.1
 *
 * 2001/07/16 Huang-Ming Huang
 * Optional components of SEQUENCE is now created on demand.
 *
 * 2001/06/26 Huang-Ming Huang
 * Version 2.1 Reimplemented to minimize the code size.
 *
 */

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "asn1.h"
#ifdef ASN1_HAS_IOSTREAM
#include <boost/smart_ptr.hpp>
#include <sstream>
#include "ios_helper.h"


namespace ASN1 {

bool AbstractData::setFromValueNotation(const std::string& valueString) {
    std::stringstream strm(valueString, std::ios_base::in);
    AVNDecoder decoder(strm);
    return accept(decoder);
}

std::ios_base::iostate AbstractData::get_from(std::istream& strm) {
    AVNDecoder decoder(strm);
    if (accept(decoder))
        return std::ios_base::goodbit;
    return std::ios_base::failbit;
}

std::istream& operator >>(std::istream& is, AbstractData& arg) {
    if (!is.good()) return is;
    return g_extractor(is, arg);
}


inline bool input_success(std::istream& strm) {
    return !(strm.rdstate() & (std::ios_base::badbit | std::ios_base::failbit));
}


bool IntegerWithNamedNumber::setFromName(const std::string& str) {
    const NameEntry* begin = info()->nameEntries, *i = begin;
    for (; i != begin + info()->entryCount; ++i)
        if (strcmp(i->name, str.c_str()) == 0) {
            value = i->value;
            return true;
        }
    return false;
}


bool ENUMERATED::setFromName(const std::string& str) {
    const char** pos = info()->names;
    const char** end = pos + getMaximum() + 1;
    for (; pos != end ; ++pos)
        if (strcmp(str.c_str(), *pos) == 0)
            break;
    value = pos - info()->names;
    if (value <= getMaximum())
        return true;
    else
        return false;
}

bool AVNDecoder::do_visit(Null&) {
    std::string tmp;
    char c;
    while (strm >> c) {
        if (isalpha(c))
            tmp += c;
        else {
            strm.putback(c);
            break;
        }
    }
    if (tmp != "NULL")
        return false;
    return true;
}

bool AVNDecoder::do_visit(BOOLEAN& value) {
    std::string tmp;
    char c;
    while (strm >> c) {
        if (isalpha(c))
            tmp += c;
        else {
            strm.putback(c);
            break;
        }
    }
    if (tmp == "TRUE")
        value = true;
    else if (tmp == "FALSE")
        value = false;
    else
        return false;
    return true;
}

bool AVNDecoder::do_visit(INTEGER& value) {
    unsigned tmp;
    if (!value.constrained() || value.getLowerLimit() < 0)
        strm >> *(int*)&tmp;
    else
        strm >> tmp;
    value = tmp;
    return input_success(strm);
}

bool AVNDecoder::do_visit(IntegerWithNamedNumber& value) {
    char c;
    if (strm >> c) {
        strm.putback(c);
        if (isdigit(c) || c == '-')
            return visit(static_cast<INTEGER&>(value));

        std::string tmp;
        if (strm >> tmp)
            return value.setFromName(tmp);
    }
    return false;
}

bool AVNDecoder::do_visit(ENUMERATED& value) {
    std::string tmp;
    if (strm >> tmp)
        return value.setFromName(tmp);
    return false;
}


bool AVNDecoder::do_visit(OBJECT_IDENTIFIER& value) {
    char c;
    if (strm >> c) {
        if (c == '{') {
            std::string subString;
            if (std::getline(strm, subString, '}')) {
                std::stringstream subIs(subString);
                while (subIs >> subString) {
                    // process ObjIdComponent , either in the form "itu(0)"(NameAndNumberForm)
                    // or a number such as "2250" (NumberForm)
                    std::stringstream s(subString);
                    unsigned v;
                    if (isdigit(subString[0])) { // parse NumberForm
                        if (s >> v)
                            value.append(v);
                        else
                            return false;
                    } else { // parse NameAndNumberForm
                        if (std::getline(s, subString, '(') &&
                                s >> v) {
                            value.append(v);
                            continue;
                        } else if (subIs >> c) {
                            if (c == '(' && s >> v) {
                                value.append(v);
                                if (s >> c && c == ')')
                                    continue;
                            }
                        } else
                            return false;
                    }
                }
                return true;
            }
        }
        return false;
    }
    return false;
}

struct is_space_or_newline {
    bool operator()(char c) { return c == ' ' || c == '\n'; }
};

bool get_from_string(std::string& str0, std::vector<char>& value, unsigned int& totalBits, int base) {
    assert(base == 16 || base == 2);

    int nCharAByte = (base == 16) ? 2 : 8;
    std::string str;
    std::remove_copy_if(str0.begin()
                        , str0.end()
                        , std::inserter(str, str.begin())
                        , is_space_or_newline());
    totalBits = str.size();
    if (str.size() % nCharAByte)
        str.insert(str.end(), nCharAByte - (str.size() % nCharAByte), '0');
    value.reserve(str.size() / nCharAByte);
    std::string tmp(nCharAByte, ' ');
    char* stopPos;
    for (std::string::iterator it = str.begin(); it != str.end() ; it += nCharAByte) {
        char v;
        std::copy(it, it + nCharAByte, tmp.begin());
        v = (char) strtol(&*tmp.begin(), &stopPos, base);
        if (stopPos == &*tmp.end())
            value.push_back(v);
        else
            return false;
    }
    return true;
}

inline bool get_from_bstring(std::string& str, std::vector<char>& value, unsigned int& totalBits) {
    return get_from_string(str, value, totalBits, 2);
}

inline bool get_from_hstring(std::string& str, std::vector<char>& value, unsigned int& totalBits) {
    return get_from_string(str, value, totalBits, 16);
}

bool get_value_from(std::istream& strm, std::vector<char>& value, unsigned int& totalBits) {
    char c;
    if (strm >> c) {
        if (c != '\'')
            return false;

        std::string str;
        if (!std::getline(strm, str, '\''))
            return false;

        if (strm >> c) {
            switch (c) {
            case 'B':
                return get_from_bstring(str, value, totalBits);
            case 'H':
                return get_from_hstring(str, value, totalBits);
            default:
                return false;
            }
        }
    }
    return false;
}

bool AVNDecoder::do_visit(BIT_STRING& value) {
    return get_value_from(strm, value.bitData, value.totalBits);
}

bool AVNDecoder::do_visit(OCTET_STRING& value) {
    unsigned totalBits;
    return get_value_from(strm, value, totalBits);
}

bool get_string(std::istream& strm, std::string& str) {
    char c;
    if (strm >> c) {
        if (c == '\"')
            return !!std::getline(strm, str , '\"');
    }
    return false;
}

bool AVNDecoder::do_visit(AbstractString& value) {
    return get_string(strm, value);
}

bool AVNDecoder::do_visit(BMPString& value) {
    std::string str;
    if (get_string(strm, str)) {
        boost::scoped_array<wchar_t> tmp(new wchar_t[str.size() + 1]);
        int len = mbstowcs(tmp.get(), str.c_str(), str.size());
        if (len == -1)
            return false;
        value.assign(tmp.get(), len);
        return true;
    }
    return false;
}

bool get_id(const char** names, unsigned size, const std::string& name, int& id) {
    for (unsigned i = 0; i < size; ++i)
        if (name == names[i]) {
            id = i;
            return true;
        }
    return false;
}

bool AVNDecoder::do_visit(CHOICE& value) {
    std::string identifier;
    char c;
    int choiceID;
    if (strm >> identifier >> c) {
        if (c == ':' && get_id(value.info()->names, value.info()->totalChoices, identifier, choiceID)
                && value.select(choiceID))
            return value.getSelection()->accept(*this);
    }
    return false;
}

bool AVNDecoder::do_visit(SEQUENCE_OF_Base& value) {
    char c;
    if (strm >> c) {
        if (c != '{' || (strm >> c, !strm))
            return false;

        value.clear();

        if (c != '}')
            strm.putback(c);
        else
            return true; // no entry, return and indicate success

        do {
            std::unique_ptr<AbstractData> entry(value.createElement());
            if (entry.get() && entry->accept(*this) && strm >> c)
                value.push_back(entry.release());
            else
                return false;
        } while (c == ',');


        if (c == '}')
            return true;
    }
    return false;
}

bool AVNDecoder::do_visit(OpenData&) {
    // not implemented
    return false;
}

bool AVNDecoder::do_revisit(OpenData&) {
    // not implemented
    return false;
}

bool AVNDecoder::do_visit(TypeConstrainedOpenData& value) {
    assert(value.has_data());
    return value.get_data().accept(*this);
}


bool AVNDecoder::do_visit(GeneralizedTime& value) {
    std::string str;
    if (get_string(strm, str)) {
        value.set(str.c_str());
        return true;
    }
    return false;
}


Visitor::VISIT_SEQ_RESULT AVNDecoder::preVisitExtensionRoots(SEQUENCE&) {
    char c;
    if (strm >> c && c == '{') {
        if (strm >> c) {
            if (c == '}')
                return STOP;
            else {
                strm.putback(c);
                std::string id;
                if (strm >> id)
                    identifiers.push_back(id);
                else
                    return FAIL;
            }
            return CONTINUE;
        }
    }
    return FAIL;
}

Visitor::VISIT_SEQ_RESULT AVNDecoder::visitExtensionRoot(SEQUENCE& value, int index, int optional_id) {
    if (identifiers.back() == value.getFieldName(index)) {
        if (optional_id != -1)
            value.includeOptionalField(optional_id, index);
        AbstractData* field = value.getField(index);
        if (field && field->accept(*this)) {
            char c;
            if (strm >> c) {
                if (c == ',' && (strm >> identifiers.back()))
                    return CONTINUE;
                else if (c == '}')
                    return STOP;
            }
        }
        return FAIL;
    } else
        return (optional_id != -1) ? CONTINUE : FAIL ; // return true only it is not a mandatory component
}

Visitor::VISIT_SEQ_RESULT AVNDecoder::visitKnownExtension(SEQUENCE& value, int index, int optional_id) {
    return visitExtensionRoot(value, index, optional_id);
}

bool AVNDecoder::visitUnknownExtensions(SEQUENCE&) {
    if (identifiers.back() != "}")
        return false;
    identifiers.pop_back();
    return true;
}


}
#endif
