greaterThan(QT_MAJOR_VERSION, 4)
greaterThan(QT_MINOR_VERSION, 9)

QT += core gui
QT += widgets
QT += serialport

TARGET = 008_qcom
TEMPLATE = app

DEFINES += QT_DEPRECATED_WARNINGS

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    formatselector.cpp \
    baudselector.cpp \
    databitsselector.cpp \
    parityselector.cpp \
    stopbitselector.cpp

HEADERS += \
    mainwindow.h \
    formatselector.h \
    baudselector.h \
    databitsselector.h \
    parityselector.h \
    stopbitselector.h

# settings for make install
target.path = $${_PRO_FILE_PWD_}/../install/bin
INSTALLS += target
