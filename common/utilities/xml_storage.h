#pragma  once

#include <map>

#include <QString>
#include <QVariant>

#include "utilities_global.h"


class QFile;


namespace utilities {


const char kDefaultXMLFileName[] = "config.xml";
const char kConfigTag[]         = "Config";
const char kPropertyTag[]       = "property";
const char kPropertyMapTag[]    = "property_map";
const char kPropertyListTag[]   = "property_list";
const char kValueTag[]          = "value";
const char kNameAttribute[]     = "name";
const char kNestingSeparator[]  = "/";


class UTILITIES_EXPORT XMLStorage {
public:
    typedef std::map<QString, QVariant> StorageMap;

    QVariant get(const QString& key) const;
    inline void set(const QString& key, const QVariant& value) {storage_[key] = value;}
    inline void remove(const QString& key) {storage_.erase(key);}
    inline bool empty() const {return storage_.empty();};
    inline void clear() {storage_.clear();}

    void load();
    void save() const;

    inline void setFilename(const QString& filename) {filename_ = filename;}
    inline const QString& filename() const {return filename_;}

private:
    void diskToStorage(QFile* file);
    void storageToDisk(QFile* file) const;

    StorageMap storage_;
    QString filename_;
};


} // namespace common
