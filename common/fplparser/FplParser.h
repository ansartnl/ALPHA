#ifndef FPLPARSER_H
#define FPLPARSER_H

#include <QtCore/QString>
#include <QtCore/QVariantMap>

namespace fplparser {

    QVariantMap parse(const QString& strFpl);
    QString compose(const QVariantMap& binFpl);

    enum FplStandard {
        PRESENT,
        NEW,
        INVALID
    };

    FplStandard whatFplStadnard(const QVariantMap& binFpl);

    QVariantMap convertFromNew2012ToPresent(const QVariantMap& newFpl);
}

#endif //#ifdef FPLPARSER_H
