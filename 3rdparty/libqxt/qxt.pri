QXT_MODULES = docs core gui network sql web berkeley zeroconf
QXT_INSTALL_PREFIX = $$PWD
win32 {
    QXT_INSTALL_LIBS = $$PWD/lib/win32
    QXT_INSTALL_BINS = $$PWD/lib/win32
} else {
    QXT_INSTALL_LIBS = $$PWD/lib
    QXT_INSTALL_BINS = $$PWD/lib
}
QXT_INSTALL_HEADERS = $$PWD/include
QXT_INSTALL_SOURCES = $$PWD/src

DEPENDPATH      += $${QXT_INSTALL_HEADERS}
INCLUDEPATH     += $${QXT_INSTALL_HEADERS}

QMAKE_RPATHDIR  += $${QXT_INSTALL_LIBS}
macx:LIBS       += -F$${QXT_INSTALL_LIBS}
LIBS            += -L$${QXT_INSTALL_LIBS}

contains(QXT, berkeley) {
    contains(QXT_MODULES, berkeley):DEFINES += QXT_BERKELEY_LIB
    else:warning(Qxt was built without QxtBerkeley)

    DEPENDPATH       += $${QXT_INSTALL_HEADERS}/QxtBerkeley \
                        $${QXT_INSTALL_SOURCES}/berkeley
    INCLUDEPATH      += $${QXT_INSTALL_HEADERS}/QxtBerkeley \
                        $${QXT_INSTALL_SOURCES}/berkeley
    macx:INCLUDEPATH += $${QXT_INSTALL_LIBS}/QxtBerkeley.framework/HEADERS/

    qtAddLibrary(QxtBerkeley)
    QXT += core
}

contains(QXT, web) {
    contains(QXT_MODULES, web):DEFINES += QXT_WEB_LIB
    else:warning(Qxt was built without QxtWeb)

    DEPENDPATH       += $${QXT_INSTALL_HEADERS}/QxtWeb \
                        $${QXT_INSTALL_SOURCES}/web
    INCLUDEPATH      += $${QXT_INSTALL_HEADERS}/QxtWeb \
                        $${QXT_INSTALL_SOURCES}/web
    macx:INCLUDEPATH += $${QXT_INSTALL_LIBS}/QxtWeb.framework/HEADERS/

    qtAddLibrary(QxtWeb)
    QXT += core network
    QT  += network
}

contains(QXT, gui) {
    contains(QXT_MODULES, gui):DEFINES += QXT_GUI_LIB
    else:warning(Qxt was built without QxtGui)

    DEPENDPATH       += $${QXT_INSTALL_HEADERS}/QxtGui \
                        $${QXT_INSTALL_SOURCES}/gui
    INCLUDEPATH      += $${QXT_INSTALL_HEADERS}/QxtGui \
                        $${QXT_INSTALL_SOURCES}/gui
    macx:INCLUDEPATH += $${QXT_INSTALL_LIBS}/QxtGui.framework/HEADERS/

    qtAddLibrary(QxtGui)
    QXT += core
    QT  += gui
}

contains(QXT, zeroconf) {
    contains(QXT_MODULES, zeroconf):DEFINES += QXT_ZEROCONF_LIB
    else:warning(Qxt was built without QxtZeroConf)

    DEPENDPATH       += $${QXT_INSTALL_HEADERS}/QxtZeroconf \
                        $${QXT_INSTALL_SOURCES}/zeroconf
    INCLUDEPATH      += $${QXT_INSTALL_HEADERS}/QxtZeroconf \
                        $${QXT_INSTALL_SOURCES}/zeroconf
    macx:INCLUDEPATH += $${QXT_INSTALL_LIBS}/QxtZeroconf.framework/HEADERS/

    qtAddLibrary(QxtZeroconf)
    QXT += core network
    QT  += network
}

contains(QXT, network) {
    contains(QXT_MODULES, network):DEFINES += QXT_NETWORK_LIB
    else:warning(Qxt was built without QxtNetwork)

    DEPENDPATH       += $${QXT_INSTALL_HEADERS}/QxtNetwork \
                        $${QXT_INSTALL_SOURCES}/network
    INCLUDEPATH      += $${QXT_INSTALL_HEADERS}/QxtNetwork \
                        $${QXT_INSTALL_SOURCES}/network
    macx:INCLUDEPATH += $${QXT_INSTALL_LIBS}/QxtNetwork.framework/HEADERS/

    qtAddLibrary(QxtNetwork)
    QXT += core
    QT  += network
}

contains(QXT, sql) {
    contains(QXT_MODULES, sql):DEFINES += QXT_SQL_LIB
    else:warning(Qxt was built without QxtSql)

    DEPENDPATH       += $${QXT_INSTALL_HEADERS}/QxtSql \
                        $${QXT_INSTALL_SOURCES}/sql
    INCLUDEPATH      += $${QXT_INSTALL_HEADERS}/QxtSql
                        $${QXT_INSTALL_SOURCES}/sql
    macx:INCLUDEPATH += $${QXT_INSTALL_LIBS}/QxtSql.framework/HEADERS/

    qtAddLibrary(QxtSql)
    QXT += core
    QT  += sql
}

contains(QXT, core) {
    contains(QXT_MODULES, core):DEFINES += QXT_CORE_LIB
    else:warning(Qxt was built without QxtCore)

    DEPENDPATH       += $${QXT_INSTALL_HEADERS}/QxtCore \
                        $${QXT_INSTALL_SOURCES}/core
    INCLUDEPATH      += $${QXT_INSTALL_HEADERS}/QxtCore \
                        $${QXT_INSTALL_SOURCES}/core
    macx:INCLUDEPATH += $${QXT_INSTALL_LIBS}/QxtCore.framework/HEADERS/

    qtAddLibrary(QxtCore)
}
