TEMPLATE = app

QT += qml quick 3dinput 3drender widgets
CONFIG += c++11

SOURCES += main.cpp \
    model/mesh.cpp \
    model/model.cpp \
    model/polygon.cpp \
    model/vertex.cpp \
    model/reserved.cpp

RESOURCES += qml.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    model/mesh.h \
    model/model.h \
    model/polygon.h \
    model/vertex.h
