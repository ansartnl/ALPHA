#ifndef XMLCMD_H
#define XMLCMD_H

#include <QtCore/QSharedPointer>

#include <QtXml/QDomDocument>

//! Base class for XML commands & responses
class CXMLData
{
public:
	//! Smart pointer to CXMLData
	typedef QSharedPointer<CXMLData> TXMLData;

	/*! \name String constants */
	//@{
	static const char XML_COMMAND[];
	static const char XML_RESPONSE[];
	static const char XMLATTR_ID[];
	static const char XMLATTR_CMDNAME[];
	//@}

	/*! Constructor. Create DOM structure */
	/*!
		\param sProtocol protocol name
		\param sMainName main node name in XML data
	*/
	CXMLData(const QString& sProtocol, const QString& sMainName);
	/*! Constructor. Parse buffer in XML structure */
	/*!
		\param Buffer buffer for parsing
	*/
	explicit CXMLData(const QByteArray& Buffer);
	/*! Destructor */
	virtual ~CXMLData(){};

	/*! Define whether XML data is a command or response*/
	/*!
		\return whether XML data is a command
	*/
	bool IsCmd() const;
	/*! Set identificator of packet */
	/*!
		\param uiId identificator
	*/
	void SetID(quint32 uiId);
	/*! Get identificator of packet */
	/*!
		\return uiId identificator
	*/
	quint32 GetID() const;
	/*! Get protocol name */
	/*!
		\return protocol name
	*/
	QString GetProtoName();
	/*! Get command name */
	/*!
		\return command name
	*/
	QString GetCmdName();
	/*! Get dom node of the main element */
	/*!
		\return main dom node
	*/
	QDomElement& GetMainNode() { return m_DNMain; }
	/*! Get dom document */
	/*!
		\return dom document
	*/
	QDomDocument& GetDoc() { return m_DomDoc; }
	/*! Transformation of XML data to data buffer */
	/*!
		\return data buffer
	*/
	QByteArray ToByteArray();
	/*! Transformation of XML data to QString */
	/*!
		\return string
	*/
	QString ToString();

private:
	QDomDocument	m_DomDoc;
	QDomElement		m_DEProtocol;
	QDomAttr		m_DAID;
	QDomElement		m_DNMain;
};

//! Wrapper of CXMLData class
/*! Class CXMLBase contain smart pointer to CXMLData class*/
class CXMLBase
{
public:
	//! Smart pointer to CXMLData
	typedef QSharedPointer<CXMLBase> TXMLBase;

	/*! Constructor. Create DOM structure */
	/*!
		\param sProtocol protocol name
		\param sMainName main node name in XML data
	*/
	CXMLBase(const QString& sProtocol, const QString& sMainName);
	/*! Constructor. Parse buffer in XML structure */
	/*!
		\param pData smart pointer to CXMLData
	*/
	explicit CXMLBase(const CXMLData::TXMLData& pData);
	/*! Destructor */
	virtual ~CXMLBase() {};

	/*! Set identificator of packet */
	/*!
		\param uiId identificator
	*/
	void SetID(quint32 uiId) { m_pData->SetID(uiId); }
	/*! Get identificator of packet */
	/*!
		\return uiId identificator
	*/
	quint32 GetID() const { return m_pData->GetID();}
	/*! Get dom node of the main element */
	/*!
		\return main dom node
	*/
	QDomElement& GetMainNode() { return m_pData->GetMainNode(); }
	/*! Get dom document */
	/*!
		\return dom document
	*/
	QDomDocument& GetDoc() { return m_pData->GetDoc(); }
	/*! Transformation of XML data to data buffer */
	/*!
		\return data buffer
	*/
	QByteArray ToByteArray() const { return m_pData->ToByteArray(); }
	/*! Transformation of XML data to QString */
	/*!
		\return string
	*/
	QString ToString() const { return m_pData->ToString(); }


private:
	CXMLData::TXMLData m_pData;
};

class CXMLCmd;

//! Base class for XML responses
class CXMLResp
	: public CXMLBase
{
public:
	//! Smart pointer to CXMLResp
	typedef QSharedPointer<CXMLResp> TXMLResp;
	/*! \name String constants */
	//@{
	static const char XMLATTR_CODE[];
	static const char XMLATTR_DESCR[];
	//@}
	/*! Constructor. Create XML response */
	/*!
		\param sProtocol protocol name
		\param refCmd reference to CXMLCmd
	*/
	CXMLResp(const QString& sProtocol, CXMLCmd& refCmd);
	/*! Constructor. Parse CXMLData in CXMLResp */
	/*!
		\param pData smart pointer to CXMLData
	*/
	CXMLResp(const CXMLData::TXMLData& pData);
	/*! Set error code in response */
	/*!
		\param iCode error code. 0 - successfully
	*/
	void SetCode(int iCode);
	/*! Get error code from response */
	/*!
		return error code
	*/
	int GetCode();
	/*! Set description of the error */
	/*!
		\param sDescr error description
	*/
	void SetDescr(const QString& sDescr);
	/*! Get description of the error */
	/*!
		return error description
	*/
	QString GetDescr();

private:
	QDomAttr		m_DACode;
	QDomAttr		m_DADescr;
};

//! Base class for XML command
class CXMLCmd
	: public CXMLBase
{
public:
	//! Smart pointer to CXMLCmd
	typedef QSharedPointer<CXMLCmd> TXMLCmd;
	/*! \name String constants */
	//@{
	static const char XMLATTR_ACK[];
	//@}
	/*! Constructor. Create XML command */
	/*!
		\param sProtocol protocol name
	*/
	CXMLCmd(const QString& sProtocol);
	/*! Constructor. Parse CXMLData in CXMLCmd */
	/*!
		\param pData smart pointer to CXMLData
	*/
	CXMLCmd(const CXMLData::TXMLData& pData);
	/*! \name Interface for command, which should be implemented in descendant class */
	//@{
	/*! Execute the command */
	/*!
		\return response for the command
	*/
	virtual CXMLResp::TXMLResp Execute();
	/*! Process the answer for this command */
	/*!
		\param pResp response for this command
	*/
	virtual void Answer(CXMLResp::TXMLResp& pResp);
	//@}
	/*! Set the command name */
	/*!
		\param sCmdName command name
	*/
	void SetCmdName(const QString& sCmdName);
	/*! Get the command name */
	/*!
		\return command name
	*/
	QString GetCmdName();
	/*! Set acknowledgement necessity */
	/*!
		\param bACK true indicate necessity of a response
	*/
	void MarkACK(bool bACK);
	/*! Get acknowledgement necessity */
	/*!
		\return bACK true indicate necessity of a response
	*/
	bool IsACK();

private:
	QDomAttr		m_DACmdName;
	QDomAttr		m_DAACK;
};

#endif // XMLCMD_H
