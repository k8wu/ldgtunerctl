cmake_minimum_required(VERSION 3.16)
project(ldgtunerctl VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

# Set up AUTOMOC and some sensible defaults for runtime execution
set(CMAKE_AUTOMOC ON)
include(GNUInstallDirs)

find_package(QT NAMES Qt5 Qt6 REQUIRED COMPONENTS Core)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Gui)
find_package(Qt${QT_VERSION_MAJOR} OPTIONAL_COMPONENTS Widgets)

qt_add_executable(ldgtunerctl WIN32 MACOSX_BUNDLE
    src/appconfig.h
    src/commlink.cpp src/commlink.h
    src/config.cpp src/config.h
    src/main.cpp
    src/windowabout.cpp src/windowabout.h
    src/windowmain.cpp src/windowmain.h
    src/windowselectdevice.cpp src/windowselectdevice.h
)
target_link_libraries(ldgtunerctl PRIVATE
    Qt::Core
    Qt::Gui
)

if((QT_VERSION_MAJOR GREATER 4))
    target_link_libraries(ldgtunerctl PRIVATE
        Qt::Widgets
    )
endif()

# libserialport does not have a CMake file to help us, so we'll use pkg-config first
find_package(PkgConfig)
if(PKG_CONFIG_FOUND)
    pkg_check_modules(LIBSERIALPORT_PREFIX serialport)
endif()

# if that fails, then hope we have an override
if(NOT DEFINED LIBSERIALPORT_PREFIX)
    message(FATAL_ERROR "ERROR: libserialport not found! Please define LIBSERIALPORT_PREFIX with the directory that contains these files.")
else()
    message(STATUS "libserialport found at ${LIBSERIALPORT_PREFIX}")
    target_include_directories(ldgtunerctl PRIVATE ${LIBSERIALPORT_PREFIX}/include)
    target_link_directories(ldgtunerctl PRIVATE ${LIBSERIALPORT_PREFIX}/lib)
    target_link_libraries(ldgtunerctl PRIVATE serialport)
endif()

install(TARGETS ldgtunerctl
    BUNDLE DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

# report what kind of build this is
