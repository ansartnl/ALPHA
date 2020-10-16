#ifndef _XT_PROTO_PARENT_H__
#define _XT_PROTO_PARENT_H__

#include "XtObject.h"

#include <QtNetwork/QHostAddress>


namespace xtProto
{
    //! Interface for internal data listeners
    class IListenerParent
    {
        public:
            virtual ~IListenerParent() {}

        //! Start service
        virtual bool Start(const QHostAddress & address = QHostAddress::Any, quint16 port = 0) = 0;
        //! Stop service
        virtual void Stop() = 0;
    };

    //! Interface to data handler
    class IParent
    {
    public:
        //! Event of some object coming
        /*!
            \param obj Internal format of data object
        */
        virtual void OnObjectData(const Xt::TObject& obj) = 0;
    };
}

#endif // _XT_PROTO_PARENT_H__
