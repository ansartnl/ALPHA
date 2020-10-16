#pragma once

#include "utilities_global.h"


namespace utilities {


template <class Ty>
class Singleton : public Ty {
public:
    static Ty& instance() {
        static Ty single_instance;
        return single_instance;
    }

private:
    Q_DISABLE_COPY(Singleton)
    Singleton();
};


template <class Ty>
class DestroyableSingleton : public Ty {
public:
    static Ty* instance() {
        if (!single_instance_) {
            single_instance_ = (Ty*)malloc(sizeof(Ty));
            memset((void*)single_instance_, 0, sizeof(Ty));
            if (single_instance_)
                single_instance_ = new(single_instance_) Ty;
        }
        return single_instance_;
    }
    static void destroyInstance() {
        if (!single_instance_) return;
        single_instance_->~Ty();
        free(single_instance_);
        single_instance_ = nullptr;
    }

private:
    Q_DISABLE_COPY(DestroyableSingleton)
    DestroyableSingleton();

    static Ty* single_instance_;
};


template <class Ty>
Ty* DestroyableSingleton<Ty>::single_instance_ = nullptr;


} // namespace utilities
