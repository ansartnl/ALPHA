/****************************************************************************
**
** Copyright (C) 2011 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (qt-info@nokia.com)
**
** This file is part of the examples of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** You may use this file under the terms of the BSD license as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of Nokia Corporation and its Subsidiary(-ies) nor
**     the names of its contributors may be used to endorse or promote
**     products derived from this software without specific prior written
**     permission.
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
** $QT_END_LICENSE$
**
****************************************************************************/

#include <QApplication>
#include <QtCore>
#include <stdlib.h>
#include <QMessageBox>
#include <QMutex>

#include "main.h"
#include "server.h"
#include "InitMain.h"

QSettings& xmlSettings(const QString &pathConfig)
{
    static QtXmlSettings xmlStg(pathConfig);
    return xmlStg;
}


int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    app.setOrganizationName( "ITOS" );
    app.setApplicationName("RunwayManager");
    InitMain::setupSearchPath();


    QString pathConfigFile = InitMain::findConfigFilePath();

    // Make sure configuration exists.
    if ( pathConfigFile.isEmpty() )
    {
        QMessageBox::warning(0, QObject::tr("Configuration"),
                             QObject::tr("Configuration file is not found.\n\n") +
                             InitMain::configUsageString());
        return 0;
    }

    // Initialize xml settings technique.
    xmlSettings(pathConfigFile);


    QThread thread;
    Server server;

    server.moveToThread(&thread);
    QObject::connect(&thread, SIGNAL(started()), &server, SLOT(startServer()));
    QObject::connect(&thread, SIGNAL(finished()), &server, SLOT(deleteLater()));
    thread.start();

    thread.wait();

    return server.returnCode();
}
