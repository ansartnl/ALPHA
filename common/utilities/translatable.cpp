#include "translatable.h"

#include <algorithm>

#include <QApplication>
#include <QTranslator>
#include <QDir>
#include <QStringList>

#include "translation.h"


namespace utilities {


QString Translatable::getTranslationsFolder() {
#ifndef Q_OS_DARWIN
    return QDir(QApplication::applicationDirPath()).absoluteFilePath("Translations");
#else
    QStringList binPathList = QApplication::applicationDirPath().split(QDir::separator());
    binPathList.removeLast();
    binPathList <<  "Resources" << "Translations";
    return  binPathList.join(QDir::separator());
    //return binPathList.join(QDir::separator()) + QDir::separator() + "Resources" + QDir::separator() + "Translations";
#endif
}

void Translatable::retranslateApp(const QString& locale) {
    if (translator_) {
        qApp->removeTranslator(translator_);
        translator_->deleteLater();
    }
    translator_ = new QTranslator(qApp);

    QString filename = qApp->applicationName() +  "_" + locale;
    if (translator_->load(filename, getTranslationsFolder()))
        qApp->installTranslator(translator_);

    Tr::RetranslateAll(qApp);
}

QStringList Translatable::getFilenames() {
    QDir dir(getTranslationsFolder());
    QStringList file_names = dir.entryList(QStringList(QApplication::applicationName() + "_*.qm"));

    std::for_each(file_names.begin(), file_names.end(), [&dir](QString & str) {str = dir.absoluteFilePath(str);});

    return file_names;
}


} // namespace utilities