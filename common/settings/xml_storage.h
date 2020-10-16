#pragma once

#include "storage_interface.h"

#include <QMap>
#include <QString>
#include <QVariant>
#include <QDomElement>
#include <QDomNode>

#include <utilities/utils.h>

#include "settings_global.h"


class QFile;
class QDomDocument;


namespace settings {


const char kXMLFileName[]       = "config.xml";
const char kConfigTag[]         = "Config";
const char kPropertyTag[]       = "property";
const char kValueTag[]          = "value";
const char kNameAttribute[]     = "name";
const char kTypeAttribute[]     = "type";
const char kMapTag[]            = "map";
const char kCodec[]             = "UTF-8";


class SETTINGS_EXPORT XMLStorage : public StorageInterface {
public:
    virtual QVariant Get(const QString& key) const override;
    virtual void Set(const QString& key, const QVariant& value) override {storage_[key] = value;}
    virtual void Remove(const QString& key)override {storage_.erase(key);};
    virtual void Load() override;
    virtual void Load(QDomNode* root) override;
    virtual void Load(const QString& /*key*/) override {/*no need to load from XML again*/}
    virtual bool IsEmpty() override { return storage_.empty(); };
    virtual void Save() override;

    inline void setFilename(const QString& file_name) {file_path_ = file_name;};
    inline const QString& Filename() {return file_path_;};

protected:
    void DiskToStorage(QFile* file);
    void StorageToDisk(QFile* file) const;

    QString file_path_;

    std::map<QString, QVariant> storage_;
};

class XMLPrivateStorage: public XMLStorage {
public:
    virtual void Load() override;
    static QString& userId() {
        static QString user_id;
        return user_id;
    }
};


} // namespace settings
