#ifndef TVAMACRO_H
#define TVAMACRO_H

#define TVA_PROPERTIE(T, V) public: \
            const T& Get##V() const { return m_##V; } \
            void Set##V(const T& V) { m_##V = V; } \
        private: T m_##V;

#define TVA_PROPERTIE_EX(T, V) public: \
            const T& Get##V() const { return m_##V; } \
            void Set##V(const T& V) { m_##V = V; Mark##V(true); } \
            bool Is##V() const { return m_b##V; } \
            void Mark##V(bool b##V) { m_b##V = b##V; } \
        private: T m_##V; bool m_b##V;


#endif // TVAMACRO_H
