#ifndef XT_OBJECT_H
#define XT_OBJECT_H

#include "xtproto_global.h"
#include "XtDefines.h"
#include <QSharedPointer>
#include <QDataStream>

XT_BEGIN_NAMESPACE

//! Base class to store data
struct XTPROTO_EXPORT CObject
{
    //! Constructs object structure.
    CObject();
    virtual ~CObject()
    {}

    virtual int GetType() const
    { return 0; }

    virtual void Serialize(QDataStream& out) const;
    virtual void Deserialize(QDataStream& in)
    { in >> mVersion; }

    // Retrieve version of the object.
    int GetVersion() const
    { return mVersion; }

    //! Retrieve compiled version of the class.
    static int GetClassVersion();

public:
    /** Object version */
    quint32     mVersion;
};

typedef QSharedPointer<CObject>     TObject;

#define XT_OBJECT_TYPE_DECL \
    public: \
        virtual int GetType() const; \
        static int GetClassType();

#define XT_OBJECT_TYPE_IMPL(BaseClass, Value) \
    int BaseClass::GetType() const { return int(Value); } \
    int BaseClass::GetClassType() { return int(Value); }

XT_END_NAMESPACE

QDataStream &operator<<(QDataStream &out, const Xt::CObject &object);

#endif // XT_OBJECT_H
