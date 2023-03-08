TARGET = TRAPTACloud
TEMPLATE = app

QT += core network qml quick
CONFIG += c++11
CONFIG += release

debug {
    DESTDIR = bin/debug
    MOC_DIR = build/lib/debug
    OBJECTS_DIR = build/lib/debug
}
release {
    DESTDIR = bin/release
    MOC_DIR = build/lib/release
    OBJECTS_DIR = build/lib/release
}

unix:!macx {
    QMAKE_LFLAGS += -no-pie
}

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    src/main.cpp \
    src/viewcontroller.cpp \
    src/udplistener.cpp \
    src/cloud.cpp \
    src/trapta.cpp

RESOURCES += resources.qrc \

RC_FILE = traptacloud.rc
ICON = traptacloud.icns

HEADERS += \
    src/viewcontroller.h \
    src/udplistener.h \
    src/cloud.h \
    src/trapta.h
