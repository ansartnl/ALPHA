#ifndef ASTERIXDB_H
#define ASTERIXDB_H

#include "common/AirplaneState.h"

class QTimer;
namespace asterix {
    //! Read data information from DB
    class CDBReader: public QObject, public IAsterixDataSrc
    {
        Q_OBJECT
        public:
            //! Constructor
            /*!
             \param pDB pointer to database
             */
            CDBReader(IParent* pParent);
            ~CDBReader();
            virtual void Start();
            virtual void Stop();

        private:
            Q_SLOT void checkProcess();
            QTimer *checkToTimer;
            IParent* m_pParent;
    };

}

#endif // ASTERIXDB_H
