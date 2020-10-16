#include "qtxmlsettings.h"

#include <QtXml/QDomDocument>

#include <QtCore/QCoreApplication>

#include <QtCore/QDebug>

const QSettings::Format QtXmlSettings::xml_format = QSettings::registerFormat(
  "xml", readXmlFile, writeXmlFile);

static void writeXmlAttribute(QDomElement &domElement, QString key,
                              const QVariant &value);
static void readXmlAttributes(QString key, QSettings::SettingsMap &map,
                              QDomNode &domElement);


bool QtXmlSettings::isAttribute(const QString &key)
{
    return !key.isEmpty() && key[0] == QChar('*');
}

QString QtXmlSettings::makeAttributeName(const QString &key)
{
    return QString("*%1").arg(key);
}

bool QtXmlSettings::readXmlFile(QIODevice &device, QSettings::SettingsMap &map)
{
  QDomDocument doc("");
  if (!doc.setContent(&device))
    return false;

  QDomElement root = doc.documentElement();

  processReadKey("", map, root);

  return true;
}

bool QtXmlSettings::writeXmlFile(QIODevice &device,
  const QSettings::SettingsMap &map)
{

  QDomDocument doc("");
  QDomElement root = doc.createElement("root");
  doc.appendChild(root);

  QMapIterator<QString, QVariant> i(map);
  while (i.hasNext()) {
    i.next();

    QString sKey = i.key();

    processWriteKey(doc, root, sKey, i.value());
  };

  QDomNode xmlNode = doc.createProcessingInstruction("xml",
    "version=\"1.0\" encoding=\"UTF-8\"");
  doc.insertBefore(xmlNode, doc.firstChild());

  QTextStream out(&device);
  doc.save(out, 4);

  return true;
}

void QtXmlSettings::processWriteKey(QDomDocument& doc, QDomElement& domElement, QString key, const QVariant& value)
{
    int slashPos = key.indexOf('/');

    // If it is key
    if (slashPos < 0)
    {
        // If the only one key is attribute - do nothing.
        if ( QtXmlSettings::isAttribute(key) )
            return;

        // DONT USE key with name 'size'
        if (key == "size")
            return;
        if (key == "comment")
        {
            QDomComment valueText = doc.createComment(value.toString());
            domElement.appendChild(valueText);
            return;
        }
        else
        {
            // Write to child element
            QDomElement keyElement = doc.createElement(key);
            QDomText valueText = doc.createTextNode(value.toString());
            keyElement.appendChild(valueText);
            domElement.appendChild(keyElement);
            return;
        }
    };

    // get group name
    QString groupName = key.left(slashPos);
    // if name is aircraftNumber then it's row of array, convert to row_?
    if (groupName.toInt())
    {
        groupName = "row_" + groupName;
        domElement.toElement().setAttribute("table", "1");
    };

    // find, create node for key
    QDomElement groupElement;
    QDomNodeList findedNodes = domElement.elementsByTagName(groupName);

    if (!findedNodes.count()) // Заданный элемент-нода не найден - создаем его
    {
        groupElement = doc.createElement(groupName);
        domElement.appendChild(groupElement);
    }
    else
    {
        Q_ASSERT(findedNodes.count() >= 1); // can not create
        groupElement = findedNodes.item(0).toElement();
    };

    // готовим обрезанную часть ключа
    key = key.right(key.size() - slashPos - 1);

    // Write atribute for group node and correct key.
    if ( QtXmlSettings::isAttribute(key) )
    {
        writeXmlAttribute(groupElement, key, value);

        slashPos = key.indexOf('/');
        // We done attribute is the last element needs to be written.
        if ( slashPos < 0 )
            return;
        else
            key = key.right(key.size() - slashPos - 1);
    }

    // продолжение обработки (создание/поиск групп) - пока не найдется параметр
    processWriteKey(doc, groupElement, key, value);
}

void writeXmlAttribute(QDomElement &domElement, QString key, const QVariant &value)
{
    Q_ASSERT(!key.isEmpty() && key[0] == QChar('*'));

    // Remove '*' special symbol to distinguish attribute from text node element.
    key.remove(0,1);

    if ( !key.isEmpty() )
    {
        int slashPos = key.indexOf('/');
        if ( slashPos > 0 )
            key = key.right(key.size() - slashPos - 1);

        domElement.setAttribute(key, value.toString());
    }
}

void QtXmlSettings::processReadKey(QString key, QSettings::SettingsMap &map, QDomElement& domElement)
{
    QDomNodeList nlChild = domElement.childNodes();

    // if element is table, than all child is rows
    bool isTable = domElement.attribute("table", "0").toInt();
    // create settings value for size
    if (isTable)
        map.insert(key + "size", nlChild.count());

    //
    for (int i = 0; i < nlChild.count(); ++i)
    {
        QDomNode childNode = nlChild.item(i);
        QString childName = childNode.toElement().tagName();
        // if element name contain row_ it array item
        if (childName.startsWith("row_"))
        {
            childName = childName.mid(4);
            // If element is text add it to map
        }
        else if (childNode.isText())
        {
            QString value = childNode.toText().data();
            QString workkey = key;
            workkey.chop(1);
            // remove slash from end
            map.insert(workkey, value.trimmed());
            continue;
        }
        else if (childNode.isComment())
        {
            childName = "comment";
            QString value = childNode.toComment().data();
            QString workkey = key + "comment";
            map.insert(workkey, value);
            continue;
        }

        QString subKey = key + childName + "/";
        QDomElement subElement = childNode.toElement();

        // Read attributes for this node.
        if ( childNode.hasAttributes() )
            readXmlAttributes(subKey, map, childNode);

        processReadKey(subKey, map, subElement);
    };
}

void readXmlAttributes(QString key, QSettings::SettingsMap &map, QDomNode& domElement)
{
    QDomNamedNodeMap attrNodes = domElement.attributes();
    for (int i = 0; i < attrNodes.count(); i++)
    {
        QDomAttr attrNode = attrNodes.item(i).toAttr();

        QString workkey = key + QtXmlSettings::makeAttributeName( attrNode.name() );
        // Remove spaces from the end.
        map.insert(workkey, attrNode.value().trimmed());
    }
}
