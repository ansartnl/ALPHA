#pragma once

#include <QString>

#include "utilities_global.h"


class QTranslator;


namespace utilities {


class UTILITIES_EXPORT Translatable {
public:
    Translatable() : translator_(nullptr) {}

    static QStringList getFilenames();

    void retranslateApp(const QString& locale);

private:
    static QString getTranslationsFolder();

    QTranslator* translator_;
};


} // namespace utilities
