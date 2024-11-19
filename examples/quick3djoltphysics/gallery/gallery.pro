QT += quick quick3d quick3djoltphysics

include(../shared/shared.pri)

SOURCES += \
    main.cpp \
    mousehandler.cpp \

HEADERS += \
    mousehandler.h \

RESOURCES += \
    qml.qrc

CONFIG += qmltypes
QML_IMPORT_NAME = Example
QML_IMPORT_MAJOR_VERSION = 1
