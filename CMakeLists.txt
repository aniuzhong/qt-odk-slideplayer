cmake_minimum_required(VERSION 3.5)

project(qt-odk-slideplayer VERSION 0.1 LANGUAGES CXX)

set(SDK_INCLUDE_DIR  "/home/aniu/libreoffice/instdir/sdk/include"  CACHE STRING  "OO_SDK_HOME/include")
set(SDK_LIBRARY_DIR  "/home/aniu/libreoffice/instdir/sdk/lib"      CACHE STRING  "OO_SDK_HOME/lib")
set(SDK_BINARY_DIR   "/home/aniu/libreoffice/instdir/sdk/bin"      CACHE STRING  "OO_SDK_HOME/bin")
set(SDK_PROGRAM_DIR  "/home/aniu/libreoffice/instdir/program"      CACHE STRING  "OO_SDK_URE_LIB_DIR")
set(SDK_LIBRARIES    "uno_sal" "uno_salhelpergcc3" "uno_purpenvhelpergcc3" "uno_cppu" "uno_cppuhelpergcc3")

execute_process(COMMAND "${SDK_BINARY_DIR}/cppumaker" -Gc -O${PROJECT_SOURCE_DIR}/include "${SDK_PROGRAM_DIR}/types.rdb" "${SDK_PROGRAM_DIR}/types/offapi.rdb")

set(CMAKE_CXX_FLAGS            "${CMAKE_CXX_FLAGS} -Wall -Wextra -pedantic -fvisibility=hidden -fpic")
set(CMAKE_SHARED_LINKER_FLAGS  "-Wl,--allow-shlib-undefined -Wl,-export-dynamic -Wl,-z,defs -Wl,--no-whole-archive")
set(CMAKE_EXE_LINKER_FLAGS     "-Wl,--allow-shlib-undefined -Wl,-export-dynamic -Wl,-z,defs -Wl,--no-whole-archive")

add_compile_definitions(UNX)
add_compile_definitions(DGCC)
add_compile_definitions(LINUX)
add_compile_definitions(DCPPU_ENV=gcc3)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

set(PROJECT_SOURCES
        main.cpp
        mainwindow.cpp
        mainwindow.h
        mainwindow.ui
        officeconnection.h
        officeconnection.cpp
        slideshowcontroller.h
        slideshowcontroller.cpp
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(qt-odk-slideplayer
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET qt-odk-slideplayer APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(qt-odk-slideplayer SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(qt-odk-slideplayer
            ${PROJECT_SOURCES}
        )
    endif()
endif()

target_link_libraries(qt-odk-slideplayer PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)
target_link_libraries(qt-odk-slideplayer PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)
target_link_directories(qt-odk-slideplayer PRIVATE ${SDK_LIBRARY_DIR})
target_include_directories(qt-odk-slideplayer PRIVATE ${SDK_INCLUDE_DIR})
target_include_directories(qt-odk-slideplayer PRIVATE ${PROJECT_SOURCE_DIR}/include)
target_link_libraries(qt-odk-slideplayer PRIVATE ${SDK_LIBRARIES})

set_target_properties(qt-odk-slideplayer PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

install(TARGETS qt-odk-slideplayer
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(qt-odk-slideplayer)
endif()
