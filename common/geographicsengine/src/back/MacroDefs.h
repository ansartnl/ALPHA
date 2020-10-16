#ifndef _MACRO_DEFS_H__
#define _MACRO_DEFS_H__

//! Determines standard property
#define GEO_PROPERTY(T, V) \
        public: \
			const T& Get##V() const { return m_##V; } \
			void Set##V(const T& V) { m_##V = V; } \
        private: \
            T m_##V;

//! Determines extended property
#define GEO_PROPERTY_EX(T, V) \
        public: \
			const T& Get##V() const { return m_##V; } \
			void Set##V(const T& V) { m_##V = V; Mark##V(true); } \
			bool Is##V() const { return m_b##V; } \
			void Mark##V(bool b##V) { m_b##V = b##V; } \
        private: \
            T m_##V; \
            bool m_b##V;


#endif // _MACRO_DEFS_H__
