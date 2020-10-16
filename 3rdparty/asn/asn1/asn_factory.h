#pragma once

#include <functional>
#include <string>
#include <utility>
#include <unordered_map>
#include <map>

#include "singleton.h"


namespace ASN1 {


const int kInvalidChoiceId = -1;


class AbstractData;


class ASNFactory {
public:
    typedef const void* InfoType;
    typedef std::function<AbstractData*(InfoType)> CreateFunction;
    typedef int ChoiceId;
    typedef std::pair<CreateFunction, InfoType> TypeInstruments;
    typedef std::string TypeId;
    typedef std::string ChoiceWrapperAndTypeId;
    typedef std::string ChoiceWrapperAndChoiceId;
    typedef std::string ChoiceKey;

    inline AbstractData* create(const TypeId& type_id) {
        auto it = creators_.find(type_id);
        return (it != creators_.end() ? it->second.first(it->second.second) : nullptr);
    }
    inline ChoiceId choiceId(const TypeId& type_id, const TypeId& wrapper_id) {
        auto it = choice_ids_.find(type_id + '|' + wrapper_id);
        return (it != choice_ids_.end() ? it->second : kInvalidChoiceId);
    }
    inline const TypeId& baseTypeId(const TypeId& type_id) {
        static TypeId empty_id;
        auto it = base_type_ids_.find(type_id);
        return (it != base_type_ids_.end() ? it->second : empty_id);
    }
    inline const ChoiceKey& choiceKey(ChoiceId choice_id, const TypeId& wrapper_id) {
        static ChoiceKey empty_key;
        auto it = choice_keys_.find(std::to_string(choice_id) + '|' + wrapper_id);
        return (it != choice_keys_.end() ? it->second : empty_key);
    }
    inline void registerType(const TypeId& type_id, CreateFunction create_function, InfoType info_type) {
        creators_.insert(std::make_pair(type_id, std::make_pair(create_function, info_type)));
    }
    inline void registerBaseTypeId(const TypeId& type_id, const TypeId& base_type_id) {
        base_type_ids_.insert(std::make_pair(type_id, base_type_id));
    }
    inline void registerChoiceId(const TypeId& type_id, const TypeId& wrapper_id, ChoiceId choice_id) {
        choice_ids_.insert(std::make_pair(type_id + '|' + wrapper_id, choice_id));
    }
    inline void registerChoiceKey(ChoiceId choice_id, const TypeId& wrapper_id, const ChoiceKey& choice_key) {
        choice_keys_.insert(std::make_pair(std::to_string(choice_id) + '|' + wrapper_id, choice_key));
    }

private:
    std::unordered_map<TypeId, TypeInstruments> creators_;
    std::unordered_map<ChoiceWrapperAndTypeId, ChoiceId> choice_ids_;
    std::unordered_map<TypeId, TypeId> base_type_ids_;
    std::unordered_map<ChoiceWrapperAndChoiceId, ChoiceKey> choice_keys_;
};


class TheASNFactory : public utilities::Singleton<ASNFactory> {};


} // namespace ASN1