#include "memory_storage.h"


namespace settings {


QVariant MemoryStorage::Get(const QString& key) const {
    auto it = storage_.find(key);
    return (it != storage_.end() ? it->second : QVariant());
}


} // namespace settings
