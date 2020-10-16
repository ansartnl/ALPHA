#pragma once

#include <map>
#include <set>
#include <algorithm>
#include <type_traits>
#include <functional>

#include <QVariant>
#include <QStringList>

#include <utilities/utils.h>
#include <utilities/singleton.h>

#include "storage_interface.h"
#include "xml_storage.h"
#include "registry_storage.h"
#include "memory_storage.h"
#include "settings_global.h"


namespace settings {


class SETTINGS_EXPORT TransformsSet : public std::map<QString, void(*)()> {};


SETTINGS_EXPORT TransformsSet& GetTransforms();


template <class StorageType>
struct StorageNamer;

template <>
struct StorageNamer<XMLStorage>;


class SETTINGS_EXPORT MultiSettings {
public:
    typedef std::set<StorageInterface*> StoragesSet;

    MultiSettings();

    template <class Key>
    void Set(const typename Key::value_t& value); // no implementation

    template <class Key>
    typename Key::value_t Get() const; // no implementation

    template <class Key>
    void Remove();

    template <class StorageType>
    inline void Save();

    void SaveAll();

    template <class StorageType>
    inline void Load();
    template <class StorageType>
    inline void Load(QDomNode* root);

    void LoadAll();

    template <class Storage_t>
    static void registerStorage() {allStorages().insert(&getStorage<Storage_t>());}
    template<class Setting_t>
    static void transformSetting();

private:
    friend struct StorageNamer<XMLStorage>;

    template <class Storage_t>
    static Storage_t& getStorage() {
        static Storage_t the_storage;
        return the_storage;
    }

    static StoragesSet& allStorages();

    void transformOldSettings();
};


class SETTINGS_EXPORT Settings : public utilities::DestroyableSingleton<MultiSettings> {};


template <class StorageType>
inline void MultiSettings::Save() {getStorage<StorageType>().Save();}
template <class StorageType>
inline void MultiSettings::Load() {getStorage<StorageType>().Load();}
template <class StorageType>
inline void MultiSettings::Load(QDomNode* root) {getStorage<StorageType>().Load(root);}

// internal use function to define xml groups to read as QVariantMaps
class SETTINGS_EXPORT SettingGroupMap : public std::map<QString, QVariantMap(*)()> {};
SETTINGS_EXPORT SettingGroupMap& getSettingGroup_QVariantMaps();

// internal use function to define xml groups to read as QVariantLists
class SETTINGS_EXPORT SettingListMap : public std::map<QString, QVariantList(*)()> {};
SETTINGS_EXPORT SettingListMap& getSettingGroup_QVariantLists();

//  internal use function to define settings set for each storage type
class SETTINGS_EXPORT SettingGroupSet : public std::set<QString> {};
template<class StorageType>
inline SettingGroupSet& getSettingsSetForStorage() {
    static SettingGroupSet settingsSet;
    return settingsSet;
}

// path nodes of a 'key' parameter should be separated by '_'
inline QString prepareSettingKey(const char* key) {
    return QString(key).replace("PROJECTNAME", PROJECT_NAME).replace("_DASH_", "-").replace("_", "/").replace("//", "_");
}

#define PREPARE_KEY(key) prepareSettingKey(#key)

template <class Key, class Val_t, class StorageType>
struct SettingsPreInitializer {
    SettingsPreInitializer(const QString& keyStr) {
        Settings::registerStorage<StorageType>();
        getSettingsSetForStorage<StorageType>().insert(keyStr);
    }
};

template <class Key, class StorageType>
struct SettingsPreInitializer <Key, QVariantMap, StorageType> {
    SettingsPreInitializer(const QString& keyStr) {
        Settings::registerStorage<StorageType>();
        getSettingGroup_QVariantMaps().insert(std::make_pair(keyStr, &SettingsPreInitializer::getValue));
        getSettingsSetForStorage<StorageType>().insert(keyStr);
    }

    static QVariantMap getValue() {
        return Settings::instance()->Get<Key>();
    }
};

template <class Key, class StorageType>
struct SettingsPreInitializer <Key, QVariantList, StorageType> {
    SettingsPreInitializer(const QString& keyStr) {
        Settings::registerStorage<StorageType>();
        getSettingGroup_QVariantLists().insert(std::make_pair(keyStr, &SettingsPreInitializer::getValue));
        getSettingsSetForStorage<StorageType>().insert(keyStr);
    }

    static QVariantList getValue() {
        return Settings::instance()->Get<Key>();
    }
};


template <typename value_type, bool>
struct SettingsDefaultMapper {
    template <typename DefTy>
    value_type operator()(const DefTy& func) {return func();}
};
template <typename value_type>
struct SettingsDefaultMapper<value_type, true> {
    template <typename DefTy>
    value_type operator()(const DefTy& var) {return static_cast<value_type>(var);}
};

template <>
struct StorageNamer<XMLStorage> {
    StorageNamer(const QString& storageName) {
        auto& storage = Settings::getStorage<XMLStorage>();
        storage.setFilename(storageName);
    }
};


#define SET_XML_STORAGE_NAME(name)                      \
    namespace settings {                                \
    static StorageNamer<XMLStorage> storageNamer(name); \
    }


#define DECLARE_SETTING(key, value_type, default_value, storage_type)                                           \
    namespace settings {                                                                                        \
    struct key {typedef value_type value_t;};                                                                   \
    static SettingsPreInitializer<key, value_type, storage_type>                                                \
    settingPreinitializer##key(PREPARE_KEY(key));                                                               \
    template <>                                                                                                 \
    inline key::value_t MultiSettings::Get<key>() const {                                                       \
        storage_type* the_storage = &getStorage<storage_type>();                                                \
        QVariant res = the_storage->Get(PREPARE_KEY(key));                                                      \
        if (!res.isValid()) {                                                                                   \
            auto temp = default_value;                                                                          \
            the_storage->Set(PREPARE_KEY(key),                                                                  \
                             res = QVariant(SettingsDefaultMapper<value_type,                                   \
                                            std::is_convertible<decltype(temp),value_type>::value>()(temp)));   \
        }                                                                                                       \
        return res.value<value_type>();                                                                         \
    }                                                                                                           \
    template <>                                                                                                 \
    inline void MultiSettings::Set<key>(const typename key::value_t& value) {                                   \
        getStorage<storage_type>().Set(PREPARE_KEY(key),QVariant(value));                                       \
    }                                                                                                           \
    template <>                                                                                                 \
    inline void MultiSettings::Remove<key>()    {                                                               \
        getStorage<storage_type>().Remove(PREPARE_KEY(key));                                                    \
    }                                                                                                           \
    }                                                                                                           \
    using ::settings::key;


#define DECLARE_SETTING_CALLBACK_SET_GET(key, value_type, set_callback, get_callback, storage_type)                             \
    namespace settings {                                                                                                        \
    struct key {typedef value_type value_t;};                                                                                   \
    static SettingsPreInitializer<key, value_type, storage_type>                                                                \
    settingPreinitializer##key(PREPARE_KEY(key));                                                                               \
    template <>                                                                                                                 \
    inline key::value_t MultiSettings::Get<key>() const {                                                                       \
        storage_type* the_storage = &getStorage<storage_type>();                                                                \
        QVariant res = the_storage->Get(PREPARE_KEY(key));                                                                      \
        QVariant temp = QVariant(get_callback(res));                                                                            \
        return temp.value<value_type>();                                                                                        \
    }                                                                                                                           \
    template <>                                                                                                                 \
    inline void MultiSettings::Set<key>(const typename key::value_t& value) {                                                   \
        QVariant tmp = QVariant(set_callback(value));                                                                           \
        getStorage<storage_type>().Set(PREPARE_KEY(key),tmp);                                                                   \
    }                                                                                                                           \
    template <>                                                                                                                 \
    inline void MultiSettings::Remove<key>()    {                                                                               \
        getStorage<storage_type>().Remove(PREPARE_KEY(key));                                                                    \
    }                                                                                                                           \
    }                                                                                                                           \
    using ::settings::key;


#define DECLARE_SETTING_BIND(key, value_type, ValueRetriever)                                                   \
    namespace settings {                                                                                        \
    struct key { typedef value_type value_t; };                                                                 \
    template <> inline value_type MultiSettings::Get<key>() const { return ValueRetriever(); }                  \
    }                                                                                                           \
    using ::settings::key;


#define DECLARE_SETTING_TRANSFORMATION(old_key, old_value_type, old_storage_type, convertion_lambda, new_key)   \
    namespace settings {                                                                                        \
    struct old_key {};                                                                                          \
    static SettingsPreInitializer<old_key, old_value_type, old_storage_type>                                    \
    settingPreinitializer##old_key(PREPARE_KEY(old_key));                                                       \
    template <>                                                                                                 \
    inline static void MultiSettings::transformSetting<old_key>() {                                             \
        old_storage_type* the_storage = &getStorage<old_storage_type>();                                        \
        QVariant res = the_storage->Get(PREPARE_KEY(old_key));                                                  \
        if (res.isValid()) {                                                                                    \
            Settings::instance()->Set<new_key>(convertion_lambda(res.value<old_value_type>()));                 \
            the_storage->Remove(PREPARE_KEY(old_key));                                                          \
        }                                                                                                       \
    }                                                                                                           \
    static struct TransformsInitializer##old_key{                                                               \
        TransformsInitializer##old_key() {                                                                      \
            GetTransforms().insert(std::make_pair(PREPARE_KEY(old_key),                                         \
                                                  static_cast<void(*)()> (MultiSettings::transformSetting<old_key>)));  \
        }                                                                                                       \
    } transformsInitializer##old_key;                                                                           \
    }                                                                                                           \
    using ::settings::old_key;


#define GET_SETTING(key)        ::settings::Settings::instance()->Get<key>()
#define SET_SETTING(key, value) ::settings::Settings::instance()->Set<key>(value)


} // namespace settings
