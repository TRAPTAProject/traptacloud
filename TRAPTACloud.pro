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
