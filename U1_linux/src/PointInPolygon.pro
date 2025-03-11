QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

DEFINES += QT_DEPRECATED_WARNINGS

SOURCES += \
    ../shapelib/dbfopen.cpp \
    ../shapelib/shpopen.cpp \
    ../shapelib/shptree.cpp \
    algorithms.cpp \
    draw.cpp \
    main.cpp \
    mainform.cpp

HEADERS += \
    ../shapelib/shapefil.h \
    algorithms.h \
    draw.h \
    mainform.h

FORMS += \
    mainform.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    icons.qrc
