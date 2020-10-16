#ifndef XT_DEFINES_H
#define XT_DEFINES_H

#define XT_BEGIN_NAMESPACE  namespace Xt {
#define XT_END_NAMESPACE    }

XT_BEGIN_NAMESPACE

#define PROPERTY_DECL(T, V) public: \
            const T& Get##V() const { return m_##V; } \
            void Set##V(const T& V) { m_##V = V; } \
        private: T m_##V

XT_END_NAMESPACE

#endif // XT_DEFINES_H
