#ifndef XML_H
#define XML_H

#include <QtXml/QDomDocument>

//! Namespace contains classes to work with XML
namespace xml
{
	//! Iterator to pass over all XML_ELEMENT tag
	class CXMLList
	{
	public:
		//! Name of an element
		static const char XML_ELEMENT[];
		//! Constuctor
		/*!
			\param Element Dom parent element
		*/
		CXMLList(const QDomElement& Element);
		//! Reset iterator
		void Begin();
		//! Check the end of iterator
		bool IsDone() const;
		//! Next element
		void Next();
		//! Get current dom element
		QDomElement GetElement();

	private:
		QDomElement		m_MainElement;
		QDomElement		m_Element;
	};
}
#endif // XML_H
