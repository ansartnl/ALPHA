#include "XML.h"

namespace xml
{

	const char CXMLList::XML_ELEMENT[] = "element";
	CXMLList::CXMLList(const QDomElement& Element)
		: m_MainElement(Element)
	{
	}

	void CXMLList::Begin()
	{
		m_Element = m_MainElement.firstChildElement();
	}

	bool CXMLList::IsDone() const
	{
		return m_Element.isNull();

	}
	void CXMLList::Next()
	{
		do
		{
			m_Element = m_Element.nextSiblingElement();
		}
		while (!m_Element.isNull() && m_Element.nodeName() != XML_ELEMENT);
	}

	QDomElement CXMLList::GetElement()
	{
		return m_Element;
	}
}
