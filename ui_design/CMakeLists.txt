cmake_minimum_required(VERSION 3.10)
project(ui_design)
set(CMAKE_CXX_STANDARD 17)

set(CMAKE_AUTOUIC_SEARCH_PATHS "${CMAKE_CURRENT_SOURCE_DIR}")

# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_PREFIX_PATH "d:/Qt/5.15.2/msvc2019_64") # Qt Kit Dir
# Instruct CMake to run moc automatically when needed
set(CMAKE_AUTOMOC ON)
# Create code from a list of Qt designer ui files
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt5 COMPONENTS Core Gui Widgets PrintSupport SerialPort REQUIRED)


# if(MSVC)
#     add_compile_options(/execution-charset:utf-8)
#     add_compile_options(/source-charset:utf-8)
#     add_compile_options(/wd4819)
# endif()
# Specify MSVC UTF-8 encoding   
add_compile_options("$<$<C_COMPILER_ID:MSVC>:/utf-8>")
add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")


# QT_QMAKE_EXECUTABLE:FILEPATH=%{Qt:qmakeExecutable}
# CMAKE_PREFIX_PATH:PATH=%{Qt:QT_INSTALL_PREFIX}
# CMAKE_C_COMPILER:FILEPATH=%{Compiler:Executable:C}
# CMAKE_CXX_COMPILER:FILEPATH=%{Compiler:Executable:Cxx}


# Populate a CMake variable with the .ui files
set(FORMS
    ${CMAKE_CURRENT_SOURCE_DIR}/mainwindow.ui
)

set(RESOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/data_csv.qrc
    ${CMAKE_CURRENT_SOURCE_DIR}/images.qrc
    ${CMAKE_CURRENT_SOURCE_DIR}/qss.qrc
)

# Run uic on .ui files
qt5_wrap_ui(UI_HEADERS ${FORMS})

# Run rcc on .qrc files
qt5_add_resources(RCC_SOURCES ${RESOURCES})

set(SOURCES
base/singleton.cpp 
base/taskrunner.cpp
controller/plotcontroller.cpp 
main.cpp 
mainwindow.cpp 
manager/buttongroupmanager.cpp 
manager/customplotmanager.cpp 
manager/inputdatalistmanager.cpp 
manager/lineeditgroupmanager.cpp 
manager/managerbase.cpp 
manager/menumanager.cpp 
manager/show1buttongroupmanager.cpp 
manager/texteditmanager.cpp 
manager/tracermanager.cpp 
matlab/AddNoiseNondB.cpp 
matlab/AddNoiseNondB_data.cpp 
matlab/AddNoiseNondB_initialize.cpp 
matlab/CaculateScatteredPhotons.cpp 
matlab/FizeauInstrument.cpp 
matlab/PMTReceive.cpp 
matlab/Retrieval/GenerationSpectrum.cpp 
matlab/Retrieval/PMTFMethods.cpp 
matlab/Retrieval/Retrieval_TS.cpp
matlab/Retrieval/RetrievalAlgorithm.cpp 
matlab/Retrieval/RetrievalAlgorithm_data.cpp 
matlab/Retrieval/RetrievalAlgorithm_initialize.cpp 
matlab/Retrieval/checkStoppingCriteria.cpp 
matlab/Retrieval/computeFiniteDifferences.cpp 
matlab/Retrieval/conv.cpp 
matlab/Retrieval/driver.cpp 
matlab/Retrieval/error_fitted_power.cpp 
matlab/Retrieval/factoryConstruct.cpp 
matlab/Retrieval/hasFiniteBounds.cpp 
matlab/Retrieval/linearLeastSquares.cpp 
matlab/Retrieval/minOrMax.cpp 
matlab/Retrieval/projectBox.cpp 
matlab/Retrieval/rtGetInf.cpp 
matlab/Retrieval/rtGetNaN.cpp 
matlab/Retrieval/rt_nonfinite.cpp 
# matlab/Retrieval/rand.cpp 
# matlab/Retrieval/randn.cpp 
matlab/Retrieval/xgemv.cpp 
matlab/Retrieval/xnrm2.cpp 
matlab/Retrieval/xzlarf.cpp 
matlab/Retrieval/xzlarfg.cpp 
matlab/SpectrumGeneration.cpp 
matlab/colon.cpp 
matlab/convolution.cpp 
matlab/div.cpp 
matlab/drawNsMByDepth.cpp 
matlab/drawSNRDepthByAlpha.cpp 
matlab/drawSNRDepthByM.cpp 
matlab/eml_mtimes_helper.cpp 
matlab/eml_rand_mt19937ar.cpp 
matlab/eml_rand_mt19937ar_stateful.cpp 
matlab/rand.cpp
matlab/randn.cpp 
matlab/sum.cpp 
model/constantmap.cpp 
model/constantstorage.cpp
model/constantstorage.cpp 
model/screencapturetask.cpp
# model/convolution.cpp \
model/fizeaudatagenerator.cpp 
model/fizeauifgenerator.cpp 
model/frequencedatagenerator.cpp 
model/laserdatagenerator.cpp 
model/pagedatagenerator.cpp 
model/pmtreceptiondatagenerator.cpp 
# model/rtGetInf.cpp \
# model/rtGetNaN.cpp \
# model/rt_nonfinite.cpp \
model/retrievalthread.cpp
model/spectrumdatagenerator.cpp 
model/underwaterspectrumdatagenerator.cpp 
module/filebrowser.cpp 
module/mycombobox.cpp 
module/mytracer.cpp 
module/qcustomplot.cpp 
module/hoverinfowidget.cpp
module/hoverbutton.cpp
utils/logger.cpp 
utils/mymath.cpp 
utils/readfiledata.cpp 
view/dynamicview.cpp
view/plotView.cpp
)

# include_directory(${SOURCES})
set(HEADERS
base/singleton.h 
base/taskrunner.h
controller/plotcontroller.h 
fftw3.h 
mainwindow.h 
manager/buttongroupmanager.h 
manager/customplotmanager.h 
manager/inputdatalistmanager.h 
manager/lineeditgroupmanager.h 
manager/managerbase.h 
manager/menumanager.h 
manager/show1buttongroupmanager.h 
manager/texteditmanager.h 
manager/tracermanager.h 
matlab/AddNoiseNondB.h 
matlab/AddNoiseNondB_data.h 
matlab/AddNoiseNondB_initialize.h 
matlab/CaculateScatteredPhotons.h 
matlab/FizeauInstrument.h 
matlab/PMTReceive.h 
matlab/Retrieval/GenerationSpectrum.h 
matlab/Retrieval/PMTFMethods.h 
matlab/Retrieval/Retrieval_TS.h
matlab/Retrieval/tmwtypes.h
matlab/Retrieval/RetrievalAlgorithm.h 
matlab/Retrieval/RetrievalAlgorithm_data.h 
matlab/Retrieval/RetrievalAlgorithm_initialize.h 
matlab/Retrieval/RetrievalAlgorithm_internal_types.h 
matlab/Retrieval/RetrievalAlgorithm_internal_types1.h 
matlab/Retrieval/RetrievalAlgorithm_internal_types11.h 
matlab/Retrieval/RetrievalAlgorithm_types.h 
matlab/Retrieval/anonymous_function.h 
matlab/Retrieval/anonymous_function1.h 
matlab/Retrieval/checkStoppingCriteria.h 
matlab/Retrieval/computeFiniteDifferences.h 
matlab/Retrieval/conv.h 
matlab/Retrieval/driver.h 
matlab/Retrieval/error_fitted_power.h 
matlab/Retrieval/factoryConstruct.h 
matlab/Retrieval/hasFiniteBounds.h 
matlab/Retrieval/linearLeastSquares.h 
matlab/Retrieval/minOrMax.h 
matlab/Retrieval/projectBox.h 
matlab/Retrieval/rtwtypes.h 
# matlab/Retrieval/rand.h 
# matlab/Retrieval/randn.h 
matlab/Retrieval/xgemv.h 
matlab/Retrieval/xnrm2.h 
matlab/Retrieval/xzlarf.h 
matlab/Retrieval/xzlarfg.h 
matlab/SpectrumGeneration.h 
matlab/coder_array.h 
matlab/colon.h 
matlab/convolution.h 
matlab/div.h 
matlab/drawNsMByDepth.h 
matlab/drawSNRDepthByAlpha.h 
matlab/drawSNRDepthByM.h 
matlab/eml_mtimes_helper.h 
matlab/eml_rand_mt19937ar.h 
matlab/eml_rand_mt19937ar_stateful.h 
matlab/rand.h 
matlab/randn.h 
matlab/rtGetInf.h 
matlab/rtGetNaN.h 
matlab/rt_nonfinite.h 
matlab/rtwtypes.h 
matlab/sum.h 
matlab/tmwtypes.h 
# model/CaculateScatteredPhotons.h 
# model/FizeauInstrument.h 
# model/PMTReceive.h 
# model/SpectrumGeneration.h 
# model/coder_array.h 
# model/colon.h 
model/constantmap.h 
model/constantstorage.h 
model/screencapturetask.h
# model/convolution.h 
model/fizeaudatagenerator.h 
model/fizeauifgenerator.h 
model/frequencedatagenerator.h 
model/laserdatagenerator.h 
model/pagedatagenerator.h 
model/pmtreceptiondatagenerator.h 
# model/rtGetInf.h 
# model/rtGetNaN.h 
# model/rt_nonfinite.h 
# model/rtwtypes.h 
model/retrievalthread.h
model/spectrumdatagenerator.h 
# model/tmwtypes.h 
model/underwaterspectrumdatagenerator.h 
module/filebrowser.h 
module/hoverinfowidget.h
module/hoverbutton.cpp
module/mycombobox.h 
module/mytracer.h 
module/qcustomplot.h 
utils/logger.h 
utils/mymath.h 
utils/readfiledata.h 
view/dynamicview.h 
view/plotView.h
)



# Add the executable (including the processed files)
add_executable(ui_design ${UI_HEADERS} ${RCC_SOURCES} ${SOURCES} ${HEADERS} )

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/module)
target_include_directories(ui_design PRIVATE module/)
target_include_directories(ui_design PRIVATE manager/)
target_include_directories(ui_design PRIVATE controller/)
target_include_directories(ui_design PRIVATE model/)
target_include_directories(ui_design PRIVATE view/)
target_include_directories(ui_design PRIVATE base/)
target_include_directories(ui_design PRIVATE utils/)
target_include_directories(ui_design PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

if(MSVC)
target_compile_options(ui_design PUBLIC "/ZI")
target_link_options(ui_design PUBLIC "/INCREMENTAL")
endif()

# LIBS += -L$$PWD/lib/ -llibfftw3-3.lib

# Link the executable to the necessary libraries
target_link_libraries(ui_design Qt5::Core Qt5::Widgets Qt5::PrintSupport Qt5::SerialPort)

link_directories(${CMAKE_CURRENT_SOURCE_DIR}/lib)   # lib文件要放到build目录下，dll文件要放到Debug目录下
target_link_libraries(ui_design libfftw3-3)
