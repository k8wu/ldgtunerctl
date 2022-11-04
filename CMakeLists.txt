cmake_minimum_required(VERSION 3.16)
project(ldgtunerctl VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

# Set up AUTOMOC and some sensible defaults for runtime execution
set(CMAKE_AUTOMOC ON)
include(GNUInstallDirs)

find_package(QT NAMES Qt5 Qt6 REQUIRED COMPONENTS Core)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Gui SerialPort)
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
    Qt::SerialPort
)

if((QT_VERSION_MAJOR GREATER 4))
    target_link_libraries(ldgtunerctl PRIVATE
        Qt::Widgets
    )
endif()

install(TARGETS ldgtunerctl
    BUNDLE DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

# report what kind of build this is
