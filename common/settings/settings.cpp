#include "settings.h"


namespace settings {


TransformsSet& GetTransforms() {
    static TransformsSet trans;
    return trans;
}


MultiSettings::MultiSettings() {
    utilities::InitializeProjectDescription();
    LoadAll();
    transformOldSettings();
}

void MultiSettings::SaveAll() {
    std::for_each(allStorages().begin(), allStorages().end(), std::mem_fun(&StorageInterface::Save));
}

void MultiSettings::LoadAll() {
    std::for_each(allStorages().begin(), allStorages().end(), std::mem_fun(&StorageInterface::Load));
    transformOldSettings();
}

MultiSettings::StoragesSet& MultiSettings::allStorages() {
    static StoragesSet storages;
    return storages;
}

void MultiSettings::transformOldSettings() {
    for (auto it = GetTransforms().begin(); it != GetTransforms().end(); ++it)
        it->second();
}


SettingGroupMap& getSettingGroup_QVariantMaps() {
    static SettingGroupMap settingGroups;
    return settingGroups;
}

SettingListMap& getSettingGroup_QVariantLists() {
    static SettingListMap settingGroups;
    return settingGroups;
}


} // namespace settings