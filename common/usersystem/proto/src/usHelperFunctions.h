#ifndef HELPERFUNCTIONS_H
#define HELPERFUNCTIONS_H

#include "usDeclare.h"

#include <QtCore/QString>

namespace UserSystem
{

static QString m_salt = QString("SOMESALTFROMFILONENKO\xAB\xCD\xEF");
void setSalt(const QString &s);
QString salt();

QString passwordHash(const QString &password, const QString &s = m_salt);

QString toString(const Request::Action &a);
QString toString(const Request::Result &r);

} // namespace

#endif // HELPERFUNCTIONS_H
