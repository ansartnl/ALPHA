#pragma once

#include "storage_interface.h"

#include <map>

#include <QString>
#include <QVariant>

#include "settings_global.h"


namespace settings {


class SETTINGS_EXPORT MemoryStorage : public StorageInterface {
public:
    virtual QVariant Get(const QString& key) const override;
    virtual void Set(const QString& key, const QVariant& value) override {storage_[key] = value;}
    virtual void Remove(const QString& key) override {storage_.erase(key);};
    virtual void Load() override {}
    virtual void Load(QDomNode* /*root*/) override {}
    virtual void Load(const QString& /*key*/) override {}
    virtual void Save() override {}
    virtual bool IsEmpty() override { return storage_.empty(); };

private:
    std::map<QString, QVariant> storage_;
};


} // namespace settings
