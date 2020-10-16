#include "registry_storage.h"

#include "settings.h"


namespace settings {


template <typename F>
void RegistryStorage::StorageTraversal(F func) {
    for (auto it = storage_.begin(); it != storage_.end(); ++it) {
        QStringList key_path = it->first.split("/");
        if (key_path.size() < 2) throw /*std::exception("Bad registry path")*/ "Bad registry path";
        QSettings registry(key_path[0], (key_path.size() > 2 ? key_path[1] : QString()));
        key_path.erase(key_path.begin());
        if (key_path.size() > 1) key_path.erase(key_path.begin());
        func(&registry, it->first, it->second, key_path.join("/"));
    }
}

QVariant RegistryStorage::Get(const QString& key) const {
    auto it = storage_.find(key);
    return (it != storage_.end() ? it->second : QVariant());
}

struct LambdaDesugarerSave {
    LambdaDesugarerSave(std::map<QString, QVariant>& storage): _storage(storage) {}

    void operator()(QSettings* registry, const QString& /*key*/, const QVariant& value, const QString& path) {
        registry->setValue(path, value);
    }

private:
    std::map<QString, QVariant>& _storage;
};

struct LambdaDesugarerLoad {
    LambdaDesugarerLoad(std::map<QString, QVariant>& storage): _storage(storage) {}

    void operator()(QSettings* registry, const QString& key, const QVariant& /*value*/, const QString& path) {
        _storage[key] = registry->value(path);
    }

private:
    std::map<QString, QVariant>& _storage;
};

struct LambdaDesugarerLoadKey {
    LambdaDesugarerLoadKey(std::map<QString, QVariant>& storage, const QString& the_key): _storage(storage), _the_key(the_key) {}

    void operator()(QSettings* registry, const QString& key, const QVariant& /*value*/, const QString& path) {
        if (key == _the_key)
            _storage[key] = registry->value(path);
    }

private:
    std::map<QString, QVariant>& _storage;
    const QString& _the_key;
};

struct LambdaDesugarerLoadSettingsName {
    LambdaDesugarerLoadSettingsName(std::map<QString, QVariant>& storage): _storage(storage) {}

    void operator()(const QString& settingName) {
        _storage[settingName] = QVariant();
    }

private:
    std::map<QString, QVariant>& _storage;
};


void RegistryStorage::Save() {
    StorageTraversal(LambdaDesugarerSave(storage_));
}

void RegistryStorage::Load() {
    std::for_each(getSettingsSetForStorage<RegistryStorage>().begin(),
                  getSettingsSetForStorage<RegistryStorage>().end(),
                  LambdaDesugarerLoadSettingsName(storage_));
}

void RegistryStorage::Load(const QString& the_key) {
    StorageTraversal(LambdaDesugarerLoadKey(storage_, the_key));
}


} // namespace settings
