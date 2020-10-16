/*
 * BEREncoder.cxx
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
 * $Log: BEREncoder.cxx,v $
 * Revision 1.4  2002/07/02 04:45:13  mangelo
 * Modify for VC.Net and GCC 3.1
 *
 * Revision 1.3  2001/10/05 19:08:12  mangelo
 * Added Log
 *
 * 2001/06/26 Huang-Ming Huang 
 * Version 2.1 Reimplemented to minimize the code size.
 *
 */

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "asn1.h"

namespace ASN1 {

extern unsigned CountBits(unsigned range);

class PrimitiveChecker : public ConstVisitor
{
private:
    bool do_visit(const AbstractData&) { return true; }
	bool do_visit(const CHOICE& value) { 
		assert(value.currentSelection() >= 0) ;
		return value.getSelection()->accept(*this);
	}
	bool do_visit(const OpenData& value) { 
		assert(value.has_data());
		return value.get_data().accept(*this); 
	}
    bool do_visit(const SEQUENCE_OF_Base&) { return false; }
    bool preVisitExtensionRoots(const SEQUENCE&) { return false; }
};

unsigned getIntegerDataLength(int value)
{
  // create a mask which is the top nine bits of a DWORD, or 0xFF800000
  // on a big endian machine
  int shift = (sizeof(value)-1)*8-1;

  // remove all sequences of nine 0's or 1's at the start of the value
  while (shift > 0 && ((value >> shift)&0x1ff) == (value < 0 ? 0x1ff : 0))
    shift -= 8;

  return (shift+9)/8;
}

unsigned getDataLength(const AbstractData& data);
unsigned getObjectLength(const AbstractData& data, unsigned tag);

class DataLengthCounter : public ConstVisitor
{
public:
	DataLengthCounter() : length(0) {}
    unsigned getDataLen() const { 
		return length; 
	}
	unsigned getObjectLen(unsigned tag) const {
		int len = 1;
		
        unsigned tagVal = tag & 0xFFFF;
		if (tagVal >= 31)
			len += (CountBits(tagVal)+6)/7;
		
		int  dataLen = getDataLen();
		
		if (dataLen < 128)
			len++;
		else
			len += (CountBits(dataLen)+7)/8 + 1;
		
		return len + dataLen;
		
	}
private:
    bool do_visit(const Null&) {
		return true; 
	}
    bool do_visit(const BOOLEAN&) {
		++length;
		return true; 
	}
	bool do_visit(const INTEGER& value) { 
		length += getIntegerDataLength(value.getValue());
		return true; 
	}
	bool do_visit(const ENUMERATED& value) { 
		length += getIntegerDataLength(value.asInt());
		return true; 
	}
	bool do_visit(const OBJECT_IDENTIFIER& value) { 
		std::vector<char> dummy;
		value.encodeCommon(dummy);
		length += dummy.size();
		return true; 
	}
	bool do_visit(const BIT_STRING& value) { 
	    length += (value.size()+7)/8 + 1;
		return true; 
	}
	bool do_visit(const OCTET_STRING& value) {
		length += value.size();
		return true;
	}
	bool do_visit(const AbstractString& value) { 
		length += value.size();
		return true; 
	}
	bool do_visit(const BMPString& value) { 
		length += value.size()*2;
		return true; 
	}
	bool do_visit(const CHOICE& value) { 
	  if (value.currentSelection() >=0 )  {
		length +=  getObjectLength(*value.getSelection(),value.getSelectionTag());
	  }
	  return true; 
	}
	bool do_visit(const OpenData& value) { 
		length += (value.has_data() ? getDataLength(value.get_data()) : 0);
		return true; 
	}
	bool do_visit(const GeneralizedTime& value) { 
		length += value.get().size();
		return true; 
	}
	bool do_visit(const SEQUENCE_OF_Base& value) {

		SEQUENCE_OF_Base::const_iterator first = value.begin(), last = value.end();
		for (; first != last; ++first)
			length += ASN1::getObjectLength(**first, (*first)->getTag()); 
		return true; 
	}

    bool preVisitExtensionRoots(const SEQUENCE&) {
		return true; 
	}
	bool visitExtensionRoot(const SEQUENCE& value, int index) {
		length += ASN1::getObjectLength(*value.getField(index), value.getFieldTag(index));
		return true; 
	}
    bool VisitExtensions(const SEQUENCE&) {
		return true;
	}
	bool visitKnownExtension(const SEQUENCE& value, int index) { 
		length += ASN1::getObjectLength(*value.getField(index), value.getFieldTag(index));
		return true; 
	}

	unsigned length,tag;
};

unsigned getDataLength(const AbstractData& data)
{
	DataLengthCounter counter;
	data.accept(counter);
	return counter.getDataLen();
}

unsigned getObjectLength(const AbstractData& data, unsigned tag)
{
	DataLengthCounter counter;
	data.accept(counter);
	if (tag == 0 || (tag ==0xffffffff && data.getTag() ==0))
			return counter.getDataLen();
	return counter.getObjectLen(tag);
}

inline void BEREncoder::encodeByte(unsigned value)
{
  encodedBuffer.push_back(value);
}

inline void BEREncoder::encodeBlock(const char * bufptr, unsigned nBytes)
{
  encodedBuffer.insert(encodedBuffer.end(), bufptr, bufptr + nBytes);
}

bool BEREncoder::do_visit(const Null& value)
{
  encodeHeader(value);
  return true;
}

bool BEREncoder::do_visit(const BOOLEAN& value)
{
  encodeHeader(value);
  encodeByte(!value ? '\x00' : '\xff');
  return true;
}



bool BEREncoder::do_visit(const INTEGER& value)
{
  encodeHeader(value);
  // output the integer bits
  for (int count = getIntegerDataLength(value.getValue())-1; count >= 0; count--)
    encodeByte(value.getValue() >> (count*8));
  return true;
}

bool BEREncoder::do_visit(const ENUMERATED& value)
{
  encodeHeader(value);
  // output the integer bits
  for (int count = getIntegerDataLength(value.asInt())-1; count >= 0; count--)
    encodeByte(value.asInt() >> (count*8));
  return true;
}


bool BEREncoder::do_visit(const OBJECT_IDENTIFIER& value)
{
  encodeHeader(value);
  std::vector<char> data;
  value.encodeCommon(data);
  encodeBlock(&data.front(), data.size());  
  return true;
}

bool BEREncoder::do_visit(const BIT_STRING& value)
{
  encodeHeader(value);
  if (value.size() == 0)
    encodeByte(0);
  else {
    encodeByte(8-value.size()%8);
    encodeBlock(&*value.getData().begin(), (value.size()+7)/8);
  }
  return true;
}

bool BEREncoder::do_visit(const OCTET_STRING& value)
{
  encodeHeader(value);
  encodeBlock(&value[0], value.size());
  return true;
}


bool BEREncoder::do_visit(const AbstractString& value)
{
  encodeHeader(value);
  encodeBlock((const char*)value.c_str(), value.size());
  return true;
}

bool BEREncoder::do_visit(const BMPString& value)
{
  encodeHeader(value);
  for (unsigned i = 0; i < value.size(); ++i)
  {
    encodeByte(value[i] >> 8);
    encodeByte(value[i]);
  }
  return true;
}

bool BEREncoder::do_visit(const CHOICE& value)
{
	if (value.currentSelection() != CHOICE::unselected_)
	{
		if (tag != 0 && (tag!=0xffffffff || value.getTag() !=0))
			encodeHeader(value);
		tag = value.getSelectionTag();
		return value.getSelection()->accept(*this);
	}
	return false;
}

bool BEREncoder::do_visit(const SEQUENCE_OF_Base& value)
{
  encodeHeader(value);
  SEQUENCE_OF_Base::const_iterator first 
      = value.begin(), last = value.end();
	for (; first != last; ++first)
	{
		tag = 0xFFFFFFFF;
		if (!(*first)->accept(*this))
			return false;
	}
  return true;
}

bool BEREncoder::do_visit(const OpenData& value)
{
    if (tag == 0xFFFFFFFF) 
        tag = value.getTag();
    if (tag != 0)
        encodeHeader(value);
    tag = 0xFFFFFFFF;
    
    if (value.has_data())
        return value.get_data().accept(*this);
    else if (value.has_buf())
    {
        encodeBlock(&*value.get_buf().begin(), value.get_buf().size());
        return true;
    }
    return false;
}

bool BEREncoder::do_visit(const GeneralizedTime& value)
{
  encodeHeader(value);
  std::string data(value.get());
  encodeBlock((const char*)data.c_str(), data.size());
  return true;
}

bool BEREncoder::preVisitExtensionRoots(const SEQUENCE& value) 
{
	encodeHeader(value);
	return true;
}

bool BEREncoder::visitExtensionRoot(const SEQUENCE& value, int index)
{
    tag = value.getFieldTag(index);
	return value.getField(index)->accept(*this);
}

bool BEREncoder::visitKnownExtension(const SEQUENCE& value, int index)
{
    tag = value.getFieldTag(index);
	return value.getField(index)->accept(*this);
}

void BEREncoder::encodeHeader(const AbstractData & obj)
{
  unsigned obj_tag = (tag == 0xFFFFFFFF) ? obj.getTag() : tag;

  char ident = (char)(obj_tag >> 16);
  PrimitiveChecker checker;
  if (!obj.accept(checker))
    ident |= 0x20;

  unsigned tagNumber = obj_tag & 0xffff;

  if (tagNumber < 31)
    encodeByte(ident|tagNumber);
  else {
    encodeByte(ident|31);
    unsigned count = (CountBits(tagNumber)+6)/7;
    while (count-- > 1)
      encodeByte((tagNumber >> (count*7))&0x7f);
    encodeByte(tagNumber & 0x7f);
  }

  unsigned len = getDataLength(obj);
  if (len < 128)
    encodeByte(len);
  else {
    unsigned count = (CountBits(len+1)+7)/8;
    encodeByte(count|0x80);
    while (count-- > 0)
      encodeByte(len >> (count*8));
  }
}


} // namespace ASN1

