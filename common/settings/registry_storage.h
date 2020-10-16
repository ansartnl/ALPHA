#pragma once

#include "storage_interface.h"

#include <map>

#include <QString>
#include <QStringList>
#include <QSettings>
#include <QVariant>

#include "settings_global.h"


namespace settings {


class SETTINGS_EXPORT RegistryStorage : public StorageInterface {
public:
    virtual void Set(const QString& key, const QVariant& value) override {storage_[key] = value;}
    virtual QVariant Get(const QString& key) const override;
    virtual void Remove(const QString& key)override {storage_.erase(key);/*TODO: think on removing from registry*/};
    virtual void Load() override;
    virtual void Load(QDomNode* /*root*/) override {}
    virtual void Load(const QString& the_key) override;
    virtual void Save() override;
    virtual bool IsEmpty() override { return storage_.empty(); };

private:
    template <typename F>
    void StorageTraversal(F func);

    std::map<QString, QVariant> storage_;
};


} // namespace settings
