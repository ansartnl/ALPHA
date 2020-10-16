#pragma once

class QString;
class QVariant;
class QDomNode;

namespace settings {


class StorageInterface {
public:
    virtual QVariant Get(const QString& key) const = 0;
    virtual void Set(const QString& key, const QVariant& value) = 0;
    virtual void Load() = 0;
    virtual void Load(QDomNode* root) = 0;
    virtual void Load(const QString& key) = 0;
    virtual void Remove(const QString& key) = 0;
    virtual void Save() = 0;
    virtual bool IsEmpty() = 0;
};


} // namespace settings
