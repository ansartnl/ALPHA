#include "XMLCmd.h"

#include "TVAError.h"

#include <QtCore/QTextStream>

const char CXMLData::XML_COMMAND[] = "command";
const char CXMLData::XML_RESPONSE[] = "response";
const char CXMLData::XMLATTR_ID[] = "id";
const char CXMLData::XMLATTR_CMDNAME[] = "name";

CXMLData::CXMLData(const QString& sProtocol, const QString& sMainName)
{
  m_DEProtocol = m_DomDoc.createElement(sProtocol);
  m_DAID = m_DomDoc.createAttribute(XMLATTR_ID);
  m_DNMain = m_DomDoc.createElement(sMainName);

  m_DomDoc.appendChild(m_DEProtocol);
  m_DEProtocol.setAttributeNode(m_DAID);
  m_DEProtocol.appendChild(m_DNMain);

  // encoding
  QDomNode DNEncod = m_DomDoc.createProcessingInstruction("xml",
    "version=\"1.0\" encoding=\"UTF-8\"");
  m_DomDoc.insertBefore(DNEncod, m_DomDoc.firstChild());
}

CXMLData::CXMLData(const QByteArray& Buffer)
{
  QString sError;
  int iLine(0);
  int iColumn(0);
  TVA_CHECK(
      m_DomDoc.setContent(Buffer, false, &sError, &iLine, &iColumn),
      QObject::tr("Unable to parse XML command. Reason %1. Line: %2 Column: %3")
          .arg(sError).arg(iLine).arg(iColumn),
          E_FAIL);

  m_DEProtocol = m_DomDoc.documentElement();
  TVA_CHECK(!m_DEProtocol.isNull(), QObject::tr("No root element in XML"),
      E_FAIL);

  bool bOk(false);
  m_DAID = m_DEProtocol.attributeNode(XMLATTR_ID);
  m_DAID.value().toUInt(&bOk);
  TVA_CHECK(!m_DAID.isNull() && bOk, QObject::tr(
          "%1 attribute in XML is absent or has wrong structure").arg(XMLATTR_ID), E_FAIL);

  m_DNMain = m_DEProtocol.firstChildElement();
  TVA_CHECK(!m_DNMain.isNull() && (m_DNMain.nodeName() == XML_COMMAND
          || m_DNMain.nodeName() == XML_RESPONSE), QObject::tr(
          "Main element in XML is wrong. Allowed only %1, %2") .arg(
          XML_COMMAND).arg(XML_RESPONSE), E_FAIL);
}

bool CXMLData::IsCmd() const
{
  return m_DNMain.nodeName() == XML_COMMAND;
}

void CXMLData::SetID(quint32 uiId)
{
  m_DAID.setValue(QString::number(uiId));
}

quint32 CXMLData::GetID() const
{
  return m_DAID.value().toUInt();
}

QString CXMLData::GetProtoName()
{
  return m_DEProtocol.nodeName();
}

QString CXMLData::GetCmdName()
{
  QDomAttr AttrCmdName = GetMainNode().attributeNode(XMLATTR_CMDNAME);
  if (!AttrCmdName.isNull())
    return AttrCmdName.nodeValue();
  return QString();
}

QByteArray CXMLData::ToByteArray()
{
  QByteArray ba;
  QTextStream strm(&ba);
  m_DomDoc.save(strm, 2, QDomNode::EncodingFromDocument);
  return ba;
}

QString CXMLData::ToString()
{
  return m_DomDoc.toString();
}

CXMLBase::CXMLBase(const QString& sProtocol, const QString& sMainName)
{
  m_pData = CXMLData::TXMLData(new CXMLData(sProtocol, sMainName));
}

CXMLBase::CXMLBase(const CXMLData::TXMLData& pData)
{
  m_pData = pData;
}

const char CXMLResp::XMLATTR_CODE[] = "code";
const char CXMLResp::XMLATTR_DESCR[] = "descr";

CXMLResp::CXMLResp(const QString& sProtocol, CXMLCmd& refCmd) :
  CXMLBase(sProtocol, CXMLData::XML_RESPONSE)
{
  m_DACode = GetDoc().createAttribute(XMLATTR_CODE);
  m_DADescr = GetDoc().createAttribute(XMLATTR_DESCR);

  GetMainNode().setAttributeNode(m_DACode);
  GetMainNode().setAttributeNode(m_DADescr);

  SetCode(0);
  SetID(refCmd.GetID());
}

CXMLResp::CXMLResp(const CXMLData::TXMLData& pData) :
  CXMLBase(pData)
{
  m_DACode = GetMainNode().attributeNode(XMLATTR_CODE);
  bool bOk(false);
  m_DACode.value().toInt(&bOk);
  TVA_CHECK(!m_DACode.isNull() && bOk, QObject::tr(
          "%1 attribute in XML command is absent or has wrong structure").arg(XMLATTR_CODE),
      E_FAIL);

  m_DADescr = GetMainNode().attributeNode(XMLATTR_DESCR);
  TVA_CHECK(!m_DADescr.isNull(), QObject::tr(
          "No description attribute in XML response"), E_FAIL);
}

void CXMLResp::SetCode(int iCode)
{
  m_DACode.setValue(QString::number(iCode));
}

int CXMLResp::GetCode()
{
  return m_DACode.value().toInt();
}

void CXMLResp::SetDescr(const QString& sDescr)
{
  m_DADescr.setValue(sDescr);
}

QString CXMLResp::GetDescr()
{
  return m_DADescr.value();
}

const char CXMLCmd::XMLATTR_ACK[] = "ack";

CXMLCmd::CXMLCmd(const QString& sProtocol) :
  CXMLBase(sProtocol, CXMLData::XML_COMMAND)
{
  m_DACmdName = GetDoc().createAttribute(CXMLData::XMLATTR_CMDNAME);
  m_DAACK = GetDoc().createAttribute(XMLATTR_ACK);

  GetMainNode().setAttributeNode(m_DACmdName);
  GetMainNode().setAttributeNode(m_DAACK);
  MarkACK(true);
}

CXMLCmd::CXMLCmd(const CXMLData::TXMLData& pData) :
  CXMLBase(pData)
{
  m_DACmdName = GetMainNode().attributeNode(CXMLData::XMLATTR_CMDNAME);
  TVA_CHECK(!m_DACmdName.isNull(), QObject::tr(
          "No name attribute in XML command"), E_FAIL);

  m_DAACK = GetMainNode().attributeNode(XMLATTR_ACK);
  TVA_CHECK(!m_DAACK.isNull(), QObject::tr(
          "No ack attribute in XML command"), E_FAIL);
}

CXMLResp::TXMLResp CXMLCmd::Execute()
{
  TVA_THROW(QObject::tr("Exuecution is not allowed"), E_FAIL);
}

void CXMLCmd::Answer(CXMLResp::TXMLResp& pResp)
{
  TVA_THROW(QObject::tr("Answer is not allowed"), E_FAIL);
}

QString CXMLCmd::GetCmdName()
{
  return m_DACmdName.nodeValue();
}

void CXMLCmd::SetCmdName(const QString& sCmdName)
{
  m_DACmdName.setNodeValue(sCmdName);
}

bool CXMLCmd::IsACK()
{
  return (m_DAACK.value().toLower() == "true");
}

void CXMLCmd::MarkACK(bool bACK)
{
  if (bACK)
    m_DAACK.setNodeValue("true");
  else
    m_DAACK.setNodeValue("false");

}

