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
    controller/plotcontroller.cpp
    main.cpp
    mainwindow.cpp
    manager/buttongroupmanager.cpp
    manager/customplotmanager.cpp
    manager/inputdatalistmanager.cpp
    manager/managerbase.cpp
    manager/menuManager.cpp
    manager/lineeditgroupmanager.cpp
    manager/show1buttongroupmanager.cpp
    manager/texteditmanager.cpp
    manager/tracermanager.cpp
    matlab/SpectrumGeneration.cpp
    matlab/convolution.cpp
    matlab/CaculateScatteredPhotons.cpp
    matlab/FizeauInstrument.cpp
    matlab/colon.cpp
    matlab/rt_nonfinite.cpp
    matlab/rtGetInf.cpp
    matlab/rtGetNaN.cpp
    matlab/PMTReceive.cpp
    matlab/sum.cpp
    matlab/rand.cpp 
    matlab/randn.cpp 
    matlab/AddNoiseNondB.cpp
    matlab/AddNoiseNondB_data.cpp
    matlab/AddNoiseNondB_initialize.cpp
    matlab/drawSNRDepthByM.cpp
    matlab/div.cpp
    matlab/eml_rand_mt19937ar.cpp 
    matlab/eml_rand_mt19937ar_stateful.cpp 
    matlab/drawSNRDepthByAlpha.cpp 
    model/constantmap.cpp
    model/constantstorage.cpp
    model/fizeaudatagenerator.cpp
    model/fizeauifgenerator.cpp
    model/frequencedatagenerator.cpp
    model/laserdatagenerator.cpp
    model/pagedatagenerator.cpp
    model/spectrumdatagenerator.cpp
    model/underwaterspectrumdatagenerator.cpp
    model/PMTReceptionDataGenerator.cpp
    module/filebrowser.cpp
    module/mycombobox.cpp
    module/mytracer.cpp
    module/qcustomplot.cpp
    utils/logger.cpp
    utils/myMath.cpp
    utils/readfiledata.cpp
    view/dynamicview.cpp
    view/plotView.cpp
)

# include_directory(${SOURCES})
set(HEADERS
    base/singleton.h
    controller/plotcontroller.h
    mainwindow.h
    manager/buttongroupmanager.h
    manager/customplotmanager.h
    manager/inputdatalistmanager.h
    manager/managerbase.h
    manager/menuManager.h
    manager/show1buttongroupmanager.h
    manager/lineeditgroupmanager.h
    manager/texteditmanager.h
    manager/tracermanager.h
    matlab/tmwtypes.h
    matlab/SpectrumGeneration.h
    matlab/coder_array.h
    matlab/rtwtypes.h
    matlab/convolution.h
    matlab/CaculateScatteredPhotons.h
    matlab/FizeauInstrument.h
    matlab/colon.h
    matlab/rt_nonfinite.h
    matlab/rtGetInf.h
    matlab/rtGetNaN.h
    matlab/PMTReceive.h
    matlab/eml_rand_mt19937ar.h
    matlab/eml_rand_mt19937ar_stateful.h 
    matlab/sum.h
    matlab/rand.h
    matlab/randn.h
    matlab/AddNoiseNondB.h
    matlab/AddNoiseNondB_data.h
    matlab/AddNoiseNondB_initialize.h
    matlab/drawSNRDepthByM.h 
    matlab/div.h
    matlab/drawSNRDepthByAlpha.h
    model/constantmap.h
    model/constantstorage.h
    model/fizeaudatagenerator.h
    model/frequencedatagenerator.h
    model/laserdatagenerator.h
    model/pagedatagenerator.h
    model/spectrumdatagenerator.h
    model/underwaterspectrumdatagenerator.h
    model/PMTReceptionDataGenerator.h
    model/fizeauifgenerator.h
    module/filebrowser.h
    module/mycombobox.h
    module/mytracer.h
    module/qcustomplot.h
    utils/logger.h
    utils/myMath.h
    utils/readfiledata.h
    view/dynamicview.h
    view/plotView.h
    fftw3.h
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
