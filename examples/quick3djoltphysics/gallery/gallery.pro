QT += quick quick3d quick3djoltphysics

SOURCES += \
    main.cpp \
    mousehandler.cpp \
    examplecharactercontactlistener.cpp \
    examplecontactlistener.cpp \
    examplegeometry.cpp \
    examplelayers.cpp \
    examplesensorcontactlistener.cpp \

HEADERS += \
    mousehandler.h \
    examplecharactercontactlistener.h \
    examplecontactlistener.h \
    examplegeometry.h \
    examplelayers.h \
    examplesensorcontactlistener.h \

RESOURCES += \
    qml.qrc

CONFIG += qmltypes
QML_IMPORT_NAME = Example
QML_IMPORT_MAJOR_VERSION = 1
