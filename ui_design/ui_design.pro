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
    manager/lineeditgroupmanager.cpp \
    manager/managerbase.cpp \
    manager/menumanager.cpp \
    manager/show1buttongroupmanager.cpp \
    manager/texteditmanager.cpp \
    manager/tracermanager.cpp \
    model/CaculateScatteredPhotons.cpp \
    model/FizeauInstrument.cpp \
    model/PMTReceive.cpp \
    model/SpectrumGeneration.cpp \
    model/colon.cpp \
    model/constantmap.cpp \
    model/constantstorage.cpp \
    model/convolution.cpp \
    model/fizeaudatagenerator.cpp \
    model/fizeauifgenerator.cpp \
    model/frequencedatagenerator.cpp \
    model/laserdatagenerator.cpp \
    model/pagedatagenerator.cpp \
    model/pmtreceptiondatagenerator.cpp \
    model/rtGetInf.cpp \
    model/rtGetNaN.cpp \
    model/rt_nonfinite.cpp \
    model/spectrumdatagenerator.cpp \
    model/underwaterspectrumdatagenerator.cpp \
    module/filebrowser.cpp \
    module/mycombobox.cpp \
    module/mytracer.cpp \
    module/qcustomplot.cpp \
    utils/logger.cpp \
    utils/mymath.cpp \
    utils/readfiledata.cpp \
    view/plotView.cpp

HEADERS += \
    base/singleton.h \
    controller/plotcontroller.h \
    fftw3.h \
    mainwindow.h \
    manager/buttongroupmanager.h \
    manager/customplotmanager.h \
    manager/inputdatalistmanager.h \
    manager/lineeditgroupmanager.h \
    manager/managerbase.h \
    manager/menumanager.h \
    manager/show1buttongroupmanager.h \
    manager/texteditmanager.h \
    manager/tracermanager.h \
    model/CaculateScatteredPhotons.h \
    model/FizeauInstrument.h \
    model/PMTReceive.h \
    model/SpectrumGeneration.h \
    model/coder_array.h \
    model/colon.h \
    model/constantmap.h \
    model/constantstorage.h \
    model/convolution.h \
    model/fizeaudatagenerator.h \
    model/fizeauifgenerator.h \
    model/frequencedatagenerator.h \
    model/laserdatagenerator.h \
    model/pagedatagenerator.h \
    model/pmtreceptiondatagenerator.h \
    model/rtGetInf.h \
    model/rtGetNaN.h \
    model/rt_nonfinite.h \
    model/rtwtypes.h \
    model/spectrumdatagenerator.h \
    model/tmwtypes.h \
    model/underwaterspectrumdatagenerator.h \
    module/filebrowser.h \
    module/mycombobox.h \
    module/mytracer.h \
    module/qcustomplot.h \
    utils/logger.h \
    utils/mymath.h \
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
    images.qrc \
    qss.qrc

DISTFILES += \
    base/singleton.cpp.txt \
    base/singleton.h.txt

LIBS += -L$$PWD/lib/ -llibfftw3-3

msvc:QMAKE_CXXFLAGS += -execution-charset:utf-8
msvc:QMAKE_CXXFLAGS += -source-charset:utf-8
QMAKE_CXXFLAGS_WARN_ON += -wd4819
