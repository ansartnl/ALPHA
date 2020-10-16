INCLUDEPATH += $$PWD/src
INCLUDEPATH += $$PWD/src/dialogs

DEPENDPATH += $$PWD/src
DEPENDPATH += $$PWD/src/dialogs

HEADERS += \
    $$PWD/src/UserClient.h \
    $$PWD/src/dialogs/LoginDialog.h \
    $$PWD/src/dialogs/ManageUsersDialog.h \
    $$PWD/src/dialogs/ChangePasswordDialog.h \
    $$PWD/src/dialogs/AddUserDialog.h \
    $$PWD/src/DialogManager.h

SOURCES += \
    $$PWD/src/UserClient.cpp \
    $$PWD/src/dialogs/LoginDialog.cpp \
    $$PWD/src/dialogs/ManageUsersDialog.cpp \
    $$PWD/src/dialogs/ChangePasswordDialog.cpp \
    $$PWD/src/dialogs/AddUserDialog.cpp \
    $$PWD/src/DialogManager.cpp

FORMS += \
    $$PWD/src/dialogs/LoginDialog.ui \
    $$PWD/src/dialogs/ManageUsersDialog.ui \
    $$PWD/src/dialogs/ChangePasswordDialog.ui \
    $$PWD/src/dialogs/AddUserDialog.ui
