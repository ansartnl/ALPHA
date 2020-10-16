#include "xml_storage.h"

#include <set>

#include <QDir>
#include <QFile>
#include <QIODevice>
#include <QTextStream>
#include <QStringList>
#include <QDomDocument>

#include "utils.h"
#include "file_save_guard.h"


namespace utilities {


namespace {


void XMLTraversal(QDomNode* node, const QString& str, XMLStorage::StorageMap& map) {
    QDomElement element(std::move(node->toElement()));
    QString tag(std::move(element.tagName()));

    if (tag == kPropertyMapTag) {
        QVariantMap map_value;
        QDomElement current_element = element.firstChild().toElement();

        while (!current_element.isNull()) {
            QString key = current_element.attribute(kNameAttribute);
            if (!key.isEmpty())
                map_value[key] = std::move(current_element.text());
            current_element = current_element.nextSiblingElement();
        }

        map[(str.isEmpty() ? str : str + "/") + element.attribute(kNameAttribute)] = std::move(map_value);
    } else if (tag == kPropertyListTag) {
        QVariantList list_value;
        QDomElement current_element = element.firstChild().toElement();

        while (!current_element.isNull()) {
            list_value.push_back(std::move(current_element.text()));
            current_element = current_element.nextSiblingElement();
        }

        map[(str.isEmpty() ? str : str + "/") + element.attribute(kNameAttribute)] = std::move(list_value);
    } else if (!element.hasAttribute(kNameAttribute)) {
        for (QDomNode i = node->firstChild(); !i.isNull(); i = i.nextSibling())
            XMLTraversal(&i, (tag == kConfigTag ? str : (str.isEmpty() ? str : str + "/") + tag), map);
    } else {
        map[(str.isEmpty() ? str : str + "/") + element.attribute(kNameAttribute)]
            = std::move(node->firstChild().toElement().text());
    }
}

void AppendSettingToXML(QDomDocument* doc, QDomNode* node, const QString& key, const QVariant& value) {
    auto appendKeyAndValue = [&doc](const QString & key, const QString & value, QDomNode * node) {
        QDomElement value_el(std::move(doc->createElement(kValueTag)));
        value_el.appendChild(std::move(doc->createTextNode(value)));
        QDomElement key_el(std::move(doc->createElement(kPropertyTag)));
        key_el.setAttribute(kNameAttribute, key);
        key_el.appendChild(std::move(value_el));
        node->appendChild(std::move(key_el));
    };

    if (value.type() == QVariant::Map) {
        auto myMap(std::move(value.toMap()));
        if (!myMap.empty()) {
            QDomElement map_node(std::move(doc->createElement(kPropertyMapTag)));
            map_node.setAttribute(kNameAttribute, key);

            auto end_it = myMap.constEnd();
            for (auto it = myMap.constBegin(); it != end_it; ++it)
                appendKeyAndValue(it.key(), std::move(it.value().toString()), &map_node);

            node->appendChild(std::move(map_node));
        }
    } else if (value.type() == QVariant::List) {
        auto myList(std::move(value.toList()));
        if (!myList.empty()) {
            QDomElement list_node(std::move(doc->createElement(kPropertyListTag)));
            list_node.setAttribute(kNameAttribute, key);

            auto end_it = myList.constEnd();
            for (auto it = myList.constBegin(); it != end_it; ++it) {
                QDomElement value_el(std::move(doc->createElement(kValueTag)));
                value_el.appendChild(std::move(doc->createTextNode(it->toString())));
                list_node.appendChild(std::move(value_el));
            }

            node->appendChild(std::move(list_node));
        }
    } else {
        appendKeyAndValue(key, std::move(value.toString()), node);
    }
}


} // namespace


QVariant XMLStorage::get(const QString& key) const {
    auto it = storage_.find(key);
    return (it != storage_.end() ? it->second : std::move(QVariant()));
}

void XMLStorage::load() {
    if (filename_.isEmpty())
        setFilename(PrepareCacheFolder() + kDefaultXMLFileName);

    QFile file(filename_);
    if (file.exists() && file.open(QIODevice::ReadOnly)) {
        diskToStorage(&file);
        file.close();
    }
}

void XMLStorage::save() const {
    if (filename_.isEmpty()) return;

    FileSaveGuard fileSafer(filename_);

    QFile file(filename_);
    if (file.open(QIODevice::WriteOnly)) {
        storageToDisk(&file);
        file.close();
    }
}

void XMLStorage::storageToDisk(QFile* file) const {
    QDomDocument xml;
    xml.appendChild(xml.createElement(kConfigTag));

    auto end_it = storage_.end();
    for (auto it = storage_.begin(); it != end_it; ++it) {
        QDomNode cur_node = xml.firstChild();
        QStringList key_path = it->first.split(kNestingSeparator);
        int key_path_size = key_path.size() - 1;
        for (int j = 0; j < key_path_size; ++j) {
            QDomElement el = cur_node.firstChildElement(key_path[j]);
            if (!el.isNull()) {
                cur_node = el;
            } else {
                cur_node.appendChild(xml.createElement(key_path[j]));
                cur_node = cur_node.firstChildElement(key_path[j]);
            }
        }
        AppendSettingToXML(&xml, &cur_node, key_path.last(), it->second);
    }

    QTextStream out(file);
    xml.save(out, 4);
}

void XMLStorage::diskToStorage(QFile* file) {
    QDomDocument xml;
    xml.setContent(file);
    QDomElement rootNode = xml.firstChildElement(kConfigTag);
    XMLTraversal(&rootNode, std::move(QString()), storage_);
}


} // namespace common
