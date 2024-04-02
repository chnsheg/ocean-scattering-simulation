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
    matlab/AddNoiseNondB.cpp \
    matlab/AddNoiseNondB_data.cpp \
    matlab/AddNoiseNondB_initialize.cpp \
    matlab/CaculateScatteredPhotons.cpp \
    matlab/FizeauInstrument.cpp \
    matlab/PMTReceive.cpp \
    matlab/Retrieval/GenerationSpectrum.cpp \
    matlab/Retrieval/PMTFMethods.cpp \
    matlab/Retrieval/RetrievalAlgorithm.cpp \
    matlab/Retrieval/RetrievalAlgorithm_data.cpp \
    matlab/Retrieval/RetrievalAlgorithm_initialize.cpp \
    matlab/Retrieval/Retrieval_TS.cpp \
    matlab/Retrieval/checkStoppingCriteria.cpp \
    matlab/Retrieval/computeFiniteDifferences.cpp \
    matlab/Retrieval/conv.cpp \
    matlab/Retrieval/driver.cpp \
    matlab/Retrieval/eml_rand_mt19937ar_1.cpp \
    matlab/Retrieval/eml_rand_mt19937ar_stateful_1.cpp \
    matlab/Retrieval/error_fitted_power.cpp \
    matlab/Retrieval/factoryConstruct.cpp \
    matlab/Retrieval/hasFiniteBounds.cpp \
    matlab/Retrieval/linearLeastSquares.cpp \
    matlab/Retrieval/minOrMax.cpp \
    matlab/Retrieval/poissrnd.cpp \
    matlab/Retrieval/projectBox.cpp \
    matlab/Retrieval/xgemv.cpp \
    matlab/Retrieval/xnrm2.cpp \
    matlab/Retrieval/xzlarf.cpp \
    matlab/Retrieval/xzlarfg.cpp \
    matlab/SpectrumGeneration.cpp \
    matlab/colon.cpp \
    matlab/convolution.cpp \
    matlab/div.cpp \
    matlab/drawNsMByDepth.cpp \
    matlab/drawSNRDepthByAlpha.cpp \
    matlab/drawSNRDepthByM.cpp \
    matlab/eml_mtimes_helper.cpp \
    matlab/eml_rand_mt19937ar.cpp \
    matlab/eml_rand_mt19937ar_stateful.cpp \
    matlab/rand.cpp \
    matlab/randn.cpp \
    matlab/rtGetInf.cpp \
    matlab/rtGetNaN.cpp \
    matlab/rt_nonfinite.cpp \
    matlab/sum.cpp \
    # model/CaculateScatteredPhotons.cpp \
    # model/FizeauInstrument.cpp \
    # model/PMTReceive.cpp \
    # model/SpectrumGeneration.cpp \
    # model/colon.cpp \
    model/constantmap.cpp \
    model/constantstorage.cpp \
    # model/convolution.cpp \
    model/fizeaudatagenerator.cpp \
    model/fizeauifgenerator.cpp \
    model/frequencedatagenerator.cpp \
    model/laserdatagenerator.cpp \
    model/pagedatagenerator.cpp \
    model/pmtreceptiondatagenerator.cpp \
    # model/rtGetInf.cpp \
    # model/rtGetNaN.cpp \
    # model/rt_nonfinite.cpp \
    model/spectrumdatagenerator.cpp \
    model/underwaterspectrumdatagenerator.cpp \
    module/filebrowser.cpp \
    module/mycombobox.cpp \
    module/mytracer.cpp \
    module/qcustomplot.cpp \
    utils/logger.cpp \
    utils/mymath.cpp \
    utils/readfiledata.cpp \
    view/dynamicview.cpp \
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
    matlab/AddNoiseNondB.h \
    matlab/AddNoiseNondB_data.h \
    matlab/AddNoiseNondB_initialize.h \
    matlab/CaculateScatteredPhotons.h \
    matlab/FizeauInstrument.h \
    matlab/PMTReceive.h \
    matlab/Retrieval/GenerationSpectrum.h \
    matlab/Retrieval/PMTFMethods.h \
    matlab/Retrieval/RetrievalAlgorithm.h \
    matlab/Retrieval/RetrievalAlgorithm_data.h \
    matlab/Retrieval/RetrievalAlgorithm_initialize.h \
    matlab/Retrieval/RetrievalAlgorithm_internal_types.h \
    matlab/Retrieval/RetrievalAlgorithm_internal_types1.h \
    matlab/Retrieval/RetrievalAlgorithm_internal_types11.h \
    matlab/Retrieval/RetrievalAlgorithm_types.h \
    matlab/Retrieval/Retrieval_TS.h \
    matlab/Retrieval/anonymous_function.h \
    matlab/Retrieval/anonymous_function1.h \
    matlab/Retrieval/checkStoppingCriteria.h \
    matlab/Retrieval/computeFiniteDifferences.h \
    matlab/Retrieval/conv.h \
    matlab/Retrieval/driver.h \
    matlab/Retrieval/eml_rand_mt19937ar_1.h \
    matlab/Retrieval/eml_rand_mt19937ar_stateful_1.h \
    matlab/Retrieval/error_fitted_power.h \
    matlab/Retrieval/factoryConstruct.h \
    matlab/Retrieval/hasFiniteBounds.h \
    matlab/Retrieval/linearLeastSquares.h \
    matlab/Retrieval/minOrMax.h \
    matlab/Retrieval/poissrnd.h \
    matlab/Retrieval/projectBox.h \
    matlab/Retrieval/xgemv.h \
    matlab/Retrieval/xnrm2.h \
    matlab/Retrieval/xzlarf.h \
    matlab/Retrieval/xzlarfg.h \
    matlab/SpectrumGeneration.h \
    matlab/coder_array.h \
    matlab/colon.h \
    matlab/convolution.h \
    matlab/div.h \
    matlab/drawNsMByDepth.h \
    matlab/drawSNRDepthByAlpha.h \
    matlab/drawSNRDepthByM.h \
    matlab/eml_mtimes_helper.h \
    matlab/eml_rand_mt19937ar.h \
    matlab/eml_rand_mt19937ar_stateful.h \
    matlab/rand.h \
    matlab/randn.h \
    matlab/rtGetInf.h \
    matlab/rtGetNaN.h \
    matlab/rt_nonfinite.h \
    matlab/rtwtypes.h \
    matlab/sum.h \
    matlab/tmwtypes.h \
    # model/CaculateScatteredPhotons.h \
    # model/FizeauInstrument.h \
    # model/PMTReceive.h \
    # model/SpectrumGeneration.h \
    # model/coder_array.h \
    # model/colon.h \
    model/constantmap.h \
    model/constantstorage.h \
    # model/convolution.h \
    model/fizeaudatagenerator.h \
    model/fizeauifgenerator.h \
    model/frequencedatagenerator.h \
    model/laserdatagenerator.h \
    model/pagedatagenerator.h \
    model/pmtreceptiondatagenerator.h \
    # model/rtGetInf.h \
    # model/rtGetNaN.h \
    # model/rt_nonfinite.h \
    # model/rtwtypes.h \
    model/spectrumdatagenerator.h \
    # model/tmwtypes.h \
    model/underwaterspectrumdatagenerator.h \
    module/filebrowser.h \
    module/mycombobox.h \
    module/mytracer.h \
    module/qcustomplot.h \
    utils/logger.h \
    utils/mymath.h \
    utils/readfiledata.h \
    view/dynamicview.h \
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


LIBS += -L$$PWD/lib/ -llibfftw3-3

msvc:QMAKE_CXXFLAGS += -execution-charset:utf-8
msvc:QMAKE_CXXFLAGS += -source-charset:utf-8
QMAKE_CXXFLAGS_WARN_ON += -wd4819
