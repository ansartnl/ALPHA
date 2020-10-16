#ifndef START_H
#define START_H

#include <QtCore/QSettings>
#include "utils/ExtendedXmlSettings.h"
#include "utils/OperationsLog.h"

ExtendedXmlSettings* networkSettings(const QString & pathFile = QString());
QSettings* globalSettings(const QString & pathFile = QString());

#include "Dao.h"
Dao* dao();

class CMaster;
CMaster* master(CMaster* pMaster = 0);

#include "logmanager.h"
#include "level.h"

#ifdef _MSC_VER
#define UTF8_QSTRING(str) QString::fromWCharArray(L##str)
#else
#define UTF8_QSTRING(str) QString::fromUtf8(str)
#endif

#define WARNING Log4Qt::Level::WARN_INT
#define ERROR Log4Qt::Level::ERROR_INT
#define INFO Log4Qt::Level::INFO_INT
#define DEBUG Log4Qt::Level::DEBUG_INT

#define LOG_STR(severity, message) \
{ Log4Qt::LogManager::rootLogger()->log(severity, QString("File: %1, line: %2, Error: %3").arg(__FILE__).arg(__LINE__).arg(message) ); }

#define LOG_TVAERROR(severity, TVAError) \
{ Log4Qt::LogManager::rootLogger()->log(severity, TVAError.reason()); }

#define LOG_TVAERROR_STR(severity, TVAError, description) \
{ TVAError.AddReason(description); LOG_TVAERROR(severity, FDEError); }

#define LOG_OPER_STR(code, type, value) \
{ Log4Qt::LogManager::rootLogger()->log(INFO, QString("<AC> CODE= %1 set %2 in %3").arg(code).arg(type).arg(value)); }

#define LOG_OPER_SIMPLE_STR(message) \
{ Log4Qt::LogManager::rootLogger()->log(INFO, QString(message)); }

class QWidget;
//! Retrieve reference to main window.
QWidget* getMainWindow();

/*!
  Class from "aftnmessageprocessor" static library for aftn message processing (validating, parsing, and database inserting)
  */
class AFTNMessageProcessor;
extern AFTNMessageProcessor* aftnMessageProcessor;

/**
* @brief Function check license file(s)

*/
bool CheckLicense(const QApplication& app);

#define GROUND_CWP

#endif //#ifndef START_H
