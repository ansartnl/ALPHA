/*
 * PEREncoder.cxx
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
 * $Log: PEREncoder.cxx,v $
 * Revision 1.7  2002/07/19 05:48:07  btrummer
 * If an illegal character is found in do_visit(const AbstractString&),
 * false is returned now to abort the encoding.
 *
 * Revision 1.6  2002/07/19 05:43:39  btrummer
 * Aaargh! If encodeConstrainedLength() fails in do_visit(const OCTET_STRING&),
 * false must be returned rather than true.
 *
 * Revision 1.5  2002/07/18 10:42:17  btrummer
 * Removed the assert statement from do_visit(const OCTET_STRING& value).
 * Replaced the assert in do_visit(const CHOICE& value) with an if-clause.
 *
 * Revision 1.4  2002/07/18 06:53:33  btrummer
 * Added range checks for PEREncoder's encodeLength(), encodeUnsigned()
 * and encodeConstrainedLength(). Encoding an object which violates its
 * length constraint will fail now.
 *
 * Revision 1.3  2002/07/02 04:45:13  mangelo
 * Modify for VC.Net and GCC 3.1
 *
 * Revision 1.2  2001/10/05 19:11:50  mangelo
 * Added Log
 *
 * 2001/06/26 Huang-Ming Huang 
 * Version 2.1 Reimplemented to minimize the code size.
 *
 * 2001/05/01
 * Fixed problem with en/decoding more than 16 extension fields in a sequence 
 * in accordance with PWLib asner.cxx Revision 1.41.
 * 
 */

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "asn1.h"

namespace ASN1 {

extern unsigned CountBits(unsigned range);

void PEREncoder::encodeBitMap(const std::vector<char>& bitData, unsigned nBits)
{
    int idx = 0;
    unsigned bitsLeft = nBits;
    while (bitsLeft >= 8) {
        encodeMultiBit(bitData[idx++],8);
        bitsLeft -= 8;
    }
    
    if (bitsLeft > 0)
        encodeMultiBit(bitData[idx] >> (8 - bitsLeft),bitsLeft);
}

inline void PEREncoder::byteAlign()
{
    if (bitOffset != 8)
        bitOffset = 8;
}

bool PEREncoder::do_visit(const Null&)
{
	return true;
}

bool PEREncoder::do_visit(const BOOLEAN& value)
{
  // X.931 Section 11
  encodeSingleBit((bool)value);
  return true;
}


bool PEREncoder::do_visit(const INTEGER& integer)
{
  // X.931 Sections 12

  unsigned value = (unsigned) integer.getValue();
  if (encodeConstraint(integer, (int)value)) { //  12.1
    unsigned nBytes;
    unsigned adjusted_value = (integer.getConstraintType() == FixedConstraint ? 
								value - integer.getLowerLimit() : value);
	unsigned nBits;
    if (adjusted_value == 0)
      nBits = 1;
    else if (integer.getConstraintType() != FixedConstraint )
		if ((int)value < 0)
			nBits = CountBits( (~value) +1);
		else
			nBits = CountBits(value+1) +1;
	else
      nBits = CountBits(adjusted_value+1);
    nBytes = (nBits+7)/8;
    encodeLength(nBytes, 0, INT_MAX);
    encodeMultiBit(adjusted_value, nBytes*8);
    return true;
  }
  
  if (static_cast<unsigned>(integer.getLowerLimit()) == integer.getUpperLimit()) // 12.2.1
    return true;

  // 12.2.2 which devolves to 10.5
  return encodeUnsigned(value, integer.getLowerLimit(), integer.getUpperLimit());
}

bool PEREncoder::do_visit(const ENUMERATED& value)
{
  if (value.extendable()) {  // 13.3
    bool extended = value.asInt() > value.getMaximum();
    encodeSingleBit(extended);
    if (extended) {
      encodeSmallUnsigned(1+value.asInt());
      return encodeUnsigned(value.asInt(), 0, value.asInt());
    }
  }
  return encodeUnsigned(value.asInt(), 0, value.getMaximum());  // 13.2
}


bool PEREncoder::do_visit(const OBJECT_IDENTIFIER& value)
{
  // X.691 Section 23

  std::vector<char> eObjId;
  value.encodeCommon(eObjId);
  if (!encodeLength(eObjId.size(), 0, 255))
    return false;
  encodeBlock(&eObjId.front(), eObjId.size());
  return true;
}

bool PEREncoder::do_visit(const BIT_STRING& value)
{
  // X.691 Section 15

  if (!encodeConstrainedLength(value, value.size()))
    return false;

  if (value.size() == 0)
    return true;

  if (value.size() > 16 && aligned())
    encodeBlock(&*value.getData().begin(), (value.size()+7)/8);   // 15.9
  else {// 15.8
    encodeBitMap(value.getData(), value.size());
  }

  return true;
}

bool PEREncoder::do_visit(const OCTET_STRING& value)
{
  // X.691 Section 16
  unsigned nBytes = value.size();

  if (!encodeConstrainedLength(value, nBytes))
    return false;

  switch (nBytes) {
    case 0 :  // 16.5
      break;

    case 1 :  // 16.6
      encodeMultiBit(value[0], 8);
      break;

    case 2 :  // 16.6
      encodeMultiBit(value[0], 8);
      encodeMultiBit(value[1], 8);
      break;

    default: // 16.7
      encodeBlock(&*value.begin(), nBytes);
  }
  return true;
}


bool PEREncoder::do_visit(const AbstractString& value)
{
  // X.691 Section 26

  unsigned len = value.size();
  if (!encodeConstrainedLength(value, len))
    return false;

  unsigned nBits = value.getNumBits(aligned());

  if (value.getConstraintType() == Unconstrained || value.getUpperLimit()*nBits > 16) {
    if (nBits == 8) {
      encodeBlock(value.c_str(), len);
      return true;
    }
    if (aligned())
      byteAlign();
  }

  for (unsigned i = 0; i < len; i++) {
    if (nBits >= value.getCanonicalSetBits() && value.getCanonicalSetBits() > 4)
      encodeMultiBit(value[i], nBits);
    else {
      const void * ptr = memchr(value.getCharacterSet(), value[i], value.getCharacterSetSize());
      if (ptr == NULL)
        return false;
      unsigned pos = ((const char *)ptr - value.getCharacterSet());
      encodeMultiBit(pos, nBits);
    }
  }
  return true;
}

bool PEREncoder::do_visit(const BMPString& value)
{
  // X.691 Section 26

  unsigned len = value.size();
  if (!encodeConstrainedLength(value, len))
    return false;

  unsigned nBits = value.getNumBits(aligned());

  if ((value.getConstraintType() == Unconstrained || value.getUpperLimit()*nBits > 16) && aligned())
    byteAlign();

  for (unsigned i = 0; i < len; i++) 
      encodeMultiBit(value[i] - value.getFirstChar(), nBits);

  return true;
}

bool PEREncoder::do_visit(const CHOICE& value)
{
  if (value.currentSelection() < 0)
    return false;

  if (value.extendable()) {
    bool extended = value.currentSelection() >= static_cast<int>(value.getNumChoices());
    encodeSingleBit(extended);
    if (extended) {
      encodeSmallUnsigned(value.currentSelection() - value.getNumChoices());
      return encodeAnyType(value.getSelection());
    }
  }

  if (value.getNumChoices() > 1)
    if (!encodeUnsigned(value.currentSelection(), 0, value.getNumChoices()-1))
      return false;

  return value.getSelection()->accept(*this);
}

bool PEREncoder::do_visit(const SEQUENCE_OF_Base& value)
{
  unsigned sz = value.size();
  if (!encodeConstrainedLength(value, sz))
    return false;

  SEQUENCE_OF_Base::const_iterator first = value.begin(), last = value.end();
	for (; first != last; ++first)
		if (!(*first)->accept(*this))
			return false;
  return true;
}

bool PEREncoder::do_visit(const OpenData& value)
{
	return encodeAnyType(&value.get_data());
}

bool PEREncoder::do_visit(const GeneralizedTime& value)
{
	std::string notion(value.get());
	encodeLength(notion.size(), 0, UINT_MAX);
	encodeBlock(&*notion.begin(), notion.size());
	return true;
}

bool PEREncoder::preVisitExtensionRoots(const SEQUENCE& value) 
{
  // X.691 Section 18
  if (value.extendable()) {
    bool hasExtensions = false;
    for (unsigned i = 0; i < value.extensionMap.size(); i++) {
      if (value.extensionMap[i]) {
        hasExtensions = true;
        break;
      }
    }
    encodeSingleBit(hasExtensions);  // 18.1
  }
  encodeBitMap(value.optionMap.bitData, value.optionMap.size());// 18.2
  return true;
}

bool PEREncoder::visitExtensionRoot(const SEQUENCE& value, int index)
{
	return value.fields[index]->accept(*this);
}

bool PEREncoder::preVisitExtensions(const SEQUENCE& value)
{
  int totalExtensions = value.extensionMap.size();
  encodeSmallUnsigned(totalExtensions-1);
  encodeBitMap(value.extensionMap.bitData, value.extensionMap.size());
  return true;
}

bool PEREncoder::visitKnownExtension(const SEQUENCE& value, int index)
{
  return encodeAnyType(value.fields[index]);
}


bool PEREncoder::encodeConstrainedLength(const ConstrainedObject & obj, unsigned length) 
{
  return (encodeConstraint(obj, length) ? // 26.4
          encodeLength(length, 0, INT_MAX) :
          encodeLength(length, obj.getLowerLimit(), obj.getUpperLimit()));
}

bool PEREncoder::encodeConstraint(const ConstrainedObject & obj, unsigned value) 
{
  if (!obj.extendable())
    return obj.getConstraintType() != FixedConstraint;

  bool needsExtending = value > obj.getUpperLimit();

  if (!needsExtending) {
    if (obj.getLowerLimit() < 0) {
      if ((int)value < obj.getLowerLimit())
        needsExtending = true;
    }
    else {
      if (value < (unsigned)obj.getLowerLimit())
        needsExtending = true;
    }
  }

  encodeSingleBit(needsExtending);

  return needsExtending || obj.getConstraintType() < FixedConstraint;
}

void PEREncoder::encodeSingleBit(bool value)
{

  if (bitOffset == 8)
      encodedBuffer.push_back(0);

  bitOffset--;

  if (value)
    encodedBuffer.back() |= 1 << bitOffset;

  if (bitOffset == 0)
    byteAlign();
}

void PEREncoder::encodeMultiBit(unsigned value, unsigned nBits)
{
  if (nBits == 0)
    return;

  if (bitOffset == 8)
      encodedBuffer.push_back(0);

  // Make sure value is in bounds of bit available.
  if (nBits < sizeof(int)*8)
    value &= ((1 << nBits) - 1);

  if (nBits < bitOffset) {
    bitOffset -= nBits;
    encodedBuffer.back() |= value << bitOffset;
    return;
  }

  nBits -= bitOffset;
  encodedBuffer.back() |= (char)(value >> nBits);
  bitOffset = 8;

  while (nBits >= 8) {
    nBits -= 8;
    encodedBuffer.push_back(value >> nBits) ;
  }

  if (nBits > 0) {
    bitOffset = 8 - nBits;
    encodedBuffer.push_back((value & ((1 << nBits)-1)) << bitOffset);
  }
}

void PEREncoder::encodeSmallUnsigned(unsigned value)
{
  if (value < 64) {
    encodeMultiBit(value, 7);
    return;
  }

  encodeSingleBit(true);// 10.6.2

  unsigned len = 4;
  if (value < 256)
    len = 1;
  else if (value < 65536)
    len = 2;
  else if (value < 0x1000000)
    len = 3;
  encodeLength(len, 0, INT_MAX);  // 10.9
  byteAlign();
  encodeMultiBit(value, len*8);
}

bool PEREncoder::encodeLength(unsigned len, unsigned lower, unsigned upper)
{
  if (len < lower || len > upper)
    return false;

  // X.691 section 10.9

  if (upper != INT_MAX && !alignedFlag) {
    assert(upper - lower < 0x10000);  // 10.9.4.2 unsupperted
    encodeMultiBit(len - lower, CountBits(upper - lower + 1));   // 10.9.4.1
    return true;
  }

  if (upper < 65536)  // 10.9.3.3
    return encodeUnsigned(len, lower, upper);

  byteAlign();

  if (len < 128) {
    encodeMultiBit(len, 8);   // 10.9.3.6
    return true;
  }

  encodeSingleBit(true);

  if (len < 0x2000) {
    encodeMultiBit(len, 15);    // 10.9.3.7
    return true;
  }

  encodeSingleBit(true);
  assert(len < 0x2000);  // 10.9.3.8 unsupported

  return true;
}

bool PEREncoder::encodeUnsigned(unsigned value, unsigned lower, unsigned upper)
{
  if (value < lower || value > upper)
    return false;

  // X.691 section 10.5

  if (lower == upper) // 10.5.4
    return true;

  unsigned range = (upper - lower) + 1;
  unsigned nBits = CountBits(range);
  unsigned adjusted_value = value - lower;

  if (alignedFlag && (range == 0 || range > 255)) { // not 10.5.6 and not 10.5.7.1
    if (nBits > 16) {                           // not 10.5.7.4
      int numBytes = adjusted_value == 0 ? 1 : (((CountBits(adjusted_value + 1))+7)/8);
      if (!encodeLength(numBytes, 1, (nBits+7)/8))    // 12.2.6
        return false;
      nBits = numBytes*8;
    }
    else if (nBits > 8)      // not 10.5.7.2
      nBits = 16;            // 10.5.7.3
    byteAlign();             // 10.7.5.2 - 10.7.5.4
  }

  encodeMultiBit(adjusted_value, nBits);

  return true;
}

bool PEREncoder::encodeAnyType(const AbstractData * value)
{
  OpenBuf buf;
  PEREncoder subEncoder(buf);

  if (value != NULL)
    if (!value->accept(subEncoder))
		return false;

  if (buf.size() == 0)				   // Make sure extension has at least one
    subEncoder.encodeSingleBit(false); // byte in its ANY type encoding.

  unsigned nBytes = buf.size();
  encodeLength(nBytes, 0, INT_MAX);
  encodeBlock(&*buf.begin(), nBytes);
  return true;
}

void PEREncoder::encodeByte(unsigned value)
{
  byteAlign();
  encodedBuffer.push_back(value);
}

void PEREncoder::encodeBlock(const char * bufptr, unsigned nBytes)
{
	if (nBytes == 0) 
		return; 
	byteAlign();
	encodedBuffer.insert(encodedBuffer.end(), bufptr, bufptr + nBytes);
}
}
