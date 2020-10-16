#include "nitaxmlsettings.h"

#include <QtCore/QCoreApplication>
#include <QtCore/QTextStream>

#include <QtXml/QDomDocument>

const QSettings::Format NitaXmlSettings::kXmlFormat = QSettings::registerFormat(
            "xml", readXmlFile, writeXmlFile);

const QString NitaXmlSettings::kSeparator = "/";

const QString NitaXmlSettings::kName = "n";
const QString NitaXmlSettings::kValue = "v";
const QString NitaXmlSettings::kDescription = "_d";
const QString NitaXmlSettings::kUnit = "_u";
const QString NitaXmlSettings::kRange = "_r";


const QString NitaXmlSettings::description(const QString key)
{
    return key + kSeparator + kDescription;
}

const QString NitaXmlSettings::unit(const QString key)
{
    return key + kSeparator + kUnit;
}

const QString NitaXmlSettings::range(const QString key)
{
    return key + kSeparator + kRange;
}

bool NitaXmlSettings::readXmlFile(QIODevice &device, QSettings::SettingsMap &map)
{
    QDomDocument doc("");
    if (!doc.setContent(&device))
        return false;

    QDomElement root = doc.documentElement();

    processReadKey("", map, root);

    return true;
}

void NitaXmlSettings::processReadKey(const QString &key, QSettings::SettingsMap &map, QDomElement &domElement)
{
    const QDomNodeList nlChild = domElement.childNodes();

    for (int i = 0; i < nlChild.count(); ++i) {
        const QDomNode childNode = nlChild.item(i);
        const QDomNamedNodeMap attributes = childNode.attributes();

        if (!attributes.contains(kName))
            continue;
        const QString name = attributes.namedItem(kName).toAttr().value();
        if (attributes.contains(kValue)) {
            // parameter
            map.insert(key + name, attributes.namedItem(kValue).toAttr().value());
            if (attributes.contains(kDescription)) {
                map.insert(key + name + kSeparator + kDescription
                           , attributes.namedItem(kDescription).toAttr().value());
            }
            if (attributes.contains(kUnit)) {
                map.insert(key + name + kSeparator + kUnit
                           , attributes.namedItem(kUnit).toAttr().value());
            }
            if (attributes.contains(kRange)) {
                map.insert(key + name + kSeparator + kRange
                           , attributes.namedItem(kRange).toAttr().value());
            }
        } else {
            // group
            if (attributes.contains(kDescription)) {
                map.insert(key + name + kSeparator + kDescription
                           , attributes.namedItem(kDescription).toAttr().value());
            }
            QDomElement subElement = childNode.toElement();
            processReadKey(key + name + kSeparator, map, subElement);
        }
    };
}

bool NitaXmlSettings::writeXmlFile(QIODevice &device, const QSettings::SettingsMap &map)
{
    QDomDocument doc("");
    QDomElement root = doc.createElement("root");
    root.setAttribute("ver", "1.1");
    doc.appendChild(root);

    const QList < QString > keys = map.keys();
    foreach (const QString &key, keys) {
        processWriteKey(doc, root, key, map.value(key));
    };
    changeTagName(root);

    QDomNode xmlNode = doc.createProcessingInstruction("xml", "version=\"1.0\" encoding=\"windows-1251\"");
    doc.insertBefore(xmlNode, doc.firstChild());

    QTextStream out(&device);
    doc.save(out, 4);

    return true;
}

void NitaXmlSettings::processWriteKey(QDomDocument &doc, QDomElement &domElement
                                      , const QString &key, const QVariant &value)
{
    int pos = key.indexOf(kSeparator);
    if (pos < 0) {
        // value
        insertAttribute(doc, domElement, key, kValue, value);
        return;
    }
    const QString name = key.left(pos);
    const QString newKey = key.right(key.size() - pos - 1);
    if (newKey.indexOf(kSeparator) < 0) {
        // last
        if (newKey == kDescription) {
            insertAttribute(doc, domElement, name, kDescription, value);
        } else if (newKey == kUnit) {
            insertAttribute(doc, domElement, name, kUnit, value);
        } else if (newKey == kRange) {
            insertAttribute(doc, domElement, name, kRange, value);
        } else {
            // value
            QDomElement groupElement;
            QDomNodeList elements = domElement.elementsByTagName(name);
            if (elements.isEmpty()) {
                groupElement = doc.createElement(name);
                groupElement.setAttribute(kName, name);
                domElement.appendChild(groupElement);
            } else {
                groupElement = elements.item(0).toElement();
            }
            processWriteKey(doc, groupElement, newKey, value);
        }
    } else {
        //not last
        QDomElement groupElement;
        QDomNodeList elements = domElement.elementsByTagName(name);
        if (elements.isEmpty()) {
            groupElement = doc.createElement(name);
            groupElement.setAttribute(kName, name);
            domElement.appendChild(groupElement);
        } else {
            groupElement = elements.item(0).toElement();
        }
        processWriteKey(doc, groupElement, newKey, value);
    }
}

void NitaXmlSettings::insertAttribute(QDomDocument &doc, QDomElement &domElement
                                      , const QString &key, const QString &attrKey, const QVariant &value)
{
    QDomNodeList children = domElement.childNodes();
    for (int i = 0; i < children.count(); ++i) {
        QDomNode node = children.item(i);
        if (node.isElement()) {
            QDomElement element = node.toElement();
            if (element.tagName() == key) {
                element.setAttribute(attrKey, value.toString());
                return;
            }
        }
    }
    QDomElement keyElement = doc.createElement(key);
    keyElement.setAttribute(kName, key);
    keyElement.setAttribute(attrKey, value.toString());
    domElement.appendChild(keyElement);
}

void NitaXmlSettings::changeTagName(QDomElement &domElement)
{
    domElement.setTagName("n");
    QDomNodeList children = domElement.childNodes();
    for (int i = 0; i < children.count(); ++i) {
        QDomNode node = children.item(i);
        if (node.isElement()) {
            QDomElement element = node.toElement();
            changeTagName(element);
        }
    }
}
