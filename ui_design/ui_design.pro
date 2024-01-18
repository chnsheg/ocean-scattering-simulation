QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport serialport

CONFIG += c++15.9

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    base/singleton.cpp \
    controller/plotcontroller.cpp \
    main.cpp \
    mainwindow.cpp \
    manager/buttongroupmanager.cpp \
    manager/customplotmanager.cpp \
    manager/inputdatalistmanager.cpp \
    manager/managerbase.cpp \
    manager/show1buttongroupmanager.cpp \
    manager/tracermanager.cpp \
    model/fizeaudatagenerator.cpp \
    model/frequencedatagenerator.cpp \
    model/laserdatagenerator.cpp \
    model/pagedatagenerator.cpp \
    model/spectrumdatagenerator.cpp \
    module/mycombobox.cpp \
    module/mytracer.cpp \
    module/qcustomplot.cpp \
    utils/readfiledata.cpp \
    view/plotView.cpp

HEADERS += \
    base/singleton.h \
    controller/plotcontroller.h \
    mainwindow.h \
    manager/buttongroupmanager.h \
    manager/customplotmanager.h \
    manager/inputdatalistmanager.h \
    manager/managerbase.h \
    manager/show1buttongroupmanager.h \
    manager/tracermanager.h \
    model/fizeaudatagenerator.h \
    model/frequencedatagenerator.h \
    model/laserdatagenerator.h \
    model/pagedatagenerator.h \
    model/spectrumdatagenerator.h \
    module/mycombobox.h \
    module/mytracer.h \
    module/qcustomplot.h \
    utils/readfiledata.h \
    view/plotView.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    data_csv.qrc \
    images.qrc

DISTFILES += \
    base/singleton.cpp.txt \
    base/singleton.h.txt
