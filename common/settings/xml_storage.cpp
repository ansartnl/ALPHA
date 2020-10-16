#include "xml_storage.h"

#include <set>

#include <QFile>
#include <QIODevice>
#include <QTextStream>
#include <QStringList>
#include <QDomDocument>

#include <utilities/file_save_guard.h>

#include "settings.h"


namespace settings {


template <typename T>
void XMLTraversal(QDomNode* node, const QString& str, T& map) {
    QString tag = node->toElement().tagName();
    auto setting_map_it = getSettingGroup_QVariantMaps().find(str + "/" + tag);
    auto setting_list_it = getSettingGroup_QVariantLists().find(str + "/" + tag);
    if (setting_map_it != getSettingGroup_QVariantMaps().end()) {
        QString key = str + "/" + tag;
        // map key
        QVariantMap settings_map = setting_map_it->second();
        QDomElement cur_el = node->toElement().firstChild().toElement();
        do {
            QString key = cur_el.attribute(kNameAttribute);
            if (!key.isEmpty()) {
                QString value = cur_el.firstChild().toElement().text();
                settings_map[key] = value;
            }
            cur_el = cur_el.nextSiblingElement();
        } while (!cur_el.isNull());
        map[key] = settings_map;
    } else if (setting_list_it != getSettingGroup_QVariantLists().end()) {
        QString key = str + "/" + tag;
        // list key
        QVariantList settings_list = setting_list_it->second();
        QDomElement cur_el = node->toElement().firstChild().toElement();
        do {
            settings_list.push_back(cur_el.text());
            cur_el = cur_el.nextSiblingElement();
        } while (!cur_el.isNull());
        map[key] = settings_list;
    } else if (!node->toElement().hasAttribute(kNameAttribute)) {
        for (QDomNode i = node->firstChild(); !i.isNull(); i = i.nextSibling())
            XMLTraversal(&i, (tag == kConfigTag ? str : (str.isEmpty() ? str : str + "/") + tag), map);
    } else {
        map[(str.isEmpty() ? str : str + "/") + node->toElement().attribute(kNameAttribute)]
            = node->firstChild().toElement().text();
    }
}

void AppendSettingToXML(QDomDocument* doc, QDomNode* node, const QString& key, const QVariant& value) {
    if (value.type() == QVariant::Map) {
        auto myMap = value.toMap();
        if (!myMap.empty()) {
            QDomElement sectionNode = doc->createElement(key);

            for (auto it = myMap.constBegin(); it != myMap.constEnd(); ++it) {
                QDomElement value_el = doc->createElement(kValueTag);
                value_el.appendChild(doc->createTextNode(it.value().toString()));
                QDomElement key_el = doc->createElement(kPropertyTag);
                key_el.setAttribute(kNameAttribute, it.key());
                key_el.appendChild(value_el);
                sectionNode.appendChild(key_el);
            }
            node->appendChild(sectionNode);
        }
    } else if (value.type() == QVariant::List) {
        auto myList = value.toList();
        if (!myList.empty()) {
            QDomElement sectionNode = doc->createElement(key);

            for (auto it = myList.constBegin(); it != myList.constEnd(); ++it) {
                QDomElement value_el = doc->createElement(kValueTag);
                value_el.appendChild(doc->createTextNode(it->toString()));
                sectionNode.appendChild(value_el);
            }
            node->appendChild(sectionNode);
        }
    } else {
        QDomElement value_el = doc->createElement(kValueTag);
        value_el.appendChild(doc->createTextNode(value.toString()));
        QDomElement key_el = doc->createElement(kPropertyTag);
        key_el.setAttribute(kNameAttribute, key);
        key_el.appendChild(value_el);
        node->appendChild(key_el);
    }
}

QVariant XMLStorage::Get(const QString& key) const {
    auto it = storage_.find(key);
    return (it != storage_.end() ? it->second : QVariant());
}

void XMLStorage::Load() {
    if (file_path_.isEmpty())
        setFilename(utilities::PrepareCacheFolder() + kXMLFileName);

    QFile file(file_path_);
    if (file.exists() && file.open(QIODevice::ReadOnly)) {
        DiskToStorage(&file);
        file.close();
    }
}


void XMLStorage::Load(QDomNode* root) {
    XMLTraversal(root, QString(), storage_);
}

void XMLStorage::Save() {
    if (file_path_.isEmpty()) return;

    utilities::FileSaveGuard fileSafer(file_path_);

    QFile file(file_path_);
    if (file.open(QIODevice::WriteOnly)) {
        StorageToDisk(&file);
        file.close();
    }
}

void XMLStorage::StorageToDisk(QFile* file) const {
    QDomDocument xml;
    xml.appendChild(xml.createElement(kConfigTag));
    for (auto it = storage_.begin(); it != storage_.end(); ++it) {
        QDomNode cur_node = xml.firstChild();
        QStringList key_path = it->first.split("/");
        for (int j = 0; j < key_path.size() - 1; ++j) {
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

    QDomNode node(xml.createProcessingInstruction("xml", QString("version=\"1.0\" encoding=\"%1\"").arg(kCodec)));
    xml.insertBefore(node, xml.firstChild());

    QTextStream out(file);
    out.setCodec(kCodec);
    xml.save(out, 4);
}

void XMLStorage::DiskToStorage(QFile* file) {
    QDomDocument xml;
    xml.setContent(file);
    QDomElement rootNode = xml.firstChildElement(kConfigTag);
    Load(&rootNode);
}


void XMLPrivateStorage::Load() {
    QString str = userId();
    if (userId().isEmpty())
        return;

    //  if (file_path_.isEmpty())
    setFilename(utilities::PrepareCacheFolder() + userId() + QString(".xml"));

    QFile file(file_path_);
    if (file.exists() && file.open(QIODevice::ReadOnly)) {
        DiskToStorage(&file);
        file.close();
    }
}


} // namespace settings
