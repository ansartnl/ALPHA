/*
 * PERDecoder.cxx
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
 * The code is adapted from asner.cxx of PWLib, but the dependancy on PWLib has
 * been removed.
 *
 * $Log: PERDecoder.cxx,v $
 * Revision 1.6  2002/07/10 07:25:48  btrummer
 * Minor simplification at the end of PERDecoder::decodeChoicePreamle().
 *
 * Revision 1.5  2002/07/03 08:15:30  btrummer
 * Fixed the assignment to beginPosition in PERDecoder::setPosition().
 *
 * Revision 1.4  2002/07/03 06:26:38  btrummer
 * Introduced an upper bound for the size of strings and arrays,
 * like PWLib's PERDecoder does. Otherwise, broken data may make the
 * PERDecoder allocate hundrets of Megabytes RAM...
 *
 * Revision 1.3  2002/07/02 04:45:13  mangelo
 * Modify for VC.Net and GCC 3.1
 *
 * Revision 1.2  2001/10/05 19:10:42  mangelo
 * Added Log
 *
 * 2001/07/16 Huang-Ming Huang
 * Optional components of SEQUENCE is now created on demand.
 *
 * 2001/06/26 Huang-Ming Huang
 * Version 2.1 Reimplemented to minimize the code size.
 *
 * 2001/05/01
 * Fixed problem with en/decoding more than 16 extension fields in a sequence
 *   in accordance with PWLib asner.cxx Revision 1.41.
 */

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "asn1.h"

namespace ASN1 {

// When resizing strings or vectors, these values will be used as upper limit.
static unsigned MaximumArraySize = 128;
static unsigned MaximumStringSize = 16 * 1024;

extern unsigned CountBits(unsigned range);

bool PERDecoder::decodeBitMap(std::vector<char>& bitData, unsigned nBit) {
    unsigned theBits;
    int idx = 0;
    unsigned bitsLeft = nBit;
    while (bitsLeft >= 8) {
        if (!decodeMultiBit(8, theBits))
            return false;
        bitData[idx++] = (unsigned char)theBits;
        bitsLeft -= 8;
    }

    if (bitsLeft > 0) {
        if (!decodeMultiBit(bitsLeft, theBits))
            return false;
        bitData[idx] = (unsigned char)(theBits << (8 - bitsLeft));
    }
    return true;
}

inline bool PERDecoder::atEnd() {
    return beginPosition >= endPosition;
}


bool PERDecoder::do_visit(Null&) {
    return true;
}

bool PERDecoder::do_visit(BOOLEAN& value) {
    if (atEnd())
        return false;

    // X.931 Section 11
    value = decodeSingleBit();
    return true;
}

bool PERDecoder::do_visit(INTEGER& integer) {
    // X.931 Sections 12

    unsigned value;
    if ((integer.getConstraintType() == ExtendableConstraint && decodeSingleBit())
            || integer.getConstraintType() < FixedConstraint) { //  12.1
        unsigned len;
        if (decodeLength(0, INT_MAX, len) != 0)
            return false;
        if (!decodeMultiBit(len * 8, value))
            return false;

        if (!integer.constrained() && (value & 1 << ((len * 8) - 1))) // negative value
            value = (0xFFFFFFFF << (len * 8)) | value;

        integer = value;
        return true;
    }

    if ((unsigned)integer.getLowerLimit() != integer.getUpperLimit())  // 12.2.1
        if (decodeUnsigned(integer.getLowerLimit(), integer.getUpperLimit(), value) == 0) { // 12.2.2 which devolves to 10.5
            integer = value;
            return true;
        }

    integer = integer.getLowerLimit();
    return true;
}

bool PERDecoder::do_visit(ENUMERATED& value) {
    // X.691 Section 13

    unsigned v;
    if (value.extendable()) {  // 13.3
        if (decodeSingleBit()) {
            unsigned len = 0;
            if (decodeSmallUnsigned(len) &&
                    len > 0 &&
                    decodeUnsigned(0, len - 1, v) == 0) {
                value.setFromInt(v);
                return true;
            }
        }
    }

    if (decodeUnsigned(0, value.getMaximum(), v) == 0) { // 13.2
        value.setFromInt(v);
        return true;
    }
    return false;
}


bool PERDecoder::do_visit(OBJECT_IDENTIFIER& value) {
    // X.691 Section 23

    unsigned dataLen;
    if (decodeLength(0, 255, dataLen) < 0)
        return false;

    beginPosition += dataLen;
    if (atEnd()) return false;

    return value.decodeCommon(beginPosition - dataLen, dataLen);
}

bool PERDecoder::do_visit(BIT_STRING& value) {
    // X.691 Section 15

    if (decodeConstrainedLength(value, value.totalBits) < 0)
        return false;

    if (value.totalBits > MaximumStringSize)
        return false;

    value.resize(value.totalBits);

    if (value.size() == 0)
        return true;   // 15.7

    if (value.size() > getBitsLeft())
        return false;

    if (value.size() > 16 && aligned()) {
        unsigned nBytes = (value.size() + 7) / 8;
        return decodeBlock(&*(value.bitData.begin()), nBytes) == nBytes;   // 15.9
    }

    return decodeBitMap(value.bitData, value.size());
}

bool PERDecoder::do_visit(OCTET_STRING& value) {
    // X.691 Section 16

    unsigned nBytes;
    if (decodeConstrainedLength(value, nBytes) < 0)
        return false;

    if (nBytes > MaximumStringSize)
        return false;

    value.resize(nBytes);   // 16.5

    unsigned theBits;
    switch (nBytes) {
    case 0 :
        break;

    case 1 :  // 16.6
        if (!decodeMultiBit(8, theBits))
            return false;
        value[0] = (char)theBits;
        break;

    case 2 :  // 16.6
        if (!decodeMultiBit(8, theBits))
            return false;
        value[0] = (char)theBits;
        if (!decodeMultiBit(8, theBits))
            return false;
        value[1] = (char)theBits;

        break;

    default: // 16.7
        return decodeBlock(&(*value.begin()), nBytes) == nBytes;
    }

    return true;
}

bool PERDecoder::do_visit(AbstractString& value) {
    // X.691 Section 26

    unsigned len;
    if (decodeConstrainedLength(value, len) < 0)
        return false;

    unsigned nBits = value.getNumBits(aligned());

    if (len > MaximumStringSize)
        return false;

    value.resize(len);
    if (value.getConstraintType() == Unconstrained || value.getUpperLimit()*nBits > 16) {
        if (nBits == 8) {
            if (decodeBlock(&(*value.begin()), len) != len)
                value.resize(0);
            return value.size() == len;
        }

        if (aligned())
            byteAlign();
    }


    unsigned i;
    for (i = 0; i < (unsigned)len; i++) {
        unsigned theBits;
        if (!decodeMultiBit(nBits, theBits))
            return false;
        if (nBits >= value.getCanonicalSetBits() && value.getCanonicalSetBits() > 4)
            value[i] = (char)theBits;
        else
            value[i] = value.getCharacterSet()[theBits];
    }
    return true;
}

bool PERDecoder::do_visit(BMPString& value) {
    // X.691 Section 26

    unsigned len;
    if (decodeConstrainedLength(value, len) < 0)
        return false;

    if (len > MaximumStringSize)
        return false;

    value.resize(len);

    unsigned nBits = value.getNumBits(aligned());

    if ((value.getConstraintType() == Unconstrained || value.getUpperLimit()*nBits > 16) && aligned())
        byteAlign();

    for (unsigned i = 0; i < (unsigned)len; i++) {
        unsigned theBits;
        if (!decodeMultiBit(nBits, theBits))
            return false;
        value[i] = (wchar_t)(theBits + value.getFirstChar());
    }
    return true;
}

bool PERDecoder::decodeChoicePreamle(CHOICE& value, memento_type& nextPosition) {
    // X.691 Section 22
    if (atEnd()) {
        return false;
    }

    unsigned choiceID;

    if (value.extendable()) {
        if (decodeSingleBit()) {
            if (!decodeSmallUnsigned(choiceID))
                return false;
            choiceID += value.getNumChoices();
            unsigned len;
            if (decodeLength(0, INT_MAX, len) != 0)
                return false;
            nextPosition.bytePosition = beginPosition + len;
            nextPosition.bitPosition = 8;
            return value.select(choiceID);
        }
    }

    if (value.getNumChoices() < 2)
        choiceID = 0;
    else {
        if (decodeUnsigned(0, value.getNumChoices() - 1, choiceID) < 0)
            return false;
    }

    nextPosition.bytePosition = 0;
    return value.select(choiceID);
}


bool PERDecoder::do_visit(CHOICE& value) {
    memento_type memento;
    if (decodeChoicePreamle(value, memento)) {
        if (!value.isUnknownSelection() && !value.getSelection()->accept(*this))
            return false;
        rollback(memento);
        return true;
    }
    return false;
}

bool PERDecoder::do_visit(SEQUENCE_OF_Base& value) {

    unsigned size;
    if (decodeConstrainedLength(value, size) < 0)
        return false;

    if (size > MaximumArraySize)
        return false;

    value.resize(size);

    SEQUENCE_OF_Base::iterator it = value.begin(), last = value.end();
    for (; it != last; ++it) {
        if (*it == NULL)
            *it = value.createElement();
        if (!(*it)->accept(*this)) {
            value.erase(it, last);
            return false;
        }
    }
    return true;
}

bool PERDecoder::do_visit(OpenData& data) {
    OCTET_STRING value;
    if (value.accept(*this)) {
        if (!data.has_buf())
            data.grab(new OpenBuf);
        data.get_buf().swap(value);
        return true;
    }
    return false;
}

bool PERDecoder::do_revisit(OpenData& value) {
    if (!value.has_buf() || !value.has_data())
        return false;
    PERDecoder decoder(&(*value.get_buf().begin()), &(*value.get_buf().end()), get_env());
    return value.get_data().accept(decoder);
}

bool PERDecoder::do_visit(TypeConstrainedOpenData& value) {
    assert(value.has_data());
    unsigned len;
    if (decodeLength(0, INT_MAX, len) != 0)
        return false;
    const char* nextPosition = beginPosition + len;
    bool ok = value.get_data().accept(*this);
    setPosition(nextPosition);
    return ok;
}

bool PERDecoder::do_visit(GeneralizedTime& value) {
    unsigned len;
    if (decodeLength(0, INT_MAX, len)) {
        std::vector<char> data(len);
        if (decodeBlock(&data[0], len)) {
            value.set(&data[0]);
            return true;
        }
    }
    return false;
}

Visitor::VISIT_SEQ_RESULT PERDecoder::preVisitExtensionRoots(SEQUENCE& value) {
    // X.691 Section 18

    bool hasExtension = false;
    if (value.extendable()) {
        if (atEnd())
            return FAIL;
        hasExtension = decodeSingleBit() ;  // 18.1
    }
    return decodeBitMap(value.optionMap.bitData, value.optionMap.size()) ?
           (hasExtension  ? CONTINUE : NO_EXTENSION) : FAIL;  // 18.2
}

Visitor::VISIT_SEQ_RESULT PERDecoder::visitExtensionRoot(SEQUENCE& value, int index, int optional_id) {
    if (optional_id == SEQUENCE::mandatory_ || value.hasOptionalField(optional_id)) {
        if (value.getField(index) == NULL)
            value.fields[index] = AbstractData::create(value.info()->fieldInfos[index]);

        if (value.getField(index) == NULL || !value.getField(index)->accept(*this))
            return FAIL;
    }
    return CONTINUE;
}

Visitor::VISIT_SEQ_RESULT PERDecoder::preVisitExtensions(SEQUENCE& value) {
    unsigned totalExtensions;
    if (!decodeSmallUnsigned(totalExtensions))
        return FAIL;
    totalExtensions++;

    if (totalExtensions > MaximumArraySize)
        return FAIL;

    value.extensionMap.resize(totalExtensions);
    Visitor::VISIT_SEQ_RESULT result;
    result = decodeBitMap(value.extensionMap.bitData, value.extensionMap.size()) ? CONTINUE : FAIL;
    return result;
}

Visitor::VISIT_SEQ_RESULT PERDecoder::visitKnownExtension(SEQUENCE& value, int index, int optional_id) {
    if (!value.hasOptionalField(optional_id))
        return CONTINUE;

    unsigned len;
    if (decodeLength(0, INT_MAX, len) != 0)
        return FAIL;
    const char* nextExtensionPosition = beginPosition + len;
    if (value.getField(index) == NULL)
        value.fields[index] = AbstractData::create(value.info()->fieldInfos[index]);

    bool ok = value.getField(index) == NULL || value.getField(index)->accept(*this);
    setPosition(nextExtensionPosition);
    return ok ? CONTINUE : FAIL;
}

bool PERDecoder::visitUnknownExtensions(SEQUENCE& value) {
    unsigned unknownCount = value.extensionMap.size() - value.info()->knownExtensions;

    if (unknownCount <= 0)
        return true; // Already read them

    for (unsigned i = value.info()->knownExtensions; i < (unsigned)value.extensionMap.size(); i++) {
        if (value.extensionMap[i]) {
            unsigned len;
            if (decodeLength(0, INT_MAX, len) != 0)
                return false;
            beginPosition += len;
            if (atEnd())
                return false;
        }
    }

    return true;
}

void PERDecoder::setPosition(const char* newPos) {
    beginPosition = newPos < endPosition ? newPos : endPosition;
    bitOffset = 8;
}

void PERDecoder::byteAlign() {
    if (bitOffset != 8) {
        bitOffset = 8;
        beginPosition++;
    }
}

unsigned PERDecoder::getBitsLeft() const {
    return (endPosition - beginPosition) * 8 - (8 - bitOffset);
}

int PERDecoder::decodeConstrainedLength(ConstrainedObject& obj, unsigned& length) {
    // The execution order is important in the following. The decodeSingleBit() function
    // must be called if extendableFlag is true, no matter what.
    if ((obj.getConstraintType() == ExtendableConstraint && decodeSingleBit())
            || obj.getConstraintType() == Unconstrained)
        return decodeLength(0, INT_MAX, length);
    else
        return decodeLength(obj.getLowerLimit(), obj.getUpperLimit(), length);
}

bool PERDecoder::decodeSingleBit() {
    if (getBitsLeft() == 0)
        return false;

    bitOffset--;
    bool value = (*beginPosition & (1 << bitOffset)) != 0;

    if (bitOffset == 0) {
        byteAlign();
    }

    return value;
}

bool PERDecoder::decodeMultiBit(unsigned nBits, unsigned& value) {

    if (nBits <= sizeof(value) * 8 && nBits <= getBitsLeft()) {

        if (nBits == 0) {
            value = 0;
            return true;
        }

        if (nBits < bitOffset) {
            bitOffset -= nBits;
            value = (static_cast<unsigned char>(*beginPosition) >> bitOffset) & ((1 << nBits) - 1);
            return true;
        }

        value = static_cast<unsigned char>(*beginPosition++) & ((1 << bitOffset) - 1);
        nBits -= bitOffset;
        bitOffset = 8;

        while (nBits >= 8) {
            value = (value << 8) | static_cast<unsigned char>(*beginPosition++);
            nBits -= 8;
        }

        if (nBits > 0) {
            bitOffset = 8 - nBits;
            value = (value << nBits) | (static_cast<unsigned char>(*beginPosition) >> bitOffset);
        }

        return true;
    }
    return false;
}

bool PERDecoder::decodeSmallUnsigned(unsigned& value) {
    // X.691 Section 10.6

    if (!decodeSingleBit())
        return decodeMultiBit(6, value);      // 10.6.1

    unsigned len;
    if (decodeLength(0, INT_MAX, len) != 0)  // 10.6.2
        return false;

    byteAlign();
    return decodeMultiBit(len * 8, value);
}


int PERDecoder::decodeLength(unsigned lower, unsigned upper, unsigned& len) {
    // X.691 section 10.9

    if (upper != INT_MAX && !alignedFlag) {
        assert(upper - lower < 0x10000);  // 10.9.4.2 unsupported
        unsigned base;
        auto tempBitOffset = bitOffset;
        auto tempBeginPosition = beginPosition;
        if (!decodeMultiBit(CountBits(upper - lower + 1), base))
            return -1;
        bitOffset = tempBitOffset;
        beginPosition = tempBeginPosition;
    }

    if (upper < 65536)  // 10.9.3.3
        return decodeUnsigned(lower, upper, len);

    // 10.9.3.5
    byteAlign();
    if (atEnd())
        return -1;

    if (decodeSingleBit() == 0) {
        return decodeMultiBit(7, len) ? 0 : -1;
    }

    if (decodeSingleBit() == 0) {
        return decodeMultiBit(14, len) ? 0 : -1;
    }

    // 10.9.3.8 unsupported
    return -1;
}

int PERDecoder::decodeUnsigned(unsigned lower, unsigned upper, unsigned& value) {
    // X.691 section 10.5

    if (lower == upper) { // 10.5.4
        value = lower;
        return 0;
    }

    if (atEnd())
        return -1;

    unsigned range = (upper - lower) + 1;
    unsigned nBits = CountBits(range);

    if (alignedFlag && (range == 0 || range > 255)) { // not 10.5.6 and not 10.5.7.1
        if (nBits > 16) {                           // not 10.5.7.4
            decodeLength(1, (nBits + 7) / 8, nBits);  // 12.2.6
            nBits *= 8;
        } else if (nBits > 8)  // not 10.5.7.2
            nBits = 16;          // 10.5.7.3
        byteAlign();           // 10.7.5.2 - 10.7.5.4
    }

    if (decodeMultiBit(nBits, value)) {
        value += lower;
        return 0;
    }
    return -1;

}


unsigned PERDecoder::decodeBlock(char* bufptr, unsigned nBytes) {

    if (nBytes == 0)
        return 0;

    byteAlign();

    if (beginPosition + nBytes > endPosition)
        nBytes = endPosition - beginPosition;

    if (nBytes == 0)
        return 0;

    memcpy(bufptr, beginPosition, nBytes);

    beginPosition += nBytes;
    return nBytes;
}

} // namespace ASN1
