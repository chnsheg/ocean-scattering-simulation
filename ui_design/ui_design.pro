QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport serialport

CONFIG += c++15.9

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    buttongroup.cpp \
    customplotmanager.cpp \
    frequencedatagenerator.cpp \
    laserdatagenerator.cpp \
    main.cpp \
    mainwindow.cpp \
    mycombobox.cpp \
    mytracer.cpp \
    pagedatagenerator.cpp \
    pageplot.cpp \
    qcustomplot.cpp \
    readfiledata.cpp \
    spectrumdatagenerator.cpp \
    viewcontroller.cpp

HEADERS += \
    buttongroup.h \
    customplotmanager.h \
    frequencedatagenerator.h \
    laserdatagenerator.h \
    mainwindow.h \
    mycombobox.h \
    mytracer.h \
    pagedatagenerator.h \
    pageplot.h \
    qcustomplot.h \
    readfiledata.h \
    spectrumdatagenerator.h \
    viewcontroller.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    data_csv.qrc \
    images.qrc
