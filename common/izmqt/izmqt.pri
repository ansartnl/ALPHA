INCLUDEPATH     += $$PWD/src $$PWD/src/zmq
DEPENDPATH      += $$PWD/src $$PWD/src/zmq

HEADERS         += $$PWD/src/nzmqt/nzmqt.hpp \
                   $$PWD/src/properties/StartedProperty.h \
                   $$PWD/src/properties/ModeProperty.h \
                   $$PWD/src/monitor/Monitor.h \
                   $$PWD/src/pubsub/PublishQueue.h \
                   $$PWD/src/pubsub/SubscribeQueue.h \
                   $$PWD/src/reqrep/ReplyQueue.h \
                   $$PWD/src/reqrep/RequestQueue.h \
                   $$PWD/src/asyncreqrep/RouterServer.h \
                   $$PWD/src/asyncreqrep/DealerClient.h \
                   $$PWD/src/asyncreqrep/MessageQueueBroker.h \
                   $$PWD/src/pushpull/PullQueue.h \
                   $$PWD/src/pushpull/PushQueue.h \
                   $$PWD/src/pubsub/XPubSubProxy.h \
                   $$PWD/src/zmq/zmq.h \
                   $$PWD/src/zmq/zmq.hpp \
                   $$PWD/src/zmq/zmq_utils.h \
                   $$PWD/src/izmq_global.h

SOURCES         += \
                   $$PWD/src/monitor/Monitor.cpp \
                   $$PWD/src/pubsub/PublishQueue.cpp \
                   $$PWD/src/pubsub/SubscribeQueue.cpp \
                   $$PWD/src/reqrep/ReplyQueue.cpp \
                   $$PWD/src/reqrep/RequestQueue.cpp \
                   $$PWD/src/asyncreqrep/RouterServer.cpp \
                   $$PWD/src/asyncreqrep/DealerClient.cpp \
                   $$PWD/src/asyncreqrep/MessageQueueBroker.cpp \
                   $$PWD/src/pushpull/PullQueue.cpp \
                   $$PWD/src/pushpull/PushQueue.cpp \
                   $$PWD/src/pubsub/XPubSubProxy.cpp

DEFINES += IZMQT_STATIC

linux-g++-32:LIBS += -L$$PWD/lib/oracle-linux-6.3-i686
linux-g++-64:LIBS += -L$$PWD/lib/linux-x64
greaterThan(QT_MAJOR_VERSION, 4) {
    win32-g++:LIBS += -L$$PWD/lib/win32-mingw-qt5
} else {
    win32-g++:LIBS += -L$$PWD/lib/win32-mingw
}
win32-msvc2005:LIBS += -L$$PWD/lib/win32-msvc

LIBS += -lzmq
